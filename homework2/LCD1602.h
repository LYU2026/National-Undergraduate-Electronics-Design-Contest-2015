#include<reg52.h>
sbit LCD1602_RS =P1^0; //P1^2;	//λ���壬Һ��������/����ѡ��
sbit LCD1602_RW = P1^1;//P1^1;	//λ���壬Һ���Ķ�дѡ��
sbit LCD1602_EN = P1^2;//P1^0;	//λ���壬Һ��ʹ���ź�

//#define LCDPORT P0	  //Һ�������ݿ�

//unsigned char code ucForum[]=" bbs.cepark.com ";	  //��CODE������һ��������ʾ�ĳ����ַ���

void Delay_lcd(unsigned int uiCount);	//��ʱ����
//void LCD1602_CheckBusy(void);	//Һ��æ���
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
void LCD1602_Init();	//Һ����ʼ��
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Һ���������ƶ���ָ��λ��
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//��Һ��ָ��λ����ʾ�ַ�
void LCD1602_DisplayString(unsigned char *ucStr);	//��Һ������ʾ�ַ���
