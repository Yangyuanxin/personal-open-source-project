/**
  ******************************************************************************
  * 文件名程: bsp_bmp.c
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: bmp图像显示实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  *
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "ff.h"
#include "bsp_bmp.h"
#include "lcd_spi2_drv.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* R(8bit) G(8bit) B(8bit)  --> RGB565  */
#define RGB24TORGB16(R,G,B) ((R>>3)<<11)|((G>>2)<<5)|(B>>3)

/* 如果不需要打印bmp相关的提示信息,将printf注释掉即可
 * 如要用printf()，需将串口驱动文件包含进来
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)	

/* 私有变量 ------------------------------------------------------------------*/
//uint8_t pColorData[960];					/* 一行真彩色数据缓存 320 * 3 = 960 */
uint8_t pColorData[720];					/* 一行真彩色数据缓存 240 * 3 = 720 */
FIL file;
extern FRESULT f_res;
UINT f_num;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 打印BMP文件的头信息，用于调试
  * 输入参数: pBmpHead:BMP文件的头信息
  * 返 回 值: 无
  * 说    明：无
  */
static void showBmpHeader(BMP_FileHeader *pBmpHead)
{
    BMP_DEBUG_PRINTF("位图文件头:\n");
    BMP_DEBUG_PRINTF("文件类型:%d\n", (*pBmpHead).bfType);
    BMP_DEBUG_PRINTF("文件大小:%d\n", (*pBmpHead).bfSize);
    BMP_DEBUG_PRINTF("保留字:%d\n", (*pBmpHead).bfReserved1);
    BMP_DEBUG_PRINTF("保留字:%d\n", (*pBmpHead).bfReserved2);
    BMP_DEBUG_PRINTF("实际位图数据的偏移字节数:%d\n", (*pBmpHead).bfOffBits);
    BMP_DEBUG_PRINTF("\n");
}

/**
  * 函数功能: 打印BMP文件的头信息，用于调试
  * 输入参数: pBmpHead:BMP文件的头信息
  * 返 回 值: 无
  * 说    明：无
  */
static void showBmpInforHeader(BMP_InfoHeader *pBmpInforHead)
{
    BMP_DEBUG_PRINTF("位图信息头:\n");
    BMP_DEBUG_PRINTF("结构体的长度:%d\n", (*pBmpInforHead).biSize);
    BMP_DEBUG_PRINTF("位图宽:%d\n", (*pBmpInforHead).biWidth);
    BMP_DEBUG_PRINTF("位图高:%d\n", (*pBmpInforHead).biHeight);
    BMP_DEBUG_PRINTF("biPlanes平面数:%d\n", (*pBmpInforHead).biPlanes);
    BMP_DEBUG_PRINTF("biBitCount采用颜色位数:%d\n", (*pBmpInforHead).biBitCount);
    BMP_DEBUG_PRINTF("压缩方式:%d\n", (*pBmpInforHead).biCompression);
    BMP_DEBUG_PRINTF("biSizeImage实际位图数据占用的字节数:%d\n", (*pBmpInforHead).biSizeImage);
    BMP_DEBUG_PRINTF("X方向分辨率:%d\n", (*pBmpInforHead).biXPelsPerMeter);
    BMP_DEBUG_PRINTF("Y方向分辨率:%d\n", (*pBmpInforHead).biYPelsPerMeter);
    BMP_DEBUG_PRINTF("使用的颜色数:%d\n", (*pBmpInforHead).biClrUsed);
    BMP_DEBUG_PRINTF("重要颜色数:%d\n", (*pBmpInforHead).biClrImportant);
    BMP_DEBUG_PRINTF("\n");
}

/**
  * 函数功能: 显示bmp图片, 24位真彩色
  * 输入参数: x:显示图片左上角x轴坐标
  *           y:显示图片左上角y轴坐标
  *           pic_name:显示图片文件名称
  * 返 回 值: 无
  * 说    明：图片宽度和高度根据图片大小而定
  */
void Lcd_show_bmp(uint16_t x, uint16_t y, char *pic_name)
{
    tos_knl_sched_lock();
    uint16_t i, j, k;
    int width, height, l_width;

    BMP_FileHeader FileHeader;
    BMP_InfoHeader InfoHeader;

    /*-------------------------------------------------------------------------------------------------------*/
    f_res = f_open(&file, pic_name, FA_OPEN_EXISTING | FA_READ);

    if(f_res == FR_OK)
    {
        BMP_DEBUG_PRINTF("Open file success\r\n");

        /* 读取文件头信息  两个字节*/
        f_res = f_read(&file, &FileHeader, sizeof(BMP_FileHeader), &f_num);

        /* 判断是不是bmp文件 "BM"*/
        if(FileHeader.bfType != 0x4d42)
        {
            BMP_DEBUG_PRINTF("file is not .bmp file!\r\n");
            return;
        }
        else
        {
            BMP_DEBUG_PRINTF("Ok this is .bmp file\r\n");
        }

        /* 读取BMP文件头信息*/
        showBmpHeader(&FileHeader);

        /* 读取位图信息头信息 */
        f_res = f_read(&file, &InfoHeader, sizeof(BMP_InfoHeader), &f_num);
        showBmpInforHeader(&InfoHeader);
    }
    else
    {
        BMP_DEBUG_PRINTF("file open fail!\r\n");
        return;
    }

    /*-------------------------------------------------------------------------------------------------------*/
    width  = InfoHeader.biWidth;
    height = InfoHeader.biHeight;

    /* 计算位图的实际宽度并确保它为32的倍数	*/
    l_width = WIDTHBYTES(width * InfoHeader.biBitCount);

    if((l_width > 960) || (InfoHeader.biBitCount != 24))
    {
        BMP_DEBUG_PRINTF("\n SORRY, PIC IS TOO BIG (X<=320 and bit!=16)\n");
        return;
    }

    f_lseek(&file, FileHeader.bfOffBits);

    if(InfoHeader.biBitCount == 24)
    {
        for(i = 0; i < height; ++i)
        {
            /* 开一个图片大小的窗口*/
            LCD_OpenWindow(x, y + height - i - 1, width, 1);
            LCD_Write_Cmd(0x2C);
            /* 读取一行bmp的数据到数组pColorData里面 */
            f_read(&file, pColorData, l_width, &f_num);

            for(j = 0; j < width; j++) //一行有效信息
            {
                k = j * 3;	//一行中第K个像素的起点
                LCD_Write_2Byte(RGB24TORGB16(pColorData[k + 2], pColorData[k + 1], pColorData[k]));
            }
        }
    }

    f_close(&file);
    tos_knl_sched_unlock();
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

