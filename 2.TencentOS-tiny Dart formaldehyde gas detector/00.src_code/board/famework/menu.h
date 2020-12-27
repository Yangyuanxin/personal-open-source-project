#ifndef __MENU_H
#define __MENU_H
#include "main.h"

typedef void (*menu_op_func)(uint8_t);
typedef struct OP_STRUCT
{
	int op_menu ; 				/*操作菜单*/
	menu_op_func opfun ;		/*带参数的操作方法*/
}OP_MENU_PAGE;

/*事件处理*/
typedef struct 
{
	void (*handler_func)();
}Event_Frame ;


void Menu_Select_Item(uint8_t current_index, uint8_t KeyValue);


#endif //__MENU_H
