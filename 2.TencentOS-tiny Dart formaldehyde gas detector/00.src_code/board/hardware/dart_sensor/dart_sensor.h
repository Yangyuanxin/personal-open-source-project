#ifndef __DART_SENSOR_H
#define __DART_SENSOR_H
#include <stdint.h>
#include <string.h>
/*
1ppm = 1000ppb
1ppb = 1000ppt

ppm = mg/L(毫克/升)
ppb = ug/L(微克/升)
ppt = ng/L(纳克/升)
*/

typedef struct
{
	/*气体浓度*/
	float gas_density ;	//ppm
	/*满量程*/
	float full_range ;		
}Dart_Sensor ;


Dart_Sensor Get_Dart_Sensor_Density(uint8_t *Data);
#endif //__DART_SENSOR_H

