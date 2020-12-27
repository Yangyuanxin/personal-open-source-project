#ifndef __PASSWORD_UI_H
#define __PASSWORD_UI_H
#include "main.h"

//请输入密码标题
#define INPUT_PASSWD_TEXT_START_X 60
#define INPUT_PASSWD_TEXT_START_Y 50
#define INPUT_PASSWD_TEXT_FONT    24

//密码第一位
#define NUM_ONE_TEXT_X					46+14
#define NUM_ONE_TEXT_Y          117
#define NUM_ONE_TEXT_FONT				24

//密码第二位
#define NUM_TWO_TEXT_X				  82+14
#define NUM_TWO_TEXT_Y					117
#define NUM_TWO_TEXT_FONT       24

//密码第三位
#define NUM_THREE_TEXT_X				116+14
#define NUM_THREE_TEXT_Y				117
#define NUM_THREE_TEXT_FONT     24

//密码第四位
#define NUM_FOUR_TEXT_X					150+14
#define NUM_FOUR_TEXT_Y					117
#define NUM_FOUR_TEXT_FONT     	24

//密码错误
#define PASSWD_ERROR_TEXT_START_X 71
#define PASSWD_ERROR_TEXT_START_Y 157
#define PASSWD_ERROR_TEXT_FONT    24

//请重新输入
#define RETRY_INPUT_TEXT_START_X 62
#define RETRY_INPUT_TEXT_START_Y 182
#define RETRY_INPUT_TEXT_FONT    24

typedef struct
{
    uint8_t select_item ;
    uint8_t Password[4];
    char 	display_buf[10];
} Password_Page ;

/*简单加密*/
void sample_encrypt_passwd(int8_t passwd[4]);
/*简单解密*/
void sample_decode_passwd(int8_t passwd[4]);

/*密码设置初始化*/
void password_page_init(void);

/*密码设置页面按键处理*/
void password_page_process(uint8_t KeyValue);

/*密码输入页面初始化*/
void password_input_page_init(void);
/*密码输入页面按键处理*/
void password_input_page_process(uint8_t KeyValue);

#endif //__PASSWORD_UI_H


