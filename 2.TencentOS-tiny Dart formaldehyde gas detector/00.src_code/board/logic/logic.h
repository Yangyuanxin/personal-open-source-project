#ifndef __LOGIC_H
#define __LOGIC_H
#include "main.h"

typedef struct Sensor
{
    /*甲醛值*/
    float Formaldehyde_Value;
		float Formaldehyde_Max_Value;
		float Formaldehyde_Min_Value;
    void (*get_Formaldehyde_Value)(struct Sensor *sensor_handle);
} Sensor ;
extern Sensor *Formaldehyde_Sensor ;
extern Sensor Formaldehyde_Sensor_interface ;
/*注册传感器*/
int Sensor_Register(Sensor *sensor_handle);

/*找到数组最大*/
float find_max_value(const float *pData, uint32_t size);
/*找到数组最小*/
float find_min_value (const float *pData, uint32_t size);

#endif //__LOGIC_H

