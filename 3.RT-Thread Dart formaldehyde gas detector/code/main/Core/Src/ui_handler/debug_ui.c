#include "debug_ui.h"

#define DEBUG_X					70
#define DEBUG_Y					43

#define DEBUG_STATUS1_X					105
#define DEBUG_STATUS1_Y    				112
#define DEBUG_STATUS1_FONT				24

#define DEBUG_STATUS2_X					105
#define DEBUG_STATUS2_Y     			145
#define DEBUG_STATUS2_FONT				24

Debug_Setting_Page debug_setting_page_ui  ;

/*调试模式页面菜单项显示*/
Item Debug_Mode_Item_Display[] =
{
    {DEBUG_X, DEBUG_Y, "调试模式", BLACK, BLACK, 24, 1},
    {DEBUG_X, DEBUG_Y, "调试模式", WHITE, BLACK, 24, 1},
};

/*调试模式页面状态1显示*/
Item Debug_Mode_Status1_Item[] =
{
    {DEBUG_STATUS1_X, DEBUG_STATUS1_Y,  "关", BLACK, BLACK, DEBUG_STATUS1_FONT, 1},
    {DEBUG_STATUS1_X, DEBUG_STATUS1_Y,  "关", WHITE, BLACK, DEBUG_STATUS1_FONT, 1},
};

/*调试模式页面状态2显示*/
Item Debug_Mode_Status2_Item[] =
{
    {DEBUG_STATUS2_X, DEBUG_STATUS2_Y,  "开", BLACK, BLACK, DEBUG_STATUS2_FONT, 1},
    {DEBUG_STATUS2_X, DEBUG_STATUS2_Y,  "开", WHITE, BLACK, DEBUG_STATUS2_FONT, 1},
};

/*调试模式页面文字--状态相关的接口*/
static void Display_debug_mode_page_item_font(int item, uint16_t color)
{
    LCD_ShowChinese(Debug_Mode_Item_Display[item].x, Debug_Mode_Item_Display[item].y,
                    Debug_Mode_Item_Display[item].Str, color,
                    Debug_Mode_Item_Display[item].back_color, Debug_Mode_Item_Display[item].font_num, Debug_Mode_Item_Display[item].mode);
}

static void Display_debug_mode_page_status1_font(int item, uint16_t color)
{
    LCD_ShowChinese(Debug_Mode_Status1_Item[item].x, Debug_Mode_Status1_Item[item].y,
                    Debug_Mode_Status1_Item[item].Str, color,
                    Debug_Mode_Status1_Item[item].back_color, Debug_Mode_Status1_Item[item].font_num, Debug_Mode_Status1_Item[item].mode);
}

static void Display_debug_mode_page_status2_font(int item, uint16_t color)
{
    LCD_ShowChinese(Debug_Mode_Status2_Item[item].x, Debug_Mode_Status2_Item[item].y,
                    Debug_Mode_Status2_Item[item].Str, color,
                    Debug_Mode_Status2_Item[item].back_color, Debug_Mode_Status2_Item[item].font_num, Debug_Mode_Status2_Item[item].mode);
}

/*左右键选择ui切换*/
void debug_mode_left_right_select(uint8_t KeyValue)
{
    /*显示状态1*/
    Display_debug_mode_page_status1_font(1, WHITE);
    /*显示状态2*/
    Display_debug_mode_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
        case LEFT:
            (debug_setting_page_ui.select_item > 0) ? \
            (debug_setting_page_ui.select_item--) :	\
            (debug_setting_page_ui.select_item = 0) ;
            Display_debug_mode_page_status1_font(1, RED);
            break ;

        case RIGHT:
            (debug_setting_page_ui.select_item < 1) ? \
            (debug_setting_page_ui.select_item++) :	\
            (debug_setting_page_ui.select_item = 1) ;
            Display_debug_mode_page_status2_font(1, RED);
            break ;

        default:
            break ;
    }
}

/*调试页面初始化*/
void debug_page_ui_init(uint8_t item)
{
    debug_setting_page_ui.select_item = item ;
    Flow_Cursor.flow_cursor = DEBUG_PAGE;
    /*显示标题*/
    Display_debug_mode_page_item_font(1, WHITE);
    /*显示状态1*/
    Display_debug_mode_page_status1_font(1, RED);
    /*显示状态2*/
    Display_debug_mode_page_status2_font(1, WHITE);
}

/*调试页面按键处理*/
void debug_page_process(uint8_t KeyValue)
{
    /*左右按键选择是/否,如果短按为否则退出回到上一级*/
    /*长按Enter退回到上一级*/
    switch(KeyValue)
    {
        case LEFT:
            debug_mode_left_right_select(KeyValue);
            break ;

        case RIGHT:
            debug_mode_left_right_select(KeyValue);
            break ;

        case RIGHT_LONG:
            /*隐藏标题/状态1/状态2*/
            Display_debug_mode_page_item_font(0, BLACK);
            Display_debug_mode_page_status1_font(0, BLACK);
            Display_debug_mode_page_status2_font(0, BLACK);
            /*保存调试模式参数*/
            setting_debug(debug_setting_page_ui.select_item);
            /*返回配置页面*/
            conf_page_ui_init(2);
            break ;

        default:
            break ;
    }
}

