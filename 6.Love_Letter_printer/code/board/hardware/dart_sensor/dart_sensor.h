#ifndef __DART_SENSOR_H
#define __DART_SENSOR_H
#include <stdint.h>
#include <string.h>
/*
ppm=mg/m3×B/M 
B：·[273/(273+T)]*（Ba/101325）即标准状态下气体的摩尔体积（0度时,B=22.4; 25度时=24.46升）
M：被测物质的分子量（甲醛HCOH M=30）
1 ppm = mg/m3×22.4/30= mg/m3×0.74666 
1 ppm = 0.74666 mg/m3
*/

typedef struct
{
	/*气体浓度*/
	double gas_density ;	//ppm
	/*满量程*/
	double full_range ;		
}Dart_Sensor ;


Dart_Sensor Get_Dart_Sensor_Density(uint8_t *Data);
#endif //__DART_SENSOR_H

