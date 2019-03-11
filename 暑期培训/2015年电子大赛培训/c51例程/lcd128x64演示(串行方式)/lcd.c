#include <reg52.h>
#include "lcd.h"

sbit lcd_cs=P2^6;
sbit lcd_sdat=P2^5;
sbit lcd_sclk=P2^7;

//sbit LCD_CS  =  P2^6;      //�Ĵ���ѡ������ 
//sbit LCD_STD  =  P2^5;      //Һ����/д����
//sbit LCD_SCLK  =  P2^7;      //Һ��ʹ�ܿ���




unsigned char lcd_buffer[16];

extern void delay(long dly);

/*********************************************************************************************************
** Function name:       HZLCD_Init
** Descriptions:        HZLCD ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void HZLCD_Init (void)
{
    lcd_cs=0;
	lcd_sdat=0;
	lcd_sclk=0;
	write_comm(0x30);			//����ָ�
	write_comm(0x01);			//�����ʾ��Ļ����DDRAMλַ����������Ϊ"00H"
	delay(1000);
	write_comm(0x03);			//��DDRAMλַ����������Ϊ"00H"���α��ԭ�㣬�ù��ܲ�Ӱ����ʾDDRAM
	write_comm(0x06);			//�������
	write_comm(0x0f);			//��ʾ����
}


/*********************************************************************************************************
** Function name:       send_data
** Descriptions:        ��lcdд��1���ֽڵ����ݡ�
** input parameters:    data��Ҫд�������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void send_data(unsigned char dat)     //8λΪ��������
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

void write_data(unsigned char dat)             //��LCDд����
{	
	lcd_cs=1;
	send_data(0xfa);
	send_data(dat&0xf0);
	send_data((dat&0x0f)<<4);
	lcd_cs=0;
}
void write_comm(unsigned char dat)				  //��LCDд����
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
		write_comm(0x0090);    //�趨DDRAM�ڶ��У���һ��
		if (i==31)
		write_comm(0x0088);    //�趨DDRAM�����У���һ��
		if (i==47)
		write_comm(0x0098);    //�趨DDRAM�����У���һ��
		if (i==63)
		write_comm(0x0080);    //�趨DDRAM�����У���һ��
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
    unsigned char code hz[65]={"  ����ģ��C ��  Һ��ͼ����ʾ��--    ��ʾ����                    "};
    unsigned char code hz1[65]={"  Ӣ���ַ���ʾ  a b c d e f g h i j k l m n o p q r s t u v wxyz"};
    unsigned char code hz2[65]={"  Ӣ���ַ���ʾ  A B C D E F G H I J K L M N O P Q R S T U V WXYZ"};
    unsigned char code hz3[65]={"  ���ַ�����ʾ  0 1 2 3 4 5 6 7 8 9 ~ !   # $ % ^ & * ( ) - + = "};
    unsigned char code hz4[65]={"+++++++++++++++++ ��ӭʹ��LCD  ++   Һ����ʾ�� +++++++++++++++++"};	
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


	