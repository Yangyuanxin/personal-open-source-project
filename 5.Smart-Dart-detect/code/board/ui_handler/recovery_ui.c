#include "recovery_ui.h"

Recovery_Page recovery_page_ui ;


/*��ʾ����ҳ������--״̬��صĽӿ�*/
static void Display_recovery_page_item_font(int item, uint16_t color)
{
    /*�ָ���������ҳ��˵�����ʾ*/
    Item Recovery_Item[] =
    {
        {RECOVERY_ITEM_X, RECOVERY_ITEM_Y, 	"�ָ���������", BLACK, BLACK, RECOVERY_ITEM_FONT, 1},
        {RECOVERY_ITEM_X, RECOVERY_ITEM_Y,  "�ָ���������", WHITE, BLACK, RECOVERY_ITEM_FONT, 1},
    };
    display_menu_color_item(Recovery_Item, item, color);
}

static void Display_recovery_page_status1_font(int item, uint16_t color)
{
    /*�ָ���������ҳ��״̬1��ʾ*/
    Item Recovery_Status1_Item[] =
    {
        {RECOVERY_STATUS1_X, RECOVERY_STATUS1_Y,  "��", BLACK, BLACK, RECOVERY_STATUS1_FONT, 1},
        {RECOVERY_STATUS1_X, RECOVERY_STATUS1_Y,  "��", WHITE, BLACK, RECOVERY_STATUS1_FONT, 1},
    };
    display_menu_color_item(Recovery_Status1_Item, item, color);
}

static void Display_recovery_page_status2_font(int item, uint16_t color)
{
    /*�ָ���������ҳ��״̬2��ʾ*/
    Item Recovery_Status2_Item[] =
    {
        {RECOVERY_STATUS2_X, RECOVERY_STATUS2_Y,  "��", BLACK, BLACK, RECOVERY_STATUS2_FONT, 1},
        {RECOVERY_STATUS2_X, RECOVERY_STATUS2_Y,  "��", WHITE, BLACK, RECOVERY_STATUS2_FONT, 1},
    };
    display_menu_color_item(Recovery_Status2_Item, item, color);
}

/*�ָ��������ó�ʼ��*/
void recovery_page_init(void)
{
    recovery_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = RECOVERY_PAGE ;
    /*��ʾ����*/
    Display_recovery_page_item_font(1, WHITE);
    /*��ʾ״̬1*/
    Display_recovery_page_status1_font(1, RED);
    /*��ʾ״̬2*/
    Display_recovery_page_status2_font(1, WHITE);
}


/*���Ҽ�ѡ��ui�л�*/
void left_right_select(uint8_t KeyValue)
{
    /*��ʾ״̬1*/
    Display_recovery_page_status1_font(1, WHITE);
    /*��ʾ״̬2*/
    Display_recovery_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
    case LEFT:
        (recovery_page_ui.select_item > 0) ? \
        (recovery_page_ui.select_item--) :	\
        (recovery_page_ui.select_item = 0) ;
        Display_recovery_page_status1_font(1, RED);
        break ;

    case RIGHT:
        (recovery_page_ui.select_item < 1) ? \
        (recovery_page_ui.select_item++) :	\
        (recovery_page_ui.select_item = 1) ;
        Display_recovery_page_status2_font(1, RED);
        break ;

    default:
        break ;
    }
}

/*ȷ�ϼ�*/
void recovery_enter_key_handler(uint8_t item)
{
    switch(item)
    {
    /*������һ��ҳ��*/
    case 0:
        /*���ر���/״̬1/״̬2*/
        Display_recovery_page_item_font(0, BLACK);
        Display_recovery_page_status1_font(0, BLACK);
        Display_recovery_page_status2_font(0, BLACK);
        /*����ϵͳ��Ϣ����*/
        Version_Info_Page_Init();
        break ;

    /*�����ָ���������*/
    case 1:
        tos_knl_sched_lock();
        Recovery_Setting();
        DEBUG("�ָ��������óɹ�\n");
        tos_knl_sched_unlock();
        HAL_NVIC_SystemReset();
        break ;

    default:
        break ;
    }
}

/*�ָ���������ҳ�水������*/
void recovery_page_process(uint8_t KeyValue)
{
    /*���Ұ���ѡ����/��,����̰�Ϊ�����˳��ص���һ��*/
    /*��������˻ص���һ���������Ҽ�ȷ��ѡ����*/
    switch(KeyValue)
    {
    case LEFT:
        left_right_select(KeyValue);
        break ;

    case RIGHT:
        left_right_select(KeyValue);
        break ;

    case RIGHT_LONG:
        recovery_enter_key_handler(recovery_page_ui.select_item);
        break ;

    case LEFT_LONG:
        /*���ر���/״̬1/״̬2*/
        Display_recovery_page_item_font(0, BLACK);
        Display_recovery_page_status1_font(0, BLACK);
        Display_recovery_page_status2_font(0, BLACK);
        /*����ϵͳ��Ϣ����*/
        Version_Info_Page_Init();
        break ;

    default:
        break ;
    }
}


