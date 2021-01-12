#ifndef __LCD_PLOT_DISPLAY_H
#define __LCD_PLOT_DISPLAY_H
#include "main.h"

#define DATA_SIZE			20

/*曲线显示区域，即相对于LCD的宽度，X轴*/
#define PLOT_DISPLAY_AREA_X  51
/*曲线显示区域，即相对于LCD的高度，Y轴*/
#define PLOT_DISPLAY_AREA_Y  210

#define LCD_X 240
#define LCD_Y	240

/*曲线处理*/
typedef struct
{
		/*曲线模式*/
		uint8_t plot_mode ;
		/*曲线定屏*/
		uint8_t fixed_screen ;
		/*实时曲线数据*/
    float rel_data_data[DATA_SIZE];
		/*旧的曲线数据*/
    float old_plot_data[DATA_SIZE];
		/*新的曲线数据*/
    float new_plot_data[DATA_SIZE];
} plot_data_handler ;
extern plot_data_handler plot_handler ;

/*曲线参数初始化*/
void LCD_Plot_Init(void);
/*
cur_data:当前要显示的曲线数据包
cur_data_size:当前要显示的曲线数据包的大小
*/
void LCD_Plot_Remap(float *cur_data, float *backup_data, uint16_t cur_data_size);
/*显示曲线*/
void LCD_Plot_Display(float *pData, uint32_t size, uint16_t color);


#endif //__LCD_PLOT_DISPLAY_H

