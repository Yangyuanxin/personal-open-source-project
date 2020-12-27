#ifndef _LCD_SPI2_DRV_H_
#define _LCD_SPI2_DRV_H_
#include "main.h"

#define LCD_PWR_Pin 		GPIO_PIN_15
#define LCD_PWR_GPIO_Port 	GPIOB
#define LCD_WR_RS_Pin 		GPIO_PIN_6
#define LCD_WR_RS_GPIO_Port GPIOC
#define LCD_RST_Pin 		GPIO_PIN_7
#define LCD_RST_GPIO_Port 	GPIOC

#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET))
#define	LCD_WR_RS(n)	(n?HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define LCD_Width		240
#define LCD_Height		240

/* Init script function */
struct st7789_function
{
    uint8_t cmd;
    uint8_t data;
};

/* Init script commands */
enum st7789_cmd
{
    ST7789_START,
    ST7789_END,
    ST7789_CMD,
    ST7789_DATA,
    ST7789_DELAY
};

/* ST7789 Commands */
#define ST7789_CASET	0x2A
#define ST7789_RASET	0x2B
#define ST7789_RAMWR	0x2C
#define ST7789_RAMRD	0x2E


void LCD_Write_Cmd(uint8_t cmd);
void LCD_Write_Data(uint8_t dat);
void LCD_Write_2Byte(const uint16_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);
void LCD_DisplayOnoff(uint8_t status);
void LCD_Clear(uint16_t color);
void LCD_Init(void);			//LCD³õÊ¼»¯º¯Êý
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_Draw_ColorLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_Draw_ColorRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_Draw_ColorCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size);
void LCD_ShowCharStr(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size);
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);

#endif /* _LCD_SPI2_DRV_H_ */
