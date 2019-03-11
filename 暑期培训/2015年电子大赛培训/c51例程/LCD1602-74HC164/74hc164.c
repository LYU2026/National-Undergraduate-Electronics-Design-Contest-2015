/*****************************************************************************************

******************************************************************************************/
#include <reg52.h> 
#include "74hc164.h"

sbit hc164_sdat=P1^3;
sbit hc164_sclk=P1^4;

void hc164_init()
{
	hc164_sclk=0;
}

void send_data(unsigned char dat)
{
	unsigned i;
	for(i=0;i<8;i++)
	{
		if((dat & 0x80)!=0)
		{
			hc164_sdat=1;
		}
		else
		{
			hc164_sdat=0;
		}
		hc164_sclk=1;
		dat=dat<<1;
		hc164_sclk=0;
	}
}

