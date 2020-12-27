#ifndef __SENSOR_FRAME_H
#define __SENSOR_FRAME_H
#include "main.h"

/*检测流程*/
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
    /*传感器检测流程*/
    uint8_t	sensor_detect_step ;
    /*传感器检测流程处理*/
    sensor_handler handler_func ;
} sensor_frame;

/*传感器状态机游标*/
typedef struct
{
    uint8_t Detect_Step ;	/*检测流程*/
    uint8_t Count_AMI		; /*动画计数器*/
    uint8_t Start_Detect;	/*开始测试标志*/
    uint8_t Count_Base  ; /*统计基准次数*/
    uint8_t Count_Alarm ;	/*统计报警次数*/
    uint8_t  Test_Process;	/*测试进度*/
	uint32_t Alarm_Threshold ;	/*报警阈值*/
	uint8_t Is_safety_or_danger ;/*安全/危险*/
} Sensor_Cursor ;
extern Sensor_Cursor Sensor_Flow_Cursor ;




/*传感器检测初始化*/
void Sensor_Detect_Init(void);
/*传感器流程处理*/
int Sensor_Handler(int8_t op, int adc_value);


#endif //__SENSOR_FRAME_H
