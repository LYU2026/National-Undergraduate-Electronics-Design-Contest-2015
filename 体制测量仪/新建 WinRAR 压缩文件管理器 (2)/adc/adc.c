#include "uart.h"
#include "intrins.h"


typedef unsigned char BYTE;
typedef unsigned int WORD;

/*Declare SFR associated with the ADC */
sfr ADC_CONTR   =   0xC5;           //ADC control register
sfr ADC_DATA    =   0xC6;           //ADC high 8-bit result register
sfr ADC_LOW2    =   0xBE;           //ADC low 2-bit result register
sfr P1M0        =   0x91;           //P1 mode control register0
sfr P1M1        =   0x92;           //P1 mode control register1

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks
void InitADC();
float GetADCResult(BYTE ch);
void Delay(WORD n);
void ShowResult(BYTE ch);
void keyscan();
sbit key=P3^7;
unsigned char count=0;
unsigned int sum=0;
void main()
{

    InitUart(); //IT0=0;EX0=1;                    //Init UART, use to show ADC result
   InitADC();                      //Init ADC sfr
    while (1)
    {
		keyscan();
		if(count==1)
       		ShowResult(0);

    }
}

/*----------------------------
Send ADC result to UART
----------------------------*/
void ShowResult(BYTE ch)
{
	float v=0;int a,b,c,d;
	SendData('$');
	v=GetADCResult(ch);
	if(v>269)
	{
		sum+=v;
	}
	a=sum/1000;
	SendData('0'+a);
	b=(sum-a*1000)/100;
	b=(b>9?9:b);
	SendData('0'+b);
	c=(sum-a*1000-b*100)/10;
	c=(c>9?9:c);
	SendData('0'+c);
	d=(sum-a*1000-b*100-c*10);
	d=(d>9?9:d);
	SendData('0'+d);
/*
	int a,b,c,d;
	SendData('$');
	v=GetADCResult(ch);
	a=v/100;
	SendData('0'+a);
	SendData('.');
	b=(v-a*100)/10;
	b=(b>9?9:b);
	SendData('0'+b);
	c=(v-a*100-b*10);
	c=(c>9?9:c);
	SendData('0'+c);
	d=(v-a*100-b*10-c)*10;
	d=(d>9?9:d);
//	SendData('0'+d);*/
	
	
	
	
	
	
	
	
//	SendData('K');
	Delay(30);
//if you want show 10-bit result, uncomment next line
//    SendData(ADC_LOW2);             //Show ADC low 2-bit result
}

/*----------------------------
Get ADC result
----------------------------*/
float GetADCResult(BYTE ch)
{
	float v;
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	v=ADC_DATA*5.0000/2.5600	;
    return v;                //Return ADC result
}

/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC()
{
    P1 = P1M0 = P1M1 = 0xff;        //Set all P1 as Open-Drain mode
    ADC_DATA = 0;                   //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC power-on and delay
}

/*----------------------------
Software delay function
----------------------------*/
void Delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}
void keyscan()
{
     if(key==0)
   {
     do{}while(key==0);
     count++;
     if(count>1)
     count=0;
   }  
}