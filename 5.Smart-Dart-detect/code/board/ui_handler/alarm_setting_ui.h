#ifndef __ALARM_SETTING_UI_H
#define __ALARM_SETTING_UI_H
#include "main.h"


typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Alarm_Setting_Page ;
extern Alarm_Setting_Page alarm_setting_page_ui ;

/*��������ҳ���ʼ��*/
void alarm_setting_page_ui_init(uint8_t item);
/*��������ҳ�水������*/
void alarm_setting_page_process(uint8_t KeyValue);







#endif //__ALARM_SETTING_UI_H

