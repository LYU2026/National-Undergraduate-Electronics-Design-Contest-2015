

#include<reg51.h>
#include<intrins.h>
#include"lcd.h"
#include"ds1302.h"
#include"temp.h"
#include"12864.h"
#define  NOP()   _nop_() 
//--定义全局变量

extern   int  Ds18b20ReadTemp();  //读取温度
unsigned char Time0[9],week[3],week0,DisplayData[8], Data0[5];
unsigned char code Seg_Data[]={	 //the information is showed in the digital tube,shows 0-F//共阳数码管的编码，并将数据定义在CODE区 
					        0xc0,/*0*/
					        0xF9,/*1*/
					        0xA4,/*2*/
					        0xB0,/*3*/
					        0x99,/*4*/
					        0x92,/*5*/
					        0x82,/*6*/
					        0xF8,/*7*/
					        0x80,/*8*/
					        0x90,/*9*/				  
							};
unsigned char code  p[]="Setting";	
void year(unsigned char k);//年加减
void month(unsigned char k);//月加减
void day(unsigned char k);
void hour(unsigned char k);
void min(unsigned char k);
void sec(unsigned char k);
void weekset(unsigned char k);//周加减
void timesetting();//电子钟设置
void readtime();  //读取DS13B20中的时间
void LcdDisplay();  //将时间显示在LCD上
void LcdDisplayTemp0(int temp); //将温度显示在LCD

void delay0(unsigned int n)
{
	unsigned int k;
	for(k=2;k>0;k--)
	{
	  while(n--){NOP();}
	}
}

unsigned char keyboardscan(unsigned char n)
{
	unsigned char board_h, board_l;
	P1=0x0f;
	board_h=P1&0x0f;
	if(board_h!=0x0f)
	{
		delay0(n);
		board_h=P1&0x0f;
		if(board_h!=0x0f)
		{
			P1=board_h|0xf0;
			board_l=P1&0xf0;
			return(board_h+board_l);
		}
	}
	return(0xff);
}

void daydata()   //设置天数函数
{
	unsigned char day;
	switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
}

unsigned char checkday()                  //自动检查是否是闰年，并为对应的月份设置天数
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//判断是否是闰年
 {
	  switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x02: day=0x29;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
 }
 else
 {
 switch(Time0[4])
	{
		case 0x01: day=0x31;break;
		case 0x02: day=0x28;break;
		case 0x03: day=0x31;break;
		case 0x04: day=0x30;break;
		case 0x05: day=0x31;break;
		case 0x06: day=0x30;break;
		case 0x07: day=0x31;break;
		case 0x08: day=0x31;break;
		case 0x09: day=0x30;break;
		case 0x10: day=0x31;break;
		case 0x11: day=0x30;break;
		case 0x12: day=0x31;break;
	}
	}
 
 return(day);
}

