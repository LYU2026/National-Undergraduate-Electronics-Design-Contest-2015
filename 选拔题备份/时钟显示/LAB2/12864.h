#include <reg52.h>
#include <intrins.h>

/************* 12864LCD���Ŷ��� *************/
#define LCD_data  P0       //���ݿ�
sbit LCD_RS  =  P2^4;      //�Ĵ���ѡ������ 
sbit LCD_RW  =  P2^5;      //Һ����/д����
sbit LCD_EN  =  P2^6;      //Һ��ʹ�ܿ���
sbit LCD_PSB =  P2^7;      //��/����ʽ����
//sbit LCD_RST =  P3^7;      //Һ����λ�˿�
#define uchar unsigned char
#define uint  unsigned int
void delay(int ms);
void delay1(int ms);
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_init();
void lcd_pos(uchar X,uchar Y);
void lcdflag();
void photodisplay(uchar *bmp);
void  clr_screen();
void  bytecode();
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};