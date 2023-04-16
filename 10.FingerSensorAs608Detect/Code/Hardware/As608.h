#ifndef __AS608_H
#define __AS608_H

#include <stdint.h>
#include <string.h>
#include "usart.h"
//#include "stm32f1xx.h"

#define AS608_NR(x) (sizeof(x) / sizeof(x[0]))

extern UART_HandleTypeDef huart2;

enum As608Command
{
   AS608_CMD_AUTH_CHECK = 0,
   AS608_CMD_GET_IMAGE,
   AS608_CMD_GET_TEMPLATECNT,
   AS608_CMD_SEARCH_CODE,
   AS608_CMD_SEARCH_0_9,
   AS608_CMD_SAVE_IMAGE_2_BUF1,
   AS608_CMD_SAVE_IMAGE_2_BUF2,
   AS608_CMD_BUF1_BUF2_SPEC_MERGE,
   AS608_CMD_DEL_ALL_TEMPLATE,
   AS608_CMD_SAVE_FINGER
};

typedef struct As608Dri
{
    uint8_t CmdId;
    void (*CmdFunc)();
}As608Dri;

#define PACKET_HEADER_HIGH      0xEF
#define PACKET_HEADER_LOW       0x01
#define CHIP_ADDRESS_0          0xFF
#define CHIP_ADDRESS_1          0xFF
#define CHIP_ADDRESS_2          0xFF
#define CHIP_ADDRESS_3          0xFF
#define COMMAND_PACKET_MARK     0x01    //命令包
#define DATA_PACKET_MARK        0x02    //数据包
#define END_PACKET_MARK         0x08    //结束包
#define RESPONSES_PACKET_MARK   0x07    //应答包

extern As608Dri As608CmdArr[];
extern __IO uint8_t As608GetId;

void As608ModuleInit(void);


#endif //__AS608_H
