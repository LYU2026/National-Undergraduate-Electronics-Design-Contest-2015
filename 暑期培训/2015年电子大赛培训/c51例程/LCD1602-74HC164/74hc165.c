/*****************************************************************************************

******************************************************************************************/
#include <reg52.h> 
#include "74hc165.h"

sbit hc165_sdo=P1^5;
sbit hc165_sclk=P1^6;
sbit hc165_pl=P1^7;

unsigned char key=0x3e;

void hc165_init()
{
	hc165_sdo=1;
	hc165_sclk=0;
	hc165_pl=1;
}

unsigned char read_hc165(void)
{
	unsigned i,x=0;
	hc165_pl=0;
	x=x+0;
	hc165_pl=1;
	if(hc165_sdo==1)
	{
		x=x+1;
	}
	for(i=0;i<7;i++)
	{
		hc165_sclk=1;
	    x=x<<1;
		hc165_sclk=0;
		if(hc165_sdo==1)
		{
			x=x+1;
		}
	}
	return x;
}

