#include<reg52.h>
sbit CLK=P1^2;    //����ʱ��
sbit INPUT=P1^3;  //�������ݣ���B����A���������ǽ�B
//sbit A=P2^2;  //���յ���һ��(A��B),������A����P22

void delay(unsigned int cnt) ;
void  HC164(unsigned char temp);