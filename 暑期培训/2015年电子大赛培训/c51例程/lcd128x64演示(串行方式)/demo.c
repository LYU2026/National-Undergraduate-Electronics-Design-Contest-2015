//************************************************************************
//       demo.c    TYD-51-1ʵ�����ʾ����
//
//       (F)����Сʱ+1
//       (E)����Сʱ-1
//       (D)������+1
//       (C)������-1
//       (B)������=0
//
//       (3)����8λLED����ܲ���
//       (2)����8λLED����ܲ���
//       (1)����4*4���̴������
//
//       p14:scl
//       p15:sda
//
//************************************************************************
#include <reg52.h>
#include "VIiC_C51.H"			//����VI2C�����
#include "lcd.H"

sbit LCD_PSB =  P3^2;      //��/����ʽ����

#define	EEPROM24C02	0xA0		//24C02������ַ
#define	PCF8563		0xA2		//PCF8563������ַ
#define	WRADDR		0x00		//����д��Ԫ�׵�ַ
#define	RDADDR		0x02		//�������Ԫ�׵�ַ

unsigned char code wei_tab[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

unsigned char code led_tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,   //0,1,2,3,4,5,6,7
					   		  0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,   //8,9,A,B,C,D,E,F
							  0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,   //0.,1.,2.,3.,4.,5.,6.,7.
							  0xff,0xef,0xf7,0xfc,0xb9,0xde,0xf9,0xf1,   //8.,9.,A.,B.,C.,D.,E.,F.
							  0x00,0x40,0x73,0x38,0x76,0x50,0x78,0x6e};  //��,-,P,L,H,r,t,y	

unsigned char led_buffer[8];

unsigned char code td[9]={0x00,0x12,0x00,0x25,0x17,0x27,0x00,0x02,0x11};	//�����ʼ����
      //                             ��   ��   ʱ   ��  ����  ��    �� 
unsigned char code tyd[8]={0x26,0x27,0x0d,0x21,0x05,0x01,0x21,0x01};	            
unsigned char rd[8];	//����I2C���ջ�����
unsigned char code disp_buffer[]={"��ӭʹ��TYD-51-12011��  03��27��  ������  25��      21:56:45    "};
unsigned char code hz[]={"��һ����������"};
int count=0;
char disp_count=0;

char year=11;
char month=3;
char day=27;
char week=0;
char hour=23;
char min=56;
char sec=20;
int ms=0;
unsigned char key_code=0;
char temp=25;

bit key_press=0;
bit	swich_date=0;


void lcd_clock()
{
	unsigned char i;
	for(i=0;i<16;i++)
	{
	    lcd_buffer[i]=disp_buffer[i];
	}
	disp_line(0,lcd_buffer);

	for(i=0;i<16;i++)
	{
	    lcd_buffer[i]=disp_buffer[i+16];
	}
	lcd_buffer[2]=year/10+'0';
	lcd_buffer[3]=year%10+'0';

	lcd_buffer[8]=month/10+'0';
	lcd_buffer[9]=month%10+'0';

	lcd_buffer[12]=day/10+'0';
	lcd_buffer[13]=day%10+'0';

	disp_line(1,lcd_buffer);

	for(i=0;i<16;i++)
	{
	    lcd_buffer[i]=disp_buffer[i+32];
	}

	lcd_buffer[6]=hz[week*2];
	lcd_buffer[7]=hz[week*2+1];

	lcd_buffer[10]=temp/10+'0';
	lcd_buffer[11]=temp%10+'0';

	disp_line(2,lcd_buffer);
	
	for(i=0;i<16;i++)
	{
	    lcd_buffer[i]=disp_buffer[i+48];
	}
	
	lcd_buffer[4]=hour/10+'0';
	lcd_buffer[5]=hour%10+'0';

	lcd_buffer[7]=min/10+'0';
	lcd_buffer[8]=min%10+'0';

	lcd_buffer[10]=sec/10+'0';
	lcd_buffer[11]=sec%10+'0';
	disp_line(3,lcd_buffer);
}

