#include "applcation.h"
#include "mc_printf_str.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"


char payload[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};

int  brightness_level_cache = 2;
char client_token_cache[128] = {0};
extern __IO uint8_t Controld_Color_LED;

/***************************************************************
* 函数名称: iot_explorer_handle_power_switch
* 说    明: 根据power switch控制开关
***************************************************************/
static void iot_explorer_handle_power_switch(int power_switch)
{
    if (0 == power_switch)
    {
        printf("iot-explorer close the light\r\n");
        LED_CLOSE;
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				OLED_Clear();
				OLED_ShowString(0, 0, (uint8_t*)"Close Led", 16);
    }
    else
    {
        printf("iot-explorer open the light\r\n");
        LED_OPEN;
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				OLED_Clear();
				OLED_ShowString(0, 0, (uint8_t*)"Open Led", 16);
    }
}

/***************************************************************
* 函数名称: iot_explorer_handle_power_switch
* 说    明: 根据power switch控制彩灯开关
***************************************************************/
static void iot_explorer_handle_power_switch1(int power_switch)
{
    if (0 == power_switch)
    {
        printf("iot-explorer close the color light\r\n");
        Controld_Color_LED = 0 ;
				OLED_Clear();
				OLED_ShowString(0, 0, (uint8_t*)"Close Color Led", 16);
    }
    else
    {
        printf("iot-explorer open the color light\r\n");
				Controld_Color_LED = 1 ;
				OLED_Clear();
				OLED_ShowString(0, 0, (uint8_t*)"Open Color Led", 16);
    }
}

/***************************************************************
* 函数名称: iot_explorer_handle_week
* 说    明: 根据week控制打印机
***************************************************************/
static void iot_explorer_handle_week(int week)
{
    /*收到情书，则蜂鸣器鸣叫3声进行提醒*/
    uint8_t count = 0 ;

    for(count = 0 ; count < 6 ; count++)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        tos_task_delay(500);
    }

    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    /*设置居中对齐*/
    Printf_send_byte(0x1B);
    Printf_send_byte(0x61);
    Printf_send_byte(0x49);
    Printf_Str("一周情书 for you\r\n\r\n");
    Printf_Str("如果我们只有一天的时间\r\n");
    Printf_Str("那我想陪你二十四个小时\r\n");
    Printf_Str("从清晨到午夜\r\n");
    Printf_Str("如果我们只有一周的时间\r\n");
    Printf_Str("那我想陪你七天\r\n");
    Printf_Str("从周一到周日\r\n\r\n");

    switch(week)
    {
        case 0:
            Printf_Str("周日，再见\r\n");
            Printf_Str("周一，你好\r\n");
            Printf_Str("纸短情长呀\r\n");
            Printf_Str("诉不完当时年少\r\n");
            Printf_Str("我的故事还是关于你呀\r\n");
            Printf_Str("总是被成为最不想面对day的周一\r\n");
            Printf_Str("往往会让大家感到痛苦\r\n");
            Printf_Str("因为又要开始熬一周\r\n");
            Printf_Str("但待你细细回想时\r\n");
            Printf_Str("这一天已不知不觉接近尾声\r\n");
            Printf_Str("新的一周以新的一天开始\r\n");
            Printf_Str("我们要以新的姿态全新出发\r\n\r\n\r\n");
            break ;

        case 1:
            Printf_Str("周一，再见\r\n");
            Printf_Str("周二，你好\r\n");
            Printf_Str("今天你做了什么？\r\n");
            Printf_Str("是否与教室相遇\r\n");
            Printf_Str("是否与操场相遇\r\n");
            Printf_Str("是否与图书馆相遇\r\n");
            Printf_Str("矿大的每个角落\r\n");
            Printf_Str("期待着与你的不期而遇\r\n\r\n\r\n");
            break ;

        case 2:
            Printf_Str("周二，再见\r\n");
            Printf_Str("周三，你好\r\n");
            Printf_Str("少了份对周一的激情与活力\r\n");
            Printf_Str("多了份对周末的期待和企盼\r\n");
            Printf_Str("周三是个适合沉淀自己的日子\r\n");
            Printf_Str("关掉手机 背上书包\r\n");
            Printf_Str("找一个安静的地方\r\n");
            Printf_Str("捧一本自己喜欢的书\r\n");
            Printf_Str("将自己置身于书中的另一个世界\r\n");
            Printf_Str("用知识来开阔视野\r\n\r\n\r\n");
            break ;

        case 3:
            Printf_Str("周三，再见\r\n");
            Printf_Str("周四，你好\r\n");
            Printf_Str("我们风华正茂\r\n");
            Printf_Str("我们满腔热情\r\n");
            Printf_Str("我们带着自己的目标\r\n");
            Printf_Str("更应该脚踏实地 勤奋刻苦\r\n");
            Printf_Str("用才华去支撑你的野心\r\n");
            Printf_Str("用能力去驾驭你的梦想\r\n");
            Printf_Str("用拼搏去争取你的未来\r\n\r\n\r\n");
            break ;

        case 4:
            Printf_Str("周四，再见\r\n");
            Printf_Str("周五，你好\r\n");
            Printf_Str("连续的忙碌即将结束\r\n");
            Printf_Str("期待的假期也即将到来\r\n");
            Printf_Str("今天我们做最真实的自己\r\n");
            Printf_Str("不要怕被生活轻视\r\n");
            Printf_Str("不要委屈了自己\r\n");
            Printf_Str("去好好生活\r\n");
            Printf_Str("去拥抱人生\r\n\r\n\r\n");
            break ;

        case 5:
            Printf_Str("周五，再见\r\n");
            Printf_Str("周六，你好\r\n");
            Printf_Str("周末如期而至\r\n");
            Printf_Str("趁时光恰好 微风不燥\r\n");
            Printf_Str("找三两伙伴 走出校园\r\n");
            Printf_Str("去看看校外的天空\r\n");
            Printf_Str("去欣赏校外的风景\r\n");
            Printf_Str("去享受旅行的意义\r\n\r\n\r\n");
            break ;

        case 6:
            Printf_Str("周六，再见\r\n");
            Printf_Str("周日，你好\r\n");
            Printf_Str("一周即将结束\r\n");
            Printf_Str("这一周的你又收获了什么\r\n");
            Printf_Str("知识？视野？\r\n");
            Printf_Str("新的一周即将开始\r\n");
            Printf_Str("你是否整理好心情\r\n");
            Printf_Str("去迎接下一个开始\r\n\r\n\r\n");
            break ;

        default:
            break ;
    }
}

