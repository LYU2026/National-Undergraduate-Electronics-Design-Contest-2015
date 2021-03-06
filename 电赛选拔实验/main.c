#include<reg52.h>
//#include"temp.h"
//#include"lcd.h"
//#include"pcf8591.h"
#include"ds1302.h"
#include"i2c.h"
#include<intrins.h>


//--定义PCF8591的读写地址--//
#define  WRITEADDR 0x90	   //写地址
#define  READADDR  0x91	   //读地址

//--定义全局变量--//
unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char DisplayData[8];
//用来存放要显示的8位数的值

//--声明全局函数--//
void DigDisplay(); //动态显示函数
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
函数名称：Delay
函数功能：延时函数
入口参数：uiCount-延时参数，每加1增加0.5ms
返回值：无
备注：无
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
										 
sbit LCD1602_RS = P2^6;	//位定义，液晶的数据/命令选择
sbit LCD1602_RW = P2^5;	//位定义，液晶的读写选择
sbit LCD1602_EN = P2^7;	//位定义，液晶使能信号

#define LCDPORT P0	  //液晶的数据口
unsigned char  code Data[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//年
														0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,//月
														0x1f,0x11,0x11,0x1f,0x11,0x11,0xff,0x00,//日
														0x0e,0x0a,0x0e,0x00,0x1f,0x11,0x11,0x1f,//吕
	                          0x04,0x1f,0x08,0x1f,0x09,0x0f,0x09,0x09,
														0x02,0x02,0x1f,0x06,0x0a,0x12,0x02,0x06,
	                          0x1c,0x14,0x1c,0x03,0x04,0x04,0x04,0x03//°c
               };

/******************************************************************************
函数名称：LCD1602_CheckBusy
函数功能：忙检测
入口参数：无
返回值：无
备注：忙检测采用了多加一个延时条件的办法，放置程序卡在检测函数中
*******************************************************************************/
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //读之前先置位，准备读取IO口数据
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //使液晶处于读数据状态
	LCD1602_EN = 1;	 //使能液晶，高电平有效
	while((i--) && (LCDPORT & 0x80));	//忙检测
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_WriteInformation
函数功能：向LCD1602液晶写入数据或者命令
入口参数：ucData-要写入的数据或者命令参数
		  bComOrData-为0时写入的时命令，为1时写入的时数据
返回值：无
备注：无
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //在写入数据或者命令前先进行忙检测
	LCDPORT = ucData;		 //先将数据或者命令送至IO
	LCD1602_RS = bComOrData;	//确定是写入数据还是写命令
	LCD1602_RW = 0;		//使液晶处于写入信息状态
	LCD1602_EN = 1;		//使能液晶，高电平有效
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_Init
函数功能：液晶初始化函数
入口参数：无
返回值：无
备注：无
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
	LCD1602_WriteInformation(0x38,0); 	//写入命令，5x7点阵工作方式，8位数据接口
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//显示设置，开显示，光标不显示，不闪烁//        （为0时否）
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//清屏指令
	Delay(20);
	LCD1602_WriteInformation(0x40,0);	
	for(i=0;i<64;i++)
	{
			LCD1602_WriteInformation(Data[i],1);	
	}
}
  
/******************************************************************************
函数名称：LCD1602_MoveToPosition
函数功能：将液晶的光标移动到指定的位置
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
返回值：无
备注：通过指定x,y的值可以将液晶的光标移动到指定的位置
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //光标定位到第一行的y列
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //光标定义到第二行的y列
}

/******************************************************************************
函数名称：LCD1602_DisplayOneCharOnAddr
函数功能：在指定的位置上显示指定的字符
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
		  ucData-要显示的字符数据
返回值：无
备注：确保x,y的取值要在指定的范围内
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //光标位置
	LCD1602_WriteInformation(ucData,1);	  //写入数据
}

/******************************************************************************
函数名称：LCD1602_DisplayString
函数功能：显示字符串
入口参数：ucStr-字符串的首地址
返回值：无
备注：无
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //字符串结束之前，循环显示
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //依次写入每一个字符
		 ucStr++;								 //指针增加
	}
}

/******************************************************************************
函数名称：Delay
函数功能：延时函数
入口参数：uiCount-延时参数，每加1增加0.5ms
返回值：无
备注：无
*******************************************************************************/

#ifndef __TEMP_H_
#define __TEMP_H_

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--定义使用的IO口--//
sbit DSPORT=P3^7;

//--声明全局函数--//
 void Delay1ms(uint );
uchar Ds18b20Init();
 void Ds18b20WriteByte(uchar com);
uchar Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();
//sbit LCD1602_RS = P2^6;	//位定义，液晶的数据/命令选择
//sbit LCD1602_RW = P2^5;	//位定义，液晶的读写选择
//sbit LCD1602_EN = P2^7;	//位定义，液晶使能信号

#define LCDPORT P0	  //液晶的数据口

