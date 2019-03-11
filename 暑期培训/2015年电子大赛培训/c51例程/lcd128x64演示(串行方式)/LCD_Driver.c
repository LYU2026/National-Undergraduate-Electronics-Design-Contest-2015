/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:                LCD_4_4.c
** Last modified Date:       2010-03-04
** Last Version:             
** Description:              功能：4*4键盘的读取
**                      
** 
**--------------------------------------------------------------------------------------------------------
** Created By:               He Zengfu
** Created date:             2010-06-22
** Version:                  V1.0
** Descriptions: 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
/*********************************************************************************************************
**包含的头文件
*********************************************************************************************************/
#include <reg52.h>
#include <string.h>
#include <math.h>
/*********************************************************************************************************
**变量定义
*********************************************************************************************************/
sbit  LCD_CS=P2^6;
sbit  LCD_STD=P2^5;
sbit  LCD_SCLK=P2^4;
/********************************************************************************************************
**外部函数声明
********************************************************************************************************/
extern unsigned char code datPic[];
extern void delay(long dly);
extern void BELL_ON(int Time);
/*********************************************************************************************************
** Function name:       InitLCD
** Descriptions:        对液晶进行初始化
** input parameters:    NONE
**
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void InitLCD(void)
{
    LCD_RST=1;
    LCD_BUSY=1;
    LCD_REQ=0;
    LCD_RST=0;
    delay(10000);
    LCD_RST=1;
}
/*********************************************************************************************************
** Function name:       send_data
** Descriptions:        发送数据
** input parameters:    dat：要发送的数据
**
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void send_data(int dat)
{
   while(LCD_BUSY==1);
   P0=dat;
   LCD_REQ=1;
   while(LCD_BUSY==0);
   LCD_REQ=0;
}
      
/*********************************************************************************************************
** Function name:       write_HZ
** Descriptions:        向屏幕写入汉字，汉字最多个数：4*12（可以是汉字的标点符号）
** input parameters:    x：    写入汉字的横坐标  （0~11）
**                      y：    写入汉字的纵坐标  （0~3）
**                      HZdat：要写入的汉字
**
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
/*void write_HZ(unsigned char x,unsigned char y,unsigned char *HZdat)
{ 
   unsigned int lengh = 0;
   unsigned int i = 0;
   lengh = strlen(HZdat);
   for(i=0;i<lengh;i=i+2)
   {
      send_data(0xf0);
      send_data(x++);
      send_data(y);
      if(x>11)
      {
         x=0;
         y++;
         if(y>3)
         {
            y=0;
         }
                        
      }
                       
      send_data(HZdat[i]-0xa0);
      send_data(HZdat[i+1]-0xa0);
   }
}*/
/*********************************************************************************************************
** Function name:       write_8_16ASCH
** Descriptions:        写入8*16字符坐标x以8为单位，坐标y以1为单位
** input parameters:    x：      写入汉字的横坐标  （0~23）
**                      y：      写入汉字的纵坐标  （0~63）
**                      ucstr：  要写入的字符
**
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
/*void write_8_16ASCH(unsigned char x,unsigned char  y, unsigned char *ucstr)
{
    unsigned int i=0;
    unsigned int a = strlen(ucstr);
    for(i=0;i<a;i++)
    {
       send_data(0xf9);
       send_data(x++);
       send_data(y);
       if(x>23)
       {
           x=0;
           y=y+16;
       }
           send_data(ucstr[i]);
    }
 }*/
           
 /*********************************************************************************************************
 ** Function name:       write_str
 ** Descriptions:        同时显示汉字和字符，在写入字符的时候x坐标以8为单位，y坐标以1为单位
 ** input parameters:    x：      写入汉字的横坐标  （0~23）//横坐标用字符串表示方法
 **                      y：      写入汉字的纵坐标  （0~63）//63?纵坐标用点阵表示为了汉字与字符相统一用
 **                      ucstr：要写入的字符（可以是汉字）
 **
 ** output parameters:   NONE
 ** Returned value:      NONE
 *********************************************************************************************************/
 void write_str(unsigned char x,  unsigned char y,unsigned char *str)
 {
    int i=0;
    while(str[i] != '\0')
    {
       if(str[i]>=0xa0)                                                /*  说明是汉字                  */
       {
           send_data(0xf0);
           send_data(x/2);
           send_data(y/16);           /*  此处需要对XY进行计算是因为要按照写入字符的标准对xy坐标进行设置，
                                          在写入字符的时候x坐标以8为单位，y坐标以1为单位*/
           x=x+2;
          if(x>23)
          {
             x=0;
             y+=16;                          
          }
          send_data(str[i]-0xa0);
          send_data(str[i+1]-0xa0);
          i=i+2;
       }
       else                                                            /*  要显示的是字符              */

       {
          send_data(0xf9);
          send_data(x);
          send_data(y);//字符Y值以1*1点阵为单位(0,16,32,48) 因此Y不用除以16
		  x++;
          if(x>23)
          {
             x=0;
             y=y+16;
          }
          send_data(str[i]);
          i=i+1;
       }
    }
 }
 /*********************************************************************************************************
 ** Function name:       write_dot_1_1
 ** Descriptions:        写入某一个点x坐标以1为单位，y坐标以1为单位
 ** input parameters:    x：      写入汉字的横坐标  （0~192）
 **                      y：      写入汉字的纵坐标  （0~63）
 **
 ** output parameters:   NONE
 ** Returned value:      NONE
 *********************************************************************************************************/
 /*void write_dot_1_1(unsigned char x,  unsigned char  y)
 {
     send_data(0xf2);
     send_data(x);
     send_data(y);
 }*/
           
 /*********************************************************************************************************
 ** Function name:       write_dot_8_1
 ** Descriptions:        写入某一个点x坐标以8为单位，y坐标以1为单位
 ** input parameters:    x：      写入汉字的横坐标  （0~23）
 **                      y：      写入汉字的纵坐标  （0~63）
 **                      BT     慈写入字节的数据
 **
 ** output parameters:   NONE
 ** Returned value:      NONE
 *********************************************************************************************************/
/* void write_dot_8_1(unsigned char x,  unsigned char  y,unsigned char BT)
 {
    send_data(0xf3);
    send_data(x);
    send_data(y);
    send_data(BT);
 }*/
/**********************************************************************************************************/