/***************************************************************
* 函数名称: iot_explorer_handle_string
* 说    明: 根据string控制打印机打印字符串
***************************************************************/
static void iot_explorer_handle_string(char *str)
{
    /*收到情书，则蜂鸣器鸣叫3声进行提醒*/
    uint8_t count = 0 ;

    for(count = 0 ; count < 6 ; count++)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        tos_task_delay(500);
    }

    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    printf("云端数据:%s\n", str);
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)str, 16);
}

/***************************************************************
* 函数名称: default_message_handler
* 说    明: IoT Explorer下行数据处理
***************************************************************/
static void default_message_handler(mqtt_message_t* msg)
{
    cJSON *root;
    cJSON *params;
    cJSON *method;
    cJSON *week_love;
    cJSON *power_switch;
		cJSON *power_switch1;
    cJSON *cloud_print ;
    // 1. 解析从云端收到的控制信息，示例控制信息为
    //"{"method":"control","clientToken":"clientToken-Xx-N_enad","params":{"power_switch":0}}"
    root = cJSON_Parse(msg->payload + 1);

    if (!root)
    {
        printf("Invalid json root\r\n");
        return;
    }

    // 2. 解析出method
    method = cJSON_GetObjectItem(root, "method");

    if (!method)
    {
        printf("Invalid json method\r\n");
        cJSON_Delete(root);
        return;
    }

    // 3. 仅处理云端下发的 control 数据，report_reply暂不处理
    if (0 != strncmp(method->valuestring, "control", sizeof("control") - 1))
    {
        cJSON_Delete(root);
        return;
    }

    // 4. 解析出params
    params = cJSON_GetObjectItem(root, "params");

    if (!params)
    {
        printf("Invalid json params\r\n");
        cJSON_Delete(root);
        return;
    }

    // 1. 解析出 power_switch
    power_switch = cJSON_GetObjectItem(params, "power_switch");

    // 2. 根据 power_switch 控制实际硬件开关
    if (power_switch)
    {
        iot_explorer_handle_power_switch(power_switch->valueint);
    }
		
		 // 1. 解析出 power_switch
    power_switch1 = cJSON_GetObjectItem(params, "power_switch1");

    // 2. 根据 power_switch 控制实际硬件开关
    if (power_switch1)
    {
        iot_explorer_handle_power_switch1(power_switch1->valueint);
    }

    // 1. 解析出 week_love
    week_love = cJSON_GetObjectItem(params, "week");

    // 2. 根据week控制打印机打印情书
    if(week_love)
    {
        iot_explorer_handle_week(week_love->valueint);
    }

    // 1. 解析出 cloud_print
    cloud_print = cJSON_GetObjectItem(params, "cloud_print");

    // 2. 根据week控制打印机打印情书
    if(cloud_print)
    {
        iot_explorer_handle_string(cloud_print->valuestring);
    }

    cJSON_Delete(root);
}

