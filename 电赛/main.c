#include<reg52.h>
#include"lcd.h"

void main()
{
	unsigned char  p0[]="DATE:";
	LCD1602_Init();
  while(1)
	{
		LCD1602_MoveToPosition(0,0);
           LCD1602_DisplayString(p0);
		LCD1602_DisplayOneCharOnAddr(0,8,'A');
		
	}		
}