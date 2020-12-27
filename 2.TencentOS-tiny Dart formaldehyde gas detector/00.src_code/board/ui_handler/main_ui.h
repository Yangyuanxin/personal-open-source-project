#ifndef __MAIN_UI_H
#define __MAIN_UI_H
#include "main.h"

typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Main_Page_Ui ;
extern Main_Page_Ui main_page_ui ;

/*��ҳ���ʼ��*/
void main_page_init(void);
/*��ҳ���¼�����*/
void main_page_process(uint8_t Event_Code);
/*���л���ҳ��˵�,�ҽ���˵���*/
void Select_Main_Menu_Item(uint8_t item);

/*��ȡRTCʱ�Ӳ���ʾ*/
void Get_RTC_Date_Time(void);
/*�ײ��˵�����ʾ*/
void	main_menu_item_display(uint8_t enable);
/*��ʾ��ҳ��*/
void display_main_page(uint8_t enable);

/*��ʾ��ȩֵ*/
void display_hcho_value(float hcho_value, uint16_t color, uint8_t enable);
/*��ʾ��ȩ�����Сֵ*/
void display_hcho_max_value(float hcho_value, uint16_t color, uint8_t enable);
void display_hcho_min_value(float hcho_value, uint16_t color, uint8_t enable);
/*��ʾ����״̬*/
void display_work_status(float hcho_value, uint8_t enable);
/*��ʾ��ȩֵ��λ*/
void display_hcho_unit(uint8_t enable);
void display_main_page_ui(uint8_t enable);
/*��ʾ����ʱ�䵥λ*/
void display_runing_time_font(uint8_t enable);
/*��ʾ����ʱ��*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable);
/*������ҳ����ֵ��ʾ*/
void update_main_page_data_display(void);
#endif //__MAIN_UI_H
