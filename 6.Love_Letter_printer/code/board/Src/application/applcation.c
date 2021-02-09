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
* ��������: iot_explorer_handle_power_switch
* ˵    ��: ����power switch���ƿ���
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
* ��������: iot_explorer_handle_power_switch
* ˵    ��: ����power switch���Ʋʵƿ���
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
* ��������: iot_explorer_handle_week
* ˵    ��: ����week���ƴ�ӡ��
***************************************************************/
static void iot_explorer_handle_week(int week)
{
    /*�յ����飬�����������3����������*/
    uint8_t count = 0 ;

    for(count = 0 ; count < 6 ; count++)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        tos_task_delay(500);
    }

    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    /*���þ��ж���*/
    Printf_send_byte(0x1B);
    Printf_send_byte(0x61);
    Printf_send_byte(0x49);
    Printf_Str("һ������ for you\r\n\r\n");
    Printf_Str("�������ֻ��һ���ʱ��\r\n");
    Printf_Str("�����������ʮ�ĸ�Сʱ\r\n");
    Printf_Str("���峿����ҹ\r\n");
    Printf_Str("�������ֻ��һ�ܵ�ʱ��\r\n");
    Printf_Str("��������������\r\n");
    Printf_Str("����һ������\r\n\r\n");

    switch(week)
    {
        case 0:
            Printf_Str("���գ��ټ�\r\n");
            Printf_Str("��һ�����\r\n");
            Printf_Str("ֽ���鳤ѽ\r\n");
            Printf_Str("�߲��굱ʱ����\r\n");
            Printf_Str("�ҵĹ��»��ǹ�����ѽ\r\n");
            Printf_Str("���Ǳ���Ϊ������day����һ\r\n");
            Printf_Str("�������ô�Ҹе�ʹ��\r\n");
            Printf_Str("��Ϊ��Ҫ��ʼ��һ��\r\n");
            Printf_Str("������ϸϸ����ʱ\r\n");
            Printf_Str("��һ���Ѳ�֪�����ӽ�β��\r\n");
            Printf_Str("�µ�һ�����µ�һ�쿪ʼ\r\n");
            Printf_Str("����Ҫ���µ���̬ȫ�³���\r\n\r\n\r\n");
            break ;

        case 1:
            Printf_Str("��һ���ټ�\r\n");
            Printf_Str("�ܶ������\r\n");
            Printf_Str("����������ʲô��\r\n");
            Printf_Str("�Ƿ����������\r\n");
            Printf_Str("�Ƿ���ٳ�����\r\n");
            Printf_Str("�Ƿ���ͼ�������\r\n");
            Printf_Str("����ÿ������\r\n");
            Printf_Str("�ڴ�������Ĳ��ڶ���\r\n\r\n\r\n");
            break ;

        case 2:
            Printf_Str("�ܶ����ټ�\r\n");
            Printf_Str("���������\r\n");
            Printf_Str("���˷ݶ���һ�ļ��������\r\n");
            Printf_Str("���˷ݶ���ĩ���ڴ�������\r\n");
            Printf_Str("�����Ǹ��ʺϳ����Լ�������\r\n");
            Printf_Str("�ص��ֻ� �������\r\n");
            Printf_Str("��һ�������ĵط�\r\n");
            Printf_Str("��һ���Լ�ϲ������\r\n");
            Printf_Str("���Լ����������е���һ������\r\n");
            Printf_Str("��֪ʶ��������Ұ\r\n\r\n\r\n");
            break ;

        case 3:
            Printf_Str("�������ټ�\r\n");
            Printf_Str("���ģ����\r\n");
            Printf_Str("���Ƿ绪��ï\r\n");
            Printf_Str("������ǻ����\r\n");
            Printf_Str("���Ǵ����Լ���Ŀ��\r\n");
            Printf_Str("��Ӧ�ý�̤ʵ�� �ڷܿ̿�\r\n");
            Printf_Str("�òŻ�ȥ֧�����Ұ��\r\n");
            Printf_Str("������ȥ��Ԧ�������\r\n");
            Printf_Str("��ƴ��ȥ��ȡ���δ��\r\n\r\n\r\n");
            break ;

        case 4:
            Printf_Str("���ģ��ټ�\r\n");
            Printf_Str("���壬���\r\n");
            Printf_Str("������æµ��������\r\n");
            Printf_Str("�ڴ��ļ���Ҳ��������\r\n");
            Printf_Str("��������������ʵ���Լ�\r\n");
            Printf_Str("��Ҫ�±���������\r\n");
            Printf_Str("��Ҫί�����Լ�\r\n");
            Printf_Str("ȥ�ú�����\r\n");
            Printf_Str("ȥӵ������\r\n\r\n\r\n");
            break ;

        case 5:
            Printf_Str("���壬�ټ�\r\n");
            Printf_Str("���������\r\n");
            Printf_Str("��ĩ���ڶ���\r\n");
            Printf_Str("��ʱ��ǡ�� ΢�粻��\r\n");
            Printf_Str("��������� �߳�У԰\r\n");
            Printf_Str("ȥ����У������\r\n");
            Printf_Str("ȥ����У��ķ羰\r\n");
            Printf_Str("ȥ�������е�����\r\n\r\n\r\n");
            break ;

        case 6:
            Printf_Str("�������ټ�\r\n");
            Printf_Str("���գ����\r\n");
            Printf_Str("һ�ܼ�������\r\n");
            Printf_Str("��һ�ܵ������ջ���ʲô\r\n");
            Printf_Str("֪ʶ����Ұ��\r\n");
            Printf_Str("�µ�һ�ܼ�����ʼ\r\n");
            Printf_Str("���Ƿ����������\r\n");
            Printf_Str("ȥӭ����һ����ʼ\r\n\r\n\r\n");
            break ;

        default:
            break ;
    }
}

