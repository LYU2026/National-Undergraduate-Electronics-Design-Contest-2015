#include<reg51.h>
#include<intrins.h>
#include"main.h"
#include"lcd.h"
#include"ds1302.h"
#include"temp.h"
#include"12864.h"
#include"I2C.h"
#include"pcf8591.h"
#define  NOP()   _nop_() 
//--∂®“Â»´æ÷±‰¡ø

//extern   int  Ds18b20ReadTemp();  //∂¡»°Œ¬∂»







void main()
{
	int t=0;
	lcd_init(); 
	while(1)
	{
		//clr_screen();
	//	P0=0x00;
	//	delay0(600);
		lcd_wcmd(0x80); 
	//	lcd_wdat(0x65);
		t=Ds18b20ReadTemp();
		LcdDisplayTemp0(t);
		//ClockPlay();
	//	ADDisplay();
	//	P0=0xfe;
		delay0(600);
		//while(1);
	}
}

void LcdDisplayTemp0(int temp) 	 //lcdœ‘ æ
{
	unsigned char i; unsigned char T[5];
   	float tp;  
	if(temp< 0)				//µ±Œ¬∂»÷µŒ™∏∫ ˝
  	{
		DisplayData[0] = 0x40; 
		//“ÚŒ™∂¡»°µƒŒ¬∂» « µº Œ¬∂»µƒ≤π¬Î£¨À˘“‘ºı1£¨‘Ÿ»°∑¥«Û≥ˆ‘≠¬Î
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//¡Ù¡Ω∏ˆ–° ˝µ„æÕ*100£¨+0.5 «Àƒ…·ŒÂ»Î£¨“ÚŒ™C”Ô—‘∏°µ„ ˝◊™ªªŒ™’˚–Õµƒ ±∫Ú∞—–° ˝µ„
		//∫Û√Êµƒ ˝◊‘∂Ø»•µÙ£¨≤ªπ‹ «∑Ò¥Û”⁄0.5£¨∂¯+0.5÷Æ∫Û¥Û”⁄0.5µƒæÕ «Ω¯1¡À£¨–°”⁄0.5µƒæÕ
		//À„”…œ0.5£¨ªπ «‘⁄–° ˝µ„∫Û√Ê°£
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//“ÚŒ™ ˝æ›¥¶¿Ì”––° ˝µ„À˘“‘Ω´Œ¬∂»∏≥∏¯“ª∏ˆ∏°µ„–Õ±‰¡ø
		//»Áπ˚Œ¬∂» «’˝µƒƒ«√¥£¨ƒ«√¥’˝ ˝µƒ‘≠¬ÎæÕ «≤π¬ÎÀ¸±æ…Ì
		temp=tp*0.0625*100+0.5;	
		//¡Ù¡Ω∏ˆ–° ˝µ„æÕ*100£¨+0.5 «Àƒ…·ŒÂ»Î£¨“ÚŒ™C”Ô—‘∏°µ„ ˝◊™ªªŒ™’˚–Õµƒ ±∫Ú∞—–° ˝µ„
		//∫Û√Êµƒ ˝◊‘∂Ø»•µÙ£¨≤ªπ‹ «∑Ò¥Û”⁄0.5£¨∂¯+0.5÷Æ∫Û¥Û”⁄0.5µƒæÕ «Ω¯1¡À£¨–°”⁄0.5µƒæÕ
		//À„º”…œ0.5£¨ªπ «‘⁄–° ˝µ„∫Û√Ê°£
	}
	//P3=tp;

	Data0[0] = temp / 10000;
	Data0[1] = temp % 10000 / 1000;
	Data0[2] = temp % 1000 / 100 ;
	Data0[3] = temp % 100 / 10;
	Data0[4] = temp % 10;
  
	for(i=0;i<=4;i++)
	{
		switch(Data0[i])
		{
			case 0:
			{T[i]=0x30;break;}
			case 1:
			{T[i]=0x31;break;}
			case 2:
			{T[i]=0x32;break;}
			case 3:
			{T[i]=0x33;break;}
			case 4:
			{T[i]=0x34;break;}
			case 5:
			{	T[i]=0x35;break;}
			case 6:
			{	T[i]=0x36;break;}
			case 7:
			{T[i]=0x37;break;}
			case 8:
			{T[i]=0x38;break;}
			case 9:
			{T[i]=0x39;break;}			
		}
	}
	lcd_wcmd(0x80); 
	 for(i=0;i<5;i++)
     {
       lcd_wdat(T[i]);
       delay0(3);
     }
		  lcd_wdat(0x65);

}
void delay_main(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<150;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}	

