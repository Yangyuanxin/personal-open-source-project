#include "lcd_spi2_drv.h"
#include "spi2.h"
#include "font.h"


#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152

static uint8_t lcd_buf[LCD_Buf_Size];

static struct st7789_function st7789_cfg_script[] =
{
    {ST7789_START, ST7789_START},
    /* Sleep Out */
    {ST7789_CMD, 0x11},
    {ST7789_DELAY, 120},
    /* Memory Data Access Control */
    {ST7789_CMD, 0x36},
    {ST7789_DATA, 0x00},
    /* RGB 5-6-5-bit  */
    {ST7789_CMD, 0x3a},
    {ST7789_DATA, 0x65},
    /* Porch Setting */
    {ST7789_CMD, 0xb2},
    {ST7789_DATA, 0x0c},
    {ST7789_DATA, 0x0c},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0x33},
    {ST7789_DATA, 0x33},
    /*  Gate Control */
    {ST7789_CMD, 0xb7},
    {ST7789_DATA, 0x72},
    /* VCOM Setting */
    {ST7789_CMD, 0xbb},
    {ST7789_DATA, 0x3d},//Vcom=1.625V
    /* LCM Control */
    {ST7789_CMD, 0xc0},
    {ST7789_DATA, 0x2c},
    /* VDV and VRH Command Enable */
    {ST7789_CMD, 0xc2},
    {ST7789_DATA, 0x01},
    /* VRH Set */
    {ST7789_CMD, 0xc3},
    {ST7789_DATA, 0x19},
    /* VDV Set */
    {ST7789_CMD, 0xc4},
    {ST7789_DATA, 0x20},
    /* Frame Rate Control in Normal Mode */
    {ST7789_CMD, 0xc6},
    {ST7789_DATA, 0x01},
    /* Power Control 1 */
    {ST7789_CMD, 0xd0},
    {ST7789_DATA, 0xa4},
    {ST7789_DATA, 0xa1},
    /* Positive Voltage Gamma Control */
    {ST7789_CMD, 0xe0},
    {ST7789_DATA, 0xD0},
    {ST7789_DATA, 0x04},
    {ST7789_DATA, 0x0D},
    {ST7789_DATA, 0x11},
    {ST7789_DATA, 0x13},
    {ST7789_DATA, 0x2B},
    {ST7789_DATA, 0x3F},
    {ST7789_DATA, 0x54},
    {ST7789_DATA, 0x4C},
    {ST7789_DATA, 0x18},
    {ST7789_DATA, 0x0D},
    {ST7789_DATA, 0x0B},
    {ST7789_DATA, 0x1F},
    {ST7789_DATA, 0x23},
    /* Negative Voltage Gamma Control */
    {ST7789_CMD, 0xe1},
    {ST7789_DATA, 0xD0},
    {ST7789_DATA, 0x04},
    {ST7789_DATA, 0x0C},
    {ST7789_DATA, 0x11},
    {ST7789_DATA, 0x13},
    {ST7789_DATA, 0x2C},
    {ST7789_DATA, 0x3F},
    {ST7789_DATA, 0x44},
    {ST7789_DATA, 0x51},
    {ST7789_DATA, 0x2F},
    {ST7789_DATA, 0x1F},
    {ST7789_DATA, 0x10},
    {ST7789_DATA, 0x20},
    {ST7789_CMD, 0x23},
    /* Display Inversion On */
    {ST7789_CMD, 0x21},
    {ST7789_CMD, 0x29},
    #if 1
    {ST7789_CMD, 0x2a},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0xef},
    {ST7789_CMD, 0x2b},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0x00},
    {ST7789_DATA, 0xef},
    {ST7789_CMD, 0x2c},
    #endif
    {ST7789_END, ST7789_END},
};

/**
 *@brief		LCD控制引脚初始化
 *@param		none
 *@retval		none
*/

static void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, LCD_WR_RS_Pin | LCD_RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = LCD_PWR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_PWR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin = LCD_WR_RS_Pin | LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* 复位LCD */
    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);

    /* 初始化SPI2接口 */
    MX_SPI2_Init();
}

/**
 * @brief	LCD底层SPI发送数据函数
 * @param   data —— 数据的起始地址
 * @param   size —— 发送数据字节数
 * @return  none
 */
static void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
    for(int i = 0 ; i < size ; i++)
    {
        *((uint8_t*)&hspi2.Instance->DR) = data[i];

        while(__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1) {}
    }
}

/**
 * @brief	写命令到LCD
 * @param   cmd —— 需要发送的命令
 * @return  none
 */
