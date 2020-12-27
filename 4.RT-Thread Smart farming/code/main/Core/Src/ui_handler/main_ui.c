#include "main_ui.h"
#include "bsp_bmp.h"
#include "lcd_spi2_drv.h"

#define TEMP_X  5
#define TEMP_Y	33+40
#define HUM_X		5
#define HUM_Y		33+40+24
#define LIGHT_X	5
#define LIGHT_Y	33+40+48

Main_Page_Ui main_page_ui ;
LCD_Ascii_Show_Para datatime_display_para = {75, 5, 70, DateTime_Handler_Info.DisPlay_DateBuf, WHITE, BLACK, 16} ;

LCD_Chinese_Show_Para title_font[] =
{
    {33, 33, (uint8_t *)"小熊派智慧农业", GREEN, BLACK, 24, 1},
    {33, 33, (uint8_t *)"小熊派智慧农业", BLACK, BLACK, 24, 1},
};

LCD_Chinese_Show_Para main_title_font[] =
{
    {TEMP_X, TEMP_Y, (uint8_t *)"温度", GREEN, BLACK, 24, 1},
    {TEMP_X, TEMP_Y, (uint8_t *)"温度", BLACK, BLACK, 24, 1},
    {HUM_X, HUM_Y, (uint8_t *)"湿度", GREEN, BLACK, 24, 1},
    {HUM_X, HUM_Y, (uint8_t *)"湿度", BLACK, BLACK, 24, 1},
    {LIGHT_X, LIGHT_Y, (uint8_t *)"光强", GREEN, BLACK, 24, 1},
    {LIGHT_X, LIGHT_Y, (uint8_t *)"光强", BLACK, BLACK, 24, 1},
};

LCD_Chinese_Show_Para main_status_runtime_font[] =
{
    {5, 215, (uint8_t *)"运行时间", YELLOW, BLACK, 16, 1},
    {5, 215, (uint8_t *)"运行时间", BLACK, BLACK, 16, 1},
};

LCD_Bmp_Show_Para fan_logo_para[] =
{
    {4, 155, "0:/UI/fan/0.bmp"},//0
    {4, 155, "0:/UI/fan/1.bmp"},//1
    {4, 155, "0:/UI/fan/2.bmp"},//2
    {4, 155, "0:/UI/fan/3.bmp"},//3
    {4, 155, "0:/UI/fan/4.bmp"}//4
};

LCD_Bmp_Show_Para led_logo_para[] =
{
    {50, 150, "0:/UI/led/0.bmp"},//0
    {50, 150, "0:/UI/led/1.bmp"},//1
};

LCD_Bmp_Show_Para temp_logo_para[] =
{
    {180, 65, "0:/UI/temp/0.bmp"},//0
    {180, 65, "0:/UI/temp/1.bmp"},//1
    {180, 65, "0:/UI/temp/2.bmp"},//2
    {180, 65, "0:/UI/temp/3.bmp"},//3
    {180, 65, "0:/UI/temp/4.bmp"},//4
    {180, 65, "0:/UI/temp/5.bmp"}	//5
};

LCD_Bmp_Show_Para hum_logo_para[] =
{
    {180, 120, "0:/UI/Hum/0.bmp"},//0
    {180, 120, "0:/UI/Hum/1.bmp"},//1
    {180, 120, "0:/UI/Hum/2.bmp"},//2
    {180, 120, "0:/UI/Hum/3.bmp"},//3
    {180, 120, "0:/UI/Hum/4.bmp"},//4
    {180, 120, "0:/UI/Hum/5.bmp"}//5
};


LCD_Bmp_Show_Para passwd_logo_para[] =
{
    {150, 6, "0:/UI/password/0.bmp"},//0
    {150, 6, "0:/UI/password/1.bmp"},//1
};

LCD_Bmp_Show_Para card_logo_para[] =
{
    {170, 6, "0:/UI/card/0.bmp"},//0
    {170, 6, "0:/UI/card/1.bmp"},//1
};

LCD_Bmp_Show_Para siganl_logo_para[] =
{
    {5, 6, "0:/UI/signal/0.bmp"},//0	//无信号
    {5, 6, "0:/UI/signal/1.bmp"},//1
    {5, 6, "0:/UI/signal/2.bmp"},//2
    {5, 6, "0:/UI/signal/3.bmp"},//3
    {5, 6, "0:/UI/signal/4.bmp"},//4	//满信号
};

