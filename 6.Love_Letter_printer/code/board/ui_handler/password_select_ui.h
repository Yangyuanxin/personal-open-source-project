#ifndef __PASSWORD_SELECT_UI_H
#define __PASSWORD_SELECT_UI_H
#include "main.h"



#define PASSWORD_SELECT_ITEM_X		   70
#define PASSWORD_SELECT_ITEM_Y     	 73-12
#define PASSWORD_SELECT_ITEM_FONT		 24

#define PASSWORD_SELECT_STATUS1_X		 70
#define PASSWORD_SELECT_STATUS1_Y    110
#define PASSWORD_SELECT_STATUS1_FONT 24

#define PASSWORD_SELECT_STATUS2_X			70
#define PASSWORD_SELECT_STATUS2_Y     135
#define PASSWORD_SELECT_STATUS2_FONT	24

typedef struct
{
    uint8_t select_item;
} Password_Select_Page ;
extern Password_Select_Page password_select_page_ui ;

/*密码选择页面初始化*/
void password_select_page_init(void);
/*密码选择页面按键处理*/
void password_select_page_process(uint8_t KeyValue);



















#endif //__PASSWORD_SELECT_UI_H

