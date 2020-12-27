#include "password_ui.h"

Password_Page password_page_ui ;


/*��ʾ1/����0�������������*/
void password_input_item_display(int enable)
{
    Item Password_Item[] = {INPUT_PASSWD_TEXT_START_X, INPUT_PASSWD_TEXT_START_Y, "����������", WHITE, BLACK, INPUT_PASSWD_TEXT_FONT, 1};

    if(enable == 1)
        display_menu_color_item(Password_Item, 0, WHITE);
    else if(enable == 0)
        display_menu_color_item(Password_Item, 0, BLACK);
}

/*��ʾ1/����0���������������ʾ*/
void password_input_error_display(int enable)
{
    Item Password_Error_Item[] =
    {
        {PASSWD_ERROR_TEXT_START_X, PASSWD_ERROR_TEXT_START_Y, "�������", BLACK, BLACK, PASSWD_ERROR_TEXT_FONT, 1},
        {RETRY_INPUT_TEXT_START_X, RETRY_INPUT_TEXT_START_Y, "����������", BLACK, BLACK, RETRY_INPUT_TEXT_FONT, 1},
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

/*��ʾ������ֵ*/
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


/*���������ʼ��*/
void password_page_para_init(void)
{
    password_page_ui.select_item = 0 ;
    memset(password_page_ui.display_buf, 0, 10);
    memset(password_page_ui.Password, 0, 4);
}

/*�������ó�ʼ��*/
void password_page_init(void)
{
    Flow_Cursor.flow_cursor = PASSWD_PAGE ;
    password_page_para_init();
    password_input_item_display(1);
    password_number_display(1);
}

/*ѡ�о�������λ��������*/
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

/*��ת����һ����������*/
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


/*��������ҳ�水������*/
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

    /*�������˵���������*/
    case LEFT_LONG:
    case RIGHT_LONG:
        /*�������������õ��û�������*/
        User_Memory_Para.password[0] = password_page_ui.Password[0];
        User_Memory_Para.password[1] = password_page_ui.Password[1];
        User_Memory_Para.password[2] = password_page_ui.Password[2];
        User_Memory_Para.password[3] = password_page_ui.Password[3];
        /*�Ե�ǰ�����������м��ܲ���*/
        sample_encrypt_passwd(User_Memory_Para.password);
        /*�����豸����*/
        setting_device_password(User_Memory_Para);
        password_input_item_display(0);
        password_number_display(0);
        DEBUG("�������õ�����:%d%d%d%d\n", User_Memory_Para.password[0], \
              User_Memory_Para.password[1], User_Memory_Para.password[2], User_Memory_Para.password[3]);
        /*��������ҳ��*/
        conf_page_ui.select_item = 2 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

/*�򵥼���*/
void sample_encrypt_passwd(int8_t passwd[4])
{
    passwd[0] = passwd[0] + 1 ;
    passwd[1] = passwd[1] + 1 ;
    passwd[2] = passwd[2] + 1 ;
    passwd[3] = passwd[3] + 1 ;
}

/*�򵥽���*/
void sample_decode_passwd(int8_t passwd[4])
{
    passwd[0] = passwd[0] - 1 ;
    passwd[1] = passwd[1] - 1 ;
    passwd[2] = passwd[2] - 1 ;
    passwd[3] = passwd[3] - 1 ;
}


/*��������ҳ���ʼ��*/
void password_input_page_init(void)
{
    Flow_Cursor.flow_cursor = PASSWD_INPUT_PAGE ;
    password_page_para_init();
    password_input_item_display(1);
    password_number_display(1);
}

/*��������ҳ�水������*/
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

    /*��������˳��ص���ҳ��*/
    case LEFT_LONG:
        password_input_item_display(0);
        password_number_display(0);
        display_tencent_logo(1);
        main_page_init();
        break ;

    /*�Ҽ�ȷ�Ͻ�����һ��ҳ��*/
    case RIGHT_LONG:
        for(int i = 0 ; i < 4 ; i++)
        {
            if(password_page_ui.Password[i] != User_Memory_Para.password[i])
            {
                error_flag = 1 ;
                break ;
            }
        }

        /*�������*/
        if(error_flag == 1)
        {
            //����ϵͳ����
            tos_knl_sched_lock();
            /*��ʾ������ʾ*/
            password_input_error_display(1);
            HAL_Delay(1000);
            password_input_error_display(0);
            //����ϵͳ����
            tos_knl_sched_unlock();
            //���³�ʼ�������������
            password_input_page_init();
            password_number_display(1);
        }
        /*������ȷ����ת������ҳ��*/
        else
        {
            password_input_item_display(0);
            password_number_display(0);
            /*��������ҳ��*/
            conf_page_ui.select_item = 0 ;
            Flow_Cursor.flow_cursor = CONF_PAGE ;
            conf_page_ui_init(conf_page_ui.select_item);
        }

        break ;

    default:
        break ;
    }
}
