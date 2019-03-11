#include<reg52.h>
#include"main.h"
#include"lcd.h"


void main()
{
  LCD1602_Init();
  Get_Maopi();
	Get_Weight();
	//MaoPi =Weight_Shiwu;
	//Weight_Shiwu = 0,MaoPi = 0; 
   /*while(1)
	 {LCD1602_DisplayOneCharOnAddr(0,0,'1');
		}
		*/
		while(1)
	 {
		 //LCD1602_DisplayOneCharOnAddr(0,0,'1');
		 /*a=(ReadCount()>>4);
		 //P1=a;
		 b=((ReadCount()>>2)&&0x00ff);
		 c=(ReadCount()&&0x0000ff);
		 LCD1602_DisplayOneCharOnAddr(0,0,a);
		 LCD1602_DisplayOneCharOnAddr(0,2,b);
		 LCD1602_DisplayOneCharOnAddr(0,4,c);
		 */
		  Get_Weight();
		  Show_Weight();
		}		 
	
}

void Get_Maopi() 
{  
	HX711_Buffer = ReadCount();  
	Weight_Maopi = HX711_Buffer;   
} 
void Get_Weight() 
{
  	HX711_Buffer =ReadCount();  
	// Weight_Shiwu = HX711_Buffer; 
	 Weight_Shiwu=HX711_Buffer ;
	 Weight_Shiwu = Weight_Shiwu - Weight_Maopi;
	Weight_Shiwu=Weight_Shiwu/100;
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu /4.22+0.05);  
	//./HX711_Buffer=HX711_Buffer/19*500;
	
	/*for(i=0;i<6;i++)
	{
		HX711_Buffer =ReadCount();  
		//HX711_Buffer=HX711_Buffer>>8;
		HX711_Buffer = HX711_Buffer/100; 
		Weight_Shiwu +=HX711_Buffer;
	}
	HX711_Buffer=Weight_Shiwu/5;
	Weight_Shiwu=(long)((float)(Weight_Shiwu-546)*9.9); //??????,???????????????
	Weight_Shiwu=(Weight_Shiwu+50)/100*100;*/
}
void Show_Weight()
{
	//Weight_Shiwu-=MaoPi;
		//LCD1602_WriteInformation((0x80+0x40),0);
	 LCD1602_DisplayOneCharOnAddr(0,0,(Weight_Shiwu/1000 + 0x30));   
	// LCD1602_DisplayOneCharOnAddr(0,1,'.');    
	 LCD1602_DisplayOneCharOnAddr(0,1,(Weight_Shiwu%1000/100 + 0x30));   
	 LCD1602_DisplayOneCharOnAddr(0,2,(Weight_Shiwu%100/10 + 0x30));  
   LCD1602_DisplayOneCharOnAddr(0,3,(Weight_Shiwu%10 + 0x30));
}