#include "mc_printf_str.h"
  
/*��ʼ����ӡ�豸*/
void init_print_device(void)
{
	InitializePrint();
}	

/*��ӡ�ַ���*/
void Printf_Str(char *buf)
{
	Print_ASCII((uint8_t *)buf,strlen(buf));
}

