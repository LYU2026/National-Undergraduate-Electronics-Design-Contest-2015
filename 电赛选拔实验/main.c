#include<reg52.h>
//#include"temp.h"
//#include"lcd.h"
//#include"pcf8591.h"
#include"ds1302.h"
#include"i2c.h"
#include<intrins.h>


//--¶¨ÒåPCF8591µÄ¶ÁÐ´µØÖ·--//
#define  WRITEADDR 0x90	   //Ð´µØÖ·
#define  READADDR  0x91	   //¶ÁµØÖ·

//--¶¨ÒåÈ«¾Ö±äÁ¿--//
unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0¡¢1¡¢2¡¢3¡¢4¡¢5¡¢6¡¢7¡¢8¡¢9¡¢A¡¢b¡¢C¡¢d¡¢E¡¢FµÄÏÔÊ¾Âë
unsigned char DisplayData[8];
//ÓÃÀ´´æ·ÅÒªÏÔÊ¾µÄ8Î»ÊýµÄÖµ

//--ÉùÃ÷È«¾Öº¯Êý--//
void DigDisplay(); //¶¯Ì¬ÏÔÊ¾º¯Êý
void Pcf8591SendByte(unsigned char channel);
unsigned char Pcf8591ReadByte();
void Pcf8591DaConversion(unsigned char value);
void DigDisplay();
void LcdDisplayTemp0(int temp); 
void LcdDisplay();
	unsigned char code p0[]="DATE:",
	                   p1[]= "TIME:",
	                   p2[]= "TEMP:",
	                   p3[]="AD1:",
	                   p4[]="Relay:";
/******************************************************************************
º¯ÊýÃû³Æ£ºDelay
º¯Êý¹¦ÄÜ£ºÑÓÊ±º¯Êý
Èë¿Ú²ÎÊý£ºuiCount-ÑÓÊ±²ÎÊý£¬Ã¿¼Ó1Ôö¼Ó0.5ms
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÎÞ
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
										 
sbit LCD1602_RS = P2^6;	//Î»¶¨Òå£¬Òº¾§µÄÊý¾Ý/ÃüÁîÑ¡Ôñ
sbit LCD1602_RW = P2^5;	//Î»¶¨Òå£¬Òº¾§µÄ¶ÁÐ´Ñ¡Ôñ
sbit LCD1602_EN = P2^7;	//Î»¶¨Òå£¬Òº¾§Ê¹ÄÜÐÅºÅ

#define LCDPORT P0	  //Òº¾§µÄÊý¾Ý¿Ú
unsigned char  code Data[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//Äê
														0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,//ÔÂ
														0x1f,0x11,0x11,0x1f,0x11,0x11,0xff,0x00,//ÈÕ
														0x0e,0x0a,0x0e,0x00,0x1f,0x11,0x11,0x1f,//ÂÀ
	                          0x04,0x1f,0x08,0x1f,0x09,0x0f,0x09,0x09,
														0x02,0x02,0x1f,0x06,0x0a,0x12,0x02,0x06,
	                          0x1c,0x14,0x1c,0x03,0x04,0x04,0x04,0x03//¡ãc
               };

/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_CheckBusy
º¯Êý¹¦ÄÜ£ºÃ¦¼ì²â
Èë¿Ú²ÎÊý£ºÎÞ
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÃ¦¼ì²â²ÉÓÃÁË¶à¼ÓÒ»¸öÑÓÊ±Ìõ¼þµÄ°ì·¨£¬·ÅÖÃ³ÌÐò¿¨ÔÚ¼ì²âº¯ÊýÖÐ
*******************************************************************************/
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //¶ÁÖ®Ç°ÏÈÖÃÎ»£¬×¼±¸¶ÁÈ¡IO¿ÚÊý¾Ý
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //Ê¹Òº¾§´¦ÓÚ¶ÁÊý¾Ý×´Ì¬
	LCD1602_EN = 1;	 //Ê¹ÄÜÒº¾§£¬¸ßµçÆ½ÓÐÐ§
	while((i--) && (LCDPORT & 0x80));	//Ã¦¼ì²â
	LCD1602_EN = 0;
}

