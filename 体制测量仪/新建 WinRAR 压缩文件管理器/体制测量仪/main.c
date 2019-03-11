#include<reg52.h>
#include"lcd12864.h"
#include"uart.h"
#include"74HC165.h"
#include"mfrc522.h"
sbit key=P2^0;
sbit sendkey=P2^1;
sbit senddx=P2^2;
sbit SEG_DS = P3^4;	//74HC595芯片的数据引脚
sbit SEG_SHCP = P3^6;	//74HC595芯片的控制引脚，上升沿移入数据
sbit SEG_STCP = P3^5;	//74HC595芯片的控制引脚，上升沿更新数据

unsigned char xdata name[]="姓名:";
unsigned char xdata height[]="身高:";
unsigned char xdata sex[]="性别:";
unsigned char xdata num[]="序号:";
unsigned char xdata lung[]="肺活量:";
unsigned char xdata weight[]="体重:";
unsigned char xdata result[]="优秀良好及格不及格极差";
unsigned char xdata welcome[]="欢迎使用请刷卡";
unsigned char xdata message[3][19]={{"杨成  男13090041066"},
                                 {"李智勤男13090041031"},
						         {"吕有才男13090041039"}
                                };
unsigned char xdata cardcode[][4]={0xE8,0xBF,0x75,0xB9,
                            0xDC,0x57,0x84,0x12,
							0x0C,0xC7,0x8D,0x12
							};
unsigned char xdata phonenumber[]={"15966841805"};
unsigned char sgg[4];
unsigned char tzz[4];
unsigned char fhll[4];
unsigned char xdata english[]="Height:Weight:Lung volume:result:";
unsigned char count;
unsigned char k;
unsigned char receiveData[5];
unsigned char j=0;
unsigned char i;
unsigned char l;
unsigned int a,b,c,d;
int fhl=0;//肺活量
float tz=0;	 //体重
int sg=0,bl=70;
bit showready=1;
bit card=0;
bit sd=0;
bit message_key=0;
unsigned char UID[5],Temp[4];
void scankey();
void InitializeSystem();
void Auto_Reader(void);
void sendandroid();
void sendshortmessage();
void keboardscan();
void main()
{
  LCD12864_Init();
  InitializeSystem();
  while(!card)
  {
  	LCD12864_SetWindow(0,2);
  	for(i=0;i<8;i++)
  		LCD12864_WriteData(welcome[i]);
	LCD12864_SetWindow(1,3);
	for(i=8;i<14;i++)
  		LCD12864_WriteData(welcome[i]);
	Auto_Reader();
  }
  LCD12864_WriteCmd(0x01);
  while(1)
  {
   scankey();
   if(count==0)
   {
   		Auto_Reader();
       LCD12864_SetWindow(0,0);
       for(i=0;i<5;i++)
      {
        LCD12864_WriteData(name[i]);
      }
	  LCD12864_WriteData(' ');
       LCD12864_SetWindow(1,0);
      for(i=0;i<5;i++)
      {
	  LCD12864_WriteData(sex[i]);
      }
	  LCD12864_WriteData(' ');
      LCD12864_SetWindow(2,0);
	  scankey();
   		Auto_Reader();
      for(i=0;i<5;i++)
     {
	  LCD12864_WriteData(num[i]);
      }
	  LCD12864_WriteData(' ');
	  if(card&&sd==0)
	  {
	  	  InitUart();
	  	 for(i=0;i<4;i++)
		 {
		 	if(cardcode[i][0]==UID[0])
				{
					k=i;
					LCD12864_SetWindow(0,3);
					for(l=0;l<6;l++)
					{
						LCD12864_WriteData(message[i][l]);
//						SendData(message[i][l]);
					}
					LCD12864_SetWindow(1,3);
//					SendData(0x0a);
					for(l=6;l<8;l++)
					{
						LCD12864_WriteData(message[i][l]);
//						SendData(message[i][l]);	
					}
					LCD12864_SetWindow(3,0);
//					SendData(0x0a);
					for(l=8;l<19;l++)
					{
						LCD12864_WriteData(message[i][l]);
						SendData(message[i][l]);	
					}
					SendData(0x0a);
				}	
		 }
		 sd=1;
	  }
   }
	scankey();
    if(count==1)
   {
	LCD12864_SetWindow(0,0);
	  for(i=0;i<5;i++)
     {
	  LCD12864_WriteData(height[i]);
     }
	 LCD12864_WriteData(' ');
	LCD12864_SetWindow(1,0);
	  for(i=0;i<5;i++)
     {
	  LCD12864_WriteData(weight[i]);
     }
	 LCD12864_SetWindow(2,0);
	  for(i=0;i<7;i++)
     {
	  LCD12864_WriteData(lung[i]);
     }
	 LCD12864_WriteData(' ');
	 scankey();
	 if(!showready)
	 {
	 	if(receiveData[0]=='@')
		{
			LCD12864_SetWindow(0,3);
			for(i=1;i<4;i++)
			{
				LCD12864_WriteData(receiveData[i]);
				sgg[i-1]=receiveData[i];
			}
			LCD12864_WriteData('.');
			LCD12864_WriteData(receiveData[4]);
			sgg[3]=receiveData[4];
			LCD12864_WriteData('c');
			LCD12864_WriteData('m');
				
		}
		if(receiveData[0]=='#')
		{
			LCD12864_SetWindow(1,3);
			for(i=1;i<3;i++)
			{
				LCD12864_WriteData(receiveData[i]);
				tzz[i-1]=receiveData[i];
			}
			tzz[2]=receiveData[3];
			tzz[3]=receiveData[4];
			LCD12864_WriteData('.');
			LCD12864_WriteData(receiveData[3]);
			LCD12864_WriteData(receiveData[4]);
			LCD12864_WriteData('K');
			LCD12864_WriteData('g');		
		}
		if(receiveData[0]=='$')
		{
			LCD12864_SetWindow(2,4);
			for(i=1;i<5;i++)
			{
				LCD12864_WriteData(receiveData[i]);
				fhll[i-1]=receiveData[i];
			}
			LCD12864_WriteData('m');
			LCD12864_WriteData('l');	
		}
		showready=1;		
	 }
	 if(sendkey==0)
	 {
	 	sendandroid();
		if(bl>=78)
		{
			LCD12864_SetWindow(3,3);
			for(i=0;i<4;i++)
				LCD12864_WriteData(result[i]);
		}
		else if(bl>=66)
		{
			LCD12864_SetWindow(3,3);
			for(i=4;i<8;i++)
				LCD12864_WriteData(result[i]);
		}
		else if(bl>=55)
		{
			LCD12864_SetWindow(3,3);
			for(i=8;i<12;i++)
				LCD12864_WriteData(result[i]);
		}
		else if(bl>=47)
		{
			LCD12864_SetWindow(3,3);
			for(i=12;i<18;i++)
				LCD12864_WriteData(result[i]);
		}
		else if(bl<47)
		{
			LCD12864_SetWindow(3,3);
			for(i=18;i<22;i++)
				LCD12864_WriteData(result[i]);
		}		 	
	 }
	 if(senddx==0)
	 {
		 message_key=1;
		 while(message_key)
		 {
			 for(i=0;i<11;)
			 {
				 switch(hc74_165())
				 {
					 case 0x77: {phonenumber[i]=0;i++;break;}
					 case 0x7b: {phonenumber[i]=1;i++;break;}
					 case 0x7d: {phonenumber[i]=2;i++;break;}
					 case 0x7e: {phonenumber[i]=3;i++;break;}
					 case 0xb7: {phonenumber[i]=4;i++;break;}
					 case 0xbb: {phonenumber[i]=5;i++;break;}
					 case 0xbd: {phonenumber[i]=6;i++;break;}
					 case 0xbe: {phonenumber[i]=7;i++;break;}
					 case 0xdb: {phonenumber[i]=8;i++;break;}
					 case 0xde: {phonenumber[i]=9;i++;break;}
				 }
			 }
			 if(i==11) {message_key=0;break;}
		 }
	 	sendshortmessage();
		sendandroid();
	 }	  
   }
  }
}

