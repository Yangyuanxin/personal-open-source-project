#include "para_ui.h"

Para_Setting_Page para_setting_page_ui ;

/*参数设置页面菜单项显示*/
Item Para_Setting_Item[] =
{
    {70, 60,   "系统参数", 	WHITE, BLACK, 24, 1},
    {22, 103,  "报警", 			WHITE, BLACK, 24, 1},
    {22, 134,  "数值", 			WHITE, BLACK, 24, 1},
    {22, 163,  "网络", 			WHITE, BLACK, 24, 1},
    {22, 192,  "单位", 		WHITE, BLACK, 24, 1},
};

/*参数设置页面状态显示*/
Item Para_Setting_Status_Item[] =
{
    {139, 103, 	"开", 	WHITE, BLACK, 24, 1},	//0
    {139, 103, 	"关", 	WHITE, BLACK, 24, 1},	//1
    {139, 134,  "开", 	WHITE, BLACK, 24, 1},	//2
    {139, 134,  "关", 	WHITE, BLACK, 24, 1},	//3
    {139, 163,  "开", 	WHITE, BLACK, 24, 1},	//4
    {139, 163,  "关", 	WHITE, BLACK, 24, 1},	//5
};

/*显示配置页面文字--状态相关的接口*/
static void Display_para_setting_page_item_font(int item, uint16_t color)
{
    display_menu_color_item(Para_Setting_Item, item, color);
}

static void Display_para_setting_page_status_font(int item, uint16_t color)
{
    display_menu_color_item(Para_Setting_Status_Item, item, color);
}

/*报警状态显示*/
void alarm_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(0, BLACK);
    Display_para_setting_page_status_font(1, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(1, color);
    else if(1 == status)
        Display_para_setting_page_status_font(0, color);
}

/*数值状态显示*/
void value_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(2, BLACK);
    Display_para_setting_page_status_font(3, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(3, color);
    else if(1 == status)
        Display_para_setting_page_status_font(2, color);
}

/*网络状态显示*/
void network_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(4, BLACK);
    Display_para_setting_page_status_font(5, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(5, color);
    else if(1 == status)
        Display_para_setting_page_status_font(4, color);
}

/*单位显示*/
void unit_display(uint8_t status,uint16_t color)
{
		//背景
		LCD_Ascii_Show_Para ascii_unit_str_bg =
    {
        .x = 139,
        .y = 192,
        .max_width = 50,
        .str = para_setting_page_ui.display_buf,
        .bc = BLACK,
        .sizey = 24
    };
		ascii_unit_str_bg.fc = BLACK ;
		sprintf(para_setting_page_ui.display_buf, "%s", "mg/m3");
		lcd_model.lcd_driver->lcd_show_ascii_str(ascii_unit_str_bg);
	
		LCD_Ascii_Show_Para ascii_unit_str =
    {
        .x = 139,
        .y = 192,
        .max_width = 50,
        .str = para_setting_page_ui.display_buf,
        .bc = BLACK,
        .sizey = 24
    };
    memset(para_setting_page_ui.display_buf, 0, 10);
		if(0 == status)
		{
			ascii_unit_str.fc = color ;
			sprintf(para_setting_page_ui.display_buf, "%s", "ppm");
		}
		else
		{
			ascii_unit_str.fc = color ;
			sprintf(para_setting_page_ui.display_buf, "%s", "mg/m3");
		}
		lcd_model.lcd_driver->lcd_show_ascii_str(ascii_unit_str);
}


