/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : LCD1602显示试验
* 实验说明       : LCD1602第一行Pechin Science，第二行显示Welcome to the world 
*                * of MCU。并一直移动。
* 连接方式       : 见连接图
* 注    意		 : 
*******************************************************************************/
#include <reg52.h>	
#include "lcd.h"
#include "74hc164.h"
#include "74hc165.h"

//--定义全局变量--//
unsigned char code PuZh[16] = " Pechin Science ";
unsigned char code CnCh[27] = "Welcome to the world of MCU";

/*******************************************************************************
* 函 数 名         : Delay10ms
* 函数功能		   : 延时函数，延时10ms
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a, b;
	//--c已经在传递过来的时候已经赋值了，所以在for语句第一句就不用赋值了--//
    for (;c>0;c--)
	{
		for (b=38;b>0;b--)
		{
			for (a=130;a>0;a--);
		}          
	}       
}

void demo()
{
	unsigned char i;
	//--写第一行
	LcdWriteCom(0x80); //设置坐标在第一行
	for(i=0; i<16; i++)
	{
		LcdWriteData(PuZh[i]);	
	}

	//--写第二行
	LcdWriteCom(0xC0); //设置坐标在第二行
	for(i=0; i<27; i++)
	{
		LcdWriteData(CnCh[i]);
	}

	LcdWriteCom(0x07); //设置每写一次整屏右移
	while(1)
	{			
		LcdWriteCom(0xC0); //设置坐标在第二行
		for(i=0; i<27; i++)
		{
			LcdWriteData(CnCh[i]);
			Delay10ms(50);
		}
	}				
}


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	hc164_init();
	hc165_init();

	LcdInit();

	disp_keycode();
    while(1)
	{
		key=read_hc165();
		if(key!=0xff)
		{
			disp_keycode();
		}
	} 


	while(1);
	{
		//demo();
		disp_demo();
	}
}

