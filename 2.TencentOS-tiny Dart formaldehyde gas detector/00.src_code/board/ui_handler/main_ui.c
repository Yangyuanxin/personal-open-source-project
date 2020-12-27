#include "main_ui.h"
#include "bsp_bmp.h"

extern Dart_Sensor sensor ;
Main_Page_Ui main_page_ui ;
/*腾讯LOGO*/
const unsigned char gImage_tencet_logo[1352];
LCD_Ascii_Show_Para datatime_display_para = {10, 2, 230, DateTime_Handler_Info.DisPlay_DateBuf, WHITE, BLACK, 24} ;

LCD_Chinese_Show_Para main_title_font[] =
{
    {5, 33, (uint8_t *)"达特甲醛探测", GREEN, BLACK, 24, 1},
    {5, 33, (uint8_t *)"达特甲醛探测", BLACK, BLACK, 24, 1},
};

LCD_Chinese_Show_Para main_status_title_font[] =
{
    {5, 130, (uint8_t *)"工作状态", GREEN, BLACK, 16, 1},
    {5, 130, (uint8_t *)"工作状态", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_work_status_title_font[] =
{
    {85, 130, (uint8_t *)"正常", GREEN, BLACK, 16, 1},
    {85, 130, (uint8_t *)"异常", RED, BLACK, 16, 1},
    {85, 130, (uint8_t *)"正常", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para hide_main_work_status_title_font[] =
{
    {85, 130, (uint8_t *)"正常", BLACK, BLACK, 16, 1},
    {85, 130, (uint8_t *)"异常", BLACK, BLACK, 16, 1},
    {85, 130, (uint8_t *)"正常", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_max_font[] =
{
    {5, 160, (uint8_t *)"最大值", GREEN, BLACK, 16, 1},
    {5, 160, (uint8_t *)"最大值", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_min_font[] =
{
    {5, 185, (uint8_t *)"最小值", GREEN, BLACK, 16, 1},
    {5, 185, (uint8_t *)"最小值", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_runtime_font[] =
{
    {5, 215, (uint8_t *)"运行时间", YELLOW, BLACK, 16, 1},
    {5, 215, (uint8_t *)"运行时间", BLACK, BLACK, 16, 1},
};

/*画框图*/
void draw_rect_func(uint8_t enable);
/*显示文字和数值*/
void display_font_and_value(uint8_t enable);

/*选择菜单项*/
void Select_Main_Menu_Item(uint8_t item)
{
    display_main_page_ui(1);
}

/*获取RTC时钟并显示*/
void Get_RTC_Date_Time(void)
{
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
    lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
}



/*显示腾讯LOGO*/
#include "lcd_spi2_drv.h"
void display_tecent_logo(uint8_t enable)
{
    LCD_Fill_Para fill_logo;
    fill_logo.x_start = 190 ;
    fill_logo.y_start = 50 ;
    fill_logo.color   = BLACK ;
    fill_logo.x_end = fill_logo.x_start + 26 ;
    fill_logo.y_end = fill_logo.y_start + 26 ;
		char kernel_version0[30] = {0};
		char kernel_version1[30] = {0};
		sprintf(kernel_version0, "Power by");
		sprintf(kernel_version1, "TencentOS tiny");
		LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para0 = {140, 160, 100, kernel_version0, BLACK, BLUE, 16};
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para1 = {120, 185, 120, kernel_version1, BLACK, BLUE, 16};
		LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para0 = {140, 160, 100, kernel_version0, BLACK, BLACK, 16};
    LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para1 = {120, 185, 120, kernel_version1, BLACK, BLACK, 16};
		
    if(1 == enable)
    {
        LCD_Show_Image(fill_logo.x_start, fill_logo.y_start, 26, 26, gImage_tencet_logo);
				/*显示Power by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para1);
    }
    else
		{
        lcd_model.lcd_driver->lcd_fill(fill_logo);
				/*隐藏Power by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para1);
		}
}

/*显示甲醛值*/
void display_hcho_value(float hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 75;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 32 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

/*显示甲醛最大最小值*/
void display_hcho_max_value(float hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 161;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 16 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

void display_hcho_min_value(float hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 186;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 16 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}



/*显示工作状态*/
void display_work_status(float hcho_value, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    /*默认为安全状态*/
    static uint8_t cutover_status = 0 ;
    sprintf(display_buf, "%.3f", hcho_value);

    if(enable == 1)
    {
        /*报警*/
        if(hcho_value >= User_Memory_Para.alarm_threshold[User_Memory_Para.sensivity])
        {
            if(1 == cutover_status)
            {
                cutover_status = 0 ;
                lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[0]);
            }

            lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[1]);
        }
        else
        {
            if(0 == cutover_status)
            {
                cutover_status = 1 ;
                lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[1]);
            }

            lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[0]);
        }
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[2]);
    }
}

/*显示甲醛值单位*/
void display_hcho_unit(uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "PPM");
    hcho_value_ascii.x = 190;
    hcho_value_ascii.y = 125;
    hcho_value_ascii.max_width = 50;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 24 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = GREEN ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

/*显示运行时间单位*/
void display_runing_time_font(uint8_t enable)
{
    LCD_Chinese_Show_Para main_runing_time_font[] =
    {
        {110, 215, (uint8_t *)"时", YELLOW, BLACK, 16, 1},
        {110, 215, (uint8_t *)"时", BLACK, BLACK, 16, 1},

        {150, 215, (uint8_t *)"分", YELLOW, BLACK, 16, 1},
        {150, 215, (uint8_t *)"分", BLACK, BLACK, 16, 1},

        {190, 215, (uint8_t *)"秒", YELLOW, BLACK, 16, 1},
        {190, 215, (uint8_t *)"秒", BLACK, BLACK, 16, 1},
    };

    if(1 == enable)
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[4]);
    }
    else
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[3]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[5]);
    }
}

/*显示运行时间*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable)
{
    char time_buf[5] = {0};
    char min_buf[5] = {0};
    char sec_buf[5] = {0};
    sprintf(time_buf, "%02d", hour);
    sprintf(min_buf, "%02d", min);
    sprintf(sec_buf, "%02d", sec);
    LCD_Ascii_Show_Para hour_display_para = {90, 215, 10, time_buf, BLACK, YELLOW, 16} ;
    LCD_Ascii_Show_Para min_display_para  = {130, 215, 10, min_buf, BLACK, YELLOW, 16} ;
    LCD_Ascii_Show_Para sec_display_para  = {170, 215, 10, sec_buf, BLACK, YELLOW, 16} ;

    LCD_Ascii_Show_Para hide_hour_display_para = {90, 215, 10, time_buf, BLACK, BLACK, 16} ;
    LCD_Ascii_Show_Para hide_min_display_para  = {130, 215, 10, min_buf, BLACK, BLACK, 16} ;
    LCD_Ascii_Show_Para hide_sec_display_para  = {170, 215, 10, sec_buf, BLACK, BLACK, 16} ;

    if(1 == enable)
    {
        lcd_model.lcd_driver->lcd_show_ascii_str(hour_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(min_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(sec_display_para);
    }
    else
    {
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_hour_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_min_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_sec_display_para);
    }
}


/*画框图*/
void draw_rect_func(uint8_t enable)
{
    if(1 == enable)
    {
        LCD_Rect_Show_Para lcd_rect = {0, 239, 0, 239, WHITE};
        lcd_model.lcd_driver->lcd_draw_rect(lcd_rect);

        LCD_Line_Show_Para line_split0 = {0, 239, 150, 150, WHITE};
        lcd_model.lcd_driver->lcd_draw_line(line_split0);

        LCD_Line_Show_Para line_split1 = {0, 239, 210, 210, WHITE};
        lcd_model.lcd_driver->lcd_draw_line(line_split1);
    }
    else
    {
        LCD_Rect_Show_Para lcd_rect = {0, 239, 0, 239, BLACK};
        lcd_model.lcd_driver->lcd_draw_rect(lcd_rect);

        LCD_Line_Show_Para line_split0 = {0, 239, 150, 150, BLACK};
        lcd_model.lcd_driver->lcd_draw_line(line_split0);

        LCD_Line_Show_Para line_split1 = {0, 239, 210, 210, BLACK};
        lcd_model.lcd_driver->lcd_draw_line(line_split1);
    }
}

/*显示文字和数值*/
void display_font_and_value(uint8_t enable)
{
    static uint8_t start_flag = 0 ;
    LCD_Ascii_Show_Para ascii_colon_str[] =
    {
        {75, 130, 125, ":", BLACK, GREEN, 16},
        {60, 160, 125, ":", BLACK, GREEN, 16},
        {60, 185, 125, ":", BLACK, GREEN, 16},
        {75, 215, 125, ":", BLACK, YELLOW, 16},

        {75, 130, 125, ":", BLACK, BLACK, 16},
        {60, 160, 125, ":", BLACK, BLACK, 16},
        {60, 185, 125, ":", BLACK, BLACK, 16},
        {75, 215, 125, ":", BLACK, BLACK, 16},
    };

    if(1 == enable)
    {
        /*显示冒号*/
        for(int i = 0 ; i < 4 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_title_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_max_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_min_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[0]);

        /*开机初始化显示，避免数据闪烁效果*/
        if(0 == start_flag)
        {
            start_flag = 1 ;
            display_hcho_value(0.00, GREEN, 1);
            display_hcho_max_value(0.00, GREEN, 1);
            display_hcho_min_value(0.00, GREEN, 1);
            display_work_status(0.00, 1);
            display_hcho_unit(1);
            display_tecent_logo(1);
        }
    }
    else
    {
        /*隐藏冒号*/
        for(int i = 4 ; i < 8 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_title_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_max_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_min_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[1]);

        display_hcho_value(0.00, BLACK, 0);
        display_hcho_max_value(0.00, BLACK, 0);
        display_hcho_min_value(0.00, BLACK, 0);
        display_work_status(0.00, 0);
        display_hcho_unit(0);
        display_tecent_logo(0);
    }
}


/*一次性更新主页面数值显示*/
void update_main_page_data_display(void)
{
    display_tecent_logo(1);
    display_hcho_value(sensor.gas_density, GREEN, 1);
    display_work_status(sensor.gas_density, 1);
    display_hcho_unit(1);
    display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
    display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value, GREEN, 1);
    display_runing_time_font(1);
    display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 1);
}

/*显示主页面UI*/
void display_main_page_ui(uint8_t enable)
{
    if(1 == enable)
    {
        draw_rect_func(1);
        display_font_and_value(1);
    }
    else
    {
        draw_rect_func(0);
        display_font_and_value(0);
    }
}

/*主页面初始化*/
void main_page_init(void)
{
    main_page_ui.select_item = 0;
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    lcd_model.lcd_driver->lcd_display_onoff(0);
    Get_RTC_Date_Time();
    Select_Main_Menu_Item(main_page_ui.select_item);
    lcd_model.lcd_driver->lcd_display_onoff(1);
}


/********************************************按键处理****************************************************/
/*右键处理*/
static void Handler_Main_Page_Right_Key(void)
{
    /*确保界面上的数据都隐藏了再跳转到新的页面*/
    //锁定系统调度
    tos_knl_sched_lock();
    display_main_page_ui(0);
    display_runing_time_font(0);
    display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 0);
		lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[0]);	
    lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[1]);
    password_input_page_init();
    //解锁系统调度
    tos_knl_sched_unlock();
}

