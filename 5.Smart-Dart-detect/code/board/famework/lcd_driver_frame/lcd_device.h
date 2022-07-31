#ifndef __LCD_DEVICE_H
#define __LCD_DEVICE_H
#include <stdint.h>


/*lcd�豸�ӿ�*/
typedef struct
{
    /*LCD��ʼ��*/
    void (*lcd_init)(void);
    /*LCD����*/
    void (*lcd_clear)(uint16_t);
    /*������Ļ��������*/
    void (*lcd_display_onoff)(uint8_t);
    /*��һ��ָ������ɫ*/
    void (*lcd_fill)(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);

    /*LCD����*/
    void (*lcd_draw_line)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t);
    /*LCD������*/
    void (*lcd_draw_rect)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t);

    /*LCD��ʾͼ��*/
    void (*lcd_show_image)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
    /*LCD��ʾ�����ַ���*/
    void (*lcd_show_chinese_str)(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
    /*LCD��ʾASCII�ַ���*/
    void (*lcd_show_ascii_str)(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size);

    /*LCD��ʾBMPͼ��*/
    void (*Lcd_show_bmp)(uint16_t x, uint16_t y, char *pic_name);
} LCD_Device ;
extern LCD_Device  lcd_device ;





#endif //__LCD_DEVICE_H

