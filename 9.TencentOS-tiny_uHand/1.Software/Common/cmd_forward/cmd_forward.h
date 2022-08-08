#ifndef __CMD_FORWARD_H
#define __CMD_FORWARD_H
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "uart.h"

#define SERVO_NUM 5
#define CMD_MULT_SERVO_MOVE 3
#define MAX_ARGS_LENTH 25      //最大命令长度
#define UNDEFINECMD	   0xFFFF  //命令buffer默认参数

enum Finger_t
{
	FINGER_1,

};

void resetServo(void);
void sendActionCmd(uint16_t *args);
void sendAngleCmd(uint8_t id, uint16_t value);
char *CmdForward(uint8_t *Cmd, uint8_t lenth, void(*CallBack)(uint8_t *,uint8_t));

#endif //__CMD_FORWARD_H
