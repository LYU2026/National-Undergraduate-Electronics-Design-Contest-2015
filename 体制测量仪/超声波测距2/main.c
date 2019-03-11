#include <reg52.h>	
#include <intrins.h>
#include"uart.h"

sbit  RX=P1^7;  //Echo
sbit  TX=P1^6;  //Trig
sbit key=P1^0;
unsigned char flag;
unsigned int  time=0;
unsigned char receiveData;
unsigned char count=0;
float S=0;
unsigned char disbuff[4]={ 0,0,0,0};
unsigned char i;
void Conut(void);
void delayms(unsigned int ms);
void  StartModule();
void keyscan()
{
     if(key==0)
   {
     //do{}while(key==0);
	 delayms(10);
	 if(key==0)
	 {
        count++;
        if(count>1)
        count=0;
	 }
   }  
}
void main(void)
{
	InitUart();
	//TMOD=0x01;		  
	TH0=0;
	TL0=0;          
	ET0=1;            
	EA=1;			  			
	while(1)
	{
	    keyscan();
	    StartModule();
	    while(!RX);		
	    TR0=1;			    
	    while(RX);			
	    TR0=0;				
        Conut();		
		delayms(80);
		keyscan();
		 if(count==1)
	   {   
		 SendData('@');
	     for(i=0;i<4;i++)
		{
		 SendData(disbuff[i]+'0');
		}
			keyscan();
	 } 
	}
}			 

   void Conut(void)
	{
	unsigned int n;
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*19.0)/1000.0; 
	 n=S*10+0.5;    
	 if((S>=700)||flag==1)
	 {	 
	  flag=0;	 
	 }
	 else
	 {
	  disbuff[0]=n/1000;
	  disbuff[1]=n%1000/100;
	  disbuff[2]=n%100/10;
	  disbuff[3]=n%10;
	 }
 }
/********************************************************/
     void zd0() interrupt 1 		 //T0?D??ó?à′??êy?÷ò?3?,3?1y2a?à·??§
  {
    flag=1;							 //?D??ò?3?±ê??
  }
/********************************************************/
     void  StartModule() 		         //???ˉ?￡?é
  {
	  TX=1;			                     //???ˉò?′??￡?é
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TX=0;
  }
 /********************************************************/ 
void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}
/*****************************************************/
/*void Usart() interrupt 4
{
	unsigned char receiveData;

	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
}*/
 