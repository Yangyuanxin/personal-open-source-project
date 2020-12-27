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

FATFS fs;						  /* FatFs文件系统对象 */
FRESULT f_res;                    /* 文件操作结果 */
#define START_LOGO	"0:/UI/start_logo/start_logo.bmp"
LCD_Driver_Model lcd_model ;


#define TASK_TIMESLICE     5

//1
/***************按键处理任务*************/
#define KEY_TASK_PRIORITY    	 3
#define KEY_TASK_SIZE       	 1500
static rt_thread_t key_task_thread = RT_NULL;
static void Start_Key_Task(void *parameter);
/***************按键处理任务*************/

//2
/***************传感器任务处理*************/
#define SENSOR_PRIORITY   	 						4
#define SENSOR_TASK_SIZE       					1024
rt_sem_t sensor_data_sem = RT_NULL;
static rt_thread_t sensor_task_thread = RT_NULL;
/*状态栏更新线程入口函数 */
static void StartSensor_Task(void *parameter);
/***************传感器任务处理*************/


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
        return ;
    }

    /*读取配置信息*/
    Load_Config_ini_File();
    lcd_model.lcd_driver->Lcd_show_bmp(boot_logo_para);
    lcd_model.lcd_driver->lcd_display_onoff(1);
    rt_thread_mdelay(3000);
    lcd_model.lcd_driver->lcd_clear(BLACK);
    Menu_Init();
    //关指示灯
    HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
}

/*启动操作系统*/
void start_rt_thread_os(void)
{
    /*1、创建按键线程*/
    key_task_thread = rt_thread_create("key_th",
                                       Start_Key_Task, RT_NULL,
                                       KEY_TASK_SIZE,
                                       KEY_TASK_PRIORITY, TASK_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (key_task_thread != RT_NULL)
        rt_thread_startup(key_task_thread);

    /*1、创建传感器线程*/
    sensor_task_thread = rt_thread_create("sensor_th",
                                          StartSensor_Task, RT_NULL,
                                          SENSOR_TASK_SIZE,
                                          SENSOR_PRIORITY, TASK_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (sensor_task_thread != RT_NULL)
        rt_thread_startup(sensor_task_thread);
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
extern Dart_Sensor sensor ;
extern rt_sem_t sensor_data_sem ;
static void StartSensor_Task(void *parameter)
{
    uint8_t i = 0 ;
    static rt_err_t result;
    uint8_t sensor_recv_buf[SENSOR_U3_BUFFER_SIZE] = {0};
    /* 创建一个动态信号量，初始值是 0 */
    sensor_data_sem = rt_sem_create("sensor_sem", 0, RT_IPC_FLAG_FIFO);

    if (sensor_data_sem == RT_NULL)
    {
        rt_kprintf("create sensor_sem failed.\n");
        return ;
    }

    /*传感器注册*/
    Sensor_Register(&Formaldehyde_Sensor_interface);

    /*获取传感器数据*/
    while(1)
    {
        /*获取传感器信号量*/
        result = rt_sem_take(sensor_data_sem, RT_WAITING_FOREVER);

        if (RT_EOK == result)
        {
            memcpy(sensor_recv_buf, dart_sensor_handler.SensorU3Buffer, SENSOR_U3_BUFFER_SIZE);

            /*判断包头数据是否正确*/
            if(sensor_recv_buf[0] == 0xFF && sensor_recv_buf[1] == 0x17)
            {
                sensor = Get_Dart_Sensor_Density(sensor_recv_buf);

                /*更新数据到队列*/
                for(i = 0 ; i <= DATA_SIZE - 2 ; i++)
                    plot_handler.rel_data_data[i] = plot_handler.rel_data_data[i + 1];

                plot_handler.rel_data_data[DATA_SIZE - 1] = sensor.gas_density ;
                Formaldehyde_Sensor_interface.Formaldehyde_Value = plot_handler.rel_data_data[DATA_SIZE - 1] ;
                Formaldehyde_Sensor_interface.Formaldehyde_Max_Value = find_max_value(plot_handler.rel_data_data, DATA_SIZE);
                Formaldehyde_Sensor_interface.Formaldehyde_Min_Value = find_min_value(plot_handler.rel_data_data, DATA_SIZE);
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

            memset(dart_sensor_handler.SensorU3Buffer, 0, SENSOR_U3_BUFFER_SIZE);
            HAL_UART_Receive_DMA(&huart3, (uint8_t*)dart_sensor_handler.SensorU3Buffer, SENSOR_U3_BUFFER_SIZE);
        }
    }
}
