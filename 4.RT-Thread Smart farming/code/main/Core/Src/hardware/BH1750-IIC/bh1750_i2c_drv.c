/**
 * @Copyright 			(c) 2019,mculover666 All rights reserved	
 * @filename  			bh1750_i2c_drv.h
 * @breif				Drive BH1750 based on iic1 commucation interface
 * @version
 *            			v1.0    ��ɻ�����������              mculover666    2019/7/15
 * @note            	��ֲ˵�����ǳ���Ҫ����	
 *						1. bh1750_i2c_drv.h�ļ��е�BH1750��д��ַ���ɣ�
 * 						2. ������������ҪSTM32CubeMX���ɵ�I2C��ʼ���ļ�i2c.h��i2c.c֧��;
 * 						3. ÿ��д��ָ��֮����Ҫ��ʱ�ٶ�ȡ���ݣ����ڵ��ڸ�ģʽ�Ĳ���ʱ�䣩��
 */

#include "bh1750_i2c_drv.h"
#include "i2c.h"

/**
 * @brief		��BH1750����һ��ָ��
 * @param		cmd ���� BH1750����ģʽָ���BH1750_MODE��ö�ٶ��壩
 * @retval		�ɹ�����HAL_OK
*/
uint8_t	BH1750_Send_Cmd(BH1750_MODE cmd)
{
		return HAL_I2C_Master_Transmit(&hi2c1, BH1750_ADDR_WRITE, (uint8_t*)&cmd, 1, 0xFFFF);
}
/**
 * @brief		��BH1750����һ�ι�ǿ����
 * @param		dat ���� �洢����ǿ�ȵĵ�ַ�������ֽ����飩
 * @retval		�ɹ� ���� ����HAL_OK
*/
uint8_t BH1750_Read_Dat(uint8_t* dat)
{
	return HAL_I2C_Master_Receive(&hi2c1, BH1750_ADDR_READ, dat, 2, 0xFFFF);
}
/**
 * @brief		��BH1750�������ֽ�����ת��Ϊ����ǿ��ֵ��0-65535��
 * @param		dat  ���� �洢����ǿ�ȵĵ�ַ�������ֽ����飩
 * @retval		�ɹ� ���� ���ع���ǿ��ֵ
*/
uint16_t BH1750_Dat_To_Lux(uint8_t* dat)
{
		uint16_t lux = 0;
		lux = dat[0];
		lux <<= 8;
		lux += dat[1];
		lux = (int)(lux / 1.2);
	
		return lux;
}
