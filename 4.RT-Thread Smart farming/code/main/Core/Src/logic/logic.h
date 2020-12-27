#ifndef __LOGIC_H
#define __LOGIC_H
#include "main.h"

typedef struct
{
    __IO uint8_t module_init ;
    float temp ;
    float humidity ;
    uint16_t light_strength ;
} Sensor ;
extern Sensor E53_IA1_Sensor ;

/*找到数组最大*/
float find_max_value(const float *pData, uint32_t size);
/*找到数组最小*/
float find_min_value (const float *pData, uint32_t size);

/*传感器注册*/
void Register_Sensor(void);
/*传感器执行*/
void Sensor_Logic_Running(void);
#endif //__LOGIC_H

