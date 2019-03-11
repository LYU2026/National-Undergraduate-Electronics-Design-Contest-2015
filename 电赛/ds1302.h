#include<reg52.h>

#define ADDR_SEC_R 0x81 
#define ADDR_MIN_R 0x83
#define ADDR_HOUR_R 0x85
#define ADDR_DAY_R 0x87
#define ADDR_MONTH_R 0x89
#define ADDR_YEAR_R 0x8D
#define ADDR_WEEK_R 0x8B

#define ADDR_SEC_W 0x80 
#define ADDR_MIN_W 0x82
#define ADDR_HOUR_W 0x84
#define ADDR_DAY_W 0x86
#define ADDR_MONTH_W 0x88
#define ADDR_YEAR_W 0x8C
#define ADDR_WEEK_W 0x8A


sbit DS1302_CLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_RST = P3^5;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;

void DS1302_WriteOneByte(unsigned char d); //向DS1302中写入一个数据
unsigned char DS1302_ReadOneByte(void);	   //从DS1302读出一个数据
void DS1302_WriteOneByteAtAddr(unsigned char ucAddr, unsigned char ucDa);	//在指定地址写入一个数据
unsigned char DS1302_ReadOneByteAtAddr(unsigned char ucAddr);		//从指定一个位置读取一个数据
void DS1302_SetInit(unsigned char *pClk);		 //初始化时间
