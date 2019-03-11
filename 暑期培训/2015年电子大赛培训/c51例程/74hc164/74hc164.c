/*****************************************************************************************
*             
*		                              π”√TYD-51-1∞Â			                             *
                   *
******************************************************************************************/
#include <reg52.h> 
#include <intrins.h>

sbit lcd_rs=P1^0;
sbit lcd_rw=P1^1;
sbit lcd_e=P1^2;
sbit hc164_sdat=P1^3;
sbit hc164_sclk=P1^4;

unsigned char x=0x37;

void send_data(unsigned char dat)
{
	unsigned i;
	for(i=0;i<8;i++)
	{
		if((dat & 0x01)!=0)
		{
			hc164_sdat=1;
		}
		else
		{
			hc164_sdat=0;
		}
		hc164_sclk=1;
		dat=dat>>1;
		hc164_sclk=0;
	}
}

void delay(long dly)
{
	while(dly--);
}

void main()
{
	lcd_rs=0;
	lcd_rw=0;
	lcd_e=0;
	hc164_sdat=0;
	hc164_sclk=0;
	while(1)
	{
		send_data(~x);
		x++;
		delay(15000);
	}
}

