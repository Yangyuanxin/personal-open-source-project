#include "sleep_ui.h"



/*����ҳ���ʼ��*/
void sleep_page_init(void)
{
    sleep_mode();
}

/*����ҳ���¼�����*/
void sleep_page_process(uint8_t Event_Code)
{
    if(Event_Code == LEFT_LONG)
        wake_up_mode();
}
