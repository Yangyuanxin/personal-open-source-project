#include "para_ui.h"

Para_Setting_Page para_setting_page_ui ;

/*��������ҳ��˵�����ʾ*/
Item Para_Setting_Item[] =
{
    {70, 60,   "ϵͳ����", 	WHITE, BLACK, 24, 1},
    {22, 103,  "����", 			WHITE, BLACK, 24, 1},
    {22, 134,  "��ֵ", 			WHITE, BLACK, 24, 1},
    {22, 163,  "����", 			WHITE, BLACK, 24, 1},
    {22, 192,  "������", 		WHITE, BLACK, 24, 1},
};

/*��������ҳ��״̬��ʾ*/
Item Para_Setting_Status_Item[] =
{
    {139, 103, 	"��", 	WHITE, BLACK, 24, 1},	//0
    {139, 103, 	"��", 	WHITE, BLACK, 24, 1},	//1
    {139, 134,  "��", 	WHITE, BLACK, 24, 1},	//2
    {139, 134,  "��", 	WHITE, BLACK, 24, 1},	//3
    {139, 163,  "��", 	WHITE, BLACK, 24, 1},	//4
    {139, 163,  "��", 	WHITE, BLACK, 24, 1},	//5

    {139, 192,  "��", 	WHITE, BLACK, 24, 1},	//6
    {139, 192,  "��", 	WHITE, BLACK, 24, 1},	//7
    {139, 192,  "��", 	WHITE, BLACK, 24, 1},	//8
};

/*��ʾ����ҳ������--״̬��صĽӿ�*/
static void Display_para_setting_page_item_font(int item, uint16_t color)
{
    display_menu_color_item(Para_Setting_Item, item, color);
}

static void Display_para_setting_page_status_font(int item, uint16_t color)
{
    display_menu_color_item(Para_Setting_Status_Item, item, color);
}

/*����״̬��ʾ*/
void alarm_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(0, BLACK);
    Display_para_setting_page_status_font(1, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(1, color);
    else if(1 == status)
        Display_para_setting_page_status_font(0, color);
}

/*��ֵ״̬��ʾ*/
void value_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(2, BLACK);
    Display_para_setting_page_status_font(3, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(3, color);
    else if(1 == status)
        Display_para_setting_page_status_font(2, color);
}

/*����״̬��ʾ*/
void network_status_display(uint8_t status, uint16_t color)
{
    Display_para_setting_page_status_font(4, BLACK);
    Display_para_setting_page_status_font(5, BLACK);

    if(0 == status)
        Display_para_setting_page_status_font(5, color);
    else if(1 == status)
        Display_para_setting_page_status_font(4, color);
}

/*��������ʾ*/
void sensivity_display(uint8_t status, int value, uint16_t color)
{
    LCD_Ascii_Show_Para ascii_sensivity_str =
    {
        .x = 139 + 40,
        .y = 192,
        .max_width = 50,
        .str = para_setting_page_ui.display_buf,
        .bc = BLACK,
        .fc = WHITE,
        .sizey = 24
    };
    Display_para_setting_page_status_font(6, BLACK);
    Display_para_setting_page_status_font(7, BLACK);
    Display_para_setting_page_status_font(8, BLACK);
    memset(para_setting_page_ui.display_buf, 0, 10);
    sprintf(para_setting_page_ui.display_buf, "%04d", value);

    if(0 == status)
        Display_para_setting_page_status_font(6, color);
    else if(1 == status)
        Display_para_setting_page_status_font(7, color);
    else if(2 == status)
        Display_para_setting_page_status_font(8, color);

    lcd_model.lcd_driver->lcd_show_ascii_str(ascii_sensivity_str);
}

