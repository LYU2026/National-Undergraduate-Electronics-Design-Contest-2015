#include <reg52.h>
#define FOSC    11095200L
#define BAUD    4800
void InitUart();
void SendData(unsigned char dat);