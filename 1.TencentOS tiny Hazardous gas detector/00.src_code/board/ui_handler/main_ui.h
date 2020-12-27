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

#endif //__MAIN_UI_H
