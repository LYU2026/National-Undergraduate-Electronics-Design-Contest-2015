#include<reg52.h> 
#include<intrins.h> 

sbit CLK_165=P3^6;
sbit QH_165=P1^6;
sbit SL_165=P1^7;

//unsigned char dat;		  


unsigned char hc74_165();
void Delay(unsigned int);	//��ʱ����
void delay1ms();

