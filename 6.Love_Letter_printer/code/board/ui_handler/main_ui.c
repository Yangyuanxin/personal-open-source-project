#include "main_ui.h"
#include "bsp_bmp.h"

extern Dart_Sensor sensor ;
Main_Page_Ui main_page_ui ;
/*WIFI���ӱ�־*/
extern __IO uint8_t wifi_connect_flag  ;
/*���������ӱ�־*/
extern __IO uint8_t server_connect_flag  ;
LCD_Ascii_Show_Para datatime_display_para = {75, 5, 70, DateTime_Handler_Info.DisPlay_DateBuf, WHITE, BLACK, 16} ;

LCD_Chinese_Show_Para main_title_font[] =
{
    {5, 33, (uint8_t *)"���ؼ�ȩ̽��", GREEN, BLACK, 24, 1},
    {5, 33, (uint8_t *)"���ؼ�ȩ̽��", BLACK, BLACK, 24, 1},
};

LCD_Chinese_Show_Para main_status_title_font[] =
{
    {5, 130, (uint8_t *)"����״̬", GREEN, BLACK, 16, 1},
    {5, 130, (uint8_t *)"����״̬", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_work_status_title_font[] =
{
    {85, 130, (uint8_t *)"����", GREEN, BLACK, 16, 1},
    {85, 130, (uint8_t *)"�쳣", RED, BLACK, 16, 1},
    {85, 130, (uint8_t *)"����", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para hide_main_work_status_title_font[] =
{
    {85, 130, (uint8_t *)"����", BLACK, BLACK, 16, 1},
    {85, 130, (uint8_t *)"�쳣", BLACK, BLACK, 16, 1},
    {85, 130, (uint8_t *)"����", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_max_font[] =
{
    {5, 160, (uint8_t *)"���ֵ", GREEN, BLACK, 16, 1},
    {5, 160, (uint8_t *)"���ֵ", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_min_font[] =
{
    {5, 185, (uint8_t *)"��Сֵ", GREEN, BLACK, 16, 1},
    {5, 185, (uint8_t *)"��Сֵ", BLACK, BLACK, 16, 1},
};

LCD_Chinese_Show_Para main_status_runtime_font[] =
{
    {5, 215, (uint8_t *)"����ʱ��", YELLOW, BLACK, 16, 1},
    {5, 215, (uint8_t *)"����ʱ��", BLACK, BLACK, 16, 1},
};

LCD_Bmp_Show_Para passwd_logo_para[] =
{
    {150, 6, "0:/UI/password/0.bmp"},//0
    {150, 6, "0:/UI/password/1.bmp"},//1
};

LCD_Bmp_Show_Para card_logo_para[] =
{
    {170, 6, "0:/UI/card/0.bmp"},//0
    {170, 6, "0:/UI/card/1.bmp"},//1
};

LCD_Bmp_Show_Para siganl_logo_para[] =
{
    {5, 6, "0:/UI/signal/0.bmp"},//0	//���ź�
    {5, 6, "0:/UI/signal/1.bmp"},//1
    {5, 6, "0:/UI/signal/2.bmp"},//2
    {5, 6, "0:/UI/signal/3.bmp"},//3
    {5, 6, "0:/UI/signal/4.bmp"},//4	//���ź�
};

LCD_Bmp_Show_Para server_logo_para[] =
{
    {25, 6, "0:/UI/server/0.bmp"},//0	//��������Ͽ�����
    {25, 6, "0:/UI/server/1.bmp"},//1	//�����ӷ�����
};

LCD_Bmp_Show_Para upload_logo_para[] =
{
    {45, 6, "0:/UI/upload/0.bmp"},//0	//��ֹ�ϴ�
    {45, 6, "0:/UI/upload/1.bmp"},//1	//�ϴ�
};

/*��ʾ�ϴ�״̬*/
void display_upload_status(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(upload_logo_para[status]);
}

/*��ʾ�ź�����*/
void display_signal_status(uint8_t status)
{
    if(status > 4)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(siganl_logo_para[status]);
}

/*��ʾ����������*/
void display_server_status(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(server_logo_para[status]);
}

/*����ͼ*/
void draw_rect_func(uint8_t enable);
/*��ʾ���ֺ���ֵ*/
void display_font_and_value(uint8_t enable);

/*ѡ��˵���*/
void Select_Main_Menu_Item(uint8_t item)
{
    display_main_page_ui(1);
}

/*��ʾSD��״̬*/
void display_sd_card(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(card_logo_para[status]);
}

/*��ʾ����״̬*/
void display_passwd_icon(uint8_t status)
{
    if(status > 1)
        return ;

    lcd_model.lcd_driver->Lcd_show_bmp(passwd_logo_para[status]);
}

/*��ȡRTCʱ�Ӳ���ʾ*/
void Get_RTC_Date_Time(void)
{
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
    lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
}



/*��ʾ��ѶLOGO*/
#include "lcd_spi2_drv.h"
void display_tecent_logo(uint8_t enable)
{
    char kernel_version0[30] = {0};
    char kernel_version1[30] = {0};
    sprintf(kernel_version0, "Power by");
    sprintf(kernel_version1, "TencentOS tiny");
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para0 = {140, 160, 100, kernel_version0, BLACK, BLUE, 16};
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para1 = {120, 185, 120, kernel_version1, BLACK, BLUE, 16};
    LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para0 = {140, 160, 100, kernel_version0, BLACK, BLACK, 16};
    LCD_Ascii_Show_Para hide_ascii_hide_tencent_os_tiny_para1 = {120, 185, 120, kernel_version1, BLACK, BLACK, 16};

    if(1 == enable)
    {
        /*��ʾPower by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para1);
    }
    else
    {
        /*����Power by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para0);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_ascii_hide_tencent_os_tiny_para1);
    }
}

/*��ʾ��ȩֵ*/
void display_hcho_value(double hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 75;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 32 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

/*��ʾ��ȩ�����Сֵ*/
void display_hcho_max_value(double hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 161;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 16 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

void display_hcho_min_value(double hcho_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
    sprintf(display_buf, "%.3f", hcho_value);
    hcho_value_ascii.x = 70;
    hcho_value_ascii.y = 186;
    hcho_value_ascii.max_width = 100;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 16 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = color ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}



/*��ʾ����״̬*/
void display_work_status(double hcho_value, uint8_t enable)
{
    float unit_value ;
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    /*Ĭ��Ϊ��ȫ״̬*/
    static uint8_t cutover_status = 0 ;
    sprintf(display_buf, "%.3f", hcho_value);

    if(enable == 1)
    {
        /*��ǰ��λΪPPM�ļ���*/
        if(User_Memory_Para.unit == 0)
				{
            unit_value = (float)0.062 ;
						/*����*/
						if(hcho_value >= unit_value)
						{
								if(1 == cutover_status)
								{
										cutover_status = 0 ;
										lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[0]);
								}

								lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[1]);
						}
						else
						{
								if(0 == cutover_status)
								{
										cutover_status = 1 ;
										lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[1]);
								}

								lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[0]);
						}
				}
				else
				{
						unit_value = (float)0.046 ;	//0.062 * 0.74666
						/*����*/
						if(hcho_value >= (float)unit_value)
						{
								if(1 == cutover_status)
								{
										cutover_status = 0 ;
										lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[0]);
								}

								lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[1]);
						}
						else
						{
								if(0 == cutover_status)
								{
										cutover_status = 1 ;
										lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[1]);
								}

								lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[0]);
						}
				}

        
    }
    else if(enable == 0)
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_work_status_title_font[2]);
    }
}

/*��ʾ��ȩֵ��λ*/
void display_hcho_unit(uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    LCD_Ascii_Show_Para hcho_value_ascii ;
		if(0 == User_Memory_Para.unit)
			sprintf(display_buf, "PPM");
		else
			sprintf(display_buf, "mg/m3");
    hcho_value_ascii.x = 170;
    //hcho_value_ascii.y = 125;
		hcho_value_ascii.y = 81;
    hcho_value_ascii.max_width = 50;
    hcho_value_ascii.str = display_buf ;
    hcho_value_ascii.bc  = BLACK;
    hcho_value_ascii.sizey = 24 ;

    if(enable == 1)
    {
        hcho_value_ascii.fc  = GREEN ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
    else if(enable == 0)
    {
        hcho_value_ascii.fc  = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(hcho_value_ascii);
    }
}

/*��ʾ����ʱ�䵥λ*/
void display_runing_time_font(uint8_t enable)
{
    LCD_Chinese_Show_Para main_runing_time_font[] =
    {
        {110, 215, (uint8_t *)"ʱ", YELLOW, BLACK, 16, 1},
        {110, 215, (uint8_t *)"ʱ", BLACK, BLACK, 16, 1},

        {150, 215, (uint8_t *)"��", YELLOW, BLACK, 16, 1},
        {150, 215, (uint8_t *)"��", BLACK, BLACK, 16, 1},

        {190, 215, (uint8_t *)"��", YELLOW, BLACK, 16, 1},
        {190, 215, (uint8_t *)"��", BLACK, BLACK, 16, 1},
    };

    if(1 == enable)
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[2]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[4]);
    }
    else
    {
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[3]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_runing_time_font[5]);
    }
}

