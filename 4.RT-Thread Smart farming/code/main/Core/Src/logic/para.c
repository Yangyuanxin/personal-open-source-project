#include "para.h"
#include "iniparser.h"

dictionary  *Config_ini = NULL;
Conf_Para User_Memory_Para ;


/*Ĭ��ϵͳ������Ϣ*/
char *System_Config_Info =
{
    "[Setting]\n"
    "Alarm_flag=1;\n"								/*��������*/
    "Fan_flag=0;\n"									/*���ȿ���*/
    "Sensivity=1;\n"								/*�����ȼ���  0:�� 1:�� 2:��*/
    "Debug_flag=1;\n"								/*���Ա�־*/
    "Light_flag=0;\n"								/*�ⲹ����־*/
    "Detect_Log_Serial_Number=0;\n"	/*���������ˮ��*/
    "SENSIVITY_TEMP_LOW    = 40;\n"   /*�¶ȵ�������*/  
    "SENSIVITY_TEMP_MEDIUM = 35;\n"   /*�¶���������*/
    "SENSIVITY_TEMP_HIGH   = 25;\n"   /*�¶ȸ�������*/
		"SENSIVITY_HUM_LOW    = 70;\n"   /*ʪ�ȵ�������*/  
    "SENSIVITY_HUM_MEDIUM = 60;\n"   /*ʪ����������*/
    "SENSIVITY_HUM_HIGH   = 50;\n"   /*ʪ�ȸ�������*/
		"SENSIVITY_LIGHT_LOW    = 300;\n"   /*��ǿ��������*/  
    "SENSIVITY_LIGHT_MEDIUM = 250;\n"   /*��ǿ��������*/
    "SENSIVITY_LIGHT_HIGH   = 200;\n"   /*��ǿ��������*/
		"Password_Enable_flag=0;\n"					/*����ʹ�ܱ�־*/
    "PASSWORD0=-0;\n"								/*�����1λ*/
    "PASSWORD1=-0;\n"								/*�����2λ*/
    "PASSWORD2=-0;\n"								/*�����3λ*/
    "PASSWORD3=-0;\n"								/*�����4λ*/
    "WIFI_NAME=602;\n"							/*WIFI�ȵ�*/
    "WIFI_PASSWORD=13602584536;\n"	/*WIFI����*/
    "SERVER_ADDR=183.230.40.33;\n"	/*��������ַ*/
    "SERVER_PORT=80;\n"							/*�������˿ں�*/
};

/*����һ��Ĭ�ϵ������ļ�*/
static void Create_Default_InI_File(void)
{
    retUSER_SYS_CONFIG = f_open(&USER_SYS_CONFIG_File, SETTING_PARA, FA_OPEN_ALWAYS | FA_WRITE);

    if(FR_OK != retUSER_SYS_CONFIG)
    {
        fprintf(stderr, "iniparser: cannot open %s\n", SETTING_PARA);
        return ;
    }

    f_printf(&USER_SYS_CONFIG_File, System_Config_Info);
    f_close(&USER_SYS_CONFIG_File);
}


