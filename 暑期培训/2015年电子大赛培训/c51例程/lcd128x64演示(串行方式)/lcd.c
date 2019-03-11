#include <reg52.h>
#include "lcd.h"

sbit lcd_cs=P2^6;
sbit lcd_sdat=P2^5;
sbit lcd_sclk=P2^7;

//sbit LCD_CS  =  P2^6;      //寄存器选择输入 
//sbit LCD_STD  =  P2^5;      //液晶读/写控制
//sbit LCD_SCLK  =  P2^7;      //液晶使能控制




unsigned char lcd_buffer[16];

extern void delay(long dly);

/*********************************************************************************************************
** Function name:       HZLCD_Init
** Descriptions:        HZLCD 初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void HZLCD_Init (void)
{
    lcd_cs=0;
	lcd_sdat=0;
	lcd_sclk=0;
	write_comm(0x30);			//基本指令集
	write_comm(0x01);			//清除显示屏幕，把DDRAM位址计数器调整为"00H"
	delay(1000);
	write_comm(0x03);			//把DDRAM位址计数器调整为"00H"，游标回原点，该功能不影响显示DDRAM
	write_comm(0x06);			//光标右移
	write_comm(0x0f);			//显示屏打开
}


/*********************************************************************************************************
** Function name:       send_data
** Descriptions:        向lcd写入1个字节的数据。
** input parameters:    data：要写入的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void send_data(unsigned char dat)     //8位为待送数据
{	
	unsigned char i;
	unsigned char data1=0x80;
	for(i=0;i<8;i++)
	{
		if((dat&data1)==0)
		{
			lcd_sdat=0;
		}
		else
		{
			lcd_sdat=1;
		}
        lcd_sclk=0; 
        lcd_sclk=1; 
        lcd_sclk=0; 
		data1=data1>>1;
	}
}

void write_data(unsigned char dat)             //向LCD写数据
{	
	lcd_cs=1;
	send_data(0xfa);
	send_data(dat&0xf0);
	send_data((dat&0x0f)<<4);
	lcd_cs=0;
}
void write_comm(unsigned char dat)				  //向LCD写命令
{	
	lcd_cs=1;
	send_data(0xf8);
	send_data(dat&0xf0);
	send_data((dat&0x0f)<<4);
	lcd_cs=0;
}
	

void screen(unsigned char *dat)
{	
	unsigned char i;
    write_comm(0x80);
	for (i=0;i<64;i++)
	{
		write_data(dat[i]);
		if (i==15)
		write_comm(0x0090);    //设定DDRAM第二行，第一列
		if (i==31)
		write_comm(0x0088);    //设定DDRAM第三行，第一列
		if (i==47)
		write_comm(0x0098);    //设定DDRAM第四行，第一列
		if (i==63)
		write_comm(0x0080);    //设定DDRAM第四行，第一列
	}
}	

void disp_line(unsigned char line,unsigned char *dat)
{	
	unsigned char i;
	switch(line)
	{
		case 0:
			write_comm(0x80);
			break;
		case 1:
			write_comm(0x90);
			break;
		case 2:
			write_comm(0x88);
			break;
		case 3:
			write_comm(0x98);
			break;
	}
	for (i=0;i<16;i++)
	{
		write_data(dat[i]);
	}
}	





void lcd_demo(void)
{
    unsigned char code hz[65]={"  中文模块C 型  液晶图文显示器--    演示程序                    "};
    unsigned char code hz1[65]={"  英文字符显示  a b c d e f g h i j k l m n o p q r s t u v wxyz"};
    unsigned char code hz2[65]={"  英文字符显示  A B C D E F G H I J K L M N O P Q R S T U V WXYZ"};
    unsigned char code hz3[65]={"  数字符号显示  0 1 2 3 4 5 6 7 8 9 ~ !   # $ % ^ & * ( ) - + = "};
    unsigned char code hz4[65]={"+++++++++++++++++ 欢迎使用LCD  ++   液晶显示屏 +++++++++++++++++"};	
//	while(1)
	{
		screen(hz);
		delay(30000);
		screen(hz1);
		delay(30000);
		screen(hz2);
		delay(30000);
		screen(hz3);
		delay(30000);
		screen(hz4);
		delay(30000);
//		paint();
//		delay(30000);

	}
}

void clear_lcd_buffer()
{
	int i;
	for(i=0;i<16;i++)
	{
        lcd_buffer[i]=' ';		
	}
}


	