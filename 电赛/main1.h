

//#define  NOP()   _nop_() 
sbit RELAY = P3^0;	
sbit KEY=P3^1;
int num1 =0;
sbit Delay1=P3^2;
sbit bee=P3^3;
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[5],ADData[8],T[8];
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
 
void LcdDisplayTemp0(int temp);
void readtime();
void LcdDisplay();
void daydata();   //设置天数函数
unsigned char checkday();
void timesetting();
void year(unsigned char k);//年加减
void month(unsigned char k);//月加减
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);