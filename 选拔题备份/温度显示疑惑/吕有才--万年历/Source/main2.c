//通过按键来调整时间

#include<reg51.h>
#include"lcd.h"
#include"ds1302.h"
#include"8DigitalTubeDisplay.h"

void LcdDisplay();
void show();
unsigned char TIME[13];
extern unsigned char code ucInitTime[]={0x10,0x20,0x15,0x12,0x04,0x04,0x11}; 	//初始化后设置为:15年04月2日星期4 19点49分00秒
                                       /* 秒 分 时 日 月 星期 年 */ 
unsigned char code Time[]={0x10,0x20,0x15,0x12,0x04,0x04,0x11};

void delay(unsigned char n)
{
	while(n--);
}

unsigned char keyboardscan(void)
{
	unsigned char board_h, board_l;
	P2=0x0f;
	board_h=P1&0x0f;
	if(board_h!=0x0f)
	{
		delay(5);
		board_h=P1&0x0f;
		if(board_h!=0x0f)
		{
			P2=board_h|0xf0;
			board_l=P2&0xf0;
			return(board_h+board_l);
		}
	}
	return(0xff);
}

/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/


void main()
{	
	 TCON=0x04;
   EA=1;EX0=1;
 	 LCD1602_Init();
	 DS1302_WriteOneByteAtAddr(0x8e,0x80);  
   DS1302_SetInit(ucInitTime);	
	while(1)
	{		LcdDisplay();}
}
/*******************************************************************************
* 函数名         : LcdDisplay()
* 函数功能		   : 显示函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void LcdDisplay(void)
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

void show()
{
		while(1)
	{

  TIME[0]=DS1302_ReadOneByteAtAddr(ADDR_SEC_R)/16;
	TIME[1]=DS1302_ReadOneByteAtAddr(ADDR_SEC_R)%16;
  TIME[2]=DS1302_ReadOneByteAtAddr(ADDR_MIN_R)/16;
	TIME[3]=DS1302_ReadOneByteAtAddr(ADDR_MIN_R)%16;		
  TIME[4]=DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)/16;
	TIME[5]=DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)%16;		
  TIME[6]=DS1302_ReadOneByteAtAddr(ADDR_DAY_R)/16;
	TIME[7]=DS1302_ReadOneByteAtAddr(ADDR_DAY_R)%16;	
  TIME[8]=DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)/16;
	TIME[9]=DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)%16;
  TIME[10]=DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)/16;
	TIME[11]=DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)%16;		
  TIME[12]=DS1302_ReadOneByteAtAddr(ADDR_WEEK_R)%16;
		
	LcdDisplay();	

	DisplayOneCharOnAddr(TIME[4],0);
	DisplayOneCharOnAddr(TIME[5],1);
	DisplayOneCharOnAddr(16,2);
	DisplayOneCharOnAddr(TIME[2],3);
	DisplayOneCharOnAddr(TIME[3],4);
	DisplayOneCharOnAddr(16,5);
	DisplayOneCharOnAddr(TIME[0],6);
	DisplayOneCharOnAddr(TIME[1],7);
  

	}
}
void time() interrupt 2
{
	while(1)
	{
		switch(keyboardscan())
     {
		//case 0x77:
		//		DisplayOneCharOnAddr(0,8); break;
		case 0xb7://年加一
				{  
       			Time[6]=Time[6]+0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xd7://年减1
				{  
       			Time[6]=ucInitTime[6]-0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}				
		case 0xe7://month add
					{  
       			Time[5]=Time[5]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}		
		case 0x7b:
						{  
       			Time[5]=Time[5]-0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				    }	
		case 0xbb://day
							{  
       			Time[4]=Time[4]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	     // DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}	
		case 0xdb:
								{  
       			Time[4]=Time[4]-0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
//DS1302_SetInit(Time);	
				//		LcdDisplay();	
				}	 
		case 0xeb://week
								{  
       			Time[3]=Time[3]+0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0x7d:
								{  
       			Time[3]=Time[3]-0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xbd://hour
								{  
       			Time[2]=Time[2]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xdd:
								{  
       			Time[2]=Time[2]-0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xed://min
		       			Time[1]=Time[1]+0xf0; 
		case 0x7e:
       			Time[1]=Time[1]-0xf0; 
		case 0xbe://sec
       			Time[0]=Time[0]+0xf0; 
		case 0xde:
	       			Time[0]=Time[0]-0xf0; 
	//	case 0xee:
			//	DisplayOneCharOnAddr(15,8); break; 
		default: 
									DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	      DS1302_SetInit(Time);	
						LcdDisplay();	break;
      }	
    show();	
    while(keyboardscan()==0x77)			
			break;
	}
}

