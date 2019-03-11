#include<reg52.h>
#include<intrins.h>
#include"main.h"
#include "uart.h"
sbit P0_0=P0^0;
sbit P0_1=P0^1;
bit key=0;


unsigned char keyboardscan(bit p)
{
	if(p!=1)
	{
		_nop_();
		if(p!=1)
		{
			return(1);
		}
	}
	else return(0);
}
void main()
{
	
	P0_0=1;P0_1=1;
	InitUart();
	Get_Maopi(); 
	showdata[0]='#';
		while(1)
	 { 
			if(keyboardscan(P0_0)==1)
				{key=1;}			
			while(key)
				{
				   /*
						if(keyboardscan(P0_1)==1)   //清零
						{
							Get_Maopi();
							//	Weight_Maopi=Weight_Shiwu;
							//Get_Weight() ;
							//for(i=1;i<5;i++)
							//{showdata[i]=0x30;}
							//Weight_Shiwu=0x0000;
						}
						*/
					 Get_Weight() ;
					 if(keyboardscan(P0_0)==1)
						{while(keyboardscan(P0_0)==1);
								 key=0;
									  _nop_();
									  _nop_();
									  _nop_();
									break;								
						 }
            for(i=0;i<5;i++)
					      {SendData(showdata[i]);}							
				}
	}	
}					

 void Get_Maopi() 
{  
  for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		//HX711_Buffer=HX711_Buffer>>8;
		Weight_Maopi +=HX711_Buffer;
	}
	Weight_Maopi=Weight_Maopi/10;  
} 
 void Get_Weight() 
{
	for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		//HX711_Buffer=HX711_Buffer>>8;
		Weight_Shiwu +=HX711_Buffer;
	}
	Weight_Shiwu=Weight_Shiwu/10;
	//if(Weight_Shiwu>=(Weight_Maopi))
	//{
		Weight_Shiwu=Weight_Shiwu-Weight_Maopi;
		Weight_Shiwu=Weight_Shiwu>>8;
		k=(unsigned int)(Weight_Shiwu*12.15/11);
	if(k<=6) k=0;
	//showdata[1]=(k%100000/10000 + 0x30);
	showdata[1]=(k%10000/1000 + 0x30);
	showdata[2]=(k%1000/100 + 0x30);
	showdata[3]=(k%100/10 + 0x30);
	showdata[4]=(k%10 + 0x30);
//	}
	//else
//	{
	//	for(i=1;i<6;i++)
	//	{showdata[i]='-';}
	//}
	//Weight_Maopi-Weight_Shiwu;
 /*if(Weight_Shiwu>0x1770)
		{Weight_Shiwu = (unsigned int)((float)Weight_Shiwu *0.5+0.005); }
	else if(Weight_Shiwu<=0x1770&&Weight_Shiwu>0x03e8)
	{Weight_Shiwu = (unsigned int)((float)Weight_Shiwu *1.04+0.006); }
	else if(Weight_Shiwu<=0x03e8&&Weight_Shiwu>0x0190)
		{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+0x26);}
	else	if(Weight_Shiwu<=0x0190&&Weight_Shiwu>0x0064)
	{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+3.8);}
	else if(Weight_Shiwu<0x0064&&Weight_Shiwu>0x0002)
	{Weight_Shiwu=(unsigned int)((float)Weight_Shiwu+0.15);}
	else if(Weight_Shiwu<=0x0002)
	{Weight_Shiwu=0;} */
	
}
void Usart() interrupt 4
{
	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
}