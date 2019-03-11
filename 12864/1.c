#include<reg52.h>
#include<intrins.h>
#include"main.h"
#include"12864.h"



void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){NOP();}
	}
}
void main()
{
   lcd_init();                 //≥ı ºªØLCD
	while(1)
	{
		LcdDisplayTemp0(Ds18b20ReadTemp());//œ‘ æŒ¬∂»
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
		switch(Seg_Data[Data0[i]])
		{
			case 0xc0:
			{T[i]=0x30;break;}
			case 0xF9:
			{T[i]=0x31;break;}
			case 0xA4:
			{T[i]=0x32;break;}
			case 0xB0:
			{T[i]=0x33;break;}
			case 0x99:
			{T[i]=0x34;break;}
			case 0x92:
			{	T[i]=0x35;break;}
			case 0x82:
			{	T[i]=0x36;break;}
			case 0xF8:
			{T[i]=0x37;break;}
			case 0x80:
			{T[i]=0x38;break;}
			case 0x90:
			{T[i]=0x39;break;}			
		}
	}
	lcd_wcmd(0x80); 
	 for(i=0;i<5;i++)
     {
       lcd_wdat(T[i]);
       //delay0();
     }
	
}