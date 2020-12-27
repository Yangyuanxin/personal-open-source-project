#include "para.h"
#include "iniparser.h"

dictionary  *Config_ini = NULL;
Conf_Para User_Memory_Para ;

/*默认系统配置信息*/
char *System_Config_Info =
{
    "[Setting]\n"
    "Alarm_flag=1;\n"								/*报警开关*/
    "Value_flag=1;\n"								/*数值开关*/
    "Sensivity=1;\n"								/*灵敏度级别  0:低 1:中 2:高*/
    "Debug_flag=1;\n"								/*调试标志*/
    "Upload_flag=0;\n"							/*数据上传标志*/
    "Detect_Log_Serial_Number=0;\n"	/*检测数据流水号*/
    "SENSIVITY_LOW    = 1000;\n"    /*低灵敏度*/
    "SENSIVITY_MEDIUM = 300;\n"     /*中灵敏度*/
    "SENSIVITY_HIGH   = 200;\n"     /*高灵敏度*/
    "PASSWORD0=-1;\n"								/*密码第1位*/
    "PASSWORD1=-1;\n"								/*密码第2位*/
    "PASSWORD2=-1;\n"								/*密码第3位*/
    "PASSWORD3=-1;\n"								/*密码第4位*/
    "WIFI_NAME=602;\n"							/*WIFI热点*/
    "WIFI_PASSWORD=13602584536;\n"	/*WIFI密码*/
    "SERVER_ADDR=183.230.40.33;\n"	/*服务器地址*/
    "SERVER_PORT=80;\n"							/*服务器端口号*/
};

/*创建一个默认的配置文件*/
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


/*加载系统参数*/
static void Load_System_Para(void)
{
    uint8_t passwd_init = 0 ;
    /*报警*/
    User_Memory_Para.alarm = iniparser_getint(Config_ini, "Setting:Alarm_flag", -1);

    if(-1 == User_Memory_Para.alarm)
        User_Memory_Para.alarm = 1 ;

    /*数值*/
    User_Memory_Para.value = iniparser_getint(Config_ini, "Setting:Value_flag", -1);

    if(-1 == User_Memory_Para.value)
        User_Memory_Para.value = 1 ;

    /*灵敏度*/
    User_Memory_Para.sensivity = iniparser_getint(Config_ini, "Setting:Sensivity", -1);

    if(-1 == User_Memory_Para.sensivity)
        User_Memory_Para.sensivity = 1 ;

    /*调试标志*/
    User_Memory_Para.debug_flag = iniparser_getint(Config_ini, "Setting:Debug_flag", -1);

    if(-1 == User_Memory_Para.debug_flag)
        User_Memory_Para.debug_flag = 1 ;

    /*上传标志*/
    User_Memory_Para.upload_flag = iniparser_getint(Config_ini, "Setting:Upload_flag", -1);

    if(-1 == User_Memory_Para.upload_flag)
        User_Memory_Para.upload_flag = 0 ;

    DEBUG("报警:%d\n", User_Memory_Para.alarm);
    DEBUG("数值:%d\n", User_Memory_Para.value);
    DEBUG("灵敏度:%d\n", User_Memory_Para.sensivity);
    DEBUG("调试标志:%d\n", User_Memory_Para.debug_flag);
    DEBUG("上传标志:%d\n", User_Memory_Para.upload_flag);

    /*检测记录流水号*/
    User_Memory_Para.detect_log_serial_number = iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1);

    if(-1 == User_Memory_Para.detect_log_serial_number)
        User_Memory_Para.detect_log_serial_number = 0 ;

    DEBUG("检测流水号:%d\n", User_Memory_Para.detect_log_serial_number);
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
        DEBUG("灵敏度:%d  阈值:%d\n", i, User_Memory_Para.alarm_threshold[i]);

    /*四位密码*/
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
        DEBUG("初始化密码:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
    }
    else
    {
        DEBUG("没解密前的密码:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
        /*对没解密前的密码进行解密*/
        sample_decode_passwd(User_Memory_Para.password);
        DEBUG("解密后的密码:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
              User_Memory_Para.password[2], User_Memory_Para.password[3]);
    }

    /*WIFI名称*/
    char *wifi_name = NULL ;
    wifi_name = iniparser_getstring(Config_ini, "Setting:WIFI_NAME", "602");
    memset(User_Memory_Para.wifi_name, 0, strlen(User_Memory_Para.wifi_name));
    memcpy(User_Memory_Para.wifi_name, wifi_name, strlen(wifi_name));
    /*WIFI密码*/
    char *wifi_passwd = NULL ;
    wifi_passwd = iniparser_getstring(Config_ini, "Setting:WIFI_PASSWORD", "13602584536");
    memset(User_Memory_Para.wifi_password, 0, strlen(User_Memory_Para.wifi_password));
    memcpy(User_Memory_Para.wifi_password, wifi_passwd, strlen(wifi_passwd));
    DEBUG("当前wifi名称:%s\n", User_Memory_Para.wifi_name);
    DEBUG("当前wifi密码:%s\n", User_Memory_Para.wifi_password);

    /*服务器地址*/
    char *server_addr = NULL ;
    server_addr = iniparser_getstring(Config_ini, "Setting:SERVER_ADDR", "183.230.40.33");
    memset(User_Memory_Para.server_ip, 0, strlen(User_Memory_Para.server_ip));
    memcpy(User_Memory_Para.server_ip, server_addr, strlen(server_addr));
    /*服务器端口号*/
    User_Memory_Para.server_port = iniparser_getint(Config_ini, "Setting:SERVER_PORT", -1);

    if(-1 == User_Memory_Para.server_port)
        User_Memory_Para.server_port = 80 ;

    DEBUG("当前服务器地址:%s\n", User_Memory_Para.server_ip);
    DEBUG("当前服务器端口号:%d\n", User_Memory_Para.server_port);
}