/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_WriteInformation
º¯Êý¹¦ÄÜ£ºÏòLCD1602Òº¾§Ð´ÈëÊý¾Ý»òÕßÃüÁî
Èë¿Ú²ÎÊý£ºucData-ÒªÐ´ÈëµÄÊý¾Ý»òÕßÃüÁî²ÎÊý
		  bComOrData-Îª0Ê±Ð´ÈëµÄÊ±ÃüÁî£¬Îª1Ê±Ð´ÈëµÄÊ±Êý¾Ý
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÎÞ
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //ÔÚÐ´ÈëÊý¾Ý»òÕßÃüÁîÇ°ÏÈ½øÐÐÃ¦¼ì²â
	LCDPORT = ucData;		 //ÏÈ½«Êý¾Ý»òÕßÃüÁîËÍÖÁIO
	LCD1602_RS = bComOrData;	//È·¶¨ÊÇÐ´ÈëÊý¾Ý»¹ÊÇÐ´ÃüÁî
	LCD1602_RW = 0;		//Ê¹Òº¾§´¦ÓÚÐ´ÈëÐÅÏ¢×´Ì¬
	LCD1602_EN = 1;		//Ê¹ÄÜÒº¾§£¬¸ßµçÆ½ÓÐÐ§
	LCD1602_EN = 0;
}

/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_Init
º¯Êý¹¦ÄÜ£ºÒº¾§³õÊ¼»¯º¯Êý
Èë¿Ú²ÎÊý£ºÎÞ
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÎÞ
*******************************************************************************/
void LCD1602_Init()		 
{
	unsigned char i;
	LCD1602_WriteInformation(0x38,0);
	Delay(300);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0); 	//Ð´ÈëÃüÁî£¬5x7µãÕó¹¤×÷·½Ê½£¬8Î»Êý¾Ý½Ó¿Ú
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//ÏÔÊ¾ÉèÖÃ£¬¿ªÏÔÊ¾£¬¹â±ê²»ÏÔÊ¾£¬²»ÉÁË¸//        £¨Îª0Ê±·ñ£©
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//ÇåÆÁÖ¸Áî
	Delay(20);
	LCD1602_WriteInformation(0x40,0);	
	for(i=0;i<64;i++)
	{
			LCD1602_WriteInformation(Data[i],1);	
	}
}
  
/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_MoveToPosition
º¯Êý¹¦ÄÜ£º½«Òº¾§µÄ¹â±êÒÆ¶¯µ½Ö¸¶¨µÄÎ»ÖÃ
Èë¿Ú²ÎÊý£ºx-Òº¾§ÏÔÊ¾µÄÐÐÊý£¬·¶Î§0-1
			x = 0:ÔÚÒº¾§µÄµÚÒ»ÐÐ
			x = 1:ÔÚÒº¾§µÄµÚ¶þÐÐ
		  y-Òº¾§ÏÔÊ¾µÄÁÐÊý£¬·¶Î§0-15
		    y = 0:ÔÚÒº¾§µÄµÚÒ»ÁÐ
			y = 1:ÔÚÒº¾§µÄµÚ¶þÁÐ
			......
			y = 15:ÔÚÒº¾§µÄµÚÊ®ÁùÁÐ
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÍ¨¹ýÖ¸¶¨x,yµÄÖµ¿ÉÒÔ½«Òº¾§µÄ¹â±êÒÆ¶¯µ½Ö¸¶¨µÄÎ»ÖÃ
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //¹â±ê¶¨Î»µ½µÚÒ»ÐÐµÄyÁÐ
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //¹â±ê¶¨Òåµ½µÚ¶þÐÐµÄyÁÐ
}

