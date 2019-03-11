#include<reg51.h>
#include "intrins.h"
#include"8DigitalTubeDisplay.h"
//send the data to the HC595

unsigned char code Seg_Data[]={	 //共阳数码管的编码，并将数据定义在CODE区
							0xC0,/*0*/
					        0xF9,/*1*/
					        0xA4,/*2*/
					        0xB0,/*3*/
					        0x99,/*4*/
					        0x92,/*5*/
					        0x82,/*6*/
					        0xF8,/*7*/
					        0x80,/*8*/
					        0x90,/*9*/
					        0x88,/*A*/
					        0x83,/*b*/
					        0xC6,/*C*/
					        0xA1,/*d*/
					        0x86,/*E*/
					        0x8E,/*F*/
							};
unsigned char code Seg_Addr[]={
							0x01,
							0x02,
							0x04,
							0x08,
							0x10,
							0x20,
							0x40,
							0x80,
							0xFF,//ALL ON
							0x00 //OFF
							};
void SEG_Send595OneByte(unsigned char ucData) //向74HC595写入一个8位的数据
{
	unsigned char i;
	
	for(i = 0;i < 8;i++)	 //8位数据依次写入，先写最低位
	{
		SEG_DS = (ucData & 0x80);	//先读入高位   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP引脚的上升沿移入数据
		ucData <<=1;	   //数据左移
  }
}

/*******************************************************
函数功能：在指定位置显示一个数据
参数说明：Data是要显示的数据，Addr是在第几位显示。

Addr取值范围是0~9。
Addr=0~7时，选择的是显示在第几位数码管上;
Addr=8  时，同时选中8位数码管，即打开所有数码管
Addr=9  时，关闭8位数码管

8位数码管，左数依次为第0位，第1位...第7位。

*******************************************************/
void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr)
{
		if(Addr ==3)
		{
			SEG_Send595OneByte(Seg_Addr[Addr-1]);
			SEG_Send595OneByte(Seg_Data[Data]& 0x7F);
			
		}
		else
		{
		SEG_Send595OneByte(Seg_Addr[Addr-1]);  //显示在哪一个数码管上
		SEG_Send595OneByte( Seg_Data[Data]);  //显示的数据
		}
		SEG_STCP = 0;
		SEG_STCP = 1;		 //STCP引脚的上升沿更新数据
		SEG_STCP = 0;
	
}

//main()


