#include<reg52.h>
#include<intrins.h>
#include"main.h"
#include"12864.h"
#include"ds1302.h"
#include"74HC165.h"
#include"temp.h"
#include"I2C.h"

#include"ad8591.h"
//void ADDisplay();
extern   int  Ds18b20ReadTemp();  //¶ÁÈ¡ÎÂ¶È
void ADDisplay()
{int i;
	ISendByte(PCF8591,0x41);
  D[0]=IRcvByte(PCF8591)*2;  //ADC0 Ä£Êı×ª»»1  ·Å´ó2±¶ÏÔÊ¾
	LedOut[0]=Disp_Tab[D[0]%10000/1000];
     LedOut[1]=Disp_Tab[D[0]%1000/100];
     LedOut[2]=Disp_Tab[D[0]%100/10];
     LedOut[3]=Disp_Tab[D[0]%10];
	lcd_pos(3,0);             //ÉèÖÃÏÔÊ¾Î»ÖÃÎªµÚÒ»ĞĞ
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS4[i]);
       delay(30);
     }
		 lcd_wcmd(0x98); 
		//lcd_wdat(LedOut[0]);
		 lcd_wdat(LedOut[1]);
		// 		 lcd_wdat(DIS6[0]);
		 lcd_wdat(DIS6[1]);
		 lcd_wdat(LedOut[2]);
     lcd_wdat(0x2e);//.
		 lcd_wdat(LedOut[3]);
		 lcd_wdat(0x56);//V

}


void main()
{
   Time0[7]=0x20;
   delay(100);                 //ÉÏµç£¬µÈ´ıÎÈ¶¨
   lcd_init();                 //³õÊ¼»¯LCD
   while(1)
   {
  /*    ClockPlay();
		 delay(500);  
		 clr_screen(); */
LcdDisplayTemp0(Ds18b20ReadTemp());//ÏÔÊ¾ÎÂ¶È	//	 Lcd12864DisplayTemp0(Ds18b20ReadTemp());
	//	 ADDisplay();
	/*	  if(hc74_165()==0xee)
			{

				timesetting();
			}
		*/
 }
/*
	 while(1);

     delay(1000);
     lcdflag();
     clr_screen();             //ÇåÆÁ        
     photodisplay(Photo1);     //ÏÔÊ¾Í¼Æ¬1
     delay(2000);    
     clr_screen();             //ÇåÆÁ
     photodisplay(Photo2);     //ÏÔÊ¾Í¼Æ¬2
     delay(2000);
     bytecode();               //ÏÔÊ¾×Ö·û±í´úÂë
     delay(2000);
     clr_screen();  */
   
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
     lcd_pos(1,0);             //ÉèÖÃÏÔÊ¾Î»ÖÃÎªµÚÒ»ĞĞ
     for(i=0;i<5;i++)
     {
       lcd_wdat(DIS1[i]);
       delay(30);
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
       delay(30);
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

void timesetting()
{
  unsigned char k=0;	
	if(hc74_165()==0x77) 
	{		
		k++;
		//P1=0xfe;
		if(k==7) k=1;
	}
	switch(k)
	{
			case 1:     //°´ÏÂ°´¼ü1£¬½øÈëµ÷ÊÔÄê
			{//P1=0xfd;
				while(1){		switch(hc74_165())
					  {
							case 0x7b://°´ÏÂK9£¬½øÈë¼Ó1
						   { //P1=0xfc;
                  year(0);break;}
							case 0xbb:
					    	{year(1);break;}
								default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}}
			case 2://½øÈëµ÷ÊÔÔÂ
			{
						switch(hc74_165())
					  {
						  case 0xf7://°´ÏÂK9£¬½øÈë¼Ó1
						      {   month(0);break;	}
						  case 0xfb:
                 {month(1);break;}
								 default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}
			case 3:// ½øÈëµ÷ÊÔday
			{
						switch(hc74_165())
					  {
						   case 0xf7://°´ÏÂK9£¬½øÈë¼Ó1
						      {  day(0);break;	}
						   case 0xfb://K13
						      {day(1);break;}
									default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}
			case 4://°´ÏÂK4 ½øÈëµ÷ÊÔhour
			{
						switch(hc74_165())
					  {
						    case 0xf7://°´ÏÂK9£¬½øÈë¼Ó1
						        {  hour(0);break;}
						    case 0xfb:
						       {hour(1);break;}
									 default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}
			case 5://°´ÏÂK5 ½øÈëµ÷ÊÔmin
			{
						switch(hc74_165())
					  {
						    case 0xf7://°´ÏÂK9£¬½øÈë¼Ó1
						       {  min(0);break;	}
						    case 0xfb:
						       {min(1);break;}
									 default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}
			case 6://°´ÏÂK6 ½øÈëµ÷sec
			{
						switch(hc74_165())
					  {
						     case 0xf7://°´ÏÂK9£¬½øÈë¼Ó1
						       { sec(0);break;	}
						     case 0xfb:
						       {sec(1);break;}
									 default:
								{
									DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	                DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øĞĞÉèÖÃ
                  DS1302_WriteOneByteAtAddr(0x8e,0x80);
									ClockPlay();
								}
					  }
			}

   }
}
void year(unsigned char k)
{
	if(k==0)                //¼ÓÒ»
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
	if(k==1)                 //¼õÒ»
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
}
void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){_nop_();}
	}
}
void LcdDisplayTemp0(int temp) 	 //lcdÏÔÊ¾
{
	unsigned char i; unsigned char T[5];
   	float tp;  
	if(temp< 0)				//µ±ÎÂ¶ÈÖµÎª¸ºÊı
  	{
		DisplayData[0] = 0x40; 
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//ËãÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//ÒòÎªÊı¾İ´¦ÀíÓĞĞ¡ÊıµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãĞÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕıµÄÄÇÃ´£¬ÄÇÃ´ÕıÊıµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
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
       delay0(30);
     }

}
/*void Lcd12864DisplayTemp0(int temp) 	 //lcdÏÔÊ¾
{
	unsigned char i; 
	unsigned char code  p2[]= "TEMP:";
   	float tp;  
	if(temp< 0)				//µ±ÎÂ¶ÈÖµÎª¸ºÊı
  	{
		DisplayData[0] = 0x40; 
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//ËãÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//ÒòÎªÊı¾İ´¦ÀíÓĞĞ¡ÊıµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãĞÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕıµÄÄÇÃ´£¬ÄÇÃ´ÕıÊıµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
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
	lcd_pos(1,0);             //ÉèÖÃÏÔÊ¾Î»ÖÃÎªµÚÒ»ĞĞ
	
    for(i=0;i<5;i++)
     {
       lcd_wdat(DIS3[i]);
       delay(30);
     }
		// lcd_wcmd(0x91); 
		 lcd_pos(2,0); 
	// lcd_wdat(T[2]);
	 for(i=1;i<4;i++)
		 {
			 lcd_wdat(T[i]);
		 }
	//	 lcd_wdat("¡ãc");
	//	 lcd_wdat(0xa1);
	//	 lcd_wdat(0xe3);
	// lcd_wdat(0x63);
	/*	   LCD1602_WriteInformation(0x01,0);	//ÇåÆÁÖ¸Áî
	LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p2);
			 LCD1602_DisplayOneCharOnAddr(0,6,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(0,7,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(0,8,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(0,9,T[3]);
       LCD1602_DisplayOneCharOnAddr(0,10,0x06);	//¡ãc
	*/
//}*/
