/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Yuanxin.Yang
 * Version            : V1.0.0
 * Date               : 2022/07/04
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include "app_main.h"
#include "SystemConfig.h"

#define DEFAULT_TASK_SIZE 2048
void StartMainTask(void *pdata);
osThreadDef(StartMainTask, osPriorityLow, 1, DEFAULT_TASK_SIZE);

#define MQTT_TASK_SIZE 2048
void StartNetworkTask(void *pdata);
osThreadDef(StartNetworkTask, osPriorityLow, 1, MQTT_TASK_SIZE);

ring_buffer Fifo;

/*MQTT任务处理*/
void StartNetworkTask(void *pdata)
{
	(void) pdata;
	printf("StartNetworkTask...\n");
	MqttTask();
}

void StartMainTask(void *arg)
{
	(void) arg;
	uint8_t data;
	osSchedLock();
	if (NULL == osThreadCreate(osThread(StartNetworkTask), NULL))
	{
		printf("osThreadCreate Mqtt Fail!\n");
		return;
	}
	osSchedUnLock();
	LCD_ShowPicture(0, 190, 240, 50, gImage_icon_for_tencentos_tiny);
	for (;;)
	{
		if (0 == ring_buffer_read(&data, &Fifo))
		{
			printf("cmd:%x\n",data);
		}
		osDelay(5);
	}
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	osStatus Status;
	Delay_Init();
	Delay_Ms(1000);
	Led_Init();
	Key_Init();
	LCD_Init();
	LCD_BLK_Clr();
	UHandUartInit(9600);
	UartDebugInit(115200);
	LCD_Fill(0, 0, 240, 240, BLACK);
	LCD_BLK_Set();
	ring_buffer_init(&Fifo);
	//关闭串口接收中断
	USART_ITConfig(UART7, USART_IT_RXNE, DISABLE);
	Status = osKernelInitialize();
	if (osOK != Status)
	{
		printf("osKernelInitialize Fail:%d!\n", Status);
		return -1;
	}
	if (NULL == osThreadCreate(osThread(StartMainTask), NULL))
	{
		printf("osThreadCreate StartMainTask Fail!\n");
		return -1;
	}
	Status = osKernelStart();
	if (osOK != Status)
	{
		printf("osKernelStart Fail:%d!\n", Status);
		return -1;
	}
	while(1)
	{
		asm("nop");
	}
}
