#include<reg51.h>
unsigned char code ucInitTime[]={0x50,0x57,0x22,0x01,0x04,0x03,0x15}; 	//��ʼ��������Ϊ:11��03��23������4 03��46��55��

#define ADDR_SEC_R 0x81 
#define ADDR_MIN_R 0x83
#define ADDR_HOUR_R 0x85
#define ADDR_DAY_R 0x87
#define ADDR_MONTH_R 0x89
#define ADDR_YEAR_R 0x8D
#define ADDR_WEEK_R 0x8B

#define ADDR_SEC_W 0x80 
#define ADDR_MIN_W 0x82
#define ADDR_HOUR_W 0x84
#define ADDR_DAY_W 0x86
#define ADDR_MONTH_W 0x88
#define ADDR_YEAR_W 0x8C
#define ADDR_WEEK_W 0x8A


sbit DS1302_CLK = P2^0;
sbit DS1302_IO = P2^1;
sbit DS1302_RST = P2^2;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;

void DS1302_WriteOneByte(unsigned char d); //��DS1302��д��һ������
unsigned char DS1302_ReadOneByte(void);	   //��DS1302����һ������
void DS1302_WriteOneByteAtAddr(unsigned char ucAddr, unsigned char ucDa);	//��ָ����ַд��һ������
unsigned char DS1302_ReadOneByteAtAddr(unsigned char ucAddr);		//��ָ��һ��λ�ö�ȡһ������
void DS1302_SetInit(unsigned char *pClk);		 //��ʼ��ʱ��

unsigned char  code Data[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//��
														0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,//��
														0x1f,0x11,0x11,0x1f,0x11,0x11,0xff,0x00,//��
														0x0e,0x0a,0x0e,0x00,0x1f,0x11,0x11,0x1f,//��
	                          0x04,0x1f,0x08,0x1f,0x09,0x0f,0x09,0x09,
														0x02,0x02,0x1f,0x06,0x0a,0x12,0x02,0x06};

sbit LCD1602_RS = P3^6;	//λ���壬Һ��������/����ѡ��
sbit LCD1602_RW = P3^5;	//λ���壬Һ���Ķ�дѡ��
sbit LCD1602_EN = P3^7;	//λ���壬Һ��ʹ���ź�

#define LCDPORT P0	  //Һ�������ݿ�

unsigned char code ucForum[]=" bbs.cepark.com ";	  //��CODE������һ��������ʾ�ĳ����ַ���

void Delay(unsigned int uiCount);	//��ʱ����
void LCD1602_CheckBusy(void);	//Һ��æ���
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
void LCD1602_Init(void);	//Һ����ʼ��
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Һ���������ƶ���ָ��λ��
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//��Һ��ָ��λ����ʾ�ַ�
void LCD1602_DisplayString(unsigned char *ucStr);	//��Һ������ʾ�ַ���


void main()
{	
	 LCD1602_Init();	  //Һ����ʼ��
	 DS1302_WriteOneByteAtAddr(0x8e,0x80);  
   //DS1302_SetInit(ucInitTime);	
	while(1)
	{

	  LCD1602_DisplayOneCharOnAddr(0,0,('D'));
		LCD1602_DisplayOneCharOnAddr(0,1,('a'));
		LCD1602_DisplayOneCharOnAddr(0,2,'t');
		LCD1602_DisplayOneCharOnAddr(0,3,'a');
		LCD1602_DisplayOneCharOnAddr(0,4,':');
		LCD1602_DisplayOneCharOnAddr(0,5,'2');
	  LCD1602_DisplayOneCharOnAddr(0,6,'0');
    LCD1602_DisplayOneCharOnAddr(0,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)/16));			//��
		LCD1602_DisplayOneCharOnAddr(0,8,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
	  LCD1602_DisplayOneCharOnAddr(0,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)/16));			//��
	  LCD1602_DisplayOneCharOnAddr(0,11,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
  	LCD1602_DisplayOneCharOnAddr(0,13,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)/16));			//��
		LCD1602_DisplayOneCharOnAddr(0,14,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)%16));	
    LCD1602_DisplayOneCharOnAddr(0,15,0x02);
		LCD1602_DisplayOneCharOnAddr(1,0,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)/16));				//ʱ
	  LCD1602_DisplayOneCharOnAddr(1,1,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)%16));				 
	  LCD1602_DisplayOneCharOnAddr(1,2,(':'));
    LCD1602_DisplayOneCharOnAddr(1,3,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)/16));				//��
	  LCD1602_DisplayOneCharOnAddr(1,4,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)%16));	
	  LCD1602_DisplayOneCharOnAddr(1,5,(':'));
	  LCD1602_DisplayOneCharOnAddr(1,6,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)/16));				//��
    LCD1602_DisplayOneCharOnAddr(1,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)%16));
    LCD1602_DisplayOneCharOnAddr(1,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_WEEK_R)%16));	//����		
   LCD1602_DisplayOneCharOnAddr(1,13,0x03);
   LCD1602_DisplayOneCharOnAddr(1,14,0x04);
   LCD1602_DisplayOneCharOnAddr(1,15,0x05);	 
}
}

/******************************************************************************
�������ƣ�DS1302_WriteOneByte
�������ܣ���DS1302д��һ���ַ�
��ڲ�����ucData-����
����ֵ����
��ע����
*******************************************************************************/
void DS1302_WriteOneByte(unsigned char ucData) 
{ 
    unsigned char i;
    ACC = ucData;
    DS1302_RST = 1;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;         
        DS1302_CLK = 0;
        DS1302_CLK = 1;				//��д�����λ��������д��
        ACC = ACC >> 1; 
    } 
}

