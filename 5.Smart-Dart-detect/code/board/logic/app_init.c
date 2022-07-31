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

FATFS fs;						  /* FatFs文件系统对象 */
FRESULT f_res;                    /* 文件操作结果 */
#define START_LOGO	"0:/UI/start_logo/start_logo_for_tencent.bmp"
#define START_LOGO1 "0:/UI/start_logo/tencent_logo.bmp"
#define WLAN_LOGO0 "0:/UI/status_bar/connect_wifi.bmp"
#define WLAN_LOGO1 "0:/UI/status_bar/connect_server.bmp"
LCD_Driver_Model lcd_model ;

/*默认任务*/
#define DEFULT_TASK_SIZE			256
void default_task(void *pdata);
osThreadDef(default_task, osPriorityLow, 1, DEFULT_TASK_SIZE);

/*按键处理*/
#define KEY_TASK_SIZE					1500
void KeyTask(void  *argument);
osThreadDef(KeyTask, osPriorityHigh, 1, KEY_TASK_SIZE);

/*串口命令接收处理*/
k_msg_q_t recv_data_msg;
uint8_t   recv_msg_pool[10 * sizeof(void *)];
#define UART_RECV_TASK_SIZE			   2048
void UartCmdTask(void  *argument);
osThreadDef(UartCmdTask, osPriorityNormal, 1, UART_RECV_TASK_SIZE);

/*状态栏任务处理*/
#define STATUS_BAR_TASK_SIZE			2048
void Status_Task(void  *argument);
osThreadDef(Status_Task, osPriorityNormal, 1, STATUS_BAR_TASK_SIZE);

/*传感器任务处理*/
k_msg_q_t sensor_data_msg;
uint8_t   sensor_msg_pool[10 * sizeof(void *)];
#define SENSOR_TASK_SIZE			1024
void Sensor_Task(void  *argument);
osThreadDef(Sensor_Task, osPriorityRealtime, 1, SENSOR_TASK_SIZE);

/*网络数据上传任务*/
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

    /*读取配置信息*/
    Load_Config_ini_File();
    lcd_model.lcd_driver->Lcd_show_bmp(boot_logo_para);
    lcd_model.lcd_driver->lcd_display_onoff(1);
    HAL_Delay(1500);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    /*初始化打印机设备*/
    init_print_device();
    Menu_Init();
    //关指示灯
    HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
    //回到真正的main函数里
    $Super$$main();
    return 0 ;
}

/*启动操作系统*/
void start_tencent_os(void)
{
    /*初始化腾讯OS tiny内核*/
    osKernelInitialize();
    /*创建并启动一个默认任务*/
    osThreadCreate(osThread(default_task), NULL);
    /*启动TencentOS tiny内核*/
    osKernelStart();
}


/*默认任务处理*/
void default_task(void *pdata)
{
    tos_knl_sched_lock();
    /*创建队列，用于接收传感器数据*/
    tos_msg_q_create(&sensor_data_msg, sensor_msg_pool, SENSOR_U3_BUFFER_SIZE);
    tos_msg_q_create(&uploader_data_msg, uploader_msg_pool, NETWORK_BUF);
    tos_msg_q_create(&recv_data_msg, recv_msg_pool, CMD_STR_SIZE);
    /*创建按键任务*/
    osThreadCreate(osThread(KeyTask), NULL);
    /*创建状态栏任务*/
    osThreadCreate(osThread(Status_Task), NULL);
    /*创建数据上传任务*/
    osThreadCreate(osThread(uploader_task), NULL);
    /*创建传感器数据任务*/
    osThreadCreate(osThread(Sensor_Task), NULL);
    /*创建串口接收任务*/
    osThreadCreate(osThread(UartCmdTask), NULL);
    tos_knl_sched_unlock();
}


