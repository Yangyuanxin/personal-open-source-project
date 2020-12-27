#include "log_ui.h"

/*��¼��ѯ*/
#define LOG_TITLE_X  		67
#define LOG_TITLE_Y	 		40

/*���*/
#define SERIAL_NUM_X		0
#define SERIAL_NUM_Y		90

/*���ʱ��*/
#define DETECT_DATETIME_X	0
#define DETECT_DATETIME_Y	135

/*�����*/
#define DETECT_RESULT_X		0
#define DETECT_RESULT_Y		180

/*�޼�¼*/
#define NO_LOG_TITLE_X 		74
#define NO_LOG_TITLE_Y 		105

/*��¼��ѯҳ��˵�����ʾ*/
Item Log_Item[] =
{
    {LOG_TITLE_X, LOG_TITLE_Y, 				"��¼��ѯ", WHITE, BLACK, 24, 1},		//0
    {SERIAL_NUM_X, SERIAL_NUM_Y, 			"���", 	WHITE, BLACK, 24, 1},		//1
    {DETECT_DATETIME_X, DETECT_DATETIME_Y,  "���ʱ��", WHITE, BLACK, 24, 1},	//2
    {DETECT_RESULT_X, DETECT_RESULT_Y, 		"�����", WHITE, BLACK, 24, 1},	//3

    {LOG_TITLE_X, LOG_TITLE_Y, 				"��¼��ѯ", BLACK, BLACK, 24, 1},		//4
    {SERIAL_NUM_X, SERIAL_NUM_Y, 			"���", 	BLACK, BLACK, 24, 1},		//5
    {DETECT_DATETIME_X, DETECT_DATETIME_Y,  "���ʱ��", BLACK, BLACK, 24, 1},	//6
    {DETECT_RESULT_X, DETECT_RESULT_Y, 		"�����", BLACK, BLACK, 24, 1},	//7
};

/*��¼��ѯҳ��״̬��ʾ*/
Item Log_Status_Item[] =
{
    {NO_LOG_TITLE_X, NO_LOG_TITLE_Y, 		"�޼�¼", WHITE, BLACK, 32, 1},		//0
    {NO_LOG_TITLE_X, NO_LOG_TITLE_Y, 		"�޼�¼", BLACK, BLACK, 32, 1},		//1
};

/*��ʾ��¼ҳ������*/
static void Display_log_setting_page_item_font(int item, uint16_t color)
{
    LCD_Chinese_Show_Para para_ch ;
    para_ch.x = 		Log_Item[item].x;
    para_ch.y = 		Log_Item[item].y;
    para_ch.s =			Log_Item[item].Str;
    para_ch.fc =		Log_Item[item].front_color;
    para_ch.bc =		Log_Item[item].back_color ;
    para_ch.sizey = Log_Item[item].font_num;
    para_ch.mode =	Log_Item[item].mode	;
    lcd_model.lcd_driver->lcd_show_chinese_str(para_ch);
}

LCD_Ascii_Show_Para log_ascii_para[] =
{
    /*��ʾ��š����ʱ�䡢�����ð��*/
    {SERIAL_NUM_X + 48, SERIAL_NUM_Y, 125, ":", BLACK, WHITE, 24},
    {DETECT_DATETIME_X + 96, DETECT_DATETIME_Y, 125, ":", BLACK, WHITE, 24},
    {DETECT_RESULT_X + 96, DETECT_RESULT_Y, 125, ":", BLACK, WHITE, 24},
    /*������š����ʱ�䡢�����ð��*/
    {SERIAL_NUM_X + 48, SERIAL_NUM_Y, 125, ":", BLACK, BLACK, 24},
    {DETECT_DATETIME_X + 96, DETECT_DATETIME_Y, 125, ":", BLACK, BLACK, 24},
    {DETECT_RESULT_X + 96, DETECT_RESULT_Y, 125, ":", BLACK, BLACK, 24},
};