/******************************************************************************
�������ƣ�DS1302_ReadOneByte
�������ܣ���DS1302��ȡһ������
��ڲ�������
����ֵ����ȡ������
��ע����
*******************************************************************************/
unsigned char DS1302_ReadOneByte(void) 
{ 
    unsigned char i;
    DS1302_RST = 1;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         //�൱�ڻ���е� RRC 
        DS1302_IO = 1;
        DS1302_CLK = 1;
        DS1302_CLK = 0;			   //�½��ض�ȡ���ȶ����λ
        ACC7 = DS1302_IO;
    } 
    return(ACC); 
}

/******************************************************************************
�������ƣ�DS1302_WriteOneByteAtAddr
�������ܣ���DS1302��ָ��λ��д��һ������
��ڲ�����ucAddr-��ַ	  ��  ucData-����
����ֵ����
��ע����
*******************************************************************************/
void DS1302_WriteOneByteAtAddr(unsigned char ucAddr, unsigned char ucData)
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(ucAddr);      
    DS1302_WriteOneByte(ucData);      
    DS1302_CLK = 1;
    DS1302_RST = 0;
    DS1302_IO = 0;
}

/******************************************************************************
�������ƣ�DS1302_ReadOneByteAtAddr
�������ܣ���ָ����λ�ö�ȡһ������
��ڲ�����ucAddr-��ַ
����ֵ����ȡ������
��ע����
*******************************************************************************/
unsigned char DS1302_ReadOneByteAtAddr(unsigned char ucAddr)
{
    unsigned char ucBackValue;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(ucAddr);             
    ucBackValue = DS1302_ReadOneByte();         
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucBackValue);
}

/******************************************************************************
�������ƣ�DS1302_SetInit
�������ܣ����ó�ʼ��
��ڲ�����pClk-��ʼ�������ָ��
����ֵ����
��ע����
*******************************************************************************/
void DS1302_SetInit(unsigned char *pClk) 
{
    unsigned char i;
    unsigned char ucAddr = 0x80; 
    DS1302_WriteOneByteAtAddr(0x8e,0x00);           /* ��������,WP=0,д����*/
    for(i =7; i>0; i--)
    { 
        DS1302_WriteOneByteAtAddr(ucAddr,*pClk);  /* �� �� ʱ �� �� ���� �� */ 
        pClk++;
        ucAddr +=2;
    }
    DS1302_WriteOneByteAtAddr(0x8e,0x80);           /* ��������,WP=1,д����*/
}

/******************************************************************************
�������ƣ�LCD1602_CheckBusy
�������ܣ�æ���
��ڲ�������
����ֵ����
��ע��æ�������˶��һ����ʱ�����İ취�����ó����ڼ�⺯����
*******************************************************************************/
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //��֮ǰ����λ��׼����ȡIO������
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //ʹҺ�����ڶ�����״̬
	LCD1602_EN = 1;	 //ʹ��Һ�����ߵ�ƽ��Ч
	while((i--) && (LCDPORT & 0x80));	//æ���
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_WriteInformation
�������ܣ���LCD1602Һ��д�����ݻ�������
��ڲ�����ucData-Ҫд������ݻ����������
		  bComOrData-Ϊ0ʱд���ʱ���Ϊ1ʱд���ʱ����
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //��д�����ݻ�������ǰ�Ƚ���æ���
	LCDPORT = ucData;		 //�Ƚ����ݻ�����������IO
	LCD1602_RS = bComOrData;	//ȷ����д�����ݻ���д����
	LCD1602_RW = 0;		//ʹҺ������д����Ϣ״̬
	LCD1602_EN = 1;		//ʹ��Һ�����ߵ�ƽ��Ч
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_Init
�������ܣ�Һ����ʼ������
��ڲ�������
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_Init(void)		 
{
	unsigned char i;
	LCD1602_WriteInformation(0x38,0);
	Delay(300);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0); 	//д�����5x7��������ʽ��8λ���ݽӿ�
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//��ʾ���ã�����ʾ����겻��ʾ������˸
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//����ָ��
	Delay(20);
	LCD1602_WriteInformation(0x40,0);	
	for(i=0;i<64;i++)
	{
			LCD1602_WriteInformation(Data[i],1);	
	}
}
  
/******************************************************************************
�������ƣ�LCD1602_MoveToPosition
�������ܣ���Һ���Ĺ���ƶ���ָ����λ��
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
����ֵ����
��ע��ͨ��ָ��x,y��ֵ���Խ�Һ���Ĺ���ƶ���ָ����λ��
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //��궨λ����һ�е�y��
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //��궨�嵽�ڶ��е�y��
}

/******************************************************************************
�������ƣ�LCD1602_DisplayOneCharOnAddr
�������ܣ���ָ����λ������ʾָ�����ַ�
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
		  ucData-Ҫ��ʾ���ַ�����
����ֵ����
��ע��ȷ��x,y��ȡֵҪ��ָ���ķ�Χ��
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //���λ��
	LCD1602_WriteInformation(ucData,1);	  //д������
}

/******************************************************************************
�������ƣ�LCD1602_DisplayString
�������ܣ���ʾ�ַ���
��ڲ�����ucStr-�ַ������׵�ַ
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //�ַ�������֮ǰ��ѭ����ʾ
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //����д��ÿһ���ַ�
		 ucStr++;								 //ָ������
	}
}

/******************************************************************************
�������ƣ�Delay
�������ܣ���ʱ����
��ڲ�����uiCount-��ʱ������ÿ��1����0.5ms
����ֵ����
��ע����
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