void delay(long dly)
{
	while(dly--);
}

void disp_led(void)
{
	P0=0xff;
	P2=wei_tab[disp_count];
	P0=~led_tab[led_buffer[disp_count]];
	disp_count++;
	if(disp_count>7)
	{
		disp_count=0;
	}
}

void get_data(long dat)
{
	long x;
	x=dat;
	led_buffer[0]=x/100000;
	x=x%100000;
	led_buffer[1]=x/10000;
	x=x%10000;
	led_buffer[2]=x/1000;
	x=x%1000;
	led_buffer[3]=x/100;
	x=x%100;
    led_buffer[4]=x/10;
	x=x%10;
	led_buffer[5]=x;
}

void get_clock(void)
{
	led_buffer[0]=hour/10;
	led_buffer[1]=hour%10;
	led_buffer[2]=0x21;

	led_buffer[3]=min/10;
	led_buffer[4]=min%10;
	led_buffer[5]=0x21;

	led_buffer[6]=sec/10;
	led_buffer[7]=sec%10;
}

void get_key_code(void)
{
	led_buffer[0]=0x0c;
	led_buffer[1]=0x00;
	led_buffer[2]=0x0d;
	led_buffer[3]=0x0e;
	led_buffer[4]=0x21;
	led_buffer[5]=key_code/16;
	led_buffer[6]=key_code%16;
	led_buffer[7]=0x24;
}

void num_led_test(void)
{
	char i,j;
	for(i=0;i<0x27;i++)
	{
		for(j=0;j<8;j++)
		{
			led_buffer[j]=i;
		}
		delay(15000);
	}
	for(j=0;j<8;j++)
	{
		led_buffer[j]=j+1;
	}
	delay(50000);
}

void lsd_led_test(void)
{
	unsigned char x,i;
/*
	led_buffer[0]=0x26;
	led_buffer[1]=0x27;
	led_buffer[2]=0x0d;
	led_buffer[3]=0x21;
	led_buffer[4]=0x05;
	led_buffer[5]=0x01;
	led_buffer[6]=0x21;
	led_buffer[7]=0x01;
*/
	P1=0x00;
	delay(15000);
	P1=0xff;
	delay(15000);
	P1=0x00;
	delay(15000);
	P1=0xff;
	delay(15000);
    x=1;
	for(i=0;i<9;i++)
	{
		P1=~x;
        x=x<<1;
		delay(15000);
	}
}

void clock(void)
{
	ms++;
	if(ms>999)
	{
		ms=0;
		sec++;
		if(sec>59)
		{
			sec=0;
			min++;
			if(min>59)
			{
				min=0;
				hour++;
				if(hour>23)
				{
					hour=0;
				}
			}
		}
	//	get_clock();
	}
}


void init_timer0(void)
{
	TMOD=0x01;              /*����T0����������ʽ1*/
	TH0=0xfc;               /*��1��������8λTH0����ֵ*/
	TL0=0x66;              /*��1��������8λTL0����ֵ*/
	ET0=1;                   /*T0���ж�*/
//	EA=1;                    /*CPU���ж�*/
	TR0=1;

}

void read_key(void)
{
	unsigned char x;
    P3=0xf0;
	x=P3|0x0f;
//	P1=P3;
	if(x!=0xff)
	{
		delay(1000);
	    P3=0xf0;
	    x=P3|0x0f;
		if(x!=0xff)
		{
		    P3=x;
			key_code=P3;
			key_press=1;
		}
	}
}

void key_test(void)
{
	get_key_code();
	while(1)
	{
		read_key();
		if(key_press==1)
		{
			key_press=0;
			get_key_code();
		}
		delay(3000);
    }
}

void key_fenxi(void)
{
	switch (key_code)
	{
		case 0x77:
			hour++;
			if(hour>23)
			{
				hour=0;
			}
			break;
		case 0x7b:
			hour--;
			if(hour<0)
			{
				hour=23;
			}
			break;
		case 0x7d:
			min++;
			if(min>59)
			{
				min=0;
			}
			break;
		case 0x7e:
			min--;
			if(min<0)
			{
				min=59;
			}
			break;
		case 0xb7:
			sec=0;
			break;
		case 0xe7:
			lsd_led_test();
			break;
		case 0xeb:
			num_led_test();
			break;
		case 0xed:
			key_test();
			break;
		case 0xee:
			swich_date=~swich_date;
			break;

	}
}

