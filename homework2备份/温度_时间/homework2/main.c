#include<reg52.h> 
#include"hc164.h"
#include"LCD1602.h"
#include"ds1302.h"
#include"74HC165.h"
#include"18B20.h"
unsigned char code ucInitTime[7]={0x55,0x46,0x17,0x21,0x01,0x01,0x13};
/* CLK=P2^0;    //¥Æ–– ±÷”
 INPUT=P2^1;  //¥Æ–– ˝æ›£¨Ω”BªÚ’ﬂA£¨’‚¿ÔŒ“√«Ω”B
  sbit A=P2^2;  //–¸ø’µƒ¡Ó“ª∂À(AªÚB),’‚¿Ô «A£¨Ω”P22
*/
void LcdDisplayTemp(int temp);
void readtime();
void LcdDisplay();
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[10],Time1[6];
//unsigned char Data0[10]={0x11,0x12,0x13,0x14,0X23,0X26,0X40};
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//π≤—Ù ˝¬Îπ‹µƒ±‡¬Î£¨≤¢Ω´ ˝æ›∂®“Â‘⁄CODE«¯ 
					        0xc0,/*0*/
					        0xF9,/*1*/
					        0xA4,/*2*/
					        0xB0,/*3*/
					        0x99,/*4*/
					        0x92,/*5*/
					        0x82,/*6*/
					        0xF8,/*7*/
					        0x80,/*8*/
					        0x90,/*9*/				  
							};

void main()
{
	 //Timep[
	LCD1602_Init();
	//DS1302_SetInit(ucInitTime);
	Time0[7]=0X20;
	while(1)
	{
		readtime();
		LcdDisplay();
		LcdDisplayTemp(Ds18b20ReadTemp());
		switch(hc74_165())
		{
			case 0x7f:
			{LCD1602_DisplayOneCharOnAddr(1,9,'1');break;	}
			case 0xfe:
			{LCD1602_DisplayOneCharOnAddr(1,10,'2');break;	}
		}
	}
}

void readtime()
{    //DS1302_SetInit();
	   Time0[0]= (DS1302_ReadOneByteAtAddr(ADDR_SEC_R));
     Time0[1]= (DS1302_ReadOneByteAtAddr(ADDR_MIN_R));
     Time0[2]= (DS1302_ReadOneByteAtAddr(ADDR_HOUR_R));
     Time0[3]= (DS1302_ReadOneByteAtAddr(ADDR_DAY_R));
     Time0[4]= (DS1302_ReadOneByteAtAddr(ADDR_MONTH_R));
     Time0[5]= (DS1302_ReadOneByteAtAddr(ADDR_WEEK_R));
 	   Time0[6]= (DS1302_ReadOneByteAtAddr(ADDR_YEAR_R));
}

void LcdDisplay()
{  
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
						//œ‘ æ –«∆⁄
			week0=('0'+Time0[5]%16);
			switch(week0)
			{
				case 0x31:
					{week[0]=0x4d;week[1]=0x4f;week[2]=0x4e;break;}//"MON";
				case 0x32:
					{week[0]=0x54;  week[1]=0x55; week[2]=0x45;break;}  //"TUE"
				case 0x33:
					{week[0]=0x57; week[1]=0x45; week[2]=0x44;break;}   //"WED"
				case 0x34:
					{week[0]=0x54;week[1]=0x48;week[2]=0x55;break;}   //"THU"
				case 0x35:
					{week[0]=0x46;week[1]=0x52;week[2]=0x49;break;}  //"FRI"
				case 0x36:
					{week[0]=0x53;week[1]=0x41;week[2]=0x54; break;}  //"SAT"
				case 0x37:  
					{week[0]=0x53;week[1]=0x55;week[2]=0x4e;break;} //"SUN"
			}
			LCD1602_DisplayOneCharOnAddr(0,13,week[0]);	//–«∆⁄		
		  LCD1602_DisplayOneCharOnAddr(0,14,week[1]);	
		  LCD1602_DisplayOneCharOnAddr(0,15,week[2]);	 
			
	  	LCD1602_DisplayOneCharOnAddr(0,0,'0'+Time0[7]/16);
	    LCD1602_DisplayOneCharOnAddr(0,1,'0'+Time0[7]%16);
      LCD1602_DisplayOneCharOnAddr(0,2,('0'+Time0[6]/16));			//ƒÍ
	   	LCD1602_DisplayOneCharOnAddr(0,3,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,4,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,5,('0'+Time0[4]/16));			//‘¬
			LCD1602_DisplayOneCharOnAddr(0,6,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,7,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[3]/16));			//»’
			LCD1602_DisplayOneCharOnAddr(0,9,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,10,0x02);
			LCD1602_DisplayOneCharOnAddr(1,0,('0'+Time0[2]/16));				// ±
			LCD1602_DisplayOneCharOnAddr(1,1,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,2,(':'));
			LCD1602_DisplayOneCharOnAddr(1,3,('0'+Time0[1]/16));				//∑÷
			LCD1602_DisplayOneCharOnAddr(1,4,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,5,(':'));
			LCD1602_DisplayOneCharOnAddr(1,6,('0'+Time0[0]/16));				//√Î
			LCD1602_DisplayOneCharOnAddr(1,7,('0'+Time0[0]%16));

}

void LcdDisplayTemp(int temp) 	 //lcdœ‘ æ
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
	/*lcd_wcmd(0x80); 
	 for(i=0;i<5;i++)
     {
       lcd_wdat(T[i]);
       delay0(30);
     }*/
			 LCD1602_DisplayOneCharOnAddr(1,11,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(1,12,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(1,13,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(1,14,T[3]);
       LCD1602_DisplayOneCharOnAddr(1,15,0x06);	//°„c
}