LCD_Bmp_Show_Para server_logo_para[] =
{
    {25, 6, "0:/UI/server/0.bmp"},//0	//与服务器断开连接
    {25, 6, "0:/UI/server/1.bmp"},//1	//已连接服务器
};

/*显示信号质量*/
void display_signal_status(uint8_t status)
{
    if(status > 4)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(siganl_logo_para[status]);
}

/*显示服务器连接*/
void display_server_status(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(card_logo_para[status]);
}

/*显示SD卡状态*/
void display_sd_card(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(server_logo_para[status]);
}

/*显示密码状态*/
void display_passwd_icon(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(passwd_logo_para[status]);
}

/*显示LED状态*/
void display_led(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(led_logo_para[status]);
}

/*显示风扇状态*/
void display_fan(uint8_t status)
{
    if(status > 4)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(fan_logo_para[status]);
}

/*显示温度状态*/
void display_temp(uint8_t status)
{
    if(status > 5)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(temp_logo_para[status]);
}

/*显示湿度状态*/
void display_hum(uint8_t status)
{
    if(status > 5)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(hum_logo_para[status]);
}

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



/*显示RT-Thread LOGO*/
void display_rt_thread_logo(uint8_t enable)
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
    sprintf(kernel_version1, "RT-Thread nano");
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para0 = {120, 165, 100, kernel_version0, BLACK, BLUE, 16};
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para1 = {100, 185, 120, kernel_version1, BLACK, BLUE, 16};
    LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para0 = {120, 160, 100, kernel_version0, BLACK, BLACK, 16};
    LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para1 = {100, 185, 120, kernel_version1, BLACK, BLACK, 16};

    if(1 == enable)
    {
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para1);
    }
    else
    {
        lcd_model.lcd_driver->lcd_fill(fill_logo);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para1);
    }
}

LCD_Chinese_Show_Para main_temp_unit[] =
{
    {TEMP_X + 126, TEMP_Y, (uint8_t *)"度", BLACK, BLACK, 24, 1},
    {TEMP_X + 126, TEMP_Y, (uint8_t *)"度", GREEN, BLACK, 24, 1},
};

/*显示温度值*/
void display_temp_value(float value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.2f", value);
    hcho_value_ascii.x = TEMP_X + 64;
    hcho_value_ascii.y = TEMP_Y;
    hcho_value_ascii.max_width = 150;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 24 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_temp_unit[1]);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_temp_unit[0]);
    }
}