/*����ϵͳ����*/
static void Load_System_Para(void)
{
    /*����*/
    User_Memory_Para.alarm = iniparser_getint(Config_ini, "Setting:Alarm_flag", -1);

    if(-1 == User_Memory_Para.alarm)
        User_Memory_Para.alarm = 1 ;

    /*����*/
    User_Memory_Para.fan = iniparser_getint(Config_ini, "Setting:Fan_flag", -1);

    if(-1 == User_Memory_Para.fan)
        User_Memory_Para.fan = 0 ;

    /*������*/
    User_Memory_Para.sensivity = iniparser_getint(Config_ini, "Setting:Sensivity", -1);

    if(-1 == User_Memory_Para.sensivity)
        User_Memory_Para.sensivity = 1 ;

    /*���Ա�־*/
    User_Memory_Para.debug_flag = iniparser_getint(Config_ini, "Setting:Debug_flag", -1);

    if(-1 == User_Memory_Para.debug_flag)
        User_Memory_Para.debug_flag = 1 ;

    /*�ⲹ����־*/
    User_Memory_Para.light_flag = iniparser_getint(Config_ini, "Setting:Light_flag", -1);

    if(-1 == User_Memory_Para.light_flag)
        User_Memory_Para.light_flag = 0 ;

    /*����¼��ˮ��*/
    User_Memory_Para.detect_log_serial_number = iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1);

    if(-1 == User_Memory_Para.detect_log_serial_number)
        User_Memory_Para.detect_log_serial_number = 0 ;

    User_Memory_Para.alarm_threshold[0] = \
                                          iniparser_getdouble(Config_ini, "Setting:SENSIVITY_LOW", -1);
    User_Memory_Para.alarm_threshold[1] = \
                                          iniparser_getdouble(Config_ini, "Setting:SENSIVITY_MEDIUM", -1);
    User_Memory_Para.alarm_threshold[2] = \
                                          iniparser_getdouble(Config_ini, "Setting:SENSIVITY_HIGH", -1);

    for(int i = 0 ; i < 3 ; i++)
    {
        if(-1 == User_Memory_Para.alarm_threshold[i])
        {
						/*�¶�*/
            User_Memory_Para.alarm_threshold[0] = 40 ;
            User_Memory_Para.alarm_threshold[1] = 35 ;
            User_Memory_Para.alarm_threshold[2] = 25 ;
						/*ʪ��*/
						User_Memory_Para.alarm_threshold[3] = 70 ;
            User_Memory_Para.alarm_threshold[4] = 60 ;
            User_Memory_Para.alarm_threshold[5] = 50 ;
						/*��ǿ*/
						User_Memory_Para.alarm_threshold[6] = 300 ;
            User_Memory_Para.alarm_threshold[7] = 250 ;
            User_Memory_Para.alarm_threshold[8] = 200 ;
            break ;
        }
    }
		
		/*����ʹ�ܱ�־*/
    User_Memory_Para.password_enable_flag = iniparser_getint(Config_ini, "Setting:Password_Enable_flag", -1);

    if(-1 == User_Memory_Para.password_enable_flag)
        User_Memory_Para.password_enable_flag = 0 ;

    /*��λ����*/
    User_Memory_Para.password[0] = iniparser_getint(Config_ini, "Setting:PASSWORD0", -1);
    User_Memory_Para.password[1] = iniparser_getint(Config_ini, "Setting:PASSWORD1", -1);
    User_Memory_Para.password[2] = iniparser_getint(Config_ini, "Setting:PASSWORD2", -1);
    User_Memory_Para.password[3] = iniparser_getint(Config_ini, "Setting:PASSWORD3", -1);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(User_Memory_Para.password[i] == -1)
        {
            User_Memory_Para.password[0] = 0 ;
            User_Memory_Para.password[1] = 0 ;
            User_Memory_Para.password[2] = 0 ;
            User_Memory_Para.password[3] = 0 ;
            break ;
        }
    }

    /*WIFI����*/
    char *wifi_name = NULL ;
    wifi_name = iniparser_getstring(Config_ini, "Setting:WIFI_NAME", "602");
    memset(User_Memory_Para.wifi_name, 0, strlen(User_Memory_Para.wifi_name));
    memcpy(User_Memory_Para.wifi_name, wifi_name, strlen(wifi_name));
    /*WIFI����*/
    char *wifi_passwd = NULL ;
    wifi_passwd = iniparser_getstring(Config_ini, "Setting:WIFI_PASSWORD", "13602584536");
    memset(User_Memory_Para.wifi_password, 0, strlen(User_Memory_Para.wifi_password));
    memcpy(User_Memory_Para.wifi_password, wifi_passwd, strlen(wifi_passwd));


    /*��������ַ*/
    char *server_addr = NULL ;
    server_addr = iniparser_getstring(Config_ini, "Setting:SERVER_ADDR", "183.230.40.33");
    memset(User_Memory_Para.server_ip, 0, strlen(User_Memory_Para.server_ip));
    memcpy(User_Memory_Para.server_ip, server_addr, strlen(server_addr));
    /*�������˿ں�*/
    User_Memory_Para.server_port = iniparser_getint(Config_ini, "Setting:SERVER_PORT", -1);

    if(-1 == User_Memory_Para.server_port)
        User_Memory_Para.server_port = 80 ;
}

/*���������ļ�*/
void Load_Config_ini_File(void)
{
    Config_ini = iniparser_load(SETTING_PARA);

    /*�����ʧ���ˣ��򴴽�һ��ȫ�µ�ini�����ļ���Ȼ�������ļ����к����Ľ�������*/
    if(NULL == Config_ini)
    {
        DEBUG("cannot parse %s file\n", SETTING_PARA);
        Create_Default_InI_File();
        Config_ini = iniparser_load(SETTING_PARA);

        if(NULL == Config_ini)
        {
            DEBUG("����Ĭ��INI�ļ���������س���\n");
            return  ;
        }
    }

    /*����ϵͳ����*/
    Load_System_Para();
}

/*���õ���*/
void setting_debug(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.debug_flag = status ;
    iniparser_set(Config_ini, "Setting:Debug_flag", buf);
    DEBUG("���õ���:%d\r\n", iniparser_getint(Config_ini, "Setting:Debug_flag", -1));
    User_Para_Save_Process();
}

/*��������*/
void setting_alarm(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.alarm = status ;
    iniparser_set(Config_ini, "Setting:Alarm_flag", buf);
    DEBUG("���ñ���:%d\r\n", iniparser_getint(Config_ini, "Setting:Alarm_flag", -1));
    User_Para_Save_Process();
}

/*���÷�����ʾ*/
void setting_fan(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.fan = status ;
    iniparser_set(Config_ini, "Setting:Fan_flag", buf);
    DEBUG("���÷���:%d\r\n", iniparser_getint(Config_ini, "Setting:Fan_flag", -1));
    User_Para_Save_Process();
}

