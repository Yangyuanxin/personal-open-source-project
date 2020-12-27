/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
CMDSTR_PARSE cmd_parse_typedef ;
Sensor_HandleTypeDef dart_sensor_handler ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t esp_8266_data;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern SD_HandleTypeDef hsd1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();

    /* USER CODE BEGIN SysTick_IRQn 1 */
    if(tos_knl_is_running())
    {
        tos_knl_irq_enter();
        tos_tick_handler();
        tos_knl_irq_leave();
    }

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

    /* USER CODE END DMA1_Channel3_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart3_rx);
    /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

    /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    tos_knl_irq_enter();
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    tos_knl_irq_leave();
    /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
    /* USER CODE BEGIN USART3_IRQn 0 */
    tos_knl_irq_enter();
    extern k_msg_q_t sensor_data_msg;

    if(RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);

        /*如果内核启动了，发生中断接收才会调用信号量接口*/
        if (!tos_knl_is_running())
            return;
        else
            tos_msg_q_post(&sensor_data_msg, dart_sensor_handler.SensorU3Buffer);
    }

    /* USER CODE END USART3_IRQn 0 */
    HAL_UART_IRQHandler(&huart3);
    /* USER CODE BEGIN USART3_IRQn 1 */
    tos_knl_irq_leave();
    /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles SDMMC1 global interrupt.
  */
void SDMMC1_IRQHandler(void)
{
    /* USER CODE BEGIN SDMMC1_IRQn 0 */

    /* USER CODE END SDMMC1_IRQn 0 */
    HAL_SD_IRQHandler(&hsd1);
    /* USER CODE BEGIN SDMMC1_IRQn 1 */

    /* USER CODE END SDMMC1_IRQn 1 */
}

/**
  * @brief This function handles LPUART1 global interrupt.
  */
void LPUART1_IRQHandler(void)
{
    /* USER CODE BEGIN LPUART1_IRQn 0 */

    /* USER CODE END LPUART1_IRQn 0 */
    HAL_UART_IRQHandler(&hlpuart1);
    /* USER CODE BEGIN LPUART1_IRQn 1 */

    /* USER CODE END LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    extern k_msg_q_t recv_data_msg;

    if (huart->Instance == LPUART1)
    {
        HAL_UART_Receive_IT(&hlpuart1, &esp_8266_data, 1);
        tos_at_uart_input_byte(esp_8266_data);
    }
    else if(huart->Instance == USART1)
    {
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&cmd_parse_typedef.Res, 1);

        if('\n' != cmd_parse_typedef.Res)
        {
            if(cmd_parse_typedef.rx_count < CMD_STR_SIZE - 1)
                cmd_parse_typedef.cmd_buffer_temp[cmd_parse_typedef.rx_count++] = cmd_parse_typedef.Res ;
            else
                cmd_parse_typedef.rx_count = 0 ;
        }
        else
        {
            //如果接收的是\n，则上一个接收的数据为'\r'结束
            if('\r' == cmd_parse_typedef.cmd_buffer_temp[cmd_parse_typedef.rx_count - 1])
            {
                //添加结束符
                cmd_parse_typedef.cmd_buffer_temp[cmd_parse_typedef.rx_count - 1] = 0x00 ;
                memcpy(cmd_parse_typedef.cmd_buffer, cmd_parse_typedef.cmd_buffer_temp, CMD_STR_SIZE);

                /*如果内核启动了，发生中断接收才会调用信号量接口*/
                if (!tos_knl_is_running())
                    return;
                else
                    tos_msg_q_post(&recv_data_msg, cmd_parse_typedef.cmd_buffer);
            }
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET)
    {
        memset(&cmd_parse_typedef, 0, sizeof(cmd_parse_typedef));
        __HAL_UART_CLEAR_OREFLAG(huart);
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&cmd_parse_typedef.Res, 1);
    }
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