//unsigned char code ucForum[]=" bbs.cepark.com ";	  //在CODE区定义一个用于显示的常量字符串

/*extern void Delay(unsigned int uiCount);	//延时函数
extern void LCD1602_CheckBusy(void);	//液晶忙检测
extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//在液晶上写数据或者写命令，0为命令，1为数据
extern void LCD1602_Init();	//液晶初始化
extern void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//液晶的坐标移动到指定位置
extern void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//在液晶指定位置显示字符
extern void LCD1602_DisplayString(unsigned char *ucStr);	//在液晶上显示字符串
*/
//extern void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//在液晶上写数据或者写命令，0为命令，1为数据
void Pcf8591SendByte(unsigned char channel);
unsigned char Pcf8591ReadByte();
void Pcf8591DaConversion(unsigned char value);
void pcf8591Play();
extern   int  Ds18b20ReadTemp();  //读取温度
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[5],ADData[8];
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//共阳数码管的编码，并将数据定义在CODE区 
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
void year(unsigned char k);//年加减
void month(unsigned char k);//月加减
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void weekset(unsigned char k);//周加减
void timesetting();//电子钟设置
void readtime();  //读取DS13B20中的时间

#define  NOP()   _nop_() 
sbit RELAY = P3^0;	
sbit KEY=P3^1;
int num1 =0;
sbit Delay1=P3^2;
sbit bee=P3^3;
void readtime();  //读取DS13B20中的时间
unsigned char  timecount=0;
unsigned char T[5];
unsigned char dat;	
unsigned char timedata[]={8,14,50};//时分秒
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
//void Delay(unsigned int n);	//延时函数
void delay1msA();

void daydata()   //设置天数函数
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

unsigned char checkday()                  //自动检查是否是闰年，并为对应的月份设置天数
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//判断是否是闰年
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
	   
		
	  // LCD1602_WriteInformation(0x01,0);	//清屏指令
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
					 LcdDisplayTemp0(Ds18b20ReadTemp());//显示温度
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
      LCD1602_DisplayOneCharOnAddr(0,7,('0'+Time0[6]/16));			//年
	   	LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,10,('0'+Time0[4]/16));			//月
			LCD1602_DisplayOneCharOnAddr(0,11,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,13,('0'+Time0[3]/16));			//日
			LCD1602_DisplayOneCharOnAddr(0,14,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,15,0x02);
			LCD1602_MoveToPosition(1,0);
      LCD1602_DisplayString(p1);
			LCD1602_DisplayOneCharOnAddr(1,8,('0'+Time0[2]/16));				//时
			LCD1602_DisplayOneCharOnAddr(1,9,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,10,(':'));
			LCD1602_DisplayOneCharOnAddr(1,11,('0'+Time0[1]/16));				//分
			LCD1602_DisplayOneCharOnAddr(1,12,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,13,(':'));
			LCD1602_DisplayOneCharOnAddr(1,14,('0'+Time0[0]/16));				//秒
			LCD1602_DisplayOneCharOnAddr(1,15,('0'+Time0[0]%16));
			
			
}
 
/*unsigned char checkday()                  //自动检查是否是闰年，并为对应的月份设置天数
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//判断是否是闰年
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
			case 0x77:     //按下按键1，进入调试年
			{
				 LCD1602_MoveToPosition(0,9);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0x7d://按下K9，进入加1
						   { year(0);break;}
							case 0x7e:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://按下K2 进入调试月
			{
				  LCD1602_MoveToPosition(0,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(100))
					  {
						  case 0x7d://按下K9，进入加1
						      {   month(0);break;	}
						  case 0x7e:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xd7://按下K3 进入调试day
			{
				  LCD1602_MoveToPosition(0,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0x7d://按下K9，进入加1
						      {  day(0);break;	}
						   case 0x7e://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xe7://按下K4 进入调试hour
			{
				  LCD1602_MoveToPosition(1,9);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://按下K9，进入加1
						        {  hour(0);break;}
						    case 0x7e:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://按下K5 进入调试min
			{
          LCD1602_MoveToPosition(1,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://按下K9，进入加1
						       {  min(0);break;	}
						    case 0x7e:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://按下K6 进入调sec
			{
				  LCD1602_MoveToPosition(1,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0x7d://按下K9，进入加1
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


void LcdDisplayTemp0(int temp) 	 //lcd显示
{
	unsigned char i; 
	unsigned char code  p2[]= "TEMP:";
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x40; 
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由�0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
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
		   LCD1602_WriteInformation(0x01,0);	//清屏指令
	LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p2);
			 LCD1602_DisplayOneCharOnAddr(0,6,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(0,7,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(0,8,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(0,9,T[3]);
       LCD1602_DisplayOneCharOnAddr(0,10,0x06);	//°c
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
	if(k==0)                //加一
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
	if(k==1)                 //减一
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
	DS1302_SetInit(Time0);                 //对时钟进行设置
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,9);           //在年的数字上闪烁
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
* 函 数 名         : Pcf8591SendByte
* 函数功能		   : 写入一个控制命令
* 输    入         : channel（转换通道）
* 输    出         : 无
*******************************************************************************/