void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR_RS(0);
    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief	写数据到LCD
 * @param 	dat —— 需要发送的数据
 * @return  none
 */
void LCD_Write_Data(uint8_t dat)
{
    LCD_WR_RS(1);
    LCD_SPI_Send(&dat, 1);
}
/**
 * @brief		写16位的数据（两个字节）到LCD
 * @param   dat —— 需要发送的16bit数据
 * @return  none
 */
void LCD_Write_2Byte(const uint16_t dat)
{
    uint8_t data[2] = {0};

    data[0] = dat >> 8;
    data[1] = dat;

    LCD_WR_RS(1);
    LCD_SPI_Send(data, 2);
}
/**
 * @brief	设置数据写入LCD缓存区域
 * @param   x1,y1	—— 起点坐标
 * @param   x2,y2	—— 终点坐标
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    LCD_Write_Cmd(0x2C);
}

/**
  * 函数功能: 在LCD显示器上开辟一个窗口
  * 输入参数: usX ：在特定扫描方向下窗口的起点X坐标
  *           usY ：在特定扫描方向下窗口的起点Y坐标
  *           usWidth ：窗口的宽度
  *           usHeight ：窗口的高度
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{
    LCD_Write_Cmd(0x2A ); 				       /* 设置X坐标 */
    LCD_Write_Data(usX >> 8);	           /* 设置起始点：先高8位 */
    LCD_Write_Data(usX & 0xff);	         /* 然后低8位 */
    LCD_Write_Data((usX + usWidth - 1) >> 8); /* 设置结束点：先高8位 */
    LCD_Write_Data((usX + usWidth - 1) & 0xff); /* 然后低8位 */

    LCD_Write_Cmd(0x2B); 			           /* 设置Y坐标*/
    LCD_Write_Data(usY >> 8);            /* 设置起始点：先高8位 */
    LCD_Write_Data(usY & 0xff);          /* 然后低8位 */
    LCD_Write_Data((usY + usHeight - 1) >> 8); /* 设置结束点：先高8位 */
    LCD_Write_Data((usY + usHeight - 1) & 0xff); /* 然后低8位 */
}

/*打开/关闭背光*/
void LCD_DisplayOnoff(uint8_t status)
{
    if(0 == status)
        LCD_PWR(0);
    else
        LCD_PWR(1);
}

/**
 * @brief	以一种颜色清空LCD屏
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    tos_knl_sched_lock();
    uint16_t i, j;
    uint8_t data[2] = {0};  //LCD屏幕色彩深度16bit，data[0]是颜色数据的高位，data[1]是颜色数据的低位

    data[0] = color >> 8;
    data[1] = color;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_WR_RS(1);

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
    }

    tos_knl_sched_unlock();
}


//配置启动参数
static void st7789_run_cfg_script(void)
{
    uint8_t data[2] = {0};
    int i = 0;
    int end_script = 0;

    do
    {
        switch (st7789_cfg_script[i].cmd)
        {
        case ST7789_START:
            break;

        case ST7789_CMD:
            data[0] = st7789_cfg_script[i].data ;
            LCD_Write_Cmd(data[0]);
            break;

        case ST7789_DATA:
            data[0] = st7789_cfg_script[i].data ;
            LCD_Write_Data(data[0]);
            break;

        case ST7789_DELAY:
            HAL_Delay(120);
            break;

        case ST7789_END:
            end_script = 1;
        }

        i++;
    }
    while (!end_script);
}

/**
 * @brief	LCD初始化
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
    LCD_GPIO_Init();
    /*使能SPI单线发送模式*/
    SPI_1LINE_TX(&hspi2);
    __HAL_SPI_ENABLE(&hspi2);
    st7789_run_cfg_script();
}
/**
 * @brief		带颜色画点函数
 * @param   x,y	—— 画点坐标
 * @return  none
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_2Byte(color);
}
/**
 * @brief		带颜色画线函数(直线、斜线)
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 * @return  none
 */
