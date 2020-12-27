#ifndef __DEBUG_UI_H
#define __DEBUG_UI_H
#include "main.h"

typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Debug_Setting_Page ;
extern Debug_Setting_Page debug_setting_page_ui ;

/*调试页面初始化*/
void debug_page_ui_init(uint8_t item);
/*调试页面按键处理*/
void debug_page_process(uint8_t KeyValue);




#endif //__DEBUG_UI_H

