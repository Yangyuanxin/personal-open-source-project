/**
  ******************************************************************************
  * �ļ�����: bsp_bmp.c
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: bmpͼ����ʾʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  *
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "ff.h"
#include "bsp_bmp.h"
#include "lcd_spi2_drv.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* R(8bit) G(8bit) B(8bit)  --> RGB565  */
#define RGB24TORGB16(R,G,B) ((R>>3)<<11)|((G>>2)<<5)|(B>>3)

/* �������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
 * ��Ҫ��printf()���轫���������ļ���������
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)	

/* ˽�б��� ------------------------------------------------------------------*/
//uint8_t pColorData[960];					/* һ�����ɫ���ݻ��� 320 * 3 = 960 */
uint8_t pColorData[720];					/* һ�����ɫ���ݻ��� 240 * 3 = 720 */
FIL file;
extern FRESULT f_res;
UINT f_num;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ӡBMP�ļ���ͷ��Ϣ�����ڵ���
  * �������: pBmpHead:BMP�ļ���ͷ��Ϣ
  * �� �� ֵ: ��
  * ˵    ������
  */
static void showBmpHeader(BMP_FileHeader *pBmpHead)
{
    BMP_DEBUG_PRINTF("λͼ�ļ�ͷ:\n");
    BMP_DEBUG_PRINTF("�ļ�����:%d\n", (*pBmpHead).bfType);
    BMP_DEBUG_PRINTF("�ļ���С:%d\n", (*pBmpHead).bfSize);
    BMP_DEBUG_PRINTF("������:%d\n", (*pBmpHead).bfReserved1);
    BMP_DEBUG_PRINTF("������:%d\n", (*pBmpHead).bfReserved2);
    BMP_DEBUG_PRINTF("ʵ��λͼ���ݵ�ƫ���ֽ���:%d\n", (*pBmpHead).bfOffBits);
    BMP_DEBUG_PRINTF("\n");
}

/**
  * ��������: ��ӡBMP�ļ���ͷ��Ϣ�����ڵ���
  * �������: pBmpHead:BMP�ļ���ͷ��Ϣ
  * �� �� ֵ: ��
  * ˵    ������
  */
static void showBmpInforHeader(BMP_InfoHeader *pBmpInforHead)
{
    BMP_DEBUG_PRINTF("λͼ��Ϣͷ:\n");
    BMP_DEBUG_PRINTF("�ṹ��ĳ���:%d\n", (*pBmpInforHead).biSize);
    BMP_DEBUG_PRINTF("λͼ��:%d\n", (*pBmpInforHead).biWidth);
    BMP_DEBUG_PRINTF("λͼ��:%d\n", (*pBmpInforHead).biHeight);
    BMP_DEBUG_PRINTF("biPlanesƽ����:%d\n", (*pBmpInforHead).biPlanes);
    BMP_DEBUG_PRINTF("biBitCount������ɫλ��:%d\n", (*pBmpInforHead).biBitCount);
    BMP_DEBUG_PRINTF("ѹ����ʽ:%d\n", (*pBmpInforHead).biCompression);
    BMP_DEBUG_PRINTF("biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\n", (*pBmpInforHead).biSizeImage);
    BMP_DEBUG_PRINTF("X����ֱ���:%d\n", (*pBmpInforHead).biXPelsPerMeter);
    BMP_DEBUG_PRINTF("Y����ֱ���:%d\n", (*pBmpInforHead).biYPelsPerMeter);
    BMP_DEBUG_PRINTF("ʹ�õ���ɫ��:%d\n", (*pBmpInforHead).biClrUsed);
    BMP_DEBUG_PRINTF("��Ҫ��ɫ��:%d\n", (*pBmpInforHead).biClrImportant);
    BMP_DEBUG_PRINTF("\n");
}

/**
  * ��������: ��ʾbmpͼƬ, 24λ���ɫ
  * �������: x:��ʾͼƬ���Ͻ�x������
  *           y:��ʾͼƬ���Ͻ�y������
  *           pic_name:��ʾͼƬ�ļ�����
  * �� �� ֵ: ��
  * ˵    ����ͼƬ��Ⱥ͸߶ȸ���ͼƬ��С����
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

        /* ��ȡ�ļ�ͷ��Ϣ  �����ֽ�*/
        f_res = f_read(&file, &FileHeader, sizeof(BMP_FileHeader), &f_num);

        /* �ж��ǲ���bmp�ļ� "BM"*/
        if(FileHeader.bfType != 0x4d42)
        {
            BMP_DEBUG_PRINTF("file is not .bmp file!\r\n");
            return;
        }
        else
        {
            BMP_DEBUG_PRINTF("Ok this is .bmp file\r\n");
        }

        /* ��ȡBMP�ļ�ͷ��Ϣ*/
        showBmpHeader(&FileHeader);

        /* ��ȡλͼ��Ϣͷ��Ϣ */
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

    /* ����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���	*/
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
            /* ��һ��ͼƬ��С�Ĵ���*/
            LCD_OpenWindow(x, y + height - i - 1, width, 1);
            LCD_Write_Cmd(0x2C);
            /* ��ȡһ��bmp�����ݵ�����pColorData���� */
            f_read(&file, pColorData, l_width, &f_num);

            for(j = 0; j < width; j++) //һ����Ч��Ϣ
            {
                k = j * 3;	//һ���е�K�����ص����
                LCD_Write_2Byte(RGB24TORGB16(pColorData[k + 2], pColorData[k + 1], pColorData[k]));
            }
        }
    }

    f_close(&file);
    tos_knl_sched_unlock();
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

