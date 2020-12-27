#include "logic.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include "usart.h"
#include "dart_sensor.h"
#include "stm32l4xx_it.h"

Dart_Sensor sensor ;
extern k_msg_q_t sensor_data_msg;
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
		uint8_t i = 0 ;
		k_err_t err;
    void *msg_received;
    uint8_t sensor_recv_buf[SENSOR_U3_BUFFER_SIZE] = {0};
		err = tos_msg_q_pend(&sensor_data_msg, &msg_received, TOS_TIME_FOREVER);
		if (err == K_ERR_NONE)
		{
			memcpy(sensor_recv_buf, msg_received, SENSOR_U3_BUFFER_SIZE);
			/*判断包头数据是否正确*/
			if(sensor_recv_buf[0] == 0xFF && sensor_recv_buf[1] == 0x17)
			{
					sensor = Get_Dart_Sensor_Density(sensor_recv_buf);
					/*更新数据到队列*/
					for(i = 0 ; i <= DATA_SIZE - 2 ; i++)
							plot_handler.rel_data_data[i] = plot_handler.rel_data_data[i + 1];
					plot_handler.rel_data_data[DATA_SIZE - 1] = sensor.gas_density ;
					Formaldehyde_Sensor_interface.Formaldehyde_Value = plot_handler.rel_data_data[DATA_SIZE - 1] ;
					Formaldehyde_Sensor_interface.Formaldehyde_Max_Value = find_max_value(plot_handler.rel_data_data, DATA_SIZE);
					Formaldehyde_Sensor_interface.Formaldehyde_Min_Value = find_min_value(plot_handler.rel_data_data, DATA_SIZE);
			}
			memset(dart_sensor_handler.SensorU3Buffer, 0, SENSOR_U3_BUFFER_SIZE);
			HAL_UART_Receive_DMA(&huart3, (uint8_t*)dart_sensor_handler.SensorU3Buffer, SENSOR_U3_BUFFER_SIZE);
		}
}



