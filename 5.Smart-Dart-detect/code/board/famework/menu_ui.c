#include "menu_ui.h"

Cursor Flow_Cursor ;

/*显示菜单文字项*/
void display_menu_item(Item item_struct[],uint8_t item)
{
	LCD_Chinese_Show_Para ch_para ;
	ch_para.x = 	item_struct[item].x;
	ch_para.y =		item_struct[item].y;
	ch_para.s =		item_struct[item].Str;
	ch_para.fc =	item_struct[item].front_color;
	ch_para.bc =	item_struct[item].back_color;
	ch_para.sizey = item_struct[item].font_num;
	ch_para.mode  = item_struct[item].mode;
	lcd_model.lcd_driver->lcd_show_chinese_str(ch_para);
}


/*显示菜单文字项*/
void display_menu_color_item(Item item_struct[],uint8_t item,uint16_t front_color)
{
	LCD_Chinese_Show_Para ch_para ;
	ch_para.x = 	item_struct[item].x;
	ch_para.y =		item_struct[item].y;
	ch_para.s =		item_struct[item].Str;
	ch_para.fc =	front_color;
	ch_para.bc =	item_struct[item].back_color;
	ch_para.sizey = item_struct[item].font_num;
	ch_para.mode  = item_struct[item].mode;
	lcd_model.lcd_driver->lcd_show_chinese_str(ch_para);
}

/*菜单初始化*/
void Menu_Init(void)
{
    main_page_init();
}




