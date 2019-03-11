#include"8DigitalTubeDisplay.h"
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//��������ܵı��룬�������ݶ�����CODE�� 

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

unsigned char code Seg_Addr[]={	    //to choose which tube to show the information//�����λѡ���룬������ʾ8λ�еĵڼ�λ
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
void SEG_Send595OneByte(unsigned char uData) //��74HC595д��һ��8λ������
{
	unsigned char i;
	
	for(i = 0;i < 8;i++)	 //8λ��������д�룬��д���λ
	{
		SEG_DS = (uData & 0x80);	//�ȶ����λ   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP���ŵ���������������
		uData <<=1;	   //��������
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
void DisplayOneCharOnAddr(unsigned char uData,unsigned char uAddr)     //Addr is the address of tube ,Data is the number that is showed in the tube
{
		SEG_Send595OneByte(Seg_Addr[uAddr]);  //send the address of tube which shows information
		SEG_Send595OneByte( Seg_Data[uData]);  //send the data to the tube
		SEG_STCP = 0;
		SEG_STCP = 1;		 //CPU update the data
		SEG_STCP = 0;
		
	
}



//main()


