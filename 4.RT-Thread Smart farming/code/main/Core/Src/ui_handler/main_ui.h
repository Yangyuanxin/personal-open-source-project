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


/*显示LED状态*/
void display_led(uint8_t status);
/*显示风扇状态*/
void display_fan(uint8_t status);
/*显示温度状态*/
void display_temp(uint8_t status);
/*显示湿度状态*/
void display_hum(uint8_t status);
/*显示SD卡状态*/
void display_sd_card(uint8_t status);
/*显示密码状态*/
void display_passwd_icon(uint8_t status);
/*显示服务器连接*/
void display_server_status(uint8_t status);
/*显示信号质量*/
void display_signal_status(uint8_t status);
/*显示温度值*/
void display_temp_value(float value, uint16_t color, uint8_t enable);
/*显示湿度值*/
void display_humidity_value(float value, uint16_t color, uint8_t enable);
/*显示光强值*/
void display_light_strength_value(uint16_t value, uint16_t color, uint8_t enable);
void display_main_page_ui(uint8_t enable);
/*显示运行时间单位*/
void display_runing_time_font(uint8_t enable);
/*显示运行时间*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable);
/*更新主页面数值显示*/
void update_main_page_data_display(void);
void refresh_all_ui(void);
#endif //__MAIN_UI_H
