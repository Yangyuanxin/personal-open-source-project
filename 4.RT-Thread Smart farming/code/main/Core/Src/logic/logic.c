#include "logic.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include "usart.h"
#include "stm32l4xx_it.h"
Sensor E53_IA1_Sensor ;

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

/*传感器注册*/
void Register_Sensor(void)
{
    SHT30_Reset();

    if(SHT30_Init() != HAL_OK)
    {
        printf("sht30 init fail.\n");
        return ;
    }
}
/*传感器逻辑*/
void Sensor_Logic_Running(void)
{
    static uint8_t status = 0 ;
    uint8_t recv_dat[6] = {0};
    float temperature = 0.0;
    float humidity = 0.0;
    uint8_t dat[2] = {0};

    switch(status)
    {
        case 0:
            if(HAL_OK == BH1750_Send_Cmd(ONCE_H_MODE))
                status = 1 ;
            else
                status = 0 ;

            break ;

        case 1:
            if(HAL_OK == BH1750_Read_Dat(dat))
            {
                E53_IA1_Sensor.light_strength = BH1750_Dat_To_Lux(dat);

                if(E53_IA1_Sensor.module_init == 1)
                {
                    if(E53_IA1_Sensor.light_strength < User_Memory_Para.alarm_threshold[LIGHT_SENSIVITY_BASE+User_Memory_Para.sensivity])
                    {
                        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                            display_light_strength_value(E53_IA1_Sensor.light_strength, YELLOW, 1);
                    }
                    else
                    {
                        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                            display_light_strength_value(E53_IA1_Sensor.light_strength, GREEN, 1);
                    }
                }
            }

            status = 2 ;
            break ;

        case 2:
            if(SHT30_Read_Dat(recv_dat) == HAL_OK)
            {
                if(SHT30_Dat_To_Float(recv_dat, &temperature, &humidity) == 0)
                {
                    E53_IA1_Sensor.temp = temperature ;
                    E53_IA1_Sensor.humidity = humidity ;

                    if(E53_IA1_Sensor.module_init == 1)
                    {
                        if(E53_IA1_Sensor.temp > User_Memory_Para.alarm_threshold[TEMP_SENSIVITY_BASE+User_Memory_Para.sensivity])
                        {
                            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                                display_temp_value(E53_IA1_Sensor.temp, RED, 1);
                        }
                        else
                        {
                            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                                display_temp_value(E53_IA1_Sensor.temp, GREEN, 1);
                        }

                        if(E53_IA1_Sensor.humidity > User_Memory_Para.alarm_threshold[HUM_SENSIVITY_BASE+User_Memory_Para.sensivity])
                        {
                            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                                display_humidity_value(E53_IA1_Sensor.humidity, RED, 1);
                        }
                        else
                        {
                            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                                display_humidity_value(E53_IA1_Sensor.humidity, GREEN, 1);
                        }
                    }
                }
            }

            status = 0 ;
            break ;

        default:
            break ;
    }
}

