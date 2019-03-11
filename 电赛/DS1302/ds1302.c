#include<reg52.h>
#include"ds1302.h"

/******************************************************************************
函数名称：DS1302_WriteOneByte
函数功能：向DS1302写入一个字符
入口参数：ucData-数据
返回值：无
备注：无
*******************************************************************************/
void DS1302_WriteOneByte(unsigned char uData) 
{ 
    unsigned char i;
    ACC = uData;
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
void DS1302_WriteOneByteAtAddr(unsigned char uAddr, unsigned char uData)
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(uAddr);      
    DS1302_WriteOneByte(uData);      
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
unsigned char DS1302_ReadOneByteAtAddr(unsigned char uAddr)
{
    unsigned char ucBackValue;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302_WriteOneByte(uAddr);             
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
    unsigned char uAddr = 0x80; 
    DS1302_WriteOneByteAtAddr(0x8e,0x00);           /* 控制命令,WP=0,写操作*/
    for(i =7; i>0; i--)
    { 
        DS1302_WriteOneByteAtAddr(uAddr,*pClk);  /* 秒 分 时 日 月 星期 年 */ 
        pClk++;
        uAddr +=2;
    }
    DS1302_WriteOneByteAtAddr(0x8e,0x80);           /* 控制命令,WP=1,写保护*/
}