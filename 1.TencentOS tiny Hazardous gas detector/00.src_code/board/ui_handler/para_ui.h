#ifndef __PARA_UI_H
#define __PARA_UI_H
#include "main.h"

typedef struct
{
    uint8_t alarm ;				/*����*/
    uint8_t value  ;			/*��ֵ��ʾ����*/
    uint8_t network ;			/*���翪��*/
    uint8_t sensivity;		/*������*/
    int     sensivity_value[3];	/*����ģʽ��������ֵ==>��Ӧ�͡��С���*/

    uint8_t select_item;		/*��ǰ������һ��*/
    char 	display_buf[10];
} Para_Setting_Page ;
extern Para_Setting_Page para_setting_page_ui ;


/*��������ҳ���ʼ��*/
void para_setting_page_init(void);
/*�������ð�������*/
void para_setting_page_process(uint8_t KeyValue);



#endif //__PARA_UI_H