Event_Frame Main_Page_Event[] =
{
    {NULL},
    {NULL},
    {Handler_Main_Page_Right_Key},
    {NULL},
};

/*主页面事件处理*/
void main_page_process(uint8_t Event_Code)
{
    if(Main_Page_Event[Event_Code - 1].handler_func != NULL)
        Main_Page_Event[Event_Code - 1].handler_func();
}
/********************************************按键处理****************************************************/

const unsigned char gImage_tencet_logo[1352] =   /* 0X10,0X10,0X00,0X1A,0X00,0X1A,0X01,0X1B, */
{
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XF7, 0XFF, 0XCE, 0X9E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XDF, 0X1F, 0XD6, 0XFE, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XC6, 0XDE, 0X0A, 0X3A, 0X00, 0XD9, 0X0A, 0X3A,
    0XD7, 0X1F, 0XFF, 0XFF, 0X6C, 0XBD, 0X01, 0X19, 0X00, 0XF9, 0X54, 0X3C, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X02, 0X5A, 0X12, 0XFB,
    0XB6, 0X7E, 0X0A, 0X5A, 0X1B, 0X1B, 0X8D, 0X9D, 0X00, 0XF9, 0X85, 0X3D, 0XB6, 0X5E, 0X00, 0XD9,
    0X85, 0X5D, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0X02, 0X1A, 0X1A, 0X7B, 0XFF, 0XDF, 0XD7, 0X5F, 0X01, 0XBA, 0X01, 0XFA, 0X2B, 0X9C, 0XFF, 0XFF,
    0XA5, 0XFE, 0X00, 0XD9, 0X74, 0XFD, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X3F, 0X2A, 0X1A, 0X7D, 0X3D, 0X8D, 0X7D, 0X02, 0X5A, 0X3B, 0XBC,
    0X12, 0X9B, 0XB6, 0X7E, 0X12, 0XBB, 0X5C, 0X5C, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XCE, 0XDE,
    0X74, 0XDD, 0X43, 0XDC, 0X3B, 0XDC, 0X3B, 0XDC, 0X5C, 0X7C, 0X3B, 0XFC, 0X12, 0XDB, 0X00, 0XB9,
    0X7D, 0X3D, 0XF7, 0XDF, 0X02, 0X1A, 0X01, 0X7A, 0X33, 0X9C, 0X54, 0X5C, 0X4C, 0X3C, 0X3B, 0XDC,
    0X3B, 0XBC, 0X54, 0X1C, 0XA5, 0XBD, 0XF7, 0XDF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0X53, 0XDC, 0X00, 0XF9, 0X01, 0X7A, 0X02, 0X7B, 0X0A, 0XBB, 0X02, 0X9B, 0X02, 0X9B, 0X12, 0XDB,
    0X22, 0XDB, 0X85, 0X3D, 0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X5F, 0X4B, 0XBC, 0X12, 0XDB, 0X0A, 0XBB,
    0X02, 0X9B, 0X02, 0X9B, 0X0A, 0XBB, 0X02, 0X1A, 0X00, 0XF9, 0X01, 0XBA, 0XA6, 0X1E, 0XFF, 0XFF,
    0XE7, 0X7F, 0X1A, 0X7B, 0X00, 0XB9, 0X74, 0XFD, 0XDF, 0X5F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XBE, 0X9E, 0X2B, 0X1B,
    0X00, 0X59, 0X7D, 0X1D, 0X95, 0XBD, 0X00, 0XD9, 0XBE, 0X9E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0X43, 0X7B, 0X01, 0XFA, 0XEF, 0XBF, 0XE7, 0X7F, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X3F, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X8D, 0X5D, 0X2B, 0X1B, 0X02, 0X5A, 0X02, 0X7B,
    0X3B, 0X7B, 0XAD, 0XFE, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XEF, 0XBF, 0X74, 0XBD, 0X1A, 0XDB, 0X02, 0X3A, 0X02, 0X9B, 0X4B, 0XDC, 0XC6, 0XBE, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X7F, 0X12, 0X5A, 0X00, 0XD9, 0X12, 0XDB,
    0X4C, 0X3C, 0X43, 0XFC, 0X02, 0X5A, 0X00, 0X99, 0X43, 0X7B, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XB6, 0X7E, 0X01, 0X7A, 0X01, 0X19, 0X23, 0X3B, 0X54, 0X5C, 0X33, 0XBC, 0X01, 0XDA,
    0X00, 0XB9, 0X6C, 0XBD, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XF7, 0XBF, 0X01, 0XDA, 0X01, 0X7A,
    0XB6, 0X7E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X95, 0X9D, 0X00, 0X99, 0X33, 0X5B,
    0XFF, 0XFF, 0XFF, 0XFF, 0XC6, 0XDE, 0X00, 0XD9, 0X22, 0X9B, 0XD7, 0X1F, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XF7, 0XDF, 0X6C, 0X7C, 0X00, 0X39, 0X6C, 0XFD, 0XFF, 0XFF, 0XFF, 0XFF, 0X44, 0X1C,
    0X00, 0XF9, 0XDF, 0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XA6, 0X1E, 0X00, 0X18, 0X85, 0X5D, 0XFF, 0XFF, 0X01, 0XBA, 0X01, 0XDA, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X64, 0X7C, 0X00, 0XD9, 0XCE, 0XFE,
    0XEF, 0X7F, 0X01, 0X5A, 0X64, 0XDD, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X54, 0X1C, 0X85, 0X5D, 0XFF, 0XFF, 0X3B, 0X7B, 0XBE, 0XDE,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0X01, 0X9A, 0X4C, 0X3C, 0XB6, 0X5E, 0X01, 0X5A, 0XC6, 0XBE, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X3F, 0X74, 0X3C, 0XBE, 0X9E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0X74, 0X9C, 0X7C, 0XFD, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0X33, 0XBC, 0X02, 0X9B, 0XA5, 0XFE, 0X01, 0X7A, 0XD7, 0X1F, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XF7, 0XBF, 0X0A, 0XBB, 0X01, 0X7A, 0X01, 0XFA, 0XCE, 0XDE,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X54, 0X3C, 0X01, 0X5A, 0X01, 0X5A, 0X6C, 0XDD, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X4C, 0X5C, 0X02, 0X3A, 0XC6, 0XDE, 0X01, 0X19,
    0XAE, 0X3E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XEF, 0X9F, 0X02, 0X1A, 0X01, 0XFA,
    0X01, 0X7A, 0XB6, 0X7E, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X3B, 0XDC, 0X01, 0X7A, 0X01, 0X5A,
    0X5C, 0X7C, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X13, 0X1B, 0X1B, 0X1B,
    0XFF, 0XDF, 0X02, 0X1A, 0X2B, 0X5B, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XB6, 0X5E, 0X3A, 0XDB, 0X85, 0X5D, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XEF, 0X9F,
    0X53, 0XDC, 0X5C, 0X3C, 0XFF, 0XDF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XD7, 0X5F,
    0X00, 0XB9, 0X7D, 0X3D, 0XFF, 0XFF, 0X95, 0XDE, 0X00, 0X39, 0X85, 0X3D, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XF7, 0XDF, 0X0A, 0X1A, 0X01, 0XDA, 0XF7, 0XDF, 0XFF, 0XFF, 0XFF, 0XFF, 0X5C, 0X3C, 0X00, 0X59,
    0X43, 0XDC, 0XC6, 0XDE, 0XEF, 0X9F, 0XE7, 0X7F, 0XB6, 0X3E, 0XDF, 0X3F, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XB6, 0X7E, 0XC6, 0XBE, 0XF7, 0XBF,
    0XE7, 0X5F, 0X95, 0XDD, 0X01, 0XDA, 0X01, 0X19, 0XC6, 0XDE, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0X85, 0X1D, 0X01, 0X7A, 0X01, 0X19, 0X01, 0XBA, 0X01, 0X9A, 0X00, 0X79, 0X7D, 0X3D,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XDF, 0X02, 0X9B,
    0X00, 0XB9, 0X01, 0XDA, 0X01, 0X7A, 0X00, 0XF9, 0X22, 0XBB, 0XCF, 0X1F, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XEF, 0X9F, 0X9D, 0X9D, 0X6C, 0XDD, 0X74, 0XFD,
    0XAE, 0X1E, 0XF7, 0XDF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XDF, 0X3F, 0X8D, 0X5D, 0X6C, 0XDD, 0X7D, 0X1D, 0XBE, 0X7E, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
    0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
};

