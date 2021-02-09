#include "mc_printf_str.h"
#include "usart.h"

//���͵����ֽڸ���ӡ��
void Printf_send_byte(uint8_t data)
{
    /*����������*/
    tos_knl_sched_lock();
    HAL_UART_Transmit(&huart3, &data, 1, 1000);

    while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) != SET);

    /*����������*/
    tos_knl_sched_unlock();
}


//�����ַ�������ӡ��
static void Printf_send_command(char* str)
{
    /*����������*/
    tos_knl_sched_lock();
    while(*str)
    {
        Printf_send_byte(*str);
        str++;
    }
    /*����������*/
    tos_knl_sched_unlock();
}
  
/*��ʼ����ӡ�豸*/
void init_print_device(void)
{
	Printf_send_byte(0x1B);
	Printf_send_byte(0x40);
}	


/*��ӡ�ַ���*/
void Printf_Str(char *buf)
{
	Printf_send_command(buf);
}

/*��ӡ�Բ�ҳ*/
void Print_self_page(void)
{
	Printf_send_byte(0x12);
	Printf_send_byte(0x54);
}

