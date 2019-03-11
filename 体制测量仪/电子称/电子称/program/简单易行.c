#include<reg52.h>
#include<intrins.h>
#include"main.h"
#include "uart.h"
sbit P0_0=P0^0;
sbit P0_1=P0^1;
bit key=0;
void main()
{

	InitUart();
	Get_Maopi(); 
	showdata[0]='#';
		while(1)
	 { 
				if(P0_0==0)
				{//break;
						while(P0_0==0);
					key=~key;
						//break;
					
					}
					if(key)
					{
				    Get_Weight() ;
				    for(i=0;i<5;i++)
					  {SendData(showdata[i]);}
					}
				}	
}					
			
				/*switch(key)
		    {
			  case 1:
			   {	 qq=1;
					for(i=0;i<5;i++)
					{SendData(showdata[i]);}
					 
					break;
	       }
			    case 0:
			   {qq=0;
					break;
	      }
		 
       }
			 if(qq==1)
			{
				if(P0_0==0)
				{
						delay_main(1);
						if(P0_0==0)
						{
								qq=0;key=0;
						}
						
					}
			}
			 
			 */
		  

 void Get_Maopi() 
{  
  for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		HX711_Buffer=HX711_Buffer>>8;
		Weight_Maopi +=HX711_Buffer;
	}
	Weight_Maopi=Weight_Maopi/10;  
} 
 void Get_Weight() 
{
	for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		HX711_Buffer=HX711_Buffer>>8;
		Weight_Shiwu +=HX711_Buffer;
	}
	Weight_Shiwu=Weight_Shiwu/10;
	Weight_Shiwu=Weight_Shiwu-Weight_Maopi;
	if(Weight_Shiwu>0x1770)
		{Weight_Shiwu = (unsigned int)((float)Weight_Shiwu *1.04+0.007); }
	if(Weight_Shiwu<=0x1770&&Weight_Shiwu>0x03e8)
	{Weight_Shiwu = (unsigned int)((float)Weight_Shiwu *1.04+0.006); }
	if(Weight_Shiwu<=0x03e8&&Weight_Shiwu>0x0190)
		{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+0x26);}
		if(Weight_Shiwu<=0x0190&&Weight_Shiwu>0x0064)
	{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+3.8);}
	if(Weight_Shiwu<0x0064&&Weight_Shiwu>0x0002)
	{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+0.15);}
	if(Weight_Shiwu<=0x0002)
	{Weight_Shiwu=0;}
	k=Weight_Shiwu;
	showdata[1]=(k%10000/1000 + 0x30);
	showdata[2]=(k%1000/100 + 0x30);
	showdata[3]=(k%100/10 + 0x30);
	showdata[4]=(k%10 + 0x30);
}
void Usart() interrupt 4
{
	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
}