/*显示湿度值*/
void display_humidity_value(float value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.2f(%%)", value);
    hcho_value_ascii.x = HUM_X + 64;
    hcho_value_ascii.y = HUM_Y;
    hcho_value_ascii.max_width = 150;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 24 ;

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

/*显示光强值*/
void display_light_strength_value(uint16_t value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%04d(Lux)", value);
    hcho_value_ascii.x = LIGHT_X + 64;
    hcho_value_ascii.y = LIGHT_Y;
    hcho_value_ascii.max_width = 120;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 24 ;

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
        LCD_Line_Show_Para line_split1 = {0, 239, 210, 210, WHITE};
        lcd_model.lcd_driver->lcd_draw_line(line_split1);
    }
    else
    {
        LCD_Rect_Show_Para lcd_rect = {0, 239, 0, 239, BLACK};
        lcd_model.lcd_driver->lcd_draw_rect(lcd_rect);
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
        {75, 215, 125, ":", BLACK, YELLOW, 16},
        {75, 215, 125, ":", BLACK, BLACK, 16},
    };

    LCD_Ascii_Show_Para ascii_font_colon_str[] =
    {
        {TEMP_X + 48,	 TEMP_Y, 125, ":", BLACK, GREEN, 24},
        {HUM_X + 48, 	 HUM_Y, 125, ":", BLACK, GREEN, 24},
        {LIGHT_X + 48, LIGHT_Y, 125, ":", BLACK, GREEN, 24},

        {TEMP_X + 48,  TEMP_Y, 125, ":", BLACK, BLACK, 24},
        {HUM_X + 48,   HUM_Y, 125, ":", BLACK, BLACK, 24},
        {LIGHT_X + 48, LIGHT_Y, 125, ":", BLACK, BLACK, 24},
    };

    if(1 == enable)
    {
        /*显示冒号*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[0]);

        for(int i = 0 ; i < 3 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_font_colon_str[i]);

        //标题显示
        lcd_model.lcd_driver->lcd_show_chinese_str(title_font[0]);
        //温度字体显示
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[0]);
        //湿度字体显示
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[2]);
        //光强字体显示
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[4]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[0]);

        /*开机初始化显示，避免数据闪烁效果*/
        if(0 == start_flag)
        {
            start_flag = 1 ;
            //显示温湿度、光强
            display_temp_value(0.00, GREEN, 1);
            display_humidity_value(0.00, GREEN, 1);
            display_light_strength_value(0.00, GREEN, 1);
            //显示温湿度、光强
            display_rt_thread_logo(1);
        }
    }
    else
    {
        /*隐藏冒号*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[1]);

        for(int i = 3 ; i < 6 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_font_colon_str[i]);

        //标题隐藏
        lcd_model.lcd_driver->lcd_show_chinese_str(title_font[1]);
        //温度隐藏
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[1]);
        //湿度隐藏
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[3]);
        //光强隐藏
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[5]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[1]);
        //隐藏温湿度、光强
        display_temp_value(0.00, BLACK, 0);
        display_humidity_value(0.00, BLACK, 0);
        display_light_strength_value(0.00, BLACK, 0);
        //隐藏温湿度、光强
        display_rt_thread_logo(0);
    }
}

/*刷新风扇图标显示*/
void refresh_fan(void)
{
    display_fan(0);
}

/*刷新补偿灯图标显示*/
void refresh_led(void)
{
    if(1 == User_Memory_Para.light_flag)
        display_led(1);
    else
        display_led(0);
}

/*刷新温度图标显示*/
void refresh_temp(void)
{
    if(E53_IA1_Sensor.temp >= 80 && E53_IA1_Sensor.temp < 100)
        display_temp(5);
    else if(E53_IA1_Sensor.temp >= 60 && E53_IA1_Sensor.temp < 80)
        display_temp(4);
    else if(E53_IA1_Sensor.temp >= 40 && E53_IA1_Sensor.temp < 60)
        display_temp(3);
    else if(E53_IA1_Sensor.temp >= 20 && E53_IA1_Sensor.temp < 40)
        display_temp(2);
    else if(E53_IA1_Sensor.temp >= 10 && E53_IA1_Sensor.temp < 20)
        display_temp(1);
    else
        display_temp(0);
}
/*刷新湿度图标显示*/
void refresh_hum(void)
{
    if(E53_IA1_Sensor.humidity >= 80 && E53_IA1_Sensor.humidity < 100)
        display_hum(5);
    else if(E53_IA1_Sensor.humidity >= 60 && E53_IA1_Sensor.humidity < 80)
        display_hum(4);
    else if(E53_IA1_Sensor.humidity >= 40 && E53_IA1_Sensor.humidity < 60)
        display_hum(3);
    else if(E53_IA1_Sensor.humidity >= 20 && E53_IA1_Sensor.humidity < 40)
        display_hum(2);
    else if(E53_IA1_Sensor.humidity >= 10 && E53_IA1_Sensor.humidity < 20)
        display_hum(1);
    else
        display_hum(0);
}

/*刷新所有UI逻辑*/
#include "tim.h"
void refresh_all_ui(void)
{
    static uint8_t count = 0 ;

    /*当前光补偿是否打开*/
    if(1 == User_Memory_Para.light_flag)
    {
        display_led(1);
        HAL_GPIO_WritePin(IA1_LED_GPIO_Port, IA1_LED_Pin, GPIO_PIN_SET);
    }
    else
    {
        display_led(0);
        HAL_GPIO_WritePin(IA1_LED_GPIO_Port, IA1_LED_Pin, GPIO_PIN_RESET);
    }

    /*当前风扇是否打开*/
    if(1 == User_Memory_Para.fan)
    {
        display_fan(count);
        count++ ;

        if(count > 3)
            count = 0 ;

        HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    }
    else
    {
        count = 0 ;
        display_fan(4);
        HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
    }

    if(E53_IA1_Sensor.temp >= 80 && E53_IA1_Sensor.temp < 100)
        display_temp(5);
    else if(E53_IA1_Sensor.temp >= 60 && E53_IA1_Sensor.temp < 80)
        display_temp(4);
    else if(E53_IA1_Sensor.temp >= 40 && E53_IA1_Sensor.temp < 60)
        display_temp(3);
    else if(E53_IA1_Sensor.temp >= 20 && E53_IA1_Sensor.temp < 40)
        display_temp(2);
    else if(E53_IA1_Sensor.temp >= 10 && E53_IA1_Sensor.temp < 20)
        display_temp(1);
    else
        display_temp(0);

    if(E53_IA1_Sensor.humidity >= 80 && E53_IA1_Sensor.humidity < 100)
        display_hum(5);
    else if(E53_IA1_Sensor.humidity >= 60 && E53_IA1_Sensor.humidity < 80)
        display_hum(4);
    else if(E53_IA1_Sensor.humidity >= 40 && E53_IA1_Sensor.humidity < 60)
        display_hum(3);
    else if(E53_IA1_Sensor.humidity >= 20 && E53_IA1_Sensor.humidity < 40)
        display_hum(2);
    else if(E53_IA1_Sensor.humidity >= 10 && E53_IA1_Sensor.humidity < 20)
        display_hum(1);
    else
        display_hum(0);

    if(1 == User_Memory_Para.alarm)
    {
        if(E53_IA1_Sensor.temp > TEMP_THRESHOLD || E53_IA1_Sensor.humidity > HUM_THRESHOLD)
            HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
    }
}

void refresh_sd_card_status(void)
{
    char Stroge_Overs_Size[20] = {0};
    uint32_t Total = 0; //读取FLASH总容量
    uint32_t Free = 0; //读取FLASH剩余容量
    f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //获取SD卡总容量和剩余容量
    sprintf(Stroge_Overs_Size, "%04dMB", (int)(float)Free / 1024);
    LCD_Ascii_Show_Para display_stroge_para ;
    display_stroge_para.x = 165 + 24 ;
    display_stroge_para.y = 6 ;
    display_stroge_para.max_width = 50 ;
    display_stroge_para.sizey = 16 ;
    display_stroge_para.bc = WHITE ;
    display_stroge_para.str = Stroge_Overs_Size ;

    /*如果剩余内存小于1G，则显示红色*/
    if((float)Free / 1024 < 1024)
    {
        display_sd_card(1);
        display_stroge_para.fc = RED ;
        lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
    }
    else
    {
        display_sd_card(0);
        display_stroge_para.fc = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
    }
}

/*一次性更新主页面数值和图标显示*/
void update_main_page_data_display(void)
{
    E53_IA1_Sensor.module_init = 0 ;
    /*更新传感器数据显示*/
    display_light_strength_value(E53_IA1_Sensor.light_strength, GREEN, 1);
    display_temp_value(E53_IA1_Sensor.temp, GREEN, 1);
    display_humidity_value(E53_IA1_Sensor.humidity, GREEN, 1);
    E53_IA1_Sensor.module_init = 1 ;
    refresh_sd_card_status();
    display_rt_thread_logo(1);
    refresh_fan();
    refresh_led();
    refresh_temp();
    refresh_hum();
    display_signal_status(0);
    display_server_status(0);

    if(1 == User_Memory_Para.password_enable_flag)
        display_passwd_icon(0);
    else
        display_passwd_icon(1);

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
    update_main_page_data_display();
    lcd_model.lcd_driver->lcd_display_onoff(1);
}


/********************************************按键处理****************************************************/
/*右键处理*/
static void Handler_Main_Page_Right_Key(void)
{
    LCD_Fill_Para status_bar_fill_para = {0, 240, 0, 27, WHITE} ;
    /*确保界面上的数据都隐藏了再跳转到新的页面*/
    Flow_Cursor.flow_cursor = PASSWD_INPUT_PAGE ;
    rt_thread_mdelay(200);
    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(IA1_LED_GPIO_Port, IA1_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_RESET);
    lcd_model.lcd_driver->lcd_display_onoff(0);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    display_main_page_ui(0);
    display_runing_time_font(0);
    display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 0);
    lcd_model.lcd_driver->lcd_fill(status_bar_fill_para);
    lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
    refresh_sd_card_status();

    if(1 == User_Memory_Para.password_enable_flag)
        display_passwd_icon(0);
    else
        display_passwd_icon(1);

    display_signal_status(0);
    display_server_status(0);
    lcd_model.lcd_driver->lcd_display_onoff(1);

    if(1 == User_Memory_Para.password_enable_flag)
        password_input_page_init();
    else
    {
        /*进入配置页面*/
        conf_page_ui.select_item = 0 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
    }
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