/***************************************************************
* ��������: iot_explorer_handle_string
* ˵    ��: ����string���ƴ�ӡ����ӡ�ַ���
***************************************************************/
static void iot_explorer_handle_string(char *str)
{
    /*�յ����飬�����������3����������*/
    uint8_t count = 0 ;

    for(count = 0 ; count < 6 ; count++)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        tos_task_delay(500);
    }

    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    printf("�ƶ�����:%s\n", str);
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)str, 16);
}

/***************************************************************
* ��������: default_message_handler
* ˵    ��: IoT Explorer�������ݴ���
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
    // 1. �������ƶ��յ��Ŀ�����Ϣ��ʾ��������ϢΪ
    //"{"method":"control","clientToken":"clientToken-Xx-N_enad","params":{"power_switch":0}}"
    root = cJSON_Parse(msg->payload + 1);

    if (!root)
    {
        printf("Invalid json root\r\n");
        return;
    }

    // 2. ������method
    method = cJSON_GetObjectItem(root, "method");

    if (!method)
    {
        printf("Invalid json method\r\n");
        cJSON_Delete(root);
        return;
    }

    // 3. �������ƶ��·��� control ���ݣ�report_reply�ݲ�����
    if (0 != strncmp(method->valuestring, "control", sizeof("control") - 1))
    {
        cJSON_Delete(root);
        return;
    }

    // 4. ������params
    params = cJSON_GetObjectItem(root, "params");

    if (!params)
    {
        printf("Invalid json params\r\n");
        cJSON_Delete(root);
        return;
    }

    // 1. ������ power_switch
    power_switch = cJSON_GetObjectItem(params, "power_switch");

    // 2. ���� power_switch ����ʵ��Ӳ������
    if (power_switch)
    {
        iot_explorer_handle_power_switch(power_switch->valueint);
    }
		
		 // 1. ������ power_switch
    power_switch1 = cJSON_GetObjectItem(params, "power_switch1");

    // 2. ���� power_switch ����ʵ��Ӳ������
    if (power_switch1)
    {
        iot_explorer_handle_power_switch1(power_switch1->valueint);
    }

    // 1. ������ week_love
    week_love = cJSON_GetObjectItem(params, "week");

    // 2. ����week���ƴ�ӡ����ӡ����
    if(week_love)
    {
        iot_explorer_handle_week(week_love->valueint);
    }

    // 1. ������ cloud_print
    cloud_print = cJSON_GetObjectItem(params, "cloud_print");

    // 2. ����week���ƴ�ӡ����ӡ����
    if(cloud_print)
    {
        iot_explorer_handle_string(cloud_print->valuestring);
    }

    cJSON_Delete(root);
}

/***************************************************************
* ��������: mqtt_demo_task
* ˵    ��: ����������ƽ̨�Լ�������������
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
    // 1. ��ʼ��ESP8266
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);

    if (ret < 0)
    {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }

    // 2. ����wifi�������Ҫ���������ȵ����wifi�����ע��
    esp8266_tencent_firmware_join_ap(YOUR_WIFI_SSID, YOUR_WIFI_PWD);

    // 3. �����豸��Ϣ����ƷID���豸�����豸��Կ
    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

    // 4. ����IoT Explorer
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

    // 5. ��������ģ�� topic
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
        // ��ȡ����ǿ�ȣ����ݴ���� e53_value
        e53_value = e53_scl_read_data();
        printf("e53 value %.0f lx\r\n", e53_value);

        memset(e53_str, 0, sizeof(e53_str));
        sprintf(e53_str, "%.0f lx(lm/m2)", e53_value);
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"TencentOS-tiny", 16);
        OLED_ShowString(0, 2, (uint8_t*)e53_str, 16);

        // 1. �����ϱ���JSON
        memset(payload, 0, sizeof(payload));
        snprintf(payload, sizeof(payload), REPORT_LX_DATA_TEMPLATE, e53_value);

        // 2. ������ģ�� topic ��������ǿ����Ϣ
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
* ��������: application_entry
* ˵    ��: Ӧ�����
***************************************************************/
void application_entry(void *arg)
{
    char *str = "TencentOS-tiny";
    // ��ʼ��tencent-os tiny��ʱ��ģ��
    timer_init();
    // ��ʼ�����Լ�����ǿ�ȼ��ģ��
    e53_sc1_init();
    // OLED ��ʾ����ʼ��
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)str, 16);
    /*��ʼ����ӡ��*/
    init_print_device();
    //Printf_Str("Hello World\r\n\r\nTencentOS-tiny\r\n\r\n\r\n");
    mqtt_demo_task();

    while (1)
    {
        /*��ת�������λ����*/
        NVIC_SystemReset();
        printf("This is a demo\r\n");
        tos_task_delay(10000);
    }
}

