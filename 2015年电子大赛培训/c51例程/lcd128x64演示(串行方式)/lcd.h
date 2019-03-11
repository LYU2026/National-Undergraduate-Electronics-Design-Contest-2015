
extern unsigned char lcd_buffer[16];

extern void HZLCD_Init(void);

extern void send_data(unsigned char dat);
extern void write_data(unsigned char dat);            //向LCD写数据
extern void write_comm(unsigned char dat);			  //向LCD写命令
extern void screen(unsigned char *dat);               //LCD屏幕显示文本

extern void lcd_demo(void);                  //LCD演示

extern void disp_line(unsigned char line,unsigned char *dat);
