#ifndef __main_H__
#define __main_H__
void Get_Weight();
void Show_Weight();

void Show_Weight2();
void Get_Maopi();
sbit ADDO=P2^0;
sbit ADSK=P2^1;
unsigned char a,b,c,d;
unsigned int i=0;
unsigned long HX711_Buffer = 0,Weight_Shiwu = 0,Weight_Maopi = 0,sum=0,k=0; 
unsigned char receiveData,senddata,showdata[5];
unsigned long ReadCount(void)
{
		unsigned long Count;
		unsigned char i;
		ADDO=1;
		ADSK=0;
		Count=0;
		while(ADDO);
		for(i=0;i<24;i++)
		{
				ADSK=1;
				Count=Count<<1;
				ADSK=0;
				if(ADDO)Count++;
		}
		ADSK=1;
		Count=Count^0x800000;
		ADSK=0;
		return(Count);
}