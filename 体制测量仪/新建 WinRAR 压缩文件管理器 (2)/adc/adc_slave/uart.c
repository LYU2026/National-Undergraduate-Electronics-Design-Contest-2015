#include "uart.h"
void InitUart()
{
    SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1 = TL1 = -(FOSC/12/16/BAUD-256); //Set auto-reload vaule
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
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

	receiveData=SBUF;//��ȥ���յ�������
	RI = 0;//��������жϱ�־λ
}
*****************************************************/