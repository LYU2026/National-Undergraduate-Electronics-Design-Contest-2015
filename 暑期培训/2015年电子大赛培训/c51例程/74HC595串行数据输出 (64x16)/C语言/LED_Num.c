/*****************************************************************************************
*          ²ÉÓÃ4½Æ¬74HC595ÊµÏÖLE16*16DµãÕóµÄ¶¯Ì¬É¨ÃèÏÔÊ¾£¨¹²Ñô¼«LdE1818µãÕó)              
*		                             Ê¹ÓÃTYD-51-1°å			                             *
*  Á¬½Ó·½·¨£º                                                                            *
*            sbit SDAT = P1^0;  //´®ĞĞÊı¾İÊä³ö£¬¶¨ÒåÔÚP1.0ÉÏ                             *
*            sbit SCLK = P1^1;  //´®ĞĞÒÆÎ»Ê±ÖÓÊä³ö£¬¶¨ÒåÔÚP1.1ÉÏ                         *
*            sbit LOAD = P1^2;  //74HC595Ëø´æÂö³åÊä³ö£¬¶¨ÒåÔÚP1.2ÉÏ                      *
******************************************************************************************/
#include <reg52.h> 
#include <intrins.h>

sbit ADDR_A = P1^0;  //74HC138µØÖ·
sbit ADDR_B = P1^1;
sbit ADDR_C = P1^2;
sbit ADDR_D = P1^3;
sbit SDAT = P1^4;  //´®ĞĞÊı¾İÊä³ö£¬¶¨ÒåÔÚP1.4ÉÏ
sbit SCLK = P1^5;  //´®ĞĞÒÆÎ»Ê±ÖÓÊä³ö£¬¶¨ÒåÔÚP1.5ÉÏ
sbit LOAD = P1^6;  //74HC595Ëø´æÂö³åÊä³ö£¬¶¨ÒåÔÚP1.6ÉÏ
sbit EN = P1^7;

#define COUNT 2500

unsigned char line_addr=0;
unsigned int code lie_tab[]={0x0001,0x0002,0x0004,0x0008,
                             0x0010,0x0020,0x0040,0x0080,
                             0x0100,0x0200,0x0400,0x0800,
							 0x1000,0x2000,0x4000,0x8000
};

unsigned char idata buffer[128];

unsigned char code hz1[]={
/*--  ÎÄ×Ö:  ÀÊ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x08,0x00,0x10,0x3E,0x7E,0x22,0x42,0x22,0x42,0x22,0x7E,0x3E,0x42,0x22,0x42,0x22,
0x7E,0x22,0x02,0x3E,0x12,0x22,0x22,0x22,0x5A,0x21,0x46,0x21,0x82,0x28,0x40,0x10,

/*--  ÎÄ×Ö:  ãå  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x00,0x02,0x04,0x02,0x08,0x02,0x08,0x02,0xE1,0x7F,0x02,0x02,0x12,0x07,0x10,0x07,
0x88,0x0A,0x88,0x0A,0x47,0x12,0x24,0x22,0x14,0x42,0x04,0x02,0x04,0x02,0x00,0x02,

/*--  ÎÄ×Ö:  ÄÉ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x08,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0xD2,0x7F,0x4F,0x44,0x48,0x44,0x44,0x44,
0x42,0x4A,0x5F,0x52,0x42,0x51,0xC0,0x40,0x58,0x40,0x47,0x40,0x42,0x50,0x40,0x20,

/*--  ÎÄ×Ö:  ĞÂ  --*/
/*--  ËÎÌå12;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=16x16   --*/
0x08,0x00,0x10,0x20,0xFE,0x1E,0x00,0x02,0x44,0x02,0x28,0x02,0xFF,0x7E,0x10,0x12,
0x10,0x12,0xFE,0x12,0x10,0x12,0x54,0x12,0x92,0x12,0x11,0x11,0x14,0x11,0x88,0x10,




};



unsigned long x=0;

int hz=0,time=50000;

