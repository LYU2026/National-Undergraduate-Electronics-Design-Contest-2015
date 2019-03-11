#include<reg51.h>
unsigned char code ucInitTime[]={0x50,0x57,0x22,0x01,0x04,0x03,0x15}; 	//初始化后设置为:11年03月23日星期4 03点46分55秒

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

void DS1302_WriteOneByte(unsigned char d); //向DS1302中写入一个数据
unsigned char DS1302_ReadOneByte(void);	   //从DS1302读出一个数据
void DS1302_WriteOneByteAtAddr(unsigned char ucAddr, unsigned char ucDa);	//在指定地址写入一个数据
unsigned char DS1302_ReadOneByteAtAddr(unsigned char ucAddr);		//从指定一个位置读取一个数据
void DS1302_SetInit(unsigned char *pClk);		 //初始化时间

unsigned char  code Data[]={0x04,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//年
														0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,//月
														0x1f,0x11,0x11,0x1f,0x11,0x11,0xff,0x00,//日
														0x0e,0x0a,0x0e,0x00,0x1f,0x11,0x11,0x1f,//吕
	                          0x04,0x1f,0x08,0x1f,0x09,0x0f,0x09,0x09,
														0x02,0x02,0x1f,0x06,0x0a,0x12,0x02,0x06};

sbit LCD1602_RS = P3^6;	//位定义，液晶的数据/命令选择
sbit LCD1602_RW = P3^5;	//位定义，液晶的读写选择
sbit LCD1602_EN = P3^7;	//位定义，液晶使能信号

#define LCDPORT P0	  //液晶的数据口

unsigned char code ucForum[]=" bbs.cepark.com ";	  //在CODE区定义一个用于显示的常量字符串

void Delay(unsigned int uiCount);	//延时函数
void LCD1602_CheckBusy(void);	//液晶忙检测
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//在液晶上写数据或者写命令，0为命令，1为数据
void LCD1602_Init(void);	//液晶初始化
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//液晶的坐标移动到指定位置
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//在液晶指定位置显示字符
void LCD1602_DisplayString(unsigned char *ucStr);	//在液晶上显示字符串


void main()
{	
	 LCD1602_Init();	  //液晶初始化
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
    LCD1602_DisplayOneCharOnAddr(0,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)/16));			//年
		LCD1602_DisplayOneCharOnAddr(0,8,('0'+DS1302_ReadOneByteAtAddr(ADDR_YEAR_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,9,(0x00));
	  LCD1602_DisplayOneCharOnAddr(0,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)/16));			//月
	  LCD1602_DisplayOneCharOnAddr(0,11,('0'+DS1302_ReadOneByteAtAddr(ADDR_MONTH_R)%16));
	  LCD1602_DisplayOneCharOnAddr(0,12,(0x01));
  	LCD1602_DisplayOneCharOnAddr(0,13,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)/16));			//日
		LCD1602_DisplayOneCharOnAddr(0,14,('0'+DS1302_ReadOneByteAtAddr(ADDR_DAY_R)%16));	
    LCD1602_DisplayOneCharOnAddr(0,15,0x02);
		LCD1602_DisplayOneCharOnAddr(1,0,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)/16));				//时
	  LCD1602_DisplayOneCharOnAddr(1,1,('0'+DS1302_ReadOneByteAtAddr(ADDR_HOUR_R)%16));				 
	  LCD1602_DisplayOneCharOnAddr(1,2,(':'));
    LCD1602_DisplayOneCharOnAddr(1,3,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)/16));				//分
	  LCD1602_DisplayOneCharOnAddr(1,4,('0'+DS1302_ReadOneByteAtAddr(ADDR_MIN_R)%16));	
	  LCD1602_DisplayOneCharOnAddr(1,5,(':'));
	  LCD1602_DisplayOneCharOnAddr(1,6,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)/16));				//秒
    LCD1602_DisplayOneCharOnAddr(1,7,('0'+DS1302_ReadOneByteAtAddr(ADDR_SEC_R)%16));
    LCD1602_DisplayOneCharOnAddr(1,10,('0'+DS1302_ReadOneByteAtAddr(ADDR_WEEK_R)%16));	//星期		
   LCD1602_DisplayOneCharOnAddr(1,13,0x03);
   LCD1602_DisplayOneCharOnAddr(1,14,0x04);
   LCD1602_DisplayOneCharOnAddr(1,15,0x05);	 
}
}

