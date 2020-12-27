#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H
#include "main.h"

/*文件名大小限制*/
#define FILE_NAME_LEN 20
/*检测数据大小限制*/
#define DETECT_DATA_LEN 50
/*一个文件存储检测记录的条数*/
#define DETECT_DATA_INDEX 100

typedef struct
{
    int serial_number ; 		//序号
    int year ; 					//2019-
    uint8_t month ; 			//0-12
    uint8_t day ;				//0-31
    uint8_t hour ; 				//0-24
    uint8_t minute ;			//0-59
    uint8_t detect_result ;		//检测结果
    /*当前位于文件的哪一行*/
    int Current_Line;
    /*当前文件编号查询索引*/
    int file_number_index ;
    /*当前流水号，每次记录初始化的时候会赋值一次*/
    int Current_Serial_Number ;
} Record_Data;
extern Record_Data csv_file_record ;


/*挂载SD卡*/
int Mount_SD(void);
/*恢复出厂设置*/
void Recovery_Setting(void);
/*保存检测数据到csv文件中*/
int save_record_to_flash(void);
/*读取文件里的一行数据*/
int ReadlLineData(int File_Count, int line_number);


#endif //__FILESYSTEM_H

