/*
 * lwip_mqtt.c
 *
 *  Created on: Oct 30, 2022
 *      Author: dawid
 */

#include "lwip/apps/mqtt.h"
#include <string.h>
#include "stm32f7xx_hal.h"
#include "tim.h"
#include "lwip_mqtt.h"
#include <stdint.h>
extern UART_HandleTypeDef huart3;
char buffer[1000];
extern char client_id[50];
//extern char sub_on_conect[100];
/* The idea is to demultiplex topic and create some reference to be used in data callbacks
   Example here uses a global variable, better would be to use a member in arg
   If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
   the topic string and use it in mqtt_incoming_data_cb
*/
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  sprintf(buffer,"Incoming publish at topic %s with total length %u\n\r", topic, (unsigned int)tot_len);
  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
  if(strcmp(topic, "Control") == 0)
  {
	  inpub_id=1;
  }

}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	  sprintf(buffer,"Incoming publish payload with length %d, flags %u\n\r", len, (unsigned int)flags);
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
	  if(inpub_id == 1) {
		  float u;
		  //sscanf (data,"{\"u\":%d}",&u);
		  sscanf ((char*)data,"%f",&u);
		  sprintf(buffer,"%f\n\r", u);
		  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
		  uint32_t compare=u/100.0*__HAL_TIM_GET_AUTORELOAD(&htim2);
		  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,compare);
		  inpub_id=0;
	  }
}

static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  sprintf(buffer,"Subscribe result: %d\n\r", result);
  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);

}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  char * topic = arg;
  err_t err;
  if(status == MQTT_CONNECT_ACCEPTED) {
    sprintf(buffer,"mqtt_connection_cb: Successfully connected\n\r");
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

    /* Subscribe to a topic named "placa" with QoS level 0, call mqtt_sub_request_cb with result */
    if(strcmp(topic,"")!=0)
    {
    err = mqtt_subscribe(client, topic, 0, mqtt_sub_request_cb, arg);
    if(err !=ERR_OK)
    {
    printf(buffer,"mqtt_connect return %d\n\r", err);
    HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
    }
    }
  } else {
    sprintf(buffer,"mqtt_connection_cb: Disconnected, reason: %d\n\r", status);
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);

    /* Its more nice to be connected, so try to reconnect */
    example_do_connect(client,topic);
  }

}
void example_subscribe(mqtt_client_t *client,char* topic)
{
	err_t err;
	if(mqtt_client_is_connected(client))
	{
		err = mqtt_subscribe(client, topic, 0, mqtt_sub_request_cb, 0);
		if(err != ERR_OK) {
		      sprintf(buffer,"mqtt_subscribe return: %d\n", err);
			  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
		    }
	}
}

void example_do_connect(mqtt_client_t *client,void *arg)
{
  struct mqtt_connect_client_info_t ci;
  err_t err;

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id = client_id;
  ci.keep_alive = 5;
  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting
     to establish a connection with the server.
     For now MQTT version 3.1.1 is always used */
  ip_addr_t mqttServerIP;
  IP4_ADDR(&mqttServerIP, 192, 168, 0, 23);
//err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, 0, &ci);
  err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, arg, &ci);

  /* For now just print the result code if something goes wrong */
  if(err != ERR_OK) {
    sprintf(buffer,"mqtt_connect return %d\n\r", err);
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
  }
}

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    sprintf(buffer,"Publish result: %d\n", result);
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
  }
}
void example_publish(mqtt_client_t *client, const char *topic,void *arg )
{
  const char *pub_payload= arg;
  err_t err;
  u8_t qos = 2;
  u8_t retain = 0;
  err = mqtt_publish(client, topic, pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    sprintf(buffer,"Publish err: %d\n\r", err);
	  HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen(buffer),1000);
  }
}

