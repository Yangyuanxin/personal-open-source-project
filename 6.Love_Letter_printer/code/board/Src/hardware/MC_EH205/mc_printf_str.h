#ifndef __MC_PRINTF_STR_H
#define __MC_PRINTF_STR_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>



/*��ʼ����ӡ�豸*/
void init_print_device(void);
//���͵����ֽڸ���ӡ��
void Printf_send_byte(uint8_t data);
/*��ӡ�ַ���*/
void Printf_Str(char *buf);
/*��ӡ�Բ�ҳ*/
void Print_self_page(void);



#endif //__MC_PRINTF_STR_H