/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_DisplayOneCharOnAddr
º¯Êý¹¦ÄÜ£ºÔÚÖ¸¶¨µÄÎ»ÖÃÉÏÏÔÊ¾Ö¸¶¨µÄ×Ö·û
Èë¿Ú²ÎÊý£ºx-Òº¾§ÏÔÊ¾µÄÐÐÊý£¬·¶Î§0-1
			x = 0:ÔÚÒº¾§µÄµÚÒ»ÐÐ
			x = 1:ÔÚÒº¾§µÄµÚ¶þÐÐ
		  y-Òº¾§ÏÔÊ¾µÄÁÐÊý£¬·¶Î§0-15
		    y = 0:ÔÚÒº¾§µÄµÚÒ»ÁÐ
			y = 1:ÔÚÒº¾§µÄµÚ¶þÁÐ
			......
			y = 15:ÔÚÒº¾§µÄµÚÊ®ÁùÁÐ
		  ucData-ÒªÏÔÊ¾µÄ×Ö·ûÊý¾Ý
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÈ·±£x,yµÄÈ¡ÖµÒªÔÚÖ¸¶¨µÄ·¶Î§ÄÚ
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //¹â±êÎ»ÖÃ
	LCD1602_WriteInformation(ucData,1);	  //Ð´ÈëÊý¾Ý
}

/******************************************************************************
º¯ÊýÃû³Æ£ºLCD1602_DisplayString
º¯Êý¹¦ÄÜ£ºÏÔÊ¾×Ö·û´®
Èë¿Ú²ÎÊý£ºucStr-×Ö·û´®µÄÊ×µØÖ·
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÎÞ
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //×Ö·û´®½áÊøÖ®Ç°£¬Ñ­»·ÏÔÊ¾
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //ÒÀ´ÎÐ´ÈëÃ¿Ò»¸ö×Ö·û
		 ucStr++;								 //Ö¸ÕëÔö¼Ó
	}
}

/******************************************************************************
º¯ÊýÃû³Æ£ºDelay
º¯Êý¹¦ÄÜ£ºÑÓÊ±º¯Êý
Èë¿Ú²ÎÊý£ºuiCount-ÑÓÊ±²ÎÊý£¬Ã¿¼Ó1Ôö¼Ó0.5ms
·µ»ØÖµ£ºÎÞ
±¸×¢£ºÎÞ
*******************************************************************************/

#ifndef __TEMP_H_
#define __TEMP_H_

//---ÖØ¶¨Òå¹Ø¼ü´Ê---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--¶¨ÒåÊ¹ÓÃµÄIO¿Ú--//
sbit DSPORT=P3^7;

//--ÉùÃ÷È«¾Öº¯Êý--//
 void Delay1ms(uint );
uchar Ds18b20Init();
 void Ds18b20WriteByte(uchar com);
uchar Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();
//sbit LCD1602_RS = P2^6;	//Î»¶¨Òå£¬Òº¾§µÄÊý¾Ý/ÃüÁîÑ¡Ôñ
//sbit LCD1602_RW = P2^5;	//Î»¶¨Òå£¬Òº¾§µÄ¶ÁÐ´Ñ¡Ôñ
//sbit LCD1602_EN = P2^7;	//Î»¶¨Òå£¬Òº¾§Ê¹ÄÜÐÅºÅ

#define LCDPORT P0	  //Òº¾§µÄÊý¾Ý¿Ú

//unsigned char code ucForum[]=" bbs.cepark.com ";	  //ÔÚCODEÇø¶¨ÒåÒ»¸öÓÃÓÚÏÔÊ¾µÄ³£Á¿×Ö·û´®

/*extern void Delay(unsigned int uiCount);	//ÑÓÊ±º¯Êý
extern void LCD1602_CheckBusy(void);	//Òº¾§Ã¦¼ì²â
extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//ÔÚÒº¾§ÉÏÐ´Êý¾Ý»òÕßÐ´ÃüÁî£¬0ÎªÃüÁî£¬1ÎªÊý¾Ý
extern void LCD1602_Init();	//Òº¾§³õÊ¼»¯
extern void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Òº¾§µÄ×ø±êÒÆ¶¯µ½Ö¸¶¨Î»ÖÃ
extern void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//ÔÚÒº¾§Ö¸¶¨Î»ÖÃÏÔÊ¾×Ö·û
extern void LCD1602_DisplayString(unsigned char *ucStr);	//ÔÚÒº¾§ÉÏÏÔÊ¾×Ö·û´®
*/
//extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//ÔÚÒº¾§ÉÏÐ´Êý¾Ý»òÕßÐ´ÃüÁî£¬0ÎªÃüÁî£¬1ÎªÊý¾Ý
void Pcf8591SendByte(unsigned char channel);
unsigned char Pcf8591ReadByte();
void Pcf8591DaConversion(unsigned char value);
void pcf8591Play();
extern   int  Ds18b20ReadTemp();  //¶ÁÈ¡ÎÂ¶È
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[5],ADData[8];
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//¹²ÑôÊýÂë¹ÜµÄ±àÂë£¬²¢½«Êý¾Ý¶¨ÒåÔÚCODEÇø 
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
unsigned char code  p[]="Setting";	
void year(unsigned char k);//Äê¼Ó¼õ
void month(unsigned char k);//ÔÂ¼Ó¼õ
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void weekset(unsigned char k);//ÖÜ¼Ó¼õ
void timesetting();//µç×ÓÖÓÉèÖÃ
void readtime();  //¶ÁÈ¡DS13B20ÖÐµÄÊ±¼ä

