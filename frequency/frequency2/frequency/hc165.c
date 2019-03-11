#include "hc165.h"
unsigned long recievedat[4]={6,6,6,6};
void HC165(void)
{
	unsigned char i,j;
	HC165_nPL=0; 
	HC165_nPL=1;
   for(i=0;i<4;i++)
	{
  		for(j=0;j<32;j++)
		{
			recievedat[i]=recievedat[i]<<1;
			if(HC165_OUT==1)
		        recievedat[i]++;
			HC165_CK=1;	 
			i+=0;
			HC165_CK=0;
		}
		
	}
}