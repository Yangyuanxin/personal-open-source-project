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

/*��������*/
#define KEY_TASK_SIZE					1500
void KeyTask(void  *argument);
osThreadDef(KeyTask, osPriorityHigh, 1, KEY_TASK_SIZE);

/*����������մ���*/
k_msg_q_t recv_data_msg;
uint8_t   recv_msg_pool[10 * sizeof(void *)];
#define UART_RECV_TASK_SIZE			   2048
void UartCmdTask(void  *argument);
osThreadDef(UartCmdTask, osPriorityNormal, 1, UART_RECV_TASK_SIZE);

/*״̬��������*/
#define STATUS_BAR_TASK_SIZE			2048
void Status_Task(void  *argument);
osThreadDef(Status_Task, osPriorityNormal, 1, STATUS_BAR_TASK_SIZE);

/*������������*/
k_msg_q_t sensor_data_msg;
uint8_t   sensor_msg_pool[10 * sizeof(void *)];
#define SENSOR_TASK_SIZE			1024
void Sensor_Task(void  *argument);
osThreadDef(Sensor_Task, osPriorityRealtime, 1, SENSOR_TASK_SIZE);

/*���������ϴ�����*/
uint8_t esp_8266_data;
k_msg_q_t uploader_data_msg;
#define NETWORK_BUF 20
uint8_t   uploader_msg_pool[10 * sizeof(void *)];
#define APPLICATION_TASK_STK_SIZE       2048
extern void uploader_task(void *arg);
osThreadDef(uploader_task, osPriorityNormal, 1, APPLICATION_TASK_STK_SIZE);


LCD_Bmp_Show_Para boot_logo_para = {0, 0, START_LOGO};
LCD_Bmp_Show_Para boot_logo1_para = {54, 213, START_LOGO1};


int $Sub$$main(void)
{
    extern int main(void);
    extern int $Super$$main(void);
    int ret = -1 ;
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
    //PowerOn();
    lcd_model.lcd_driver->lcd_init();
    ret = Mount_SD();

    if(ret != 0)
    {
        printf("SD Card mount ERROR\r\n");
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        return -1 ;
    }

    /*��ȡ������Ϣ*/
    Load_Config_ini_File();
    lcd_model.lcd_driver->Lcd_show_bmp(boot_logo_para);
    lcd_model.lcd_driver->lcd_display_onoff(1);
    HAL_Delay(1500);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    /*��ʼ����ӡ���豸*/
    init_print_device();
    Menu_Init();
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
    /*�������У����ڽ��մ���������*/
    tos_msg_q_create(&sensor_data_msg, sensor_msg_pool, SENSOR_U3_BUFFER_SIZE);
    tos_msg_q_create(&uploader_data_msg, uploader_msg_pool, NETWORK_BUF);
    tos_msg_q_create(&recv_data_msg, recv_msg_pool, CMD_STR_SIZE);
    /*������������*/
    osThreadCreate(osThread(KeyTask), NULL);
    /*����״̬������*/
    osThreadCreate(osThread(Status_Task), NULL);
    /*���������ϴ�����*/
    osThreadCreate(osThread(uploader_task), NULL);
    /*������������������*/
    osThreadCreate(osThread(Sensor_Task), NULL);
    /*�������ڽ�������*/
    osThreadCreate(osThread(UartCmdTask), NULL);
    tos_knl_sched_unlock();
}


