#include "alarm_setting_ui.h"

#define ALARM_SETTING_X					70
#define ALARM_SETTING_Y					43

#define ALARM_STATUS1_X					105
#define ALARM_STATUS1_Y    				112
#define ALARM_STATUS1_FONT				24

#define ALARM_STATUS2_X					105
#define ALARM_STATUS2_Y     			145
#define ALARM_STATUS2_FONT				24

Alarm_Setting_Page alarm_setting_page_ui ;

/*报警设置页面菜单项显示*/
Item Alarm_Setting_Item_Display[] =
{
    {ALARM_SETTING_X, ALARM_SETTING_Y, "报警设置", BLACK, BLACK, 24, 1},
    {ALARM_SETTING_X, ALARM_SETTING_Y, "报警设置", WHITE, BLACK, 24, 1},
};

/*报警设置页面状态1显示*/
Item Alarm_Setting_Status1_Item[] =
{
    {ALARM_STATUS1_X, ALARM_STATUS1_Y,  "关", BLACK, BLACK, ALARM_STATUS1_FONT, 1},
    {ALARM_STATUS1_X, ALARM_STATUS1_Y,  "关", WHITE, BLACK, ALARM_STATUS1_FONT, 1},
};

/*报警设置页面状态2显示*/
Item Alarm_Setting_Status2_Item[] =
{
    {ALARM_STATUS2_X, ALARM_STATUS2_Y,  "开", BLACK, BLACK, ALARM_STATUS2_FONT, 1},
    {ALARM_STATUS2_X, ALARM_STATUS2_Y,  "开", WHITE, BLACK, ALARM_STATUS2_FONT, 1},
};

/*显示配置页面文字--状态相关的接口*/
static void Display_alarm_setting_page_item_font(int item, uint16_t color)
{
    LCD_ShowChinese(Alarm_Setting_Item_Display[item].x, Alarm_Setting_Item_Display[item].y,
                    Alarm_Setting_Item_Display[item].Str, color,
                    Alarm_Setting_Item_Display[item].back_color, Alarm_Setting_Item_Display[item].font_num, Alarm_Setting_Item_Display[item].mode);
}

static void Display_alarm_setting_page_status1_font(int item, uint16_t color)
{
    LCD_ShowChinese(Alarm_Setting_Status1_Item[item].x, Alarm_Setting_Status1_Item[item].y,
                    Alarm_Setting_Status1_Item[item].Str, color,
                    Alarm_Setting_Status1_Item[item].back_color, Alarm_Setting_Status1_Item[item].font_num, Alarm_Setting_Status1_Item[item].mode);
}

static void Display_alarm_setting_page_status2_font(int item, uint16_t color)
{
    LCD_ShowChinese(Alarm_Setting_Status2_Item[item].x, Alarm_Setting_Status2_Item[item].y,
                    Alarm_Setting_Status2_Item[item].Str, color,
                    Alarm_Setting_Status2_Item[item].back_color, Alarm_Setting_Status2_Item[item].font_num, Alarm_Setting_Status2_Item[item].mode);
}

/*报警设置页面初始化*/
void alarm_setting_page_ui_init(uint8_t item)
{
    alarm_setting_page_ui.select_item = item ;
    Flow_Cursor.flow_cursor = ALARM_SETTING_PAGE;
    /*显示标题*/
    Display_alarm_setting_page_item_font(1, WHITE);
    /*显示状态1*/
    Display_alarm_setting_page_status1_font(1, RED);
    /*显示状态2*/
    Display_alarm_setting_page_status2_font(1, WHITE);
}

/*左右键选择ui切换*/
void left_right_select(uint8_t KeyValue)
{
    /*显示状态1*/
    Display_alarm_setting_page_status1_font(1, WHITE);
    /*显示状态2*/
    Display_alarm_setting_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
        case LEFT:
            (alarm_setting_page_ui.select_item > 0) ? \
            (alarm_setting_page_ui.select_item--) :	\
            (alarm_setting_page_ui.select_item = 0) ;
            Display_alarm_setting_page_status1_font(1, RED);
            break ;

        case RIGHT:
            (alarm_setting_page_ui.select_item < 1) ? \
            (alarm_setting_page_ui.select_item++) :	\
            (alarm_setting_page_ui.select_item = 1) ;
            Display_alarm_setting_page_status2_font(1, RED);
            break ;

        default:
            break ;
    }
}


/*报警设置页面按键处理*/
void alarm_setting_page_process(uint8_t KeyValue)
{
    /*左右按键选择是/否,如果短按为否则退出回到上一级*/
    /*长按Enter退回到上一级*/
    switch(KeyValue)
    {
        case LEFT:
            left_right_select(KeyValue);
            break ;

        case RIGHT:
            left_right_select(KeyValue);
            break ;

        case RIGHT_LONG:
            /*隐藏标题/状态1/状态2*/
            Display_alarm_setting_page_item_font(0, BLACK);
            Display_alarm_setting_page_status1_font(0, BLACK);
            Display_alarm_setting_page_status2_font(0, BLACK);
            /*保存报警设置参数*/
            setting_volume(alarm_setting_page_ui.select_item);
            /*返回配置页面*/
            conf_page_ui_init(1);
            break ;

        default:
            break ;
    }
}


