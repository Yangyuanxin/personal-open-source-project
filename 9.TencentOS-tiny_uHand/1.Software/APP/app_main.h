#ifndef __APP_MAIN_H
#define __APP_MAIN_H
#include "SystemConfig.h"

typedef struct ap_info
{
	char ssid[32] ;
	char bssid[32] ;
	char channel[10] ;
	char rssi[10] ;
}ap_info;

//屏幕显示宽度、高度定义
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   240

//物联网平台三元组
#define PRODUCT_ID              "6AOIHRSLBY"
#define DEVICE_NAME             "uHand"
#define DEVICE_KEY              "VOBSz6zoH6I5cs/NdRCtaA=="

/*回复消息上报*/
#define CONTROL_REPLY_DATA_TEMPLATE       \
"{\\\"method\\\":\\\"control_reply\\\"\\,\\\"clientToken\\\":\\\"%s\\\"\\,\\\"code\\\":0\\,\\\"status\\\":\\\"ok\\\"}"

#define DEBUG_GET_TEMP_LED(STATUS)	\
		do{	\
			GPIO_WriteBit(GPIOE, GPIO_Pin_2, !STATUS); \
		}while(0);

#define DEBUG_LED(STATUS) \
		do{ \
			{GPIO_WriteBit(GPIOE, GPIO_Pin_3, !STATUS); \
			GPIO_WriteBit(GPIOE, GPIO_Pin_4, !STATUS); \
			GPIO_WriteBit(GPIOE, GPIO_Pin_5, !STATUS);}\
		}while(0);

void MqttTask(void);

#endif //__APP_MAIN_H

