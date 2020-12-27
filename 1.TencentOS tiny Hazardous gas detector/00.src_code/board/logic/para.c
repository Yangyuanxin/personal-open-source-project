#include "para.h"
#include "iniparser.h"

dictionary  *Config_ini = NULL;
Conf_Para User_Memory_Para ;

/*Ĭ��ϵͳ������Ϣ*/
char *System_Config_Info =
{
    "[Setting]\n"
    "Alarm_flag=1;\n"								/*��������*/
    "Value_flag=1;\n"								/*��ֵ����*/
    "Sensivity=1;\n"								/*�����ȼ���  0:�� 1:�� 2:��*/
    "Debug_flag=1;\n"								/*���Ա�־*/
    "Upload_flag=0;\n"							/*�����ϴ���־*/
    "Detect_Log_Serial_Number=0;\n"	/*���������ˮ��*/
    "SENSIVITY_LOW    = 1000;\n"    /*��������*/
    "SENSIVITY_MEDIUM = 300;\n"     /*��������*/
    "SENSIVITY_HIGH   = 200;\n"     /*��������*/
    "PASSWORD0=-1;\n"								/*�����1λ*/
    "PASSWORD1=-1;\n"								/*�����2λ*/
    "PASSWORD2=-1;\n"								/*�����3λ*/
    "PASSWORD3=-1;\n"								/*�����4λ*/
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
    uint8_t passwd_init = 0 ;
    /*����*/
    User_Memory_Para.alarm = iniparser_getint(Config_ini, "Setting:Alarm_flag", -1);

    if(-1 == User_Memory_Para.alarm)
        User_Memory_Para.alarm = 1 ;

    /*��ֵ*/
    User_Memory_Para.value = iniparser_getint(Config_ini, "Setting:Value_flag", -1);

    if(-1 == User_Memory_Para.value)
        User_Memory_Para.value = 1 ;

    /*������*/
    User_Memory_Para.sensivity = iniparser_getint(Config_ini, "Setting:Sensivity", -1);

    if(-1 == User_Memory_Para.sensivity)
        User_Memory_Para.sensivity = 1 ;

    /*���Ա�־*/
    User_Memory_Para.debug_flag = iniparser_getint(Config_ini, "Setting:Debug_flag", -1);

    if(-1 == User_Memory_Para.debug_flag)
        User_Memory_Para.debug_flag = 1 ;

    /*�ϴ���־*/
    User_Memory_Para.upload_flag = iniparser_getint(Config_ini, "Setting:Upload_flag", -1);

    if(-1 == User_Memory_Para.upload_flag)
        User_Memory_Para.upload_flag = 0 ;

    DEBUG("����:%d\n", User_Memory_Para.alarm);
    DEBUG("��ֵ:%d\n", User_Memory_Para.value);
    DEBUG("������:%d\n", User_Memory_Para.sensivity);
    DEBUG("���Ա�־:%d\n", User_Memory_Para.debug_flag);
    DEBUG("�ϴ���־:%d\n", User_Memory_Para.upload_flag);

    /*����¼��ˮ��*/
    User_Memory_Para.detect_log_serial_number = iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1);

    if(-1 == User_Memory_Para.detect_log_serial_number)
        User_Memory_Para.detect_log_serial_number = 0 ;

    DEBUG("�����ˮ��:%d\n", User_Memory_Para.detect_log_serial_number);
    User_Memory_Para.alarm_threshold[0] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_LOW", -1);
    User_Memory_Para.alarm_threshold[1] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_MEDIUM", -1);
    User_Memory_Para.alarm_threshold[2] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_HIGH", -1);

    for(int i = 0 ; i < 3 ; i++)
    {
        if(-1 == User_Memory_Para.alarm_threshold[i])
        {
            User_Memory_Para.alarm_threshold[0] = 1000 ;
            User_Memory_Para.alarm_threshold[1] = 300 ;
            User_Memory_Para.alarm_threshold[2] = 200 ;
            break ;
        }
    }

    for(int i = 0 ; i < 3 ; i++)
        DEBUG("������:%d  ��ֵ:%d\n", i, User_Memory_Para.alarm_threshold[i]);

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
            passwd_init = 1 ;
            break ;
        }
    }

    if(passwd_init == 1)
    {
        passwd_init = 0 ;
        DEBUG("��ʼ������:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
    }
    else
    {
        DEBUG("û����ǰ������:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
        /*��û����ǰ��������н���*/
        sample_decode_passwd(User_Memory_Para.password);
        DEBUG("���ܺ������:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
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
    DEBUG("��ǰwifi����:%s\n", User_Memory_Para.wifi_name);
    DEBUG("��ǰwifi����:%s\n", User_Memory_Para.wifi_password);

    /*��������ַ*/
    char *server_addr = NULL ;
    server_addr = iniparser_getstring(Config_ini, "Setting:SERVER_ADDR", "183.230.40.33");
    memset(User_Memory_Para.server_ip, 0, strlen(User_Memory_Para.server_ip));
    memcpy(User_Memory_Para.server_ip, server_addr, strlen(server_addr));
    /*�������˿ں�*/
    User_Memory_Para.server_port = iniparser_getint(Config_ini, "Setting:SERVER_PORT", -1);

    if(-1 == User_Memory_Para.server_port)
        User_Memory_Para.server_port = 80 ;

    DEBUG("��ǰ��������ַ:%s\n", User_Memory_Para.server_ip);
    DEBUG("��ǰ�������˿ں�:%d\n", User_Memory_Para.server_port);
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

    DEBUG("����INI�ļ��ɹ�\n");
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

/*������ֵ��ʾ*/
void setting_value(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.value = status ;
    iniparser_set(Config_ini, "Setting:Value_flag", buf);
    DEBUG("������ֵ:%d\r\n", iniparser_getint(Config_ini, "Setting:Value_flag", -1));
    User_Para_Save_Process();
}

/*����������ʾ*/
void setting_network(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.upload_flag = status ;
    iniparser_set(Config_ini, "Setting:Upload_flag", buf);
    DEBUG("��������:%d\r\n", iniparser_getint(Config_ini, "Setting:Upload_flag", -1));
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
    User_Para_Save_Process();
}

/*������ֵ*/
void Alarm_Threhold_Setting(void)
{
    char buf1[5] = {0};
    char buf2[5] = {0};
    char buf3[5] = {0};
    sprintf(buf1, "%d", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "%d", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "%d", User_Memory_Para.alarm_threshold[2]);

    iniparser_set(Config_ini, "Setting:SENSIVITY_LOW", buf1);		/*��������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_MEDIUM", buf2);	/*��������*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_HIGH", buf3);		/*��������*/
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
    tos_knl_sched_lock();
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
    tos_knl_sched_unlock();
}

