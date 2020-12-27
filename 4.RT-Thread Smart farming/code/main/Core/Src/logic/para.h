#ifndef __PARA_H
#define __PARA_H
#include "main.h"

/*�¶���ֵ*/
#define TEMP_THRESHOLD  30
/*ʪ����ֵ*/
#define HUM_THRESHOLD   70
/*��ǿ��ֵ*/
#define LIGHT_THRESHOLD 200

/*�������¼��csv�ļ�*/
#define DETECT_CSV 	 "0:BearPi.csv"
/*��������ϵͳ����*/
#define SETTING_PARA "0:Para.ini"
/*��ֵ��ַ*/
#define TEMP_SENSIVITY_BASE  0
#define HUM_SENSIVITY_BASE   3
#define LIGHT_SENSIVITY_BASE 6

//���Խӿ�
//������԰汾����ʱ���ӡ���ڵ�����Ϣ
#define __DEBUG_VERSION
#ifdef __DEBUG_VERSION
    #define DEBUG(format,...)  printf("[File:%s, Line:%d] "format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG(format,...)
#endif

/*���ò���*/
typedef struct
{
    int8_t alarm ;					/*����*/
    int8_t fan  ;						/*���ȿ���*/
    int8_t sensivity;				/*������*/
    int8_t debug_flag ;			/*���Ա�־*/
    int8_t light_flag  ;		/*�ⲹ����־*/
		int8_t password_enable_flag ;/*����ʹ�ܱ�־*/
    int detect_log_serial_number ; /*����¼��ˮ��*/
    /*������ֵ����==>�������ȹ���*/
    int16_t alarm_threshold[9];		/*������ֵ*/
    int8_t password[4];			/*����*/
    char wifi_name[32] ;		/*wifi����*/
    char wifi_password[32];	/*wifi����*/
    char server_ip[32];			/*������ip*/
    int  server_port ;			/*����˿ں�*/
} Conf_Para  ;
extern Conf_Para User_Memory_Para ;


/*���������ļ�*/
void Load_Config_ini_File(void);
/*������ֵ*/
void Alarm_Threhold_Setting(void);
/*���õ���*/
void setting_debug(uint8_t status);
/*���ñ���*/
void setting_alarm(uint8_t status);
/*���÷���*/
void setting_fan(uint8_t status);
/*���ùⲹ��*/
void setting_light(uint8_t status);
/*����������*/
void setting_sensivity(uint8_t status);
/*��¼��ˮ�ű��洦��*/
void User_Detect_Log_Save_Process(void);
/*��������ʹ��*/
void setting_password_enable(uint8_t status);
/*���û�������*/
void setting_device_password(Conf_Para para);


/*�û��������洦��*/
void User_Para_Save_Process(void);



#endif //__PARA_H