/*������������*/
extern k_mail_q_t mail_q;
/*WIFI���ӱ�־*/
extern __IO uint8_t wifi_connect_flag  ;
/*���������ӱ�־*/
extern __IO uint8_t server_connect_flag  ;
extern double  ch20_ppm_value_printf;
void Sensor_Task(void  *argument)
{
    char ch20_data[20] = {0};
    /*������ע��*/
    Sensor_Register(&Formaldehyde_Sensor_interface);
    while(1)
    {
        /*��ȡ����������*/
        Formaldehyde_Sensor_interface.get_Formaldehyde_Value(&Formaldehyde_Sensor_interface);

        /*������������չʾ*/
        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
        {
            display_work_status(Formaldehyde_Sensor_interface.Formaldehyde_Value, 1);

            //PPM
            if(0 == User_Memory_Para.unit)
            {
                /* ��������֮��ͨ�����з��� */
                memset(ch20_data, 0, 20);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 2)
                {
                    Formaldehyde_Sensor_interface.Formaldehyde_Value = 2 ;
                    sprintf(ch20_data, "ppm:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);
                }
                else
                    sprintf(ch20_data, "ppm:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);

                /*������繦�ܿ���״̬�²Ż���ж��з���*/
                if(1 == User_Memory_Para.upload_flag && 1 == wifi_connect_flag && 1 == server_connect_flag)
                    tos_msg_q_post(&uploader_data_msg, ch20_data);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 2)
                {
                    ch20_ppm_value_printf = 2;
                    display_hcho_value(ch20_ppm_value_printf, RED, 1);

                    if(1 == User_Memory_Para.value)
                    {
                        display_hcho_max_value(ch20_ppm_value_printf, GREEN, 1);
                        display_hcho_min_value(ch20_ppm_value_printf, GREEN, 1);
                    }
                    else
                    {
                        display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                    }
                }
                else
                {
                    if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.062)
                    {
                        ch20_ppm_value_printf = Formaldehyde_Sensor_interface.Formaldehyde_Value;
                        display_hcho_value(Formaldehyde_Sensor_interface.Formaldehyde_Value, RED, 1);

                        if(1 == User_Memory_Para.value)
                        {
                            display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
                            display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value, GREEN, 1);
                        }
                        else
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        }
                    }
                    else
                    {
                        ch20_ppm_value_printf = Formaldehyde_Sensor_interface.Formaldehyde_Value;
                        display_hcho_value(Formaldehyde_Sensor_interface.Formaldehyde_Value, GREEN, 1);

                        if(1 == User_Memory_Para.value)
                        {
                            display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
                            display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value, GREEN, 1);
                        }
                        else
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        }
                    }
                }
            }
            //mg/m3
            else
            {
                /* ��������֮��ͨ�����з��� */
                memset(ch20_data, 0, 20);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.046)
                {
                    Formaldehyde_Sensor_interface.Formaldehyde_Value = 0.046 ;
                    sprintf(ch20_data, "mgm3:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);
                }
                else
                    sprintf(ch20_data, "mgm3:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);

                /*������繦�ܿ���״̬�²Ż���ж��з���*/
                if(1 == User_Memory_Para.upload_flag && 1 == wifi_connect_flag && 1 == server_connect_flag)
                    tos_msg_q_post(&uploader_data_msg, ch20_data);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 1.493)		//2 * 0.74666
                {
                    ch20_ppm_value_printf = 1.493;
                    display_hcho_value(ch20_ppm_value_printf, RED, 1);

                    if(1 == User_Memory_Para.value)
                    {
                        display_hcho_max_value(ch20_ppm_value_printf, GREEN, 1);
                        display_hcho_min_value(ch20_ppm_value_printf, GREEN, 1);
                    }
                    else
                    {
                        display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                    }
                }
                else
                {
                    if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.046)
                    {
                        ch20_ppm_value_printf = Formaldehyde_Sensor_interface.Formaldehyde_Value;
                        display_hcho_value(ch20_ppm_value_printf, RED, 1);

                        if(1 == User_Memory_Para.value)
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, GREEN, 1);
                            display_hcho_min_value(ch20_ppm_value_printf, GREEN, 1);
                        }
                        else
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        }
                    }
                    else
                    {
                        ch20_ppm_value_printf = Formaldehyde_Sensor_interface.Formaldehyde_Value;
                        display_hcho_value(ch20_ppm_value_printf, GREEN, 1);

                        if(1 == User_Memory_Para.value)
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, GREEN, 1);
                            display_hcho_min_value(ch20_ppm_value_printf, GREEN, 1);
                        }
                        else
                        {
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                            display_hcho_max_value(ch20_ppm_value_printf, BLACK, 0);
                        }
                    }
                }
            }
        }
    }
}

/*����������*/
void KeyTask(void *argument)
{
    __IO uint8_t KeyCode = 255;

    while(1)
    {
        /*��ȡ��ֵ*/
        GetKey(&KeyCode);

        if(255 != KeyCode)
        {
            Menu_Select_Item(Flow_Cursor.flow_cursor, KeyCode);
            KeyCode = 255 ;
        }

        tos_sleep_ms(5);
    }
}


extern LCD_Ascii_Show_Para datatime_display_para ;
void Status_Task(void *argument)
{
    LCD_Fill_Para status_bar_fill_para = {0, 240, 0, 27, WHITE} ;
    lcd_model.lcd_driver->lcd_fill(status_bar_fill_para);
    display_runing_time_font(1);

    while(1)
    {
        Get_Date_Time();

        if(0 == plot_handler.plot_mode)
        {
            sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
                    DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
            lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);

            if(MAIN_PAGE == Flow_Cursor.flow_cursor)
                display_runing_time(DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec, 1);
        }

        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
        {
            tos_knl_sched_lock();
            save_record_to_flash();
            tos_knl_sched_unlock();
        }

        if(1 == User_Memory_Para.alarm)
        {
            /*�������0.062ppm���򱨾�����˸,����Ϩ��*/
            if(0 == User_Memory_Para.unit)
            {
                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.062)
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                else
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            /*�������0.062*0.74666mg/m3���򱨾�����˸,����Ϩ��*/
            else
            {
                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.046)
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                else
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
        }
        else
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }

        /*ˢ��WIFI����״̬*/
        if(0 == wifi_connect_flag)
            display_signal_status(0);
        else
            display_signal_status(4);

        /*ˢ�·���������״̬*/
        if(0 == server_connect_flag)
            display_server_status(0);
        else
            display_server_status(1);

        /*ˢ���ϴ�״̬*/
        if(0 == User_Memory_Para.upload_flag)
            display_upload_status(0);
        else
            display_upload_status(1);

        /*ˢ���豸����״̬*/
        if(1 == User_Memory_Para.password_enable_flag)
            display_passwd_icon(0);
        else
            display_passwd_icon(1);

        /*ˢ��SD��״̬*/
        refresh_sd_card_status();
        tos_sleep_ms(1000);
    }
}


