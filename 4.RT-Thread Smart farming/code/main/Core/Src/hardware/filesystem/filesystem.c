#include "filesystem.h"
#include "fatfs.h"
#include "rtc.h"

uint8_t work[_MAX_SS] ;
Record_Data csv_file_record ;

/*����SD��*/
int Mount_SD(void)
{
    /*����SD��*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    return 0 ;
}

/*�ָ���������*/
void Recovery_Setting(void)
{
    FRESULT status ;
    status = f_unlink(SETTING_PARA);

    if(FR_OK != status)
        DEBUG("ɾ�������ļ�ʧ��\n");
    else
        DEBUG("ɾ�������ļ��ɹ�\n");

    for(int i = 0 ; i <= csv_file_record.file_number_index ; i++)
    {
        status = f_unlink(SETTING_PARA);

        if(FR_OK != status)
            DEBUG("ɾ����¼�ļ�%dʧ��\n", csv_file_record.file_number_index);
        else
            DEBUG("ɾ����¼�ļ�%d�ɹ�\n", csv_file_record.file_number_index);
    }
}


/*��ȡ�ļ����һ������*/
int ReadlLineData(int File_Count, int line_number)
{
    /*ָ��Ҫ��ȡ��һ��*/
    int WhichLine ;
    /*��ǰ��ȡ����*/
    int CurrentIndex = 0;
    char *StrLine = NULL ;
    char *ptr = NULL ;
    int data_serial_num_file_count ;
    char filename[FILE_NAME_LEN] = {0};
    WhichLine = line_number - 1;
    StrLine = malloc(100) ;
    assert_param(NULL != strLine);
    /*���㵱ǰ����ˮ������Ӧ���ļ�*/
    data_serial_num_file_count = File_Count ;
    sprintf(filename, "0:BearPi_Log%d.csv", data_serial_num_file_count);
    retSD = f_open(&SDFile, filename, FA_OPEN_EXISTING | FA_READ);

    if(FR_OK != retSD)
    {
        DEBUG("��%s�ļ�ʧ�ܣ�err = %d\r\n", filename, retSD);
        free(StrLine);
        return -2;
    }

    while(!f_eof(&SDFile))
    {
        if(CurrentIndex == WhichLine)
        {
            //��ȡһ��
            f_gets(StrLine, 100, &SDFile);
            DEBUG("��ǰ����:%s\n", StrLine);
            //Para1:��ȡ��ˮ��
            csv_file_record.serial_number = atoi(StrLine);
            //Para2:��ȡ��
            ptr = strstr(StrLine, ",");
            csv_file_record.year = atoi(ptr + 1);
            //Para3:��ȡ��
            ptr = strstr(ptr + 1, "/");
            csv_file_record.month = atoi(ptr + 1);
            //Para4:��ȡ��
            ptr = strstr(ptr + 1, "/");
            csv_file_record.day = atoi(ptr + 1);
            //Para5:��ȡʱ
            ptr = strstr(ptr + 1, " ");
            csv_file_record.hour = atoi(ptr + 1);
            //Para6:��ȡ��
            ptr = strstr(ptr + 1, ":");
            csv_file_record.minute = atoi(ptr + 1);
            //Para7:��ȡ�����===>Ϊ0���ǰ�ȫ��Ϊ1����Σ��
            ptr = strstr(ptr + 1, ",");
            csv_file_record.detect_result = atoi(ptr + 1);
        }

        //��ȡһ��,����λ����һ��
        f_gets(StrLine, 100, &SDFile);
        CurrentIndex++;
    }

    f_close(&SDFile);
    free(StrLine);
    return CurrentIndex ;
}

/*��ȡ���ڴ�*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);//�õ�������Ϣ�����д�����

    if(res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize; //�õ���������
        fre_sect = fre_clust * fs1->csize;        //�õ�����������
        #if _MAX_SS!=512                                  //������С����512�ֽ�,��ת��Ϊ512�ֽ�
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
        #endif
        *total = tot_sect >> 1; //��λΪKB
        *free = fre_sect >> 1; //��λΪKB
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
		uint32_t Total = 0; //��ȡFLASH������
    uint32_t Free = 0; //��ȡFLASHʣ������
		f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //��ȡSD����������ʣ������
		sprintf(Stroge_Overs_Size,"%04dMB",(int)(float)Free / 1024);
		LCD_Ascii_Show_Para display_stroge_para ;
		display_stroge_para.x = 165+24 ;
		display_stroge_para.y = 6 ;
		display_stroge_para.max_width = 50 ;
		display_stroge_para.sizey = 16 ;
		display_stroge_para.bc = WHITE ;
		display_stroge_para.str = Stroge_Overs_Size ;
		/*���ʣ���ڴ�С��1G������ʾ��ɫ*/
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
					DEBUG("��/����%s�ļ�ʧ�ܣ�err = %d\r\n", FILE_NAME, retSD);
					return -1 ;
			}
			/*ÿһ�ζ�ƫ�Ƶ��ļ���ĩβ*/
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
					DEBUG("f_write ��������err = %d\r\n", retSD);
					DEBUG("�رմ򿪵�BearPi_Log.csv�ļ�\r\n");
					f_close(&SDFile);
					return -2 ;
			}
			//printf("д��¼%lld�ɹ�!==>FLASH������:%0.2fMB ʣ��:%0.2fMB,д:%lld==>%lldKB\n",count_log,(float)Total / 1024,(float)Free / 1024,file_size, file_size / 1024);
			f_sync(&SDFile);
			f_close(&SDFile);
			count_log++ ;
		}
    return 0 ;
}



