#include "cmd_forward.h"
#include "SystemConfig.h"

char *CmdForward(uint8_t *Cmd, uint8_t lenth, void(*CallBack)(uint8_t *,uint8_t))
{
	if(NULL == Cmd || NULL == CallBack)
		return NULL;
	CallBack(Cmd,lenth);
	return "OK";
}

void makeAndSendCmd(int cmdType, uint16_t *args) //�������ת���ɱ�׼����Э���ʽȻ����
{
	uint8_t lenth;
	uint8_t dataSend[50];
	memset(dataSend, 0, sizeof(dataSend));
	dataSend[0] = 0x55;
	dataSend[1] = 0x55;
	dataSend[3] = (uint8_t) cmdType;

	int i = 0;

	while (i <= MAX_ARGS_LENTH && args[i] != UNDEFINECMD)
	{
		dataSend[4 + i] = (uint8_t)args[i];
		i++;
	}
	lenth = (uint8_t) (i + 2);
	dataSend[2] = lenth; //���볤����Ϣ
	CmdForward(dataSend,lenth+2,WriteData);
}

void sendAngleCmd(uint8_t id, uint16_t value) //�����ϵ���������ĽǶȱ仯��������
{
	uint16_t dataSend[MAX_ARGS_LENTH];
	memset(dataSend, 0, sizeof(dataSend));
	for (int i = 0; i < MAX_ARGS_LENTH; i++)
		dataSend[i] = UNDEFINECMD;
	dataSend[0] = 1;
	dataSend[1] = 0;
	dataSend[2] = 0;
	dataSend[3] = (uint8_t) id;
	dataSend[4] = (uint8_t) (value & 0x00ff);
	dataSend[5] = (uint8_t) (value >> 8);
	makeAndSendCmd(CMD_MULT_SERVO_MOVE, dataSend);
}

void sendActionCmd(uint16_t *args) //����һ֡��������
{
	uint16_t dataSend[MAX_ARGS_LENTH];
	memset(dataSend, 0, sizeof(dataSend));
	for (int i = 0; i < MAX_ARGS_LENTH; i++)
		dataSend[i] = UNDEFINECMD;
	dataSend[0] = (uint16_t) SERVO_NUM;
	dataSend[1] = (uint16_t) (args[0] & 0x00ff); //����ʱ��
	dataSend[2] = (uint16_t) (args[0] >> 8);
	for (int i = 0; i < SERVO_NUM; i++)
	{
		dataSend[3 + i * 3] = (uint16_t) (i + 1);
		dataSend[4 + i * 3] = (uint16_t) (args[i + 1] & 0x00ff);
		dataSend[5 + i * 3] = (uint16_t) (args[i + 1] >> 8);
	}
	makeAndSendCmd(CMD_MULT_SERVO_MOVE, dataSend);
}

//�������
void resetServo(void)
{
	uint16_t data[7];
	memset(data, 0, sizeof(data));
	//�̶��ĸ�λ����ٶ�
	data[0] = 1000;
	for (int i = 0; i < SERVO_NUM; i++)
	{
		data[i + 1] = 1500;
	}
	sendActionCmd(data);
}
