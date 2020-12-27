#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H
#include "main.h"

/*�ļ�����С����*/
#define FILE_NAME_LEN 20
/*������ݴ�С����*/
#define DETECT_DATA_LEN 50
/*һ���ļ��洢����¼������*/
#define DETECT_DATA_INDEX 100
#define FILE_NAME "0:BearPi_Log.csv"

typedef struct
{
    int serial_number ; 		//���
    int year ; 					//2019-
    uint8_t month ; 			//0-12
    uint8_t day ;				//0-31
    uint8_t hour ; 				//0-24
    uint8_t minute ;			//0-59
    uint8_t detect_result ;		//�����
    /*��ǰλ���ļ�����һ��*/
    int Current_Line;
    /*��ǰ�ļ���Ų�ѯ����*/
    int file_number_index ;
    /*��ǰ��ˮ�ţ�ÿ�μ�¼��ʼ����ʱ��ḳֵһ��*/
    int Current_Serial_Number ;
} Record_Data;
extern Record_Data csv_file_record ;


/*����SD��*/
int Mount_SD(void);
/*�ָ���������*/
void Recovery_Setting(void);
/*���������ݵ�csv�ļ���*/
int save_record_to_flash(void);
/*��ȡ�ļ����һ������*/
int ReadlLineData(int File_Count, int line_number);
/*��ȡ���ڴ�*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free);
#endif //__FILESYSTEM_H

