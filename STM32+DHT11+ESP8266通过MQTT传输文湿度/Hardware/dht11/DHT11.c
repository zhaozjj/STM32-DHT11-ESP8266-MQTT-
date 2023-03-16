#include "stm32f10x.h"
#include "DHT11.h"
void DHT11_IO_IN(void)//��ʪ��ģ�����뺯��
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = IO_DHT11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
}

void DHT11_IO_OUT(void)//��ʪ��ģ���������
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IO_DHT11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
}

//��λDHT11
void DHT11_Rst(void)
{
    DHT11_IO_OUT(); //SET OUTPUT
    DHT11_DQ_Low; //DQ=0
    DelayXms(20);    //��������18ms
    DHT11_DQ_High; //DQ=1
    DelayUs(30);     //��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void)
{
    u8 retry = 0; //������ʱ����
    DHT11_IO_IN();//SET INPUT
    while ((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1) && retry < 100) //DHT11������40~80us
    {
        retry++;
        DelayUs(1);
    };
    if(retry >= 100)return 1;
    else retry = 0;
    while ((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 0) && retry < 100) //DHT11���ͺ���ٴ�����40~80us
    {
        retry++;
        DelayUs(1);
    };
    if(retry >= 100)return 1;
    return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;
    while((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1) && retry < 100) //�ȴ���Ϊ�͵�ƽ
    {
        retry++;
        DelayUs(1);
    }
    retry = 0;
    while((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 0) && retry < 100) //�ȴ���ߵ�ƽ
    {
        retry++;
        DelayUs(1);
    }
    DelayUs(40);//�ȴ�40us
    if(GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11) == 1)
        return 1;
    else
        return 0;
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;
    DHT11_Rst();
    if(DHT11_Check() == 0)
    {
        for(i = 0; i < 5; i++) //��ȡ40λ����
        {
            buf[i] = DHT11_Read_Byte();
        }
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
    }
    else return 1;
    return 0;
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����
void DHT11_Init(void)
{
    DHT11_Rst();  //��λDHT11
    DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
}
