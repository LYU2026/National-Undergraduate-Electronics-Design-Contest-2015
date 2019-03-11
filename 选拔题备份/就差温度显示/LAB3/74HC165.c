#include<reg51.h> 
#include"74HC165.h"
#include<intrins.h> 



unsigned char hc74_165()
{
	unsigned char i,p;
	
	SL_165=0;//����λ/������ƶ�S/LΪ�͵�ƽʱ���������ݣ�A��H��������Ĵ���.��ʱ�ӣ�CLK,CLK INH�����������ݣ�SER�����޹�

	_nop_();
	_nop_();

	SL_165=1;//��S/LΪ�ߵ�ƽʱ�������������ܱ���ֹ
	delay1_165ms();

	CLK_165=0;

	p=0;
	for(i=0;i<8;i++) 
	{  
		p<<=1;
		p|=QH_165;
		CLK_165=0;
		_nop_();
		_nop_();
		CLK_165=1;//ʱ������ˣ���������Ч��
		_nop_();
	
		 
	}

	return(p);	
}



//��ʱ����
void Delay_165(unsigned int m)
{
	while(m--);
}

 //1ms��ʱ�ӳ���12MHZ��
void delay1_165ms()
{
	unsigned char i,j;	
	for(i=0;i<10;i++)
	for(j=0;j<33;j++);		 
}

/*void main(void)
{	
	unsigned char i;
		P1=0Xff;
	while(1)
	{ 
		
		for(i = 0;i < 8;i++) //LED��ˮ��ʾ
		{
			P0 = ~(0x01<<i);//P0��������74HC165�Ĳ��������A-H
			Delay_165(60000);
			P1= hc74_165();//P1�ڽ���74HC165��������ݣ�������LED����ʾ������ǰֵ��
			Delay_165(60000);
		}
		

	}
}
*/