/*传感器任务处理*/
extern k_mail_q_t mail_q;
/*WIFI连接标志*/
extern __IO uint8_t wifi_connect_flag  ;
/*服务器连接标志*/
extern __IO uint8_t server_connect_flag  ;
extern double  ch20_ppm_value_printf;
void Sensor_Task(void  *argument)
{
    char ch20_data[20] = {0};
    /*传感器注册*/
    Sensor_Register(&Formaldehyde_Sensor_interface);
    while(1)
    {
        /*获取传感器数据*/
        Formaldehyde_Sensor_interface.get_Formaldehyde_Value(&Formaldehyde_Sensor_interface);

        /*处理传感器数据展示*/
        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
        {
            display_work_status(Formaldehyde_Sensor_interface.Formaldehyde_Value, 1);

            //PPM
            if(0 == User_Memory_Para.unit)
            {
                /* 正常解析之后通过队列发送 */
                memset(ch20_data, 0, 20);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 2)
                {
                    Formaldehyde_Sensor_interface.Formaldehyde_Value = 2 ;
                    sprintf(ch20_data, "ppm:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);
                }
                else
                    sprintf(ch20_data, "ppm:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);

                /*如果网络功能开的状态下才会进行队列发送*/
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
                /* 正常解析之后通过队列发送 */
                memset(ch20_data, 0, 20);

                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.046)
                {
                    Formaldehyde_Sensor_interface.Formaldehyde_Value = 0.046 ;
                    sprintf(ch20_data, "mgm3:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);
                }
                else
                    sprintf(ch20_data, "mgm3:%.3f", Formaldehyde_Sensor_interface.Formaldehyde_Value);

                /*如果网络功能开的状态下才会进行队列发送*/
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

/*按键任务处理*/
void KeyTask(void *argument)
{
    __IO uint8_t KeyCode = 255;

    while(1)
    {
        /*获取键值*/
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
            /*如果大于0.062ppm，则报警灯闪烁,否则熄灭*/
            if(0 == User_Memory_Para.unit)
            {
                if(Formaldehyde_Sensor_interface.Formaldehyde_Value > 0.062)
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                else
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            /*如果大于0.062*0.74666mg/m3，则报警灯闪烁,否则熄灭*/
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

        /*刷新WIFI连接状态*/
        if(0 == wifi_connect_flag)
            display_signal_status(0);
        else
            display_signal_status(4);

        /*刷新服务器连接状态*/
        if(0 == server_connect_flag)
            display_server_status(0);
        else
            display_server_status(1);

        /*刷新上传状态*/
        if(0 == User_Memory_Para.upload_flag)
            display_upload_status(0);
        else
            display_upload_status(1);

        /*刷新设备密码状态*/
        if(1 == User_Memory_Para.password_enable_flag)
            display_passwd_icon(0);
        else
            display_passwd_icon(1);

        /*刷新SD卡状态*/
        refresh_sd_card_status();
        tos_sleep_ms(1000);
    }
}


/*串口命令接收处理*/

/*打开LED灯*/
void led_on_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/*关闭LED灯*/
void led_off_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/*获取传感器数据*/
void get_sensor_data(void)
{
    DEBUG("传感器数据:%.3fppm\n", Formaldehyde_Sensor_interface.Formaldehyde_Value);
}

/*获取当前系统版本*/
void get_system_version(void)
{
    DEBUG("当前系统版本:%s %s\n", __DATE__, __TIME__);
}

/*系统复位*/
void cpu_reset_process(void)
{
    DEBUG("系统重启\n");
    NVIC_SystemReset();
}

/*获取系统参数*/
void get_system_para(void)
{
    char buf[50] = {0};
    DEBUG("报警:%d\n", User_Memory_Para.alarm);
    DEBUG("数值:%d\n", User_Memory_Para.value);
    DEBUG("单位:%d\n", User_Memory_Para.unit);
    DEBUG("调试标志:%d\n", User_Memory_Para.debug_flag);
    DEBUG("上传标志:%d\n", User_Memory_Para.upload_flag);
    DEBUG("设备密码:%d%d%d%d\n", User_Memory_Para.password[0], User_Memory_Para.password[1],
          User_Memory_Para.password[2], User_Memory_Para.password[3]);
    DEBUG("当前wifi名称:%s\n", User_Memory_Para.wifi_name);
    DEBUG("当前wifi密码:%s\n", User_Memory_Para.wifi_password);
    DEBUG("当前服务器地址:%s\n", User_Memory_Para.server_ip);
    DEBUG("当前服务器端口号:%d\n", User_Memory_Para.server_port);
    DEBUG("当前系统版本:%s %s\n", __DATE__, __TIME__);
    /*居中*/
    Sel_Align_Way(1);
    Printf_Str("达特甲醛探测\r\n");
    Printf_Str("作者:杨源鑫\r\n");
    Printf_Str("硬件平台:小熊派\r\n");
    Printf_Str("操作系统:TencentOS tiny\r\n");
    Printf_Str("当前系统参数如下：\r\n");
    /*左对齐*/
    Sel_Align_Way(0);
    memset(buf, 0, 50);
    sprintf(buf, "报警:%d\r\n", User_Memory_Para.alarm);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "数值:%d\r\n", User_Memory_Para.value);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "单位:%d\r\n", User_Memory_Para.unit);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "调试标志:%d\r\n", User_Memory_Para.debug_flag);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "上传标志:%d\r\n", User_Memory_Para.upload_flag);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "设备密码:%d%d%d%d\r\n", User_Memory_Para.password[0], User_Memory_Para.password[1], User_Memory_Para.password[2], User_Memory_Para.password[3]);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "当前wifi名称:%s\r\n", User_Memory_Para.wifi_name);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "当前wifi密码:%s\r\n", User_Memory_Para.wifi_password);
    Printf_Str(buf);
    memset(buf, 0, 50);
    sprintf(buf, "当前统版本:%s %s\r\n\r\n", __DATE__, __TIME__);
    Printf_Str(buf);
}

/*注册命令*/
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
    /*命令初始化*/
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