/*���ùⲹ����ʾ*/
void setting_light(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.light_flag = status ;
    iniparser_set(Config_ini, "Setting:Light_flag", buf);
    DEBUG("���ùⲹ��:%d\r\n", iniparser_getint(Config_ini, "Setting:Light_flag", -1));
    User_Para_Save_Process();
}

/*��������ʹ��*/
void setting_password_enable(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.light_flag = status ;
    iniparser_set(Config_ini, "Setting:Password_Enable_flag", buf);
    DEBUG("��������ʹ��:%d\r\n", iniparser_getint(Config_ini, "Setting:Password_Enable_flag", -1));
    User_Para_Save_Process();
}

/*����������*/
void setting_sensivity(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    iniparser_set(Config_ini, "Setting:Sensivity", buf);
    DEBUG("����������:%d\r\n", iniparser_getint(Config_ini, "Setting:Sensivity", -1));
    User_Para_Save_Process();
}


/*�����豸����*/
void setting_device_password(Conf_Para para)
{
    char buf1[2] = {0}, buf2[2] = {0}, buf3[2] = {0}, buf4[2] = {0};
    sprintf(buf1, "%d", para.password[0]);
    sprintf(buf2, "%d", para.password[1]);
    sprintf(buf3, "%d", para.password[2]);
    sprintf(buf4, "%d", para.password[3]);
    iniparser_set(Config_ini, "Setting:PASSWORD0", buf1);
    iniparser_set(Config_ini, "Setting:PASSWORD1", buf2);
    iniparser_set(Config_ini, "Setting:PASSWORD2", buf3);
    iniparser_set(Config_ini, "Setting:PASSWORD3", buf4);
		/*ʹ������*/
		setting_password_enable(1);
    User_Para_Save_Process();
}


/*������ֵ*/
void Alarm_Threhold_Setting(void)
{
    char buf1[5] = {0};
    char buf2[5] = {0};
    char buf3[5] = {0};
		
		char buf4[5] = {0};
    char buf5[5] = {0};
    char buf6[5] = {0};
		
		char buf7[5] = {0};
    char buf8[5] = {0};
    char buf9[5] = {0};
    sprintf(buf1, "%d", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "%d", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "%d", User_Memory_Para.alarm_threshold[2]);

		sprintf(buf4, "%d", User_Memory_Para.alarm_threshold[3]);
    sprintf(buf5, "%d", User_Memory_Para.alarm_threshold[4]);
    sprintf(buf6, "%d", User_Memory_Para.alarm_threshold[5]);
		
		sprintf(buf7, "%d", User_Memory_Para.alarm_threshold[6]);
    sprintf(buf8, "%d", User_Memory_Para.alarm_threshold[7]);
    sprintf(buf9, "%d", User_Memory_Para.alarm_threshold[8]);
		
    iniparser_set(Config_ini, "Setting:SENSIVITY_TEMP_LOW", buf1);			/*�¶ȵ�������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_TEMP_MEDIUM", buf2);		/*�¶���������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_TEMP_HIGH", buf3);			/*�¶ȸ�������*/
		
		iniparser_set(Config_ini, "Setting:SENSIVITY_HUM_LOW", buf4);				/*ʪ�ȵ�������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_HUM_MEDIUM", buf5);		/*ʪ����������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_HUM_HIGH", buf6);			/*ʪ�ȸ�������*/
		
		iniparser_set(Config_ini, "Setting:SENSIVITY_LIGHT_LOW", buf7);			/*��ǿ��������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_LIGHT_MEDIUM", buf8);	/*��ǿ��������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_LIGHT_HIGH", buf9);		/*��ǿ��������*/
		
    User_Para_Save_Process();
}

/*��¼��ˮ�ű��洦��*/
void User_Detect_Log_Save_Process(void)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", User_Memory_Para.detect_log_serial_number);
    iniparser_set(Config_ini, "Setting:Detect_Log_Serial_Number", buf);
    DEBUG("���ü����ˮ��:%d\r\n", iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1));
    User_Para_Save_Process();
}

/*�û��������洦��*/
void User_Para_Save_Process(void)
{
    //����ϵͳ����
    rt_enter_critical();
    /*write config.ini parse*/
    retUSER_SYS_CONFIG = f_open(&USER_SYS_CONFIG_File, SETTING_PARA, FA_OPEN_EXISTING | FA_WRITE);

    if(FR_OK != retUSER_SYS_CONFIG)
    {
        printf("iniparser: cannot open %s\n", SETTING_PARA);
        return ;
    }

    printf("�������ñ���ɹ�\n");
    iniparser_dump_ini(Config_ini, &USER_SYS_CONFIG_File);
    f_close(&USER_SYS_CONFIG_File);
    iniparser_freedict(Config_ini);
    Load_Config_ini_File();
    //����ϵͳ����
    rt_exit_critical();
}

