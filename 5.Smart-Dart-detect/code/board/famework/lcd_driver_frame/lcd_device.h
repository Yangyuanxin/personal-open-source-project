#ifndef __LCD_DEVICE_H
#define __LCD_DEVICE_H
#include <stdint.h>


/*lcd设备接口*/
typedef struct
{
    /*LCD初始化*/
    void (*lcd_init)(void);
    /*LCD清屏*/
    void (*lcd_clear)(uint16_t);
    /*控制屏幕背光亮灭*/
    void (*lcd_display_onoff)(uint8_t);
    /*给一块指定区域赋色*/
    void (*lcd_fill)(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);

    /*LCD画线*/
    void (*lcd_draw_line)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t);
    /*LCD画矩形*/
    void (*lcd_draw_rect)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t);

    /*LCD显示图像*/
    void (*lcd_show_image)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
    /*LCD显示中文字符串*/
    void (*lcd_show_chinese_str)(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
    /*LCD显示ASCII字符串*/
    void (*lcd_show_ascii_str)(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size);

    /*LCD显示BMP图像*/
    void (*Lcd_show_bmp)(uint16_t x, uint16_t y, char *pic_name);
} LCD_Device ;
extern LCD_Device  lcd_device ;





#endif //__LCD_DEVICE_H

