#ifndef __TEST_UI_H
#define  __TEST_UI_H
#include "main.h"



//检测状态机
#define BASE_LINE 0
#define DETECTING 1
#define DETECT_SAFETY 2
#define DETECT_DANGER 3
#define NULL_STATUS   99


/*基准图标*/
#define B0_LOGO "0:/UI/detect_page/b0.bmp"
#define B1_LOGO "0:/UI/detect_page/b1.bmp"
#define B2_LOGO "0:/UI/detect_page/b2.bmp"

/*检测图标*/
#define D0_LOGO "0:/UI/detect_page/d0.bmp"
#define D1_LOGO "0:/UI/detect_page/d1.bmp"
#define D2_LOGO "0:/UI/detect_page/d2.bmp"

/*安全图标*/
#define R0_LOGO "0:/UI/detect_page/r0.bmp"
/*危险图标*/
#define R1_LOGO "0:/UI/detect_page/r1.bmp"

/*烟感值显示*/
#define SMOKE_X 74
#define SMOKE_Y 130 + 24


/*测试页面初始化*/
void test_page_init(void);
/*测试页面事件处理*/
void test_page_process(uint8_t Event_Code);
/*显示1隐藏0进度条框*/
void Display_Process_Bar_Frame(int enable);
/*显示1隐藏0进度条*/
void Display_Process_Bar(int Process, int enable);
/*显示烟感值*/
void display_smoke_value(int smoke_value, uint16_t color, uint8_t enable);

/*显示基准1隐藏0*/
void display_base(uint8_t enable);
/*显示检测1隐藏0*/
void display_detect(uint8_t enable);
/*显示安全1隐藏0*/
void display_safety(uint8_t enable);
/*显示危险1隐藏0*/
void display_danger(uint8_t enable);
//刷新动画
void icon_reflash(uint8_t status);

#endif //__TEST_UI_H

