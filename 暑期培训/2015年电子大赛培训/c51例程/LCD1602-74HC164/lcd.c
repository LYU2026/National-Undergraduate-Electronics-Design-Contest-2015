#include <reg52.h>

#include "lcd.h"
#include "74hc164.h"
#include "74hc165.h"
/**********************************
PIN口定义
**********************************/
sbit LCD_E=P1^2;
sbit LCD_RW=P1^1;
sbit LCD_RS=P1^0;

unsigned char lcd_buffer[]={"0123456789ABCDEF"};

unsigned char code ascii[]={"0123456789abcdef"};

/*******************************************************************************
* 函 数 名         : Lcd1602_Delay1ms
* 函数功能		   : 延时函数，延时1ms
* 输    入         : c
* 输    出         : 无
* 说    名         : 该函数是在12MHZ晶振下，12分频单片机的延时。
*******************************************************************************/

void Lcd1602_Delay1ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
}

/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : com
* 输    出         : 无
*******************************************************************************/
void LcdWriteCom(unsigned char com)	  //写入命令
{	
	unsigned char x;
	LCD_E = 0;	 //使能清零
	LCD_RS = 0;	 //选择写入命令
	LCD_RW = 0;	 //选择写入

	send_data(com);	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	x++;
	LCD_E = 1;	 //写入时序
	x++;
	LCD_E = 0;
	Lcd1602_Delay1ms(1);
}
/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/		   
void LcdWriteData(unsigned char dat)			//写入数据
{
	unsigned char x;
	LCD_E = 0;	  //使能清零
	LCD_RS = 1;	  //选择写入数据
	LCD_RW = 0;	  //选择写入

	send_data(dat);	
	x++;
	LCD_E = 1;	  //写入时序
	x++;
	LCD_E = 0;
	Lcd1602_Delay1ms(1);
}
/*******************************************************************************
* 函 数 名       : LcdInit()
* 函数功能		 : 初始化LCD屏
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/		   
void LcdInit()						  //LCD初始化子程序
{
    unsigned char code zifu[] ={0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,0x02,   		//年  ascii码=0x00
						  		0x0F,0x09,0x0F,0x09,0x0F,0x09,0x11,0x00,	 	//月  ascii码=0x01
						  		0x00,0x0F,0x09,0x0F,0x09,0x0F,0x00,0x00,	 	//日  ascii码=0x01
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char i;

	LcdWriteCom(0x38);	 
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	Lcd1602_Delay1ms(5);

	LcdWriteCom(0x40);	 

    for(i=0;i<64;i++)
	{
		LcdWriteData(zifu[i]);
	}
}

void disp_line(unsigned char line)   //line=0,第1行显示；line=1,第2行显示
{
	unsigned char i;
	if(line==0)
	{
		LcdWriteCom(0x80+0x00);  //
	}
	else
	{
		LcdWriteCom(0x80+0x40);  //
	}
	for(i=0;i<16;i++)
	{
		LcdWriteData(lcd_buffer[i]);
	}
}

void disp_demo()
{
	unsigned char i,j;
	unsigned char x=0;

	for(j=0;j<8;j++)
	{ 
    	LcdWriteCom(0x80+0x00);
		for(i=0;i<16;i++)
		{
			LcdWriteData(x++);
		}
	    LcdWriteCom(0x80+0x40); 
		for(i=0;i<16;i++)
		{
			LcdWriteData(x++);
		}
	    Lcd1602_Delay1ms(5000);
	}		
}

void disp_keycode()
{
    lcd_buffer[0]='K';
    lcd_buffer[1]='E';
    lcd_buffer[2]='Y';
    lcd_buffer[3]='C';
    lcd_buffer[4]='O';
    lcd_buffer[5]='D';
    lcd_buffer[6]='E';
    lcd_buffer[7]=':';
    lcd_buffer[8]='0';
    lcd_buffer[9]='x';
    lcd_buffer[10]=ascii[key/16];
    lcd_buffer[11]=ascii[key%16];
    lcd_buffer[12]=' ';
    lcd_buffer[13]=' ';
    lcd_buffer[14]=' ';
    lcd_buffer[15]=' ';
	disp_line(0);
}