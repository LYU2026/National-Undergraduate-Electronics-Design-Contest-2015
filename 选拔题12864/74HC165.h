//#include<reg52.h> 
#include<intrins.h> 

sbit CLK_165=P2^3;
sbit QH_165=P2^4;
sbit SL_165=P2^5;

//unsigned char dat165;		  


unsigned char hc74_165();
void Delay165(unsigned int);	//ÑÓÊ±º¯Êý
void delay1ms();

