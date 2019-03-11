#include "lcd.h"
#include "uart.h"
unsigned char i=0,j=0;
unsigned char receiveData[6];
bit showready=1;
void main(void)
{
	LcdInit();
    InitUART();
	LcdWriteCom(0x80);
	while(1)
	{
		if(!showready)
		{
			LcdWriteCom(0x80);
			for(j=0;j<6;j++)
			{
				LcdWriteData(receiveData[j]);
				showready=1;
			}		
		}

	}				   
}
void Usart() interrupt 4
{
	if(showready)
	{
		receiveData[i]=SBUF;
		RI = 0;
		i++;
		if(i>=6)
		{
			i=0;
			showready=0;
		}
	}
	else
		RI=0;	
}