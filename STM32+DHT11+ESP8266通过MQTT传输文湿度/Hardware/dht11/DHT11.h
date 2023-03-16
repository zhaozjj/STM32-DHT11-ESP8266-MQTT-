#ifndef __DHT11_H
#define __DHT11_H
#include "delay.h"
#include "stm32f10x.h"

#define IO_DHT11 GPIO_Pin_12
#define GPIO_DHT11 GPIOB

#define DHT11_DQ_High GPIO_SetBits(GPIO_DHT11,IO_DHT11)
#define DHT11_DQ_Low  GPIO_ResetBits(GPIO_DHT11,IO_DHT11)

void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
void DHT11_Init(void);
u8   DHT11_Read_Data(u8 *temp,u8 *humi);
u8   DHT11_Read_Byte(void);
u8   DHT11_Read_Bit(void);
u8   DHT11_Check(void);
void DHT11_Rst(void);

#endif

