#include "filesystem.h"
#include "fatfs.h"
#include "rtc.h"

uint8_t work[_MAX_SS] ;
Record_Data csv_file_record ;

/*挂载SD卡*/
int Mount_SD(void)
{
    /*挂载SD卡*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    return 0 ;
}

/*恢复出厂设置*/
void Recovery_Setting(void)
{
    FRESULT status ;
    status = f_unlink(SETTING_PARA);

    if(FR_OK != status)
        DEBUG("删除配置文件失败\n");
    else
        DEBUG("删除配置文件成功\n");

    for(int i = 0 ; i <= csv_file_record.file_number_index ; i++)
    {
        status = f_unlink(SETTING_PARA);

        if(FR_OK != status)
            DEBUG("删除记录文件%d失败\n", csv_file_record.file_number_index);
        else
            DEBUG("删除记录文件%d成功\n", csv_file_record.file_number_index);
    }
}


/*读取文件里的一行数据*/
int ReadlLineData(int File_Count, int line_number)
{
    /*指定要读取哪一行*/
    int WhichLine ;
    /*当前读取的行*/
    int CurrentIndex = 0;
    char *StrLine = NULL ;
    char *ptr = NULL ;
    int data_serial_num_file_count ;
    char filename[FILE_NAME_LEN] = {0};
    WhichLine = line_number - 1;
    StrLine = malloc(100) ;
    assert_param(NULL != strLine);
    /*计算当前的流水号所对应的文件*/
    data_serial_num_file_count = File_Count ;
    sprintf(filename, "0:BearPi_Log%d.csv", data_serial_num_file_count);
    retSD = f_open(&SDFile, filename, FA_OPEN_EXISTING | FA_READ);

    if(FR_OK != retSD)
    {
        DEBUG("打开%s文件失败，err = %d\r\n", filename, retSD);
        free(StrLine);
        return -2;
    }

    while(!f_eof(&SDFile))
    {
        if(CurrentIndex == WhichLine)
        {
            //读取一行
            f_gets(StrLine, 100, &SDFile);
            DEBUG("当前数据:%s\n", StrLine);
            //Para1:获取流水号
            csv_file_record.serial_number = atoi(StrLine);
            //Para2:获取年
            ptr = strstr(StrLine, ",");
            csv_file_record.year = atoi(ptr + 1);
            //Para3:获取月
            ptr = strstr(ptr + 1, "/");
            csv_file_record.month = atoi(ptr + 1);
            //Para4:获取日
            ptr = strstr(ptr + 1, "/");
            csv_file_record.day = atoi(ptr + 1);
            //Para5:获取时
            ptr = strstr(ptr + 1, " ");
            csv_file_record.hour = atoi(ptr + 1);
            //Para6:获取分
            ptr = strstr(ptr + 1, ":");
            csv_file_record.minute = atoi(ptr + 1);
            //Para7:获取检测结果===>为0则是安全，为1则是危险
            ptr = strstr(ptr + 1, ",");
            csv_file_record.detect_result = atoi(ptr + 1);
        }

        //读取一行,并定位到下一行
        f_gets(StrLine, 100, &SDFile);
        CurrentIndex++;
    }

    f_close(&SDFile);
    free(StrLine);
    return CurrentIndex ;
}

/*获取盘内存*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);//得到磁盘信息及空闲簇数量

    if(res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize; //得到总扇区数
        fre_sect = fre_clust * fs1->csize;        //得到空闲扇区数
        #if _MAX_SS!=512                                  //扇区大小不是512字节,则转换为512字节
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
        #endif
        *total = tot_sect >> 1; //单位为KB
        *free = fre_sect >> 1; //单位为KB
    }
    return res;
}




int save_record_to_flash(void)
{
    uint32_t count = 0  ;
    char Detect_Data[100] = {0} ;
		long long int file_size = 0;
		char Stroge_Overs_Size[20] = {0};
		static long long int count_log = 0 ;
		uint32_t Total = 0; //读取FLASH总容量
    uint32_t Free = 0; //读取FLASH剩余容量
		f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //获取SD卡总容量和剩余容量
		sprintf(Stroge_Overs_Size,"%04dMB",(int)(float)Free / 1024);
		LCD_Ascii_Show_Para display_stroge_para ;
		display_stroge_para.x = 165+24 ;
		display_stroge_para.y = 6 ;
		display_stroge_para.max_width = 50 ;
		display_stroge_para.sizey = 16 ;
		display_stroge_para.bc = WHITE ;
		display_stroge_para.str = Stroge_Overs_Size ;
		/*如果剩余内存小于1G，则显示红色*/
		if((float)Free / 1024 < 1024)
		{
			display_sd_card(1);
			display_stroge_para.fc = RED ;
			lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
		}
		else
		{
			display_sd_card(0);
			display_stroge_para.fc = BLACK ;
			lcd_model.lcd_driver->lcd_show_ascii_str(display_stroge_para);
			retSD = f_open(&SDFile, FILE_NAME, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK != retSD)
			{
					DEBUG("打开/创建%s文件失败，err = %d\r\n", FILE_NAME, retSD);
					return -1 ;
			}
			/*每一次都偏移到文件的末尾*/
			file_size = f_size(&SDFile) ;
			f_lseek(&SDFile,file_size);
			sprintf(Detect_Data, "%2d/%02d/%02d %02d:%02d,%.2f,%.2f,%d\r\n",		\
							DateTime_Handler_Info.year, 	\
							DateTime_Handler_Info.month, DateTime_Handler_Info.day, 						\
							DateTime_Handler_Info.hour, DateTime_Handler_Info.minute,					\
							E53_IA1_Sensor.temp, E53_IA1_Sensor.humidity, E53_IA1_Sensor.light_strength);
			retSD = f_write(&SDFile, Detect_Data, strlen((char *)Detect_Data), &count);

			if(retSD != FR_OK)
			{
					DEBUG("f_write 发生错误，err = %d\r\n", retSD);
					DEBUG("关闭打开的BearPi_Log.csv文件\r\n");
					f_close(&SDFile);
					return -2 ;
			}
			//printf("写记录%lld成功!==>FLASH总容量:%0.2fMB 剩余:%0.2fMB,写:%lld==>%lldKB\n",count_log,(float)Total / 1024,(float)Free / 1024,file_size, file_size / 1024);
			f_sync(&SDFile);
			f_close(&SDFile);
			count_log++ ;
		}
    return 0 ;
}