#define  NOP()   _nop_() 
sbit RELAY = P3^0;	
sbit KEY=P3^1;
int num1 =0;
sbit Delay1=P3^2;
sbit bee=P3^3;
void readtime();  //¶ÁÈ¡DS13B20ÖÐµÄÊ±¼ä
unsigned char  timecount=0;
unsigned char T[5];
unsigned char dat;	
unsigned char timedata[]={8,14,50};//Ê±·ÖÃë
unsigned char time1[]={8,15,00};
 unsigned char time2[]={21,30,00};
unsigned char i,timenum,u=10;
unsigned char Time0[9];
 void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){NOP();}
	}
}
 void delay1(unsigned int n)
{
	unsigned int k;
	for(k=1000;k>0;k--)
	{
	  while(n--);
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
			bee=1;
			delay0(n);
			bee=0;
			return(board_h+board_l);
		}
	}
	return(0xff);
}

void clockdisaplay();
void delayA(unsigned int n);
void clocksetting();
void conctrlRelay();
void KEY_Detect();
unsigned char hc74_165();
//void Delay(unsigned int n);	//ÑÓÊ±º¯Êý
void delay1msA();

void daydata()   //ÉèÖÃÌìÊýº¯Êý
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

unsigned char checkday()                  //×Ô¶¯¼ì²éÊÇ·ñÊÇÈòÄê£¬²¢Îª¶ÔÓ¦µÄÔÂ·ÝÉèÖÃÌìÊý
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//ÅÐ¶ÏÊÇ·ñÊÇÈòÄê
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
void main()
{
	unsigned char code p0[]="DATE:",
	                   p1[]= "TIME:",
	                  
	                   p4[]="Relay:";
	

	   	TMOD=0x01;
                	TH0=0xd8;TL0=0xf0; //10ms
                 	EA=1;ET0=1;
	               IT0=0;EX0=1;
	   LCD1602_Init();
	//LCD1602_DisplayOneCharOnAddr(0,0,'A');
	//while(1);
	   Time0[7]=0x20;
     readtime(); 	
	   
		
	  // LCD1602_WriteInformation(0x01,0);	//ÇåÆÁÖ¸Áî
	//LCD1602_MoveToPosition(0,0);
      //     LCD1602_DisplayString(p0);
	//while(1);
	//num1=0;
	
		 while(1)
	 {
			 
	    	
		   switch(num1)
		   {
				 case 0:
				 {	
					 LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p0);
					 LcdDisplay();
					 timesetting();
					 //break;
				 }
				 case 1:
				 {
					 LcdDisplayTemp0(Ds18b20ReadTemp());//ÏÔÊ¾ÎÂ¶È
					 pcf8591Play();
					 //break;
					}
			 }
			 switch(keyboardscan(1))
			 {
			 case 0xbd:
				    // LCD1602_WriteInformation(0x01,0);
				    num1=0;break;
			    case 0xbe:
						  //LCD1602_WriteInformation(0x01,0);
						num1=1;break;
       }
}
}

