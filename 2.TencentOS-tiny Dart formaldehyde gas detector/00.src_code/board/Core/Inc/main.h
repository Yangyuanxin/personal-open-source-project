/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tos_k.h"
#include "tos_at.h"
#include "esp8266.h"
#include "cmsis_os.h"
#include "tos_at_socket.h"

#include "rtc.h"
#include "key.h"
#include "para.h"
#include "filesystem.h"

#include "cmd.h"
#include "menu.h"
#include "menu_ui.h"

#include "logic.h"
#include "log_ui.h"
#include "main_ui.h"
#include "conf_ui.h"
#include "para_ui.h"
#include "recovery_ui.h"
#include "datetime_ui.h"
#include "password_ui.h"
#include "version_info_ui.h"

#include "app_init.h"
#include "dart_sensor.h"
#include "lcd_plot_display.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define MQ2_Sensor_Pin GPIO_PIN_2
#define MQ2_Sensor_GPIO_Port GPIOC
#define SF1_LED_Pin GPIO_PIN_0
#define SF1_LED_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOB
#define LCD_PWR_Pin GPIO_PIN_15
#define LCD_PWR_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