/*��¼ҳ����ʾ1����0*/
void log_item_display(uint8_t enable)
{
    if(enable == 1)
    {
        for(int i = 0 ; i < 4 ; i++)
            Display_log_setting_page_item_font(i, WHITE);

        for(int i = 0 ; i < 3 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(log_ascii_para[i]);
    }
    else if(enable == 0)
    {
        for(int i = 4 ; i < 8 ; i++)
            Display_log_setting_page_item_font(i, BLACK);

        for(int i = 3 ; i < 6 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(log_ascii_para[i]);
    }
}

/*��ʾ�޼�¼*/
void display_no_log(uint8_t enable)
{
    LCD_Chinese_Show_Para para_ch ;

    if(0 == enable)
    {
        para_ch.x = 		Log_Status_Item[1].x;
        para_ch.y = 		Log_Status_Item[1].y;
        para_ch.s =			Log_Status_Item[1].Str;
        para_ch.fc =		Log_Status_Item[1].front_color;
        para_ch.bc =		Log_Status_Item[1].back_color ;
        para_ch.sizey = Log_Status_Item[1].font_num;
        para_ch.mode =	Log_Status_Item[1].mode	;
    }
    else
    {
        para_ch.x = 		Log_Status_Item[0].x;
        para_ch.y = 		Log_Status_Item[0].y;
        para_ch.s =			Log_Status_Item[0].Str;
        para_ch.fc =		Log_Status_Item[0].front_color;
        para_ch.bc =		Log_Status_Item[0].back_color ;
        para_ch.sizey = Log_Status_Item[0].font_num;
        para_ch.mode =	Log_Status_Item[0].mode	;
    }

    lcd_model.lcd_driver->lcd_show_chinese_str(para_ch);
}

/*��ʾ���*/
static void display_log_serial_number(int serial_number, uint8_t enable)
{
    char buf[10] = {0};
    memset(buf, 0, 10);
    sprintf(buf, "%05d", serial_number);
    LCD_Ascii_Show_Para log_serial_para[] = { \
        {SERIAL_NUM_X + 60, SERIAL_NUM_Y, 100, buf, BLACK, WHITE, 24},
        {SERIAL_NUM_X + 60, SERIAL_NUM_Y, 100, buf, BLACK, BLACK, 24},
    };

    if(enable == 1)
        lcd_model.lcd_driver->lcd_show_ascii_str(log_serial_para[0]);
    else if(enable == 0)
        lcd_model.lcd_driver->lcd_show_ascii_str(log_serial_para[1]);
}

/*��ʾ������ں�ʱ��*/
static void display_log_detect_datetime(int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t enable)
{
    char buf[20] = {0};
    memset(buf, 0, 10);
    sprintf(buf, "%02d%02d%02d%02d%02d", year - 2000, month, day, hour, minute);
    LCD_Ascii_Show_Para log_detect_time_para[] = { \
        {DETECT_DATETIME_X + 108, DETECT_DATETIME_Y, 120, buf, BLACK, WHITE, 24},
        {DETECT_DATETIME_X + 108, DETECT_DATETIME_Y, 120, buf, BLACK, BLACK, 24},
    };

    if(enable == 1)
        lcd_model.lcd_driver->lcd_show_ascii_str(log_detect_time_para[0]);
    else if(enable == 0)
        lcd_model.lcd_driver->lcd_show_ascii_str(log_detect_time_para[1]);
}

/*��ʾ�����*/
static void display_log_detect_result(uint8_t result, uint8_t enable)
{
    char buf[10] = {0};
    memset(buf, 0, 10);
    LCD_Fill_Para	para_fill ;
    LCD_Chinese_Show_Para para_ch[] =
    {
        {DETECT_RESULT_X + 108, DETECT_RESULT_Y, (uint8_t *)"��ȫ", GREEN, BLACK, 24, 1},
        {DETECT_RESULT_X + 108, DETECT_RESULT_Y, (uint8_t *)"Σ��", RED, BLACK, 24, 1},
        {DETECT_RESULT_X + 108, DETECT_RESULT_Y, (uint8_t *)"��ȫ", BLACK, BLACK, 24, 0},
        {DETECT_RESULT_X + 108, DETECT_RESULT_Y, (uint8_t *)"Σ��", BLACK, BLACK, 24, 0},
    };
    para_fill.x_start = DETECT_RESULT_X + 108 ;
    para_fill.x_end = DETECT_RESULT_X + 108 + 48;
    para_fill.y_start = DETECT_RESULT_Y;
    para_fill.y_end = DETECT_RESULT_Y + 24;
    para_fill.color = BLACK ;
    lcd_model.lcd_driver->lcd_fill(para_fill);

    if(enable == 1)
    {
        if(result == 0)
            lcd_model.lcd_driver->lcd_show_chinese_str(para_ch[0]);
        else if(result == 1)
            lcd_model.lcd_driver->lcd_show_chinese_str(para_ch[1]);
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(para_ch[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(para_ch[3]);
    }
}

/*��¼��ѯҳ���ʼ��*/
void log_page_init(void)
{
    Flow_Cursor.flow_cursor = LOG_PAGE ;
    /*��ȡ��ǰ������ˮ��*/
    csv_file_record.Current_Serial_Number = User_Memory_Para.detect_log_serial_number ;

    if(0 == csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX)
        csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / DETECT_DATA_INDEX - 1 ;
    else
        csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / DETECT_DATA_INDEX;

    /*���㵱ǰ������λ�ڸ��ļ��ĵڼ���*/
    if((csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX == 0) && (csv_file_record.Current_Serial_Number >= DETECT_DATA_INDEX))
        csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX + DETECT_DATA_INDEX;
    else
        csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX ;

    /*��ǰû���κ�����*/
    if(0 == csv_file_record.Current_Serial_Number)
        display_no_log(1);

    /*��ǰ������*/
    else
    {
        log_item_display(1);
        /*��ʾ���¼��õ���һ�����ݣ���Ҫ����õ���ǰλ���ĸ��ļ��������Լ���ǰλ����һ��*/
        ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);
        /*��ʾ��¼�洢��*/
        display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
        display_log_detect_datetime(csv_file_record.year, csv_file_record.month, csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
        display_log_detect_result(csv_file_record.detect_result, 1);
        /*��ʾ��¼�洢��*/
    }
}

/*�����¼���*/
void Handler_Log_Page_Left_Key(void)
{
    int ret = -1 ;

    if(csv_file_record.Current_Serial_Number != 0)
    {
        (csv_file_record.Current_Serial_Number > 1) ? (csv_file_record.Current_Serial_Number--) :	(csv_file_record.Current_Serial_Number = 1);

        /*���㵱ǰ������λ�ڸ��ļ�������===>������1*/
        if(0 == csv_file_record.Current_Serial_Number % 100)
            csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 - 1 ;
        else
            csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 ;

        /*���㵱ǰ������λ�ڸ��ļ��ĵڼ���*/
        if((csv_file_record.Current_Serial_Number % 100 == 0) && (csv_file_record.Current_Serial_Number >= 100))
            csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 + 100;
        else
            csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 ;

        ret = ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);

        if(ret > 0)
        {
            display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
            display_log_detect_datetime(csv_file_record.year, csv_file_record.month, csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
            display_log_detect_result(csv_file_record.detect_result, 1);
        }
    }
}

/*�����¼�Ҽ�*/
void Handler_Log_Page_Right_Key(void)
{
    int ret = -1 ;

    if(csv_file_record.Current_Serial_Number != 0)
    {
        (csv_file_record.Current_Serial_Number < User_Memory_Para.detect_log_serial_number) ?	(csv_file_record.Current_Serial_Number++) :	(csv_file_record.Current_Serial_Number = User_Memory_Para.detect_log_serial_number);

        /*���㵱ǰ������λ�ڸ��ļ�������*/
        if(0 == csv_file_record.Current_Serial_Number % 100)
            csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 - 1 ;
        else
            csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 ;

        /*���㵱ǰ������λ�ڸ��ļ��ĵڼ���*/
        if((csv_file_record.Current_Serial_Number % 100 == 0) && (csv_file_record.Current_Serial_Number >= 100))
            csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 + 100;
        else
            csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 ;

        ret = ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);

        if(ret > 0)
        {
            display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
            display_log_detect_datetime(csv_file_record.year, csv_file_record.month, csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
            display_log_detect_result(csv_file_record.detect_result, 1);
        }
    }
}

/*�����˻���ҳ���¼ѡ��*/
void Handler_Log_Page_Right_Long_Key(void)
{
    log_item_display(0);

    /*��������޼�¼��ʾ��ҳ�棬�˳�ʱҪ����ѡ��*/
    if(csv_file_record.Current_Serial_Number == 0)
        display_no_log(0);
    /*���ؼ�¼����ѡ��*/
    else
    {
        display_log_serial_number(csv_file_record.Current_Serial_Number, 0);
        display_log_detect_datetime(csv_file_record.year, csv_file_record.month, csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 0);
        display_log_detect_result(csv_file_record.detect_result, 0);
    }

    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    lcd_model.lcd_driver->lcd_display_onoff(0);
    display_tencent_logo(1);
    Select_Main_Menu_Item(main_page_ui.select_item);
    lcd_model.lcd_driver->lcd_display_onoff(1);
}

Event_Frame Log_Page_Event[] =
{
    {Handler_Log_Page_Left_Key},
    {NULL},
    {Handler_Log_Page_Right_Key},
    {Handler_Log_Page_Right_Long_Key},
};


/*��¼ҳ�水������*/
void log_page_process(uint8_t KeyValue)
{
    if(Log_Page_Event[KeyValue - 1].handler_func != NULL)
        Log_Page_Event[KeyValue - 1].handler_func();
}


