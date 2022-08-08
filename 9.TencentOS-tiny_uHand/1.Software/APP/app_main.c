#include "app_main.h"

ap_info esp8266_info;

char payload[256];
char token_cache[128];
static char report_topic_name[TOPIC_NAME_MAX_SIZE];
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE];
static void WifiSmartConfig(void);
static void ReportClientToken(void);

void MessageParamsHandler(mqtt_message_t* msg)
{
	cJSON *root = NULL;
	cJSON *token = NULL;
	cJSON *params = NULL;
	cJSON *method = NULL;
	cJSON *reset_control = NULL;
	cJSON *thumbs = NULL;
	cJSON *index_finger = NULL;
	cJSON *middle_finger = NULL;
	cJSON *ring_finger = NULL;
	cJSON *little_finger = NULL;
	cJSON *wrist = NULL;
	printf("mqtt callback:\r\n");
	printf("---------------------------------------------------------\r\n");
	printf("\ttopic:%s\r\n", msg->topic);
	printf("\tpayload:%s\r\n", msg->payload);
	printf("---------------------------------------------------------\r\n");
	/*1. 解析从云端收到的控制信息*/
	root = cJSON_Parse(msg->payload + 1);
	if (!root)
	{
		printf("Invalid json root\r\n");
		return;
	}
	/*2. 解析method*/
	method = cJSON_GetObjectItem(root, "method");
	if (!method)
	{
		printf("Invalid json method\r\n");
		cJSON_Delete(root);
		return;
	}
	/*3. 仅处理云端下发的 control 数据，report_reply暂不处理*/
	if (0 != strncmp(method->valuestring, "control", strlen("control")))
	{
		cJSON_Delete(root);
		return;
	}
	/*4. 解析出params*/
	params = cJSON_GetObjectItem(root, "params");
	if (!params)
	{
		printf("Invalid json params\r\n");
		cJSON_Delete(root);
		return;
	}
	/*5. 根据params，解析"params":{"power_switch":0}*/
	reset_control = cJSON_GetObjectItem(params, "power_switch");
	if (reset_control)
	{
		if (reset_control->valueint)
		{
			DEBUG_LED(1)
			resetServo();
			sendAngleCmd(6, 2500);
		}
		else
		{
			DEBUG_LED(0)
		}
	}

	thumbs = cJSON_GetObjectItem(params, "thumbs");
	if (thumbs)
		sendAngleCmd(1, thumbs->valueint);

	index_finger = cJSON_GetObjectItem(params, "index_finger");
	if (index_finger)
		sendAngleCmd(2, index_finger->valueint);

	middle_finger = cJSON_GetObjectItem(params, "middle_finger");
	if (middle_finger)
		sendAngleCmd(3, middle_finger->valueint);

	ring_finger = cJSON_GetObjectItem(params, "ring_finger");
	if (ring_finger)
		sendAngleCmd(4, ring_finger->valueint);

	little_finger = cJSON_GetObjectItem(params, "little_finger");
	if (little_finger)
		sendAngleCmd(5, little_finger->valueint);

	wrist = cJSON_GetObjectItem(params, "wrist");
	if (wrist)
		sendAngleCmd(6, wrist->valueint);

	token = cJSON_GetObjectItem(root, "clientToken");
	if (token)
	{
		printf("clientToken reply!\n");
		ReportClientToken();
	}
	cJSON_Delete(root);
	root = NULL;
}

