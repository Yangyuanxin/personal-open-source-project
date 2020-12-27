#include "conf_ui.h"

/**********�Ӳ˵���ʱ����Ҫ�������²���**********/
/*��ʾ*/
#define DISPLAY_START 0
#define DISPLAY_END 	5

//��ʾ���
#define DISPLAY_GAP   12
/**********�Ӳ˵���ʱ����Ҫ�������²���**********/

Conf_Page conf_page_ui ;

/*����ҳ��˵�����ʾ*/
Item Conf_Item_Display[] =
{
    {70, 73 - DISPLAY_GAP,  "ϵͳ����", WHITE, BLACK, 24, 1},		//0		/*�����ֵ===>�͡��С���*/
    {70, 102 - DISPLAY_GAP, "ʱ������", WHITE, BLACK, 24, 1},		//1		/*������LED����ֵ��ʾ*/
    {70, 131 - DISPLAY_GAP, "��������", WHITE, BLACK, 24, 1},		//2		/*������*/
    {70, 160 - DISPLAY_GAP, "������Ϣ", WHITE, BLACK, 24, 1},		//3		/*��ʾ�汾�š�ROM��RAM����Ϣ*/
    {70, 191 - DISPLAY_GAP, "����ģʽ", WHITE, BLACK, 24, 1},		//4		/*����ģʽ===>��ʾ������ʵʱ����*/
};

/*ѡ��˵���Ŀ*/

void select_conf_item(uint8_t item, uint16_t color)
{
    LCD_Ascii_Show_Para disselect_ascii[] =
    {
        {46, 73 - DISPLAY_GAP, 24,  "*", BLACK, BLACK, 24},
        {46, 102 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 131 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 160 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 191 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},

        {46, 73 - DISPLAY_GAP, 24,  "*", BLACK, color, 24},
        {46, 102 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 131 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 160 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 191 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
    };

    for(int i = 0 ; i < 5 ; i++)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[i]);

    if(item == 0)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[5]);
    else if(item == 1)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[6]);
    else if(item == 2)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[7]);
    else if(item == 3)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[8]);
    else if(item == 4)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[9]);
}

//����ҳ���ʼ��
void conf_page_ui_init(uint8_t item)
{
    Flow_Cursor.flow_cursor = CONF_PAGE ;
    conf_page_ui.select_item = item ;

    for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
        display_menu_color_item(Conf_Item_Display, i, WHITE);

    select_conf_item(conf_page_ui.select_item, RED);
}

//����ĳһ������
void enter_conf_item(uint8_t item)
{
    for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
        display_menu_color_item(Conf_Item_Display, i, BLACK);

    switch(item)
    {
    //ϵͳ����
    case 0:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*ϵͳ����ҳ���ʼ��*/
        para_setting_page_init();
        break ;

    //ʱ������
    case 1:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*ʱ��ҳ���ʼ��*/
        datetime_page_init();
        break ;

    //��������
    case 2:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*����ҳ���ʼ��*/
        password_page_init();
        break ;

    //������Ϣ
    case 3:
        select_conf_item(conf_page_ui.select_item, BLACK);
        Version_Info_Page_Init();
        break ;

    //����ģʽ
    case 4:
        plot_page_init();
        break ;

    default:
        break ;
    }
}


//����ҳ�水������
void conf_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
    //�л�����һ��
    case LEFT:
        (conf_page_ui.select_item < DISPLAY_END - 1) ? 	\
        (conf_page_ui.select_item++) : 							\
        (conf_page_ui.select_item = DISPLAY_START);
        select_conf_item(conf_page_ui.select_item, RED);
        break ;

    //����ĳһ��
    case RIGHT:
        lcd_model.lcd_driver->lcd_display_onoff(0);
        enter_conf_item(conf_page_ui.select_item);
        lcd_model.lcd_driver->lcd_display_onoff(1);
        break ;

    //�˳��˵��ص����˵�
    case RIGHT_LONG:
        lcd_model.lcd_driver->lcd_display_onoff(0);
        select_conf_item(conf_page_ui.select_item, BLACK);

        for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
            display_menu_color_item(Conf_Item_Display, i, BLACK);

        Flow_Cursor.flow_cursor = MAIN_PAGE ;
        display_tencent_logo(1);
        Select_Main_Menu_Item(main_page_ui.select_item);
        lcd_model.lcd_driver->lcd_display_onoff(1);
        break ;

    default:
        break ;
    }
}
