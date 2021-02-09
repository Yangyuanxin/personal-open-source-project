#include "mc_printf_str.h"
#include "usart.h"

//发送单个字节给打印机
void Printf_send_byte(uint8_t data)
{
    /*调度器上锁*/
    tos_knl_sched_lock();
    HAL_UART_Transmit(&huart3, &data, 1, 1000);

    while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) != SET);

    /*调度器解锁*/
    tos_knl_sched_unlock();
}


//发送字符串给打印机
static void Printf_send_command(char* str)
{
    /*调度器上锁*/
    tos_knl_sched_lock();
    while(*str)
    {
        Printf_send_byte(*str);
        str++;
    }
    /*调度器解锁*/
    tos_knl_sched_unlock();
}
  
/*初始化打印设备*/
void init_print_device(void)
{
	Printf_send_byte(0x1B);
	Printf_send_byte(0x40);
}	


/*打印字符串*/
void Printf_Str(char *buf)
{
	Printf_send_command(buf);
}

/*打印自测页*/
void Print_self_page(void)
{
	Printf_send_byte(0x12);
	Printf_send_byte(0x54);
}

