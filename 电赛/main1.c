#include<reg52.h>
#include"lcd.h"
#include"18B20.h"
#include"ds1302.h"
#include"main1.h"
#include<intrins.h>


void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){NOP();}
	}
}

unsigned char keyboardscan(unsigned char n)
{
	unsigned char board_h, board_l;
	P1=0x0f;
	board_h=P1&0x0f;
	if(board_h!=0x0f)
	{
		delay0(n);
		board_h=P1&0x0f;
		if(board_h!=0x0f)
		{
			P1=board_h|0xf0;
			board_l=P1&0xf0;
			return(board_h+board_l);
		}
	}
	return(0xff);
}

void main()
{
	unsigned char  p0[]="DATE:";
	LCD1602_Init();
	Time0[7]=0x20;
    // readtime(); 	
  while(1)
  {
		//readtime();
			//LcdDisplayTemp0(Ds18b20ReadTemp());//œ‘ æŒ¬∂»
	//	LcdDisplay();
		timesetting();
		
	}		
}

void LcdDisplayTemp0(int temp) 	 //lcdœ‘ æ
{
	unsigned char i; 
	unsigned char code  p2[]= "TEMP:";
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
			{	T[i]=0x30;break;}
			case 0xF8:
			{T[i]=0x36;break;}
			case 0x80:
			{T[i]=0x37;break;}
			case 0x90:
			{T[i]=0x38;break;}			
		}
	}
		 
	LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p2);
			 LCD1602_DisplayOneCharOnAddr(0,6,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(0,7,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(0,8,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(0,9,T[3]);
       LCD1602_DisplayOneCharOnAddr(0,10,0x06);	//°„c
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
void LcdDisplay()
{  unsigned char p0[]="DATE:",p1[]="TIME:";
	//readtime();
    	if((Time0[7]&&0x0a)==0x0a)    
			{		
			  	Time0[7]=(Time0[7]&&0xf0);
		  		Time0[7]=Time0[7]+0x10;
		  }
			if((Time0[7]&&0x0f)==0x0f)
			{		
					Time0[7]=(Time0[7]&&0xf0);
					Time0[7]=Time0[7]+0x09;
			}
			LCD1602_MoveToPosition(0,0);
      LCD1602_DisplayString(p0);
	  	LCD1602_DisplayOneCharOnAddr(0,5,'0'+Time0[7]/16);
	    LCD1602_DisplayOneCharOnAddr(0,6,'0'+Time0[7]%16);
      LCD1602_DisplayOneCharOnAddr(0,7,('0'+Time0[6]/16));			//ƒÍ
	   	LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,10,('0'+Time0[4]/16));			//‘¬
			LCD1602_DisplayOneCharOnAddr(0,11,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,13,('0'+Time0[3]/16));			//»’
			LCD1602_DisplayOneCharOnAddr(0,14,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,15,0x02);
			LCD1602_MoveToPosition(1,0);
      LCD1602_DisplayString(p1);
			LCD1602_DisplayOneCharOnAddr(1,8,('0'+Time0[2]/16));				// ±
			LCD1602_DisplayOneCharOnAddr(1,9,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,10,(':'));
			LCD1602_DisplayOneCharOnAddr(1,11,('0'+Time0[1]/16));				//∑÷
			LCD1602_DisplayOneCharOnAddr(1,12,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,13,(':'));
			LCD1602_DisplayOneCharOnAddr(1,14,('0'+Time0[0]/16));				//√Î
			LCD1602_DisplayOneCharOnAddr(1,15,('0'+Time0[0]%16));
}



void daydata()   //…Ë÷√ÃÏ ˝∫Ø ˝
{
	unsigned char day;
	switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
}

unsigned char checkday()                  //◊‘∂ØºÏ≤È «∑Ò «»ÚƒÍ£¨≤¢Œ™∂‘”¶µƒ‘¬∑›…Ë÷√ÃÏ ˝
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//≈–∂œ «∑Ò «»ÚƒÍ
 {
	  switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x02: day=0x29;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
 }
 else
 {
 switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x02: day=0x28;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
	}
 
 return(day);
}


void timesetting()
{	 
	LcdDisplay();
//	LCD1602_MoveToPosition(1,9);
	//LCD1602_DisplayString(p);
	switch(keyboardscan(2))
	{
			case 0x77:     //∞¥œ¬∞¥º¸1£¨Ω¯»Îµ˜ ‘ƒÍ
			{
				 LCD1602_MoveToPosition(0,4);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						   { year(0);break;}
							case 0xeb:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://∞¥œ¬K2 Ω¯»Îµ˜ ‘‘¬
			{
				  LCD1602_MoveToPosition(0,7);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						  case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						      {   month(0);break;	}
						  case 0xeb:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7d://∞¥œ¬K3 Ω¯»Îµ˜ ‘day
			{
				  LCD1602_MoveToPosition(0,10);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						      {  day(0);break;	}
						   case 0xeb://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7e://∞¥œ¬K4 Ω¯»Îµ˜ ‘hour
			{
				  LCD1602_MoveToPosition(1,1);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						        {  hour(0);break;}
						    case 0xeb:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://∞¥œ¬K5 Ω¯»Îµ˜ ‘min
			{
          LCD1602_MoveToPosition(1,4);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						       {  min(0);break;	}
						    case 0xeb:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://∞¥œ¬K6 Ω¯»Îµ˜sec
			{
				  LCD1602_MoveToPosition(1,7);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0xdb://∞¥œ¬K9£¨Ω¯»Îº”1
						       { sec(0);break;	}
						     case 0xeb:
						       {sec(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}

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
   LcdDisplay();
	LCD1602_MoveToPosition(0,3);           //‘⁄ƒÍµƒ ˝◊÷…œ…¡À∏
  LCD1602_WriteInformation(0x0f,0); 
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
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	 LCD1602_MoveToPosition(0,6);
   LCD1602_WriteInformation(0x0f,0);
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
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(0,9);
  LCD1602_WriteInformation(0x0f,0);
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
		DS1302_SetInit(Time0);
	  DS1302_WriteOneByteAtAddr(0x8e,0x80);
    LcdDisplay();
	  LCD1602_MoveToPosition(1,1);
    LCD1602_WriteInformation(0x0f,0);
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
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
  LCD1602_MoveToPosition(1,4);
  LCD1602_WriteInformation(0x0f,0);
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
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(1,7);
  LCD1602_WriteInformation(0x0f,0);
}



