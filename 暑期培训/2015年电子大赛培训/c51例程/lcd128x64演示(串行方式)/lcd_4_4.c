
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
         
 /*********************************************************************************************************
 **变量定义
 *********************************************************************************************************/
 extern int KeyFlag;
 sbit  KeyL1 = P2^0;
 sbit  KeyL2 = P2^1;
 sbit  KeyL3 = P2^2;
 sbit  KeyL4 = P2^3;
           
 /********************************************************************************************************
 **外部函数声明
 ********************************************************************************************************/
 extern void delay(long dly);
 extern void write_str(unsigned char x,  unsigned char y,unsigned char *str);
 extern void BELL_ON(int Time);
          
 /*********************************************************************************************************
 ** Function name:       KeyGet
 ** Descriptions:        获得按键值
 ** input parameters:    NONE
 **
 ** output parameters:   NONE
 ** Returned value:      读取到的按键值
*********************************************************************************************************/
unsigned int  KeyGet1(void)
{
    int A_data=0x0f;
    int _4_4key_code=0xff;
    P2 = 0x0f;
    A_data = P2 | 0xf0;
    if(A_data != 0xff)
    {
        delay(1000);                                                        /*  防止按键抖动                */
        A_data = P2 | 0xf0;
        if(A_data != 0xfff)
        {
     
            KeyL1 = 0;
            KeyL2 = 1; 
            KeyL3 = 1; 
            KeyL4 = 1; 
            P2 = P2 | 0xf0;
            A_data = P2 ;
            switch(A_data)
            {
                case 0xee: _4_4key_code=0;KeyFlag = 1;break;
                case 0xde: _4_4key_code=1;KeyFlag = 1;break;
                case 0xbe: _4_4key_code=2;KeyFlag = 1;break;
                case 0x7e: _4_4key_code=3;KeyFlag = 1;break;
            }
            if(KeyFlag == 1)
            {       
                BELL_ON(2000);
                return _4_4key_code;
            }
            else
            {
                KeyL1 = 1; 
                KeyL2 = 0;
                KeyL3 = 1; 
                KeyL4 = 1;
                P2 = P2 | 0xf0;
                A_data = P2 ;
                switch(A_data)
                {
                    case 0xed : _4_4key_code=4;KeyFlag = 1;break;
                    case 0xdd : _4_4key_code=5;KeyFlag = 1;break;
                    case 0xbd : _4_4key_code=6;KeyFlag = 1;break;
                    case 0x7d : _4_4key_code=7;KeyFlag = 1;break;
                }
                if(KeyFlag == 1)
                {
                    BELL_ON(2000);
                    return _4_4key_code;
                       
                }
                else 
                {
                    KeyL1 = 1; 
                    KeyL2 = 1;
                    KeyL3 = 0; 
                    KeyL4 = 1;
                    P2 = P2 | 0xf0;
                    A_data = P2 ;
                    switch(A_data)
                    {
                        case 0xeb : _4_4key_code=8;KeyFlag = 1;break;
                        case 0xdb : _4_4key_code=9;KeyFlag = 1;break;
                        case 0xbb : _4_4key_code=10;KeyFlag = 1;break;
                        case 0x7b : _4_4key_code=11;KeyFlag = 1;break;
                    }
                    if(KeyFlag == 1)
                    {
                        BELL_ON(2000);
                        return  _4_4key_code;
                              
                    }
                    else
                    {
                        KeyL1 = 1; 
                        KeyL2 = 1;
                        KeyL3 = 1; 
                        KeyL4 = 0;
                        P2 = P2 | 0xf0;
                        A_data = P2 ;
                        switch(A_data)
                        {
                            case 0xe7 : _4_4key_code=12;KeyFlag = 1;break;
                            case 0xd7 : _4_4key_code=13;KeyFlag = 1;break;
                            case 0xb7 : _4_4key_code=14;KeyFlag = 1;break;
                            case 0x77 : _4_4key_code=15;KeyFlag = 1;break;
                        }
                        if(KeyFlag == 1)
                        {
                            BELL_ON(2000);
                            return  _4_4key_code;
                                                      
                        }
                    }
                }
             }
          }
      }
 }




 
