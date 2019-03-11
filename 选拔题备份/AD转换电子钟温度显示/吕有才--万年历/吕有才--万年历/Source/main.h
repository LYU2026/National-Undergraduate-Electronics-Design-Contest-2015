#include<reg51.h>
#define uchar unsigned char
#define uint  unsigned int

unsigned char Time0[9],DisplayData[8],Data0[5];
uchar code  DIS1[] = {"DATE:"},
            DIS2[] = {"TIME:"},
            DIS3[] = {"TEMP:"},
						DIS4[] = {"AD1:"};
void delay_main(int ms);
void readtime();
void Lcd12864Display();
void ClockPlay();
void LcdDisplayTemp0(int temp);
void ADDisplay();						
void delay0(unsigned int n);						
						