/***************************************************************
* 函数名称: mqtt_demo_task
* 说    明: 主任务，连接平台以及发送上行数据
***************************************************************/
static void mqtt_demo_task(void)
{
    int ret = 0;
    int size = 0;
    mqtt_state_t state;

    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;

    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));

    float e53_value = 0;
    char e53_str[16] = {0};

    OLED_ShowString(0, 2, (uint8_t*)"connecting...", 16);

    /**
     * Please Choose your AT Port first, default is HAL_UART_2(USART2)
    */
    // 1. 初始化ESP8266
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);

    if (ret < 0)
    {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }

    // 2. 连接wifi，如果需要自行配置热点或者wifi，请打开注释
    esp8266_tencent_firmware_join_ap(YOUR_WIFI_SSID, YOUR_WIFI_PWD);

    // 3. 设置设备信息：产品ID，设备名，设备密钥
    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

    // 4. 连接IoT Explorer
    mqtt_param_t init_params = DEFAULT_MQTT_PARAMS;

    if (tos_tf_module_mqtt_conn(init_params) != 0)
    {
        printf("module mqtt conn fail\n");
    }
    else
    {
        printf("module mqtt conn success\n");
    }

    if (tos_tf_module_mqtt_state_get(&state) != -1)
    {
        printf("MQTT: %s\n", state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    }

    // 5. 订阅数据模板 topic
    size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/down/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_reply_topic_name) - 1)
    {
        printf("sub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_reply_topic_name));
    }

    if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0, default_message_handler) != 0)
    {
        printf("module mqtt sub fail\n");
    }
    else
    {
        printf("module mqtt sub success\n");
    }

    memset(report_topic_name, 0, sizeof(report_topic_name));
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1)
    {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }

    while (1)
    {
        // 读取光照强度，数据存放在 e53_value
        e53_value = e53_scl_read_data();
        printf("e53 value %.0f lx\r\n", e53_value);

        memset(e53_str, 0, sizeof(e53_str));
        sprintf(e53_str, "%.0f lx(lm/m2)", e53_value);
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"TencentOS-tiny", 16);
        OLED_ShowString(0, 2, (uint8_t*)e53_str, 16);

        // 1. 构造上报的JSON
        memset(payload, 0, sizeof(payload));
        snprintf(payload, sizeof(payload), REPORT_LX_DATA_TEMPLATE, e53_value);

        // 2. 向数据模板 topic 发布光照强度信息
        if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
        {
            printf("module mqtt pub fail\n");
            break;
        }
        else
        {
            printf("module mqtt pub success\n");
        }

        tos_sleep_ms(7000);
    }
}


/***************************************************************
* 函数名称: application_entry
* 说    明: 应用入口
***************************************************************/
void application_entry(void *arg)
{
    char *str = "TencentOS-tiny";
    // 初始化tencent-os tiny定时器模块
    timer_init();
    // 初始化灯以及光照强度检测模块
    e53_sc1_init();
    // OLED 显示屏初始化
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)str, 16);
    /*初始化打印机*/
    init_print_device();
    //Printf_Str("Hello World\r\n\r\nTencentOS-tiny\r\n\r\n\r\n");
    mqtt_demo_task();

    while (1)
    {
        /*跳转到这里，复位重启*/
        NVIC_SystemReset();
        printf("This is a demo\r\n");
        tos_task_delay(10000);
    }
}

