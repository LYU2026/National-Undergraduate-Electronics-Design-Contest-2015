
 /****************************************Copyright (c)****************************************************
 **                               Guangzhou ZHIYUAN electronics Co.,LTD.
 **                                     
 **                                 http://www.embedtools.com
 **
 **--------------File Info---------------------------------------------------------------------------------
 ** File Name:                LCD_4_4.c
 ** Last modified Date:       2010-03-04
 ** Last Version:             
 ** Description:              ���ܣ�4*4���̵Ķ�ȡ
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
 **������ͷ�ļ�
 *********************************************************************************************************/
 #include <reg52.h>
         
 /*********************************************************************************************************
 **��������
 *********************************************************************************************************/
 extern int KeyFlag;
 sbit  KeyL1 = P2^0;
 sbit  KeyL2 = P2^1;
 sbit  KeyL3 = P2^2;
 sbit  KeyL4 = P2^3;
           
 /********************************************************************************************************
 **�ⲿ��������
 ********************************************************************************************************/
 extern void delay(long dly);
 extern void write_str(unsigned char x,  unsigned char y,unsigned char *str);
 extern void BELL_ON(int Time);
          
 /*********************************************************************************************************
 ** Function name:       KeyGet
 ** Descriptions:        ��ð���ֵ
 ** input parameters:    NONE
 **
 ** output parameters:   NONE
 ** Returned value:      ��ȡ���İ���ֵ
*********************************************************************************************************/
unsigned int  KeyGet1(void)
{
    int A_data=0x0f;
    int _4_4key_code=0xff;
    P2 = 0x0f;
    A_data = P2 | 0xf0;
    if(A_data != 0xff)
    {
        delay(1000);                                                        /*  ��ֹ��������                */
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




 
#define KEY P2 //���������ӵ�I/O�ӿ��鶨��
unsigned char key_code;
/*********************************************************************************************
��������16������ʽ������������
��  �ã�? = Key ();
��  ������
����ֵ��unsigned char ��ֵ0~16
��  �����м�����ʱ����ֵΪ��ֵ1~16���޼�����ʱ����ֵΪ0
��  ע�����������в��ϵ���
/**********************************************************************************************/



unsigned char  KeyGet(void)         //read_key (void)
{ //4*4���м��̴������
	unsigned char a,b,c;
	KEY = 0x0f; //�趨���̳�ʼ��ƽ״̬
	if (KEY != 0x0f)
	{ //��ȡ����״̬�Ƿ�ı�
   		delay (2000); //��ʱ20msȥ����
   		if (KEY != 0x0f)
		{ //���¶�ȡ
	 		 a = KEY; //�Ĵ�״ֵ̬��a
		}
		KEY = 0xf0; //�趨���̷����ƽ״̬
		c = KEY; //��ȡ�����ƽ״ֵ̬��c
		a = a|c; //a��c���
		switch(a)
		{ //����״̬���
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
    return (b); //��b�еļ�ֵ�������뺯������ֵ
}


/**********************************************************************************************/



 /*********************************************************************************************************
 ** Function name:       Keytest
 ** Descriptions:        ���а������ԣ�����˺���֮���ܹ��˳���ֻ�ܶϵ縴λ�˳�
 ** input parameters:    NONE
 **
 ** output parameters:   NONE
 ** Returned value:      NONE
 *********************************************************************************************************/
 void Keytest(void)
 {
     int key=0xff;
     write_str(0,0,"��ֵ������ԣ�   ��");
     write_str(4,48,"ֻ�ܶϵ縴λ�˳�");
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
