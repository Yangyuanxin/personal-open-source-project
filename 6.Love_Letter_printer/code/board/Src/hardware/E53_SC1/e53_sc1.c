/*----------------------------------------------------------------------------
 * Tencent is pleased to support the open source community by making TencentOS
 * available.
 *
 * Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
 * If you have downloaded a copy of the TencentOS binary from Tencent, please
 * note that the TencentOS binary is licensed under the BSD 3-Clause License.
 *
 * If you have downloaded a copy of the TencentOS source code from Tencent,
 * please note that TencentOS source code is licensed under the BSD 3-Clause
 * License, except for the third-party components listed below which are
 * subject to different license terms. Your integration of TencentOS into your
 * own projects may require compliance with the BSD 3-Clause License, as well
 * as the other licenses applicable to the third-party components included
 * within TencentOS.
 *---------------------------------------------------------------------------*/
#include "e53_sc1.h"
#include "i2c.h"

/***************************************************************
* 函数名称: Init_BH1750
* 说    明: 写命令初始化BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
static void Init_BH1750(void)
{
	uint8_t t_Data = 0x01;
	HAL_I2C_Master_Transmit(&hi2c2,BH1750_Addr,&t_Data,1,0xff);
}

/***************************************************************
* 函数名称: Start_BH1750
* 说    明: 启动BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
static void Start_BH1750(void)
{
	uint8_t t_Data = 0x10;
	HAL_I2C_Master_Transmit(&hi2c2,BH1750_Addr,&t_Data,1,0xff); 
}

/***************************************************************
* 函数名称: Convert_BH1750
* 说    明: 数值转换
* 参    数: 无
* 返 回 值: 光强值
***************************************************************/
static float Convert_BH1750(void)
{
	float result_lx;
	uint8_t BUF[2];
	int result;
	Start_BH1750();
	HAL_Delay(180);
	HAL_I2C_Master_Receive(&hi2c2, BH1750_Addr+1,BUF,2,0xff); 
	result=BUF[0];
	result=(result<<8)+BUF[1];  //Synthetic Digital Illumination Intensity Data
	result_lx=(float)(result/1.2);
	return result_lx;
}

/***************************************************************
* 函数名称: e53_scl_read_data
* 说    明: 读取e53接口数据
* 参    数: 无
* 返 回 值: 无
***************************************************************/
float e53_scl_read_data(void)
{
	return Convert_BH1750();
}

/***************************************************************
* 函数名称: e53_sc1_init
* 说    明: 初始化e53
* 参    数: 无
* 返 回 值: 无
***************************************************************/
int e53_sc1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : E53_SC1_LED_Pin */
	GPIO_InitStruct.Pin = E53_SC1_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(E53_SC1_LED_GPIO_Port, &GPIO_InitStruct);
	Init_BH1750();
	return 0;
}
