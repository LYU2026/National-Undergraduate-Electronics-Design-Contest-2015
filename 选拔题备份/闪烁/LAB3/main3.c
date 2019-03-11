#include<reg51.h>
#include<intrins.h>
#include"main.h"
#include"ds1302.h"
#include"temp.h"
#include"12864.h"
#include"I2C.h"
#include"pcf8591.h"
//#include"74HC165.h"
#define  NOP()   _nop_() 
//--∂®“Â»´æ÷±‰¡ø


void main()
{
  Time0[7]=0x20;
	lcd_init(); 
/*	lcd_wcmd(0x80);	     lcd_wdat(0x30);
	lcd_wcmd(0x0d);      //œ‘ æø™£¨πÿπ‚±Í
    delay12(5);
	lcd_wcmd(0x90); lcd_wdat(0x30);

	while(1);*/
	//readtime();
//	ClockPlay();
	while(1)
	{
		
		switch(keyboardscan(4))
		{
			case 0x7e: 
				{//n=0;
					n++;
					if(n==3) n=1;
					clr_screen();
					break;
				}
			case  0xbe: {n=4;clr_screen();break;}
		}
		switch(n)
		{
			case 1:{	readtime(); ClockPlay();break;}
			case 2:{LcdDisplayTemp0(Ds18b20ReadTemp());ADDisplay();break;}
			case 4:{timesetting();clr_screen();break;}
     }
		
		//sec(1);// min(1);//hour(1);//day(1);//month(1);//year(1);
		//P1=hc74_165();
		//lcd_wcmd(0x80); lcd_wdat(T[3]);
   // LcdDisplayTemp0(Ds18b20ReadTemp());ADDisplay();
		//
		//	  readtime();
		//ClockPlay();
  	//
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
	     lcd_pos(1,0);             //??????????
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS3[i]);
     }
		 lcd_wcmd(0x93);
		 lcd_wdat(T[0]);
	lcd_wdat(T[1]);
	lcd_wdat(T[2]);
	lcd_wdat(0x2e);
	lcd_wdat(T[3]);
	lcd_wdat(T[4]);//lcd_pos(3,0);
	lcd_wdat(0xa1);
	lcd_wdat(0xe3);
	lcd_wdat(0x63);
	/* for(i=1;i<5;i++)
     {
       lcd_wdat(T[i]);
       delay0(3);
     }
		  lcd_wdat(0x65);
*/
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
		 lcd_wcmd(0x9B); 
		 lcd_wdat(0x30+Time0[2]/16);
     lcd_wdat(0x30+Time0[2]%16);

     lcd_wdat(':');
		 		 lcd_wcmd(0x9D); 
		 lcd_wdat(0x30+Time0[1]/16);
     lcd_wdat(0x30+Time0[1]%16);
		 lcd_wdat(':');
		 		 lcd_wcmd(0x9F); 
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
		 lcd_wcmd(0x9C); 
		//lcd_wdat(LedOut[0]);
		 lcd_wdat(LedOut[1]);
		// 		 lcd_wdat(DIS6[0]);
		 //lcd_wdat(DIS6[1]);
		      lcd_wdat(0x2e);//.
		 lcd_wdat(LedOut[2]);

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

