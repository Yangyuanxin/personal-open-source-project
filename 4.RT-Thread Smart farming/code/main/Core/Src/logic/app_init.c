#include "app_init.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "i2c.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_bmp.h"
#include "stm32l4xx_it.h"

FATFS fs;						  /* FatFs文件系统对象 */
FRESULT f_res;                    /* 文件操作结果 */
#define START_LOGO	"0:/UI/start_logo/start_logo.bmp"
LCD_Driver_Model lcd_model ;

#define TASK_TIMESLICE     5

//1
/***************按键处理任务*************/
#define KEY_TASK_PRIORITY    	 3
#define KEY_TASK_SIZE       	 2000
static rt_thread_t key_task_thread = RT_NULL;
static void Start_Key_Task(void *parameter);
/***************按键处理任务*************/

//2
/***************传感器任务处理*************/
#define SENSOR_PRIORITY   	 						4
#define SENSOR_TASK_SIZE       					2048
rt_sem_t sensor_data_sem = RT_NULL;
static rt_thread_t sensor_task_thread = RT_NULL;
/*状态栏更新线程入口函数 */
static void StartSensor_Task(void *parameter);
/***************传感器任务处理*************/


//2
/***************控制任务处理*************/
#define CONTROL_PRIORITY   	 						5
#define CONTROL_TASK_SIZE       				2048
static rt_thread_t control_task_thread = RT_NULL;
/*控制任务更新线程入口函数 */
static void StartControl_Task(void *parameter);
/***************控制任务处理*************/

LCD_Bmp_Show_Para boot_logo_para = {0, 0, START_LOGO};

/*硬件环境初始化*/
void hardware_init(void)
{
    int ret = -1 ;
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_RTC_Init();
    MX_ADC1_Init();
    MX_TIM16_Init();
    MX_I2C1_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    Register_Driver_Model(&lcd_model);
    //PowerOn();
    lcd_model.lcd_driver->lcd_init();
    ret = Mount_SD();

    if(ret != 0)
    {
        printf("SD Card mount ERROR\r\n");
        HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);
        return ;
    }

    /*读取配置信息*/
    Load_Config_ini_File();
    lcd_model.lcd_driver->Lcd_show_bmp(boot_logo_para);
    lcd_model.lcd_driver->lcd_display_onoff(1);
    /*1、创建传感器线程*/
    sensor_task_thread = rt_thread_create("sensor_th",
                                          StartSensor_Task, RT_NULL,
                                          SENSOR_TASK_SIZE,
                                          SENSOR_PRIORITY, TASK_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (sensor_task_thread != RT_NULL)
        rt_thread_startup(sensor_task_thread);

    rt_thread_mdelay(2000);
    lcd_model.lcd_driver->lcd_clear(BLACK);
}

/*启动其它任务*/
void start_other_rt_thread(void)
{
    /*1、创建按键线程*/
    key_task_thread = rt_thread_create("key_th",
                                       Start_Key_Task, RT_NULL,
                                       KEY_TASK_SIZE,
                                       KEY_TASK_PRIORITY, TASK_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (key_task_thread != RT_NULL)
        rt_thread_startup(key_task_thread);

    /*2、创建控制线程*/
    control_task_thread = rt_thread_create("con_th",
                                           StartControl_Task, RT_NULL,
                                           CONTROL_TASK_SIZE,
                                           CONTROL_PRIORITY, TASK_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (control_task_thread != RT_NULL)
        rt_thread_startup(control_task_thread);

    Menu_Init();
    //关指示灯
    HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_RESET);
}

/*按键任务处理*/
static void Start_Key_Task(void *parameter)
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

        rt_thread_mdelay(5);
    }
}


/*传感器任务处理*/
static void StartSensor_Task(void *parameter)
{
    Register_Sensor();

    while(1)
    {
        Sensor_Logic_Running();
        rt_thread_mdelay(500);
    }
}

/*控制任务更新线程入口函数 */
static void StartControl_Task(void *parameter)
{
    display_fan(4);
    display_led(0);
    display_hum(0);
    display_temp(0);

    while(1)
    {
        if(MAIN_PAGE == Flow_Cursor.flow_cursor)
            refresh_all_ui();

        rt_thread_mdelay(50);
    }
}
