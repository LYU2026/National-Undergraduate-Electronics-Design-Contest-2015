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
  receiveData=SBUF;//��ȥ���յ�������
	//��������жϱ�־λ
	SendData(receiveData);
	RI = 0;
}