void delay10(unsigned int n)
{
	  while(n--);
}
void timesetting()
{
	while(1)
	{
		ClockPlay();
		lcd_wcmd(0x84); 
		for(a=0;a<7;a++)
		 {lcd_wdat(DIS5[a]);}
	switch(keyboardscan(4))
	{
			case 0x77:     //∞¥œ¬∞¥º¸1£¨Ω¯»Îµ˜ ‘ƒÍ
			{
				 while (1)
				 {
					 lcd_wcmd(0x92); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
							case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						   { year(0);break;}
							case 0xbd:
					    	{year(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
			case 0xb7://∞¥œ¬K2 Ω¯»Îµ˜ ‘‘¬
			{
					while (1)
					{lcd_wcmd(0x94); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
						  case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						      {   month(0);break;	}
						  case 0xbd:
                 {month(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
			case 0xd7://∞¥œ¬K3 Ω¯»Îµ˜ ‘day
			{
					while (1)
					{lcd_wcmd(0x96); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
						   case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						      {  day(0);break;	}
						   case 0xbd://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
			case 0xe7://∞¥œ¬K4 Ω¯»Îµ˜ ‘hour
			{
					while (1)
					{lcd_wcmd(0x9B); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
						    case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						        {  hour(0);break;}
						    case 0xbd:
						       {hour(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
			case 0x7b://∞¥œ¬K5 Ω¯»Îµ˜ ‘min
			{
				while(1)
					{lcd_wcmd(0x9D); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
						    case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						       {  min(0);break;	}
						    case 0xbd:
						       {min(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
			case 0xbb://∞¥œ¬K6 Ω¯»Îµ˜sec
			{
					while (1)
					{
						lcd_wcmd(0x9F); lcd_wcmd(0x0d);  
						switch(keyboardscan(8))
					  {
						     case 0x7d://∞¥œ¬K9£¨Ω¯»Îº”1
						       { sec(0);break;	}
						     case 0xbd:
						       {sec(1);break;}
					  }
				   if(keyboardscan(4)==0xed)
			     {lcd_wcmd(0x0c);break;}
				 }break;
			}
   }
	 if(keyboardscan(4)==0xee)
			     {n=1; lcd_wcmd(0x0c);break;}  //lcd_wcmd(0x0c);
 }
}
void year(unsigned char k)
{
	if(k==0)                //º”“ª
	{	
		Time0[6]+=0x01;
		if(Time0[6]==0x9a)
		{
				Time0[6]=0x00;
				Time0[7]+=0x01;
				if((Time0[7]&0x0a)==0x0a)
				{		
						Time0[7]=(Time0[7]&0xf0);
						Time0[7]+=0x10;
			  }
		}
	  if((Time0[6]&0x0a)==0x0a)
		{		
				Time0[6]=(Time0[6]&0xf0);
				Time0[6]+=0x10;
     }	
	}
	if(k==1)                 //ºı“ª
	{
		 Time0[6]-=0x01;
		 if((Time0[6]&0x0f)==0x0f)
			{		
					Time0[6]=(Time0[6]&0xf0);
					Time0[6]+=0x09;
			}
			if(Time0[6]==0x00)
			{		
					Time0[6]=0x99;
					Time0[7]-=0x01;
				  if((Time0[7]&0x0f)==0x0f)
					{		
							 Time0[7]=(Time0[7]&0xf0);
							 Time0[7]+=0x09;
					}
			}	
	}
  DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
	lcd_wcmd(0x0d);      //œ‘ æø™£¨πÿπ‚±Í
  ClockPlay();
}

void month(unsigned char k)
{
	if(k==0)
	{
		Time0[4]+=0x01;
		if(Time0[4]==0x13)
		{
				Time0[4]=0x01;
		}
    if((Time0[4]&0x0a)==0x0a)
	  {		
			Time0[4]=(Time0[4]&0xf0);
			Time0[4]+=0x10;
	  }
   }
  if(k==1)
  {
	  Time0[4]-=0x01;
		if(Time0[4]==0x00)
		{		
				Time0[4]=0x12;
		}	
   if((Time0[4]&0x0f)==0x0f)
		{		
				Time0[4]=(Time0[4]&0xf0);
				Time0[4]+=0x09;
		}
  }
	DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  ClockPlay();
}

void day(unsigned char k)
{
	 if(k==0)
	 {				
		 if(Time0[3]==checkday())
		 {		
			  Time0[3]=0x00;
		 }	
		Time0[3]+=0x01;
		if((Time0[3]&0x0a)==0x0a)
		{		
				Time0[3]=(Time0[3]&0xf0);
				Time0[3]+=0x10;
		}
	}
	if(k==1)
  {
		 if(Time0[3]==0x01)
			{		
					Time0[3]=checkday()+0x01;
			}	
      Time0[3]-=0x01;
			if((Time0[3]&0x0f)==0x0f)
			{		
			  	Time0[3]=(Time0[3]&0xf0);
					Time0[3]+=0x09;
			}
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  ClockPlay();
}
void hour(unsigned char k)
{
	 if(k==0)
	 {
		 Time0[2]+=0x01;
		 if(Time0[2]==0x24)
		 {
				Time0[2]=0x00;
		 }
		if((Time0[2]&0x0a)==0x0a)
		{		
				Time0[2]=(Time0[2]&0xf0);
				Time0[2]+=0x10;
		 }
	 }
	 if(k==1)	
		{
			if(Time0[2]==0x00)
			{		
					Time0[2]=0x24;
			}	
 			Time0[2]-=0x01;
			if((Time0[2]&0x0f)==0x0f)
			{		
					Time0[2]=(Time0[2]&0xf0);
					Time0[2]+=0x09;
			}	
		}
   DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  ClockPlay();		
}

void min(unsigned char k)
{
	if(k==0)
	{
		Time0[1]+=0x01;
		if((Time0[1]&0x0a)==0x0a)
		{		
			 Time0[1]=(Time0[1]&0xf0);
			 Time0[1]+=0x10;
		}
		if(Time0[1]==0x60)
		{
				Time0[1]=0x00;
		}
	}
	if(k==1)
	{
		if(Time0[1]==0x00)
		{		
				Time0[1]=0x60;
		}
    Time0[1]-=0x01;
    if((Time0[1]&0x0f)==0x0f)
		{		
				Time0[1]=(Time0[1]&0xf0);
				Time0[1]+=0x09;
		}	
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  ClockPlay();
}
void sec(unsigned char k)
{
	if(k==0)
	{
		Time0[0]+=0x01;
		if((Time0[0]&0x0a)==0x0a)
		{		
			 Time0[0]=(Time0[0]&0xf0);
			Time0[0]+=0x10;
    }
		if(Time0[0]==0x60)
		{
					Time0[0]=0x00;
		}
	}
	if(k==1)
  {
		if(Time0[0]==0x00)
		{		
				Time0[0]=0x60;
		}	
    Time0[0]-=0x01;
		if((Time0[0]&0x0f)==0x0f)
		{		
					Time0[0]=(Time0[0]&0xf0);
					Time0[0]+=0x09;
		}	
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  ClockPlay();
}
