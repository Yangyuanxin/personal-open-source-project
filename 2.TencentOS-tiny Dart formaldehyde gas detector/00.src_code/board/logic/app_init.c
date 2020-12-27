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

#define NETWORK_FUNC 0

FATFS fs;						  /* FatFs文件系统对象 */
FRESULT f_res;                    /* 文件操作结果 */
#define START_LOGO	"0:/UI/start_logo/start_logo.bmp"
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
void StartKeyTask(void  *argument);
osThreadDef(StartKeyTask, osPriorityHigh, 1, KEY_TASK_SIZE);

/*串口命令接收处理*/
k_msg_q_t recv_data_msg;
uint8_t   recv_msg_pool[10 * sizeof(void *)];
#define UART_RECV_TASK_SIZE			   1000
void StartUartRecvTask(void  *argument);
osThreadDef(StartUartRecvTask, osPriorityNormal, 1, UART_RECV_TASK_SIZE);

/*状态栏任务处理*/
#define STATUS_BAR_TASK_SIZE			400
void StartStatus_Bar_Task(void  *argument);
osThreadDef(StartStatus_Bar_Task, osPriorityNormal, 1, STATUS_BAR_TASK_SIZE);

/*传感器任务处理*/
k_msg_q_t sensor_data_msg;
uint8_t   sensor_msg_pool[10 * sizeof(void *)];
#define SENSOR_TASK_SIZE			512
void StartSensor_Task(void  *argument);
osThreadDef(StartSensor_Task, osPriorityRealtime, 1, SENSOR_TASK_SIZE);

/*网络数据上传任务处理*/
uint8_t esp_8266_data;
#define NETWORK_STK_SIZE         2096
void StartNetWorkTask(void *argument);
osThreadDef(StartNetWorkTask, osPriorityNormal, 1, NETWORK_STK_SIZE);
__IO uint8_t connect_wifi = 0;
__IO uint8_t connect_server = 0;

LCD_Bmp_Show_Para boot_logo_para = {0, 0, START_LOGO};
LCD_Bmp_Show_Para boot_logo1_para = {54, 213, START_LOGO1};

LCD_Bmp_Show_Para status_wlan_para[] =
{
    {2, 5, WLAN_LOGO0},
    {35, 5, WLAN_LOGO1},
};


/*显示无线状态*/
void display_wlan_status(uint8_t status)
{
    LCD_Fill_Para fill_logo;
    fill_logo.x_start = 15 ;
    fill_logo.y_start = 37 ;
    fill_logo.color   = BLACK ;
    fill_logo.x_end = fill_logo.x_start + 24 ;
    fill_logo.y_end = fill_logo.y_start + 24 ;

    if(0 == status)
        lcd_model.lcd_driver->lcd_fill(fill_logo);
    else if(1 == status)
        lcd_model.lcd_driver->Lcd_show_bmp(status_wlan_para[0]);
    else if(2 == status)
        lcd_model.lcd_driver->Lcd_show_bmp(status_wlan_para[1]);
}


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
    tos_msg_q_create(&recv_data_msg, recv_msg_pool, CMD_STR_SIZE);
    /*创建按键任务*/
    osThreadCreate(osThread(StartKeyTask), NULL);
    /*创建状态栏任务*/
    osThreadCreate(osThread(StartStatus_Bar_Task), NULL);
		#if defined NETWORK_FUNC == 1
    /*创建数据上传任务*/
    osThreadCreate(osThread(StartNetWorkTask), NULL);
		#endif
    /*创建传感器数据任务*/
    osThreadCreate(osThread(StartSensor_Task), NULL);
    /*创建串口接收任务*/
    osThreadCreate(osThread(StartUartRecvTask), NULL);
    tos_knl_sched_unlock();
}


/*传感器任务处理*/
void StartSensor_Task(void  *argument)
{
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
            display_hcho_value(Formaldehyde_Sensor_interface.Formaldehyde_Value, GREEN, 1);
            display_hcho_max_value(Formaldehyde_Sensor_interface.Formaldehyde_Max_Value, GREEN, 1);
            display_hcho_min_value(Formaldehyde_Sensor_interface.Formaldehyde_Min_Value, GREEN, 1);
        }
    }
}

/*按键任务处理*/
void StartKeyTask(void *argument)
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
void StartStatus_Bar_Task(void *argument)
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

        tos_sleep_ms(1000);
    }
}


