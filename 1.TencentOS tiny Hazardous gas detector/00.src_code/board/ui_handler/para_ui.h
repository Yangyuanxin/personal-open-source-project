#ifndef __PARA_UI_H
#define __PARA_UI_H
#include "main.h"

typedef struct
{
    uint8_t alarm ;				/*报警*/
    uint8_t value  ;			/*数值显示开关*/
    uint8_t network ;			/*网络开关*/
    uint8_t sensivity;		/*灵敏度*/
    int     sensivity_value[3];	/*吸气模式灵敏度数值==>对应低、中、高*/

    uint8_t select_item;		/*当前设置哪一项*/
    char 	display_buf[10];
} Para_Setting_Page ;
extern Para_Setting_Page para_setting_page_ui ;


/*参数设置页面初始化*/
void para_setting_page_init(void);
/*参数设置按键操作*/
void para_setting_page_process(uint8_t KeyValue);



#endif //__PARA_UI_H

