#include "app_init.h"
#include "adc.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_bmp.h"


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
osThreadDef(StartKeyTask, osPriorityRealtime, 1, KEY_TASK_SIZE);

/*状态栏任务处理*/
#define STATUS_BAR_TASK_SIZE			400
void StartStatus_Bar_Task(void  *argument);
osThreadDef(StartStatus_Bar_Task, osPriorityRealtime, 1, STATUS_BAR_TASK_SIZE);

/*传感器任务处理*/
#define SENSOR_TASK_SIZE			512
void StartSensor_Task(void  *argument);
osThreadDef(StartSensor_Task, osPriorityHigh, 1, SENSOR_TASK_SIZE);

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

LCD_Chinese_Show_Para main_title_font[] =
{
    {40, 180, (uint8_t *)"危险气体探测仪", WHITE, BLACK, 24, 1},
    {40, 180, (uint8_t *)"危险气体探测仪", BLACK, BLACK, 24, 1},
};

/*显示无线状态*/
void display_wlan_status(uint8_t status)
{
    LCD_Fill_Para fill_logo;
    fill_logo.x_start = 2 ;
    fill_logo.y_start = 5 ;
    fill_logo.color   = BLACK ;
    fill_logo.x_end = 2 + 24 ;
    fill_logo.y_end = 5 + 24 ;

    if(0 == status)
        lcd_model.lcd_driver->lcd_fill(fill_logo);
    else if(1 == status)
        lcd_model.lcd_driver->Lcd_show_bmp(status_wlan_para[0]);
    else if(2 == status)
        lcd_model.lcd_driver->Lcd_show_bmp(status_wlan_para[1]);
}

/*显示TencentOS tiny logo*/
void display_tencent_logo(uint8_t status)
{
    LCD_Fill_Para fill_logo;
    fill_logo.x_start = 54 ;
    fill_logo.y_start = 213 ;
    fill_logo.color   = BLACK ;
    fill_logo.x_end = 54 + 130 ;
    fill_logo.y_end = 213 + 26 ;

    if(1 == status)
    {
        lcd_model.lcd_driver->Lcd_show_bmp(boot_logo1_para);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[0]);
    }
    else
    {
        lcd_model.lcd_driver->lcd_fill(fill_logo);
        lcd_model.lcd_driver->lcd_show_chinese_str(main_title_font[1]);
    }
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
    PowerOn();
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
    Sensor_Register(&mq2_sensor_interface);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    Menu_Init();
    display_tencent_logo(1);
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
    /*创建按键任务*/
    osThreadCreate(osThread(StartKeyTask), NULL);
    /*创建状态栏任务*/
    osThreadCreate(osThread(StartStatus_Bar_Task), NULL);
    /*创建数据上传任务*/
    osThreadCreate(osThread(StartNetWorkTask), NULL);
    /*创建传感器数据任务*/
    osThreadCreate(osThread(StartSensor_Task), NULL);
    tos_knl_sched_unlock();
}

/*传感器任务处理*/
void StartSensor_Task(void  *argument)
{
    uint32_t i = 0 ;

    while(1)
    {
        mq2_sensor_interface.get_smoke_value(&mq2_sensor_interface);

        /*更新数据到队列*/
        for(i = 0 ; i <= DATA_SIZE - 2 ; i++)
            plot_handler.rel_data_data[i] = plot_handler.rel_data_data[i + 1];

        plot_handler.rel_data_data[DATA_SIZE - 1] = mq2_sensor_interface.Smoke_Value ;

        if(1 == plot_handler.plot_mode && 0 == plot_handler.fixed_screen)
        {
            /*锁调度*/
            tos_knl_sched_lock();
            LCD_Plot_Display(plot_handler.old_plot_data, DATA_SIZE, BLACK);
            LCD_Plot_Remap(plot_handler.rel_data_data, plot_handler.new_plot_data, DATA_SIZE);
            LCD_Plot_Display(plot_handler.new_plot_data, DATA_SIZE, GREEN);
            memcpy(plot_handler.old_plot_data, plot_handler.new_plot_data, sizeof(plot_handler.new_plot_data));
            /*解锁调度*/
            tos_knl_sched_unlock();
        }

        HAL_Delay(20);
    }
}

/*按键任务处理*/
void StartKeyTask(void *argument)
{
    int Count_Timer = 0 ;
    __IO uint8_t KeyCode = 255;
    static uint16_t count_sleep = 0 ;

    while(1)
    {
        /*获取键值*/
        GetKey(&KeyCode);

        if(255 != KeyCode)
        {
            /*有事件发生则清除休眠计数*/
            count_sleep = 0 ;
            Menu_Select_Item(Flow_Cursor.flow_cursor, KeyCode);
            KeyCode = 255 ;
        }

        /*如果当前在测试页面 && 开始检测标志为1，则进入传感器数据处理*/
        if(Flow_Cursor.flow_cursor == TEST_PAGE && Sensor_Flow_Cursor.Start_Detect == 1)
        {
            /*核心检测流程中清除休眠计数*/
            count_sleep = 0 ;
            ++Count_Timer ;

            if(Count_Timer == 20)
            {
                Count_Timer = 0 ;
                //mq2_sensor_interface.get_smoke_value(&mq2_sensor_interface);
                Sensor_Handler(Sensor_Flow_Cursor.Detect_Step, mq2_sensor_interface.Smoke_Value);
            }
        }

        /*1分钟内没有操作，则进入待机模式*/
        count_sleep++;

        if(count_sleep > 12000)
        {
            count_sleep = 0 ;
            sleep_mode();
        }

        /*如果在曲线显示模式下，则清除休眠计数*/
        if(1 == plot_handler.plot_mode)
            count_sleep = 0 ;

        tos_sleep_ms(5);
    }
}

/*状态栏任务显示处理*/
extern LCD_Ascii_Show_Para datatime_display_para ;
void StartStatus_Bar_Task(void *argument)
{
    while(1)
    {
        Get_Date_Time();

        if(0 == plot_handler.plot_mode)
        {
            sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%02d:%02d:%02d", \
                    DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec);
            lcd_model.lcd_driver->lcd_show_ascii_str(datatime_display_para);
        }

        tos_sleep_ms(1000);
    }
}


/*网络数据上传任务*/
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
    if(1 == User_Memory_Para.upload_flag && 1 == Sensor_Flow_Cursor.Is_safety_or_danger)
    {
        Sensor_Flow_Cursor.Is_safety_or_danger = 0 ;
        OneNet_Packet_Send(fd, ONENET_DEVICE_ID, ONENET_API_KEY, Sensor_Flow_Cursor.Alarm_Threshold);
    }
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

