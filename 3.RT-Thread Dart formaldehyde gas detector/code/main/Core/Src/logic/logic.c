#include "logic.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include "usart.h"
#include "dart_sensor.h"
#include "stm32l4xx_it.h"

Dart_Sensor sensor ;
Sensor *Formaldehyde_Sensor = NULL;
void get_formaldehyde_sensor_value(Sensor *sensor_handle);


/*找到数组最大*/
float find_max_value(const float *pData, uint32_t size)
{
    uint32_t i = 0;
    float value = 0;

    for(i = 0; i < size; i++)
    {
        if(pData[i] > value)
        {
            value = pData[i];
        }
    }

    return value;
}

/*找到数组最小*/
float find_min_value (const float *pData, uint32_t size)
{
    uint32_t i = 0;
    float value = pData[0];

    for(i = 0; i < size; i++)
    {
        if(pData[i] < value)
        {
            value = pData[i];
        }
    }

    return value;
}


/*提供访问传感器的接口*/
Sensor Formaldehyde_Sensor_interface =
{
    /*获取接口*/
    .get_Formaldehyde_Value = get_formaldehyde_sensor_value
};

/*注册传感器*/
int Sensor_Register(Sensor *sensor_handle)
{
    if(NULL == Formaldehyde_Sensor)
    {
        Formaldehyde_Sensor = sensor_handle ;

        if(NULL == Formaldehyde_Sensor)
            return -1 ;
        else
        {
            MX_DMA_Init();
            MX_USART3_UART_Init();
            //开启DMA接收
            HAL_UART_DMAStop(&huart3);
            memset(dart_sensor_handler.SensorU3Buffer, 0, SENSOR_U3_BUFFER_SIZE);
            HAL_UART_Receive_DMA(&huart3, (uint8_t*)dart_sensor_handler.SensorU3Buffer, SENSOR_U3_BUFFER_SIZE);

            for(int i = 0 ; i < DATA_SIZE ; i++)
                plot_handler.rel_data_data[i] = 0 ;
        }
    }

    return 0 ;
}


/*获取甲醛值*/

void get_formaldehyde_sensor_value(Sensor *sensor_handle)
{


}



