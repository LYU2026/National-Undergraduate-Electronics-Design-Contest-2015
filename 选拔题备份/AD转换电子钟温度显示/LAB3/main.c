//连线：
//LCD1602_RS = P3^6;	LCD1602_RW = P3^5;	LCD1602_EN = P3^7;	LCDPORT P0
// DS1302_CLK = P2^0; DS1302_IO = P2^1; DS1302_RST = P2^2;



#include<reg51.h>
#include"lcd.h"
#include"ds1302.h"
//#include"8DigitalTubeDisplay.h"



extern unsigned char code ucInitTime[]={0x10,0x20,0x15,0x12,0x04,0x04,0x11}; 	//初始化后设置为:15年04月2日星期4 19点49分00秒
                                       /* 秒 分 时 日 月 星期 年 */ 
void LcdDisplay();

void main()
{	

	 LCD1602_Init();	  //液晶初始化
	 DS1302_WriteOneByteAtAddr(0x8e,0x80);  
  DS1302_SetInit(ucInitTime);	
   while(1)
	 {
		 LcdDisplay();
		}
}

void LcdDisplay()
{
		while(1)
	{

	  LCD1602_DisplayOneCharOnAddr(0,0,('D'));
		LCD1602_DisplayOneCharOnAddr(0,1,('a'));
		LCD1602_DisplayOneCharOnAddr(0,2,'t');
		LCD1602_DisplayOneCharOnAddr(0,3,'a');
		LCD1602_DisplayOneCharOnAddr(0,4,':');
		LCD1602_DisplayOneCharOnAddr(0,5,'2');
	  LCD1602_DisplayOneCharOnAddr(0,6,'0');
    LCD1602_DisplayOneCharOnAddr(0,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)/16));			//年
		LCD1602_DisplayOneCharOnAddr(0,8,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
	  LCD1602_DisplayOneCharOnAddr(0,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)/16));			//月
	  LCD1602_DisplayOneCharOnAddr(0,11,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
  	LCD1602_DisplayOneCharOnAddr(0,13,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)/16));			//日
		LCD1602_DisplayOneCharOnAddr(0,14,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)%16));	
    LCD1602_DisplayOneCharOnAddr(0,15,0x02);
		LCD1602_DisplayOneCharOnAddr(1,0,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)/16));				//时
	  LCD1602_DisplayOneCharOnAddr(1,1,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)%16));				 
	  LCD1602_DisplayOneCharOnAddr(1,2,(':'));
    LCD1602_DisplayOneCharOnAddr(1,3,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)/16));				//分
	  LCD1602_DisplayOneCharOnAddr(1,4,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)%16));	
	  LCD1602_DisplayOneCharOnAddr(1,5,(':'));
	  LCD1602_DisplayOneCharOnAddr(1,6,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)/16));				//秒
    LCD1602_DisplayOneCharOnAddr(1,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)%16));
    LCD1602_DisplayOneCharOnAddr(1,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_WEEK_R)%16));	//星期		
    LCD1602_DisplayOneCharOnAddr(1,13,0x03);
    LCD1602_DisplayOneCharOnAddr(1,14,0x04);
    LCD1602_DisplayOneCharOnAddr(1,15,0x05);	 
  }
}

