#include <reg51.h>
sbit HC165_CK  = P1^6;
sbit HC165_OUT = P1^5;
sbit HC165_nPL = P1^7;
void HC165(void);