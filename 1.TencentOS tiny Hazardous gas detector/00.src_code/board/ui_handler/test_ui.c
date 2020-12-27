#include "test_ui.h"
#include "bsp_bmp.h"



LCD_Bmp_Show_Para test_baseline_ui_bmp[] =
{
    {76, 40, B0_LOGO},
    {76, 40, B1_LOGO},
    {76, 40, B2_LOGO},
};

LCD_Bmp_Show_Para test_detecting_ui_bmp[] =
{
    {76, 40, D0_LOGO},
    {76, 40, D1_LOGO},
    {76, 40, D2_LOGO},
};

LCD_Bmp_Show_Para test_result_ui_bmp[] =
{
    {76, 40, R0_LOGO},
    {76, 40, R1_LOGO},
};

LCD_Chinese_Show_Para test_font[] =
{
    {94, 130, (uint8_t *)"基准", WHITE, BLACK, 24, 1},
    {94, 130, (uint8_t *)"检测", WHITE, BLACK, 24, 1},
    {94, 130, (uint8_t *)"安全", GREEN, BLACK, 24, 1},
    {94, 130, (uint8_t *)"危险", RED, BLACK, 24, 1},
};

LCD_Fill_Para test_fill[] =
{
    /*图标刷黑*/
    {76, 76 + 84, 40, 40 + 84, BLACK},
    /*字体刷黑*/
    {94, 94 + 48, 130, 130 + 24, BLACK},
};

/*刷新动画*/
void icon_reflash(uint8_t status)
{
    if(Sensor_Flow_Cursor.Detect_Step == BASE_LINE)
        lcd_model.lcd_driver->Lcd_show_bmp(test_baseline_ui_bmp[status]);
    else if(Sensor_Flow_Cursor.Detect_Step == DETECTING)
        lcd_model.lcd_driver->Lcd_show_bmp(test_detecting_ui_bmp[status]);
}

/*显示基准1隐藏0*/
void display_base(uint8_t enable)
{
    if(enable == 1)
    {
        lcd_model.lcd_driver->Lcd_show_bmp(test_baseline_ui_bmp[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(test_font[0]);
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_fill(test_fill[0]);
        lcd_model.lcd_driver->lcd_fill(test_fill[1]);
    }
}

/*显示检测1隐藏0*/
void display_detect(uint8_t enable)
{
    if(enable == 1)
    {
        lcd_model.lcd_driver->Lcd_show_bmp(test_detecting_ui_bmp[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(test_font[1]);
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_fill(test_fill[0]);
        lcd_model.lcd_driver->lcd_fill(test_fill[1]);
    }
}

/*显示安全1隐藏0*/
void display_safety(uint8_t enable)
{
    if(enable == 1)
    {
        lcd_model.lcd_driver->Lcd_show_bmp(test_result_ui_bmp[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(test_font[2]);
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_fill(test_fill[0]);
        lcd_model.lcd_driver->lcd_fill(test_fill[1]);
    }
}

/*显示危险1隐藏0*/
void display_danger(uint8_t enable)
{
    if(enable == 1)
    {
        lcd_model.lcd_driver->Lcd_show_bmp(test_result_ui_bmp[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(test_font[3]);
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_fill(test_fill[0]);
        lcd_model.lcd_driver->lcd_fill(test_fill[1]);
    }
}

/*显示进度条框*/
void Display_Process_Bar_Frame(int enable)
{
    LCD_Fill_Para fill_frame ;
    LCD_Rect_Show_Para rect_para ;
    rect_para.x_start = 10 ;
    rect_para.x_end = 230 ;
    rect_para.y_start = 180 ;
    rect_para.y_end = 190;

    fill_frame.x_start = 12 ;
    fill_frame.x_end = 228 ;
    fill_frame.y_start = 182;
    fill_frame.y_end = 188 ;
    fill_frame.color = BLACK ;

    if(enable == 1)
    {
        rect_para.color = BLUE ;
        lcd_model.lcd_driver->lcd_draw_rect(rect_para);
        lcd_model.lcd_driver->lcd_fill(fill_frame);
    }
    else if(enable == 0)
    {
        rect_para.color = BLACK ;
        lcd_model.lcd_driver->lcd_draw_rect(rect_para);
        lcd_model.lcd_driver->lcd_fill(fill_frame);
    }
}

/*显示1隐藏0进度条*/
void Display_Process_Bar(int Process, int enable)
{
    uint8_t buf[] = {20, 40, 80, 100, 120, 140, 160, 180, 200, 228};
    LCD_Fill_Para fill_frame ;
    fill_frame.x_start = 12 ;
    fill_frame.y_start = 182 ;

    if(enable == 1)
    {
        fill_frame.x_end = buf[Process / 10] ;
        fill_frame.y_end = 188 ;
        fill_frame.color = GREEN ;
        lcd_model.lcd_driver->lcd_fill(fill_frame);
    }
    else if(enable == 0)
    {
        Display_Process_Bar_Frame(enable);
        fill_frame.x_end = buf[9] ;
        fill_frame.y_end = 188 ;
        fill_frame.color = BLACK ;
        lcd_model.lcd_driver->lcd_fill(fill_frame);
    }
}

/*显示烟感值*/
void display_smoke_value(int smoke_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para smoke_value_ascii ;
    sprintf(display_buf, "%04dppm", smoke_value);
    smoke_value_ascii.x = SMOKE_X;
    smoke_value_ascii.y = SMOKE_Y;
    smoke_value_ascii.max_width = 100;
    smoke_value_ascii.str = display_buf ;
    smoke_value_ascii.bc  = BLACK;
    smoke_value_ascii.sizey = 24 ;

    /*当打开了调试标志才会显示烟感值*/
    if(User_Memory_Para.debug_flag == 1)
    {
        if(enable == 1)
        {
            smoke_value_ascii.fc  = color ;
            lcd_model.lcd_driver->lcd_show_ascii_str(smoke_value_ascii);
        }
        else if(enable == 0)
        {
            smoke_value_ascii.fc  = BLACK ;
            lcd_model.lcd_driver->lcd_show_ascii_str(smoke_value_ascii);
        }
    }
}

/*测试页面初始化*/
void test_page_init(void)
{
    Flow_Cursor.flow_cursor = TEST_PAGE ;
    /*传感器检测流程初始化*/
    Sensor_Detect_Init();
    /*显示基准*/
    display_base(1);
    display_smoke_value(0, GREEN, 1);
}


/*重新开始测试*/
void Handler_Test_Page_Left_Key(void)
{
    /*传感器检测流程初始化*/
    Sensor_Detect_Init();
    mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
    mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
    Display_Process_Bar(0, 0);
    display_smoke_value(0, BLACK, 0);
    lcd_model.lcd_driver->lcd_fill(test_fill[1]);
    /*显示基准*/
    display_base(1);
}

/*退出*/
void Handler_Test_Page_Right_Key(void)
{
    tos_knl_sched_lock();
    Sensor_Flow_Cursor.Start_Detect = 0 ;
    Sensor_Flow_Cursor.Count_Alarm = 0 ;
    Sensor_Flow_Cursor.Test_Process = 0 ;
    Sensor_Flow_Cursor.Detect_Step = NULL_STATUS ;
    mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
    mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
    lcd_model.lcd_driver->lcd_display_onoff(0);
    Display_Process_Bar(0, 0);
    display_smoke_value(0, BLACK, 0);
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    main_page_ui.select_item = 0;
    Select_Main_Menu_Item(main_page_ui.select_item);
    display_tencent_logo(1);
    lcd_model.lcd_driver->lcd_display_onoff(1);
    tos_knl_sched_unlock();
}


Event_Frame Test_Page_Event[] =
{
    {Handler_Test_Page_Left_Key},
    {NULL},
    {Handler_Test_Page_Right_Key},
    {NULL},
};

/*测试页面事件处理*/
void test_page_process(uint8_t Event_Code)
{
    if(Test_Page_Event[Event_Code - 1].handler_func != NULL)
        Test_Page_Event[Event_Code - 1].handler_func();
}