void LcdDisplay()
{  
	readtime();
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
	  	LCD1602_DisplayOneCharOnAddr(0,5,'0'+Time0[7]/16);
	    LCD1602_DisplayOneCharOnAddr(0,6,'0'+Time0[7]%16);
      LCD1602_DisplayOneCharOnAddr(0,7,('0'+Time0[6]/16));			//Äê
	   	LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,10,('0'+Time0[4]/16));			//ÔÂ
			LCD1602_DisplayOneCharOnAddr(0,11,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,13,('0'+Time0[3]/16));			//ÈÕ
			LCD1602_DisplayOneCharOnAddr(0,14,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,15,0x02);
			LCD1602_MoveToPosition(1,0);
      LCD1602_DisplayString(p1);
			LCD1602_DisplayOneCharOnAddr(1,8,('0'+Time0[2]/16));				//Ê±
			LCD1602_DisplayOneCharOnAddr(1,9,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,10,(':'));
			LCD1602_DisplayOneCharOnAddr(1,11,('0'+Time0[1]/16));				//·Ö
			LCD1602_DisplayOneCharOnAddr(1,12,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,13,(':'));
			LCD1602_DisplayOneCharOnAddr(1,14,('0'+Time0[0]/16));				//Ãë
			LCD1602_DisplayOneCharOnAddr(1,15,('0'+Time0[0]%16));
			
			
}
 
/*unsigned char checkday()                  //×Ô¶¯¼ì²éÊÇ·ñÊÇÈòÄê£¬²¢Îª¶ÔÓ¦µÄÔÂ·ÝÉèÖÃÌìÊý
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//ÅÐ¶ÏÊÇ·ñÊÇÈòÄê
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
 */

