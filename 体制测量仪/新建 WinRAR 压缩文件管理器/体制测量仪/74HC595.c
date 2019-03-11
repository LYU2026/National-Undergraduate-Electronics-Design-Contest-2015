#include<reg52.h>
#include"74HC595.h"
void SEG_Send595OneByte(unsigned char ucData) //��74HC595д��һ��8λ������
{
	unsigned char i;
	
	for(i = 0;i < 8;i++)	 //8λ��������д�룬��д���λ
	{
		SEG_DS = (ucData & 0x80);	//�ȶ����λ   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP���ŵ���������������
		ucData <<=1;	   //��������
  }
}

void Send595A(unsigned char A)
{
	SEG_Send595OneByte(A);  //send1
	//SEG_Send595OneByte(0x);  //send2
	SEG_STCP = 0;
	SEG_STCP = 1;		 //CPU update the data
	SEG_STCP = 0;
}

void Send595AB(unsigned char A,unsigned char B )
{
	SEG_Send595OneByte(A);  //send1
	SEG_Send595OneByte(B);  //send2
	SEG_STCP = 0;
	SEG_STCP = 1;		 //CPU update the data
	SEG_STCP = 0;
}