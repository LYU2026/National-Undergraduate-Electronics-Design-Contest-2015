/*======================================================
             �ൺͨ�����ӿƼ����޹�˾����
                     Ӳ����TYD-51-1
          ���ܣ�8λ�������˸���Σ�Ȼ��������
          ���������µ缼��֧�����18764250687
======================================================*/

#include "reg51.h"

#define DLY_TIME 50000

void delay(long dly)            //��ʱ�ӳ���
{
	while(dly--);	
}

void main(void)
{
	char i;
	unsigned char x;
	while(1)
	{
		P1=0x00;                //8λ�����ȫ����
		delay(DLY_TIME);           //��ʱԼ1��
	    P1=0xff;                //8λ�����ȫ��
		delay(DLY_TIME);
		P1=0x00;
		delay(DLY_TIME);
	    P1=0xff;
		delay(DLY_TIME);
		x=1;                     //����ʾ��ֵ
		for(i=0;i<8;i++)         //��������ε���
		{
			P1=~x;
			delay(DLY_TIME);
			x=x<<1;
		}
	}
}