void readtime()
{
	   Time0[0]= (DS1302_ReadOneByteAtAddr(ADDR_SEC_R));
     Time0[1]= (DS1302_ReadOneByteAtAddr(ADDR_MIN_R));
     Time0[2]= (DS1302_ReadOneByteAtAddr(ADDR_HOUR_R));
     Time0[3]= (DS1302_ReadOneByteAtAddr(ADDR_DAY_R));
     Time0[4]= (DS1302_ReadOneByteAtAddr(ADDR_MONTH_R));
     Time0[5]= (DS1302_ReadOneByteAtAddr(ADDR_WEEK_R));
 	   Time0[6]= (DS1302_ReadOneByteAtAddr(ADDR_YEAR_R));
}
void ClockPlay()
{   uchar i;
	
    	if((Time0[7]&0x0a)==0x0a)    
			{		
			  	Time0[7]=(Time0[7]&0xf0);
		  		Time0[7]+=0x10;
		  }
			if((Time0[7]&0x0f)==0x0f)
			{		
					Time0[7]=(Time0[7]&0xf0);
					Time0[7]+=0x09;
			}
	  readtime();
     lcd_pos(1,0);             //…Ë÷√œ‘ æŒª÷√Œ™µ⁄“ª––
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS1[i]);
       delay_main(5);
     }
		 lcd_wcmd(0x91); 
		 lcd_wdat(0x30+Time0[7]/16);
     lcd_wdat(0x30+Time0[7]%16);
		 lcd_wdat(0x30+Time0[6]/16);
     lcd_wdat(0x30+Time0[6]%16);
		 lcd_wdat(0xc4);
		 lcd_wdat(0xea);
		 lcd_wdat(0x30+Time0[4]/16);
     lcd_wdat(0x30+Time0[4]%16);
		 lcd_wdat(0xd4);
		 lcd_wdat(0xc2);
		 lcd_wdat(0x30+Time0[3]/16);
     lcd_wdat(0x30+Time0[3]%16);
		 lcd_wdat(0xc8);
		 lcd_wdat(0xd5);
		 lcd_pos(3,0);           
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS2[i]);
       delay_main(5);
     }
		 //lcd_pos(4,0); 
		 lcd_wcmd(0x9C); 
		 lcd_wdat(0x30+Time0[2]/16);
     lcd_wdat(0x30+Time0[2]%16);
     lcd_wdat(':');
		 lcd_wdat(0x30+Time0[1]/16);
     lcd_wdat(0x30+Time0[1]%16);
		 lcd_wdat(':');
		 lcd_wdat(0x30+Time0[0]/16);
     lcd_wdat(0x30+Time0[0]%16);
}
void ADDisplay()
{int i;
	ISendByte(PCF8591,0x41);
  D[0]=IRcvByte(PCF8591)*2;  //ADC0 ƒ£ ˝◊™ªª1  ∑≈¥Û2±∂œ‘ æ
	LedOut[0]=Disp_Tab[D[0]%10000/1000];
     LedOut[1]=Disp_Tab[D[0]%1000/100];
     LedOut[2]=Disp_Tab[D[0]%100/10];
     LedOut[3]=Disp_Tab[D[0]%10];
	    lcd_pos(3,0);             //…Ë÷√œ‘ æŒª÷√Œ™µ⁄“ª––
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS4[i]);
       delay_main(5);
     }
		 lcd_wcmd(0x98); 
		//lcd_wdat(LedOut[0]);
		 lcd_wdat(LedOut[1]);
		// 		 lcd_wdat(DIS6[0]);
		 //lcd_wdat(DIS6[1]);
		 lcd_wdat(LedOut[2]);
     lcd_wdat(0x2e);//.
		 lcd_wdat(LedOut[3]);
		 lcd_wdat(0x56);//V

}
void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){NOP();}
	}
}
