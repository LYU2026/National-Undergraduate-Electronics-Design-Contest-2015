#include<reg52.h>
#include"main.h"
#include"lcd.h"
void main()
{
  LCD1602_Init();
	Get_Maopi(); 
		while(1)
	 {
		 Get_Weight() ;
		 Show_Weight();
	 }
 }
 void Get_Maopi() 
{  
//	HX711_Buffer = ReadCount();  
	//Weight_Maopi = HX711_Buffer; 
  for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		HX711_Buffer=HX711_Buffer>>8;
		//HX711_Buffer = HX711_Buffer/100; 
		Weight_Maopi +=HX711_Buffer;
	}
	Weight_Maopi=Weight_Maopi/10;  
	//Weight_Shiwu=(unsigned int)((float)Weight_Shiwu*0.05-6);
} 
 void Get_Weight() 
{
	/*HX711_Buffer =ReadCount();  
	Weight_Shiwu = HX711_Buffer/100;
	Weight_Shiwu = (unsigned int)((float)Weight_Shiwu /4.22+0.05); */
	for(i=0;i<10;i++)
	{
		HX711_Buffer =ReadCount();  
		HX711_Buffer=HX711_Buffer>>8;
		//HX711_Buffer = HX711_Buffer/100; 
		Weight_Shiwu +=HX711_Buffer;
	}
	Weight_Shiwu=Weight_Shiwu/10;
	//Weight_Shiwu=(unsigned int)((float)Weight_Shiwu*0.05-6);
	//Weight_Shiwu=(long)((float)(Weight_Shiwu-546)*9.9); //??????,???????????????
	//Weight_Shiwu=(Weight_Shiwu+50)/100*100;
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
}
void Show_Weight()
{
	//Weight_Shiwu-=MaoPi;
		LCD1602_WriteInformation((0x80+0x40),0);
	//ight_Shiwu=Weight_Shiwu<<4;
	 //Weight_Shiwu=(unsigned int)((float)Weight_Shiwu*10+200);
	//Weight_Shiwu=Weight_Shiwu>>4;
//	k=((k+12.61)*0.591/47.9);
	//k=100*k;
	//k=k>>8;
	//LCD1602_DisplayOneCharOnAddr(0,0,(k/100000 + 0x30));  
	//LCD1602_DisplayOneCharOnAddr(0,1,(k%100000/10000 + 0x30));  
	//LCD1602_DisplayOneCharOnAddr(0,2,(k/10000 + 0x30));  
	 LCD1602_DisplayOneCharOnAddr(0,0,(k%10000/1000 + 0x30));   
	// LCD1602_DisplayOneCharOnAddr(0,1,'.');    
	 LCD1602_DisplayOneCharOnAddr(0,1,(k%1000/100 + 0x30));   
	LCD1602_DisplayOneCharOnAddr(0,2,'.');  
	 LCD1602_DisplayOneCharOnAddr(0,3,(k%100/10 + 0x30));  
   LCD1602_DisplayOneCharOnAddr(0,4,(k%10 + 0x30));
}