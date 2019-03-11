//#include<reg52.h>
#include"LCD1602.h"
#include<stc12c2052ad.h>
#define FOSC    12000000L
#define BAUD    4800
void SendData(unsigned char dat)
{
	SBUF = dat; 
    while (!TI);                    //Wait for the previous data is sent
    TI = 0;                         //Clear TI flag
                        //Send current data
}
void delay_AD(unsigned char n)
{
	unsigned char k=5;
	for (;k>0;k--)
	{
		while(n--);
	}
}
void main()
{
	unsigned char n=0X00,shishu=0;
	//LCDInit()
	TMOD=0X20;
	PCON=0X80;
	SCON=0X50;
	ES=1;EA=1;
	TH1 = TL1 = 256-(FOSC/12/16/BAUD);
	TR1=1;
	SendData(0x30);
	delay_AD(200);
	SendData(0x41);
	//while(1);
	ADC_CONTR|=0x80;
	delay_AD(200);
	P1M0|=0xff;
	P1M1|=0xff;
	ADC_CONTR=0xe0;//1110 0000;p1.0,270t
	delay_AD(200);   
	ADC_CONTR|=0x08;  //START
	delay_AD(200);
	while((ADC_CONTR&0x10)!=0x10); //wait ADC_FLAG=1;
	ADC_CONTR&=0xe7;  //ADC_FLAG=0;
	delay_AD(1000);
	n=ADC_DATA;    //8bit 
	SendData(n);
  delay_AD(200);
	//}
	/*while(1)
	{//LCDInit();
		//SendData(0X30);
		//P1=0x00;
	/*	if(ADC_FLAG==1)
		{
			ADC_FLAG=0;
			ADC_POWER=0;
			n=ADC_DATA;
			shishu=n*5/256;
			
		}*/
		
	/*if((ADC_CONTR&0X10)==0X10)
		{
		ADC_CONTR=0xe0;n=ADC_DATA;SendData(n);}
			
			shishu=n*5/256;
			
		//ADC_CONTR|=0x08;
		//}
		//if(ADC_CONTR&=0x00)
		//{P3=n;}+
		/*show(0,0,((shishu*100)/100+'0'));
		show(0,2,((shishu*100)%100/10+'0'));
		show(0,4,((shishu*100)%10+'0'));
		/*show(0,0,0x31);
		show(0,1,0x30);
		show(1,0,'1');
		show(0,15,0x31);
		show(0,14,0x30);
		show(1,15,'1');*/
		
	//}*/
}