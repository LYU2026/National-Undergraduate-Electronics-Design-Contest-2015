#include<reg51.h>
#include "intrins.h"
#include"8DigitalTubeDisplay.h"
//send the data to the HC595

unsigned char code Seg_Data[]={	 //��������ܵı��룬�������ݶ�����CODE��
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

/*******************************************************
�������ܣ���ָ��λ����ʾһ������
����˵����Data��Ҫ��ʾ�����ݣ�Addr���ڵڼ�λ��ʾ��

Addrȡֵ��Χ��0~9��
Addr=0~7ʱ��ѡ�������ʾ�ڵڼ�λ�������;
Addr=8  ʱ��ͬʱѡ��8λ����ܣ��������������
Addr=9  ʱ���ر�8λ�����

8λ����ܣ���������Ϊ��0λ����1λ...��7λ��

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
		SEG_Send595OneByte(Seg_Addr[Addr-1]);  //��ʾ����һ���������
		SEG_Send595OneByte( Seg_Data[Data]);  //��ʾ������
		}
		SEG_STCP = 0;
		SEG_STCP = 1;		 //STCP���ŵ������ظ�������
		SEG_STCP = 0;
	
}

//main()