/*��ʾ����ʱ��*/
void display_runing_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t enable)
{
    char time_buf[5] = {0};
    char min_buf[5] = {0};
    char sec_buf[5] = {0};
    sprintf(time_buf, "%02d", hour);
    sprintf(min_buf, "%02d", min);
    sprintf(sec_buf, "%02d", sec);
    LCD_Ascii_Show_Para hour_display_para = {90, 215, 10, time_buf, BLACK, YELLOW, 16} ;
    LCD_Ascii_Show_Para min_display_para  = {130, 215, 10, min_buf, BLACK, YELLOW, 16} ;
    LCD_Ascii_Show_Para sec_display_para  = {170, 215, 10, sec_buf, BLACK, YELLOW, 16} ;

    LCD_Ascii_Show_Para hide_hour_display_para = {90, 215, 10, time_buf, BLACK, BLACK, 16} ;
    LCD_Ascii_Show_Para hide_min_display_para  = {130, 215, 10, min_buf, BLACK, BLACK, 16} ;
    LCD_Ascii_Show_Para hide_sec_display_para  = {170, 215, 10, sec_buf, BLACK, BLACK, 16} ;

    if(1 == enable)
    {
        lcd_model.lcd_driver->lcd_show_ascii_str(hour_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(min_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(sec_display_para);
    }
    else
    {
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_hour_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_min_display_para);
        lcd_model.lcd_driver->lcd_show_ascii_str(hide_sec_display_para);
    }
}


/*����ͼ*/
void draw_rect_func(uint8_t enable)
{
    if(1 == enable)
    {
        LCD_Rect_Show_Para lcd_rect = {0, 239, 0, 239, WHITE};
        lcd_model.lcd_driver->lcd_draw_rect(lcd_rect);

        LCD_Line_Show_Para line_split0 = {0, 239, 150, 150, WHITE};
        lcd_model.lcd_driver->lcd_draw_line(line_split0);

        LCD_Line_Show_Para line_split1 = {0, 239, 210, 210, WHITE};
        lcd_model.lcd_driver->lcd_draw_line(line_split1);
    }
    else
    {
        LCD_Rect_Show_Para lcd_rect = {0, 239, 0, 239, BLACK};
        lcd_model.lcd_driver->lcd_draw_rect(lcd_rect);

        LCD_Line_Show_Para line_split0 = {0, 239, 150, 150, BLACK};
        lcd_model.lcd_driver->lcd_draw_line(line_split0);

        LCD_Line_Show_Para line_split1 = {0, 239, 210, 210, BLACK};
        lcd_model.lcd_driver->lcd_draw_line(line_split1);
    }
}

/*��ʾ���ֺ���ֵ*/
void display_font_and_value(uint8_t enable)
{
    static uint8_t start_flag = 0 ;
    LCD_Ascii_Show_Para ascii_colon_str[] =
    {
        {75, 130, 125, ":", BLACK, GREEN, 16},
        {60, 160, 125, ":", BLACK, GREEN, 16},
        {60, 185, 125, ":", BLACK, GREEN, 16},
        {75, 215, 125, ":", BLACK, YELLOW, 16},

        {75, 130, 125, ":", BLACK, BLACK, 16},
        {60, 160, 125, ":", BLACK, BLACK, 16},
        {60, 185, 125, ":", BLACK, BLACK, 16},
        {75, 215, 125, ":", BLACK, BLACK, 16},
    };

    if(1 == enable)
    {
        /*��ʾð��*/
        for(int i = 0 ; i < 4 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_title_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_max_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_min_font[0]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[0]);

        /*������ʼ����ʾ������������˸Ч��*/
        if(0 == start_flag)
        {
            start_flag = 1 ;
            display_hcho_value(0.00, GREEN, 1);
						if(User_Memory_Para.value)
						{
							display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
							display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
						}
						else
						{
							display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, BLACK, 0);
							display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, BLACK, 0);
						}
            display_work_status(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, 1);
            display_hcho_unit(1);
            display_tecent_logo(1);
        }
    }
    else
    {
        /*����ð��*/
        for(int i = 4 ; i < 8 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_colon_str[i]);

        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_title_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_max_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_min_font[1]);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_status_runtime_font[1]);

        display_hcho_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, BLACK, 0);
        display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, BLACK, 0);
        display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, BLACK, 0);
        display_work_status(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, 0);
        display_hcho_unit(0);
        display_tecent_logo(0);
    }
}

