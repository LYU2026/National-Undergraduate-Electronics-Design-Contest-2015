#include <reg52.h>
#define FOSC    11059200L
#define BAUD    9600
void InitUart();
void SendData(unsigned char dat);