#include<Function.h>
#include<reg52.h>

#define  AT24C02 0xa0  //AT24C02 ��ַ

sbit    K1 = P1^0;	 //����
sbit    K2 = P1^1;	 //��ȡ
sbit    K3 = P1^2;	 //+����
sbit    K4 = P1^3;   //-����
void delay(unsigned int i);


//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

unsigned char Count1;
unsigned long D[17], LedOut[5],LedNumVal;
unsigned int idata USEC;
void  system_ini();
  