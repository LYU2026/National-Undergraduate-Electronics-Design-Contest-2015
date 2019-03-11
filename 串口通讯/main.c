#include<reg52.h>//11.0952M
sbit P0_0=P0^0;
sbit P0_1=P0^1;
unsigned char Data0[10]="1234dd",Data1[10]=="dd4321";
unsigned int j=0;
void delay(unsigned int i);
void main()
{
	SCON=0x40;
	PCON&= 0x7f;
	TMOD|=0x20;
	TH1=0xfa; TL1=0xfa;
	TR1=1;
	ES=1;
	EA=1;
	P0=0xff;
	while(1)
	{
		if(P0_0==0)
		{
			for(j=0;j<10;j++)
			{
				SBUF=Data0[i];
			}
		}
		if(P0_1==0)
		{
				for(j=0;j<10;j++)
			{
				SBUF=Data1[i];
			}
		}
	}
}


 void delay(unsigned int i)
{
	unsigned char j;
	for(i; i > 0; i--)
		for(j = 200; j > 0; j--) ;

}