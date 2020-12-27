#ifndef __LOGIC_H
#define __LOGIC_H
#include "main.h"

typedef struct Sensor
{
    /*��ȩֵ*/
    float Formaldehyde_Value;
		float Formaldehyde_Max_Value;
		float Formaldehyde_Min_Value;
    void (*get_Formaldehyde_Value)(struct Sensor *sensor_handle);
} Sensor ;
extern Sensor *Formaldehyde_Sensor ;
extern Sensor Formaldehyde_Sensor_interface ;
/*ע�ᴫ����*/
int Sensor_Register(Sensor *sensor_handle);

/*�ҵ��������*/
float find_max_value(const float *pData, uint32_t size);
/*�ҵ�������С*/
float find_min_value (const float *pData, uint32_t size);

#endif //__LOGIC_H

