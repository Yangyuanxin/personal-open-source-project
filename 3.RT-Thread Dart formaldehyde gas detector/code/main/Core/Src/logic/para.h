#ifndef __PARA_H
#define __PARA_H
#include "main.h"

/*�������¼��csv�ļ�*/
#define DETECT_CSV 	 "0:BearPi.csv"
/*��������ϵͳ����*/
#define SETTING_PARA "0:Para.ini"

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
    int8_t value  ;					/*��ֵ����*/
    int8_t sensivity;				/*������*/
    int8_t debug_flag ;			/*���Ա�־*/
    int8_t upload_flag ;		/*�����ϴ���־*/
    int detect_log_serial_number ; /*����¼��ˮ��*/
    /*������ֵ����==>�������ȹ���*/
    float alarm_threshold[3];		/*������ֵ*/
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
/*������ֵ��ʾ*/
void setting_value(uint8_t status);
/*����������ʾ*/
void setting_network(uint8_t status);
/*����������*/
void setting_sensivity(uint8_t status);
/*��¼��ˮ�ű��洦��*/
void User_Detect_Log_Save_Process(void);
/*���û�������*/
void setting_device_password(Conf_Para para);

/*�û��������洦��*/
void User_Para_Save_Process(void);



#endif //__PARA_H