void refresh_sd_card_status(void)
{
    char Stroge_Overs_Size[20] = {0};
    uint32_t Total = 0; //��ȡFLASH������
    uint32_t Free = 0; //��ȡFLASHʣ������
    f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //��ȡSD����������ʣ������
    sprintf(Stroge_Overs_Size, "%04dMB", (int)(float)Free / 1024);
    LCD_Ascii_Show_Para display_stroge_para ;
    display_stroge_para.x = 165 + 24 ;
    display_stroge_para.y = 6 ;
    display_stroge_para.max_width = 50 ;
    display_stroge_para.sizey = 16 ;
    display_stroge_para.bc = WHITE ;
    display_stroge_para.str = Stroge_Overs_Size ;

    /*���ʣ���ڴ�С��1G������ʾ��ɫ*/
    if((float)Free / 1024 < 1024)
    {
        display_sd_card(1);
        display_stroge_para.fc = RED ;
        lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
    }
    else
    {
        display_sd_card(0);
        display_stroge_para.fc = BLACK ;
        lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
    }
}

/*һ���Ը�����ҳ����ֵ��ʾ*/
void update_main_page_data_display(void)
{
		/*ˢ�²�����������״̬*/
    if(1 == User_Memory_Para.password_enable_flag)
        display_passwd_icon(0);
    else
        display_passwd_icon(1);

    display_tecent_logo(1);
    refresh_sd_card_status();
		
		
		
		/*ˢ��WIFI����״̬*/
		if(0 == wifi_connect_flag)
			display_signal_status(0);
		else
			display_signal_status(4);
		
		/*ˢ�·���������״̬*/
		if(0 == server_connect_flag)
			display_server_status(0);
		else 
			display_server_status(1);
		
		/*ˢ���ϴ�״̬*/
		if(0 == User_Memory_Para.upload_flag)
			display_upload_status(0);
		else
			display_upload_status(1);
		//PPM
		if(0 == User_Memory_Para.unit)
		{
			if(sensor.gas_density > 2)
			{
				sensor.gas_density = 2 ;
				display_hcho_value(sensor.gas_density, RED, 1);
			}
			else
			{
				if(sensor.gas_density > 0.062)
					display_hcho_value(sensor.gas_density, RED, 1);
				else
					display_hcho_value(sensor.gas_density, GREEN, 1);
			}
			display_work_status(sensor.gas_density, 1);
		}
		//mg/m3
		else
		{
			if(sensor.gas_density > 2 * 0.74666)
			{
				sensor.gas_density = 2 *0.74666 ;
				display_hcho_value(sensor.gas_density, RED, 1);
			}
			else
			{
				if(sensor.gas_density > 0.046)
					display_hcho_value(sensor.gas_density*0.74666, RED, 1);
				else
					display_hcho_value(sensor.gas_density*0.74666, GREEN, 1);
			}
			display_work_status(sensor.gas_density*0.74666, 1);
		}
    display_hcho_unit(1);
		//PPM
		if(0 == User_Memory_Para.unit)
		{
			if(User_Memory_Para.value)
			{
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
				display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value, GREEN, 1);
			}
			else
			{
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value,BLACK,0);
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value,BLACK,0);
			}
		}
		//mg/m3
		else
		{
			if(User_Memory_Para.value)
			{
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value*0.74666, GREEN, 1);
				display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value*0.74666, GREEN, 1);
			}
			else
			{
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value,BLACK,0);
				display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value,BLACK,0);
			}
		}
    display_runing_time_font(1);
    display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 1);
}

