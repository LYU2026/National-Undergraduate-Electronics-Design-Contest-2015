/*======================================================
             青岛通宇达电子科技有限公司开发
                     硬件：TYD-51-1
          功能：8位发光管闪烁两次，然后依次亮
          有问题请致电技术支持杨工：18764250687
======================================================*/

#include "reg51.h"

#define DLY_TIME 50000

void delay(long dly)            //延时子程序
{
	while(dly--);	
}

void main(void)
{
	char i;
	unsigned char x;
	while(1)
	{
		P1=0x00;                //8位发光管全不亮
		delay(DLY_TIME);           //延时约1秒
	    P1=0xff;                //8位发光管全亮
		delay(DLY_TIME);
		P1=0x00;
		delay(DLY_TIME);
	    P1=0xff;
		delay(DLY_TIME);
		x=1;                     //置显示初值
		for(i=0;i<8;i++)         //发光管依次点亮
		{
			P1=~x;
			delay(DLY_TIME);
			x=x<<1;
		}
	}
}
