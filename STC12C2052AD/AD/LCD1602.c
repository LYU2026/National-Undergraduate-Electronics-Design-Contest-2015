#include<stc12c2052ad.h>
//#include<reg52.h>
#include"LCD1602.h"
unsigned char  code Datacode[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//��
	                          0x0f,0x09,0x0f,0x09,0x0f,0x09,0x0b,0x10,//��
                            0x0e,0x0a,0x0a,0x0e,0x0a,0x0a,0x0e,0x00};//��
void checkbusy(void)
{
	unsigned int m=256;

	RS=0;
	RW=1;
	D=0xff;
	while((m--)&(m>0)&(D&(0x80)));
	D=0;
}
void write(unsigned char ucData, bit b)
{
	checkbusy();
		D=ucData;
	RS=b;
	RW=0;
	//delay(1);
	E = 1;
	//delay(1);
	E = 0;
}


void LCDInit()
{
	unsigned char i;
	write(0x38,0);//����
	delay(2);
	write(0x38,0);//��긴λ
	delay(2);
        write(0x38,0);//���뷽ʽ����
	delay(2);
	write(0x38,0);//��ʾ���ؿ���
	delay(2);
	write(0x0c,0);// �����λ
	delay(2);
	write(0x01,0);//��������
	delay(2);
	write(0x40,0); //�Զ����ַ�����
	for(i=0;i<64;i++)//�����Զ����ַ�����
	{
		write(Datacode[i],1);
	}
}
void position(unsigned char x,unsigned char y)
{
	if(x==0)
		write((0x80|y),0);
	if(x==1)
		write((0xc0|y),0);
}
void show(unsigned char x,unsigned char y,unsigned char ucData)
{
	position(x,y);
	write(ucData,1);
}
void string(unsigned char *ucStr)
{
	while(*ucStr!='\0')
	{	write(*ucStr,1);
	ucStr++;}
}
void delay(unsigned int n)
{
	unsigned int k=2;
	for(;k>0;k--)
	{
		while(n--);
	}
}