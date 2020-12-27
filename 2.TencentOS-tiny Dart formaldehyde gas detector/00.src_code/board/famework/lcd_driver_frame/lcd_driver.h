#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H
#include <stdint.h>

/*画笔颜色*/
#define WHITE         0xFFFF	//白色
#define YELLOW        0xFFE0	//黄色
#define BRRED 		  0XFC07    //棕红色
#define PINK          0XF81F	//粉色
#define RED           0xF800	//红色
#define BROWN 		  0XBC40    //棕色
#define GRAY  		  0X8430    //灰色
#define GBLUE		  0X07FF	//兰色
#define GREEN         0x07E0	//绿色
#define BLUE          0x001F    //蓝色
#define BLACK         0x0000	//黑色

/*给一块区域赋色接口参数*/
typedef struct
{
    uint16_t x_start;
    uint16_t x_end	;
    uint16_t y_start;
    uint16_t y_end	;
    uint16_t color	;
} LCD_Fill_Para ;

/*画线显示接口参数*/
typedef struct
{
    uint16_t x_start;
    uint16_t x_end	;
    uint16_t y_start;
    uint16_t y_end	;
    uint16_t color	;
} LCD_Line_Show_Para ;

/*画矩形显示接口参数*/
typedef struct
{
    uint16_t x_start;
    uint16_t x_end	;
    uint16_t y_start;
    uint16_t y_end	;
    uint16_t color	;
} LCD_Rect_Show_Para ;

/*ASCII字符串显示接口参数*/
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint8_t max_width;
    char* str;
    uint16_t bc;
    uint16_t fc;
    uint8_t sizey;
} LCD_Ascii_Show_Para ;

/*图像显示接口参数*/
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    const uint8_t *p;
} LCD_Image_Show_Para ;

/*中文显示接口参数*/
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint8_t *s;
    uint16_t fc;
    uint16_t bc;
    uint8_t sizey;
    uint8_t mode ;
} LCD_Chinese_Show_Para ;

/*bmp显示接口参数*/
typedef struct
{
    uint16_t x;
    uint16_t y;
    char *pic_name;
} LCD_Bmp_Show_Para ;

/*lcd驱动接口*/
typedef struct
{
    /*LCD初始化*/
    void (*lcd_init)(void);
    /*LCD清屏*/
    void (*lcd_clear)(uint16_t);
    /*控制屏幕背光亮灭*/
    void (*lcd_display_onoff)(uint8_t);
    /*给一块指定区域赋色*/
    void (*lcd_fill)(LCD_Fill_Para para);

    /*LCD画线*/
    void (*lcd_draw_line)(LCD_Line_Show_Para para);
    /*LCD画矩形*/
    void (*lcd_draw_rect)(LCD_Rect_Show_Para para);

    /*LCD显示图像*/
    void (*lcd_show_image)(LCD_Image_Show_Para para);
    /*LCD显示中文字符串*/
    void (*lcd_show_chinese_str)(LCD_Chinese_Show_Para para);
    /*LCD显示ASCII字符串*/
    void (*lcd_show_ascii_str)(LCD_Ascii_Show_Para para);

    /*LCD显示BMP图像*/
    void (*Lcd_show_bmp)(LCD_Bmp_Show_Para para);
} LCD_Driver ;
extern LCD_Driver lcd_driver;








#endif //LCD_DRIVER_H

