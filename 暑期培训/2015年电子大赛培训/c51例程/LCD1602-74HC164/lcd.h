
extern unsigned char lcd_buffer[16];

/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
extern void Lcd1602_Delay1ms(unsigned int c);   //误差 0us
/*LCD1602写入8位命令子函数*/
extern void LcdWriteCom(unsigned char com);
/*LCD1602写入8位数据子函数*/	
extern void LcdWriteData(unsigned char dat)	;
/*LCD1602初始化子程序*/		
extern void LcdInit();						  

extern void disp_line(unsigned char line);

extern void disp_demo();

extern void disp_keycode();

