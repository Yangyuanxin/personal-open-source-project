#ifndef __DATETIME_UI_H
#define __DATETIME_UI_H
#include "main.h"

//�·ݱ�Ŷ���
#define Jan 1
#define Feb 2
#define Mar 3
#define Apr 4
#define May 5
#define Jun 6
#define Jul 7
#define Aug 8
#define Sep 9
#define Oct 10
#define Nov 11
#define Dec 12

//ʱ��������ֵ����
#define MAX_YEAR 2099
#define MIN_YEAR 2000
#define MAX_MONTH 12
#define MIN_MONTH  1
#define MAX_MON_DAY  31
#define MIN_MON_DAY  30
#define FEB_MON_MAX_DAY  29
#define FEB_MON_MIN_DAY  28
#define MIN_DAY  1
#define MAX_HOUR  23
#define MIN_HOUR   1
#define MAX_MINUTE 59
#define MIN_MINUTE 0

/*************UI��ʾ���***************/
//ʱ�����ñ���
#define CLOCK_TEXT_START_X 70
#define CLOCK_TEXT_START_Y 50
#define CLOCK_TEXT_FONT    24

//��
#define YEAR_TEXT_START_X 53
#define YEAR_TEXT_START_Y 100
#define YEAR_TEXT_FONT    24

//��
#define MONTH_TEXT_START_X 110
#define MONTH_TEXT_START_Y 100
#define MONTH_TEXT_FONT    24

//��
#define DAY_TEXT_START_X 172
#define DAY_TEXT_START_Y 100
#define DAY_TEXT_FONT    24

//ʱ
#define HOUR_TEXT_START_X 53
#define HOUR_TEXT_START_Y 143
#define HOUR_TEXT_FONT    24

//��
#define MIN_TEXT_START_X 110
#define MIN_TEXT_START_Y 143
#define MIN_TEXT_FONT    24

//��
#define SEC_TEXT_START_X 172
#define SEC_TEXT_START_Y 143
#define SEC_TEXT_FONT    24
/*************UI��ʾ���***************/
typedef struct
{
    uint16_t year   ;		//��
    uint8_t month   ; 	//��
    uint8_t day     ;		//��
    uint8_t hour    ;		//ʱ
    uint8_t minute  ;		//��
    uint8_t sec 		;		//��

    uint8_t select_item ;
    char 	  display_buf[10];
} Clock_Setting;

//���������㷨
#define IS_LEAP_YEAR(y) (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0)

/*����ʱ�����ó�ʼ��*/
void datetime_page_init(void);
/*����ʱ������ҳ�水������*/
void datetime_page_process(uint8_t KeyValue);

#endif //__DATETIME_UI_H

