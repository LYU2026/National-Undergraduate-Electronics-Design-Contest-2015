#include<stc12c2052ad.h>
#include"main.h"
void main()
{
  Init();
	send(0x31);
}

void Init()
{
	SCON|=0x50;   //方式1  REN=1
	PCON|=0x80;
	EA=1;ES=1;
	AUXR|=0x40;      //12倍，不分频
	TH1=TL1=256-(FOSC/16/BAUD);
  TR1=1;	
}

void send(unsigned char p)
{
	TI=0;
	SBUF=p;
	while(!TI);
	TI=0;
}