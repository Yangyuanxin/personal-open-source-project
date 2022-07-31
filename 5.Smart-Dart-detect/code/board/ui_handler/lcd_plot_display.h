#ifndef __LCD_PLOT_DISPLAY_H
#define __LCD_PLOT_DISPLAY_H
#include "main.h"

#define DATA_SIZE			20

/*������ʾ���򣬼������LCD�Ŀ�ȣ�X��*/
#define PLOT_DISPLAY_AREA_X  51
/*������ʾ���򣬼������LCD�ĸ߶ȣ�Y��*/
#define PLOT_DISPLAY_AREA_Y  210

#define LCD_X 240
#define LCD_Y	240

/*���ߴ���*/
typedef struct
{
		/*����ģʽ*/
		uint8_t plot_mode ;
		/*���߶���*/
		uint8_t fixed_screen ;
		/*ʵʱ��������*/
    float rel_data_data[DATA_SIZE];
		/*�ɵ���������*/
    float old_plot_data[DATA_SIZE];
		/*�µ���������*/
    float new_plot_data[DATA_SIZE];
} plot_data_handler ;
extern plot_data_handler plot_handler ;

/*���߲�����ʼ��*/
void LCD_Plot_Init(void);
/*
cur_data:��ǰҪ��ʾ���������ݰ�
cur_data_size:��ǰҪ��ʾ���������ݰ��Ĵ�С
*/
void LCD_Plot_Remap(float *cur_data, float *backup_data, uint16_t cur_data_size);
/*��ʾ����*/
void LCD_Plot_Display(float *pData, uint32_t size, uint16_t color);


#endif //__LCD_PLOT_DISPLAY_H

