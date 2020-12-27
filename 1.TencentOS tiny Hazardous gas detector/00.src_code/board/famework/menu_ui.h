#ifndef __MENU_UI_H
#define __MENU_UI_H
#include "main.h"

/*菜单按键键值定义*/
#define LEFT          		1
#define LEFT_LONG 			2
#define RIGHT   			3
#define RIGHT_LONG    		4
#define STR_LEN 			20


/*全局菜单显示项定义*/
typedef struct 
{
	uint8_t x;
	uint8_t y;
	uint8_t Str[STR_LEN];
	uint16_t front_color;
	uint16_t back_color;
	uint8_t font_num;
	uint8_t mode ;
}Item;

typedef struct
{
    /*界面操作游标*/
    uint8_t flow_cursor ;
} Cursor ;
extern Cursor Flow_Cursor ;

/*当前菜单*/
typedef enum 
{
	SLEEP_PAGE = 0,
	MAIN_PAGE,
	TEST_PAGE,
	LOG_PAGE,
	CONF_PAGE,
	PARA_PAGE,
	VERSION_PAGE,
	PASSWD_PAGE,
	PASSWD_INPUT_PAGE,
	DATETIME_PAGE,
	PLOT_PAGE,
	RECOVERY_PAGE,
}OP_PAGE;

/*菜单初始化*/
void Menu_Init(void);
/*显示菜单文字项*/
void display_menu_item(Item item_struct[],uint8_t item);
/*显示菜单文字项*/
void display_menu_color_item(Item item_struct[],uint8_t item,uint16_t front_color);

#endif //__MENU_UI_H

