#include "key.h"
#include "bear.h"


void PowerOn(void)
{
    static uint32_t power_press_count = 0;
    HAL_Delay(500);
    while(1)
    {
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
        {
            power_press_count ++;
						
            if(power_press_count >= 100)
            {
								//开指示灯
								HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_SET);
                while(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
                {
                    HAL_Delay(10);
                }
                break;
            }
        }
        else
        {
            power_press_count = 0;
        }

        HAL_Delay(10);
    }
}


void GetKey(__IO uint8_t *pKeyValue)
{
    Key_F1(pKeyValue);
    Key_F2(pKeyValue);
}


void Key_F1(__IO uint8_t *pKeyValue)
{
    static int key_time = 0;
    static char key_status = 0;

    switch(key_status)
    {
        case 0:     //检测.消抖
            if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
                key_status++;

            break;

        case 1:     //确定
            if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
            {
                key_status++;
            }
            else key_status = 0;

            break;

        //计时
        case 2:
            if((key_time < 200) && HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
                key_time++;
            else
            {
                if(key_time >= KEY_PRESS_LONG_TIME)
                    *pKeyValue = KEY_F2 ;
                else
                    *pKeyValue = KEY_F1 ;

                key_time = 0;
                key_status++;
            }

            break;

        case 3:     //等待松开
            if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 1)
                key_status = 0;

            break;

        default:
            break;
    }
}


void Key_F2(__IO uint8_t *pKeyValue)
{
    static int key_time = 0;
    static char key_status = 0;

    switch(key_status)
    {
        case 0:     //检测.消抖
            if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0)
                key_status++;

            break;

        case 1:     //确定
            if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0)
            {
                key_status++;
            }
            else key_status = 0;

            break;

        //计时
        case 2:
            if((key_time < 200) && HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0)
                key_time++;
            else
            {
                if(key_time >= KEY_PRESS_LONG_TIME)
                    *pKeyValue = KEY_F4 ;
                else
                    *pKeyValue = KEY_F3 ;

                key_time = 0;
                key_status++;
            }

            break;

        case 3:     //等待松开
            if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 1)
                key_status = 0;

            break;

        default:
            break;
    }
}