/*加载配置文件*/
void Load_Config_ini_File(void)
{
    Config_ini = iniparser_load(SETTING_PARA);

    /*如果打开失败了，则创建一个全新的ini配置文件，然后打开这个文件进行后续的解析工作*/
    if(NULL == Config_ini)
    {
        DEBUG("cannot parse %s file\n", SETTING_PARA);
        Create_Default_InI_File();
        Config_ini = iniparser_load(SETTING_PARA);

        if(NULL == Config_ini)
        {
            DEBUG("创建默认INI文件后继续加载出错\n");
            return  ;
        }
    }

    DEBUG("加载INI文件成功\n");
    /*加载系统参数*/
    Load_System_Para();
}

/*设置调试*/
void setting_debug(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.debug_flag = status ;
    iniparser_set(Config_ini, "Setting:Debug_flag", buf);
    DEBUG("设置调试:%d\r\n", iniparser_getint(Config_ini, "Setting:Debug_flag", -1));
    User_Para_Save_Process();
}

/*设置音量*/
void setting_alarm(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.alarm = status ;
    iniparser_set(Config_ini, "Setting:Alarm_flag", buf);
    DEBUG("设置报警:%d\r\n", iniparser_getint(Config_ini, "Setting:Alarm_flag", -1));
    User_Para_Save_Process();
}

/*设置数值显示*/
void setting_value(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.value = status ;
    iniparser_set(Config_ini, "Setting:Value_flag", buf);
    DEBUG("设置数值:%d\r\n", iniparser_getint(Config_ini, "Setting:Value_flag", -1));
    User_Para_Save_Process();
}

/*设置网络显示*/
void setting_network(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.upload_flag = status ;
    iniparser_set(Config_ini, "Setting:Upload_flag", buf);
    DEBUG("设置网络:%d\r\n", iniparser_getint(Config_ini, "Setting:Upload_flag", -1));
    User_Para_Save_Process();
}

/*设置灵敏度*/
void setting_sensivity(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    iniparser_set(Config_ini, "Setting:Sensivity", buf);
    DEBUG("设置灵敏度:%d\r\n", iniparser_getint(Config_ini, "Setting:Sensivity", -1));
    User_Para_Save_Process();
}


/*设置设备密码*/
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

/*设置阈值*/
void Alarm_Threhold_Setting(void)
{
    char buf1[5] = {0};
    char buf2[5] = {0};
    char buf3[5] = {0};
    sprintf(buf1, "%d", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "%d", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "%d", User_Memory_Para.alarm_threshold[2]);

    iniparser_set(Config_ini, "Setting:SENSIVITY_LOW", buf1);		/*低灵敏度*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_MEDIUM", buf2);	/*中灵敏度*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_HIGH", buf3);		/*高灵敏度*/
    User_Para_Save_Process();
}

/*记录流水号保存处理*/
void User_Detect_Log_Save_Process(void)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", User_Memory_Para.detect_log_serial_number);
    iniparser_set(Config_ini, "Setting:Detect_Log_Serial_Number", buf);
    DEBUG("设置检测流水号:%d\r\n", iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1));
    User_Para_Save_Process();
}

/*用户参数保存处理*/
void User_Para_Save_Process(void)
{
    //锁定系统调度
    tos_knl_sched_lock();
    /*write config.ini parse*/
    retUSER_SYS_CONFIG = f_open(&USER_SYS_CONFIG_File, SETTING_PARA, FA_OPEN_EXISTING | FA_WRITE);

    if(FR_OK != retUSER_SYS_CONFIG)
    {
        printf("iniparser: cannot open %s\n", SETTING_PARA);
        return ;
    }

    printf("参数设置保存成功\n");
    iniparser_dump_ini(Config_ini, &USER_SYS_CONFIG_File);
    f_close(&USER_SYS_CONFIG_File);
    iniparser_freedict(Config_ini);
    Load_Config_ini_File();
    //解锁系统调度
    tos_knl_sched_unlock();
}

