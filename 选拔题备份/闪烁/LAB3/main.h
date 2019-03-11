#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int
sbit beat=P3^0;
int n=1,a; 
unsigned char Time0[9],DisplayData[8],Data0[5],key;
uchar code  DIS1[] = {"DATE:"},
            DIS2[] = {"TIME:"},
            DIS3[] = {"TEMP:"},
						DIS4[] = {"AD1:"},
						DIS5[] = {"Setting"};
void delay_main(int ms);
void readtime();
void Lcd12864Display();
void ClockPlay();
void LcdDisplayTemp0(int temp);
void ADDisplay();						
void delay0(unsigned int n);		
void delay10(unsigned int n);						
void timesetting();	
void year(unsigned char k);//年加减
void month(unsigned char k);//月加减
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void timesetting();//电子钟设置			

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
void Delaybuzz(unsigned int t)
{
	while(t--)
	{
		delay10(245);
		delay10(245);
	}
}
unsigned char keyboardscan(unsigned char n)
{
	unsigned char board_h, board_l;

	P1=0x0f;
	board_h=P1&0x0f;
	if(board_h!=0x0f)
	{
		delay10(n);
		board_h=P1&0x0f;
		if(board_h!=0x0f)
		{
			P1=board_h|0xf0;
			board_l=P1&0xf0;

			for(a=0;a<50;a++)
		{delay10(100);
		 beat=!beat;
		}
			beat=0;
		for(a=0;a<100;a++)
			{Delaybuzz(1);}
						return(board_h+board_l);
		}
	}
	return(0xff);
}

