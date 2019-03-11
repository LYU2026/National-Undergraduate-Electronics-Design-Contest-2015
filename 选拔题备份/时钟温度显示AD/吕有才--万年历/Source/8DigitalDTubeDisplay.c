#include"8DigitalTubeDisplay.h"
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
					        0x88,/*A*/
					        0x83,/*b*/
					        0xC6,/*C*/
					        0xA1,/*d*/
					        0x86,/*E*/
					        0x8E,/*F*/
	               ~0X40 //-/
							};

unsigned char code Seg_Addr[]={	    //to choose which tube to show the information//数码管位选编码，控制显示8位中的第几位
							0x01,
							0x02,
							0x04,
							0x08,
							0x10,
							0x20,
							0x40,
							0x80,
							0xFF,//ALL ON
							0x00 //All OFF
							};
//send the data to the HC595
void SEG_Send595OneByte(unsigned char uData) //向74HC595写入一个8位的数据
{
	unsigned char i;
	
	for(i = 0;i < 8;i++)	 //8位数据依次写入，先写最低位
	{
		SEG_DS = (uData & 0x80);	//先读入高位   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP引脚的上升沿移入数据
		uData <<=1;	   //数据左移
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
void DisplayOneCharOnAddr(unsigned char uData,unsigned char uAddr)     //Addr is the address of tube ,Data is the number that is showed in the tube
{
		SEG_Send595OneByte(Seg_Addr[uAddr]);  //send the address of tube which shows information
		SEG_Send595OneByte( Seg_Data[uData]);  //send the data to the tube
		SEG_STCP = 0;
		SEG_STCP = 1;		 //CPU update the data
		SEG_STCP = 0;
		
	
}



//main()


