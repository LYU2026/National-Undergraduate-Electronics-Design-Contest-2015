#include<reg52.h>//12M
sbit P0_0=P0^0;
sbit P0_1=P0^1;
unsigned char Data[10];
unsigned int i=0;
void main()
{
	SCON=0x50;
	//SMOD=0;
	PCON|= 0x80;
	TMOD|=0x20;
	TH1=0xf3; TL1=0xf3;
	TR1=1;
	ES=1;
	EA=1;
	//P0=0xff;
	while(1)
	{
		if(RI)
		{
			while(SBUF!='\0')
			{
				Data[i]=SBUF;
				i++;
				//for(i=0;i<10;i++)
				//{Data[i]=SBUF;}
				
			}
		
	   }
		  if(Data[10]=="1234dd")
		  {
			   P0_0=0;
				//RI=0;
		   }
		    if(Data[10]=="dd4321")
		   {
			   P0_0=1;
				// RI=0;
		   }
   }
 }