//LCD1602_RS = P3^6;	LCD1602_RW = P3^5;	LCD1602_EN = P3^7;	LCDPORT P0
//DS1302_CLK = P2^0; DS1302_IO = P2^1; DS1302_RST = P2^2;
//矩阵键盘 P1
//P3^3=外部中断1

#include<reg51.h>
#include"lcd.h"
#include"ds1302.h"
//#include"8DigitalTubeDisplay.h"
extern unsigned char code ucInitTime[]={0x58,0x59,0x23,0x31,0x12,0x04,0x25}; 	//初始化后设置为:15年04月2日星期4 19点49分00秒
  	 unsigned char Time0[9];                                     /* 秒 分 时 日 月 星期 年 */ 


void LcdDisplay();
void LcdDisplay0();

void delay0(n)
{
	while(n--);
}
unsigned char keyboardscan()
{
	unsigned char board_h, board_l;
	P1=0x0f;
	board_h=P1&0x0f;
	if(board_h!=0x0f)
	{
		delay0(5);
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

void main()
{	

	 LCD1602_Init();	  //液晶初始化
	 DS1302_WriteOneByteAtAddr(0x8e,0x80);  
 // DS1302_SetInit(ucInitTime);	

   while(1)
	 {
		 LcdDisplay();
		}
}


void LcdDisplay()
{
	         Time0[7]=0x20;
		while(1)
	{

				   Time0[0]=      (DS1302_ReadOneByteAtAddr(ADDR_SEC_R));
           Time0[1]= (DS1302_ReadOneByteAtAddr(ADDR_MIN_R));
           Time0[2]=   (DS1302_ReadOneByteAtAddr(ADDR_HOUR_R));
           Time0[3]=  (DS1302_ReadOneByteAtAddr(ADDR_DAY_R));
        	 Time0[4]= (DS1302_ReadOneByteAtAddr(ADDR_MONTH_R));
           Time0[5]=  (DS1302_ReadOneByteAtAddr(ADDR_WEEK_R));
		       Time0[6]=  (DS1302_ReadOneByteAtAddr(ADDR_YEAR_R));
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
	  LCD1602_DisplayOneCharOnAddr(0,0,('D'));
		LCD1602_DisplayOneCharOnAddr(0,1,('a'));
		LCD1602_DisplayOneCharOnAddr(0,2,'t');
		LCD1602_DisplayOneCharOnAddr(0,3,'a');
		LCD1602_DisplayOneCharOnAddr(0,4,':');
		LCD1602_DisplayOneCharOnAddr(0,5,'0'+Time0[7]/16);
	  LCD1602_DisplayOneCharOnAddr(0,6,'0'+Time0[7]%16);
    LCD1602_DisplayOneCharOnAddr(0,7,('0'+Time0[6]/16));			//年
		LCD1602_DisplayOneCharOnAddr(0,8,('0'+Time0[6]%16));
	  LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
	  LCD1602_DisplayOneCharOnAddr(0,10,('0'+Time0[4]/16));			//月
	  LCD1602_DisplayOneCharOnAddr(0,11,('0'+Time0[4]%16));
	  LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
  	LCD1602_DisplayOneCharOnAddr(0,13,('0'+Time0[3]/16));			//日
		LCD1602_DisplayOneCharOnAddr(0,14,('0'+Time0[3]%16));	
    LCD1602_DisplayOneCharOnAddr(0,15,0x02);
		LCD1602_DisplayOneCharOnAddr(1,0,('0'+Time0[2]/16));				//时
	  LCD1602_DisplayOneCharOnAddr(1,1,('0'+Time0[2]%16));				 
	  LCD1602_DisplayOneCharOnAddr(1,2,(':'));
    LCD1602_DisplayOneCharOnAddr(1,3,('0'+Time0[1]/16));				//分
	  LCD1602_DisplayOneCharOnAddr(1,4,('0'+Time0[1]%16));	
	  LCD1602_DisplayOneCharOnAddr(1,5,(':'));
	  LCD1602_DisplayOneCharOnAddr(1,6,('0'+Time0[0]/16));				//秒
    LCD1602_DisplayOneCharOnAddr(1,7,('0'+Time0[0]%16));
    LCD1602_DisplayOneCharOnAddr(1,10,('0'+Time0[5]%16));	//星期		
    LCD1602_DisplayOneCharOnAddr(1,13,0x03);
    LCD1602_DisplayOneCharOnAddr(1,14,0x04);
    LCD1602_DisplayOneCharOnAddr(1,15,0x05);	 
		switch(keyboardscan())
     {
			 		case 0x77://年加一
					{ 
						LCD1602_Init();	
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

						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
					}
					case 0xb7:
					{ 
						LCD1602_Init();					
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
           			DS1302_WriteOneByteAtAddr(0x8e,0x80); 
					    	DS1302_SetInit(Time0);
						  break;
				  }
				case 0xd7://month add
					{  
       			LCD1602_Init();	
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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				  }
	       case 0xe7:
						{  LCD1602_Init();	
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
             DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;						
				    }		
			
				case 0x7b://day
					{  
       			LCD1602_Init();	
								if(Time0[3]==0x31)
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

						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				  }
	       case 0xbb:
						{  LCD1602_Init();	
       			   
								if(Time0[3]==0x01)
						  {		
								Time0[3]=0x32;
						  }	
                Time0[3]-=0x01;
							if((Time0[3]&0x0f)==0x0f)
					 	{		
								Time0[3]=(Time0[3]&0xf0);
								Time0[3]+=0x09;
						}
             DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;						
				    }
	      	case 0xdb://hour
						{  
       			LCD1602_Init();	
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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;	
				   }
	     	case 0xeb:
								{  
       			LCD1602_Init();	

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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
			   	}	
		  		case 0x7d://min
					{  
       			LCD1602_Init();	
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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
			  	}
	       case 0xbd:
						{  LCD1602_Init();	
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
	
             DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;						
				    }		
		    case 0xdd://sec
 					{  
       			LCD1602_Init();	
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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				}
		    case 0xed:
					{  
       			LCD1602_Init();	
						
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
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				}			
		    case 0xd7e://week
 					{  
       			LCD1602_Init();	
						Time0[5]+=0x01;
							if(Time0[5]==0x08)
								{
									Time0[5]=0x01;
								}
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				}
		    case 0xbe:
					{  
       			LCD1602_Init();	
						Time0[5]-=0x01;
								if(Time0[5]==0x00)
						  {		
								Time0[5]=0x07;
						  }	
						DS1302_WriteOneByteAtAddr(0x8e,0x80); 
						DS1302_SetInit(Time0);
						break;
				}				
    }
  }
}