void timesetting()
{	 
	//LcdDisplay();
	//LCD1602_MoveToPosition(1,9);
	//LCD1602_DisplayString(p);
	switch(keyboardscan(1))
	{
			case 0x77:     //°´ÏÂ°´¼ü1£¬½øÈëµ÷ÊÔÄê
			{
				 LCD1602_MoveToPosition(0,9);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						   { year(0);break;}
							case 0x7e:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://°´ÏÂK2 ½øÈëµ÷ÊÔÔÂ
			{
				  LCD1602_MoveToPosition(0,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(100))
					  {
						  case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						      {   month(0);break;	}
						  case 0x7e:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xd7://°´ÏÂK3 ½øÈëµ÷ÊÔday
			{
				  LCD1602_MoveToPosition(0,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						      {  day(0);break;	}
						   case 0x7e://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xe7://°´ÏÂK4 ½øÈëµ÷ÊÔhour
			{
				  LCD1602_MoveToPosition(1,9);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						        {  hour(0);break;}
						    case 0x7e:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://°´ÏÂK5 ½øÈëµ÷ÊÔmin
			{
          LCD1602_MoveToPosition(1,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						       {  min(0);break;	}
						    case 0x7e:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://°´ÏÂK6 ½øÈëµ÷sec
			{
				  LCD1602_MoveToPosition(1,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0x7d://°´ÏÂK9£¬½øÈë¼Ó1
						       { sec(0);break;	}
						     case 0x7e:
						       {sec(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			 
   }
}


void LcdDisplayTemp0(int temp) 	 //lcdÏÔÊ¾
{
	unsigned char i; 
	unsigned char code  p2[]= "TEMP:";
   	float tp;  
	if(temp< 0)				//µ±ÎÂ¶ÈÖµÎª¸ºÊý
  	{
		DisplayData[0] = 0x40; 
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öÐ¡Êýµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊý×ª»»ÎªÕûÐÍµÄÊ±ºò°ÑÐ¡Êýµã
		//ºóÃæµÄÊý×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ð¡ÓÚ0.5µÄ¾Í
		//ËãÓÉÏ0.5£¬»¹ÊÇÔÚÐ¡ÊýµãºóÃæ¡£
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//ÒòÎªÊý¾Ý´¦ÀíÓÐÐ¡ÊýµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãÐÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕýµÄÄÇÃ´£¬ÄÇÃ´ÕýÊýµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öÐ¡Êýµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊý×ª»»ÎªÕûÐÍµÄÊ±ºò°ÑÐ¡Êýµã
		//ºóÃæµÄÊý×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ð¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚÐ¡ÊýµãºóÃæ¡£
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
		   LCD1602_WriteInformation(0x01,0);	//ÇåÆÁÖ¸Áî
	LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p2);
			 LCD1602_DisplayOneCharOnAddr(0,6,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(0,7,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(0,8,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(0,9,T[3]);
       LCD1602_DisplayOneCharOnAddr(0,10,0x06);	//¡ãc
}
 
 void delayA(unsigned int n)
{
	unsigned int t;
	for(t=5;t>0;t--)
	{
     while(n--){ NOP();}
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
  DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //¶ÔÊ±ÖÓ½øÐÐÉèÖÃ
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,9);           //ÔÚÄêµÄÊý×ÖÉÏÉÁË¸
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
	 LCD1602_MoveToPosition(0,12);
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
	LCD1602_MoveToPosition(0,15);
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
	  LCD1602_MoveToPosition(1,9);
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
  LCD1602_MoveToPosition(1,12);
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
	LCD1602_MoveToPosition(1,15);
  LCD1602_WriteInformation(0x0f,0);
}








/*******************************************************************************
* º¯ Êý Ãû         : Pcf8591SendByte
* º¯Êý¹¦ÄÜ		   : Ð´ÈëÒ»¸ö¿ØÖÆÃüÁî
* Êä    Èë         : channel£¨×ª»»Í¨µÀ£©
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void Pcf8591SendByte(unsigned char channel)
{	
	I2C_Start();
	I2C_SendByte(WRITEADDR, 1);    //·¢ËÍÐ´Æ÷¼þµØÖ·
	I2C_SendByte(0x40|channel, 0); //·¢ËÍ¿ØÖÆ¼Ä´æÆ÷
	I2C_Stop();
}
/*******************************************************************************
* º¯ Êý Ãû         : Pcf8591ReadByte
* º¯Êý¹¦ÄÜ		   : ¶ÁÈ¡Ò»¸ö×ª»»Öµ
* Êä    Èë         : ÎÞ
* Êä    ³ö         : dat
*******************************************************************************/

unsigned char Pcf8591ReadByte()
{
	unsigned char dat;
	I2C_Start();
	I2C_SendByte(READADDR, 1);//·¢ËÍ¶ÁÆ÷¼þµØÖ·
	dat=I2C_ReadByte();    //¶ÁÈ¡Êý¾Ý
	I2C_Stop();            //½áÊø×ÜÏß
    return dat;
}
/*******************************************************************************
* º¯ Êý Ãû         : Pcf8591DaConversion
* º¯Êý¹¦ÄÜ		   : PCF8591µÄÊä³ö¶ËÊä³öÄ£ÄâÁ¿
* Êä    Èë         : value£¨×ª»»µÄÊýÖµ£©
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void Pcf8591DaConversion(unsigned char value)
{
	I2C_Start();

	I2C_SendByte(WRITEADDR, 1);//·¢ËÍÐ´Æ÷¼þµØÖ·
	I2C_SendByte(0x40, 1);     //¿ªÆôDAÐ´µ½¿ØÖÆ¼Ä´æÆ÷
	I2C_SendByte(value, 0);    //·¢ËÍ×ª»»ÊýÖµ
	I2C_Stop();	
}

void pcf8591Play()
{unsigned char i,AD[5];
		unsigned int adNum;
	unsigned char code  p3[]="AD1:";
	float value;
		//--ÏÔÊ¾µçÎ»Æ÷µçÑ¹--//
		Pcf8591SendByte(0);             //·¢ËÍµçÎ»Æ÷×ª»»ÃüÁî

		adNum = Pcf8591ReadByte()*2;//½«×ª»»½á¹û¶Á×ß

		//--ÎÒÃÇ8591Ã¿¶ÁÈ¡µ½Ò»¸ö1¾Í±íÊ¾5/256V£¬ËùÒÔÒªÖªµÀµçÑ¹Öµ¾Í³ËÒÔ0.01953--//
		value = adNum / 2 * 0.01953;  //×ªÎªµçÑ¹Öµ
		adNum = value * 100;        //±£ÁôÁ½Î»Ð¡Êý
	LCD1602_MoveToPosition(1,0);
           LCD1602_DisplayString(p3);
	ADData[0] = adNum/ 10000;
	ADData[1] = adNum % 10000 / 1000;
	ADData[2] = adNum % 1000 / 100 ;
	ADData[3] = adNum % 100/10;
	ADData[4] = adNum % 10;
		for(i=0;i<=4;i++)
	{
		switch(Seg_Data[ADData[i]])
		{
			case 0xc0:
			{AD[i]=0x30;break;}
			case 0xF9:
			{AD[i]=0x31;break;}
			case 0xA4:
			{AD[i]=0x32;break;}
			case 0xB0:
			{AD[i]=0x33;break;}
			case 0x99:
			{AD[i]=0x34;break;}
			case 0x92:
			{	AD[i]=0x35;break;}
			case 0x82:
			{	AD[i]=0x30;break;}
			case 0xF8:
			{AD[i]=0x36;break;}
			case 0x80:
			{AD[i]=0x37;break;}
			case 0x90:
			{AD[i]=0x38;break;}			
		}
	}
	 LCD1602_DisplayOneCharOnAddr(1,6,AD[2]);
	  	LCD1602_DisplayOneCharOnAddr(1,8,AD[3]);
	  	LCD1602_DisplayOneCharOnAddr(1,7,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(1,9,AD[4]);
	LCD1602_DisplayOneCharOnAddr(1,10,'V');
			Pcf8591DaConversion(adNum/2); //DAC	  ÊýÄ£×ª»»
		
}

void RestTime()
{
	unsigned char timenum;
	if((Time0[2]/16)==0)
	{
		if((Time0[2]%16)==8)
		{  if(Time0[1]/16==0)
				   { if(Time0[1]==0)
				      {
		            	RELAY=0;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==0)
	{
		if((Time0[2]%16)==8)
		{  if(Time0[1]/16==5)
				   { if(Time0[1]==0)
				      {
		            	RELAY=1;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==0)
	{
		if((Time0[2]%16)==9)
		{  if(Time0[1]/16==0)
				   { if(Time0[1]==0)
				      {
		            	RELAY=0;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==0)
	{
		if((Time0[2]%16)==9)
		{  if(Time0[1]/16==5)
				   { if(Time0[1]==0)
				      {
		            	RELAY=1;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==1)
	{
		if((Time0[2]%16)==0)
		{  if(Time0[1]/16==1)
				   { if(Time0[1]==0)
				      {
		            	RELAY=0;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==1)
	{
		if((Time0[2]%16)==1)
		{  if(Time0[1]/16==0)
				   { if(Time0[1]==0)
				      {
		            	RELAY=1;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==1)
	{
		if((Time0[2]%16)==1)
		{  if(Time0[1]/16==1)
				   { if(Time0[1]==0)
				      {
		            	RELAY=0;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
		if((Time0[2]/16)==1)
	{
		if((Time0[2]%16)==2)
		{  if(Time0[1]/16==0)
				   { if(Time0[1]==0)
				      {
		            	RELAY=1;
		            	
                 timenum=0;   
	               timecount=0;
                 TR0=1;
									if(timecount==20)
									{
											RELAY=1;timecount=0;
									}
								}
							}
		}
	}
	
}



void time() interrupt 1     //????,
{
	TH0=0xd8;TL0=0xf0;
	timenum++;
	if(timenum==100)
	{ timenum=0;
		timecount++; 
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


void Delay1ms(uint y)
{
	uint x;
	for( ; y>0; y--)
	{
		for(x=110; x>0; x--);
	}
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20Init
* º¯Êý¹¦ÄÜ		   : ³õÊ¼»¯
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ³õÊ¼»¯³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
*******************************************************************************/

uchar Ds18b20Init()
{
	uchar i;
	DSPORT = 0;			 //½«×ÜÏßÀ­µÍ480us~960us
	i = 70;	
	while(i--);//ÑÓÊ±642us
	DSPORT = 1;			//È»ºóÀ­¸ß×ÜÏß£¬Èç¹ûDS18B20×ö³ö·´Ó¦»á½«ÔÚ15us~60usºó×ÜÏßÀ­µÍ
	i = 0;
	while(DSPORT)	//µÈ´ýDS18B20À­µÍ×ÜÏß
	{
		Delay1ms(1);
		i++;
		if(i>5)//µÈ´ý>5MS
		{
			return 0;//³õÊ¼»¯Ê§°Ü
		}
	
	}
	return 1;//³õÊ¼»¯³É¹¦
}

/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20WriteByte
* º¯Êý¹¦ÄÜ		   : Ïò18B20Ð´ÈëÒ»¸ö×Ö½Ú
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void Ds18b20WriteByte(uchar dat)
{
	uint i, j;

	for(j=0; j<8; j++)
	{
		DSPORT = 0;	     	  //Ã¿Ð´ÈëÒ»Î»Êý¾ÝÖ®Ç°ÏÈ°Ñ×ÜÏßÀ­µÍ1us
		i++;
		DSPORT = dat & 0x01;  //È»ºóÐ´ÈëÒ»¸öÊý¾Ý£¬´Ó×îµÍÎ»¿ªÊ¼
		i=6;
		while(i--); //ÑÓÊ±68us£¬³ÖÐøÊ±¼ä×îÉÙ60us
		DSPORT = 1;	//È»ºóÊÍ·Å×ÜÏß£¬ÖÁÉÙ1us¸ø×ÜÏß»Ö¸´Ê±¼ä²ÅÄÜ½Ó×ÅÐ´ÈëµÚ¶þ¸öÊýÖµ
		dat >>= 1;
	}
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadByte
* º¯Êý¹¦ÄÜ		   : ¶ÁÈ¡Ò»¸ö×Ö½Ú
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/


uchar Ds18b20ReadByte()
{
	uchar byte, bi;
	uint i, j;	
	for(j=8; j>0; j--)
	{
		DSPORT = 0;//ÏÈ½«×ÜÏßÀ­µÍ1us
		i++;
		DSPORT = 1;//È»ºóÊÍ·Å×ÜÏß
		i++;
		i++;//ÑÓÊ±6usµÈ´ýÊý¾ÝÎÈ¶¨
		bi = DSPORT;	 //¶ÁÈ¡Êý¾Ý£¬´Ó×îµÍÎ»¿ªÊ¼¶ÁÈ¡
		/*½«byte×óÒÆÒ»Î»£¬È»ºóÓëÉÏÓÒÒÆ7Î»ºóµÄbi£¬×¢ÒâÒÆ¶¯Ö®ºóÒÆµôÄÇÎ»²¹0¡£*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;		//¶ÁÈ¡ÍêÖ®ºóµÈ´ý48usÔÙ½Ó×Å¶ÁÈ¡ÏÂÒ»¸öÊý
		while(i--);
	}				
	return byte;
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ChangTemp
* º¯Êý¹¦ÄÜ		   : ÈÃ18b20¿ªÊ¼×ª»»ÎÂ¶È
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//Ìø¹ýROM²Ù×÷ÃüÁî		 
	Ds18b20WriteByte(0x44);	    //ÎÂ¶È×ª»»ÃüÁî
//	Delay1ms(100);	//µÈ´ý×ª»»³É¹¦£¬¶øÈç¹ûÄãÊÇÒ»Ö±Ë¢×ÅµÄ»°£¬¾Í²»ÓÃÕâ¸öÑÓÊ±ÁË
   
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadTempCom
* º¯Êý¹¦ÄÜ		   : ·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //Ìø¹ýROM²Ù×÷ÃüÁî
	Ds18b20WriteByte(0xbe);	 //·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
}
/*******************************************************************************
* º¯ Êý Ãû         : Ds18b20ReadTemp
* º¯Êý¹¦ÄÜ		   : ¶ÁÈ¡ÎÂ¶È
* Êä    Èë         : com
* Êä    ³ö         : ÎÞ
*******************************************************************************/

int Ds18b20ReadTemp()
{
	int temp = 0;
	uchar tmh, tml;
	Ds18b20ChangTemp();			 	//ÏÈÐ´Èë×ª»»ÃüÁî
	Ds18b20ReadTempCom();			//È»ºóµÈ´ý×ª»»Íêºó·¢ËÍ¶ÁÈ¡ÎÂ¶ÈÃüÁî
	tml = Ds18b20ReadByte();		//¶ÁÈ¡ÎÂ¶ÈÖµ¹²16Î»£¬ÏÈ¶ÁµÍ×Ö½Ú
	tmh = Ds18b20ReadByte();		//ÔÙ¶Á¸ß×Ö½Ú
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}
