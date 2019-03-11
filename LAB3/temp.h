#include<reg51.h>
sbit DQ = P2^3;	//温度传感器引脚




void Delay_us(unsigned int uiUs);	//us延时函数,12MHZ晶振有效
void Delay_ms(unsigned int uiMs);	//ms延时函数,12MHZ晶振有效

void DS18B20_Init(void);   //DS18B20初始化
unsigned char DS18B20_ReadOneChar(void);   //读取一个数据
void DS18B20_WriteOneChar(unsigned char dat);	  //写入一个数据
unsigned int DS18B20_ReadTemperature();		