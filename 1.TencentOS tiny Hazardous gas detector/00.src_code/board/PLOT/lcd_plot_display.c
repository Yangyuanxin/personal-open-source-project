#include "lcd_plot_display.h"
#include "spi.h"
#include "lcd_spi2_drv.h"

plot_data_handler plot_handler ;
/*
cur_data:当前要显示的曲线数据包
cur_data_size:当前要显示的曲线数据包的大小
*/
void LCD_Plot_Remap(uint16_t *cur_data, uint16_t *backup_data, uint16_t cur_data_size)
{
		uint32_t i = 0 ;
		float temp = 0;
		/*数据包最大值*/
    uint16_t max = 0;
		/*数据包最小值*/
    uint16_t min = 0;
		float scale = 0.0;
		uint16_t value = 0;
		float max_min_diff = 0.0;
		/*曲线显示的高度*/
		float height = PLOT_DISPLAY_AREA_Y;
		char display_rel_buf[20] = {0};
    char display_max_buf[20] = {0};
		char display_min_buf[20] = {0};
		char display_sub_buf[20] = {0};
		/*显示X坐标轴*/
		for(uint8_t i = PLOT_DISPLAY_AREA_X-1 ; i < 240 ; i++)
        LCD_Draw_ColorPoint(i, 239, RED);
		/*显示Y坐标轴*/
    for(uint8_t i = LCD_Y-PLOT_DISPLAY_AREA_Y ; i < 240 ; i++)
        LCD_Draw_ColorPoint(PLOT_DISPLAY_AREA_X-1, i, RED);

		value = 0 ;
		for(i = 0; i < cur_data_size; i++)
        if(cur_data[i] > value)
            value = cur_data[i];
		max = value ;
		value = cur_data[0];
		for(i = 0; i < cur_data_size; i++)
        if(cur_data[i] < value)
            value = cur_data[i];
		min = value ;
		
		sprintf(display_rel_buf,"%04d",cur_data[DATA_SIZE-1]);
		sprintf(display_max_buf,"%04d",max);
		sprintf(display_min_buf,"%04d",min);
		sprintf(display_sub_buf,"%04d",max-min);
				
		//void LCD_ShowCharStr(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size);
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+10,LCD_X,"rel:",BLACK,WHITE,16);
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+20+10,LCD_X,display_rel_buf,BLACK,WHITE,16);
				
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+50+10,LCD_X,"max:",BLACK,WHITE,16);
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+70+10,LCD_X,display_rel_buf,BLACK,WHITE,16);
				
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+100+10,LCD_X,"min:",BLACK,WHITE,16);
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+120+10,LCD_X,display_rel_buf,BLACK,WHITE,16);
				
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+150+10,LCD_X,"sub:",BLACK,WHITE,16);
		LCD_ShowCharStr(5,LCD_Y-PLOT_DISPLAY_AREA_Y+170+10,LCD_X,display_rel_buf,BLACK,WHITE,16);

    if(min > max) 
			return ;

    max_min_diff = (float)(max - min);
    scale = (float)(max_min_diff / height);

    if(cur_data_size < DATA_SIZE) 
			return;

    for(i = 0; i < DATA_SIZE; i ++)
    {
        temp = cur_data[i] - min;
        backup_data[i] =  DATA_SIZE - (uint16_t)(temp / scale) - 1;
    }
}

/*显示曲线*/
void LCD_Plot_Display(uint16_t *pData, uint32_t size, uint16_t color)
{
    uint32_t i, j;
    uint8_t color_L = (uint8_t) color;
    uint8_t color_H = (uint8_t) (color >> 8);
		uint32_t x1 = 0, x2 = 0;
    if(size < DATA_SIZE)	return ;

    for (i = PLOT_DISPLAY_AREA_X; i < DATA_SIZE - 1; i++)
    {
        if (pData[i + 1] >= pData[i])
        {
						x1 = i;
						x2 = x1  ;
						LCD_Address_Set(x1, pData[i], x2, pData[i + 1]);
            LCD_WR_RS(1);

            for (j = pData[i]; j <= pData[i + 1]; j++)
            {
                *((uint8_t*) &hspi2.Instance->DR) = color_H;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);

                *((uint8_t*) &hspi2.Instance->DR) = color_L;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);
            }
        }
        else
        {
						x1 = i ;
						x2 = x1  ;
						LCD_Address_Set(x1, pData[i + 1], x2, pData[i]);
            LCD_WR_RS(1);

            for (j = pData[i + 1]; j <= pData[i]; j++)
            {
                *((uint8_t*) &hspi2.Instance->DR) = color_H;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);

                *((uint8_t*) &hspi2.Instance->DR) = color_L;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);
            }
        }
    }
}

