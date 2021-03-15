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
#include "fatfs.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/*����SD��*/
int Mount_SD(void)
{
    /*����SD��*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    return 0 ;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 0
uint8_t framebuffer[57600] = {1};
void sd_show_picture_bin(const char *path)
{
    UINT br;
    FIL fil ;
    FRESULT res;
    res = f_open(&fil, path,FA_READ);
    res = f_read(&fil, framebuffer, 57600, &br);
    LCD_Show_Image(0, 0, 240, 120, (uint8_t *)framebuffer);
		LCD_Show_Image(0, 120, 240, 120, (uint8_t *)framebuffer);
    f_close(&fil);
}
#endif

/*һ��RGB565ͼƬռ�õĻ����С*/
#define PIC_SIZE 32768
/*��LCD��X����*/
#define DISPLAY_START_X 0
/*��LCD��Y����*/
#define DISPLAY_START_Y 0
/*LCD��ȷֱ���*/
#define OLED_WIDTH   128
/*LCD�߶ȷֱ���*/
#define OLED_HEIGHT  128
/*������ʾ�Ļ�����*/
uint8_t framebuffer[PIC_SIZE] = {1};
int sd_show_picture_bin(const char *path)
{
    UINT br;
    FIL fil ;
    int offset = 0 ;
    int file_size ;
    FRESULT res ;
    /*1.�򿪴�������Fatfs�ļ�·��: path*/
    res = f_open(&fil, path, FA_READ);

    if(FR_OK != res)
        return -1;

    /*2.��������ļ��Ĵ�С*/
    file_size = f_size(&fil);
    /*3.��ʼ��ƫ�Ʊ���Ϊ0*/
    offset = 0 ;

    /*4.����bin�ļ���һ������������ͼƬ��Ȼ�󲻶ϵĸ�LCD������ʾ*/
    for(int i = 0 ; i < file_size / PIC_SIZE ; i++)
    {
        /*5.��ȡһ��ͼƬ,һ��ͼƬ�Ĵ�С��PIC_SIZE��
        		����ȡ������ͼƬ��ŵ�������framebuffer������*/
        res = f_read(&fil, framebuffer, PIC_SIZE, &br);

        if(FR_OK != res)
            return -2;

        /*6.���������鸳ֵ����ʾ�ӿڣ���ʾͼƬ*/
        LCD_Show_Image(DISPLAY_START_X, DISPLAY_START_Y, OLED_WIDTH, OLED_HEIGHT, (uint8_t *)framebuffer);
        /*7.��ƫ�������PIC_SIZE���͵�����һ��ͼƬ*/
        offset += PIC_SIZE;
        res = f_lseek(&fil, offset);

        if(FR_OK != res)
            return -3;
    }

    /*8.�ر��ļ�������*/
    res = f_close(&fil);

    if(FR_OK != res)
        return -4;

    return 0 ;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    int ret = -1 ;
		int index = 0 ;
		char buf[20] = {0};
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_RTC_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    /*LCD��ʼ����û������*/
    LCD_Init();
    /*����SD��*/
    ret = Mount_SD();
    if(ret != 0)
    {
        printf("SD Card mount ERROR\r\n");
        /*�������ʧ������������ϵĵ��Ե�*/
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        return -1 ;
    }

    printf("SD�����سɹ�!\n");
    LCD_DisplayOn();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
				sd_show_picture_bin("0:/boy.bin");
    }

    /* USER CODE END 3 */
}

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
                                         | RCC_PERIPHCLK_SDMMC1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;

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