#define KEY P2 //键盘所连接的I/O接口组定义
unsigned char key_code;
/*********************************************************************************************
函数名：16个阵列式键盘驱动程序
调  用：? = Key ();
参  数：无
返回值：unsigned char 键值0~16
结  果：有键按下时返回值为键值1~16，无键按下时返回值为0
备  注：在主函数中不断调用
/**********************************************************************************************/



unsigned char  KeyGet(void)         //read_key (void)
{ //4*4阵列键盘处理程序
	unsigned char a,b,c;
	KEY = 0x0f; //设定键盘初始电平状态
	if (KEY != 0x0f)
	{ //读取键盘状态是否改变
   		delay (2000); //延时20ms去抖动
   		if (KEY != 0x0f)
		{ //重新读取
	 		 a = KEY; //寄存状态值到a
		}
		KEY = 0xf0; //设定键盘反向电平状态
		c = KEY; //读取反向电平状态值到c
		a = a|c; //a与c相或
		switch(a)
		{ //键盘状态查表
		    case 0xbb: b = 0; KeyFlag = 1;break;
			case 0x7e: b = 1;KeyFlag = 1; break;
			case 0xbe: b = 2;KeyFlag = 1; break;
			case 0xde: b = 3;KeyFlag = 1; break;
			case 0xee: b = 4;KeyFlag = 1; break;
			case 0x7d: b = 5;KeyFlag = 1; break;
			case 0xbd: b = 6;KeyFlag = 1; break;
			case 0xdd: b = 7; KeyFlag = 1;break;
			case 0xed: b = 8; KeyFlag = 1;break;
			case 0x7b: b = 9; KeyFlag = 1;break;
			case 0xdb: b = 10; KeyFlag = 1;break;
			case 0xeb: b = 11; KeyFlag = 1;break;
			case 0x77: b = 12; KeyFlag = 1;break;
			case 0xb7: b = 13; KeyFlag = 1;break;
			case 0xd7: b = 14; KeyFlag = 1;break;
			case 0xe7: b = 15; KeyFlag = 1;break;
			
		//	default:   b = 0 ; KeyFlag = 0;break;
        }
		BELL_ON(2000);
	}
    return (b); //将b中的键值代号送入函数返回值
}


/**********************************************************************************************/



 /*********************************************************************************************************
 ** Function name:       Keytest
 ** Descriptions:        进行按键测试，进入此函数之后不能够退出，只能断电复位退出
 ** input parameters:    NONE
 **
 ** output parameters:   NONE
 ** Returned value:      NONE
 *********************************************************************************************************/
 void Keytest(void)
 {
     int key=0xff;
     write_str(0,0,"键值代码测试：   键");
     write_str(4,48,"只能断电复位退出");
     while(1)
     {
         key = KeyGet();
         if(KeyFlag == 1)
         {
             KeyFlag = 0 ;
             switch(key)
             {
                 case 0:
                    write_str(14,0,"0");
                    break;
    			 case 1:
                    write_str(14,0,"1");
                    break;
                 case 2:
                    write_str(14,0,"2");
                    break;
                 case 3:
                    write_str(14,0,"3");
                    break;
                 case 4:
                    write_str(14,0,"4");
                    break;
                 case 5:
                    write_str(14,0,"5");
                    break;
                 case 6:
                    write_str(14,0,"6");
                    break;
                 case 7:
                    write_str(14,0,"7");
                    break;
                 case 8:
                    write_str(14,0,"8");
                    break;
                 case 9:
                    write_str(14,0,"9");
                    break;

                 case 10:
                    write_str(14,0,"A");
                    break;
                 case 11:
                    write_str(14,0,"B");
                    break;
                 case 12:
                    write_str(14,0,"C");
                    break;
                 case 13:
                    write_str(14,0,"D");
                    break;
                 case 14:
                    write_str(14,0,"E");
                    break;
                 case 15:
                    write_str(14,0,"F");
                    break;
             }
          }
       }
}
/********************************************************************************************************
-*
** End  File
*********************************************************************************************************/
