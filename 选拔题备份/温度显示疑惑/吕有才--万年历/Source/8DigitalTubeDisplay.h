#include<reg51.h>

sbit SEG_DS = P1^0;	//74HC595芯片的数据引脚
sbit SEG_SHCP = P1^1;	//74HC595芯片的控制引脚，上升沿移入数据
sbit SEG_STCP= P1^2;	//74HC595芯片的控制引脚，上升沿更新数据

void SEG_Send595OneByte(unsigned char uData);
void DisplayOneCharOnAddr(unsigned char uData,unsigned char uAddr);