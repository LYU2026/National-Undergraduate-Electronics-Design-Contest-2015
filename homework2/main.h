unsigned char code ucInitTime[7]={0x55,0x46,0x17,0x21,0x01,0x01,0x13};
/* CLK=P2^0;    //串行时钟
 INPUT=P2^1;  //串行数据，接B或者A，这里我们接B
  sbit A=P2^2;  //悬空的令一端(A或B),这里是A，接P22
*/
void LcdDisplayTemp(int temp);
void readtime();
void LcdDisplay();
void year(unsigned char k);//年加减
void month(unsigned char k);//月加减
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void weekset(unsigned char k);//周加减
void timesetting();
void delay_main(unsigned int cnt); 
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[10],Time1[6],c=0,x=0;
unsigned char checkday()                  //自动检查是否是闰年，并为对应的月份设置天数
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//判断是否是闰年
 {
	  switch(Time0[4])
	{
		case 0x01: {day=0x31;break;}
		case 0x02: {day=0x29;break;}
		case 0x03: {day=0x31;break;}
		case 0x04: {day=0x30;break;}
		case 0x05: {day=0x31;break;}
		case 0x06: {day=0x30;break;}
		case 0x07: {day=0x31;break;}
		case 0x08: {day=0x31;break;}
		case 0x09: {day=0x30;break;}
		case 0x10: {day=0x31;break;}
		case 0x11: {day=0x30;break;}
		case 0x12: {day=0x31;break;}
	}
 }
 else
 {
 switch(Time0[4])
	{
		case 0x01: {day=0x31;break;}
		case 0x02: {day=0x28;break;}
		case 0x03: {day=0x31;break;}
		case 0x04: {day=0x30;break;}
		case 0x05: {day=0x31;break;}
		case 0x06: {day=0x30;break;}
		case 0x07: {day=0x31;break;}
		case 0x08: {day=0x31;break;}
		case 0x09: {day=0x30;break;}
		case 0x10: {day=0x31;break;}
		case 0x11: {day=0x30;break;}
		case 0x12: {day=0x31;break;}
	}
	}
 
 return(day);
}


//unsigned char Data0[10]={0x11,0x12,0x13,0x14,0X23,0X26,0X40};
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
