#ifndef __PARA_H
#define __PARA_H
#include "main.h"

/*温度阈值*/
#define TEMP_THRESHOLD  30
/*湿度阈值*/
#define HUM_THRESHOLD   70
/*光强阈值*/
#define LIGHT_THRESHOLD 200

/*保存检测记录的csv文件*/
#define DETECT_CSV 	 "0:BearPi.csv"
/*保存设置系统参数*/
#define SETTING_PARA "0:Para.ini"
/*阈值基址*/
#define TEMP_SENSIVITY_BASE  0
#define HUM_SENSIVITY_BASE   3
#define LIGHT_SENSIVITY_BASE 6

//调试接口
//定义调试版本，此时会打印串口调试信息
#define __DEBUG_VERSION
#ifdef __DEBUG_VERSION
    #define DEBUG(format,...)  printf("[File:%s, Line:%d] "format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG(format,...)
#endif

/*配置参数*/
typedef struct
{
    int8_t alarm ;					/*报警*/
    int8_t fan  ;						/*风扇开关*/
    int8_t sensivity;				/*灵敏度*/
    int8_t debug_flag ;			/*调试标志*/
    int8_t light_flag  ;		/*光补偿标志*/
		int8_t password_enable_flag ;/*密码使能标志*/
    int detect_log_serial_number ; /*检测记录流水号*/
    /*报警阈值参数==>与灵敏度关联*/
    int16_t alarm_threshold[9];		/*报警阈值*/
    int8_t password[4];			/*密码*/
    char wifi_name[32] ;		/*wifi名称*/
    char wifi_password[32];	/*wifi密码*/
    char server_ip[32];			/*服务器ip*/
    int  server_port ;			/*服务端口号*/
} Conf_Para  ;
extern Conf_Para User_Memory_Para ;


/*加载配置文件*/
void Load_Config_ini_File(void);
/*设置阈值*/
void Alarm_Threhold_Setting(void);
/*设置调试*/
void setting_debug(uint8_t status);
/*设置报警*/
void setting_alarm(uint8_t status);
/*设置风扇*/
void setting_fan(uint8_t status);
/*设置光补偿*/
void setting_light(uint8_t status);
/*设置灵敏度*/
void setting_sensivity(uint8_t status);
/*记录流水号保存处理*/
void User_Detect_Log_Save_Process(void);
/*设置密码使能*/
void setting_password_enable(uint8_t status);
/*设置机器密码*/
void setting_device_password(Conf_Para para);


/*用户参数保存处理*/
void User_Para_Save_Process(void);



#endif //__PARA_H

