#include "plot_mode_ui.h"

/*�������ݳ�ʼ��*/
void plot_data_init(void)
{
    for(int i = 0 ; i < DATA_SIZE ; i++)
        plot_handler.rel_data_data[i] = mq2_sensor_interface.Smoke_Value ;

    memcpy(plot_handler.new_plot_data, plot_handler.rel_data_data, sizeof(plot_handler.new_plot_data));
    memcpy(plot_handler.old_plot_data, plot_handler.new_plot_data, sizeof(plot_handler.new_plot_data));
}

/*����ҳ���ʼ��*/
void plot_page_init(void)
{
    Item Plot_Item_Display[] = {{70, 0, "����ģʽ", BLACK, BLACK, 24, 1}};
    plot_handler.plot_mode = 1 ;
    plot_handler.fixed_screen = 0 ;
    Flow_Cursor.flow_cursor = PLOT_PAGE ;
    lcd_model.lcd_driver->lcd_clear(BLACK);
    display_menu_item(Plot_Item_Display, 0);
    plot_data_init();
}

/*���߶���*/
void plot_fixed_screen(void)
{
    switch(plot_handler.fixed_screen)
    {
    case 0:
        plot_handler.fixed_screen = 1 ;
        break ;

    case 1:
        plot_handler.fixed_screen = 0 ;
        break ;

    default:
        break ;
    }
}

/*�������*/
void plot_clear_screen(void)
{
    /*������*/
    tos_knl_sched_lock();

    for(int i = 0 ; i < DATA_SIZE ; i++)
        plot_handler.rel_data_data[i] = 0 ;

    memcpy(plot_handler.new_plot_data, plot_handler.rel_data_data, sizeof(plot_handler.new_plot_data));
    memcpy(plot_handler.old_plot_data, plot_handler.new_plot_data, sizeof(plot_handler.new_plot_data));
    LCD_Plot_Display(plot_handler.old_plot_data, DATA_SIZE, BLACK);
    LCD_Plot_Remap(plot_handler.rel_data_data, plot_handler.new_plot_data, DATA_SIZE);
    LCD_Plot_Display(plot_handler.new_plot_data, DATA_SIZE, GREEN);
    /*��������*/
    tos_knl_sched_unlock();
    /*������*/
    tos_knl_sched_lock();
    plot_page_init();
    /*��������*/
    tos_knl_sched_unlock();
}

/*����ҳ�水������*/
void plot_page_process(uint8_t KeyValue)
{
    switch(KeyValue)
    {
    /*����*/
    case LEFT:
        plot_fixed_screen();
        break ;

    /*�������*/
    case RIGHT:
        plot_clear_screen();
        break ;

    /*�˳�����ģʽ*/
    case LEFT_LONG:
    case RIGHT_LONG:
        plot_handler.plot_mode = 0 ;
        plot_handler.fixed_screen = 0 ;
        lcd_model.lcd_driver->lcd_clear(BLACK);
        Get_RTC_Date_Time();
        conf_page_ui.select_item = 4 ;
        Flow_Cursor.flow_cursor = CONF_PAGE ;
        conf_page_ui_init(conf_page_ui.select_item);
        break ;

    default:
        break ;
    }
}

