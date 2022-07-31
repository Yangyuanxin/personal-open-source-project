#ifndef __CONF_UI_H
#define __CONF_UI_H
#include "main.h"


typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Conf_Page ;
extern Conf_Page conf_page_ui ;



void conf_page_ui_init(uint8_t item);
void conf_page_process(uint8_t Event_Code);
/*选择菜单项目*/
void select_conf_item(uint8_t item, uint16_t color);


#endif //__CONF_UI_H

