/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "usart.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_init.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void HAL_Delay(uint32_t Delay)
{
    rt_thread_mdelay(Delay);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern LCD_Ascii_Show_Para datatime_display_para ;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int main(void)
{
    hardware_init();
    start_rt_thread_os();
    LCD_Fill_Para status_bar_fill_para = {0, 240, 0, 27, WHITE} ;
    lcd_model.lcd_driver->lcd_fill(status_bar_fill_para);
    display_runing_time_font(1);

    while(1)
    {
        Get_Date_Time();

        if(0 == plot_handler.plot_mode)
        {
            sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
                    DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
            lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);

            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 1);
        }

        rt_thread_mdelay(1000);
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USART1
                                         | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_LPUART1
                                         | RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_ADC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK | RCC_PLLSAI1_ADC1CLK;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
/**
 * This function will show the version of rt-thread rtos
 */
void reboot(void)
{
    rt_kprintf("\n \\ | /\n");
    rt_kprintf("- RT -     Thread Operating System\n");
    rt_kprintf(" / | \\     %d.%d.%d build %s\n",
               RT_VERSION, RT_SUBVERSION, RT_REVISION, __DATE__);
    rt_kprintf(" 2006 - 2019 Copyright by rt-thread team\n");
    rt_kprintf("RT-Thread Operating System Restart!\n");
    HAL_NVIC_SystemReset();
}
MSH_CMD_EXPORT(reboot, reset stm32 system);


extern int msh_help(int argc, char **argv);
void ls(int argc, char **argv)
{
    msh_help(argc, argv);
}
MSH_CMD_EXPORT(ls, help cmd);

void system_para(void)
{
    DEBUG("alarm:%d\r\n", User_Memory_Para.alarm);
    DEBUG("value:%d\n", User_Memory_Para.value);
    DEBUG("sensivity:%d\r\n", User_Memory_Para.sensivity);
    DEBUG("debug_flag:%d\r\n", User_Memory_Para.debug_flag);
    DEBUG("upload_flag:%d\r\n", User_Memory_Para.upload_flag);
    DEBUG("log_serial_number:%d\r\n", User_Memory_Para.detect_log_serial_number);

    for(int i = 0 ; i < 3 ; i++)
        DEBUG("sensivity:%d  value:%.3f\r\n", i, User_Memory_Para.alarm_threshold[i]);

    DEBUG("password:%d%d%d%d\r\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
          User_Memory_Para.password[2], User_Memory_Para.password[3]);
    DEBUG("wifi_name:%s\r\n", User_Memory_Para.wifi_name);
    DEBUG("wifi_passwd:%s\r\n", User_Memory_Para.wifi_password);
    DEBUG("server_addr:%s\r\n", User_Memory_Para.server_ip);
    DEBUG("server_port:%d\r\n", User_Memory_Para.server_port);
}
MSH_CMD_EXPORT(system_para, system_para);
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
