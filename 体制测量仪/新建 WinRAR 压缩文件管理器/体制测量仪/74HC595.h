#include<reg52.h>
sbit SEG_DS = P3^4;	//74HC595оƬ����������
sbit SEG_SHCP = P3^6;	//74HC595оƬ�Ŀ������ţ���������������
sbit SEG_STCP = P3^5;	//74HC595оƬ�Ŀ������ţ������ظ�������
void SEG_Send595OneByte(unsigned char ucData) //��74HC595д��һ��8λ������
void Send595A(unsigned char A);
void Send595AB(unsigned char A,unsigned char B );