#ifndef __ALARM_SETTING_UI_H
#define __ALARM_SETTING_UI_H
#include "main.h"


typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Alarm_Setting_Page ;
extern Alarm_Setting_Page alarm_setting_page_ui ;

/*报警设置页面初始化*/
void alarm_setting_page_ui_init(uint8_t item);
/*报警设置页面按键处理*/
void alarm_setting_page_process(uint8_t KeyValue);







#endif //__ALARM_SETTING_UI_H

