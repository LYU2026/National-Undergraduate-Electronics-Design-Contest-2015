#include<reg52.h>//11.0952M
sbit P0_0=P0^0;
sbit P0_1=P0^1;
unsigned char Data0[10]="1234dd",Data1[10]="dd4321";
unsigned int j=0;
void delay(unsigned int i);
void UART_Send_Byte(unsigned char ucData);
void UART_Send_Enter();
void UART_Send_Str(char *pStr);
void main()
{
	SCON=0x40;
	/*PCON&= 0x7f;
	TMOD|=0x20;
	TH1=0xfa; TL1=0xfa;*/
  PCON&= 0x7f;
	TMOD|=0x20;
	TH1=0xf3; TL1=0xf3;
	TR1=1;
	ES=1;
	EA=1;
 //UART_Send_Byte(11);
		UART_Send_Str("dkdhdifj454");
//	UART_Send_Enter();
	
	while(1){UART_Send_Str("dkdhdifj454");}
}
	
	
	
void UART_Send_Byte(unsigned char ucData)
{
 TI = 0;
 SBUF = ucData;
 while(!TI);
 TI = 0;
}


void UART_Send_Enter()
{
 UART_Send_Byte(0x0d);
 UART_Send_Byte(0x0a);
}

void UART_Send_Str(char *pStr)
{
    
	while(*pStr != '\0')
	{
		UART_Send_Byte(*pStr++);
	}
	UART_Send_Enter();

}
 void uar() interrupt 4
{
	if(RI) {RI=0;}
	else TI=0; 
}
