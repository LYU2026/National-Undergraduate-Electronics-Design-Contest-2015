#include<reg52.h>
#include "intrins.h"

#define FOSC 12000000L//1843200L
#define BAUD 4800//9600

typedef unsigned char BYTE;
typedef unsigned char WORD;

sfr     AUXR      =0x8e;
sfr   	ADC_CONTR =0xc5;
sfr     ADC_DATA	=0xc6;
sfr     ADC_LOW2  =0XBE;
sfr     P1M0      =0x91;
sfr     P1M1      =0x92;

#define  ADC_POWER    0x80
#define  ADC_FLAG     0x10
#define  ADC_START    0x08
#define  ADC_SPEEDLL  0x00
#define  ADC_SPEEDL   0x20
#define  ADC_SPEEDH   0x40
#define  ADC_SPEEDHH  0x60

void InitUart();
void SendData(BYTE dat);
void Delay(WORD n);
void InitADC();
BYTE ch=0;

void main()
{
	InitUart();
	InitADC();
	AUXR|=0x10;
	IE=0xa0;
	while(1);
}

void adc() interrupt 5 using 1
{
	ADC_CONTR&=!ADC_FLAG;
	SendData(ch);
	SendData(ADC_LOW2);
	if(++ch>7) ch=0;
	ADC_CONTR =ADC_POWER|ADC_SPEEDLL|ADC_START|ch;
}

void InitADC()
{
	P1=P1M0=P1M1=0xff;
	ADC_DATA=0;
	ADC_CONTR= ADC_POWER|ADC_SPEEDLL|ADC_START|ch;
	Delay(2);
}

void InitUart()
{
	SCON=0x5a;
	TMOD=0x20;
	TH1=TL1=256-(FOSC/12/32/BAUD);
	TR1=1;
}


void SendData(BYTE dat)
{
	while(!TI);
	TI=0;
	SBUF=dat;
}

void Delay(WORD n)
{
	WORD x;
	while(n--)
	{
		x=5000;
		while(x--);
	}
}