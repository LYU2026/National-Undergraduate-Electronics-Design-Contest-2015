#include<reg51.h>
#include <I2C.h>

#define  PCF8591 0x90    //PCF8591 ��ַ
unsigned char code Disp_Tab[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39}; 

unsigned char AD_CHANNEL;
unsigned long xdata  LedOut[8];
unsigned int  D[32];
/*******************************************************************
DAC �任, ת������               
*******************************************************************/
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack==0)return(0);
   SendByte(c);              //���Ϳ����ֽ�
   if(ack==0)return(0);
   SendByte(Val);            //����DAC����ֵ  
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC�����ֽ�[����]���ݺ���               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack==0)return(0);
   SendByte(c);              //��������
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //��������
   SendByte(sla+1);      //����������ַ
   if(ack==0)return(0);
   c=RcvByte();          //��ȡ����0

   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
   return(c);
}