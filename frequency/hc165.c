#include "hc165.h"
unsigned char recievedat[16]={0};
void HC165(void)
{
   unsigned char i,j;
    unsigned char Temp;
     HC165_CK = 1;
     HC165_nPL = 0;        //HC165¶Á°´¼ü
     HC165_nPL = 1;        //ÔÝÍ£HC165¶Á°´¼ü
	for(j=0;j<16;j++)
	{
     Temp = 0;
     if(HC165_OUT == 1) Temp |= 0x01;
     for(i = 0;i < 7;i++)
     {    
         HC165_CK = 0;  
         HC165_CK = 1;  
         Temp <<= 1;
         if(HC165_OUT == 1) 
         {
             Temp |= 0x01;
         }
     }
			recievedat[j]=Temp;
	 }
	 HC165_CK = 0;
}