void MqttTask(void)
{
	int ret = 0;
	int size = 0;
	mqtt_state_t state;
	char *key = DEVICE_KEY;
	device_info_t dev_info;
	char *product_id = PRODUCT_ID;
	char *device_name = DEVICE_NAME;
	memset(&dev_info, 0, sizeof(device_info_t));
	LCD_ShowString(10, 10, "Wait Connecting...", WHITE, BLACK, 16, 0);
	ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);
	if (ret < 0)
	{
		printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
		NVIC_SystemReset();
	}
	tos_task_delay(6000);
	/*执行WIFI配网逻辑*/
	WifiSmartConfig();
	strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
	strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
	strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
	tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

	mqtt_param_t init_params = DEFAULT_MQTT_PARAMS;
	if (tos_tf_module_mqtt_conn(init_params) != 0)
	{
		printf("module mqtt conn fail\n");
		NVIC_SystemReset();
	}

	if (tos_tf_module_mqtt_state_get(&state) != -1)
	{
		printf("MQTT: %s\n",
				state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
	}

	/* 开始订阅topic */
	size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE,
			"$thing/down/property/%s/%s", product_id, device_name);
	if (size < 0 || size > sizeof(report_reply_topic_name) - 1)
	{
		printf(
				"sub topic content length not enough! content size:%d  buf size:%d",
				size, (int) sizeof(report_reply_topic_name));
		return;
	}
	if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0,
			MessageParamsHandler) != 0)
	{
		printf("module mqtt sub fail\n");
		NVIC_SystemReset();
	}

	memset(report_topic_name, 0, sizeof(report_topic_name));
	size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE,
			"$thing/up/property/%s/%s", product_id, device_name);
	if (size < 0 || size > sizeof(report_topic_name) - 1)
	{
		printf(
				"pub topic content length not enough! content size:%d  buf size:%d",
				size, (int) sizeof(report_topic_name));
	}

	LCD_ShowString(10, 10, "MQTT Connect OK", WHITE, BLACK, 16, 0);
	resetServo(); //让舵机复位
	sendAngleCmd(6, 2500);
	while (1)
	{

		osDelay(4000);
	}
}

//上报ClientToken
static void ReportClientToken(void)
{
	char buf[50];
	static uint32_t counter = 0;
	memset(buf, 0, 50);
	memset(payload, 0, 256);
	memset(token_cache, 0, 128);
	snprintf(token_cache, 128, "%d", counter++);
	snprintf(payload, 256, CONTROL_REPLY_DATA_TEMPLATE, token_cache);
	if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
		NVIC_SystemReset();
}

static void WifiSmartConfig(void)
{
	int rssi;
	char ssid[50];
	char bssid[50];
	int channel = -1;
	static uint8_t ConfigWifi = 0;
	memset(ssid, 0, sizeof(ssid));
	memset(bssid, 0, sizeof(bssid));
	//Enter to config wifi info
	if (-1 == tos_tf_module_get_info(ssid, bssid, &channel, &rssi)
			|| 3 == Key_Scan())
	{
		LCD_Fill(0, 0, 240, 240, WHITE);
		//config wifi led open
		GPIO_WriteBit(GPIOE, GPIO_Pin_5, 0);
		LCD_ShowString(30, 10, "uHand Add", BLACK, WHITE, 24, 0);
		LCD_ShowPicture((240 - 150) / 2, (240 - 150) / 2, 150, 150,
				gImage_wifi_config);
		if (0 == tos_tf_module_smartconfig_start())
		{
			ConfigWifi = 1;
			LCD_Fill(0, 0, 240, 240, BLACK);
			tos_tf_module_smartconfig_stop();
		}
		else
		{
			LCD_ShowString(10, 10, "WifiConfig Error!", WHITE, BLACK, 32, 0);
			NVIC_SystemReset();
		}
	}
	if (0 == ConfigWifi)
		LCD_Fill(0, 0, 240, 120, BLACK);
	//config wifi led close
	GPIO_WriteBit(GPIOE, GPIO_Pin_5, 1);
	snprintf(esp8266_info.ssid, 32, "ssid:%s", ssid);
	snprintf(esp8266_info.bssid, 32, "bssid:%s", bssid);
	snprintf(esp8266_info.channel, 10, "channel:%d", channel);
	snprintf(esp8266_info.rssi, 10, "rssi:%d", rssi);
	LCD_ShowString(10, 10, "Wifi Connect OK", WHITE, BLACK, 16, 0);
	LCD_ShowString(10, 26, esp8266_info.ssid, WHITE, BLACK, 16, 0);
	LCD_ShowString(10, 42, esp8266_info.bssid, WHITE, BLACK, 16, 0);
	LCD_ShowString(10, 58, esp8266_info.channel, WHITE, BLACK, 16, 0);
	LCD_ShowString(10, 74, esp8266_info.rssi, WHITE, BLACK, 16, 0);
	LCD_ShowPicture(0, 190, 240, 50, gImage_icon_for_tencentos_tiny);
}
