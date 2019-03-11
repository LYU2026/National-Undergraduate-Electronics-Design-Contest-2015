#include <reg52.h>

#include "lcd.h"
#include "74hc164.h"
#include "74hc165.h"
/**********************************
PIN�ڶ���
**********************************/
sbit LCD_E=P1^2;
sbit LCD_RW=P1^1;
sbit LCD_RS=P1^0;

unsigned char lcd_buffer[]={"0123456789ABCDEF"};

unsigned char code ascii[]={"0123456789abcdef"};

/*******************************************************************************
* �� �� ��         : Lcd1602_Delay1ms
* ��������		   : ��ʱ��������ʱ1ms
* ��    ��         : c
* ��    ��         : ��
* ˵    ��         : �ú�������12MHZ�����£�12��Ƶ��Ƭ������ʱ��
*******************************************************************************/

void Lcd1602_Delay1ms(unsigned int c)   //��� 0us
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
* �� �� ��         : LcdWriteCom
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/
void LcdWriteCom(unsigned char com)	  //д������
{	
	unsigned char x;
	LCD_E = 0;	 //ʹ������
	LCD_RS = 0;	 //ѡ��д������
	LCD_RW = 0;	 //ѡ��д��

	send_data(com);	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	x++;
	LCD_E = 1;	 //д��ʱ��
	x++;
	LCD_E = 0;
	Lcd1602_Delay1ms(1);
}
/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/		   
void LcdWriteData(unsigned char dat)			//д������
{
	unsigned char x;
	LCD_E = 0;	  //ʹ������
	LCD_RS = 1;	  //ѡ��д������
	LCD_RW = 0;	  //ѡ��д��

	send_data(dat);	
	x++;
	LCD_E = 1;	  //д��ʱ��
	x++;
	LCD_E = 0;
	Lcd1602_Delay1ms(1);
}
/*******************************************************************************
* �� �� ��       : LcdInit()
* ��������		 : ��ʼ��LCD��
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/		   
void LcdInit()						  //LCD��ʼ���ӳ���
{
    unsigned char code zifu[] ={0x04,0x0F,0x12,0x0F,0x0A,0x1F,0x02,0x02,   		//��  ascii��=0x00
						  		0x0F,0x09,0x0F,0x09,0x0F,0x09,0x11,0x00,	 	//��  ascii��=0x01
						  		0x00,0x0F,0x09,0x0F,0x09,0x0F,0x00,0x00,	 	//��  ascii��=0x01
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						  		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char i;

	LcdWriteCom(0x38);	 
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	Lcd1602_Delay1ms(5);

	LcdWriteCom(0x40);	 

    for(i=0;i<64;i++)
	{
		LcdWriteData(zifu[i]);
	}
}

void disp_line(unsigned char line)   //line=0,��1����ʾ��line=1,��2����ʾ
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