
extern unsigned char lcd_buffer[16];

extern void HZLCD_Init(void);

extern void send_data(unsigned char dat);
extern void write_data(unsigned char dat);            //��LCDд����
extern void write_comm(unsigned char dat);			  //��LCDд����
extern void screen(unsigned char *dat);               //LCD��Ļ��ʾ�ı�

extern void lcd_demo(void);                  //LCD��ʾ

extern void disp_line(unsigned char line,unsigned char *dat);
