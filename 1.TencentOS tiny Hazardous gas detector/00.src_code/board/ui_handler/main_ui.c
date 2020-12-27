#include "main_ui.h"
#include "bsp_bmp.h"

Main_Page_Ui main_page_ui ;
LCD_Fill_Para LOGO_Fill_Para = {55, 183, 45, 183, BLACK} ;
LCD_Ascii_Show_Para datatime_display_para = {70, 5, 170, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24} ;
LCD_Bmp_Show_Para main_logo_para[] =
{
    {55, 45, "0:/UI/main_page/detect_logo.bmp"},
    {55, 45, "0:/UI/main_page/log_logo.bmp"},
    {55, 45, "0:/UI/main_page/conf_logo.bmp"},
};

/*ѡ��˵���*/
void Select_Main_Menu_Item(uint8_t item)
{
    if(0 == item)
        lcd_model.lcd_driver->Lcd_show_bmp(main_logo_para[0]);
    else if(1 == item)
        lcd_model.lcd_driver->Lcd_show_bmp(main_logo_para[1]);
    else if(2 == item)
        lcd_model.lcd_driver->Lcd_show_bmp(main_logo_para[2]);
}

/*��ȡRTCʱ�Ӳ���ʾ*/
void Get_RTC_Date_Time(void)
{
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
    lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
}

/*��ҳ���ʼ��*/
void main_page_init(void)
{
    main_page_ui.select_item = 0;
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    lcd_model.lcd_driver->lcd_display_onoff(0);
    Get_RTC_Date_Time();
    Select_Main_Menu_Item(main_page_ui.select_item);
    lcd_model.lcd_driver->lcd_display_onoff(1);
}


/********************************************��������****************************************************/
/*�������*/
static void Handler_Main_Page_Left_Key(void)
{
    (main_page_ui.select_item < 2) ? \
    (main_page_ui.select_item++) : 	 \
    (main_page_ui.select_item = 0);
    Select_Main_Menu_Item(main_page_ui.select_item);
}

/*�����������*/
static void Handler_Main_Page_Left_Long_Key(void)
{
    sleep_page_init();
}

/*�Ҽ�����*/
static void Handler_Main_Page_Right_Key(void)
{
    lcd_model.lcd_driver->lcd_fill(LOGO_Fill_Para);

    if(0 == main_page_ui.select_item)
    {
        display_tencent_logo(0);
        test_page_init();
    }
    else if(1 == main_page_ui.select_item)
    {
        display_tencent_logo(0);
        log_page_init();
    }
    else if(2 == main_page_ui.select_item)
    {
        display_tencent_logo(0);
        //conf_page_ui_init(0);
        password_input_page_init();
    }
}

Event_Frame Main_Page_Event[] =
{
    {Handler_Main_Page_Left_Key},
    {Handler_Main_Page_Left_Long_Key},
    {Handler_Main_Page_Right_Key},
    {NULL},
};

/*��ҳ���¼�����*/
void main_page_process(uint8_t Event_Code)
{
    if(Main_Page_Event[Event_Code - 1].handler_func != NULL)
        Main_Page_Event[Event_Code - 1].handler_func();
}
/********************************************��������****************************************************/