unsigned char code hz_dot[]={
/*--  ÎÄ×Ö:  ÀÊ  --*/
/*--  ËÎÌå11;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=15x15   --*/
/*--  ¿í¶È²»ÊÇ8µÄ±¶Êı£¬ÏÖµ÷ÕûÎª£º¿í¶Èx¸ß¶È=16x15  --*/
0x08,0x00,0x10,0x3E,0x7E,0x22,0x42,0x22,0x42,0x22,0x7E,0x3E,0x42,0x22,0x42,0x22,
0x7E,0x3E,0x0A,0x22,0x12,0x22,0x2A,0x21,0x46,0x21,0x82,0x38,0x00,0x00,

/*--  ÎÄ×Ö:  ãå  --*/
/*--  ËÎÌå11;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=15x15   --*/
/*--  ¿í¶È²»ÊÇ8µÄ±¶Êı£¬ÏÖµ÷ÕûÎª£º¿í¶Èx¸ß¶È=16x15  --*/
0x00,0x02,0x04,0x02,0x08,0x02,0xD0,0x3F,0x00,0x02,0x02,0x07,0x04,0x07,0x90,0x0A,
0x90,0x0A,0x48,0x12,0x28,0x22,0x04,0x02,0x04,0x02,0x00,0x02,0x00,0x00,

/*--  ÎÄ×Ö:  ÄÉ  --*/
/*--  ËÎÌå11;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=15x15   --*/
/*--  ¿í¶È²»ÊÇ8µÄ±¶Êı£¬ÏÖµ÷ÕûÎª£º¿í¶Èx¸ß¶È=16x15  --*/
0x08,0x02,0x08,0x02,0x04,0x02,0xD2,0x1F,0x5E,0x12,0x48,0x12,0x44,0x12,0x42,0x15,
0xDE,0x18,0x40,0x10,0x40,0x10,0x58,0x10,0x46,0x10,0x40,0x1C,0x00,0x00,

/*--  ÎÄ×Ö:  ĞÂ  --*/
/*--  ËÎÌå11;  ´Ë×ÖÌåÏÂ¶ÔÓ¦µÄµãÕóÎª£º¿íx¸ß=15x15   --*/
/*--  ¿í¶È²»ÊÇ8µÄ±¶Êı£¬ÏÖµ÷ÕûÎª£º¿í¶Èx¸ß¶È=16x15  --*/
0x08,0x30,0x10,0x0E,0xFE,0x02,0x44,0x02,0x28,0x02,0xFE,0x3E,0x00,0x12,0x10,0x12,
0xFE,0x12,0x10,0x12,0x54,0x12,0x92,0x12,0x10,0x11,0x9C,0x10,0x00,0x00,







};


unsigned int timecount;

unsigned char cnta;

unsigned char cntb;

unsigned char disp_count;


/**************************************************************************
*
*                      ÑÓÊ±´¦Àí
*
****************************************************************************/
void delay(unsigned long dly)
{
    while(dly--);
}

//**********************************************************************

void send_byte(unsigned char dat)
{
   
    unsigned char i,x;
	x=dat;
	for(i=0;i<8;i++)
	{
		if((x & 0x80)!=0)
		{
			SDAT=1;
		}
		else
		{
			SDAT=0;
		}
		SCLK=1;

		SCLK=0;
		x=x<<1;
	}
}


void send_4x595out(unsigned char dat1,unsigned char dat2,unsigned char dat3,unsigned char dat4)
{
    unsigned long dly;
	send_byte(dat4);
	send_byte(dat3);
	send_byte(dat2);
	send_byte(dat1);
    LOAD=0;
	dly++;
	LOAD=1;
}

void send_8x595out(unsigned char dat1,unsigned char dat2,unsigned char dat3,unsigned char dat4,unsigned char dat5,unsigned char dat6,unsigned char dat7,unsigned char dat8)
{
    unsigned long dly;
	send_byte(dat8);
	send_byte(dat7);
	send_byte(dat6);
	send_byte(dat5);
	send_byte(dat4);
	send_byte(dat3);
	send_byte(dat2);
	send_byte(dat1);
    LOAD=0;
	dly++;
	LOAD=1;
}

