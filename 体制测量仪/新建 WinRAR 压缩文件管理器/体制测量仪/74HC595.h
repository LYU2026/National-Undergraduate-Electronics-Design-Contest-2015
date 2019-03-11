#include<reg52.h>
sbit SEG_DS = P3^4;	//74HC595芯片的数据引脚
sbit SEG_SHCP = P3^6;	//74HC595芯片的控制引脚，上升沿移入数据
sbit SEG_STCP = P3^5;	//74HC595芯片的控制引脚，上升沿更新数据
void SEG_Send595OneByte(unsigned char ucData) //向74HC595写入一个8位的数据
void Send595A(unsigned char A);
void Send595AB(unsigned char A,unsigned char B );