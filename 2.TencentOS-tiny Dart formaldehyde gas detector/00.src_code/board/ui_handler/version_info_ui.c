#include "version_info_ui.h"

/*ID地址*/
#define ID_ADDR 		*(__IO uint32_t *)0x1FFF7590
/*FLASH地址*/
#define FLASH_ADDR		*(__IO uint16_t *)0x1FFF75E0
/*MCU封装类型地址*/
#define MCU_TYPE_ADDR	*(__IO uint32_t *)0x1FFF7500

Type Package_Type[] =
{
    {0x00000, "LQFP64"},
    {0x00001, "WLCSP64"},
    {0x00010, "LQFP100"},
    {0x00101, "WLCSP36"},
    {0x01000, "UFQFPN32"},
    {0x01001, "LQFP32"},
    {0x01010, "UFQFPN48"},
    {0x01011, "LQFP48"},
    {0x01100, "WLCSP49"},
    {0x01101, "UFBGA64"},
    {0x01110, "UFBGA100"},
    {0x01111, "WLCSP36 with external SMPS"},
    {0x10110, "LQFP64 with external SMPS"},
};

version_para version_para_info ;

/*版本信息显示参数*/
LCD_Ascii_Show_Para ascii_show_para[] =
{
    /*显示*/
    {VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, version_para_info.id, BLACK, RED, VERSION1_TEXT_FONT},
    {VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, version_para_info.flash_size, BLACK, RED, VERSION2_TEXT_FONT},
    {VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, version_para_info.Mcu_Package, BLACK, RED, VERSION3_TEXT_FONT},
    /*隐藏*/
    {VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, version_para_info.id, BLACK, BLACK, VERSION1_TEXT_FONT},
    {VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, version_para_info.flash_size, BLACK, BLACK, VERSION2_TEXT_FONT},
    {VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, version_para_info.Mcu_Package, BLACK, BLACK, VERSION3_TEXT_FONT}
};

/*显示1隐藏0版本信息*/
void display_version_info(int enable)
{
    uint32_t Type_Value = 0 ;
    char kernel_version[30] = {0};
    memset(kernel_version, 0, 30);
    memset(version_para_info.id, 0, 50);
    memset(version_para_info.flash_size, 0, 20);
    memset(version_para_info.Mcu_Package, 0, 20);
    sprintf(kernel_version, "Power by TencentOS tiny");
    sprintf(version_para_info.id, "id:0x%x %x %x", ID_ADDR, ID_ADDR + 0x04, ID_ADDR + 0x08);
    sprintf(version_para_info.flash_size, "flash_size:%dKB", FLASH_ADDR);
    Type_Value = MCU_TYPE_ADDR & 0x1f ;
    LCD_Ascii_Show_Para ascii_show_tencent_os_tiny_para = {20, 200, 220, kernel_version, BLACK, WHITE, 16};
    LCD_Ascii_Show_Para ascii_hide_tencent_os_tiny_para = {20, 200, 220, kernel_version, BLACK, BLACK, 16};

    for(int i = 0 ; i < sizeof(Package_Type) / sizeof(Package_Type[0]); i++)
    {
        if(Type_Value == Package_Type[i].value)
        {
            sprintf(version_para_info.Mcu_Package, "Mcu_Package:%s", Package_Type[i].Name);
            break ;
        }
    }

    if(enable == 1)
    {
        for(int i = 0 ; i < 3 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_para[i]);

        /*显示Power by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_tencent_os_tiny_para);
    }
    else if(enable == 0)
    {
        for(int i = 3 ; i < 6 ; i++)
            lcd_model.lcd_driver->lcd_show_ascii_str(ascii_show_para[i]);

        /*隐藏Power by TencentOS tiny*/
        lcd_model.lcd_driver->lcd_show_ascii_str(ascii_hide_tencent_os_tiny_para);
    }
}

/*版本信息页面初始化*/
void Version_Info_Page_Init(void)
{
    Flow_Cursor.flow_cursor = VERSION_PAGE ;
    display_version_info(1);
    printf("版本信息页面初始化：%d\n", Flow_Cursor.flow_cursor);
}


/*按键逻辑处理*/
static void Handle_Version_Info_Page_Left_Key(void)
{
    display_version_info(0);
    conf_page_ui.select_item = 4 ;
    conf_page_ui_init(conf_page_ui.select_item);
}

static void Handle_Version_Info_Page_Left_Long_Key(void)
{
    display_version_info(0);
    recovery_page_init();
}

static void Handle_Version_Info_Page_Right_Key(void)
{
    display_version_info(0);
    conf_page_ui.select_item = 4 ;
    conf_page_ui_init(conf_page_ui.select_item);
}


Event_Frame Version_Info_Page_Event[] =
{
    {Handle_Version_Info_Page_Left_Key},
    {Handle_Version_Info_Page_Left_Long_Key},
    {Handle_Version_Info_Page_Right_Key},
    NULL,
};

/*版本信息按键操作*/
void version_info_page_process(uint8_t KeyValue)
{
    if(Version_Info_Page_Event[KeyValue - 1].handler_func != NULL)
        Version_Info_Page_Event[KeyValue - 1].handler_func();
}
