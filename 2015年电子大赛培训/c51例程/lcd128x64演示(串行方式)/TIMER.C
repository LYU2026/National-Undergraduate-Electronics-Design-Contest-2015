#include	"reg52.h"
#include	"VIiC_C51.H"	//包含VI2C软件包
#include    "zlg7290.h"

#define	PCF8563	0xA2		//定义器件地址
#define	WRADDR	0x00		//定义写单元首地址
#define	RDADDR	0x02		//定义读单元首地址

sbit	KEY_INT=P3^2;
sbit	RST=P1^0;
unsigned char disp_buf[8]={0,0,0,0,0,0,0,0};//显示缓存
unsigned char KEY;							//保存键值
bit	swich_date=0;
unsigned char display_time(unsigned char  *sd)
{ 

	sd[0]=sd[0]&0x7f;//秒屏蔽保留位
	sd[1]=sd[1]&0x7f;//分屏蔽保留位
	sd[2]=sd[2]&0x3f;//时屏蔽保留位
  	disp_buf[0] =(sd[0]%16);			
    disp_buf[1] =(sd[0]/16);		
    disp_buf[2] = 31;
    disp_buf[3] =(sd[1]%16);
    disp_buf[4] =(sd[1]/16);
    disp_buf[5] = 31;
    disp_buf[6] =(sd[2]%16);	
    disp_buf[7] =(sd[2]/16);
    ZLG7290_SendBuf(disp_buf,8);
	return 0;
}
unsigned char display_date(unsigned char  *sd)
{ 
	sd[0]=sd[0]&0x3f;//日屏蔽保留位
	sd[2]=sd[2]&0x1f;//月屏蔽保留位
	disp_buf[0] =(sd[0]%16);			
    disp_buf[1] =(sd[0]/16);		
    disp_buf[2] =(sd[2]%16)+0x80;//后面加小数点
    disp_buf[3] =(sd[2]/16);
    disp_buf[4] =(sd[3]%16)+0x80;//后面加小数点
    disp_buf[5] =(sd[3]/16);
   	disp_buf[6] =0;	
    disp_buf[7] =2;
    ZLG7290_SendBuf(disp_buf,8);
	return 0;
}
unsigned char DelayNS(unsigned char  no)
{ 
	unsigned char  i,j;					//延时参数

  	for(; no>0; no--)
  		for(i=0; i<100; i++)
     		for(j=0; j<100; j++);
  	return 0; 
}
void	main()
{
	unsigned char code td[9]={0x00,0x12,0x00,0x30,0x12,0x06,0x05,0x02,0x04};	//定义初始化字
	unsigned char rd[7];	//定义接收缓冲区
	RST=0;
	DelayNS(1);
	RST=1;
	ISendStr(PCF8563,WRADDR,td,0x5);	//初始化PCF8563,如果需要的话可以不去掉
	DelayNS(1);
	ISendStr(PCF8563,WRADDR+5,&td[5],0x4);//初始化PCF8563,如果需要的话可以不去掉
	while(1)
	{
		DelayNS(1);
		IRcvStr(PCF8563,RDADDR,rd,0x7);	//读现在的时、分、秒
		DelayNS(1);
		if(swich_date)
			display_date(rd+3);	//调显示日期子程序
		else
			display_time(rd);	//调显示时间子程序
		if(KEY_INT==0)
		{
			KEY=ZLG7290_GetKey();
			if(KEY==1)
				swich_date=~swich_date;
		}
		
	}
}
