/**********************************************************************************
*  ����:       ************* DA-AD����*******************                         *
*																				  *
*   1.ͨ���������˽Ⲣ����AD-DAת��оƬ�Ļ���ԭ���ʹ�� 						  *
*   2.�˽�����I2C���߽ӿڵĹ���ԭ��һ���̷�����								  *
*   																			  *
*    ���ӷ��������ս���ͼ							          *
*    																			  *
* ͨ���ı�ѧϰ���ϵ�2����λ����Ӧ��2��ģ�����룬ʵ��ģ�����룬ѧԱ�۲�����ܵ����ֱ仯���
* ͨ����D[4]��ֵ��ʵ��ģ�������ѧԱ�۲�ѧϰ����DA��LED�����ȱ仯				  *
*				  *																  *
* ע�⣺I2C��������ֻ����12Tģʽ�¹���   6Tģʽ�Լ��޸���Ӧʱ��                   *                 	                                                              *
* ��ѧԱ�������������̣�����AD-DA ��C�����еĲ���Ӧ��                       	  *
***********************************************************************************/

#include<reg52.h>
#include<I2C.h>
#include"ad8591.h"




//else IO
//sbit    LS138A=P3^0;  
//sbit    LS138B=P3^1;
//sbit    LS138C=P3^2;  

/***********************************************
*ADC���ֽ����ݺ���               
*******************************************************************/
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

//******************************************************************/
/*main()
{  char i,j;

 while(1)
 {/********����AD-DA����*************/  
 /*  switch(AD_CHANNEL)
   {
     case 0: ISendByte(PCF8591,0x41);
             D[0]=IRcvByte(PCF8591)*2;  //ADC0 ģ��ת��1  �Ŵ�2����ʾ
			 break;  
 
	 case 1: ISendByte(PCF8591,0x42);
             D[1]=IRcvByte(PCF8591)*2;  //ADC1  ģ��ת��2
			 break;  

	 case 2: ISendByte(PCF8591,0x43);
             D[2]=IRcvByte(PCF8591)*2;  //ADC2	ģ��ת��3
			 break;  

	 case 3: ISendByte(PCF8591,0x40);
             D[3]=IRcvByte(PCF8591)*2;  //ADC3   ģ��ת��4
			 break;  

	 case 4: DACconversion(PCF8591,0x40, D[4]/4); //DAC	  ��ģת��
	         break;
   }

	      // D[4]=400;  //����--->>ģ�����
		   D[4]=D[0];  //   ��ģ������ �������ź� ͨ����ģת�����
   if(++AD_CHANNEL>4) AD_CHANNEL=0;
*/
 /********���½�AD��ֵ�͵�LED�������ʾ*************/
  /*     		
	 LedOut[0]=Disp_Tab[D[0]%10000/1000];
     LedOut[1]=Disp_Tab[D[0]%1000/100];
     LedOut[2]=Disp_Tab[D[0]%100/10]|0x80;
     LedOut[3]=Disp_Tab[D[0]%10];
	 
	 LedOut[4]=Disp_Tab[D[0]%10000/1000];
     LedOut[5]=Disp_Tab[D[0]%1000/100];
     LedOut[6]=Disp_Tab[D[0]%100/10]|0x80;
     LedOut[7]=Disp_Tab[D[0]%10];  
	  
	
	 for( i=0; i<8; i++) 
	 {	 
			
	  switch(i)	  //ʹ��switch ������138������  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;
			
	     }
	 P0 = LedOut[i];
	     for (j = 0 ; j<90 ;j++) { ;}	   //ɨ����ʱ��
			  P0 = 0; 
	  }

	   

 }  
}

*/

