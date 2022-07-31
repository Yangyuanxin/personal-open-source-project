#include "datetime_ui.h"
#include "rtc.h"

Item Clock_Item[] =
{
    {CLOCK_TEXT_START_X, CLOCK_TEXT_START_Y, "时间设置", BLACK, BLACK, CLOCK_TEXT_FONT, 1},
    {YEAR_TEXT_START_X, YEAR_TEXT_START_Y, "年", BLACK, BLACK, YEAR_TEXT_FONT, 1},
    {MONTH_TEXT_START_X, MONTH_TEXT_START_Y, "月", BLACK, BLACK, MONTH_TEXT_FONT, 1},
    {DAY_TEXT_START_X, DAY_TEXT_START_Y, "日", BLACK, BLACK, DAY_TEXT_FONT, 1},
    {HOUR_TEXT_START_X, HOUR_TEXT_START_Y, "时", BLACK, BLACK, HOUR_TEXT_FONT, 1},
    {MIN_TEXT_START_X, MIN_TEXT_START_Y, "分", BLACK, BLACK, MIN_TEXT_FONT, 1},
    {SEC_TEXT_START_X, SEC_TEXT_START_Y, "秒", BLACK, BLACK, SEC_TEXT_FONT, 1},
};

Clock_Setting Clock_Para ;
//计算天数算法
static void cal_day(void);

//显示日期和时间
void display_datetime(DateTime_HandleTypeDef item, uint8_t hide)
{
    LCD_Ascii_Show_Para datetime_ascii[] =
    {
        {0, YEAR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {MONTH_TEXT_START_X - 24, MONTH_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {DAY_TEXT_START_X - 24, DAY_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, WHITE, 24},
        {HOUR_TEXT_START_X - 24, HOUR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {MIN_TEXT_START_X - 24, MIN_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {SEC_TEXT_START_X - 24, SEC_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, WHITE, 24},

        {0, YEAR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, BLACK, 24},
        {MONTH_TEXT_START_X - 24, MONTH_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, BLACK, 24},
        {DAY_TEXT_START_X - 24, DAY_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, BLACK, 24},
        {HOUR_TEXT_START_X - 24, HOUR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, BLACK, 24},
        {MIN_TEXT_START_X - 24, MIN_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, BLACK, 24},
        {SEC_TEXT_START_X - 24, SEC_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, BLACK, 24},
    };

    Clock_Para.year   = item.year;
    Clock_Para.month  = item.month;
    Clock_Para.day    = item.day;
    Clock_Para.hour   = item.hour;
    Clock_Para.minute = item.minute;
    Clock_Para.sec    = item.sec;

    if(hide == 0)
    {
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%04d", Clock_Para.year);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[0]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.month);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[1]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.day);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[2]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.hour);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[3]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.minute);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[4]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.sec);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[5]);
    }
    else
    {
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%04d", Clock_Para.year);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[6]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.month);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[7]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.day);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[8]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.hour);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[9]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.minute);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[10]);

        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.sec);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[11]);
    }
}

/*选中具体调整项*/
void select_datatime_item(int item)
{
    LCD_Ascii_Show_Para datetime_ascii[] =
    {
        {0, YEAR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {MONTH_TEXT_START_X - 24, MONTH_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {DAY_TEXT_START_X - 24, DAY_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, WHITE, 24},
        {HOUR_TEXT_START_X - 24, HOUR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {MIN_TEXT_START_X - 24, MIN_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, WHITE, 24},
        {SEC_TEXT_START_X - 24, SEC_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, WHITE, 24},

        {0, YEAR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {MONTH_TEXT_START_X - 24, MONTH_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {DAY_TEXT_START_X - 24, DAY_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, RED, 24},
        {HOUR_TEXT_START_X - 24, HOUR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {MIN_TEXT_START_X - 24, MIN_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {SEC_TEXT_START_X - 24, SEC_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, RED, 24},
    };


    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%04d", Clock_Para.year);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[0]);

    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%02d", Clock_Para.month);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[1]);

    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%02d", Clock_Para.day);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[2]);

    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%02d", Clock_Para.hour);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[3]);

    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%02d", Clock_Para.minute);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[4]);

    memset(Clock_Para.display_buf, 0, 10);
    sprintf(Clock_Para.display_buf, "%02d", Clock_Para.sec);
    lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[5]);

    switch(Clock_Para.select_item)
    {
    case 0:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%04d", Clock_Para.year);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[6]);
        break ;

    case 1:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.month);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[7]);
        break ;

    case 2:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.day);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[8]);
        break ;

    case 3:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.hour);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[9]);
        break ;

    case 4:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.minute);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[10]);
        break ;

    case 5:
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.sec);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[11]);
        break ;

    default:
        break ;
    }
}

void clock_item_display(uint8_t enable)
{
    if(enable == 1)
    {
        display_menu_color_item(Clock_Item, 0, WHITE);

        for(uint8_t i = 1 ; i < 7 ; i++)
            display_menu_color_item(Clock_Item, i, WHITE);

        display_datetime(DateTime_Handler_Info, 0);
    }
    else if(enable == 0)
    {
        display_menu_color_item(Clock_Item, 0, BLACK);

        for(uint8_t i = 1 ; i < 7 ; i++)
            display_menu_color_item(Clock_Item, i, BLACK);

        display_datetime(DateTime_Handler_Info, 1);
    }
}


