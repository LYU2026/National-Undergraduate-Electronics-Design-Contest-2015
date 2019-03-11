#include<reg52.h>
//#include"temp.h"
//#include"lcd.h"
//#include"pcf8591.h"
#include"ds1302.h"
#include"i2c.h"
#include<intrins.h>


//--����PCF8591�Ķ�д��ַ--//
#define  WRITEADDR 0x90	   //д��ַ
#define  READADDR  0x91	   //����ַ

//--����ȫ�ֱ���--//
unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0��1��2��3��4��5��6��7��8��9��A��b��C��d��E��F����ʾ��
unsigned char DisplayData[8];
//�������Ҫ��ʾ��8λ����ֵ

//--����ȫ�ֺ���--//
void DigDisplay(); //��̬��ʾ����
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
�������ƣ�Delay
�������ܣ���ʱ����
��ڲ�����uiCount-��ʱ������ÿ��1����0.5ms
����ֵ����
��ע����
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
										 
sbit LCD1602_RS = P2^6;	//λ���壬Һ��������/����ѡ��
sbit LCD1602_RW = P2^5;	//λ���壬Һ���Ķ�дѡ��
sbit LCD1602_EN = P2^7;	//λ���壬Һ��ʹ���ź�

#define LCDPORT P0	  //Һ�������ݿ�
unsigned char  code Data[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//��
														0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,//��
														0x1f,0x11,0x11,0x1f,0x11,0x11,0xff,0x00,//��
														0x0e,0x0a,0x0e,0x00,0x1f,0x11,0x11,0x1f,//��
	                          0x04,0x1f,0x08,0x1f,0x09,0x0f,0x09,0x09,
														0x02,0x02,0x1f,0x06,0x0a,0x12,0x02,0x06,
	                          0x1c,0x14,0x1c,0x03,0x04,0x04,0x04,0x03//��c
               };

/******************************************************************************
�������ƣ�LCD1602_CheckBusy
�������ܣ�æ���
��ڲ�������
����ֵ����
��ע��æ�������˶��һ����ʱ�����İ취�����ó����ڼ�⺯����
*******************************************************************************/
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //��֮ǰ����λ��׼����ȡIO������
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //ʹҺ�����ڶ�����״̬
	LCD1602_EN = 1;	 //ʹ��Һ�����ߵ�ƽ��Ч
	while((i--) && (LCDPORT & 0x80));	//æ���
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_WriteInformation
�������ܣ���LCD1602Һ��д�����ݻ�������
��ڲ�����ucData-Ҫд������ݻ����������
		  bComOrData-Ϊ0ʱд���ʱ���Ϊ1ʱд���ʱ����
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //��д�����ݻ�������ǰ�Ƚ���æ���
	LCDPORT = ucData;		 //�Ƚ����ݻ�����������IO
	LCD1602_RS = bComOrData;	//ȷ����д�����ݻ���д����
	LCD1602_RW = 0;		//ʹҺ������д����Ϣ״̬
	LCD1602_EN = 1;		//ʹ��Һ�����ߵ�ƽ��Ч
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_Init
�������ܣ�Һ����ʼ������
��ڲ�������
����ֵ����
��ע����
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
	LCD1602_WriteInformation(0x38,0); 	//д�����5x7��������ʽ��8λ���ݽӿ�
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//��ʾ���ã�����ʾ����겻��ʾ������˸//        ��Ϊ0ʱ��
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//����ָ��
	Delay(20);
	LCD1602_WriteInformation(0x40,0);	
	for(i=0;i<64;i++)
	{
			LCD1602_WriteInformation(Data[i],1);	
	}
}
  
