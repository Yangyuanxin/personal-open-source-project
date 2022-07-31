#ifndef __KEYS_H_
#define __KEYS_H_
#include "main.h"

//按键长按计数
#define KEY_PRESS_LONG_TIME 100

//菜单按键键值定义
#define KEY_F1        	1			/*左*/
#define KEY_F2 		    2			/*左长按*/
#define KEY_F3			3			/*右*/
#define KEY_F4			4			/*右长按*/

void PowerOn(void);
void GetKey(__IO uint8_t *pKeyValue);
void Key_F1(__IO uint8_t *pKeyValue);
void Key_F2(__IO uint8_t *pKeyValue);

#endif
