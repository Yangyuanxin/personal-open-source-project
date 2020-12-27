#ifndef __MAIN_UI_H
#define __MAIN_UI_H
#include "main.h"

typedef struct
{
    uint8_t select_item ;
    char 	display_buf[10];
} Main_Page_Ui ;
extern Main_Page_Ui main_page_ui ;

/*主页面初始化*/
void main_page_init(void);
/*主页面事件处理*/
void main_page_process(uint8_t Event_Code);
/*左切换主页面菜单,右进入菜单项*/
void Select_Main_Menu_Item(uint8_t item);

/*获取RTC时钟并显示*/
void Get_RTC_Date_Time(void);
/*底部菜单栏显示*/
void	main_menu_item_display(uint8_t enable);
/*显示主页面*/
void display_main_page(uint8_t enable);

/*显示甲醛值*/
void display_hcho_value(float hcho_value, uint16_t color, uint8_t enable);
/*显示甲醛最大最小值*/
void display_hcho_max_value(float hcho_value, uint16_t color, uint8_t enable);
void display_hcho_min_value(float hcho_value, uint16_t color, uint8_t enable);
/*显示工作状态*/
void display_work_status(float hcho_value, uint8_t enable);
/*显示甲醛值单位*/
void display_hcho_unit(uint8_t enable);
void display_main_page_ui(uint8_t enable);
/*显示运行时间单位*/
void display_runing_time_font(uint8_t enable);
/*显示运行时间*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable);
/*更新主页面数值显示*/
void update_main_page_data_display(void);
#endif //__MAIN_UI_H
