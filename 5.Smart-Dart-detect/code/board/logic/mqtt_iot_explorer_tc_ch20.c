#include "mqtt_iot_explorer_tc_ch20.h"
#include "tos_k.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"

#define WIFI_NAME								"602"
#define WIFI_PSW  							"13602584536"

#define PRODUCT_ID              "TZN8H2UI6W"
#define DEVICE_NAME             "dev001"
#define DEVICE_KEY              "jVTLBIkmxm56gKNqK+maJA=="

/*WIFI���ӱ�־*/
__IO uint8_t wifi_connect_flag = 0 ;
/*���������ӱ�־*/
__IO uint8_t server_connect_flag = 0 ;

//��λ��ppm
#define REPORT_DATA_TEMPLATE    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"ch20_ppm_value\\\":%.3f}}"
//��λ��mg/m3
#define REPORT_DATA_MG_M3TEMPLATE    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"ch20_mgm3_value\\\":%.3f}}"

/*��ӡ���������==>20210107*/
#include "mc_printf_str.h"
double  ch20_ppm_value_printf;
char payload_str[256] = {0};
char payload_str_copy[256] = {0};
char printf_buffer[1024] = {0};
char printf_sensor_value[20] = {0};
void default_message_handler(mqtt_message_t* msg)
{
    __IO static uint8_t flag = 0 ;
    printf("mqtt callback:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("\ttopic:%s\r\n", msg->topic);
    printf("\tpayload:%s\r\n", msg->payload);
    printf("---------------------------------------------------------\r\n");
    flag = 0 ;
    printf("���������·����ַ�����\r\n");

    if(strstr(msg->payload, "success"))
    {
        printf("�����ϴ���ƽ̨�ɹ�\n");
    }
    else
    {
        memset(payload_str, 0, 256);
        memset(payload_str_copy, 0, 256);
        memset(printf_sensor_value, 0, 20);
        char *ptr = NULL ;
        char *ptr1 = NULL ;
        /*�ҵ����һ���ֺ�*/
        int str_len = 0 ;
        char *ptr_tail = NULL ;
        memcpy(payload_str, msg->payload, strlen(msg->payload));
        ptr = strstr(payload_str, "\"cloud_print\":\"");
        ptr1 = ptr + 15;
        ptr_tail = strstr(ptr1, "\"");
        str_len = ptr_tail - ptr1 ;
        memcpy(payload_str_copy, ptr1, str_len);
        printf("�ַ���:%s,����:%d\n", payload_str_copy, str_len);
        sprintf(printf_buffer, "%s\r\n\r\n\r\n\r\n\r\n", payload_str_copy);
        /*����*/
        Sel_Align_Way(1);
        Printf_Str("���ؼ�ȩ̽��\r\n");
        Printf_Str("����:��Դ��\r\n");
        Printf_Str("Ӳ��ƽ̨:С����\r\n");
        Printf_Str("����ϵͳ:TencentOS tiny\r\n");
        /*�����*/
        Sel_Align_Way(0);
        sprintf(printf_sensor_value, "\r\n��ȩֵ��%.3fppm or %.3fmg/m3\r\n", ch20_ppm_value_printf, ch20_ppm_value_printf * 0.74666);
        Printf_Str(printf_sensor_value);
        memset(payload_str, 0, 256);
        sprintf(payload_str, "��Ѷ�ƶ˽���:%s\r\n", payload_str_copy);
        printf("payload_str_copy:%s\n", payload_str_copy);

        if(strstr(payload_str_copy, "led_on"))
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        }
        else if(strstr(payload_str_copy, "led_off"))
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }
        else if(strstr(payload_str_copy, "reboot"))
        {
            flag = 1 ;
            printf("�豸����\n");
        }
        else
        {
            printf("��лʹ����Ѷ��IoT explorer��ƽ̨\n");
        }

        Printf_Str(payload_str);
        memset(payload_str, 0, 256);
        sprintf(payload_str, "����汾:%s %s\r\n\r\n\r\n", __DATE__, __TIME__);
        Printf_Str(payload_str);

        if(1 == flag)
            NVIC_SystemReset();
    }
}

char payload[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};


void mqtt_demo_task(void)
{
    int ret = 0;
    int size = 0;
    int lightness = 0;
    k_err_t err;
    void *msg_received;
    char ch20_str[20] = {0};
    mqtt_state_t state;

    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;

    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));
    double  ch20_ppm_value;
    double  ch20_mgm3_value;

    /* OLED��ʾ��־ */
    printf("connecting....\n");
    wifi_connect_flag = 0 ;
    server_connect_flag = 0 ;
    /**
     * Please Choose your AT Port first, default is HAL_UART_PORT_0(LPUART1)
    */
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_0);

    if (ret < 0)
    {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }

    esp8266_tencent_firmware_join_ap(WIFI_NAME, WIFI_PSW);

    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

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

        /*���wifi���ӳɹ�,����ʾ����״̬��һ*/
        if(MQTT_STATE_CONNECTED == state)
            wifi_connect_flag = 1 ;
        else
            wifi_connect_flag = 0 ;
    }

    /* ��ʼ����topic */
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

    memset(report_topic_name, sizeof(report_topic_name), 0);
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1)
    {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }

    extern k_msg_q_t uploader_data_msg;
    /*��������Ѷ�Ʒ�����*/
    wifi_connect_flag = 1 ;
    server_connect_flag = 1 ;

    while (1)
    {

        err = tos_msg_q_pend(&uploader_data_msg, &msg_received, TOS_TIME_FOREVER);

        if (err == K_ERR_NONE)
        {
            /*�����յ������ݿ�����ch20_value����*/
            memcpy(ch20_str, msg_received, 20);

            /*������յ��ĵ�ǰ��λ��ppm*/
            if(strstr(ch20_str, "ppm:"))
            {
                ch20_ppm_value = atof(ch20_str + strlen("ppm:"));
                /* �ϱ�ֵ */
                memset(payload, 0, sizeof(payload));
                /*��ǰ�ϱ���λΪppm*/
                snprintf(payload, sizeof(payload), REPORT_DATA_TEMPLATE, ch20_ppm_value);

                if (lightness > 100)
                {
                    lightness = 0;
                }

                if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
                {
                    printf("module mqtt pub fail\n");
                    break;
                }
                else
                {
                    printf("module mqtt pub success\n");
                }
            }
            /*������յ��ĵ�ǰ��λ��mgm3*/
            else if(strstr(ch20_str, "mgm3:"))
            {
                ch20_mgm3_value = atof(ch20_str + strlen("mgm3:"));
                /* �ϱ�ֵ */
                memset(payload, 0, sizeof(payload));
                /*��ǰ�ϱ���λΪppm*/
                snprintf(payload, sizeof(payload), REPORT_DATA_MG_M3TEMPLATE, ch20_mgm3_value);

                if (lightness > 100)
                {
                    lightness = 0;
                }

                if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
                {
                    /*�ϴ�ʧ�ܣ���������Ѷ�Ʒ������Ͽ�����*/
                    server_connect_flag = 0 ;
                    printf("module mqtt pub fail\n");
                    break;
                }
                else
                {
                    /*��������Ѷ�Ʒ�����*/
                    wifi_connect_flag = 1 ;
                    server_connect_flag = 1 ;
                    printf("module mqtt pub success\n");

                }
            }
        }
    }
}

void uploader_task(void *arg)
{
    mqtt_demo_task();

    while (1)
    {
        printf("This is a mqtt demo!\r\n");
        tos_task_delay(1000);
    }
}

