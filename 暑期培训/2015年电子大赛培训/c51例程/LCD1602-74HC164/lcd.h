
extern unsigned char lcd_buffer[16];

/**********************************
��������
**********************************/
/*��51��Ƭ��12MHZʱ���µ���ʱ����*/
extern void Lcd1602_Delay1ms(unsigned int c);   //��� 0us
/*LCD1602д��8λ�����Ӻ���*/
extern void LcdWriteCom(unsigned char com);
/*LCD1602д��8λ�����Ӻ���*/	
extern void LcdWriteData(unsigned char dat)	;
/*LCD1602��ʼ���ӳ���*/		
extern void LcdInit();						  

extern void disp_line(unsigned char line);

extern void disp_demo();

extern void disp_keycode();

