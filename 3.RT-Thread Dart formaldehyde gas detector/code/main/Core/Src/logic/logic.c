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


/*�ҵ��������*/
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

/*�ҵ�������С*/
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


/*�ṩ���ʴ������Ľӿ�*/
Sensor Formaldehyde_Sensor_interface =
{
    /*��ȡ�ӿ�*/
    .get_Formaldehyde_Value = get_formaldehyde_sensor_value
};

/*ע�ᴫ����*/
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
            //����DMA����
            HAL_UART_DMAStop(&huart3);
            memset(dart_sensor_handler.SensorU3Buffer, 0, SENSOR_U3_BUFFER_SIZE);
            HAL_UART_Receive_DMA(&huart3, (uint8_t*)dart_sensor_handler.SensorU3Buffer, SENSOR_U3_BUFFER_SIZE);

            for(int i = 0 ; i < DATA_SIZE ; i++)
                plot_handler.rel_data_data[i] = 0 ;
        }
    }

    return 0 ;
}


/*��ȡ��ȩֵ*/

void get_formaldehyde_sensor_value(Sensor *sensor_handle)
{


}



