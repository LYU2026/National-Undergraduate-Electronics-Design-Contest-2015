#include<reg51.h>
#include<intrins.h>

/************* 12864LCD引脚定义 *************/
#define LCD_data  P0       //数据口
sbit LCD_RS  =  P2^4;      //寄存器选择输入 
sbit LCD_RW  =  P2^5;      //液晶读/写控制
sbit LCD_EN  =  P2^6;      //液晶使能控制
sbit LCD_PSB =  P2^7;      //串/并方式控制
//sbit LCD_RST =  P3^7;      //液晶复位端口
#define uchar unsigned char
#define uint  unsigned int
void delay12(int ms);
void delay1(int ms);
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_init();
void lcd_pos(uchar X,uchar Y);
void lcdflag();
void photodisplay(uchar *bmp);
void  clr_screen();
void  bytecode();
void LCD12864_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};