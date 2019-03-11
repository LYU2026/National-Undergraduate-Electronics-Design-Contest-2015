#include <stc12c2052ad.h>
//#include<reg52.h>
#define D  P1
sbit RS =P3^0;
sbit RW =P3^1;
sbit E  =P3^2;

void delay(unsigned int n);
void checkbusy(void);
void first(void);
void position(unsigned char x,unsigned char y);
void write(unsigned char ucData, bit b);
void show(unsigned char x,unsigned char y,unsigned char ucData);
void string(unsigned char *ucStr);
//unsigned char code String[]="11AA ";
                             
