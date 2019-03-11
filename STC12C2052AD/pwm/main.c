#include<stc12c2052ad.h>

void main()
{
	CCON=0;
	CMOD=0x02;
	CL=0x00;
	CH=0x00;
	CCAP0L=CCAP0H=0x80;
	CCAPM0=0x42;
	
	CCAP1L=CCAP1H=0xFF;
	CCAPM1=0x03;
	
	CR=1;
	while(1);
}