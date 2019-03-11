//外部中断0
//P3.2接KEY1   P2接键盘

//往main函数添加
TCON=0x0c;
EA=1;EX1=1;


//interrupt 
void time() interrupt 0
{
	while(1)
	{
		switch(keyboardscan())
     {
		//case 0x77:
		//		DisplayOneCharOnAddr(0,8); break;
		case 0xb7://年加一
				{  
       			Time[6]=ucInitTime[6]+0xf0; 
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
       			Time[5]=ucInitTime[5]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}		
		case 0x7b:
						{  
       			Time[5]=ucInitTime[5]-0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				    }	
		case 0xbb://day
							{  
       			Time[4]=ucInitTime[4]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	     // DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}	
		case 0xdb:
								{  
       			Time[4]=ucInitTime[4]-0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
//DS1302_SetInit(Time);	
				//		LcdDisplay();	
				}	 
		case 0xeb://week
								{  
       			Time[3]=ucInitTime[3]+0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0x7d:
								{  
       			Time[3]=ucInitTime[3]-0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xbd://hour
								{  
       			Time[2]=ucInitTime[2]+0xf0; 
					//	DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	    //  DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xdd:
								{  
       			Time[2]=ucInitTime[2]-0xf0; 
				//		DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	   //   DS1302_SetInit(Time);	
					//	LcdDisplay();	
				}
		case 0xed://min
		       			Time[1]=ucInitTime[1]+0xf0; 
		case 0x7e:
       			Time[1]=ucInitTime[1]-0xf0; 
		case 0xbe://sec
       			Time[0]=ucInitTime[0]+0xf0; 
		case 0xde:
	       			Time[0]=ucInitTime[0]-0xf0; 
	//	case 0xee:
			//	DisplayOneCharOnAddr(15,8); break; 
		default: 
									DS1302_WriteOneByteAtAddr(0x8e,0x80); 
    	      DS1302_SetInit(Time);	
						LcdDisplay();	break;
      }	
    show();			
	}
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