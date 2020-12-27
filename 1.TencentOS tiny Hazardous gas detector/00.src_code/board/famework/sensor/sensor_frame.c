#include "sensor_frame.h"

Sensor_Cursor Sensor_Flow_Cursor ;

/*��׼����*/
void sensor_base_line_step(int adc);
/*���������*/
void sensor_detecting_step(int adc);
/*��ⰲȫ*/
void sensor_detect_safety(int adc);
/*���Σ��*/
void sensor_detect_danger(int adc);


/*��������������*/
static sensor_frame sensor_opStruct[] =
{
    {BASE_LINE_STEP, 	 sensor_base_line_step},
    {DETECTING_STEP, 	 sensor_detecting_step},
    {DETECT_SAFETY_STEP, sensor_detect_safety},
    {DETECT_DANGER_STEP, sensor_detect_danger},
};

/*���������̴���*/
int Sensor_Handler(int8_t op, int adc_value)
{
    assert(op >= sizeof(sensor_opStruct) / sizeof(sensor_opStruct[0]));
    assert(op < 0);
    sensor_opStruct[op].handler_func(adc_value);
    return 0 ;
}

/*����������ʼ��*/
void Sensor_Detect_Init(void)
{
    Sensor_Flow_Cursor.Count_AMI = 0 ;
    Sensor_Flow_Cursor.Count_Base = 0 ;
    Sensor_Flow_Cursor.Count_Alarm = 0 ;
    Sensor_Flow_Cursor.Test_Process = 0 ;
    Sensor_Flow_Cursor.Start_Detect = 1 ;
    Sensor_Flow_Cursor.Detect_Step = BASE_LINE_STEP ;
}


/*��׼����*/
void sensor_base_line_step(int adc)
{
    Sensor_Flow_Cursor.Count_AMI++ ;

    if(Sensor_Flow_Cursor.Count_AMI > 2)
        Sensor_Flow_Cursor.Count_AMI = 0 ;

    /*ˢ�¶���*/
    icon_reflash(Sensor_Flow_Cursor.Count_AMI);

    /*�ж��Ƿ������׼������ͨ������Ϊ��ֵ��1/2*/
    if(adc < User_Memory_Para.alarm_threshold[User_Memory_Para.sensivity] / 2)
    {
        display_smoke_value(adc, GREEN, 1);
        ++Sensor_Flow_Cursor.Count_Base ;
    }
    else
    {
        display_smoke_value(adc, RED, 1);
    }

    if(Sensor_Flow_Cursor.Count_Base > 10)
    {
        Sensor_Flow_Cursor.Count_Base = 0 ;
        /*���ػ�׼*/
        display_base(0);
        /*��ʾ���*/
        display_detect(1);
        /*��ʾ��������*/
        Display_Process_Bar_Frame(1);
        /*�л��������*/
        Sensor_Flow_Cursor.Detect_Step = DETECTING_STEP ;
    }
}

/*���������*/
void sensor_detecting_step(int adc)
{
    Sensor_Flow_Cursor.Count_AMI++ ;

    if(Sensor_Flow_Cursor.Count_AMI > 2)
        Sensor_Flow_Cursor.Count_AMI = 0 ;

    icon_reflash(Sensor_Flow_Cursor.Count_AMI);
    ++Sensor_Flow_Cursor.Test_Process ;

    /*���԰�ȫ*/
    if(Sensor_Flow_Cursor.Test_Process > 50 && adc < User_Memory_Para.alarm_threshold[User_Memory_Para.sensivity])
    {
        Sensor_Flow_Cursor.Count_AMI = 0 ;
        Sensor_Flow_Cursor.Test_Process = 0 ;

        Sensor_Flow_Cursor.Detect_Step = DETECT_SAFETY ;
        Display_Process_Bar(0, 0);
        display_smoke_value(adc, BLACK, 0);
        Sensor_Flow_Cursor.Alarm_Threshold = adc ;
        Sensor_Flow_Cursor.Is_safety_or_danger = 0 ;
        /*���ؼ��*/
        display_detect(0);
        /*��ʾ��ȫ*/
        display_safety(1);
        /*�������¼*/
        save_record_to_flash();
    }
    else
    {
        /*��ʾ������*/
        Display_Process_Bar(Sensor_Flow_Cursor.Test_Process, 1);

        if(adc < User_Memory_Para.alarm_threshold[User_Memory_Para.sensivity])
        {
            display_smoke_value(adc, GREEN, 1);
        }
        else
        {
            display_smoke_value(adc, RED, 1);
            Sensor_Flow_Cursor.Count_Alarm++ ;

            if(Sensor_Flow_Cursor.Count_Alarm > 5)
            {
                Sensor_Flow_Cursor.Detect_Step = DETECT_DANGER ;
                Sensor_Flow_Cursor.Count_Alarm = 0 ;
                Sensor_Flow_Cursor.Count_AMI = 0 ;
                Sensor_Flow_Cursor.Test_Process = 0 ;
                display_smoke_value(adc, BLACK, 0);
                Sensor_Flow_Cursor.Alarm_Threshold = adc ;
                Sensor_Flow_Cursor.Is_safety_or_danger = 1 ;
                Display_Process_Bar(0, 0);
                /*���ؼ��*/
                display_detect(0);
                /*��ʾΣ��*/
                display_danger(1);
                /*�������¼*/
                save_record_to_flash();
            }
        }
    }
}

/*��ⰲȫ*/
void sensor_detect_safety(int adc)
{
    Sensor_Flow_Cursor.Start_Detect = 0 ;

    if(Sensor_Flow_Cursor.Count_AMI == 0)
    {
        Sensor_Flow_Cursor.Count_AMI = 1 ;
        display_smoke_value(adc, GREEN, 1);
    }
}

/*���Σ��*/
void sensor_detect_danger(int adc)
{
    static uint8_t Refresh_Alarm = 0 ;
    Refresh_Alarm = !Refresh_Alarm ;
    mq2_sensor_interface.led_control(&mq2_sensor_interface, Refresh_Alarm);

    if(User_Memory_Para.alarm == 1)
        mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, Refresh_Alarm);

    if(Sensor_Flow_Cursor.Count_AMI == 0)
    {
        Sensor_Flow_Cursor.Count_AMI = 1 ;
        display_smoke_value(adc, RED, 1);
    }
}



