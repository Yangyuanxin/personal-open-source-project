#ifndef __LCD_ST7735_H
#define __LCD_ST7735_H
#include "main.h"


/*背光*/
#define LCD_BLK_Pin				GPIO_PIN_8
#define LCD_BLK_GPIO_Port GPIOA

/*写(命令-数据)*/
#define LCD_RS_Pin				GPIO_PIN_5
#define LCD_RS_GPIO_Port 	GPIOC

/*复位*/
#define LCD_RST_Pin				GPIO_PIN_4
#define LCD_RST_GPIO_Port GPIOC

/*片选*/
#define LCD_CS_Pin				GPIO_PIN_9
#define LCD_CS_GPIO_Port  GPIOC

#define LCD_BLK(n)   (n?HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET))
#define LCD_RS(n) (n?HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET))
#define	LCD_RST(n)	 (n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define	LCD_CS(n)		 (n?HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET))


//LCD
#define LCD_CS_0		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_1		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_RST_0		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_RST_1		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#define LCD_DC_0		HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
#define LCD_DC_1		HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)

#define LCD_BL_0		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET)
#define LCD_BL_1		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET)



#define	COLOR				uint16_t		//The variable type of the color (unsigned short) 
#define	POINT				uint16_t		//The type of coordinate (unsigned short) 
#define	LENGTH				uint16_t		//The type of coordinate (unsigned short) 



/********************************************************************************
function:
		Define the full screen height length of the display
********************************************************************************/
#define LCD_1IN44
//#define LCD_1IN8
#if defined(LCD_1IN44)
    #define LCD_WIDTH  128  //LCD width
    #define LCD_HEIGHT  128 //LCD height
    #define LCD_X	2
    #define LCD_Y	1
#elif defined(LCD_1IN8)
    #define LCD_WIDTH  128  //LCD width
    #define LCD_HEIGHT  160 //LCD height
    #define LCD_X	 2
    #define LCD_Y	 1
#endif

#define LCD_X_MAXPIXEL  132  //LCD width maximum memory 
#define LCD_Y_MAXPIXEL  162 //LCD height maximum memory


/********************************************************************************
function:
			scanning method
********************************************************************************/
typedef enum
{
    L2R_U2D  = 0,	//The display interface is displayed , left to right, up to down
    L2R_D2U,
    R2L_U2D,
    R2L_D2U,	//ok

    U2D_L2R,
    U2D_R2L,
    D2U_L2R,
    D2U_R2L,
} LCD_SCAN_DIR;
#define SCAN_DIR_DFT  R2L_D2U  //Default scan direction = L2R_U2D


/********************************************************************************
function:
	Defines the total number of rows in the display area
********************************************************************************/

typedef struct
{
    LENGTH LCD_Dis_Column;	//COLUMN
    LENGTH LCD_Dis_Page;	//PAGE
    LCD_SCAN_DIR LCD_Scan_Dir;
    POINT LCD_X_Adjust;		//LCD x actual display position calibration
    POINT LCD_Y_Adjust;		//LCD y actual display position calibration
} LCD_DIS;
extern LCD_DIS sLCD_DIS;


/********************************************************************************
function:
			dot pixel
********************************************************************************/
typedef enum
{
    DOT_PIXEL_1X1  = 1,		// dot pixel 1 x 1
    DOT_PIXEL_2X2, 		// dot pixel 2 X 2
    DOT_PIXEL_3X3,		// dot pixel 3 X 3
    DOT_PIXEL_4X4,		// dot pixel 4 X 4
    DOT_PIXEL_5X5, 		// dot pixel 5 X 5
    DOT_PIXEL_6X6, 		// dot pixel 6 X 6
    DOT_PIXEL_7X7, 		// dot pixel 7 X 7
    DOT_PIXEL_8X8, 		// dot pixel 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

/********************************************************************************
function:
			dot Fill style
********************************************************************************/
typedef enum
{
    DOT_FILL_AROUND  = 1,		// dot pixel 1 x 1
    DOT_FILL_RIGHTUP, 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex
/********************************************************************************
function:
			solid line and dotted line
********************************************************************************/
typedef enum
{
    LINE_SOLID = 0,
    LINE_DOTTED,
} LINE_STYLE;

/********************************************************************************
function:
			DRAW Internal fill
********************************************************************************/
typedef enum
{
    DRAW_EMPTY = 0,
    DRAW_FULL,
} DRAW_FILL;

/********************************************************************************
function:
			Defines commonly used colors for the display
********************************************************************************/
#define LCD_BACKGROUND		WHITE   //Default background color
#define FONT_BACKGROUND		WHITE   //Default font background color
#define FONT_FOREGROUND	    GRED    //Default font foreground color

#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BRED           0XF81F
#define GRED 		   0XFFE0
#define GBLUE		   0X07FF
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN 		   0XBC40
#define BRRED 		   0XFC07
#define GRAY  		   0X8430

/********************************************************************************
function:
			Macro definition variable name
********************************************************************************/
void LCD_Init( LCD_SCAN_DIR Lcd_ScanDir );
void LCD_SetGramScanWay(LCD_SCAN_DIR Scan_dir);

//LCD set cursor + windows + color
void LCD_SetWindows( POINT Xstart, POINT Ystart, POINT Xend, POINT Yend );
void LCD_SetCursor ( POINT Xpoint, POINT Ypoint );
void LCD_SetColor( COLOR Color, POINT Xpoint, POINT Ypoint);
void LCD_SetPointlColor ( POINT Xpoint, POINT Ypoint, COLOR Color );
void LCD_SetArealColor ( POINT Xstart, POINT Ystart, POINT Xend, POINT Yend, COLOR  Color);
void LCD_Clear(COLOR  Color);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);

//Drawing
void LCD_DrawPoint(POINT Xpoint, POINT Ypoint, COLOR Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
void LCD_DrawLine ( POINT Xstart, POINT Ystart, POINT Xend, POINT Yend, COLOR Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel);
void LCD_DrawRectangle ( POINT Xstart, POINT Ystart, POINT Xend, POINT Yend, COLOR Color, DRAW_FILL Filled, DOT_PIXEL Dot_Pixel );
void LCD_DrawCircle ( POINT X_Center, POINT Y_Center, LENGTH Radius, COLOR Color, DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel );

//Display string
//void LCD_DisplayChar( POINT Xstart, POINT Ystart, const char Acsii_Char, sFONT* Font, COLOR Color_Background, COLOR Color_Foreground );
//void LCD_DisplayString (POINT Xstart, POINT Ystart, const char * pString, sFONT* Font, COLOR Color_Background, COLOR Color_Foreground );
//void LCD_DisplayNum(POINT Xpoint, POINT Ypoint, int32_t Nummber, sFONT* Font, COLOR Color_Background, COLOR Color_Foreground );
void LCD_Show(void);



#endif //__LCD_ST7735_H

