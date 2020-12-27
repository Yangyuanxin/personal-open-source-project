#ifndef __DART_SENSOR_H
#define __DART_SENSOR_H
#include <stdint.h>
#include <string.h>
/*
1ppm = 1000ppb
1ppb = 1000ppt

ppm = mg/L(����/��)
ppb = ug/L(΢��/��)
ppt = ng/L(�ɿ�/��)
*/

typedef struct
{
	/*����Ũ��*/
	float gas_density ;	//ppm
	/*������*/
	float full_range ;		
}Dart_Sensor ;


Dart_Sensor Get_Dart_Sensor_Density(uint8_t *Data);
#endif //__DART_SENSOR_H

