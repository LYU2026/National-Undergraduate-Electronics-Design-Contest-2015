#include<reg51.h> 
#include"74HC165.h"
#include<intrins.h> 



unsigned char hc74_165()
{
	unsigned char i,p;
	
	SL_165=0;//当移位/置入控制端S/L为低电平时，并行数据（A－H）被置入寄存器.而时钟（CLK,CLK INH）及串行数据（SER）均无关

	_nop_();
	_nop_();

	SL_165=1;//当S/L为高电平时，并行置数功能被禁止
	delay1_165ms();

	CLK_165=0;

	p=0;
	for(i=0;i<8;i++) 
	{  
		p<<=1;
		p|=QH_165;
		CLK_165=0;
		_nop_();
		_nop_();
		CLK_165=1;//时钟输入端（上升沿有效）
		_nop_();
	
		 
	}

	return(p);	
}



//延时函数
void Delay_165(unsigned int m)
{
	while(m--);
}

 //1ms延时子程序（12MHZ）
void delay1_165ms()
{
	unsigned char i,j;	
	for(i=0;i<10;i++)
	for(j=0;j<33;j++);		 
}

/*void main(void)
{	
	unsigned char i;
		P1=0Xff;
	while(1)
	{ 
		
		for(i = 0;i < 8;i++) //LED流水显示
		{
			P0 = ~(0x01<<i);//P0口数据送74HC165的并口输入端A-H
			Delay_165(60000);
			P1= hc74_165();//P1口接收74HC165的输出数据，并利用LED灯显示出来当前值。
			Delay_165(60000);
		}
		

	}
}
*/
