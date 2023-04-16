#include "As608.h"



__IO uint8_t As608GetId;

static void As608SendCommand(uint8_t Cmd[], uint8_t CmdLen)
{
    HAL_UART_Transmit(&huart2, Cmd, CmdLen, 1000);
}

/*����У��*/
void As608SendAuthCheck(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[16] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x07;
    //ָ����
    Command[9] = 0x13;
    //����
    Command[10] = 0x00;
    Command[11] = 0x00;
    Command[12] = 0x00;
    Command[13] = 0x00;
    
    for(Index = 6; Index < 14; Index++)
        CheckSum += Command[Index];

    Command[14] = CheckSum >> 8;
    Command[15] = CheckSum & 0xFF;

    As608SendCommand(Command, AS608_NR(Command));
}

/*¼��ͼ������*/
void As608GetImage(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[12] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x03;
    //ָ����
    Command[9] = 0x01;
    
    for(Index = 6; Index < 10; Index++)
        CheckSum += Command[Index];

    Command[10] = CheckSum >> 8;
    Command[11] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*��ͼ��ת��������������Buffer1��*/
void As608Image2Buffer1(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[13] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x04;
    //ָ����
    Command[9] = 0x02;
    //��������
    Command[10] = 0x01;
    
    for(Index = 6; Index < 11; Index++)
        CheckSum += Command[Index];

    Command[11] = CheckSum >> 8;
    Command[12] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*��ͼ��ת��������������Buffer2��*/
void As608Image2Buffer2(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[13] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x04;
    //ָ����
    Command[9] = 0x02;
    //��������
    Command[10] = 0x02;
    
    for(Index = 6; Index < 11; Index++)
        CheckSum += Command[Index];

    Command[11] = CheckSum >> 8;
    Command[12] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*����ָ����Ϣ*/
void As608SerachFingerInfo(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[17] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x08;
    //ָ����
    Command[9] = 0x04;
    //��������
    Command[10] = 0x01;
    //����->��ʼҳ
    Command[11] = 0x00;
    Command[12] = 0x00;
    //����->ҳ��
    Command[13] = 0x03;
    Command[14] = 0xE7;
    for(Index = 6; Index < 11; Index++)
        CheckSum += Command[Index];

    Command[15] = CheckSum >> 8;
    Command[16] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*�ϳ�����ģ��*/
void As608MergeSpecTemplate(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[12] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x03;
    //ָ����
    Command[9] = 0x05;
    
    for(Index = 6; Index < 10; Index++)
        CheckSum += Command[Index];

    Command[10] = CheckSum >> 8;
    Command[11] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*���ָ�ƿ�*/
void As608DelAllFingerTemplate(void)
{
    uint8_t Index = 0;
    uint16_t CheckSum = 0;
    uint8_t Command[12] = {0};
    
    //��ͷ
    Command[0] = PACKET_HEADER_HIGH;
    Command[1] = PACKET_HEADER_LOW;
    //оƬ��ַ
    Command[2] = CHIP_ADDRESS_0;
    Command[3] = CHIP_ADDRESS_1;
    Command[4] = CHIP_ADDRESS_2;
    Command[5] = CHIP_ADDRESS_3;
    //����ʶ
    Command[6] = COMMAND_PACKET_MARK;
    //������
    Command[7] = 0x00;
    Command[8] = 0x03;
    //ָ����
    Command[9] = 0x0D;
    
    for(Index = 6; Index < 10; Index++)
        CheckSum += Command[Index];

    Command[10] = CheckSum >> 8;
    Command[11] = CheckSum & 0xFF;
    As608SendCommand(Command, AS608_NR(Command));
}

/*����ָ��*/
void As608SaveFingerInfo(uint32_t Id)
{
    #if 0
    uint8_t Index = 0;
    uint32_t CheckSum = 0;
    uint8_t As608SaveFinger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};
    As608SaveFinger[5] = (Id & 0xFF00) >> 8;
    As608SaveFinger[6] = (Id & 0x00FF);
    for(Index = 0; Index < 7; Index++)
        CheckSum += As608SaveFinger[Index];
    As608SaveFinger[7] = (CheckSum & 0x00FF00) >> 8;
    As608SaveFinger[8] = (CheckSum & 0x0000FF);
    As608SendPacketHeader();
    As608SendCommand(As608SaveFinger, AS608_NR(As608SaveFinger));
    #endif
}

As608Dri As608CmdArr[] = 
{   
    {AS608_CMD_AUTH_CHECK,              As608SendAuthCheck},
    {AS608_CMD_GET_IMAGE,               As608GetImage},
    {AS608_CMD_SAVE_IMAGE_2_BUF1,       As608Image2Buffer1},
    {AS608_CMD_SAVE_IMAGE_2_BUF2,       As608Image2Buffer2},
    {AS608_CMD_SEARCH_CODE,             As608SerachFingerInfo},
    {AS608_CMD_BUF1_BUF2_SPEC_MERGE,    As608MergeSpecTemplate},
    {AS608_CMD_DEL_ALL_TEMPLATE,        As608DelAllFingerTemplate}
};


void As608ModuleInit(void)
{
    As608GetId = AS608_CMD_AUTH_CHECK;
    As608CmdArr[As608GetId].CmdFunc();
}