void main()
{
    unsigned char code p0[]="Please press ",
	                     p1[]= "    Welcome!    ",
	                     p2[]= "   This is a    ",
	                     p3[]="product which is",
	                     p4[]="created by",
	                     p5[]="Mode1: ClockPlay",
                       p6[]="K13",
	                     p7[]="Mode2: ClockInit",
	                     p8[]="K9"
	                         ;

     LCD1602_Init();
	delay0(10);                 //上电，等待稳定
   lcd_init();                 //初始化LCD
	while(1)
	{
		LcdDisplayTemp0(Ds18b20ReadTemp());//显示温度
	}
	   Time0[7]=0x20;
     readtime(); 	
	   LCD1602_MoveToPosition(0,0);
     LCD1602_DisplayString(p1);
		 delay0(20);
	   LCD1602_MoveToPosition(1,0);
	   LCD1602_DisplayString(p2);
		 delay0(20);
	   LCD1602_MoveToPosition(1,0);
	   LCD1602_DisplayString(p3);
		 delay0(20);
		 LCD1602_WriteInformation(0x01,0);	//清屏指令
     LCD1602_MoveToPosition(0,0);
     LCD1602_DisplayString(p1);	 
	   LCD1602_MoveToPosition(1,0);
	   LCD1602_DisplayString(p4);		 
	   LCD1602_DisplayOneCharOnAddr(1,13,0x03);
	   LCD1602_DisplayOneCharOnAddr(1,14,0x04);
	   LCD1602_DisplayOneCharOnAddr(1,15,0x05);	
		 delay0(20);
	   LCD1602_WriteInformation(0x01,0);	//清屏指令
		 while(1)
	 {
			 LCD1602_MoveToPosition(0,0);
	     LCD1602_DisplayString(p5);
       LCD1602_MoveToPosition(1,0);
	     LCD1602_DisplayString(p0);
       LCD1602_MoveToPosition(1,13);
	     LCD1602_DisplayString(p6);	
		   delay0(10);
		   LCD1602_WriteInformation(0x01,0);	//清屏指令
       LCD1602_MoveToPosition(0,0);
	     LCD1602_DisplayString(p7);
       LCD1602_MoveToPosition(1,0);
	     LCD1602_DisplayString(p0);
       LCD1602_MoveToPosition(1,14);
	     LCD1602_DisplayString(p8);
		   delay0(10);
       LCD1602_WriteInformation(0x01,0);	//清屏指令	
		   switch(keyboardscan(1))
		  {
			   case 0xd7:
		     {LCD1602_WriteInformation(0x01,0);	//清屏指令
				    while (1)
           {
               timesetting();   //电子钟设置
					     if(keyboardscan(1)==0xee) break;
           }
				   break;
			   }
				case 0xe7:
				{LCD1602_WriteInformation(0x01,0);	//清屏指令
					while (1)
					{
						readtime();//读取时间
						LcdDisplay();//显示时间
						LcdDisplayTemp0(Ds18b20ReadTemp());//显示温度
						if(keyboardscan(1)==0xee) break;
					}
					break;
				}
		 }
	 
	 }
}


void LcdDisplay()
{  
    	if((Time0[7]&0x0a)==0x0a)    
			{		
			  	Time0[7]=(Time0[7]&0xf0);
		  		Time0[7]+=0x10;
		  }
			if((Time0[7]&0x0f)==0x0f)
			{		
					Time0[7]=(Time0[7]&0xf0);
					Time0[7]+=0x09;
			}
	  	LCD1602_DisplayOneCharOnAddr(0,0,'0'+Time0[7]/16);
	    LCD1602_DisplayOneCharOnAddr(0,1,'0'+Time0[7]%16);
      LCD1602_DisplayOneCharOnAddr(0,2,('0'+Time0[6]/16));			//年
	   	LCD1602_DisplayOneCharOnAddr(0,3,('0'+Time0[6]%16));
	    LCD1602_DisplayOneCharOnAddr(0,4,(0x00));
			LCD1602_DisplayOneCharOnAddr(0,5,('0'+Time0[4]/16));			//月
			LCD1602_DisplayOneCharOnAddr(0,6,('0'+Time0[4]%16));
			LCD1602_DisplayOneCharOnAddr(0,7,(0x01));
			LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[3]/16));			//日
			LCD1602_DisplayOneCharOnAddr(0,9,('0'+Time0[3]%16));	
			LCD1602_DisplayOneCharOnAddr(0,10,0x02);
			LCD1602_DisplayOneCharOnAddr(1,0,('0'+Time0[2]/16));				//时
			LCD1602_DisplayOneCharOnAddr(1,1,('0'+Time0[2]%16));				 
			LCD1602_DisplayOneCharOnAddr(1,2,(':'));
			LCD1602_DisplayOneCharOnAddr(1,3,('0'+Time0[1]/16));				//分
			LCD1602_DisplayOneCharOnAddr(1,4,('0'+Time0[1]%16));	
			LCD1602_DisplayOneCharOnAddr(1,5,(':'));
			LCD1602_DisplayOneCharOnAddr(1,6,('0'+Time0[0]/16));				//秒
			LCD1602_DisplayOneCharOnAddr(1,7,('0'+Time0[0]%16));
			//显示 星期
			week0=('0'+Time0[5]%16);
			switch(week0)
			{
				case 0x31:
					{week[0]=0x4d;week[1]=0x4f;week[2]=0x4e;break;}//"MON";
				case 0x32:
					{week[0]=0x54;  week[1]=0x55; week[2]=0x45;break;}  //"TUE"
				case 0x33:
					{week[0]=0x57; week[1]=0x45; week[2]=0x44;break;}   //"WED"
				case 0x34:
					{week[0]=0x54;week[1]=0x48;week[2]=0x55;break;}   //"THU"
				case 0x35:
					{week[0]=0x46;week[1]=0x52;week[2]=0x49;break;}  //"FRI"
				case 0x36:
					{week[0]=0x53;week[1]=0x41;week[2]=0x54; break;}  //"SAT"
				case 0x37:  
					{week[0]=0x53;week[1]=0x55;week[2]=0x4e;break;} //"SUN"
			}
			LCD1602_DisplayOneCharOnAddr(0,13,week[0]);	//星期		
		  LCD1602_DisplayOneCharOnAddr(0,14,week[1]);	
		  LCD1602_DisplayOneCharOnAddr(0,15,week[2]);	 
}

