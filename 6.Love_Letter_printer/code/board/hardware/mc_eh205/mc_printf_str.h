#ifndef __MC_PRINTF_STR_H
#define __MC_PRINTF_STR_H
#include <stdint.h>
/*添加打印机模块*/
#include "MC_Print_inc.h"
#include <string.h>
#include <stdio.h>



/*初始化打印设备*/
void init_print_device(void);
/*打印字符串*/
void Printf_Str(char *buf);




#endif //__MC_PRINTF_STR_H

