#ifndef __SYSTEMCONFIG_H
#define __SYSTEMCONFIG_H
/*ISO*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*ISO End*/

/*Hardware*/
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "uart.h"
#include "delay.h"
#include "lcd_init.h"
/*Hardware End*/

/*Custom*/
#include "cJSON.h"
#include "cmd_forward.h"
#include "ring_buffer.h"
extern ring_buffer Fifo;
/*Custom End*/

/*OperatingSystem*/
#include "tos_k.h"
#include "tos_at.h"
#include "cmsis_os.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"
extern at_agent_t esp8266_tf_agent;
extern k_msg_q_t CmdMsg;
/*OperatingSystem End*/

/*McuPlatform*/
#include "ch32v30x_it.h"
/*McuPlatform End*/

extern const unsigned char gImage_wifi_config[45000];
extern const unsigned char gImage_icon_for_tencentos_tiny[24008];

#endif //__SYSTEMCONFIG_H
