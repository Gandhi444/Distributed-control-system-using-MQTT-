/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BH1750.h"
#include "BMP280.h"
#include "lwip/apps/mqtt.h"
#include "lwip_mqtt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
BMP280_typedef BMP280;
mqtt_client_t *client;
char buf[1000];
char client_id[50];
char sub_on_connect[100];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* USER CODE END Variables */
/* Definitions for LwipTask */
osThreadId_t LwipTaskHandle;
const osThreadAttr_t LwipTask_attributes = {
  .name = "LwipTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Sensors */
osTimerId_t SensorsHandle;
const osTimerAttr_t Sensors_attributes = {
  .name = "Sensors"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartLwipTask(void *argument);
void SensorsCallback(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Sensors */
  SensorsHandle = osTimerNew(SensorsCallback, osTimerPeriodic, NULL, &Sensors_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LwipTask */
  LwipTaskHandle = osThreadNew(StartLwipTask, NULL, &LwipTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartLwipTask */
/**
  * @brief  Function implementing the LwipTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLwipTask */
void StartLwipTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartLwipTask */
//  client = mqtt_client_new();
//  sprintf(client_id, "Temp");
//  if (client != NULL) {
//  		example_do_connect(client, sub_on_connect);
//  	}
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartLwipTask */
}

/* SensorsCallback function */
void SensorsCallback(void *argument)
{
  /* USER CODE BEGIN SensorsCallback */
	BMP280ReadTemp(&BMP280);
	int len = sprintf(buf, "{\"temp\":%f} \n",BMP280.temp);
	if (mqtt_client_is_connected(client)) {
					example_publish(client, "Sensors", buf);
				}
  /* USER CODE END SensorsCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

