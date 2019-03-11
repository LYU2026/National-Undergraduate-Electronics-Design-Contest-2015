#include<reg52.h>
#include"ds1302.h"

/******************************************************************************
�������ƣ�DS1302_WriteOneByte
�������ܣ���DS1302д��һ���ַ�
��ڲ�����ucData-����
����ֵ����
��ע����
*******************************************************************************/
void DS1302_WriteOneByte(unsigned char uData) 
{ 
    unsigned char i;
    ACC = uData;
    DS1302_RST = 1;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;         
        DS1302_CLK = 0;
        DS1302_CLK = 1;				//��д�����λ��������д��
        ACC = ACC >> 1; 
    } 
}

/******************************************************************************
�������ƣ�DS1302_ReadOneByte
�������ܣ���DS1302��ȡһ������
��ڲ�������
����ֵ����ȡ������
��ע����
*******************************************************************************/
unsigned char DS1302_ReadOneByte(void) 
{ 
    unsigned char i;
    DS1302_RST = 1;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         //�൱�ڻ���е� RRC 
        DS1302_IO = 1;
        DS1302_CLK = 1;
        DS1302_CLK = 0;			   //�½��ض�ȡ���ȶ����λ
        ACC7 = DS1302_IO;
    } 
    return(ACC); 
}

/******************************************************************************
�������ƣ�DS1302_WriteOneByteAtAddr
�������ܣ���DS1302��ָ��λ��д��һ������
��ڲ�����ucAddr-��ַ	  ��  ucData-����
����ֵ����
��ע����
*******************************************************************************/
void DS1302_WriteOneByteAtAddr(unsigned char uAddr, unsigned char uData)
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(uAddr);      
    DS1302_WriteOneByte(uData);      
    DS1302_CLK = 1;
    DS1302_RST = 0;
    DS1302_IO = 0;
}

/******************************************************************************
�������ƣ�DS1302_ReadOneByteAtAddr
�������ܣ���ָ����λ�ö�ȡһ������
��ڲ�����ucAddr-��ַ
����ֵ����ȡ������
��ע����
*******************************************************************************/
unsigned char DS1302_ReadOneByteAtAddr(unsigned char uAddr)
{
    unsigned char ucBackValue;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(uAddr);             
    ucBackValue = DS1302_ReadOneByte();         
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucBackValue);
}

/******************************************************************************
�������ƣ�DS1302_SetInit
�������ܣ����ó�ʼ��
��ڲ�����pClk-��ʼ�������ָ��
����ֵ����
��ע����
*******************************************************************************/
void DS1302_SetInit(unsigned char *pClk) 
{
    unsigned char i;
    unsigned char uAddr = 0x80; 
    DS1302_WriteOneByteAtAddr(0x8e,0x00);           /* ��������,WP=0,д����*/
    for(i =7; i>0; i--)
    { 
        DS1302_WriteOneByteAtAddr(uAddr,*pClk);  /* �� �� ʱ �� �� ���� �� */ 
        pClk++;
        uAddr +=2;
    }
    DS1302_WriteOneByteAtAddr(0x8e,0x80);           /* ��������,WP=1,д����*/
}