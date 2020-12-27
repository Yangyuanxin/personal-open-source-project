#ifndef __RECOVERY_UI_H
#define __RECOVERY_UI_H
#include "main.h"

#define RECOVERY_ITEM_X		  	46
#define RECOVERY_ITEM_Y     	71
#define RECOVERY_ITEM_FONT		24

#define RECOVERY_STATUS1_X		105
#define RECOVERY_STATUS1_Y     	112
#define RECOVERY_STATUS1_FONT	24

#define RECOVERY_STATUS2_X		105
#define RECOVERY_STATUS2_Y     	145
#define RECOVERY_STATUS2_FONT	24

typedef struct
{
    uint8_t select_item;
} Recovery_Page ;
extern Recovery_Page recovery_page_ui ;

/*�ָ��������ó�ʼ��*/
void recovery_page_init(void);
/*�ָ���������ҳ�水������*/
void recovery_page_process(uint8_t KeyValue);



#endif //__RECOVERY_UI_H