//////////////////////////////////
char high_H8=0x80,high_L8=0x80,width_L8=0x00,width_H8=0x01;
char flg=0;
//////////////////////////////////
void demo(void)
{
	while(1)
	{
		send_4x595out(high_L8^0xff,high_H8^0xff,width_L8^0xff,width_H8^0xff);
		delay(time);
		high_H8=high_H8>>1;
		if((high_H8==0)&&(flg==0))
		{
			high_L8=0x80;
			flg=1;
		}
		else
			high_L8=high_L8>>1;
		if((high_L8==0)&&(flg==1))
		{
			high_H8=0x80;
			flg=0;
		}
	}			
}

void get_dotdata(void)
{
	unsigned char i,j=0;
    
	for(i=0;i<16;i++)
	{
        buffer[j]=~hz1[i*2];j++;
		buffer[j]=~hz1[i*2+1];j++;
		buffer[j]=~hz1[32+i*2];j++;
		buffer[j]=~hz1[32+i*2+1];j++;
        buffer[j]=~hz1[64+i*2];j++;
		buffer[j]=~hz1[64+i*2+1];j++;
		buffer[j]=~hz1[96+i*2];j++;
		buffer[j]=~hz1[96+i*2+1];j++;
	}
}

//*************************************************
void main(void)
{
    unsigned char i,j;
   
    EN=0;

    get_dotdata();

/*	while(1)
	{
	    line_addr=0;

		for(i=0;i<16;i++)
		{	
			send_8x595out(~dot1[i*8+0],~dot1[i*8+1],~dot1[i*8+2],~dot1[i*8+3],~dot1[i*8+4],~dot1[i*8+5],~dot1[i*8+6],~dot1[i*8+7]);
		
	
 			P1=(P1 & 0xf0)+line_addr;
         	line_addr++;
		

		}

	}*/

	while(1)
	{
	    line_addr=0;
//        P1=(P1 & 0xf0)+line_addr;
//		send_8x595out(~0x01,~0x03,~0x07,~0x0f,~0x1f,~0x3f,~0x7f,~0xff);

		for(i=0;i<16;i++)
		{
			send_8x595out(buffer[i*8+0],buffer[i*8+1],buffer[i*8+2],buffer[i*8+3],buffer[i*8+4],buffer[i*8+5],buffer[i*8+6],buffer[i*8+7]);
 			P1=(P1 & 0xf0)+line_addr;
			line_addr++;
//			delay(5);
		}

	}
/*
	TMOD=0x01;
	TH0=(65536-COUNT)/256;
	TL0=(65536-COUNT)%256;
	TR0=1;	   //¿ªÆô¶¨Ê±0
	ET0=1;
	LOAD=1;
    SCLK=0;
	SDAT=0;
//	EA=1;	   //¿ªÆôÖĞ¶Ï
	cntb=0;
    send_4x595out(0,0,0,0);
	send_4x595out(0,255,0,255);

    while(1);
	{   
        for(j=0;j<16;j++)
		{
		    send_4x595out((~lie_tab[j]%256),(~lie_tab[j]/256),~hz_dot[j*2+1+hz*2],~hz_dot[j*2+hz*2]);
		    delay(10);
		}
		x++;
		if(x>2)
		{
			hz++;
			x=0;
			if(hz>111)
			{
				hz=0;
			}
		}

	}//*/	 
}

/*************************************************
*
*	   ¶¨Ê±ÖĞ¶Ï
********************************************************/		                                    
void t0(void) interrupt 1 using 0
{
	unsigned char x1,x2;

  	TH0=(65536-COUNT)/256;	 //¶¨Ê±Æ÷¸ßÎ»×°ÔØÊı¾İ
  	TL0=(65536-COUNT)%256;	 //¶¨Ê±Æ÷µÍÎ»×°ÔØÊı¾İ


}



