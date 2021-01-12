#include "mqtt_iot_explorer_tc_ch20.h"
#include "tos_k.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"

#define WIFI_NAME								"602"
#define WIFI_PSW  							"13602584536"

//情书打印机
#define PRODUCT_ID              "WONYDFWVJO"
#define DEVICE_NAME             "Print_Device"
#define DEVICE_KEY              "sjNLp7TwCcK96zeTd5qRGg=="


/*打印机函数添加==>20210107*/
#include "mc_printf_str.h"
#include "lcd_driver_frame.h"

extern LCD_Driver_Model lcd_model ;

LCD_Chinese_Show_Para wait_connect_title_font[] =
{
    {33, 33, (uint8_t *)"等待连接", WHITE, BLACK, 32, 1},
    {33, 33, (uint8_t *)"等待连接", BLACK, BLACK, 32, 1},
};

LCD_Chinese_Show_Para connect_title_font[] =
{
    {33, 33, (uint8_t *)"正在连接", YELLOW, BLACK, 32, 1},
    {33, 33, (uint8_t *)"正在连接", BLACK, BLACK, 32, 1},
};

LCD_Chinese_Show_Para connect_success_font[] =
{
    {33, 33, (uint8_t *)"连接成功", GREEN, BLACK, 32, 1},
    {33, 33, (uint8_t *)"连接成功", BLACK, BLACK, 32, 1},
};

LCD_Chinese_Show_Para connect_error_font[] =
{
    {33, 33, (uint8_t *)"连接失败", RED, BLACK, 32, 1},
    {33, 33, (uint8_t *)"连接失败", BLACK, BLACK, 32, 1},
};

/*打开LED灯*/
void led_on_process(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/*关闭LED灯*/
void led_off_process(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void default_message_handler(mqtt_message_t* msg)
{
    printf("mqtt callback:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("\ttopic:%s\r\n", msg->topic);
    printf("\tpayload:%s\r\n", msg->payload);
    printf("---------------------------------------------------------\r\n");
    printf("即将解析下发的字符串：\r\n");
		//char *str_ptr = "\"power_switch\":";
		#define PRINT_STR "\"power_switch\":"
		char *str_ptr = strstr(msg->payload,PRINT_STR);
		if(strstr(str_ptr,"0"))
		{
			lcd_model.lcd_driver->lcd_clear(RED);
		}
		else if(strstr(str_ptr,"1"))
		{
			lcd_model.lcd_driver->lcd_clear(GREEN);
		}
}

char payload[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};

void mqtt_demo_task(void)
{
    int ret = 0;
    int size = 0;
    k_err_t err;
    mqtt_state_t state;
    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;
    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));
    printf("connecting....\n");
		//等待连接
		lcd_model.lcd_driver->lcd_show_chinese_str(wait_connect_title_font[0]);
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
		//等待连接==>正在连接
		lcd_model.lcd_driver->lcd_show_chinese_str(wait_connect_title_font[1]);
		lcd_model.lcd_driver->lcd_show_chinese_str(connect_title_font[0]);
    if (tos_tf_module_mqtt_conn(init_params) != 0)
    {
        printf("module mqtt conn fail\n");
				led_on_process();
				return ;
    }
    else
    {
        printf("module mqtt conn success\n");
    }

    if (tos_tf_module_mqtt_state_get(&state) != -1)
    {
        printf("MQTT: %s\n", state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    }
    /* 开始订阅topic */
    size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/down/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_reply_topic_name) - 1)
    {
        printf("sub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_reply_topic_name));
    }

    if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0, default_message_handler) != 0)
    {
        printf("module mqtt sub fail\n");
				led_on_process();
				//正在连接==>连接失败
				lcd_model.lcd_driver->lcd_show_chinese_str(connect_title_font[1]);
				lcd_model.lcd_driver->lcd_show_chinese_str(connect_error_font[0]);
				return ;
    }
    else
    {
        printf("module mqtt sub success\n");
				//正在连接==>连接成功
				lcd_model.lcd_driver->lcd_show_chinese_str(connect_title_font[1]);
				lcd_model.lcd_driver->lcd_show_chinese_str(connect_success_font[0]);
    }

    memset(report_topic_name, sizeof(report_topic_name), 0);
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1)
    {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }

    while (1)
    {
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
			tos_task_delay(500);
    }
}

void uploader_task(void *arg)
{
    mqtt_demo_task();
}

