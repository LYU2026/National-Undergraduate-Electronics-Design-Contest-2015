#include<reg52.h>
#include "uart.h"
#include"main.h"
void main()
{
	InitUart();
		while(1)
	 {
	 }
 }
 void Usart() interrupt 4
{
	unsigned char receiveData;
  receiveData=SBUF;//出去接收到的数据
	//清除接收中断标志位
	SendData(receiveData);
	RI = 0;
}