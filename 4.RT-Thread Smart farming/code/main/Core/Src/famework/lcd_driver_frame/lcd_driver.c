#include "lcd_driver.h"
#include "lcd_device.h"


/*LCD初始化*/
static void lcd_init(void)
{
    lcd_device.lcd_init();
}

/*LCD清屏*/
static void lcd_clear(uint16_t color)
{
    lcd_device.lcd_clear(color);
}


/*控制屏幕背光亮灭*/
static void lcd_display_onoff(uint8_t status)
{
    lcd_device.lcd_display_onoff(status);
}


/*给一块指定区域赋色*/
static void lcd_fill(LCD_Fill_Para para)
{
    lcd_device.lcd_fill(para.x_start, para.y_start, para.x_end, para.y_end, para.color);
}


/*LCD画线*/
static void lcd_draw_line(LCD_Line_Show_Para para)
{
    lcd_device.lcd_draw_line(para.x_start, para.y_start, para.x_end, para.y_end, para.color);
}

/*LCD画矩形*/
static void lcd_draw_rect(LCD_Rect_Show_Para para)
{
    lcd_device.lcd_draw_rect(para.x_start, para.y_start, para.x_end, para.y_end, para.color);
}

/*LCD显示图像*/
static void lcd_show_image(LCD_Image_Show_Para para)
{
    lcd_device.lcd_show_image(para.x, para.y, para.width, para.height, para.p);
}


/*LCD显示中文字符串*/
static void lcd_show_chinese_str(LCD_Chinese_Show_Para para)
{
    lcd_device.lcd_show_chinese_str(para.x, para.y, para.s, para.fc, para.bc, para.sizey, para.mode);
}

/*LCD显示ASCII字符串*/
void lcd_show_ascii_str(LCD_Ascii_Show_Para para)
{
    lcd_device.lcd_show_ascii_str(para.x, para.y, para.max_width, para.str, para.bc, para.fc, para.sizey);
}

/*LCD显示BMP图像*/
void lcd_show_bmp(LCD_Bmp_Show_Para para)
{
    lcd_device.Lcd_show_bmp(para.x, para.y, para.pic_name);
}

LCD_Driver lcd_driver =
{
    .lcd_init              =  lcd_init,
    .lcd_fill		       =  lcd_fill,
    .lcd_clear 		       =  lcd_clear,
    .lcd_display_onoff     =  lcd_display_onoff,

    .lcd_draw_rect         =  lcd_draw_rect,
    .lcd_draw_line     	   =  lcd_draw_line,

    .lcd_show_image        =  lcd_show_image,
    .lcd_show_ascii_str    =  lcd_show_ascii_str,
    .lcd_show_chinese_str  =  lcd_show_chinese_str,
    .Lcd_show_bmp					 =  lcd_show_bmp,
};



