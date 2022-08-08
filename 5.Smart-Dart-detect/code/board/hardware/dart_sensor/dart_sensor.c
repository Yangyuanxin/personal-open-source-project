#include "dart_sensor.h"

/*甲醛传感器协议*/
typedef struct
{
	/*起始位*/
	uint8_t start_bit ;
	/*气体名称*/
	uint8_t gas_name  ;
	/*单位*/
	uint8_t unit ;
	/*小数位数*/
	uint8_t decimal_places ;
	/*气体浓度高位*/
	uint8_t gas_density_high ;
	/*气体浓度低位*/
	uint8_t gas_density_low ;
	/*满量程高位*/
	uint8_t full_range_high ;
	/*满量程低位*/
	uint8_t full_range_low ;
	/*校验值*/
	uint8_t checksum_value ;
}Dart_Sensor_Procol_TypeDef ;

/*甲醛传感器协议解析*/
Dart_Sensor_Procol_TypeDef Dart_Sensor_Data_Parse(uint8_t *Data)
{
	uint16_t temp = 0 ;
	uint16_t check_sum = 0 ;
	uint16_t check_sum_negate = 0 ;
	Dart_Sensor_Procol_TypeDef dart_sensor ;
	memcpy(&dart_sensor,Data,sizeof(Dart_Sensor_Procol_TypeDef));
	check_sum = dart_sensor.gas_name + dart_sensor.unit + 					\
	dart_sensor.gas_density_low + dart_sensor.gas_density_high + 		\
	dart_sensor.full_range_high + dart_sensor.full_range_low + dart_sensor.decimal_places ;
	check_sum_negate = ~check_sum ;
	temp = check_sum_negate + 1 ;
	if((temp & 0xff) != dart_sensor.checksum_value)	
	{
		memset(&dart_sensor,0,sizeof(Dart_Sensor_Procol_TypeDef));
		return dart_sensor ;
	}
	return dart_sensor ;
}


/*
	获取气体浓度
	Data:		传感器数据
	return: xxx ppm
*/
Dart_Sensor Get_Dart_Sensor_Density(uint8_t *Data)
{
	Dart_Sensor sensor ;
	Dart_Sensor_Procol_TypeDef dart_sensor ;
	dart_sensor = Dart_Sensor_Data_Parse(Data);
	sensor.gas_density = ((dart_sensor.gas_density_high << 8) + (dart_sensor.gas_density_low))/1000.0 ;
	sensor.full_range  = ((dart_sensor.full_range_high << 8)  + (dart_sensor.full_range_low))/1000.0 ; 
	return sensor ;
}


