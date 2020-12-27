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

/*�ҵ��������*/
float find_max_value(const float *pData, uint32_t size);
/*�ҵ�������С*/
float find_min_value (const float *pData, uint32_t size);

/*������ע��*/
void Register_Sensor(void);
/*������ִ��*/
void Sensor_Logic_Running(void);
#endif //__LOGIC_H

