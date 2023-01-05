/*
 * lwip_mqtt.h
 *
 *  Created on: Oct 30, 2022
 *      Author: dawid
 */

#ifndef LWIP_MQTT_H
#define LWIP_MQTT_H
#include "lwip/apps/mqtt.h"
void example_do_connect(mqtt_client_t *client,void *arg);
void example_publish(mqtt_client_t *client, const char *topic,void *arg );
void example_subscribe(mqtt_client_t *client,char* topic);
#endif
