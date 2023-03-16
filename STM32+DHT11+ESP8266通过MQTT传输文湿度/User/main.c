#include <stdio.h> 
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "DHT11.h"
#include "usart.h"	
#include "esp8266.h"	  
#include "onenet.h"
#include "beep.h"

void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//?????????

	Delay_Init();									//systick???
	
	Usart1_Init(115200);							//??1,?????
	
	Usart2_Init(115200);							//??2,??ESP8266?
	
	Beep_Init();	
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}


int main(void)
{

	u8 wd=0;      
        u8 sd=0;
	char PUB_BUF[256];//�ϴ�����
	//const char *devSubTipic[] = {"tests"};
	const char devPubTipic[] = "test";
	unsigned short timeCount = 0;	//���ͼ������
	//unsigned char DIS_BUF[18];
	unsigned char *dataPtr = NULL;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Hardware_Init();
	DHT11_Init();
        ESP8266_Init();
	OLED_Init();

	
	while(OneNet_DevLink())			//??OneNET
	DelayXms(500);
	
	Beep_Set(BEEP_OFF);				//????????
	DelayXms(250);
	Beep_Set(BEEP_ON);
		
	
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
        OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	while(1)
	{
		if(timeCount % 40 == 0)	
		{	
		DHT11_Read_Data(&wd,&sd);//��ȡ��ʪ��ֵ    
                UsartPrintf(USART1, "�¶�: %d ��,ʪ��:%d%%\r\n",wd,sd);

		OLED_ShowChinese(0,0,0,16,1); //�� x,y
		OLED_ShowChinese(18,0,1,16,1);//��
		OLED_ShowNum(50,0,wd,2,16,1);     //��ʾ�¶�ֵ
		OLED_ShowString(36,0,":",16,1);

		OLED_ShowChinese(0,16,2,16,1);//ʪ
		OLED_ShowChinese(18,16,1,16,1);//��	
		OLED_ShowNum(50,16,sd,2,16,1);    //��ʾʪ��ֵ
		OLED_ShowString(36,16,":",16,1);  
		OLED_ShowString(68,16,"%",16,1);   //
		//OLED_ShowChinese(0,32,6,16,1);//��
		//OLED_ShowChinese(18,32,7,16,1);//��	
		//OLED_ShowNum(50,32,sd,2,16,1);    //��ʾʪ��ֵ
		//OLED_ShowString(36,32,":",16,1);  
	
		}
			if(++timeCount >= 100)									//????5s
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			
		//sprintf (PUB_BUF ,"{"name":"STM32","Temp":"%d","Humi":"%d"}",wd,sd);
			sprintf (PUB_BUF ,"{\"name\":\"STM32\",\"Temp\":\"%d\",\"Humi\":\"%d\"}",wd,sd);
			OneNet_Publish(devPubTipic, PUB_BUF);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		
		delay_ms(10);

		OLED_Refresh();

	}
}

