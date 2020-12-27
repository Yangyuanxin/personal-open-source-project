#include "threshold_ui.h"

#define THRESHOLD_SETTING_X					70
#define THRESHOLD_SETTING_Y					43

#define SENSIVITY_L_X							30
#define SENSIVITY_L_Y							100

#define SENSIVITY_M_X							30
#define SENSIVITY_M_Y							130

#define SENSIVITY_H_X							30
#define SENSIVITY_H_Y							160

Threshold_Page threshold_page_ui ;

/*��ֵ����ҳ��˵�����ʾ*/
Item Threshold_Item_Display[] =
{
    {THRESHOLD_SETTING_X, THRESHOLD_SETTING_Y, "��ֵ����", BLACK, BLACK, 24, 1},
    {THRESHOLD_SETTING_X, THRESHOLD_SETTING_Y, "��ֵ����", WHITE, BLACK, 24, 1},
};

/*��ֵ����ҳ��˵�����ʾ*/
Item Threshold_Item_Status_Display[] =
{
    {SENSIVITY_L_X, SENSIVITY_L_Y, "�����ȵ�", BLACK, BLACK, 24, 1},
    {SENSIVITY_M_X, SENSIVITY_M_Y, "��������", BLACK, BLACK, 24, 1},
    {SENSIVITY_H_X, SENSIVITY_H_Y, "�����ȸ�", BLACK, BLACK, 24, 1},

    {SENSIVITY_L_X, SENSIVITY_L_Y, "�����ȵ�", WHITE, BLACK, 24, 1},
    {SENSIVITY_M_X, SENSIVITY_M_Y, "��������", WHITE, BLACK, 24, 1},
    {SENSIVITY_H_X, SENSIVITY_H_Y, "�����ȸ�", WHITE, BLACK, 24, 1},

    {SENSIVITY_L_X, SENSIVITY_L_Y, "�����ȵ�", RED, BLACK, 24, 1},
    {SENSIVITY_M_X, SENSIVITY_M_Y, "��������", RED, BLACK, 24, 1},
    {SENSIVITY_H_X, SENSIVITY_H_Y, "�����ȸ�", RED, BLACK, 24, 1},
};

/*ѡ��ǰ��������ֵ��ʾ*/
void select_sensivity_value(uint8_t item)
{
    char buf1[20] = {0};
    char buf2[20] = {0};
    char buf3[20] = {0};
    memset(buf1, 0, 20);
    memset(buf2, 0, 20);
    memset(buf3, 0, 20);
    sprintf(buf1, "(%04d)", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "(%04d)", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "(%04d)", User_Memory_Para.alarm_threshold[2]);

    LCD_ShowCharStr(SENSIVITY_L_X + (24 * 4), SENSIVITY_L_Y, 100, buf1, BLACK, WHITE, 24);
    LCD_ShowCharStr(SENSIVITY_M_X + (24 * 4), SENSIVITY_M_Y, 100, buf2, BLACK, WHITE, 24);
    LCD_ShowCharStr(SENSIVITY_H_X + (24 * 4), SENSIVITY_H_Y, 100, buf3, BLACK, WHITE, 24);

    switch(item)
    {
    case 0:
        LCD_ShowCharStr(SENSIVITY_L_X + (24 * 4), SENSIVITY_L_Y, 100, buf1, BLACK, RED, 24);
        break ;

    case 1:
        LCD_ShowCharStr(SENSIVITY_M_X + (24 * 4), SENSIVITY_M_Y, 100, buf2, BLACK, RED, 24);
        break ;

    case 2:
        LCD_ShowCharStr(SENSIVITY_H_X + (24 * 4), SENSIVITY_H_Y, 100, buf3, BLACK, RED, 24);
        break ;

    default:
        break ;
    }
}

/*ѡ��ǰ��������ʾ*/
void select_sensivity(uint8_t item)
{
    display_menu_item(Threshold_Item_Status_Display, 3);
    display_menu_item(Threshold_Item_Status_Display, 4);
    display_menu_item(Threshold_Item_Status_Display, 5);

    switch(item)
    {
    case 0:
        display_menu_item(Threshold_Item_Status_Display, 6);
        select_sensivity_value(item);
        break ;

    case 1:
        display_menu_item(Threshold_Item_Status_Display, 7);
        select_sensivity_value(item);
        break ;

    case 2:
        display_menu_item(Threshold_Item_Status_Display, 8);
        select_sensivity_value(item);
        break ;

    default:
        break ;
    }
}

//��ֵ����ҳ���ʼ��
void threshold_page_ui_init(uint8_t item)
{
    Flow_Cursor.flow_cursor = THRESHOLD_PAGE ;
    /*��ʾ����*/
    display_menu_item(Threshold_Item_Display, 1);
    display_menu_item(Threshold_Item_Status_Display, 3);
    display_menu_item(Threshold_Item_Status_Display, 4);
    display_menu_item(Threshold_Item_Status_Display, 5);
    threshold_page_ui.select_item = item ;
    select_sensivity(threshold_page_ui.select_item);
}

//��ֵ����ҳ�水������
void threshold_page_process(uint8_t Event_Code)
{
    char buf1[20] = {0};
    char buf2[20] = {0};
    char buf3[20] = {0};
    memset(buf1, 0, 20);
    memset(buf2, 0, 20);
    memset(buf3, 0, 20);
    sprintf(buf1, "(%04d)", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "(%04d)", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "(%04d)", User_Memory_Para.alarm_threshold[2]);


    switch(Event_Code)
    {
    case LEFT:
        (threshold_page_ui.select_item > 0) ?	\
        (threshold_page_ui.select_item--) :		\
        (threshold_page_ui.select_item = 2);
        select_sensivity(threshold_page_ui.select_item);
        break ;

    case RIGHT:
        (threshold_page_ui.select_item < 2) ?	\
        (threshold_page_ui.select_item++) :		\
        (threshold_page_ui.select_item = 0);
        select_sensivity(threshold_page_ui.select_item);
        break ;

    //�˳��ص����ò˵�
    case RIGHT_LONG:
        LCD_ShowCharStr(SENSIVITY_L_X + (24 * 4), SENSIVITY_L_Y, 100, buf1, BLACK, BLACK, 24);
        LCD_ShowCharStr(SENSIVITY_M_X + (24 * 4), SENSIVITY_M_Y, 100, buf2, BLACK, BLACK, 24);
        LCD_ShowCharStr(SENSIVITY_H_X + (24 * 4), SENSIVITY_H_Y, 100, buf3, BLACK, BLACK, 24);
        display_menu_item(Threshold_Item_Display, 0);
        display_menu_item(Threshold_Item_Status_Display, 0);
        display_menu_item(Threshold_Item_Status_Display, 1);
        display_menu_item(Threshold_Item_Status_Display, 2);
        conf_page_ui_init(0);
        /*���������Ȳ���*/
        setting_sensivity(threshold_page_ui.select_item);
        break ;

    default:
        break ;
    }
}


