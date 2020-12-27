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
				DEBUG("删除记录文件%d失败\n",csv_file_record.file_number_index);	
			else
				DEBUG("删除记录文件%d成功\n",csv_file_record.file_number_index);
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


/*保存检测数据到csv文件中*/
int save_record_to_flash(void)
{
		#if 0
    uint32_t count = 0  ;
    char filename[FILE_NAME_LEN] = {0};
    char Detect_Data[100] = {0} ;
    /*计算当前流水号一个可以产生多少个文件，一个文件超出100个记录项则自动创建新的文件*/
    int data_serial_num_file_count =  User_Memory_Para.detect_log_serial_number / DETECT_DATA_INDEX ;
    sprintf(filename, "0:BearPi_Log%d.csv", data_serial_num_file_count);
    retSD = f_open(&SDFile, filename, FA_OPEN_ALWAYS | FA_WRITE);

    if(FR_OK != retSD)
    {
        DEBUG("打开/创建%s文件失败，err = %d\r\n", filename, retSD);
        return -1 ;
    }

    printf("打开/创建%s文件成功，向文件写入数据。\r\n", filename);
    /*每一次都偏移到文件的末尾*/
    f_lseek(&SDFile, f_size(&SDFile));
    /*流水号自增*/
    ++User_Memory_Para.detect_log_serial_number;
    /*记录流水号保存处理*/
    User_Detect_Log_Save_Process();
    sprintf(Detect_Data, "%d,%2d/%02d/%02d %02d:%02d,%d\r\n",		\
            User_Memory_Para.detect_log_serial_number, DateTime_Handler_Info.year, 	\
            DateTime_Handler_Info.month, DateTime_Handler_Info.day, 						\
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute,					\
            Sensor_Flow_Cursor.Is_safety_or_danger);
    retSD = f_write(&SDFile, Detect_Data, strlen((char *)Detect_Data), &count);

    if(retSD != FR_OK)
    {
        DEBUG("f_write 发生错误，err = %d\r\n", retSD);
        DEBUG("关闭打开的BearPi_Log.csv文件\r\n");
        count = 0;
        memset(Detect_Data, 0, DETECT_DATA_LEN);
        f_close(&SDFile);
        return -2 ;
    }

    DEBUG("文件写入成功，写入字节数据：%d\n", count);
    DEBUG("向文件写入的数据为：\r\n%s\r\n", Detect_Data);
    DEBUG("关闭打开的BearPi_Log.csv文件\r\n");
    count = 0;
    memset(Detect_Data, 0, DETECT_DATA_LEN);
    f_close(&SDFile);
		#endif
    return 0 ;
}



