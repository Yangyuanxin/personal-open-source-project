#ifndef __SENSOR_FRAME_H
#define __SENSOR_FRAME_H
#include "main.h"

/*�������*/
enum
{
    BASE_LINE_STEP = 0,
    DETECTING_STEP,
    DETECT_SAFETY_STEP,
    DETECT_DANGER_STEP
};

typedef void (*sensor_handler)(int);
typedef struct
{
    /*�������������*/
    uint8_t	sensor_detect_step ;
    /*������������̴���*/
    sensor_handler handler_func ;
} sensor_frame;

/*������״̬���α�*/
typedef struct
{
    uint8_t Detect_Step ;	/*�������*/
    uint8_t Count_AMI		; /*����������*/
    uint8_t Start_Detect;	/*��ʼ���Ա�־*/
    uint8_t Count_Base  ; /*ͳ�ƻ�׼����*/
    uint8_t Count_Alarm ;	/*ͳ�Ʊ�������*/
    uint8_t  Test_Process;	/*���Խ���*/
	uint32_t Alarm_Threshold ;	/*������ֵ*/
	uint8_t Is_safety_or_danger ;/*��ȫ/Σ��*/
} Sensor_Cursor ;
extern Sensor_Cursor Sensor_Flow_Cursor ;




/*����������ʼ��*/
void Sensor_Detect_Init(void);
/*���������̴���*/
int Sensor_Handler(int8_t op, int adc_value);


#endif //__SENSOR_FRAME_H