void LCD_Draw_ColorLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t	i = 0;
    int16_t		delta_x = 0, delta_y = 0;
    int8_t		incx = 0, incy = 0;
    uint16_t	distance = 0;
    uint16_t    t = 0;
    uint16_t	x = 0, y = 0;
    uint16_t 	x_temp = 0, y_temp = 0;


    if(y1 == y2)
    {
        /* 快速画水平线 */
        LCD_Address_Set(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_WR_RS(1);
        LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }
    else
    {
        /* 画斜线（Bresenham算法） */
        /* 计算两点之间在x和y方向的间距，得到画笔在x和y方向的步进值 */
        delta_x = x2 - x1;
        delta_y = y2 - y1;

        if(delta_x > 0)
        {
            //斜线(从左到右)
            incx = 1;
        }
        else if(delta_x == 0)
        {
            //垂直斜线(竖线)
            incx = 0;
        }
        else
        {
            //斜线(从右到左)
            incx = -1;
            delta_x = -delta_x;
        }

        if(delta_y > 0)
        {
            //斜线(从左到右)
            incy = 1;
        }
        else if(delta_y == 0)
        {
            //水平斜线(水平线)
            incy = 0;
        }
        else
        {
            //斜线(从右到左)
            incy = -1;
            delta_y = -delta_y;
        }

        /* 计算画笔打点距离(取两个间距中的最大值) */
        if(delta_x > delta_y)
        {
            distance = delta_x;
        }
        else
        {
            distance = delta_y;
        }

        /* 开始打点 */
        x = x1;
        y = y1;

        //第一个点无效，所以t的次数加一
        for(t = 0; t <= distance + 1; t++)
        {
            LCD_Draw_ColorPoint(x, y, color);

            /* 判断离实际值最近的像素点 */
            x_temp += delta_x;

            if(x_temp > distance)
            {
                //x方向越界，减去距离值，为下一次检测做准备
                x_temp -= distance;
                //在x方向递增打点
                x += incx;

            }

            y_temp += delta_y;

            if(y_temp > distance)
            {
                //y方向越界，减去距离值，为下一次检测做准备
                y_temp -= distance;
                //在y方向递增打点
                y += incy;
            }
        }
    }
}
/**
 * @breif	带颜色画矩形函数
 * @param   x1,y1 —— 矩形起始点
 * @param	x2,y2 —— 矩形终点
 * @param	color	—— 颜色
 * @retval	none
 */
void LCD_Draw_ColorRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    tos_knl_sched_lock();
    LCD_Draw_ColorLine(x1, y1, x2, y1, color);
    LCD_Draw_ColorLine(x1, y1, x1, y2, color);
    LCD_Draw_ColorLine(x1, y2, x2, y2, color);
    LCD_Draw_ColorLine(x2, y1, x2, y2, color);
    tos_knl_sched_unlock();
}
/**
 * @breif	带颜色画圆函数
 * @param   x1,x2 —— 圆心坐标
 * @param	r —— 半径
 * @param	color	—— 颜色
 * @retval	none
 */
void LCD_Draw_ColorCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    /* Bresenham画圆算法 */
    int16_t a = 0, b = r;
    int16_t d = 3 - (r << 1);		//算法决策参数

    /* 如果圆在屏幕可见区域外，直接退出 */
    if (x - r < 0 || x + r > LCD_Width || y - r < 0 || y + r > LCD_Height)
    {
        return;
    }

    /* 开始画圆 */
    while(a <= b)
    {
        LCD_Draw_ColorPoint(x - b, y - a, color);
        LCD_Draw_ColorPoint(x + b, y - a, color);
        LCD_Draw_ColorPoint(x - a, y + b, color);
        LCD_Draw_ColorPoint(x - b, y - a, color);
        LCD_Draw_ColorPoint(x - a, y - b, color);
        LCD_Draw_ColorPoint(x + b, y + a, color);
        LCD_Draw_ColorPoint(x + a, y - b, color);
        LCD_Draw_ColorPoint(x + a, y + b, color);
        LCD_Draw_ColorPoint(x - b, y + a, color);
        a++;

        if(d < 0)
        {
            d += 4 * a + 6;
        }
        else
        {
            d += 10 + 4 * (a - b);
            b--;
        }

        LCD_Draw_ColorPoint(x + a, y + b, color);
    }
}
/**
 * @brief	以一种颜色填充/清空某个矩形区域
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    tos_knl_sched_lock();
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x2 - x1 + 1) * (y2 - y1 + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x1, y1, x2, y2);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_WR_RS(1);
        LCD_SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }

    tos_knl_sched_unlock();
}

/**
 * @brief	显示一个ASCII码字符
 * @param   x,y		显示起始坐标
 * @param   ch		需要显示的字符
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note	需要font.h字库文件的支持
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
    int i = 0, j = 0;
    uint8_t temp = 0;
    uint8_t size = 0;
    uint8_t t = 0;

    /* 检测显示是否会越界 */
    if((x > (LCD_Width - font_size / 2)) || (y > (LCD_Height - font_size)))
        return;

    /* 根据字符大小设置显存操作区域 */
    LCD_Address_Set(x, y, x + font_size / 2 - 1, y + font_size - 1);

    /* 计算字符在字库中的偏移值*/
    ch = ch - ' ';

    /* 显示16号/32号字体 */
    if(font_size == 16)
    {
        /* 计算字体一个字符对应点阵集所占的字节数 */
        size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

        for(i = 0; i < size; i++)
        {
            if(font_size == 16)
                temp = asc2_1608[ch][i];	//调用1608字体
            else
                return;			//没有的字库

            for(j = 0; j < 8; j++)
            {
                if(temp & 0x80)
                    LCD_Write_2Byte(font_color);
                else
                    LCD_Write_2Byte(back_color);

                temp <<= 1;
            }
        }
    }
    /* 显示12号字体 */
    else if(font_size == 12)
    {
        /* 计算字体一个字符对应点阵集所占的字节数 */
        size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

        for(i = 0; i < size; i++)
        {
            temp = asc2_1206[ch][i];

            for(j = 0; j < 6; j++)
            {
                if(temp & 0x80)
                    LCD_Write_2Byte(font_color);
                else
                    LCD_Write_2Byte(back_color);

                temp <<= 1;
            }
        }
    }
    /* 显示24号字体 */
    else if(font_size == 24)
    {
        /* 计算字体一个字符对应点阵集所占的字节数 */
        size = (font_size * 16) / 8;

        for(i = 0; i < size; i++)
        {
            temp = asc2_2412[ch][i];

            if(i % 2 == 0)
                t = 8;
            else
                t = 4;

            for(j = 0; j < t; j++)
            {
                if(temp & 0x80)
                    LCD_Write_2Byte(font_color);
                else
                    LCD_Write_2Byte(back_color);

                temp <<= 1;
            }
        }
    }
    /* 其余字体 */
    else
        return;
}
/**
 * @brief		显示一个ASCII码字符串
 * @param   x,y		显示起始坐标
 * @param   str		需要显示的字符串
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note		1. 需要font.h字库文件的支持
 * 					2. 超过指定width不显示超过的字符
 */
