#include"temp.h"
#include"intrins.h"
/*******************************************************************************
* 函 数 名         : Delay1ms
* 函数功能		   : 延时函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void DS18B20_Init(void)
{
	unsigned char x=0;
	DQ = 1;    //DQ复位
	Delay_us(10);
	//Delay(8);  //稍做延时,10us
	DQ = 0;    //单片机将DQ拉低
	Delay_us(500);
	//Delay(80); //精确延时 大于 480us ,498us
	DQ = 1;    //拉高总线
	Delay_us(154);
	//Delay(14);	//154us
	x=DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	Delay_us(212);
	//Delay(20); //212us
}



unsigned char DS18B20_ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // 给脉冲信号
		dat>>=1;
		DQ = 1; // 给脉冲信号
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
	DS18B20_WriteOneChar(0xCC); // 跳过读序号列号的操作
	DS18B20_WriteOneChar(0x44); // 启动温度转换
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); //跳过读序号列号的操作
	DS18B20_WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a=DS18B20_ReadOneChar();
	b=DS18B20_ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	//t= t/2; //放大10倍输出并四舍五入---此行没用
	return(t);
}
void Delay_us(unsigned int uiUs)	//us延时函数
{
	for(;uiUs > 0;uiUs--)
	{
		_nop_();
	}	
}



void Delay_ms(unsigned int uiMs)	//ms延时函数
{
	unsigned int i;
	for(;uiMs > 0;uiMs--)
	{
		for(i = 1498;i > 0;i--);
	}
}
