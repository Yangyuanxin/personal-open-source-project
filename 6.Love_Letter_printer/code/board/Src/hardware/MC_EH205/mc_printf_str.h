#ifndef __MC_PRINTF_STR_H
#define __MC_PRINTF_STR_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>



/*初始化打印设备*/
void init_print_device(void);
//发送单个字节给打印机
void Printf_send_byte(uint8_t data);
/*打印字符串*/
void Printf_Str(char *buf);
/*打印自测页*/
void Print_self_page(void);



#endif //__MC_PRINTF_STR_H