/*标题设置显示1隐藏0*/
void para_item_display(uint8_t enable)
{
    LCD_Ascii_Show_Para ascii_colon_str[] =
    {
        {100, 103, 125, ":", BLACK, WHITE, 24},/*显示报警冒号*/
        {100, 134, 125, ":", BLACK, WHITE, 24},/*显示数值冒号*/
        {100, 163, 125, ":", BLACK, WHITE, 24},/*显示网络冒号*/
        {100, 192, 125, ":", BLACK, WHITE, 24},/*显示单位冒号*/

        {100, 103, 125, ":", BLACK, BLACK, 24},
        {100, 134, 125, ":", BLACK, BLACK, 24},
        {100, 163, 125, ":", BLACK, BLACK, 24},
        {100, 192, 125, ":", BLACK, BLACK, 24},

        {139, 192, 50, para_setting_page_ui.display_buf, BLACK, BLACK, 24},
    };

    if(enable == 1)
    {
        /*显示菜单*/
        for(int i = 0 ; i < 5 ; i++)
            Display_para_setting_page_item_font(i, WHITE);

        /*显示冒号*/
        for(int i = 0 ; i < 4 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        /*显示选择对应项*/
        alarm_status_display(para_setting_page_ui.alarm, RED);
        value_status_display(para_setting_page_ui.value, WHITE);
        network_status_display(para_setting_page_ui.network, WHITE);
				unit_display(para_setting_page_ui.unit,WHITE);
    }
    else if(enable == 0)
    {
        /*隐藏菜单*/
        for(int i = 0 ; i < 5 ; i++)
            Display_para_setting_page_item_font(i, BLACK);

        /*隐藏冒号*/
        for(int i = 4 ; i < 8 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        /*隐藏状态相关*/
        for(int i = 0 ; i < 6 ; i++)
            Display_para_setting_page_status_font(i, BLACK);

        /*隐藏单位显示*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[8]);
    }
}



/*基本参数初始化*/
void para_setting_para_init(void)
{
    para_setting_page_ui.select_item = 0 ;
    para_setting_page_ui.alarm = User_Memory_Para.alarm ;
    para_setting_page_ui.value = User_Memory_Para.value ;
    para_setting_page_ui.network = User_Memory_Para.upload_flag ;
    para_setting_page_ui.unit = User_Memory_Para.unit ;
}

/*参数设置页面初始化*/
void para_setting_page_init(void)
{
    Flow_Cursor.flow_cursor = PARA_PAGE ;
    para_setting_para_init();
    para_item_display(1);

}


/*选中具体调整项*/
void select_para_item(int item)
{
    alarm_status_display(para_setting_page_ui.alarm, WHITE);
    value_status_display(para_setting_page_ui.value, WHITE);
    network_status_display(para_setting_page_ui.network, WHITE);
    unit_display(para_setting_page_ui.unit,WHITE);

    /*报警*/
    if(0 == item)
        alarm_status_display(para_setting_page_ui.alarm, RED);
    /*数值*/
    else if(1 == item)
        value_status_display(para_setting_page_ui.value, RED);
    /*网络*/
    else if(2 == item)
        network_status_display(para_setting_page_ui.network, RED);
    /*单位*/
    else if(3 == item)
				unit_display(para_setting_page_ui.unit,RED);
}

/*跳转到下一参数设置项*/
void jump_next_para_item(void)
{
    switch(para_setting_page_ui.select_item)
    {
    case 0:
        para_setting_page_ui.select_item = 1 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 1:
        para_setting_page_ui.select_item = 2 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 2:
        para_setting_page_ui.select_item = 3 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 3:
        para_setting_page_ui.select_item = 0 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

/*设置当前选择项*/
void setting_current_select_item(void)
{
    switch(para_setting_page_ui.select_item)
    {
    case 0:
        (para_setting_page_ui.alarm < 1) ? (para_setting_page_ui.alarm++) : (para_setting_page_ui.alarm = 0);
        alarm_status_display(para_setting_page_ui.alarm, RED);
        break ;

    case 1:
        (para_setting_page_ui.value < 1) ? (para_setting_page_ui.value++) : (para_setting_page_ui.value = 0);
        value_status_display(para_setting_page_ui.value, RED);
        break ;

    case 2:
        (para_setting_page_ui.network < 1) ? (para_setting_page_ui.network++) : (para_setting_page_ui.network = 0);
        network_status_display(para_setting_page_ui.network, RED);
        break ;

    case 3:
        (para_setting_page_ui.unit < 1) ? (para_setting_page_ui.unit++) : (para_setting_page_ui.unit = 0);
        unit_display(para_setting_page_ui.unit,RED);
        break ;

    default:
        break ;
    }
}

/*参数保存并返回上一级页面*/
void para_save_and_return_prev_page(void)
{
    /*保存配置参数*/
    setting_alarm(para_setting_page_ui.alarm);
    setting_value(para_setting_page_ui.value);
    setting_network(para_setting_page_ui.network);
    setting_unit(para_setting_page_ui.unit);
    para_item_display(0);
    conf_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = CONF_PAGE ;
    conf_page_ui_init(conf_page_ui.select_item);
}


/*切换到下一设置项*/
void Para_Setting_Page_Left_Key(void)
{
    jump_next_para_item();
}

/*保存并返回*/
void Para_Setting_Page_Left_Long_Key(void)
{
    para_save_and_return_prev_page();
}

/*设置当前项*/
void Para_Setting_Page_Right_Key(void)
{
    setting_current_select_item();
}

/*保存并返回*/
void Para_Setting_Page_Right_Long_Key(void)
{
    para_save_and_return_prev_page();
}

/*参数设置事件*/
Event_Frame Para_Setting_Page_Event[] =
{
    {Para_Setting_Page_Left_Key},
    {Para_Setting_Page_Left_Long_Key},
    {Para_Setting_Page_Right_Key},
    {Para_Setting_Page_Right_Long_Key},
};

/*参数设置按键操作*/
void para_setting_page_process(uint8_t KeyValue)
{
    if(Para_Setting_Page_Event[KeyValue - 1].handler_func != NULL)
        Para_Setting_Page_Event[KeyValue - 1].handler_func();
}

