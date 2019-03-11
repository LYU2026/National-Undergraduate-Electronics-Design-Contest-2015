#include "uart.h"
void InitUart()
{
    SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1 = TL1 = -(FOSC/12/16/BAUD-256); //Set auto-reload vaule
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;                        //T1 start running
}
void SendData(unsigned char dat)
{
	SBUF = dat;
    while (!TI);                    //Wait for the previous data is sent
    TI = 0;                         //Clear TI flag                     //Send current data
}
/*****************************************************
void Usart() interrupt 4
{
	unsigned char receiveData;

	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
}
*****************************************************/