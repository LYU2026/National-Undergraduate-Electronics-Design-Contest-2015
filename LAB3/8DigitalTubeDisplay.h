//#include<reg51.h>
sbit SEG_DS = P3^3;	//74HC595芯片的数据引脚
sbit SEG_SHCP = P3^4;	//74HC595芯片的控制引脚，上升沿移入数据
sbit SEG_STCP = P3^5;	//74HC595芯片的控制引脚，上升沿更新数据


void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr); //在指定的位置上显示一个数字
void SEG_Send595OneByte(unsigned char ucData);	 //向74HC595写入一个8位的数据