/*����������ʾ1����0*/
void para_item_display(uint8_t enable)
{
    LCD_Ascii_Show_Para ascii_colon_str[] =
    {
        {100, 103, 125, ":", BLACK, WHITE, 24},/*��ʾ����ð��*/
        {100, 134, 125, ":", BLACK, WHITE, 24},/*��ʾ��ֵð��*/
        {100, 163, 125, ":", BLACK, WHITE, 24},/*��ʾ����ð��*/
        {100, 192, 125, ":", BLACK, WHITE, 24},/*��ʾ������ð��*/

        {100, 103, 125, ":", BLACK, BLACK, 24},
        {100, 134, 125, ":", BLACK, BLACK, 24},
        {100, 163, 125, ":", BLACK, BLACK, 24},
        {100, 192, 125, ":", BLACK, BLACK, 24},

        {139 + 40, 192, 50, para_setting_page_ui.display_buf, BLACK, BLACK, 24},
    };

    if(enable == 1)
    {
        /*��ʾ�˵�*/
        for(int i = 0 ; i < 5 ; i++)
            Display_para_setting_page_item_font(i, WHITE);

        /*��ʾð��*/
        for(int i = 0 ; i < 4 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        /*��ʾѡ���Ӧ��*/
        alarm_status_display(para_setting_page_ui.alarm, RED);
        value_status_display(para_setting_page_ui.value, WHITE);
        network_status_display(para_setting_page_ui.network, WHITE);
        sensivity_display(para_setting_page_ui.sensivity, \
                          para_setting_page_ui.sensivity_value[para_setting_page_ui.sensivity], WHITE);
    }
    else if(enable == 0)
    {
        /*���ز˵�*/
        for(int i = 0 ; i < 5 ; i++)
            Display_para_setting_page_item_font(i, BLACK);

        /*����ð��*/
        for(int i = 4 ; i < 8 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        /*����״̬���*/
        for(int i = 0 ; i < 9 ; i++)
            Display_para_setting_page_status_font(i, BLACK);

        /*������������ֵ��ʾ*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[8]);
    }
}



/*����������ʼ��*/
void para_setting_para_init(void)
{
    para_setting_page_ui.select_item = 0 ;
    para_setting_page_ui.alarm = User_Memory_Para.alarm ;
    para_setting_page_ui.value = User_Memory_Para.value ;
    para_setting_page_ui.network = User_Memory_Para.upload_flag ;
    para_setting_page_ui.sensivity = User_Memory_Para.sensivity ;

    for(int i = 0 ; i < 3 ; i++)
        para_setting_page_ui.sensivity_value[i] = User_Memory_Para.alarm_threshold[i];
}

/*��������ҳ���ʼ��*/
void para_setting_page_init(void)
{
    Flow_Cursor.flow_cursor = PARA_PAGE ;
    para_setting_para_init();
    para_item_display(1);

}


/*ѡ�о��������*/
void select_para_item(int item)
{
    alarm_status_display(para_setting_page_ui.alarm, WHITE);
    value_status_display(para_setting_page_ui.value, WHITE);
    network_status_display(para_setting_page_ui.network, WHITE);
    sensivity_display(para_setting_page_ui.sensivity, \
                      para_setting_page_ui.sensivity_value[para_setting_page_ui.sensivity], WHITE);

    /*����*/
    if(0 == item)
        alarm_status_display(para_setting_page_ui.alarm, RED);
    /*��ֵ*/
    else if(1 == item)
        value_status_display(para_setting_page_ui.value, RED);
    /*����*/
    else if(2 == item)
        network_status_display(para_setting_page_ui.network, RED);
    /*������*/
    else if(3 == item)
        sensivity_display(para_setting_page_ui.sensivity, \
                          para_setting_page_ui.sensivity_value[para_setting_page_ui.sensivity], RED);
}

/*��ת����һ����������*/
void jump_next_para_item(void)
{
    switch(para_setting_page_ui.select_item)
    {
    case 0:
        para_setting_page_ui.select_item = 1 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 1:
        para_setting_page_ui.select_item = 2 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 2:
        para_setting_page_ui.select_item = 3 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    case 3:
        para_setting_page_ui.select_item = 0 ;
        select_para_item(para_setting_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

/*���õ�ǰѡ����*/
void setting_current_select_item(void)
{
    switch(para_setting_page_ui.select_item)
    {
    case 0:
        (para_setting_page_ui.alarm < 1) ? (para_setting_page_ui.alarm++) : (para_setting_page_ui.alarm = 0);
        alarm_status_display(para_setting_page_ui.alarm, RED);
        break ;

    case 1:
        (para_setting_page_ui.value < 1) ? (para_setting_page_ui.value++) : (para_setting_page_ui.value = 0);
        value_status_display(para_setting_page_ui.value, RED);
        break ;

    case 2:
        (para_setting_page_ui.network < 1) ? (para_setting_page_ui.network++) : (para_setting_page_ui.network = 0);
        network_status_display(para_setting_page_ui.network, RED);
        break ;

    case 3:
        (para_setting_page_ui.sensivity < 2) ? (para_setting_page_ui.sensivity++) : (para_setting_page_ui.sensivity = 0);
        sensivity_display(para_setting_page_ui.sensivity, \
                          para_setting_page_ui.sensivity_value[para_setting_page_ui.sensivity], RED);
        break ;

    default:
        break ;
    }
}

/*�������沢������һ��ҳ��*/
void para_save_and_return_prev_page(void)
{
    /*�������ò���*/
    setting_alarm(para_setting_page_ui.alarm);
    setting_value(para_setting_page_ui.value);
    setting_network(para_setting_page_ui.network);
    setting_sensivity(para_setting_page_ui.sensivity);
    para_item_display(0);
    conf_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = CONF_PAGE ;
    conf_page_ui_init(conf_page_ui.select_item);
}


/*�л�����һ������*/
void Para_Setting_Page_Left_Key(void)
{
    jump_next_para_item();
}

/*���沢����*/
void Para_Setting_Page_Left_Long_Key(void)
{
    para_save_and_return_prev_page();
}

/*���õ�ǰ��*/
void Para_Setting_Page_Right_Key(void)
{
    setting_current_select_item();
}

/*���沢����*/
void Para_Setting_Page_Right_Long_Key(void)
{
    para_save_and_return_prev_page();
}

/*���������¼�*/
Event_Frame Para_Setting_Page_Event[] =
{
    {Para_Setting_Page_Left_Key},
    {Para_Setting_Page_Left_Long_Key},
    {Para_Setting_Page_Right_Key},
    {Para_Setting_Page_Right_Long_Key},
};

/*�������ð�������*/
void para_setting_page_process(uint8_t KeyValue)
{
    if(Para_Setting_Page_Event[KeyValue - 1].handler_func != NULL)
        Para_Setting_Page_Event[KeyValue - 1].handler_func();
}

