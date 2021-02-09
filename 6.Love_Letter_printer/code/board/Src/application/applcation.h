#ifndef __APPLCATION_H
#define __APPLCATION_H
#include "main.h"

// 物联网开发平台设备信息
#define PRODUCT_ID              "WONYDFWVJO"
#define DEVICE_NAME             "Print_Device"
#define DEVICE_KEY              "sjNLp7TwCcK96zeTd5qRGg=="

// WIFI信息
#define YOUR_WIFI_SSID          "602"
#define YOUR_WIFI_PWD           "13602584536"

// 数据模板
#define REPORT_LX_DATA_TEMPLATE               "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"lx\\\":%.0f}}"
#define REPORT_POWER_SWITCH_DATA_TEMPLATE     "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"power_switch\\\":%d}}"
#define REPORT_BRIGHTNESS_LEVEL_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"week\\\":%d}}"
#define CONTROL_REPLY_DATA_TEMPLATE       	  "{\\\"method\\\":\\\"control_reply\\\"\\,\\\"clientToken\\\":\\\"%s\\\"\\,\\\"code\\\":0\\,\\\"status\\\":\\\"ok\\\"}"













#endif //__APPLCATION_H

