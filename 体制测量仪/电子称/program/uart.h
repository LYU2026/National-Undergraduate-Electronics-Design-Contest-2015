#include <reg52.h>
#define FOSC    11095200L
#define BAUD    9600
void InitUart();
void SendData(unsigned char dat);