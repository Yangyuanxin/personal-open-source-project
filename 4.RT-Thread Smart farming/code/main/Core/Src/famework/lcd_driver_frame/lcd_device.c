#include "lcd_device.h"
#include "lcd_spi2_drv.h"
#include "bsp_bmp.h"


LCD_Device  lcd_device =
{
    .lcd_init              =  LCD_Init,
    .lcd_fill		       =  LCD_Fill,
    .lcd_clear 		       =  LCD_Clear,
    .lcd_display_onoff     =  LCD_DisplayOnoff,

    .lcd_draw_rect         =  LCD_Draw_ColorRect,
    .lcd_draw_line     	   =  LCD_Draw_ColorLine,

    .lcd_show_image        =  LCD_Show_Image,
    .lcd_show_ascii_str    =  LCD_ShowCharStr,
    .lcd_show_chinese_str  =  LCD_ShowChinese,
    .Lcd_show_bmp		   =  Lcd_show_bmp,
};
