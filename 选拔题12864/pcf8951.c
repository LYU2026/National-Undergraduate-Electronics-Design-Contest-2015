/**********************************************************************************
*  标题:       ************* DA-AD试验*******************                         *
*																				  *
*   1.通过本例程了解并掌握AD-DA转换芯片的基本原理和使用 						  *
*   2.了解掌握I2C总线接口的工作原理及一般编程方法。								  *
*   																			  *
*    连接方法：参照接线图							          *
*    																			  *
* 通过改变学习板上的2个电位器对应的2段模拟输入，实现模拟输入，学员观察数码管的数字变化情况
* 通过改D[4]的值，实现模拟输出，学员观察学习板上DA处LED的亮度变化				  *
*				  *																  *
* 注意：I2C驱动程序只能在12T模式下工作   6T模式自己修改相应时序                   *                 	                                                              *
* 请学员认真消化本例程，懂得AD-DA 在C语言中的操作应用                       	  *
***********************************************************************************/

#include<reg52.h>
#include<I2C.h>
#include"ad8591.h"




//else IO
//sbit    LS138A=P3^0;  
//sbit    LS138B=P3^1;
//sbit    LS138C=P3^2;  

/***********************************************
*ADC读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}

//******************************************************************/
/*main()
{  char i,j;

 while(1)
 {/********以下AD-DA处理*************/  
 /*  switch(AD_CHANNEL)
   {
     case 0: ISendByte(PCF8591,0x41);
             D[0]=IRcvByte(PCF8591)*2;  //ADC0 模数转换1  放大2倍显示
			 break;  
 
	 case 1: ISendByte(PCF8591,0x42);
             D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2
			 break;  

	 case 2: ISendByte(PCF8591,0x43);
             D[2]=IRcvByte(PCF8591)*2;  //ADC2	模数转换3
			 break;  

	 case 3: ISendByte(PCF8591,0x40);
             D[3]=IRcvByte(PCF8591)*2;  //ADC3   模数转换4
			 break;  

	 case 4: DACconversion(PCF8591,0x40, D[4]/4); //DAC	  数模转换
	         break;
   }

	      // D[4]=400;  //数字--->>模拟输出
		   D[4]=D[0];  //   把模拟输入 采样的信号 通过数模转换输出
   if(++AD_CHANNEL>4) AD_CHANNEL=0;
*/
 /********以下将AD的值送到LED数码管显示*************/
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
			
	  switch(i)	  //使用switch 语句控制138译码器  也可以是用查表的方式 学员可以试着自己修改				  
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
	     for (j = 0 ; j<90 ;j++) { ;}	   //扫描间隔时间
			  P0 = 0; 
	  }

	   

 }  
}

*/

