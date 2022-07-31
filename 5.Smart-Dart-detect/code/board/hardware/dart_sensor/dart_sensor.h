#ifndef __DART_SENSOR_H
#define __DART_SENSOR_H
#include <stdint.h>
#include <string.h>
/*
ppm=mg/m3��B/M 
B����[273/(273+T)]*��Ba/101325������׼״̬�������Ħ�������0��ʱ,B=22.4; 25��ʱ=24.46����
M���������ʵķ���������ȩHCOH M=30��
1 ppm = mg/m3��22.4/30= mg/m3��0.74666 
1 ppm = 0.74666 mg/m3
*/

typedef struct
{
	/*����Ũ��*/
	double gas_density ;	//ppm
	/*������*/
	double full_range ;		
}Dart_Sensor ;


Dart_Sensor Get_Dart_Sensor_Density(uint8_t *Data);
#endif //__DART_SENSOR_H

