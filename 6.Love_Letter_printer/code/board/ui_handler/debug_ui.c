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

/*����ģʽҳ��˵�����ʾ*/
Item Debug_Mode_Item_Display[] =
{
    {DEBUG_X, DEBUG_Y, "����ģʽ", BLACK, BLACK, 24, 1},
    {DEBUG_X, DEBUG_Y, "����ģʽ", WHITE, BLACK, 24, 1},
};

/*����ģʽҳ��״̬1��ʾ*/
Item Debug_Mode_Status1_Item[] =
{
    {DEBUG_STATUS1_X, DEBUG_STATUS1_Y,  "��", BLACK, BLACK, DEBUG_STATUS1_FONT, 1},
    {DEBUG_STATUS1_X, DEBUG_STATUS1_Y,  "��", WHITE, BLACK, DEBUG_STATUS1_FONT, 1},
};

/*����ģʽҳ��״̬2��ʾ*/
Item Debug_Mode_Status2_Item[] =
{
    {DEBUG_STATUS2_X, DEBUG_STATUS2_Y,  "��", BLACK, BLACK, DEBUG_STATUS2_FONT, 1},
    {DEBUG_STATUS2_X, DEBUG_STATUS2_Y,  "��", WHITE, BLACK, DEBUG_STATUS2_FONT, 1},
};

/*����ģʽҳ������--״̬��صĽӿ�*/
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

/*���Ҽ�ѡ��ui�л�*/
void debug_mode_left_right_select(uint8_t KeyValue)
{
    /*��ʾ״̬1*/
    Display_debug_mode_page_status1_font(1, WHITE);
    /*��ʾ״̬2*/
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

/*����ҳ���ʼ��*/
void debug_page_ui_init(uint8_t item)
{
    debug_setting_page_ui.select_item = item ;
    Flow_Cursor.flow_cursor = DEBUG_PAGE;
    /*��ʾ����*/
    Display_debug_mode_page_item_font(1, WHITE);
    /*��ʾ״̬1*/
    Display_debug_mode_page_status1_font(1, RED);
    /*��ʾ״̬2*/
    Display_debug_mode_page_status2_font(1, WHITE);
}

/*����ҳ�水������*/
void debug_page_process(uint8_t KeyValue)
{
    /*���Ұ���ѡ����/��,����̰�Ϊ�����˳��ص���һ��*/
    /*����Enter�˻ص���һ��*/
    switch(KeyValue)
    {
    case LEFT:
        debug_mode_left_right_select(KeyValue);
        break ;

    case RIGHT:
        debug_mode_left_right_select(KeyValue);
        break ;

    case RIGHT_LONG:
        /*���ر���/״̬1/״̬2*/
        Display_debug_mode_page_item_font(0, BLACK);
        Display_debug_mode_page_status1_font(0, BLACK);
        Display_debug_mode_page_status2_font(0, BLACK);
        /*�������ģʽ����*/
        setting_debug(debug_setting_page_ui.select_item);
        /*��������ҳ��*/
        conf_page_ui_init(2);
        break ;

    default:
        break ;
    }
}