/******************************************************************************
函数名称：DS1302_WriteOneByte
函数功能：向DS1302写入一个字符
入口参数：ucData-数据
返回值：无
备注：无
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
        DS1302_CLK = 1;				//先写入最低位，上升沿写入
        ACC = ACC >> 1; 
    } 
}

/******************************************************************************
函数名称：DS1302_ReadOneByte
函数功能：从DS1302读取一个数据
入口参数：无
返回值：读取的数据
备注：无
*******************************************************************************/
unsigned char DS1302_ReadOneByte(void) 
{ 
    unsigned char i;
    DS1302_RST = 1;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         //相当于汇编中的 RRC 
        DS1302_IO = 1;
        DS1302_CLK = 1;
        DS1302_CLK = 0;			   //下降沿读取，先读最低位
        ACC7 = DS1302_IO;
    } 
    return(ACC); 
}

/******************************************************************************
函数名称：DS1302_WriteOneByteAtAddr
函数功能：在DS1302的指定位置写入一个数据
入口参数：ucAddr-地址	  ；  ucData-数据
返回值：无
备注：无
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
函数名称：DS1302_ReadOneByteAtAddr
函数功能：在指定的位置读取一个数据
入口参数：ucAddr-地址
返回值：读取的数据
备注：无
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
函数名称：DS1302_SetInit
函数功能：设置初始化
入口参数：pClk-初始化数组的指针
返回值：无
备注：无
*******************************************************************************/
void DS1302_SetInit(unsigned char *pClk) 
{
    unsigned char i;
    unsigned char ucAddr = 0x80; 
    DS1302_WriteOneByteAtAddr(0x8e,0x00);           /* 控制命令,WP=0,写操作*/
    for(i =7; i>0; i--)
    { 
        DS1302_WriteOneByteAtAddr(ucAddr,*pClk);  /* 秒 分 时 日 月 星期 年 */ 
        pClk++;
        ucAddr +=2;
    }
    DS1302_WriteOneByteAtAddr(0x8e,0x80);           /* 控制命令,WP=1,写保护*/
}

/******************************************************************************
函数名称：LCD1602_CheckBusy
函数功能：忙检测
入口参数：无
返回值：无
备注：忙检测采用了多加一个延时条件的办法，放置程序卡在检测函数中
*******************************************************************************/
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //读之前先置位，准备读取IO口数据
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //使液晶处于读数据状态
	LCD1602_EN = 1;	 //使能液晶，高电平有效
	while((i--) && (LCDPORT & 0x80));	//忙检测
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_WriteInformation
函数功能：向LCD1602液晶写入数据或者命令
入口参数：ucData-要写入的数据或者命令参数
		  bComOrData-为0时写入的时命令，为1时写入的时数据
返回值：无
备注：无
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //在写入数据或者命令前先进行忙检测
	LCDPORT = ucData;		 //先将数据或者命令送至IO
	LCD1602_RS = bComOrData;	//确定是写入数据还是写命令
	LCD1602_RW = 0;		//使液晶处于写入信息状态
	LCD1602_EN = 1;		//使能液晶，高电平有效
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_Init
函数功能：液晶初始化函数
入口参数：无
返回值：无
备注：无
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
	LCD1602_WriteInformation(0x38,0); 	//写入命令，5x7点阵工作方式，8位数据接口
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//显示设置，开显示，光标不显示，不闪烁
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//清屏指令
	Delay(20);
	LCD1602_WriteInformation(0x40,0);	
	for(i=0;i<64;i++)
	{
			LCD1602_WriteInformation(Data[i],1);	
	}
}
  
/******************************************************************************
函数名称：LCD1602_MoveToPosition
函数功能：将液晶的光标移动到指定的位置
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
返回值：无
备注：通过指定x,y的值可以将液晶的光标移动到指定的位置
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //光标定位到第一行的y列
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //光标定义到第二行的y列
}

/******************************************************************************
函数名称：LCD1602_DisplayOneCharOnAddr
函数功能：在指定的位置上显示指定的字符
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
		  ucData-要显示的字符数据
返回值：无
备注：确保x,y的取值要在指定的范围内
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //光标位置
	LCD1602_WriteInformation(ucData,1);	  //写入数据
}

/******************************************************************************
函数名称：LCD1602_DisplayString
函数功能：显示字符串
入口参数：ucStr-字符串的首地址
返回值：无
备注：无
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //字符串结束之前，循环显示
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //依次写入每一个字符
		 ucStr++;								 //指针增加
	}
}

/******************************************************************************
函数名称：Delay
函数功能：延时函数
入口参数：uiCount-延时参数，每加1增加0.5ms
返回值：无
备注：无
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}
