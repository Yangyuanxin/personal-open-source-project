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

/*��������ҳ��˵�����ʾ*/
Item Alarm_Setting_Item_Display[] =
{
    {ALARM_SETTING_X, ALARM_SETTING_Y, "��������", BLACK, BLACK, 24, 1},
    {ALARM_SETTING_X, ALARM_SETTING_Y, "��������", WHITE, BLACK, 24, 1},
};

/*��������ҳ��״̬1��ʾ*/
Item Alarm_Setting_Status1_Item[] =
{
    {ALARM_STATUS1_X, ALARM_STATUS1_Y,  "��", BLACK, BLACK, ALARM_STATUS1_FONT, 1},
    {ALARM_STATUS1_X, ALARM_STATUS1_Y,  "��", WHITE, BLACK, ALARM_STATUS1_FONT, 1},
};

/*��������ҳ��״̬2��ʾ*/
Item Alarm_Setting_Status2_Item[] =
{
    {ALARM_STATUS2_X, ALARM_STATUS2_Y,  "��", BLACK, BLACK, ALARM_STATUS2_FONT, 1},
    {ALARM_STATUS2_X, ALARM_STATUS2_Y,  "��", WHITE, BLACK, ALARM_STATUS2_FONT, 1},
};

/*��ʾ����ҳ������--״̬��صĽӿ�*/
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

/*��������ҳ���ʼ��*/
void alarm_setting_page_ui_init(uint8_t item)
{
    alarm_setting_page_ui.select_item = item ;
    Flow_Cursor.flow_cursor = ALARM_SETTING_PAGE;
    /*��ʾ����*/
    Display_alarm_setting_page_item_font(1, WHITE);
    /*��ʾ״̬1*/
    Display_alarm_setting_page_status1_font(1, RED);
    /*��ʾ״̬2*/
    Display_alarm_setting_page_status2_font(1, WHITE);
}

/*���Ҽ�ѡ��ui�л�*/
void left_right_select(uint8_t KeyValue)
{
    /*��ʾ״̬1*/
    Display_alarm_setting_page_status1_font(1, WHITE);
    /*��ʾ״̬2*/
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


/*��������ҳ�水������*/
void alarm_setting_page_process(uint8_t KeyValue)
{
    /*���Ұ���ѡ����/��,����̰�Ϊ�����˳��ص���һ��*/
    /*����Enter�˻ص���һ��*/
    switch(KeyValue)
    {
    case LEFT:
        left_right_select(KeyValue);
        break ;

    case RIGHT:
        left_right_select(KeyValue);
        break ;

    case RIGHT_LONG:
        /*���ر���/״̬1/״̬2*/
        Display_alarm_setting_page_item_font(0, BLACK);
        Display_alarm_setting_page_status1_font(0, BLACK);
        Display_alarm_setting_page_status2_font(0, BLACK);
        /*���汨�����ò���*/
        setting_volume(alarm_setting_page_ui.select_item);
        /*��������ҳ��*/
        conf_page_ui_init(1);
        break ;

    default:
        break ;
    }
}


