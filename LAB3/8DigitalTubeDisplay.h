//#include<reg51.h>
sbit SEG_DS = P3^3;	//74HC595оƬ����������
sbit SEG_SHCP = P3^4;	//74HC595оƬ�Ŀ������ţ���������������
sbit SEG_STCP = P3^5;	//74HC595оƬ�Ŀ������ţ������ظ�������


void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr); //��ָ����λ������ʾһ������
void SEG_Send595OneByte(unsigned char ucData);	 //��74HC595д��һ��8λ������
