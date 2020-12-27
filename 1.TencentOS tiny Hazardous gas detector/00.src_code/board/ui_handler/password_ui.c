#include "password_ui.h"

Password_Page password_page_ui ;


/*显示1/隐藏0请输入密码标题*/
void password_input_item_display(int enable)
{
    Item Password_Item[] = {INPUT_PASSWD_TEXT_START_X, INPUT_PASSWD_TEXT_START_Y, "请输入密码", WHITE, BLACK, INPUT_PASSWD_TEXT_FONT, 1};

    if(enable == 1)
        display_menu_color_item(Password_Item, 0, WHITE);
    else if(enable == 0)
        display_menu_color_item(Password_Item, 0, BLACK);
}

/*显示1/隐藏0请输入密码错误提示*/
void password_input_error_display(int enable)
{
    Item Password_Error_Item[] =
    {
        {PASSWD_ERROR_TEXT_START_X, PASSWD_ERROR_TEXT_START_Y, "密码错误", BLACK, BLACK, PASSWD_ERROR_TEXT_FONT, 1},
        {RETRY_INPUT_TEXT_START_X, RETRY_INPUT_TEXT_START_Y, "请重新输入", BLACK, BLACK, RETRY_INPUT_TEXT_FONT, 1},
    };

    if(enable == 1)
    {
        display_menu_color_item(Password_Error_Item, 0, RED);
        display_menu_color_item(Password_Error_Item, 1, RED);
    }
    else if(enable == 0)
    {
        display_menu_color_item(Password_Error_Item, 0, BLACK);
        display_menu_color_item(Password_Error_Item, 1, BLACK);
    }
}

/*显示密码数值*/
void password_number_display(int enable)
{
    LCD_Ascii_Show_Para password_ascii[] =
    {
        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, WHITE, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, WHITE, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, WHITE, NUM_FOUR_TEXT_FONT},

        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, BLACK, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, BLACK, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, BLACK, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, BLACK, NUM_FOUR_TEXT_FONT},
    };

    if(enable == 1)
    {
        memset(password_page_ui.Password, 0, 3);
        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[0]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[0]);

        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[1]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[1]);

        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[2]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[2]);

        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[3]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[3]);
    }
    else if(enable == 0)
    {
        for(int i = 4 ; i < 8 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[i]);
    }
}


/*密码参数初始化*/
void password_page_para_init(void)
{
    password_page_ui.select_item = 0 ;
    memset(password_page_ui.display_buf, 0, 10);
    memset(password_page_ui.Password, 0, 4);
}

/*密码设置初始化*/
void password_page_init(void)
{
    Flow_Cursor.flow_cursor = PASSWD_PAGE ;
    password_page_para_init();
    password_input_item_display(1);
    password_number_display(1);
}

/*选中具体密码位数调整项*/
void select_password_item(int item)
{
    LCD_Ascii_Show_Para password_ascii[] =
    {
        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, WHITE, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, WHITE, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, WHITE, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, WHITE, NUM_FOUR_TEXT_FONT},

        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_FOUR_TEXT_FONT},
    };


    memset(password_page_ui.display_buf, 0, 10);
    sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[0]);
    lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[0]);

    memset(password_page_ui.display_buf, 0, 10);
    sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[1]);
    lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[1]);

    memset(password_page_ui.display_buf, 0, 10);
    sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[2]);
    lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[2]);

    memset(password_page_ui.display_buf, 0, 10);
    sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[3]);
    lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[3]);

    switch(password_page_ui.select_item)
    {
    case 0:
        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[0]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[4]);
        break ;

    case 1:
        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[1]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[5]);
        break ;

    case 2:
        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[2]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[6]);
        break ;

    case 3:
        memset(password_page_ui.display_buf, 0, 10);
        sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[3]);
        lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[7]);
        break ;

    default:
        break ;
    }
}

/*跳转到下一密码设置项*/
void jump_next_device_cail_password_item(void)
{
    switch(password_page_ui.select_item)
    {
    case 0:
        password_page_ui.select_item = 1 ;
        select_password_item(password_page_ui.select_item);
        break ;

    case 1:
        password_page_ui.select_item = 2 ;
        select_password_item(password_page_ui.select_item);
        break ;

    case 2:
        password_page_ui.select_item = 3 ;
        select_password_item(password_page_ui.select_item);
        break ;

    case 3:
        password_page_ui.select_item = 0 ;
        select_password_item(password_page_ui.select_item);
        break ;

    default:
        break ;
    }
}


