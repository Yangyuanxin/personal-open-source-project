#include "conf_ui.h"

/**********加菜单的时候需要更改以下参数**********/
/*显示*/
#define DISPLAY_START 0
#define DISPLAY_END 	5

//显示间隔
#define DISPLAY_GAP   12
/**********加菜单的时候需要更改以下参数**********/

Conf_Page conf_page_ui ;

/*配置页面菜单项显示*/
Item Conf_Item_Display[] =
{
    {70, 73 - DISPLAY_GAP,  "系统参数", WHITE, BLACK, 24, 1},		//0		/*检测阈值===>低、中、高*/
    {70, 102 - DISPLAY_GAP, "时间设置", WHITE, BLACK, 24, 1},		//1		/*声音、LED、数值显示*/
    {70, 131 - DISPLAY_GAP, "密码设置", WHITE, BLACK, 24, 1},		//2		/*开、关*/
    {70, 160 - DISPLAY_GAP, "仪器信息", WHITE, BLACK, 24, 1},		//3		/*显示版本号、ROM、RAM等信息*/
    {70, 191 - DISPLAY_GAP, "曲线模式", WHITE, BLACK, 24, 1},		//4		/*曲线模式===>显示传感器实时曲线*/
};

/*选择菜单项目*/

void select_conf_item(uint8_t item, uint16_t color)
{
    LCD_Ascii_Show_Para disselect_ascii[] =
    {
        {46, 73 - DISPLAY_GAP, 24,  "*", BLACK, BLACK, 24},
        {46, 102 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 131 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 160 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},
        {46, 191 - DISPLAY_GAP, 24, "*", BLACK, BLACK, 24},

        {46, 73 - DISPLAY_GAP, 24,  "*", BLACK, color, 24},
        {46, 102 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 131 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 160 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
        {46, 191 - DISPLAY_GAP, 24, "*", BLACK, color, 24},
    };

    for(int i = 0 ; i < 5 ; i++)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[i]);

    if(item == 0)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[5]);
    else if(item == 1)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[6]);
    else if(item == 2)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[7]);
    else if(item == 3)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[8]);
    else if(item == 4)
        lcd_model.lcd_driver->lcd_show_ascii_str(disselect_ascii[9]);
}

//配置页面初始化
void conf_page_ui_init(uint8_t item)
{
    Flow_Cursor.flow_cursor = CONF_PAGE ;
    conf_page_ui.select_item = item ;

    for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
        display_menu_color_item(Conf_Item_Display, i, WHITE);

    select_conf_item(conf_page_ui.select_item, RED);
}

//进入某一配置项
void enter_conf_item(uint8_t item)
{
    for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
        display_menu_color_item(Conf_Item_Display, i, BLACK);

    switch(item)
    {
    //系统参数
    case 0:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*系统参数页面初始化*/
        para_setting_page_init();
        break ;

    //时间设置
    case 1:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*时间页面初始化*/
        datetime_page_init();
        break ;

    //密码设置
    case 2:
        select_conf_item(conf_page_ui.select_item, BLACK);
        /*密码页面初始化*/
        password_page_init();
        break ;

    //仪器信息
    case 3:
        select_conf_item(conf_page_ui.select_item, BLACK);
        Version_Info_Page_Init();
        break ;

    //曲线模式
    case 4:
        plot_page_init();
        break ;

    default:
        break ;
    }
}


//配置页面按键处理
void conf_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
    //切换到下一项
    case LEFT:
        (conf_page_ui.select_item < DISPLAY_END - 1) ? 	\
        (conf_page_ui.select_item++) : 							\
        (conf_page_ui.select_item = DISPLAY_START);
        select_conf_item(conf_page_ui.select_item, RED);
        break ;

    //进入某一项
    case RIGHT:
        lcd_model.lcd_driver->lcd_display_onoff(0);
        enter_conf_item(conf_page_ui.select_item);
        lcd_model.lcd_driver->lcd_display_onoff(1);
        break ;

    //退出菜单回到主菜单
    case RIGHT_LONG:
        lcd_model.lcd_driver->lcd_display_onoff(0);
        select_conf_item(conf_page_ui.select_item, BLACK);

        for(int i = DISPLAY_START ; i < DISPLAY_END ; i++)
            display_menu_color_item(Conf_Item_Display, i, BLACK);

        Flow_Cursor.flow_cursor = MAIN_PAGE ;
        display_tencent_logo(1);
        Select_Main_Menu_Item(main_page_ui.select_item);
        lcd_model.lcd_driver->lcd_display_onoff(1);
        break ;

    default:
        break ;
    }
}