/*日期时间初始化*/
void datetime_page_init(void)
{
    Flow_Cursor.flow_cursor = DATETIME_PAGE ;
    Clock_Para.select_item = 0 ;
    clock_item_display(1);
}

//跳转到下一设置项
void jump_next_item(void)
{
    switch(Clock_Para.select_item)
    {
    case 0:
        Clock_Para.select_item = 1 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    case 1:
        Clock_Para.select_item = 2 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    case 2:
        Clock_Para.select_item = 3 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    case 3:
        Clock_Para.select_item = 4 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    case 4:
        Clock_Para.select_item = 5 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    case 5:
        Clock_Para.select_item = 0 ;
        select_datatime_item(Clock_Para.select_item);
        break ;

    default:
        break ;
    }
}

//设置日期时间
void setting_datetime(void)
{
    LCD_Ascii_Show_Para datetime_ascii[] =
    {
        {0, YEAR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {MONTH_TEXT_START_X - 24, MONTH_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {DAY_TEXT_START_X - 24, DAY_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, RED, 24},
        {HOUR_TEXT_START_X - 24, HOUR_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {MIN_TEXT_START_X - 24, MIN_TEXT_START_Y, 125, Clock_Para.display_buf, BLACK, RED, 24},
        {SEC_TEXT_START_X - 24, SEC_TEXT_START_Y, 20, Clock_Para.display_buf, BLACK, RED, 24},
    };

    switch(Clock_Para.select_item)
    {
    case 0:
        (Clock_Para.year < 2099) ? (Clock_Para.year++) : (Clock_Para.year = 2020);
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%04d", Clock_Para.year);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[0]);
        break ;

    case 1:
        (Clock_Para.month < 12) ? (Clock_Para.month++) : (Clock_Para.month = 1);
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.month);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[1]);
        break ;

    case 2:
        cal_day();
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.day);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[2]);
        break ;

    case 3:
        (Clock_Para.hour < 23) ? (Clock_Para.hour++) : (Clock_Para.hour = 0);
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.hour);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[3]);
        break ;

    case 4:
        (Clock_Para.minute < 59) ? (Clock_Para.minute++) : (Clock_Para.minute = 0);
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.minute);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[4]);
        break ;

    case 5:
        (Clock_Para.sec < 59) ? (Clock_Para.sec++) : (Clock_Para.sec = 0);
        memset(Clock_Para.display_buf, 0, 10);
        sprintf(Clock_Para.display_buf, "%02d", Clock_Para.sec);
        lcd_model.lcd_driver->lcd_show_ascii_str(datetime_ascii[5]);
        break ;
    }
}
/*日期时间设置页面按键处理*/
void datetime_page_process(uint8_t KeyValue)
{
    switch(KeyValue)
    {
    case LEFT:
        jump_next_item();
        break ;

    case RIGHT:
        setting_datetime();
        break ;

    /*返回配置菜单*/
    case LEFT_LONG:
    case RIGHT_LONG:
        if(HAL_OK == RTC_Set_Date(Clock_Para.year - 2000, Clock_Para.month, Clock_Para.day, RTC_WEEKDAY_MONDAY))
        {
            if(HAL_OK == RTC_Set_Time(Clock_Para.hour, Clock_Para.minute, Clock_Para.sec, 0))
            {
                DEBUG("设置日期时间成功\n");
            }
        }

        clock_item_display(0);
        /*进入配置页面*/
        conf_page_ui.select_item = 1 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

//计算天数算法
static void cal_day(void)
{
    if(IS_LEAP_YEAR(Clock_Para.year))
    {
        switch(Clock_Para.month)
        {
        case Jan:
        case Mar:
        case May:
        case Jul:
        case Aug:
        case Oct:
        case Dec:
            if(Clock_Para.day < MAX_MON_DAY)
                Clock_Para.day++ ;

            break ;

        case Apr:
        case Jun:
        case Sep:
        case Nov:
            if(Clock_Para.day < MIN_MON_DAY)
                Clock_Para.day++ ;

            break ;

        default:
            if(Clock_Para.day < FEB_MON_MAX_DAY)
                Clock_Para.day++ ;

            ;
            break ;

        }
    }
    else
    {
        switch(Clock_Para.month)
        {
        case Jan:
        case Mar:
        case May:
        case Jul:
        case Aug:
        case Oct:
        case Dec:
            if(Clock_Para.day < MAX_MON_DAY)
                Clock_Para.day++ ;

            break ;

        case Apr:
        case Jun:
        case Sep:
        case Nov:
            if(Clock_Para.day < MIN_MON_DAY)
                Clock_Para.day++ ;

            ;
            break ;

        default:
            if(Clock_Para.day < FEB_MON_MIN_DAY)
                Clock_Para.day++ ;

            ;
            break ;
        }
    }
}