/*密码设置页面按键处理*/
void password_page_process(uint8_t KeyValue)
{
    LCD_Ascii_Show_Para password_ascii[] =
    {
        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_FOUR_TEXT_FONT},
    };

    switch(KeyValue)
    {
    case LEFT:
        jump_next_device_cail_password_item();
        break ;

    case RIGHT:
        switch(password_page_ui.select_item)
        {
        case 0:
            (password_page_ui.Password[0] < 9) ? \
            (password_page_ui.Password[0]++) :		\
            (password_page_ui.Password[0] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[0]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[0]);
            break ;

        case 1:
            (password_page_ui.Password[1] < 9) ? \
            (password_page_ui.Password[1]++) :		\
            (password_page_ui.Password[1] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[1]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[1]);
            break ;

        case 2:
            (password_page_ui.Password[2] < 9) ? \
            (password_page_ui.Password[2]++) :		\
            (password_page_ui.Password[2] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[2]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[2]);
            break ;

        case 3:
            (password_page_ui.Password[3] < 9) ? \
            (password_page_ui.Password[3]++) :		\
            (password_page_ui.Password[3] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[3]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[3]);
            break ;

        default:
            break;
        }

        break ;

    /*返回主菜单的配置项*/
    case LEFT_LONG:
    case RIGHT_LONG:
        /*将设置密码设置到用户参数区*/
        User_Memory_Para.password[0] = password_page_ui.Password[0];
        User_Memory_Para.password[1] = password_page_ui.Password[1];
        User_Memory_Para.password[2] = password_page_ui.Password[2];
        User_Memory_Para.password[3] = password_page_ui.Password[3];
        /*对当前输入的密码进行加密操作*/
        sample_encrypt_passwd(User_Memory_Para.password);
        /*保存设备密码*/
        setting_device_password(User_Memory_Para);
        password_input_item_display(0);
        password_number_display(0);
        DEBUG("最终设置的密码:%d%d%d%d\n", User_Memory_Para.password[0], \
              User_Memory_Para.password[1], User_Memory_Para.password[2], User_Memory_Para.password[3]);
        /*进入配置页面*/
        conf_page_ui.select_item = 2 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

/*简单加密*/
void sample_encrypt_passwd(int8_t passwd[4])
{
    passwd[0] = passwd[0] + 1 ;
    passwd[1] = passwd[1] + 1 ;
    passwd[2] = passwd[2] + 1 ;
    passwd[3] = passwd[3] + 1 ;
}

/*简单解密*/
void sample_decode_passwd(int8_t passwd[4])
{
    passwd[0] = passwd[0] - 1 ;
    passwd[1] = passwd[1] - 1 ;
    passwd[2] = passwd[2] - 1 ;
    passwd[3] = passwd[3] - 1 ;
}


/*密码输入页面初始化*/
void password_input_page_init(void)
{
    Flow_Cursor.flow_cursor = PASSWD_INPUT_PAGE ;
    password_page_para_init();
    password_input_item_display(1);
    password_number_display(1);
}

/*密码输入页面按键处理*/
void password_input_page_process(uint8_t KeyValue)
{
    uint8_t error_flag = 0 ;
    LCD_Ascii_Show_Para password_ascii[] =
    {
        {NUM_ONE_TEXT_X, NUM_ONE_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_ONE_TEXT_FONT},
        {NUM_TWO_TEXT_X, NUM_TWO_TEXT_Y, 125, password_page_ui.display_buf, BLACK, RED, NUM_TWO_TEXT_FONT},
        {NUM_THREE_TEXT_X, NUM_THREE_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_THREE_TEXT_FONT},
        {NUM_FOUR_TEXT_X, NUM_FOUR_TEXT_Y, 20, password_page_ui.display_buf, BLACK, RED, NUM_FOUR_TEXT_FONT},
    };

    switch(KeyValue)
    {
    case LEFT:
        jump_next_device_cail_password_item();
        break ;

    case RIGHT:
        switch(password_page_ui.select_item)
        {
        case 0:
            (password_page_ui.Password[0] < 9) ? \
            (password_page_ui.Password[0]++) :		\
            (password_page_ui.Password[0] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[0]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[0]);
            break ;

        case 1:
            (password_page_ui.Password[1] < 9) ? \
            (password_page_ui.Password[1]++) :		\
            (password_page_ui.Password[1] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[1]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[1]);
            break ;

        case 2:
            (password_page_ui.Password[2] < 9) ? \
            (password_page_ui.Password[2]++) :		\
            (password_page_ui.Password[2] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[2]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[2]);
            break ;

        case 3:
            (password_page_ui.Password[3] < 9) ? \
            (password_page_ui.Password[3]++) :		\
            (password_page_ui.Password[3] = 0);
            memset(password_page_ui.display_buf, 0, 10);
            sprintf(password_page_ui.display_buf, "%d", password_page_ui.Password[3]);
            lcd_model.lcd_driver->lcd_show_ascii_str(password_ascii[3]);
            break ;

        default:
            break;
        }

        break ;

    /*长按左键退出回到主页面*/
    case LEFT_LONG:
        password_input_item_display(0);
        password_number_display(0);
        display_tencent_logo(1);
        main_page_init();
        break ;

    /*右键确认进入下一个页面*/
    case RIGHT_LONG:
        for(int i = 0 ; i < 4 ; i++)
        {
            if(password_page_ui.Password[i] != User_Memory_Para.password[i])
            {
                error_flag = 1 ;
                break ;
            }
        }

        /*密码错误*/
        if(error_flag == 1)
        {
            //锁定系统调度
            tos_knl_sched_lock();
            /*提示错误并显示*/
            password_input_error_display(1);
            HAL_Delay(1000);
            password_input_error_display(0);
            //解锁系统调度
            tos_knl_sched_unlock();
            //重新初始化密码输入参数
            password_input_page_init();
            password_number_display(1);
        }
        /*密码正确，跳转到配置页面*/
        else
        {
            password_input_item_display(0);
            password_number_display(0);
            /*进入配置页面*/
            conf_page_ui.select_item = 0 ;
            Flow_Cursor.flow_cursor = CONF_PAGE ;
            conf_page_ui_init(conf_page_ui.select_item);
        }

        break ;

    default:
        break ;
    }
}