/*��ʾ��ҳ��UI*/
void display_main_page_ui(uint8_t enable)
{
    if(1 == enable)
    {
        draw_rect_func(1);
        display_font_and_value(1);
    }
    else
    {
        draw_rect_func(0);
        display_font_and_value(0);
    }
}

/*��ҳ���ʼ��*/
void main_page_init(void)
{
    main_page_ui.select_item = 0;
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    lcd_model.lcd_driver->lcd_display_onoff(0);
    Select_Main_Menu_Item(main_page_ui.select_item);
    lcd_model.lcd_driver->lcd_display_onoff(1);
}


/********************************************��������****************************************************/
/*�Ҽ�����*/
static void Handler_Main_Page_Right_Key(void)
{
    /*ȷ�������ϵ����ݶ�����������ת���µ�ҳ��*/
    //����ϵͳ����
    tos_knl_sched_lock();
    display_main_page_ui(0);
    display_runing_time_font(0);
    display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 0);
    lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[0]);
    lcd_model.lcd_driver->lcd_show_chinese_str(hide_main_work_status_title_font[1]);

    if(1 == User_Memory_Para.password_enable_flag)
        password_input_page_init();
    else
    {
        /*��������ҳ��*/
        conf_page_ui.select_item = 0 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
    }

		/*ˢ�²�����������״̬*/
    if(1 == User_Memory_Para.password_enable_flag)
        display_passwd_icon(0);
    else
        display_passwd_icon(1);
		
		/*ˢ��WIFI����״̬*/
		if(0 == wifi_connect_flag)
			display_signal_status(0);
		else
			display_signal_status(4);
		
		/*ˢ�·���������״̬*/
		if(0 == server_connect_flag)
			display_server_status(0);
		else 
			display_server_status(1);
		
		/*ˢ���ϴ�״̬*/
		if(0 == User_Memory_Para.upload_flag)
			display_upload_status(0);
		else
			display_upload_status(1);

    lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
    //����ϵͳ����
    tos_knl_sched_unlock();
}

Event_Frame Main_Page_Event[] =
{
    {NULL},
    {NULL},
    {Handler_Main_Page_Right_Key},
    {NULL},
};

/*��ҳ���¼�����*/
void main_page_process(uint8_t Event_Code)
{
    if(Main_Page_Event[Event_Code - 1].handler_func != NULL)
        Main_Page_Event[Event_Code - 1].handler_func();
}