/*����������մ���*/

/*��LED��*/
void led_on_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/*�ر�LED��*/
void led_off_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/*��ȡ����������*/
void get_sensor_data(void)
{
    DEBUG("����������:%.3fppm\n", Formaldehyde_Sensor_interface.Formaldehyde_Value);
}

/*��ȡ��ǰϵͳ�汾*/
void get_system_version(void)
{
    DEBUG("��ǰϵͳ�汾:%s %s\n", __DATE__, __TIME__);
}

/*ϵͳ��λ*/
void cpu_reset_process(void)
{
    DEBUG("ϵͳ����\n");
    NVIC_SystemReset();
}

/*��ȡϵͳ����*/
void get_system_para(void)
{
    char buf[50] = {0};
    DEBUG("����:%d\n", User_Memory_Para.alarm);
    DEBUG("��ֵ:%d\n", User_Memory_Para.value);
    DEBUG("��λ:%d\n", User_Memory_Para.unit);
    DEBUG("���Ա�־:%d\n", User_Memory_Para.debug_flag);
    DEBUG("�ϴ���־:%d\n", User_Memory_Para.upload_flag);
    DEBUG("�豸����:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
          User_Memory_Para.password[2], User_Memory_Para.password[3]);
    DEBUG("��ǰwifi����:%s\n", User_Memory_Para.wifi_name);
    DEBUG("��ǰwifi����:%s\n", User_Memory_Para.wifi_password);
    DEBUG("��ǰ��������ַ:%s\n", User_Memory_Para.server_ip);
    DEBUG("��ǰ�������˿ں�:%d\n", User_Memory_Para.server_port);
    DEBUG("��ǰϵͳ�汾:%s %s\n", __DATE__, __TIME__);
    /*����*/
    Sel_Align_Way(1);
    Printf_Str("���ؼ�ȩ̽��\r\n");
    Printf_Str("����:��Դ��\r\n");
    Printf_Str("Ӳ��ƽ̨:С����\r\n");
    Printf_Str("����ϵͳ:TencentOS tiny\r\n");
    Printf_Str("��ǰϵͳ�������£�\r\n");
    /*�����*/
    Sel_Align_Way(0);
    memset(buf, 0, 50);
    sprintf(buf, "����:%d\r\n", User_Memory_Para.alarm);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "��ֵ:%d\r\n", User_Memory_Para.value);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "��λ:%d\r\n", User_Memory_Para.unit);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "���Ա�־:%d\r\n", User_Memory_Para.debug_flag);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "�ϴ���־:%d\r\n", User_Memory_Para.upload_flag);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "�豸����:%d%d%d%d\r\n", User_Memory_Para.password[0], User_Memory_Para.password[1], User_Memory_Para.password[2], User_Memory_Para.password[3]);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "��ǰwifi����:%s\r\n", User_Memory_Para.wifi_name);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "��ǰwifi����:%s\r\n", User_Memory_Para.wifi_password);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "��ǰͳ�汾:%s %s\r\n\r\n", __DATE__, __TIME__);
    Printf_Str(buf);
}

/*ע������*/
REGISTER_CMD(reset, cpu_reset_process);
REGISTER_CMD(led_on, led_on_process);
REGISTER_CMD(led_off, led_off_process);
REGISTER_CMD(get_sensor_data, get_sensor_data);
REGISTER_CMD(get_system_para, get_system_para);
REGISTER_CMD(get_system_version, get_system_version);
void UartCmdTask(void  *argument)
{
    k_err_t err;
    void *msg_received;
    uint8_t uart_recv_buf[CMD_STR_SIZE] = {0};
    /*�����ʼ��*/
    cmd_init();
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&cmd_parse_typedef.Res, 1);

    while(1)
    {
        err = tos_msg_q_pend(&recv_data_msg, &msg_received, TOS_TIME_FOREVER);

        if (err == K_ERR_NONE)
        {
            memcpy(uart_recv_buf, msg_received, CMD_STR_SIZE);
            cmd_parsing((char *)uart_recv_buf);
            memset(&cmd_parse_typedef, 0, sizeof(cmd_parse_typedef));
        }
    }
}

