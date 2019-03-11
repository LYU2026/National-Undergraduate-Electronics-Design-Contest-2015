#include<reg52.h>
unsigned int sendCount,sendPosi=0,i=0;
unsigned char sendBuf[]="123456b";
void uart_init() 
{ 
   SCON=0x50;
	//SMOD=0;
	PCON|= 0x80;
	TMOD|=0x20;
	TH1=0xf3; TL1=0xf3;
	TR1=1;
	ES=1;
	EA=1;
  TI=0;//????????? 
  RI=0;//????????? 
  TR1=1;//?????1 
   EA=1; //??????
	ES=1;  //??????
}

void main()
{
	P1=0xff;
   uart_init() ;
	while(sendBuf[i]!='b')
	{
     i++;
		sendCount=i;
  }
	TI=1;
}
void commIntProc() interrupt 4
{
   if(TI)
		{
				TI = 0; 
				if(sendPosi < sendCount) //????????
				{
						sendPosi++;                     //?????????
						SBUF = sendBuf[sendPosi];//????
				}
			}
}

