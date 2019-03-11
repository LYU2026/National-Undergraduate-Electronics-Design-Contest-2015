#include<reg52.h> 
#include"hc164.h"
#include"LCD1602.h"
#include"ds1302.h"
#include"74HC165.h"
#include"18B20.h"
#include"main.h"
void main()
{
	LCD1602_Init();
	//DS1302_SetInit(ucInitTime);
	Time0[7]=0X20;
	readtime();x=0;
	//LCD1602_DisplayOneCharOnAddr(0,0,'0');
	while(1)
	{
		/*while(1)
			{readtime();
			//	year(0);month(1);day(1);min(1);hour(1);sec(1);weekset(1);
				LcdDisplay();}  */
		switch(x)
		{
			case 0:
			{//year(0);month(1);day(1);min(1);hour(1);sec(1);weekset(1);
				readtime();
				LcdDisplay();
				LcdDisplayTemp(Ds18b20ReadTemp());
				break;
			}
			case 1:
			{
				timesetting();
				break;
			}
		}
		if(hc74_165()==0x7f)
		{
			x++;
			if(x==2) x=0;
		}
	}
}

void delay_main(unsigned int cnt) 
{
 while(--cnt);
}
void readtime()
{    //DS1302_SetInit();
	   Time0[0]= (DS1302_ReadOneByteAtAddr(ADDR_SEC_R));delay_main(1);
     Time0[1]= (DS1302_ReadOneByteAtAddr(ADDR_MIN_R));delay_main(1);
     Time0[2]= (DS1302_ReadOneByteAtAddr(ADDR_HOUR_R));delay_main(1);
     Time0[3]= (DS1302_ReadOneByteAtAddr(ADDR_DAY_R));delay_main(1);
     Time0[4]= (DS1302_ReadOneByteAtAddr(ADDR_MONTH_R));delay_main(1);
     Time0[5]= (DS1302_ReadOneByteAtAddr(ADDR_WEEK_R));delay_main(1);
 	   Time0[6]= (DS1302_ReadOneByteAtAddr(ADDR_YEAR_R));delay_main(1);
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
			//LCD1602_DisplayOneCharOnAddr(0,12,week[0]);
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
/*  DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //∂‘ ±÷”Ω¯––…Ë÷√
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,3);           //‘⁄ƒÍµƒ ˝◊÷…œ…¡À∏
  LCD1602_WriteInformation(0x0f,0); */
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
/*	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	 LCD1602_MoveToPosition(0,6);
   LCD1602_WriteInformation(0x0f,0);*/
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
        DS1302_SetInit(Time0);
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
/*	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(0,9);
  LCD1602_WriteInformation(0x0f,0);*/
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
/*		DS1302_SetInit(Time0);
	  DS1302_WriteOneByteAtAddr(0x8e,0x80);
    LcdDisplay();
	  LCD1602_MoveToPosition(1,1);
    LCD1602_WriteInformation(0x0f,0);*/
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
/*	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
  LCD1602_MoveToPosition(1,4);
  LCD1602_WriteInformation(0x0f,0);*/
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
/*	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(1,7);
  LCD1602_WriteInformation(0x0f,0);*/
}

void weekset(unsigned char k)
{
	if(k==0)
	{
		Time0[5]+=0x01;
		if(Time0[5]==0x08)
		{
				Time0[5]=0x01;
		}
	}
	if(k==1)
	{	
		Time0[5]-=0x01;
		if(Time0[5]==0x00)
		 {		
				Time0[5]=0x07;
		}	
	}
/*	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(0,15);
  LCD1602_WriteInformation(0x0f,0);*/
}

void timesetting()
{LCD1602_WriteInformation(0x01,0);	
	LCD1602_MoveToPosition(1,13);
	LCD1602_DisplayString("Set");
	if(hc74_165()==0xbf) 
	{
			c++;
		  if(c==8) {c=1;}
			switch(c)
			{
				case 1:{LCD1602_MoveToPosition(0,3);break;}
				case 2:{LCD1602_MoveToPosition(0,6);break;}
				case 3:{LCD1602_MoveToPosition(0,9);break;}
				case 4:{LCD1602_MoveToPosition(1,1);break;}
				case 5:{LCD1602_MoveToPosition(1,4);break;}
				case 6:{LCD1602_MoveToPosition(1,7);break;}
				case 7:{LCD1602_MoveToPosition(0,15);break;}
			}
			LCD1602_WriteInformation(0x0e,0);
	}
	
	switch (hc74_165())
	{
		case 0xdf:
		{
			switch(c)
			{
				case 1: {year(1);break;}
				case 2: {month(1);break;}
				case 3: {day(1);break;}
				case 4: {hour(1); break;}
				case 5: {min(1);break;}
				case 6: {sec(1);break;}
				case 7: {weekset(1);break;}
				default: 
				{
					DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	        DS1302_SetInit(Time0);
	        DS1302_WriteOneByteAtAddr(0x8e,0x80);
          LcdDisplay();
          LCD1602_WriteInformation(0x0f,0);
				}
			}
			break;
		}	
		case 0xef:
		{
			switch(c)
			{
				case 1: {year(0);break;}
				case 2: {month(0);break;}
				case 3: {day(0);break;}
				case 4: {hour(0); break;}
				case 5: {min(0);break;}
				case 6: {sec(0);break;}
				case 7: {weekset(0);break;}
				default: 
				{
					DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	        DS1302_SetInit(Time0);
	        DS1302_WriteOneByteAtAddr(0x8e,0x80);
          LcdDisplay();
          LCD1602_WriteInformation(0x0f,0);
				}
			}break;
		}
	}
}