void Pcf8591SendByte(unsigned char channel)
{	
	I2C_Start();
	I2C_SendByte(WRITEADDR, 1);    //发送写器件地址
	I2C_SendByte(0x40|channel, 0); //发送控制寄存器
	I2C_Stop();
}
/*******************************************************************************
* 函 数 名         : Pcf8591ReadByte
* 函数功能		   : 读取一个转换值
* 输    入         : 无
* 输    出         : dat
*******************************************************************************/

unsigned char Pcf8591ReadByte()
{
	unsigned char dat;
	I2C_Start();
	I2C_SendByte(READADDR, 1);//发送读器件地址
	dat=I2C_ReadByte();    //读取数据
	I2C_Stop();            //结束总线
    return dat;
}
/*******************************************************************************
* 函 数 名         : Pcf8591DaConversion
* 函数功能		   : PCF8591的输出端输出模拟量
* 输    入         : value（转换的数值）
* 输    出         : 无
*******************************************************************************/

void Pcf8591DaConversion(unsigned char value)
{
	I2C_Start();

	I2C_SendByte(WRITEADDR, 1);//发送写器件地址
	I2C_SendByte(0x40, 1);     //开启DA写到控制寄存器
	I2C_SendByte(value, 0);    //发送转换数值
	I2C_Stop();	
}

void pcf8591Play()
{unsigned char i,AD[5];
		unsigned int adNum;
	unsigned char code  p3[]="AD1:";
	float value;
		//--显示电位器电压--//
		Pcf8591SendByte(0);             //发送电位器转换命令

		adNum = Pcf8591ReadByte()*2;//将转换结果读走

		//--我们8591每读取到一个1就表示5/256V，所以要知道电压值就乘以0.01953--//
		value = adNum / 2 * 0.01953;  //转为电压值
		adNum = value * 100;        //保留两位小数
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
			Pcf8591DaConversion(adNum/2); //DAC	  数模转换
		
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
* 函 数 名         : Ds18b20Init
* 函数功能		   : 初始化
* 输    入         : 无
* 输    出         : 初始化成功返回1，失败返回0
*******************************************************************************/

uchar Ds18b20Init()
{
	uchar i;
	DSPORT = 0;			 //将总线拉低480us~960us
	i = 70;	
	while(i--);//延时642us
	DSPORT = 1;			//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	i = 0;
	while(DSPORT)	//等待DS18B20拉低总线
	{
		Delay1ms(1);
		i++;
		if(i>5)//等待>5MS
		{
			return 0;//初始化失败
		}
	
	}
	return 1;//初始化成功
}

/*******************************************************************************
* 函 数 名         : Ds18b20WriteByte
* 函数功能		   : 向18B20写入一个字节
* 输    入         : com
* 输    出         : 无
*******************************************************************************/

void Ds18b20WriteByte(uchar dat)
{
	uint i, j;

	for(j=0; j<8; j++)
	{
		DSPORT = 0;	     	  //每写入一位数据之前先把总线拉低1us
		i++;
		DSPORT = dat & 0x01;  //然后写入一个数据，从最低位开始
		i=6;
		while(i--); //延时68us，持续时间最少60us
		DSPORT = 1;	//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadByte
* 函数功能		   : 读取一个字节
* 输    入         : com
* 输    出         : 无
*******************************************************************************/


uchar Ds18b20ReadByte()
{
	uchar byte, bi;
	uint i, j;	
	for(j=8; j>0; j--)
	{
		DSPORT = 0;//先将总线拉低1us
		i++;
		DSPORT = 1;//然后释放总线
		i++;
		i++;//延时6us等待数据稳定
		bi = DSPORT;	 //读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (bi << 7);						  
		i = 4;		//读取完之后等待48us再接着读取下一个数
		while(i--);
	}				
	return byte;
}
/*******************************************************************************
* 函 数 名         : Ds18b20ChangTemp
* 函数功能		   : 让18b20开始转换温度
* 输    入         : com
* 输    出         : 无
*******************************************************************************/

void  Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
//	Delay1ms(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
   
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadTempCom
* 函数功能		   : 发送读取温度命令
* 输    入         : com
* 输    出         : 无
*******************************************************************************/

void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xbe);	 //发送读取温度命令
}
/*******************************************************************************
* 函 数 名         : Ds18b20ReadTemp
* 函数功能		   : 读取温度
* 输    入         : com
* 输    出         : 无
*******************************************************************************/

int Ds18b20ReadTemp()
{
	int temp = 0;
	uchar tmh, tml;
	Ds18b20ChangTemp();			 	//先写入转换命令
	Ds18b20ReadTempCom();			//然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadByte();		//读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte();		//再读高字节
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}
