#ifndef __THRESHOLD_UI_H
#define __THRESHOLD_UI_H
#include "main.h"

typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Threshold_Page ;
extern Threshold_Page threshold_page_ui ;





//阈值设置页面初始化
void threshold_page_ui_init(uint8_t item);
//阈值设置页面按键处理
void threshold_page_process(uint8_t Event_Code);





#endif //__THRESHOLD_UI_H