/******************************************************************************
�������ƣ�LCD1602_MoveToPosition
�������ܣ���Һ���Ĺ���ƶ���ָ����λ��
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
����ֵ����
��ע��ͨ��ָ��x,y��ֵ���Խ�Һ���Ĺ���ƶ���ָ����λ��
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //��궨λ����һ�е�y��
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //��궨�嵽�ڶ��е�y��
}

/******************************************************************************
�������ƣ�LCD1602_DisplayOneCharOnAddr
�������ܣ���ָ����λ������ʾָ�����ַ�
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
		  ucData-Ҫ��ʾ���ַ�����
����ֵ����
��ע��ȷ��x,y��ȡֵҪ��ָ���ķ�Χ��
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //���λ��
	LCD1602_WriteInformation(ucData,1);	  //д������
}

/******************************************************************************
�������ƣ�LCD1602_DisplayString
�������ܣ���ʾ�ַ���
��ڲ�����ucStr-�ַ������׵�ַ
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //�ַ�������֮ǰ��ѭ����ʾ
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //����д��ÿһ���ַ�
		 ucStr++;								 //ָ������
	}
}

/******************************************************************************
�������ƣ�Delay
�������ܣ���ʱ����
��ڲ�����uiCount-��ʱ������ÿ��1����0.5ms
����ֵ����
��ע����
*******************************************************************************/

#ifndef __TEMP_H_
#define __TEMP_H_

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--����ʹ�õ�IO��--//
sbit DSPORT=P3^7;

//--����ȫ�ֺ���--//
 void Delay1ms(uint );
uchar Ds18b20Init();
 void Ds18b20WriteByte(uchar com);
uchar Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();
//sbit LCD1602_RS = P2^6;	//λ���壬Һ��������/����ѡ��
//sbit LCD1602_RW = P2^5;	//λ���壬Һ���Ķ�дѡ��
//sbit LCD1602_EN = P2^7;	//λ���壬Һ��ʹ���ź�

#define LCDPORT P0	  //Һ�������ݿ�

//unsigned char code ucForum[]=" bbs.cepark.com ";	  //��CODE������һ��������ʾ�ĳ����ַ���

/*extern void Delay(unsigned int uiCount);	//��ʱ����
extern void LCD1602_CheckBusy(void);	//Һ��æ���
extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
extern void LCD1602_Init();	//Һ����ʼ��
extern void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Һ���������ƶ���ָ��λ��
extern void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//��Һ��ָ��λ����ʾ�ַ�
extern void LCD1602_DisplayString(unsigned char *ucStr);	//��Һ������ʾ�ַ���
*/
//extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
void Pcf8591SendByte(unsigned char channel);
unsigned char Pcf8591ReadByte();
void Pcf8591DaConversion(unsigned char value);
void pcf8591Play();
extern   int  Ds18b20ReadTemp();  //��ȡ�¶�
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[5],ADData[8];
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//��������ܵı��룬�������ݶ�����CODE�� 
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
void year(unsigned char k);//��Ӽ�
void month(unsigned char k);//�¼Ӽ�
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void weekset(unsigned char k);//�ܼӼ�
void timesetting();//����������
void readtime();  //��ȡDS13B20�е�ʱ��

#define  NOP()   _nop_() 
sbit RELAY = P3^0;	
sbit KEY=P3^1;
int num1 =0;
sbit Delay1=P3^2;
sbit bee=P3^3;
void readtime();  //��ȡDS13B20�е�ʱ��
unsigned char  timecount=0;
unsigned char T[5];
unsigned char dat;	
unsigned char timedata[]={8,14,50};//ʱ����
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
//void Delay(unsigned int n);	//��ʱ����
void delay1msA();

void daydata()   //������������
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

unsigned char checkday()                  //�Զ�����Ƿ������꣬��Ϊ��Ӧ���·���������
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//�ж��Ƿ�������
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
	   
		
	  // LCD1602_WriteInformation(0x01,0);	//����ָ��
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
					 LcdDisplayTemp0(Ds18b20ReadTemp());//��ʾ�¶�
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
      LCD1602_DisplayOneCharOnAddr(0,7,('0'+Time0[6]/16));			//��
	   	LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,10,('0'+Time0[4]/16));			//��
			LCD1602_DisplayOneCharOnAddr(0,11,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,13,('0'+Time0[3]/16));			//��
			LCD1602_DisplayOneCharOnAddr(0,14,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,15,0x02);
			LCD1602_MoveToPosition(1,0);
      LCD1602_DisplayString(p1);
			LCD1602_DisplayOneCharOnAddr(1,8,('0'+Time0[2]/16));				//ʱ
			LCD1602_DisplayOneCharOnAddr(1,9,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,10,(':'));
			LCD1602_DisplayOneCharOnAddr(1,11,('0'+Time0[1]/16));				//��
			LCD1602_DisplayOneCharOnAddr(1,12,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,13,(':'));
			LCD1602_DisplayOneCharOnAddr(1,14,('0'+Time0[0]/16));				//��
			LCD1602_DisplayOneCharOnAddr(1,15,('0'+Time0[0]%16));
			
			
}
 