void timesetting()
{	 
	LcdDisplay();
	LCD1602_MoveToPosition(1,9);
	LCD1602_DisplayString(p);
	switch(keyboardscan(2))
	{
			case 0x77:     //按下按键1，进入调试年
			{
				 LCD1602_MoveToPosition(0,4);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0xdb://按下K9，进入加1
						   { year(0);break;}
							case 0xeb:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://按下K2 进入调试月
			{
				  LCD1602_MoveToPosition(0,7);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						  case 0xdb://按下K9，进入加1
						      {   month(0);break;	}
						  case 0xeb:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7d://按下K3 进入调试day
			{
				  LCD1602_MoveToPosition(0,10);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0xdb://按下K9，进入加1
						      {  day(0);break;	}
						   case 0xeb://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7e://按下K4 进入调试hour
			{
				  LCD1602_MoveToPosition(1,1);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0xdb://按下K9，进入加1
						        {  hour(0);break;}
						    case 0xeb:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://按下K5 进入调试min
			{
          LCD1602_MoveToPosition(1,4);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0xdb://按下K9，进入加1
						       {  min(0);break;	}
						    case 0xeb:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://按下K6 进入调sec
			{
				  LCD1602_MoveToPosition(1,7);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0xdb://按下K9，进入加1
						       { sec(0);break;	}
						     case 0xeb:
						       {sec(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbd://按下K7 进入调试week
			{
				
				  LCD1602_MoveToPosition(0,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0xdb://按下K9，进入加1
						        {  weekset(0);break;}
						     case 0xeb:
						        {weekset(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
		  } 
   }
}

void LcdDisplayTemp0(int temp) 	 //lcd显示
{
	unsigned char i; unsigned char T[5];
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x40; 
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由�0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	//P3=tp;

	Data0[0] = temp / 10000;
	Data0[1] = temp % 10000 / 1000;
	Data0[2] = temp % 1000 / 100 ;
	Data0[3] = temp % 100 / 10;
	Data0[4] = temp % 10;
  
	for(i=0;i<=4;i++)
	{
		switch(Seg_Data[Data0[i]])
		{
			case 0xc0:
			{T[i]=0x30;break;}
			case 0xF9:
			{T[i]=0x31;break;}
			case 0xA4:
			{T[i]=0x32;break;}
			case 0xB0:
			{T[i]=0x33;break;}
			case 0x99:
			{T[i]=0x34;break;}
			case 0x92:
			{	T[i]=0x35;break;}
			case 0x82:
			{	T[i]=0x36;break;}
			case 0xF8:
			{T[i]=0x37;break;}
			case 0x80:
			{T[i]=0x38;break;}
			case 0x90:
			{T[i]=0x39;break;}			
		}
	}
	lcd_wcmd(0x80); 
	 for(i=0;i<5;i++)
     {
       lcd_wdat(T[i]);
       delay0(30);
     }
			 LCD1602_DisplayOneCharOnAddr(1,11,T[1]);
	  	LCD1602_DisplayOneCharOnAddr(1,12,T[2]);
	  	LCD1602_DisplayOneCharOnAddr(1,13,0x2e);//.
	  	LCD1602_DisplayOneCharOnAddr(1,14,T[3]);
       LCD1602_DisplayOneCharOnAddr(1,15,0x06);	//°c
}

void year(unsigned char k)
{
	if(k==0)                //加一
	{	
		Time0[6]+=0x01;
		if(Time0[6]==0x9a)
		{
				Time0[6]=0x00;
				Time0[7]+=0x01;
				if((Time0[7]&0x0a)==0x0a)
				{		
						Time0[7]=(Time0[7]&0xf0);
						Time0[7]+=0x10;
			  }
		}
	  if((Time0[6]&0x0a)==0x0a)
		{		
				Time0[6]=(Time0[6]&0xf0);
				Time0[6]+=0x10;
     }	
	}
	if(k==1)                 //减一
	{
		 Time0[6]-=0x01;
		 if((Time0[6]&0x0f)==0x0f)
			{		
					Time0[6]=(Time0[6]&0xf0);
					Time0[6]+=0x09;
			}
			if(Time0[6]==0x00)
			{		
					Time0[6]=0x99;
					Time0[7]-=0x01;
				  if((Time0[7]&0x0f)==0x0f)
					{		
							 Time0[7]=(Time0[7]&0xf0);
							 Time0[7]+=0x09;
					}
			}	
	}
  DS1302_WriteOneByteAtAddr(0x8e,0x00);								
	DS1302_SetInit(Time0);                 //对时钟进行设置
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,3);           //在年的数字上闪烁
  LCD1602_WriteInformation(0x0f,0); 
}

void month(unsigned char k)
{
	if(k==0)
	{
		Time0[4]+=0x01;
		if(Time0[4]==0x13)
		{
				Time0[4]=0x01;
		}
    if((Time0[4]&0x0a)==0x0a)
	  {		
			Time0[4]=(Time0[4]&0xf0);
			Time0[4]+=0x10;
	  }
   }
  if(k==1)
  {
	  Time0[4]-=0x01;
		if(Time0[4]==0x00)
		{		
				Time0[4]=0x12;
		}	
   if((Time0[4]&0x0f)==0x0f)
		{		
				Time0[4]=(Time0[4]&0xf0);
				Time0[4]+=0x09;
		}
  }
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	 LCD1602_MoveToPosition(0,6);
   LCD1602_WriteInformation(0x0f,0);
}

void day(unsigned char k)
{
	 if(k==0)
	 {				
		 if(Time0[3]==checkday())
		 {		
			  Time0[3]=0x00;
		 }	
		Time0[3]+=0x01;
		if((Time0[3]&0x0a)==0x0a)
		{		
				Time0[3]=(Time0[3]&0xf0);
				Time0[3]+=0x10;
        DS1302_SetInit(Time0);
		}
	}
	if(k==1)
  {
		 if(Time0[3]==0x01)
			{		
					Time0[3]=checkday()+0x01;
			}	
      Time0[3]-=0x01;
			if((Time0[3]&0x0f)==0x0f)
			{		
			  	Time0[3]=(Time0[3]&0xf0);
					Time0[3]+=0x09;
			}
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(0,9);
  LCD1602_WriteInformation(0x0f,0);
}
void hour(unsigned char k)
{
	 if(k==0)
	 {
		 Time0[2]+=0x01;
		 if(Time0[2]==0x24)
		 {
				Time0[2]=0x00;
		 }
		if((Time0[2]&0x0a)==0x0a)
		{		
				Time0[2]=(Time0[2]&0xf0);
				Time0[2]+=0x10;
		 }
	 }
	 if(k==1)	
		{
			if(Time0[2]==0x00)
			{		
					Time0[2]=0x24;
			}	
 			Time0[2]-=0x01;
			if((Time0[2]&0x0f)==0x0f)
			{		
					Time0[2]=(Time0[2]&0xf0);
					Time0[2]+=0x09;
			}	
		}				
		DS1302_WriteOneByteAtAddr(0x8e,0x00); 
		DS1302_SetInit(Time0);
	  DS1302_WriteOneByteAtAddr(0x8e,0x80);
    LcdDisplay();
	  LCD1602_MoveToPosition(1,1);
    LCD1602_WriteInformation(0x0f,0);
}

void min(unsigned char k)
{
	if(k==0)
	{
		Time0[1]+=0x01;
		if((Time0[1]&0x0a)==0x0a)
		{		
			 Time0[1]=(Time0[1]&0xf0);
			 Time0[1]+=0x10;
		}
		if(Time0[1]==0x60)
		{
				Time0[1]=0x00;
		}
	}
	if(k==1)
	{
		if(Time0[1]==0x00)
		{		
				Time0[1]=0x60;
		}
    Time0[1]-=0x01;
    if((Time0[1]&0x0f)==0x0f)
		{		
				Time0[1]=(Time0[1]&0xf0);
				Time0[1]+=0x09;
		}	
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
  LCD1602_MoveToPosition(1,4);
  LCD1602_WriteInformation(0x0f,0);
}
void sec(unsigned char k)
{
	if(k==0)
	{
		Time0[0]+=0x01;
		if((Time0[0]&0x0a)==0x0a)
		{		
			 Time0[0]=(Time0[0]&0xf0);
			Time0[0]+=0x10;
    }
		if(Time0[0]==0x60)
		{
					Time0[0]=0x00;
		}
	}
	if(k==1)
  {
		if(Time0[0]==0x00)
		{		
				Time0[0]=0x60;
		}	
    Time0[0]-=0x01;
		if((Time0[0]&0x0f)==0x0f)
		{		
					Time0[0]=(Time0[0]&0xf0);
					Time0[0]+=0x09;
		}	
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(1,7);
  LCD1602_WriteInformation(0x0f,0);
}

void weekset(unsigned char k)
{
	if(k==0)
	{
		Time0[5]+=0x01;
		if(Time0[5]==0x08)
		{
				Time0[5]=0x01;
		}
	}
	if(k==1)
	{	
		Time0[5]-=0x01;
		if(Time0[5]==0x00)
		 {		
				Time0[5]=0x07;
		}	
	}
	DS1302_WriteOneByteAtAddr(0x8e,0x00); 
	DS1302_SetInit(Time0);
	DS1302_WriteOneByteAtAddr(0x8e,0x80);
  LcdDisplay();
	LCD1602_MoveToPosition(0,15);
  LCD1602_WriteInformation(0x0f,0);
}
void readtime()
{
	   Time0[0]= (DS1302_ReadOneByteAtAddr(ADDR_SEC_R));
     Time0[1]= (DS1302_ReadOneByteAtAddr(ADDR_MIN_R));
     Time0[2]= (DS1302_ReadOneByteAtAddr(ADDR_HOUR_R));
     Time0[3]= (DS1302_ReadOneByteAtAddr(ADDR_DAY_R));
     Time0[4]= (DS1302_ReadOneByteAtAddr(ADDR_MONTH_R));
     Time0[5]= (DS1302_ReadOneByteAtAddr(ADDR_WEEK_R));
 	   Time0[6]= (DS1302_ReadOneByteAtAddr(ADDR_YEAR_R));
}


