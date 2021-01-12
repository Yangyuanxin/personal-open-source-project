#include "app_init.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_bmp.h"
#include "stm32l4xx_it.h"
#include "mqtt_iot_explorer_tc_ch20.h"

FATFS fs;						  /* FatFs�ļ�ϵͳ���� */
FRESULT f_res;                    /* �ļ�������� */
#define START_LOGO	"0:/UI/start_logo/start_logo_for_tencent.bmp"
#define START_LOGO1 "0:/UI/start_logo/tencent_logo.bmp"
#define WLAN_LOGO0 "0:/UI/status_bar/connect_wifi.bmp"
#define WLAN_LOGO1 "0:/UI/status_bar/connect_server.bmp"
LCD_Driver_Model lcd_model ;

/*Ĭ������*/
#define DEFULT_TASK_SIZE			256
void default_task(void *pdata);
osThreadDef(default_task, osPriorityLow, 1, DEFULT_TASK_SIZE);

/*���������ϴ�����*/
uint8_t esp_8266_data;
k_msg_q_t uploader_data_msg;
#define NETWORK_BUF 20
uint8_t   uploader_msg_pool[10 * sizeof(void *)];
#define APPLICATION_TASK_STK_SIZE       2048
extern void uploader_task(void *arg);
osThreadDef(uploader_task, osPriorityNormal, 1, APPLICATION_TASK_STK_SIZE);


int $Sub$$main(void)
{
    extern int main(void);
    extern int $Super$$main(void);
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_RTC_Init();
    MX_ADC1_Init();
    MX_TIM16_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    Register_Driver_Model(&lcd_model);
    lcd_model.lcd_driver->lcd_init();
    lcd_model.lcd_driver->lcd_display_onoff(1);
    HAL_Delay(1500);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    /*��ʼ����ӡ���豸*/
    init_print_device();
    //��ָʾ��
    HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
    //�ص�������main������
    $Super$$main();
    return 0 ;
}

/*��������ϵͳ*/
void start_tencent_os(void)
{
    /*��ʼ����ѶOS tiny�ں�*/
    osKernelInitialize();
    /*����������һ��Ĭ������*/
    osThreadCreate(osThread(default_task), NULL);
    /*����TencentOS tiny�ں�*/
    osKernelStart();
}


/*Ĭ��������*/
void default_task(void *pdata)
{
    tos_knl_sched_lock();
    tos_msg_q_create(&uploader_data_msg, uploader_msg_pool, NETWORK_BUF);
    /*���������ϴ�����*/
    osThreadCreate(osThread(uploader_task), NULL);
    tos_knl_sched_unlock();
		/*WIFI���Ӷ���״̬��˸*/
		while(1)
		{
			tos_task_delay(500);
		}
}