/*unsigned char checkday()                  //�Զ�����Ƿ������꣬��Ϊ��Ӧ���·���������
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//�ж��Ƿ�������
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
			case 0x77:     //���°���1�����������
			{
				 LCD1602_MoveToPosition(0,9);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0x7d://����K9�������1
						   { year(0);break;}
							case 0x7e:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://����K2 ���������
			{
				  LCD1602_MoveToPosition(0,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(100))
					  {
						  case 0x7d://����K9�������1
						      {   month(0);break;	}
						  case 0x7e:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xd7://����K3 �������day
			{
				  LCD1602_MoveToPosition(0,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0x7d://����K9�������1
						      {  day(0);break;	}
						   case 0x7e://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xe7://����K4 �������hour
			{
				  LCD1602_MoveToPosition(1,9);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://����K9�������1
						        {  hour(0);break;}
						    case 0x7e:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://����K5 �������min
			{
          LCD1602_MoveToPosition(1,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://����K9�������1
						       {  min(0);break;	}
						    case 0x7e:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://����K6 �����sec
			{
				  LCD1602_MoveToPosition(1,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0x7d://����K9�������1
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


void LcdDisplayTemp0(int temp) 	 //lcd��ʾ
{
	unsigned char i; 
	unsigned char code  p2[]= "TEMP:";
   	float tp;  
	if(temp< 0)				//���¶�ֵΪ����
  	{
		DisplayData[0] = 0x40; 
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
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
		   LCD1602_WriteInformation(0x01,0);	//����ָ��
	LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p2);
			 LCD1602_DisplayOneCharOnAddr(0,6,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(0,7,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(0,8,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(0,9,T[3]);
       LCD1602_DisplayOneCharOnAddr(0,10,0x06);	//��c
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
	if(k==0)                //��һ
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
	if(k==1)                 //��һ
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
	DS1302_SetInit(Time0);                 //��ʱ�ӽ�������
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,9);           //�������������˸
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
* �� �� ��         : Pcf8591SendByte
* ��������		   : д��һ����������
* ��    ��         : channel��ת��ͨ����
* ��    ��         : ��
*******************************************************************************/

void Pcf8591SendByte(unsigned char channel)
{	
	I2C_Start();
	I2C_SendByte(WRITEADDR, 1);    //����д������ַ
	I2C_SendByte(0x40|channel, 0); //���Ϳ��ƼĴ���
	I2C_Stop();
}
/*******************************************************************************
* �� �� ��         : Pcf8591ReadByte
* ��������		   : ��ȡһ��ת��ֵ
* ��    ��         : ��
* ��    ��         : dat
*******************************************************************************/

unsigned char Pcf8591ReadByte()
{
	unsigned char dat;
	I2C_Start();
	I2C_SendByte(READADDR, 1);//���Ͷ�������ַ
	dat=I2C_ReadByte();    //��ȡ����
	I2C_Stop();            //��������
    return dat;
}
/*******************************************************************************
* �� �� ��         : Pcf8591DaConversion
* ��������		   : PCF8591����������ģ����
* ��    ��         : value��ת������ֵ��
* ��    ��         : ��
*******************************************************************************/

void Pcf8591DaConversion(unsigned char value)
{
	I2C_Start();

	I2C_SendByte(WRITEADDR, 1);//����д������ַ
	I2C_SendByte(0x40, 1);     //����DAд�����ƼĴ���
	I2C_SendByte(value, 0);    //����ת����ֵ
	I2C_Stop();	
}

