#include<reg51.h>

sbit SEG_DS = P1^0;	//74HC595оƬ����������
sbit SEG_SHCP = P1^1;	//74HC595оƬ�Ŀ������ţ���������������
sbit SEG_STCP= P1^2;	//74HC595оƬ�Ŀ������ţ������ظ�������

void SEG_Send595OneByte(unsigned char uData);
void DisplayOneCharOnAddr(unsigned char uData,unsigned char uAddr);