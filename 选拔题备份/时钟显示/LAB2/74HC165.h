//#include<reg52.h> 
#include<intrins.h> 

sbit CLK_165=P3^6;
sbit QH_165=P1^6;
sbit SL_165=P21^7;

unsigned char dat;		  


unsigned char hc74_165();
void Delay_165(unsigned int);	//ÑÓÊ±º¯Êý
void delay1_165ms();

