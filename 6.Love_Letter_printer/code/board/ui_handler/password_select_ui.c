#include "password_select_ui.h"

Password_Select_Page password_select_page_ui ;

/*显示配置页面文字--状态相关的接口*/
static void Display_recovery_page_item_font(int item, uint16_t color)
{
    Item Password_Select_Item[] =
    {
        {PASSWORD_SELECT_ITEM_X, PASSWORD_SELECT_ITEM_Y, 	"密码设置", BLACK, BLACK, PASSWORD_SELECT_ITEM_FONT, 1},
        {PASSWORD_SELECT_ITEM_X, PASSWORD_SELECT_ITEM_Y,  "密码设置", WHITE, BLACK, PASSWORD_SELECT_ITEM_FONT, 1},
    };
    display_menu_color_item(Password_Select_Item, item, color);
}

static void Display_Password_Select_page_status1_font(int item, uint16_t color)
{
    /*恢复出厂设置页面状态1显示*/
    Item Password_Select_Status1_Item[] =
    {
        {PASSWORD_SELECT_STATUS1_X, PASSWORD_SELECT_STATUS1_Y,  "关闭密码", BLACK, BLACK, PASSWORD_SELECT_STATUS1_FONT, 1},
        {PASSWORD_SELECT_STATUS1_X, PASSWORD_SELECT_STATUS1_Y,  "关闭密码", WHITE, BLACK, PASSWORD_SELECT_STATUS1_FONT, 1},
    };
    display_menu_color_item(Password_Select_Status1_Item, item, color);
}

static void Display_Password_Select_page_status2_font(int item, uint16_t color)
{
    /*恢复出厂设置页面状态2显示*/
    Item Password_Select_Status2_Item[] =
    {
        {PASSWORD_SELECT_STATUS2_X, PASSWORD_SELECT_STATUS2_Y,  "修改密码", BLACK, BLACK, PASSWORD_SELECT_STATUS2_FONT, 1},
        {PASSWORD_SELECT_STATUS2_X, PASSWORD_SELECT_STATUS2_Y,  "修改密码", WHITE, BLACK, PASSWORD_SELECT_STATUS2_FONT, 1},
    };
    display_menu_color_item(Password_Select_Status2_Item, item, color);
}

/*密码选择页面初始化*/
void password_select_page_init(void)
{
    password_select_page_ui.select_item = 0 ;
    Flow_Cursor.flow_cursor = PASSWORD_SELECT_PAGE ;
    /*显示标题*/
    Display_recovery_page_item_font(1, WHITE);
    /*显示状态1*/
    Display_Password_Select_page_status1_font(1, RED);
    /*显示状态2*/
    Display_Password_Select_page_status2_font(1, WHITE);
}

/*左右键选择ui切换*/
void password_select_left_right_select(uint8_t KeyValue)
{
    /*显示状态1*/
    Display_Password_Select_page_status1_font(1, WHITE);
    /*显示状态2*/
    Display_Password_Select_page_status2_font(1, WHITE);

    switch(KeyValue)
    {
        case LEFT:
            (password_select_page_ui.select_item > 0) ? \
            (password_select_page_ui.select_item--) :	\
            (password_select_page_ui.select_item = 0) ;
            Display_Password_Select_page_status1_font(1, RED);
            break ;

        case RIGHT:
            (password_select_page_ui.select_item < 1) ? \
            (password_select_page_ui.select_item++) :	\
            (password_select_page_ui.select_item = 1) ;
            Display_Password_Select_page_status2_font(1, RED);
            break ;

        default:
            break ;
    }
}

/*确认键*/
void password_select_enter_key_handler(uint8_t item)
{
    switch(item)
    {
        /*返回到配置页面*/
        case 0:
            /*隐藏标题/状态1/状态2*/
            Display_recovery_page_item_font(0, BLACK);
            Display_Password_Select_page_status1_font(0, BLACK);
            Display_Password_Select_page_status2_font(0, BLACK);
            conf_page_ui.select_item = 2 ;
            conf_page_ui_init(conf_page_ui.select_item);
            setting_password_enable(0);
            break ;

        /*跳转到修改密码*/
        case 1:
						/*隐藏标题/状态1/状态2*/
            Display_recovery_page_item_font(0, BLACK);
            Display_Password_Select_page_status1_font(0, BLACK);
            Display_Password_Select_page_status2_font(0, BLACK);
            /*密码页面初始化*/
            password_page_init();
            break ;

        default:
            break ;
    }
}

/*密码选择页面按键处理*/
void password_select_page_process(uint8_t KeyValue)
{
    /*左右按键选择是/否,如果短按为否则退出回到上一级*/
    /*长按右键确认选中项*/
    switch(KeyValue)
    {
        case LEFT:
            password_select_left_right_select(KeyValue);
            break ;

        case RIGHT:
            password_select_left_right_select(KeyValue);
            break ;

        case RIGHT_LONG:
            password_select_enter_key_handler(password_select_page_ui.select_item);
            break ;

        default:
            break ;
    }
}

