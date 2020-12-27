#ifndef __TEST_UI_H
#define  __TEST_UI_H
#include "main.h"



//���״̬��
#define BASE_LINE 0
#define DETECTING 1
#define DETECT_SAFETY 2
#define DETECT_DANGER 3
#define NULL_STATUS   99


/*��׼ͼ��*/
#define B0_LOGO "0:/UI/detect_page/b0.bmp"
#define B1_LOGO "0:/UI/detect_page/b1.bmp"
#define B2_LOGO "0:/UI/detect_page/b2.bmp"

/*���ͼ��*/
#define D0_LOGO "0:/UI/detect_page/d0.bmp"
#define D1_LOGO "0:/UI/detect_page/d1.bmp"
#define D2_LOGO "0:/UI/detect_page/d2.bmp"

/*��ȫͼ��*/
#define R0_LOGO "0:/UI/detect_page/r0.bmp"
/*Σ��ͼ��*/
#define R1_LOGO "0:/UI/detect_page/r1.bmp"

/*�̸�ֵ��ʾ*/
#define SMOKE_X 74
#define SMOKE_Y 130 + 24


/*����ҳ���ʼ��*/
void test_page_init(void);
/*����ҳ���¼�����*/
void test_page_process(uint8_t Event_Code);
/*��ʾ1����0��������*/
void Display_Process_Bar_Frame(int enable);
/*��ʾ1����0������*/
void Display_Process_Bar(int Process, int enable);
/*��ʾ�̸�ֵ*/
void display_smoke_value(int smoke_value, uint16_t color, uint8_t enable);

/*��ʾ��׼1����0*/
void display_base(uint8_t enable);
/*��ʾ���1����0*/
void display_detect(uint8_t enable);
/*��ʾ��ȫ1����0*/
void display_safety(uint8_t enable);
/*��ʾΣ��1����0*/
void display_danger(uint8_t enable);
//ˢ�¶���
void icon_reflash(uint8_t status);

#endif //__TEST_UI_H

