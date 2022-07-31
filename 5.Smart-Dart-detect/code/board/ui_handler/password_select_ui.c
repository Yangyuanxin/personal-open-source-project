#include "password_select_ui.h"

Password_Select_Page password_select_page_ui ;

/*��ʾ����ҳ������--״̬��صĽӿ�*/
static void Display_recovery_page_item_font(int item, uint16_t color)
{
    Item Password_Select_Item[] =
    {
        {PASSWORD_SELECT_ITEM_X, PASSWORD_SELECT_ITEM_Y, 	"��������", BLACK, BLACK, PASSWORD_SELECT_ITEM_FONT, 1},
        {PASSWORD_SELECT_ITEM_X, PASSWORD_SELECT_ITEM_Y,  "��������", WHITE, BLACK, PASSWORD_SELECT_ITEM_FONT, 1},
    };
    display_menu_color_item(Password_Select_Item, item, color);
}

static void Display_Password_Select_page_status1_font(int item, uint16_t color)
{
    /*�ָ���������ҳ��״̬1��ʾ*/
    Item Password_Select_Status1_Item[] =
    {
        {PASSWORD_SELECT_STATUS1_X, PASSWORD_SELECT_STATUS1_Y,  "�ر�����", BLACK, BLACK, PASSWORD_SELECT_STATUS1_FONT, 1},
        {PASSWORD_SELECT_STATUS1_X, PASSWORD_SELECT_STATUS1_Y,  "�ر�����", WHITE, BLACK, PASSWORD_SELECT_STATUS1_FONT, 1},
    };
    display_menu_color_item(Password_Select_Status1_Item, item, color);
}

static void Display_Password_Select_page_status2_font(int item, uint16_t color)
{
    /*�ָ���������ҳ��״̬2��ʾ*/
    Item Password_Select_Status2_Item[] =
    {
        {PASSWORD_SELECT_STATUS2_X, PASSWORD_SELECT_STATUS2_Y,  "�޸�����", BLACK, BLACK, PASSWORD_SELECT_STATUS2_FONT, 1},
        {PASSWORD_SELECT_STATUS2_X, PASSWORD_SELECT_STATUS2_Y,  "�޸�����", WHITE, BLACK, PASSWORD_SELECT_STATUS2_FONT, 1},
    };
    display_menu_color_item(Password_Select_Status2_Item, item, color);
}

/*����ѡ��ҳ���ʼ��*/
void password_select_page_init(void)
{
    password_select_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = PASSWORD_SELECT_PAGE ;
    /*��ʾ����*/
    Display_recovery_page_item_font(1, WHITE);
    /*��ʾ״̬1*/
    Display_Password_Select_page_status1_font(1, RED);
    /*��ʾ״̬2*/
    Display_Password_Select_page_status2_font(1, WHITE);
}

/*���Ҽ�ѡ��ui�л�*/
void password_select_left_right_select(uint8_t KeyValue)
{
    /*��ʾ״̬1*/
    Display_Password_Select_page_status1_font(1, WHITE);
    /*��ʾ״̬2*/
    Display_Password_Select_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
        case LEFT:
            (password_select_page_ui.select_item > 0) ? \
            (password_select_page_ui.select_item--) :	\
            (password_select_page_ui.select_item = 0) ;
            Display_Password_Select_page_status1_font(1, RED);
            break ;

        case RIGHT:
            (password_select_page_ui.select_item < 1) ? \
            (password_select_page_ui.select_item++) :	\
            (password_select_page_ui.select_item = 1) ;
            Display_Password_Select_page_status2_font(1, RED);
            break ;

        default:
            break ;
    }
}

/*ȷ�ϼ�*/
void password_select_enter_key_handler(uint8_t item)
{
    switch(item)
    {
        /*���ص�����ҳ��*/
        case 0:
            /*���ر���/״̬1/״̬2*/
            Display_recovery_page_item_font(0, BLACK);
            Display_Password_Select_page_status1_font(0, BLACK);
            Display_Password_Select_page_status2_font(0, BLACK);
            conf_page_ui.select_item = 2 ;
            conf_page_ui_init(conf_page_ui.select_item);
            setting_password_enable(0);
            break ;

        /*��ת���޸�����*/
        case 1:
						/*���ر���/״̬1/״̬2*/
            Display_recovery_page_item_font(0, BLACK);
            Display_Password_Select_page_status1_font(0, BLACK);
            Display_Password_Select_page_status2_font(0, BLACK);
            /*����ҳ���ʼ��*/
            password_page_init();
            break ;

        default:
            break ;
    }
}

/*����ѡ��ҳ�水������*/
void password_select_page_process(uint8_t KeyValue)
{
    /*���Ұ���ѡ����/��,����̰�Ϊ�����˳��ص���һ��*/
    /*�����Ҽ�ȷ��ѡ����*/
    switch(KeyValue)
    {
        case LEFT:
            password_select_left_right_select(KeyValue);
            break ;

        case RIGHT:
            password_select_left_right_select(KeyValue);
            break ;

        case RIGHT_LONG:
            password_select_enter_key_handler(password_select_page_ui.select_item);
            break ;

        default:
            break ;
    }
}

