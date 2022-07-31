#include "mc_printf_str.h"
  
/*初始化打印设备*/
void init_print_device(void)
{
	InitializePrint();
}	

/*打印字符串*/
void Printf_Str(char *buf)
{
	Print_ASCII((uint8_t *)buf,strlen(buf));
}