void scankey()
{ 
  if(key==0)
  {LCD12864_WriteCmd(0x01);
     do{}while(key==0);
     count++;
     if(count>1)
	 {
	  count=0;

	 }
  }
  
}
void Usart() interrupt 4
{
	if(RI==1)
	{
		if(showready)
		{
			receiveData[j]=SBUF;
			RI = 0;
			j++;
			if(j>=5)
			{
				j=0;
				showready=0;
			}
		}
		else
			RI=0;
	}	
}
void Auto_Reader(void)
{
    if(PcdRequest(0x52,Temp)==MI_OK)  //寻卡，成功后Temp数组为卡类型
    {
      if(PcdAnticoll(UID)==MI_OK)	  //防冲突，UID数组数据为卡序列号
      { 
        card=1;            
      }
    } 
}

void InitializeSystem()
{
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;	
	EA=1;
	ES=1;
    PcdReset();
    PcdAntennaOff(); 
    PcdAntennaOn();  
	M500PcdConfigISOType( 'A' );
}
void sendandroid()
{
		a=fhll[0]-'0';
		b=fhll[1]-'0';
		c=fhll[2]-'0';
		d=fhll[3]-'0';
		fhl=a*1000+b*100+c*10+d;
		a=tzz[0]-'0';
		b=tzz[1]-'0';
		c=tzz[2]-'0';
		d=tzz[3]-'0';
		tz=a*10+b+c*0.1+d*0.01;
		bl=fhl/tz;
		for(i=26;i<33;i++)
		{
			SendData(english[i]);
		}
		if(bl>=78)
		{
			SendData('A');
		}
		else if(bl>=66)
		{
			SendData('B');
		}
		else if(bl>=55)
		{
			SendData('C');
		}
		else if(bl>=47)
		{
			SendData('D');
		}
		else if(bl<47)
		{
			SendData('E');
		}
		SendData(0x0a);
		SendData('0'+bl/10);
		SendData('0'+bl%10);
		for(i=0;i<7;i++)
		{
			SendData(english[i]);
		}
		for(i=0;i<4;i++)
		{
			SendData(sgg[i]);
		}
		SendData(0x0a);
		for(i=7;i<14;i++)
		{
			SendData(english[i]);
		}
		for(i=0;i<4;i++)
		{
			SendData(tzz[i]);
		}
		SendData(0x0a);
		for(i=14;i<26;i++)
		{
			SendData(english[i]);
		}
		for(i=0;i<4;i++)
		{
			SendData(fhll[i]);
		}
		SendData(0x0a);	
}
void sendshortmessage()
{
		SendData('D');
		SendData('X');
		for(i=0;i<11;i++)
		{
			SendData(phonenumber[i]);
		}
		for(l=0;l<6;l++)
		{
			SendData(message[k][l]);
		}
		SendData(0x0a);
		for(l=6;l<8;l++)
		{
			SendData(message[k][l]);	
		}
		LCD12864_SetWindow(3,0);
		SendData(0x0a);
		for(l=8;l<19;l++)
		{
			SendData(message[k][l]);	
		}
		SendData(0x0a);
}
void keyboardscan()
{
	
}
