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


/*��ʾLED״̬*/
void display_led(uint8_t status);
/*��ʾ����״̬*/
void display_fan(uint8_t status);
/*��ʾ�¶�״̬*/
void display_temp(uint8_t status);
/*��ʾʪ��״̬*/
void display_hum(uint8_t status);
/*��ʾSD��״̬*/
void display_sd_card(uint8_t status);
/*��ʾ����״̬*/
void display_passwd_icon(uint8_t status);
/*��ʾ����������*/
void display_server_status(uint8_t status);
/*��ʾ�ź�����*/
void display_signal_status(uint8_t status);
/*��ʾ�¶�ֵ*/
void display_temp_value(float value, uint16_t color, uint8_t enable);
/*��ʾʪ��ֵ*/
void display_humidity_value(float value, uint16_t color, uint8_t enable);
/*��ʾ��ǿֵ*/
void display_light_strength_value(uint16_t value, uint16_t color, uint8_t enable);
void display_main_page_ui(uint8_t enable);
/*��ʾ����ʱ�䵥λ*/
void display_runing_time_font(uint8_t enable);
/*��ʾ����ʱ��*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable);
/*������ҳ����ֵ��ʾ*/
void update_main_page_data_display(void);
void refresh_all_ui(void);
#endif //__MAIN_UI_H
