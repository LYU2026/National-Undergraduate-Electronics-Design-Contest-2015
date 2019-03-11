#include<reg52.h> 
#include"hc164.h"



/*------------------------------------------------
                   延时子程序
------------------------------------------------*/
void delay(unsigned int cnt) 
{
 while(--cnt);
}

void HC164(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)
	     {
	       INPUT=temp&0x01;//取最后一位赋值
		   temp>>=1;       //右移
		   CLK=0;          //上升沿作用
		   CLK=1;
		  }
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
/*main()
{
 unsigned char temp;        //作为测试数据
 unsigned char i;
 A=1;//悬空的一端要置高电平
	 
 while(1)
      {
	         temp=0xc0;    //共阳数码管0
	  for(i=0;i<8;i++)
	     {
	       INPUT=temp&0x01;//取最后一位赋值
		   temp>>=1;       //右移
		   CLK=0;          //上升沿作用
		   CLK=1;
		  }
			 delay(30000); 
		   delay(30000);  //演示有助于观察效果
			temp=0xf9;    //共阳数码管1
			for(i=0;i<8;i++)
	     {
	       INPUT=temp&0x01;//取最后一位赋值
		   temp>>=1;       //右移
		   CLK=0;          //上升沿作用
		   CLK=1;
		  }
			delay(30000); 
		   delay(30000);  //演示有助于观察效果
      }
}
*/