void LCD_ShowCharStr(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
    tos_knl_sched_lock();
    max_width += x;

    while((*str <= '~') && (*str >= ' '))	//判断是否非法字符
    {
        if(x >= max_width)
        {
            //x方向越界，结束
            break;
        }

        LCD_ShowChar(x, y, *str, back_color, font_color, font_size);
        x += font_size / 2;
        str++;
    }

    tos_knl_sched_unlock();
}

/**
 * @brief	显示图片函数
 * @param   x,y	    —— 起点坐标
 * @param   width	—— 图片宽度
 * @param   height	—— 图片高度
 * @param   p       —— 图片缓存数据起始地址
 * @return  none
 * @note	Image2Lcd取模方式：C语言数据/水平扫描/16位真彩色(RGB565)/高位在前，其他的不选
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    uint32_t img_size = width * height * 2;		//图片所占字节数
    uint32_t remain_size = img_size;		    //图片每次发送后剩余的字节数
    uint8_t i = 0;

    /* 错误检测 */
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_WR_RS(1);

    /* SPI每次最大发送2^16 = 65536个数据,图片最大大小为240*240*2 = 115200，会超过此大小，所以设计循环发送算法 */
    for(i = 0; i <= img_size / 65536; i++)
    {
        if(remain_size / 65536 >= 1)
        {
            LCD_SPI_Send((uint8_t *)p, 65535);
            p += 65535;
            remain_size -= 65535;
        }
        else
        {
            LCD_SPI_Send((uint8_t *)p, remain_size % 65535);
        }

    }
}

/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);	//统计汉字数目

    for(k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for(i = 0; i < TypefaceNum; i++)
            {
                for(j = 0; j < 8; j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont24[k].Msk[i] & (0x01 << j))LCD_Write_Data(fc);
                        else LCD_Write_Data(bc);

                        m++;

                        if(m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont24[k].Msk[i] & (0x01 << j))	LCD_Draw_ColorPoint(x, y, fc); //画一个点

                        x++;

                        if((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;//汉字数目
    uint16_t TypefaceNum;//一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);	//统计汉字数目

    for(k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for(i = 0; i < TypefaceNum; i++)
            {
                for(j = 0; j < 8; j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i] & (0x01 << j))LCD_Write_Data(fc);
                        else LCD_Write_Data(bc);

                        m++;

                        if(m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32[k].Msk[i] & (0x01 << j))	LCD_Draw_ColorPoint(x, y, fc); //画一个点

                        x++;

                        if((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

#include <stdio.h>
/**********************************************************/
//显示汉字
/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    tos_knl_sched_lock();

    while(*s != 0)
    {
        if(sizey == 24) LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        else if(sizey == 32) LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        else return;

        s += 2;
        x += sizey;
    }

    tos_knl_sched_unlock();
}





