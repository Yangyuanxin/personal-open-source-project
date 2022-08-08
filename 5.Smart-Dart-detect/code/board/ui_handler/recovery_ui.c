#include "recovery_ui.h"

Recovery_Page recovery_page_ui ;


/*显示配置页面文字--状态相关的接口*/
static void Display_recovery_page_item_font(int item, uint16_t color)
{
    /*恢复出厂设置页面菜单项显示*/
    Item Recovery_Item[] =
    {
        {RECOVERY_ITEM_X, RECOVERY_ITEM_Y, 	"恢复出厂设置", BLACK, BLACK, RECOVERY_ITEM_FONT, 1},
        {RECOVERY_ITEM_X, RECOVERY_ITEM_Y,  "恢复出厂设置", WHITE, BLACK, RECOVERY_ITEM_FONT, 1},
    };
    display_menu_color_item(Recovery_Item, item, color);
}

static void Display_recovery_page_status1_font(int item, uint16_t color)
{
    /*恢复出厂设置页面状态1显示*/
    Item Recovery_Status1_Item[] =
    {
        {RECOVERY_STATUS1_X, RECOVERY_STATUS1_Y,  "否", BLACK, BLACK, RECOVERY_STATUS1_FONT, 1},
        {RECOVERY_STATUS1_X, RECOVERY_STATUS1_Y,  "否", WHITE, BLACK, RECOVERY_STATUS1_FONT, 1},
    };
    display_menu_color_item(Recovery_Status1_Item, item, color);
}

static void Display_recovery_page_status2_font(int item, uint16_t color)
{
    /*恢复出厂设置页面状态2显示*/
    Item Recovery_Status2_Item[] =
    {
        {RECOVERY_STATUS2_X, RECOVERY_STATUS2_Y,  "是", BLACK, BLACK, RECOVERY_STATUS2_FONT, 1},
        {RECOVERY_STATUS2_X, RECOVERY_STATUS2_Y,  "是", WHITE, BLACK, RECOVERY_STATUS2_FONT, 1},
    };
    display_menu_color_item(Recovery_Status2_Item, item, color);
}

/*恢复出厂设置初始化*/
void recovery_page_init(void)
{
    recovery_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = RECOVERY_PAGE ;
    /*显示标题*/
    Display_recovery_page_item_font(1, WHITE);
    /*显示状态1*/
    Display_recovery_page_status1_font(1, RED);
    /*显示状态2*/
    Display_recovery_page_status2_font(1, WHITE);
}


/*左右键选择ui切换*/
void left_right_select(uint8_t KeyValue)
{
    /*显示状态1*/
    Display_recovery_page_status1_font(1, WHITE);
    /*显示状态2*/
    Display_recovery_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
    case LEFT:
        (recovery_page_ui.select_item > 0) ? \
        (recovery_page_ui.select_item--) :	\
        (recovery_page_ui.select_item = 0) ;
        Display_recovery_page_status1_font(1, RED);
        break ;

    case RIGHT:
        (recovery_page_ui.select_item < 1) ? \
        (recovery_page_ui.select_item++) :	\
        (recovery_page_ui.select_item = 1) ;
        Display_recovery_page_status2_font(1, RED);
        break ;

    default:
        break ;
    }
}

/*确认键*/
void recovery_enter_key_handler(uint8_t item)
{
    switch(item)
    {
    /*返回上一级页面*/
    case 0:
        /*隐藏标题/状态1/状态2*/
        Display_recovery_page_item_font(0, BLACK);
        Display_recovery_page_status1_font(0, BLACK);
        Display_recovery_page_status2_font(0, BLACK);
        /*返回系统信息界面*/
        Version_Info_Page_Init();
        break ;

    /*即将恢复出厂设置*/
    case 1:
        tos_knl_sched_lock();
        Recovery_Setting();
        DEBUG("恢复出厂设置成功\n");
        tos_knl_sched_unlock();
        HAL_NVIC_SystemReset();
        break ;

    default:
        break ;
    }
}

/*恢复出厂设置页面按键处理*/
void recovery_page_process(uint8_t KeyValue)
{
    /*左右按键选择是/否,如果短按为否则退出回到上一级*/
    /*长按左键退回到上一级，长按右键确认选中项*/
    switch(KeyValue)
    {
    case LEFT:
        left_right_select(KeyValue);
        break ;

    case RIGHT:
        left_right_select(KeyValue);
        break ;

    case RIGHT_LONG:
        recovery_enter_key_handler(recovery_page_ui.select_item);
        break ;

    case LEFT_LONG:
        /*隐藏标题/状态1/状态2*/
        Display_recovery_page_item_font(0, BLACK);
        Display_recovery_page_status1_font(0, BLACK);
        Display_recovery_page_status2_font(0, BLACK);
        /*返回系统信息界面*/
        Version_Info_Page_Init();
        break ;

    default:
        break ;
    }
}