void pcf8591Play()
{unsigned char i,AD[5];
		unsigned int adNum;
	unsigned char code  p3[]="AD1:";
	float value;
		//--��ʾ��λ����ѹ--//
		Pcf8591SendByte(0);             //���͵�λ��ת������

		adNum = Pcf8591ReadByte()*2;//��ת���������

		//--����8591ÿ��ȡ��һ��1�ͱ�ʾ5/256V������Ҫ֪����ѹֵ�ͳ���0.01953--//
		value = adNum / 2 * 0.01953;  //תΪ��ѹֵ
		adNum = value * 100;        //������λС��
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
			Pcf8591DaConversion(adNum/2); //DAC	  ��ģת��
		
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
* �� �� ��         : Ds18b20Init
* ��������		   : ��ʼ��
* ��    ��         : ��
* ��    ��         : ��ʼ���ɹ�����1��ʧ�ܷ���0
*******************************************************************************/

uchar Ds18b20Init()
{
	uchar i;
	DSPORT = 0;			 //����������480us~960us
	i = 70;	
	while(i--);//��ʱ642us
	DSPORT = 1;			//Ȼ���������ߣ����DS18B20������Ӧ�Ὣ��15us~60us����������
	i = 0;
	while(DSPORT)	//�ȴ�DS18B20��������
	{
		Delay1ms(1);
		i++;
		if(i>5)//�ȴ�>5MS
		{
			return 0;//��ʼ��ʧ��
		}
	
	}
	return 1;//��ʼ���ɹ�
}

/*******************************************************************************
* �� �� ��         : Ds18b20WriteByte
* ��������		   : ��18B20д��һ���ֽ�
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/

void Ds18b20WriteByte(uchar dat)
{
	uint i, j;

	for(j=0; j<8; j++)
	{
		DSPORT = 0;	     	  //ÿд��һλ����֮ǰ�Ȱ���������1us
		i++;
		DSPORT = dat & 0x01;  //Ȼ��д��һ�����ݣ������λ��ʼ
		i=6;
		while(i--); //��ʱ68us������ʱ������60us
		DSPORT = 1;	//Ȼ���ͷ����ߣ�����1us�����߻ָ�ʱ����ܽ���д��ڶ�����ֵ
		dat >>= 1;
	}
}
/*******************************************************************************
* �� �� ��         : Ds18b20ReadByte
* ��������		   : ��ȡһ���ֽ�
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/


uchar Ds18b20ReadByte()
{
	uchar byte, bi;
	uint i, j;	
	for(j=8; j>0; j--)
	{
		DSPORT = 0;//�Ƚ���������1us
		i++;
		DSPORT = 1;//Ȼ���ͷ�����
		i++;
		i++;//��ʱ6us�ȴ������ȶ�
		bi = DSPORT;	 //��ȡ���ݣ������λ��ʼ��ȡ
		/*��byte����һλ��Ȼ����������7λ���bi��ע���ƶ�֮���Ƶ���λ��0��*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;		//��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
		while(i--);
	}				
	return byte;
}
/*******************************************************************************
* �� �� ��         : Ds18b20ChangTemp
* ��������		   : ��18b20��ʼת���¶�
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/

void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//����ROM��������		 
	Ds18b20WriteByte(0x44);	    //�¶�ת������
//	Delay1ms(100);	//�ȴ�ת���ɹ������������һֱˢ�ŵĻ����Ͳ��������ʱ��
   
}
/*******************************************************************************
* �� �� ��         : Ds18b20ReadTempCom
* ��������		   : ���Ͷ�ȡ�¶�����
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //����ROM��������
	Ds18b20WriteByte(0xbe);	 //���Ͷ�ȡ�¶�����
}
/*******************************************************************************
* �� �� ��         : Ds18b20ReadTemp
* ��������		   : ��ȡ�¶�
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/

int Ds18b20ReadTemp()
{
	int temp = 0;
	uchar tmh, tml;
	Ds18b20ChangTemp();			 	//��д��ת������
	Ds18b20ReadTempCom();			//Ȼ��ȴ�ת������Ͷ�ȡ�¶�����
	tml = Ds18b20ReadByte();		//��ȡ�¶�ֵ��16λ���ȶ����ֽ�
	tmh = Ds18b20ReadByte();		//�ٶ����ֽ�
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}
