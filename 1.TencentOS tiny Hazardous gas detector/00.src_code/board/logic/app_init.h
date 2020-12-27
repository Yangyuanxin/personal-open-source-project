#ifndef __APP_INIT_H
#define __APP_INIT_H
#include "main.h"
#include "lcd_driver_frame.h"

#define NR_ARRAY(x) sizeof(x)/sizeof(x[0])

//#define WIFI_SSID 				"SRED"
//#define WIFI_PASSWORD			"12345678"
//#define SERVER_ADDR				"183.230.40.33"
//#define SERVER_PORT				80
#define ONENET_DEVICE_ID  		"604618155"
#define ONENET_API_KEY	  		"PShVObl=1Sx87Or4lI3VOPGEpaY="

#define http_uploader_head	\
    "POST /devices/%s/datapoints?type=3 HTTP/1.1\r\n"	\
    "api-key:%s\r\n"					\
    "Host:api.heclouds.com\r\n"									\
    "Content-Length:%d\r\n\r\n"

#define http_uploader_data \
    "{\"ppm\":%d}"

extern LCD_Driver_Model lcd_model ;

/*显示无线状态*/
void display_wlan_status(uint8_t status);
/*启动操作系统*/
void start_tencent_os(void);
/*显示TencentOS tiny logo*/
void display_tencent_logo(uint8_t status);






#endif //__APP_INIT_H


