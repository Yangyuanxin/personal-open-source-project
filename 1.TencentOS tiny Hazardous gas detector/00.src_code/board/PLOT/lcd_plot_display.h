#ifndef __LCD_PLOT_DISPLAY_H
#define __LCD_PLOT_DISPLAY_H
#include "main.h"

#define DATA_SIZE			240

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
    uint16_t rel_data_data[DATA_SIZE];
		/*�ɵ���������*/
    uint16_t old_plot_data[DATA_SIZE];
		/*�µ���������*/
    uint16_t new_plot_data[DATA_SIZE];
} plot_data_handler ;
extern plot_data_handler plot_handler ;

/*���߲�����ʼ��*/
void LCD_Plot_Init(void);
/*
cur_data:��ǰҪ��ʾ���������ݰ�
cur_data_size:��ǰҪ��ʾ���������ݰ��Ĵ�С
*/
void LCD_Plot_Remap(uint16_t *cur_data, uint16_t *backup_data, uint16_t cur_data_size);
/*��ʾ����*/
void LCD_Plot_Display(uint16_t *pData, uint32_t size, uint16_t color);


#endif //__LCD_PLOT_DISPLAY_H