void display_time(unsigned char  *sd)
{ 
	sd[0]=sd[0]&0x7f;//�����α���λ
	sd[1]=sd[1]&0x7f;//�����α���λ
	sd[2]=sd[2]&0x3f;//ʱ���α���λ
    led_buffer[0] =(sd[2]/16);
    led_buffer[1] =(sd[2]%16);	
    led_buffer[2] =0x21;
    led_buffer[3] =(sd[1]/16);
    led_buffer[4] =(sd[1]%16);
    led_buffer[5] =0x21;
    led_buffer[6] =(sd[0]/16);		
  	led_buffer[7] =(sd[0]%16);			
}
void display_date(unsigned char  *sd)
{ 
	sd[0]=sd[0]&0x3f;//�����α���λ
	sd[2]=sd[2]&0x1f;//�����α���λ
    led_buffer[0] =2;
   	led_buffer[1] =0;	
    led_buffer[2] =(sd[3]/16);
    led_buffer[3] =(sd[3]%16)+0x10;//�����С����
    led_buffer[4] =(sd[2]/16);
    led_buffer[5] =(sd[2]%16)+0x10;//�����С����
    led_buffer[6] =(sd[0]/16);		
	led_buffer[7] =(sd[0]%16);			
}

void get_tyd(void)
{
	char i;
	for(i=0;i<8;i++)
	{
		led_buffer[i] =rd[i];
    }
} 

void get_clock1(void)
{
	rd[0]=rd[0]&0x7f;//�����α���λ
	rd[1]=rd[1]&0x7f;//�����α���λ
	rd[2]=rd[2]&0x3f;//ʱ���α���λ
	rd[3]=rd[3]&0x3f;//�����α���λ
	rd[4]=rd[4]&0x07;//�����α���λ
	rd[5]=rd[5]&0x1f;//�����α���λ

    sec=(rd[0]/16)*10+(rd[0]%16);
    min=(rd[1]/16)*10+(rd[1]%16);
    hour=(rd[2]/16)*10+(rd[2]%16);
    week=rd[4];
	day=(rd[3]/16)*10+(rd[3]%16);
	month=(rd[5]/16)*10+(rd[5]%16);
	year=(rd[6]/16)*10+(rd[6]%16);
}


void main(void)
{
	long x=0;
	LCD_PSB=0;

//	ISendStr(EEPROM24C02,0x00,tyd,8);
//	delay(1000);
//	IRcvStr(EEPROM24C02,0x00,rd,0x8);	//�����ڵ�ʱ���֡���
//  get_tyd();
	HZLCD_Init ();
	while(1);
	screen(disp_buffer);
	delay(60000);
//	lcd_demo();
	init_timer0();
//	lsd_led_test();
    
//	ISendStr(PCF8563,WRADDR,td,0x5);	//��ʼ��PCF8563,�����Ҫ�Ļ����Բ�ȥ��
//	delay(1000);
//	ISendStr(PCF8563,WRADDR+5,&td[5],0x4);//��ʼ��PCF8563,�����Ҫ�Ļ����Բ�ȥ��

	while(1)
	{
		delay(100);
		IRcvStr(PCF8563,RDADDR,rd,0x7);	//�����ڵ�ʱ���֡���
        get_clock1();
		lcd_clock();
		delay(10000);
	}


    while(1)
	{
		read_key();
		if(key_press==1)
		{
			key_press=0;
			key_fenxi();
		}
		delay(3000);
		get_clock();
    }
}

void Timer0(void)  interrupt 1 
{
	TH0=0xfc;               /*��1��������8λTH0����ֵ*/
	TL0=0x7d;              /*��1��������8λTL0����ֵ  0x67  */
	clock();
//	disp_led();
}
