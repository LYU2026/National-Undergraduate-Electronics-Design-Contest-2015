#include<reg51.h>
sbit DQ = P2^3;	//�¶ȴ���������




void Delay_us(unsigned int uiUs);	//us��ʱ����,12MHZ������Ч
void Delay_ms(unsigned int uiMs);	//ms��ʱ����,12MHZ������Ч

void DS18B20_Init(void);   //DS18B20��ʼ��
unsigned char DS18B20_ReadOneChar(void);   //��ȡһ������
void DS18B20_WriteOneChar(unsigned char dat);	  //д��һ������
unsigned int DS18B20_ReadTemperature();		