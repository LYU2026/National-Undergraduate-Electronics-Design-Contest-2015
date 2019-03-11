#include <reg52.h>
#define FOSC    12000000L
#define BAUD    4800
void InitUart();
void SendData(unsigned char dat);