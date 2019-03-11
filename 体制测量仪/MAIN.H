//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC522
sbit     MF522_RST  =    P1^4;                   //RC500片选
sbit     MF522_NSS  =    P1^0;
sbit     MF522_SCK  =    P1^1;
sbit     MF522_SI   =    P1^2;
sbit     MF522_SO   =    P1^3;
sbit     ff         =    P0^0;    
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();                                                                   
#define OSC_FREQ          12000000L
#define  RCAP2_50us      65536L - OSC_FREQ/40417L
#define  RCAP2_1ms       65536L - OSC_FREQ/2000L
#define  RCAP2_10ms      65536L - OSC_FREQ/1200L
#define  TIME0_500us     65536L - OSC_FREQ/8000L
#define  TIME0_10ms      65536L - OSC_FREQ/200
#define CALL_isr_UART()         TI = 1
#define TRUE 1
#define FALSE 0