/*网络数据上传任务*/
#if defined NETWORK_FUNC == 1
#define RECV_LEN 1024
uint8_t recv_data[RECV_LEN];
char buf[512] = {0};
char buf_data[120] = {0};
void OneNet_Packet_Send(int fd, const char *device_id, const char *api_key, int lux)
{
    int len = 0 ;

    sprintf(buf_data, http_uploader_data, lux);
    len = strlen(buf_data);
    sprintf(buf, http_uploader_head, device_id, api_key, len);
    strcat(buf, buf_data);
    printf("数据包:\n");
    printf("%s", buf);
    send(fd, buf, strlen(buf), 0);
}

void OneNet_SendData(int fd)
{
    /*如果打开了数据上传按钮，如果检测到危险时，则开启上传模式*/
    if(1 == User_Memory_Para.upload_flag)
        OneNet_Packet_Send(fd, ONENET_DEVICE_ID, ONENET_API_KEY, Formaldehyde_Sensor_interface.Formaldehyde_Value);
}

void StartNetWorkTask(void *argument)
{
    int ret = 0 ;
    ret = esp8266_sal_init(HAL_UART_PORT_0);

    if(ret != 0 )
    {
        printf("esp8266 sal初始化失败\n");
        return ;
    }

    printf("esp8266 sal初始化成功\n");
    esp8266_join_ap(User_Memory_Para.wifi_name, User_Memory_Para.wifi_password);
    /*WIFI连接成功*/
    display_wlan_status(1);
    connect_wifi = 1 ;

    while(1)
    {
        int recv_len = -1;
        int fd, rc, cnt = 0;

        struct sockaddr_in addr;

        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(User_Memory_Para.server_ip);
        addr.sin_port = htons(User_Memory_Para.server_port);

        fd = socket(AF_INET, SOCK_STREAM, 0);

        if (fd < 0)
        {
            DEBUG("socket failed\n");
            return;
        }

        rc = connect(fd, (struct sockaddr *)&addr, sizeof(addr));

        if (rc < 0)
        {
            DEBUG("connect failed\n");
            close(fd);
            return;
        }

        /*服务器连接成功*/
        display_wlan_status(2);
        connect_server = 1 ;

        while (1)
        {

            OneNet_SendData(fd);
            recv_len = recv(fd, recv_data, sizeof(recv_data), 0);

            if (recv_len < 0)
            {
                // printf("task receive error\n");
            }
            else if (recv_len == 0)
            {
                // printf("task receive none\n");
            }
            else
            {
                recv_data[recv_len] = 0;
                DEBUG("task: receive len: %d\nmsg from remote: %s\n", recv_len, recv_data);
            }

            if (++cnt >= 20)
            {
                break;
            }

            tos_sleep_ms(2000);
        }

        close(fd);
    }
}
#endif

/*串口命令接收处理*/
void led_on_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

void led_off_process(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

void get_sensor_data(void)
{
    printf("传感器数据:%.3fppm\n",Formaldehyde_Sensor_interface.Formaldehyde_Value);
}

void get_system_version(void)
{
    printf("当前系统版本:%s %s\n", __DATE__, __TIME__);
}

void cpu_reset_process(void)
{
    printf("系统重启\n");
    NVIC_SystemReset();
}

/*注册命令*/
REGISTER_CMD(reset, cpu_reset_process);
REGISTER_CMD(led_on, led_on_process);
REGISTER_CMD(led_off, led_off_process);
REGISTER_CMD(get_sensor_data, get_sensor_data);
REGISTER_CMD(get_system_version, get_system_version);
void StartUartRecvTask(void  *argument)
{
    k_err_t err;
    void *msg_received;
    uint8_t uart_recv_buf[CMD_STR_SIZE] = {0};
    /*命令初始化*/
    cmd_init();
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&cmd_parse_typedef.Res, 1);
    printf("串口接收命令任务启动\n");

    while(1)
    {
        err = tos_msg_q_pend(&recv_data_msg, &msg_received, TOS_TIME_FOREVER);

        if (err == K_ERR_NONE)
        {
            memcpy(uart_recv_buf, msg_received, CMD_STR_SIZE);
            printf("接收到指令:%s\n", uart_recv_buf);
            cmd_parsing((char *)uart_recv_buf);
            memset(&cmd_parse_typedef, 0, sizeof(cmd_parse_typedef));
        }
    }
}

