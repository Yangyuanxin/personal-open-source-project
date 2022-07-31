#ifndef __MENU_H
#define __MENU_H
#include "main.h"

typedef void (*menu_op_func)(uint8_t);
typedef struct OP_STRUCT
{
	int op_menu ; 				/*�����˵�*/
	menu_op_func opfun ;		/*�������Ĳ�������*/
}OP_MENU_PAGE;

/*�¼�����*/
typedef struct 
{
	void (*handler_func)();
}Event_Frame ;


void Menu_Select_Item(uint8_t current_index, uint8_t KeyValue);


#endif //__MENU_H
