#include"temp.h"
#include"intrins.h"
/*******************************************************************************
* �� �� ��         : Delay1ms
* ��������		   : ��ʱ����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void DS18B20_Init(void)
{
	unsigned char x=0;
	DQ = 1;    //DQ��λ
	Delay_us(10);
	//Delay(8);  //������ʱ,10us
	DQ = 0;    //��Ƭ����DQ����
	Delay_us(500);
	//Delay(80); //��ȷ��ʱ ���� 480us ,498us
	DQ = 1;    //��������
	Delay_us(154);
	//Delay(14);	//154us
	x=DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	Delay_us(212);
	//Delay(20); //212us
}



unsigned char DS18B20_ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // �������ź�
		dat>>=1;
		DQ = 1; // �������ź�
		if(DQ)
		dat|=0x80;
		Delay_us(56);
		//Delay(4); //56us
	}
	return(dat);
}



void DS18B20_WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_us(66);
		//Delay(5); //66us
		DQ = 1;
		dat>>=1;
	}
}



unsigned int DS18B20_ReadTemperature()
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); // ����������кŵĲ���
	DS18B20_WriteOneChar(0x44); // �����¶�ת��
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); //����������кŵĲ���
	DS18B20_WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	a=DS18B20_ReadOneChar();
	b=DS18B20_ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	//t= t/2; //�Ŵ�10���������������---����û��
	return(t);
}
void Delay_us(unsigned int uiUs)	//us��ʱ����
{
	for(;uiUs > 0;uiUs--)
	{
		_nop_();
	}	
}



void Delay_ms(unsigned int uiMs)	//ms��ʱ����
{
	unsigned int i;
	for(;uiMs > 0;uiMs--)
	{
		for(i = 1498;i > 0;i--);
	}
}
