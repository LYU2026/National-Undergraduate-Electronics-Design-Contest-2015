//#include<reg52.h>
sbit CLK=P2^0;    //����ʱ��
sbit INPUT=P2^1;  //�������ݣ���B����A���������ǽ�B
sbit A=P2^2;  //���յ���һ��(A��B),������A����P22

void delay(unsigned int cnt) ;
void HC164(unsigned char temp);