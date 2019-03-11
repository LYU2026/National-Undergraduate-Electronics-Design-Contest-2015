#include<reg52.h>
#include"lcd.h"
#include"ds1302.h"
#include"main1.h"
#include<intrins.h>
void daydata()   //������������
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

unsigned char checkday()                  //�Զ�����Ƿ������꣬��Ϊ��Ӧ���·���������
{
	unsigned char day;
	unsigned int year;
	year=Time0[7]/16*1000+Time0[7]%16*100+Time0[6]/16*10+Time0[6]%16;
	
  if(((year%4==0)&&(year%100==0))||(year%400==0))//�ж��Ƿ�������
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


void timesetting()
{	 
	//LcdDisplay();
	//LCD1602_MoveToPosition(1,9);
	//LCD1602_DisplayString(p);
	switch(keyboardscan(1))
	{
			case 0x77:     //���°���1�����������
			{
				 LCD1602_MoveToPosition(0,9);
         LCD1602_WriteInformation(0x0f,0);
				 while (1)
				 {
						switch(keyboardscan(1))
					  {
							case 0x7d://����K9�������1
						   { year(0);break;}
							case 0x7e:
					    	{year(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xb7://����K2 ���������
			{
				  LCD1602_MoveToPosition(0,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(100))
					  {
						  case 0x7d://����K9�������1
						      {   month(0);break;	}
						  case 0x7e:
                 {month(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xd7://����K3 �������day
			{
				  LCD1602_MoveToPosition(0,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						   case 0x7d://����K9�������1
						      {  day(0);break;	}
						   case 0x7e://K13
						      {day(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xe7://����K4 �������hour
			{
				  LCD1602_MoveToPosition(1,9);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://����K9�������1
						        {  hour(0);break;}
						    case 0x7e:
						       {hour(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0x7b://����K5 �������min
			{
          LCD1602_MoveToPosition(1,12);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						    case 0x7d://����K9�������1
						       {  min(0);break;	}
						    case 0x7e:
						       {min(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			case 0xbb://����K6 �����sec
			{
				  LCD1602_MoveToPosition(1,15);
           LCD1602_WriteInformation(0x0f,0);
					while (1)
					{
						switch(keyboardscan(1))
					  {
						     case 0x7d://����K9�������1
						       { sec(0);break;	}
						     case 0x7e:
						       {sec(1);break;}
					  }
				   if(keyboardscan(1)==0xed)
			     {break;}
				 }
			}
			 
   }
}


void year(unsigned char k)
{
	if(k==0)                //��һ
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
	if(k==1)                 //��һ
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
	DS1302_SetInit(Time0);                 //��ʱ�ӽ�������
  DS1302_WriteOneByteAtAddr(0x8e,0x80);
   LcdDisplay();
	LCD1602_MoveToPosition(0,9);           //�������������˸
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
	 LCD1602_MoveToPosition(0,12);
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
	LCD1602_MoveToPosition(0,15);
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
	  LCD1602_MoveToPosition(1,9);
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
  LCD1602_MoveToPosition(1,12);
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
	LCD1602_MoveToPosition(1,15);
  LCD1602_WriteInformation(0x0f,0);
}




