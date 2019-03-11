/*-----------------------------------------------.c??-----------------------------------------*/ 
#include"rc522.h" 
#include<reg52.h>  \
#include<intrins.h>
/*void delay_ns(uint16 ns) {    uint16 xdata i;   for(i=0;i<ns;i++)   {      nop();     nop();     nop();   } }*/ 
void delay10us(void)   //?? 0us
{   
		unsigned char a,b;     
		for(b=1;b>0;b--)          
		for(a=2;a>0;a--);
}  
void delay1ms(void)   //?? 0us 
{    
  unsigned char a,b,c;   
  for(c=1;c>0;c--)          
  for(b=142;b>0;b--)   
	for(a=2;a>0;a--); 
} 
void delay1ms500us(void)   //?? 0us
{     
	uint xdata a,b;
	for(b=115;b>0;b--)
	for(a=5;a>0;a--);
	
} 
uint SPIReadByte(void) 
{ 
	uint xdata SPICount;                                       // Counter used to clock out the data 
	uint xdata SPIData;  
	SPIData = 0;
	for (SPICount = 0;SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read   
	{
    SPIData <<=1;                                               // Rotate the data    
		CLR_SPI_CK;
		nop();
		nop();    
		// Raise the clock to clock the data out of the MAX7456
    if(SET_SPI_MISO)
    { 
      SPIData|=0x01;
    }
		SET_SPI_CK; 
		_nop_();
		_nop_();                                               // Drop the clock ready for the next bit
   }                                                             // and loop back   
	 return (SPIData);                              // Finally return the read data  
}  
void SPIWriteByte(uint SPIData) 
{  
  uint xdata SPICount;                                       // Counter used to clock out the data 
	for (SPICount = 0; SPICount < 8; SPICount++) 
	{  
    if (SPIData & 0x80)
     {        SET_SPI_MOSI;     } 
    else     {        CLR_SPI_MOSI;     } 
		_nop_();
		_nop_();
    CLR_SPI_CK;
		nop();
		nop(); 
    SET_SPI_CK;nop();
		nop(); 
    SPIData <<= 1; 
  }             
 }      
 /////////////////////////////////////////////////////////////////////
 //?    ?:??  
 //????: req_code[IN]:????  
 //                0x52 = ?????????14443A???? 
 //                0x26 = ??????????
 //          pTagType[OUT]:??????
 //                0x4400 = Mifare_UltraLight
 //                0x0400 = Mifare_One(S50) 
 //                0x0200 = Mifare_One(S70) 
 //                0x0800 = Mifare_Pro(X) 
		//                0x4403 = Mifare_DESFire 
 //?    ?: ????MI_OK 
 ///////////////////////////////////////////////////////////////////// 
 char PcdRequest(uint8 req_code,uint8 *pTagType)
 {  
	 char xdata status;   
	 uint xdata unLen;
	 uint xdata ucComMF522Buf[MAXRLEN];
	 ClearBitMask(Status2Reg,0x08);
	 WriteRawRC(BitFramingReg,0x07);
	 SetBitMask(TxControlReg,0x03); 
   ucComMF522Buf[0] = req_code; 
	 status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen); 
	 if ((status == MI_OK) && (unLen == 0x10))
  {       *pTagType     = ucComMF522Buf[0];   *(pTagType+1) = ucComMF522Buf[1];  } 
   else  {   status = MI_ERR;   } 
     return status; 
}  
///////////////////////////////////////////////////////////////////// 
//?    ?:???  
//????: pSnr[OUT]:?????,4?? 
//?    ?: ????MI_OK  
/////////////////////////////////////////////////////////////////////   
char PcdAnticoll(uint *pSnr)
{    
  char xdata status; 
	uint xdata i,snr_check=0;  
	uint xdata unLen;  
	uint xdata ucComMF522Buf[MAXRLEN];
	ClearBitMask(Status2Reg,0x08); 
	WriteRawRC(BitFramingReg,0x00);  
	ClearBitMask(CollReg,0x80);	
   ucComMF522Buf[0] = PICC_ANTICOLL1;  
	ucComMF522Buf[1] = 0x20;  
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen); 
	if (status == MI_OK)
  {      
		for (i=0; i<4; i++) 
		{       
           *(pSnr+i)  = ucComMF522Buf[i];  
            snr_check ^= ucComMF522Buf[i];
      }  
         if (snr_check != ucComMF522Buf[i])
          {   status = MI_ERR;    } 
    }  
         SetBitMask(CollReg,0x80); 
    return status; 
}   
///////////////////////////////////////////////////////////////////// 
//?    ?:????  //????: pSnr[IN]:?????,4?? 
//?    ?: ????MI_OK  
///////////////////////////////////////////////////////////////////// 
char PcdSelect(uint8 *pSnr) 
{    
  char xdata status;   
  uint xdata i;   
	uint xdata unLen;  
	uint xdata ucComMF522Buf[MAXRLEN];      
	ucComMF522Buf[0] = PICC_ANTICOLL1;   
  ucComMF522Buf[1] = 0x70;    
	ucComMF522Buf[6] = 0;    
	for (i=0; i<4; i++)  
		{   
   ucComMF522Buf[i+2] = *(pSnr+i); 
     ucComMF522Buf[6]  ^= *(pSnr+i); 
    } 
		CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]); 
		ClearBitMask(Status2Reg,0x08);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen); 
          if ((status == MI_OK) && (unLen == 0x18))     {   status = MI_OK;  } 
    else      {   status = MI_ERR;    }    
   return status;
 }  
 ///////////////////////////////////////////////////////////////////// 
 //?    ?:??????  //????: auth_mode[IN]: ?????? //                 0x60 = ??A?? //                 0x61 = ??B??  
 //          addr[IN]:??? //          pKey[IN]:??  //          pSnr[IN]:?????,4?? //?    ?: ????MI_OK  
 /////////////////////////////////////////////////////////////////////  
 char PcdAuthState(uint8 auth_mode,uint8 addr,uint8 *pKey,uint8 *pSnr) 
 {    
	 char xdata status; 
   uint xdata unLen;
	 uint xdata i,ucComMF522Buf[MAXRLEN];  
	 ucComMF522Buf[0] = auth_mode;   
  ucComMF522Buf[1] = addr;
     for (i=0; i<6; i++)  
	 {    ucComMF522Buf[i+2] = *(pKey+i);   }   
  for (i=0; i<6; i++)    
  {    ucComMF522Buf[i+8] = *(pSnr+i);  } 
	//   memcpy(&ucComMF522Buf[2], pKey, 6);  
	//   memcpy(&ucComMF522Buf[8], pSnr, 4);       
	status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);  
   if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08))) 
    {   status = MI_ERR;   }        
   return status; 
} 
/////////////////////////////////////////////////////////////////////
						 
						 //?    ?:??M1????? //????: addr[IN]:???  //          pData[OUT]:?????,16?? //?    ?: ????MI_OK 
/////////////////////////////////////////////////////////////////////  
char PcdRead(uint8 addr,uint8 *pData) 
{   
	char xdata status;  
	uint16 xdata unLen; 
	uint8 xdata i,ucComMF522Buf[MAXRLEN];  
	ucComMF522Buf[0] = PICC_READ;
	ucComMF522Buf[1] = addr; 
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen); 
	if ((status == MI_OK) && (unLen == 0x90))   //   {   memcpy(pData, ucComMF522Buf, 16);   } 
    {         
        for (i=0; i<16; i++) 
         {   
						*(pData+i) = ucComMF522Buf[i];
         } 
    }    
  else      {   status = MI_ERR;   } 
          return status; 
}  
///////////////////////////////////////////////////////////////////// //?    ?:????M1??? 
//????: addr[IN]:???  //          pData[IN]:?????,16?? //?    ?: ????MI_OK 
/////////////////////////////////////////////////////////////////////            
char PcdWrite(uint addr,uint *pData)
{      
	char xdata status;
	uint xdata unLen; 
	uint xdata i,ucComMF522Buf[MAXRLEN];  
	ucComMF522Buf[0] = PICC_WRITE;  
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
  if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A)) 
    {   status = MI_ERR;   }              
	if (status == MI_OK)   
		{          //memcpy(ucComMF522Buf, pData, 16);     
			for (i=0; i<16; i++)     
			{              ucComMF522Buf[i] = *(pData+i);            }      
			CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);  
			status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);  
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))   
      {   status = MI_ERR;   }  
   }           
		return status;
 }  
 ///////////////////////////////////////////////////////////////////// 
 //?    ?:?????????? //?    ?: ????MI_OK  
 ///////////////////////////////////////////////////////////////////// 
char PcdHalt(void) 
 {      
	 char xdata status;  
   uint xdata unLen;  
	 uint xdata ucComMF522Buf[MAXRLEN]; 
	 ucComMF522Buf[0] = PICC_HALT;   
  ucComMF522Buf[1] = 0;     
 CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);   
     status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen); 
      return MI_OK; 
}  
///////////////////////////////////////////////////////////////////// 
//?MF522??CRC16??
				 ///////////////////////////////////////////////////////////////////// 
void CalulateCRC(uint *pIndata,uint len,uint *pOutData)
{    
  uint xdata i,n;    
  ClearBitMask(DivIrqReg,0x04);  
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80); 
	for (i=0; i<len; i++)  
	{   WriteRawRC(FIFODataReg, *(pIndata+i));   } 
	WriteRawRC(CommandReg, PCD_CALCCRC); 
	i = 0xFF;  
	do      
		{          n = ReadRawRC(DivIrqReg);         i--;     }    
	while ((i!=0) && !(n&0x04));     
	pOutData[0] = ReadRawRC(CRCResultRegL);   
	pOutData[1] = ReadRawRC(CRCResultRegM); 
}   
/////////////////////////////////////////////////////////////////////  
//?    ?:??RC522 //?    ?: ????MI_OK  
///////////////////////////////////////////////////////////////////// 
char PcdReset(void) 
{  //PORTD|=(1<<RC522RST); 
	SET_RC522RST;   
  delay10us(); 
	//PORTD&=~(1<<RC522RST);  
	CLR_RC522RST;     
	delay10us();  
	//PORTD|=(1<<RC522RST); 
	SET_RC522RST;    
	delay10us();     
	WriteRawRC(CommandReg,PCD_RESETPHASE); 
	delay10us();         
  WriteRawRC(ModeReg,0x3D);   
	//?Mifare???,CRC???0x6363  
	WriteRawRC(TReloadRegL,30);  
	WriteRawRC(TReloadRegH,0);    
	WriteRawRC(TModeReg,0x8D);  
	WriteRawRC(TPrescalerReg,0x3E); 			 
	WriteRawRC(TxAutoReg,0x40);//???      
	return MI_OK; 
} 
//////////////////////////////////////////////////////////////////////
//??RC632?????  
////////////////////////////////////////////////////////////////////// 
char M500PcdConfigISOType(uint type) 
{     if (type == 'A')       	//ISO14443_A   
	{        
		ClearBitMask(Status2Reg,0x08);    
    WriteRawRC(ModeReg,0x3D);//3F 
		WriteRawRC(RxSelReg,0x86);//84     
		WriteRawRC(RFCfgReg,0x7F);   //4F   
		WriteRawRC(TReloadRegL,30);
		//tmoLength);
		// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);    
    WriteRawRC(TModeReg,0x8D);    
		WriteRawRC(TPrescalerReg,0x3E);  
   delay1ms();     
    PcdAntennaOn(); 
   }    
 else{ return -1; }     
    return MI_OK;
 } 
 ///////////////////////////////////////////////////////////////////// 
 //?    ?:?RC632???  //????:Address[IN]:????? //?    ?:????  
 ///////////////////////////////////////////////////////////////////// 
 uint ReadRawRC(uint Address) 
{     
		uint xdata ucAddr;  
		uint xdata ucResult=0; 
		CLR_SPI_CS;
		ucAddr = ((Address<<1)&0x7E)|0x80;  
		SPIWriteByte(ucAddr);
		ucResult=SPIReadByte(); 
		SET_SPI_CS; 
		return ucResult; 
}
///////////////////////////////////////////////////////////////////
//?    ?:?RC632???  //????:Address[IN]:????? //          value[IN]:???? 
///////////////////////////////////////////////////////////////////// 
void WriteRawRC(uint Address, uint value)
{      
	uint xdata ucAddr; 
	CLR_SPI_CS;  
	ucAddr = ((Address<<1)&0x7E); 
	SPIWriteByte(ucAddr);
	SPIWriteByte(value); 
	SET_SPI_CS;
}  

 ///////////////////////////////////////////////////////////////////// //????    //?????????????????1ms???
						 ///////////////////////////////////////////////////////////////////// 
 void PcdAntennaOn(void)
 {      uint xdata i;  
    i = ReadRawRC(TxControlReg);   
  if (!(i & 0x03))    
   {         
        SetBitMask(TxControlReg, 0x03);   
  }
 }  
 /////////////////////////////////////////////////////////////////////
 //???? 
 ///////////////////////////////////////////////////////////////////// 
 void PcdAntennaOff(void)
 {  ClearBitMask(TxControlReg, 0x03); }
 ///////////////////////////////////////////////////////////////////// 
 //rc522??? 
 ///////////////////////////////////////////////////////////////////// 
 void InitRc522(void)
 {    PcdReset();   
    PcdAntennaOff();
    delay1ms500us();
    PcdAntennaOn();  
     M500PcdConfigISOType( 'A' ); 
}

///////////////////////////////////////////////////////////////////// 
//?    ?:?RC522???? //????:reg[IN]:????? //          mask[IN]:???  
/////////////////////////////////////////////////////////////////////
void SetBitMask(uint8 reg,uint8 mask) 
{     
	char xdata tmp = 0x0;  
	tmp = ReadRawRC(reg);
	WriteRawRC(reg,tmp | mask);  // set bit mask 
	}  
///////////////////////////////////////////////////////////////////// 
//?    ?:?RC522???? //????:reg[IN]:????? //          mask[IN]:???  
/////////////////////////////////////////////////////////////////////
void ClearBitMask(uint reg,uint mask)   
{     
	char xdata tmp = 0x0;
	tmp = ReadRawRC(reg); 
	WriteRawRC(reg, tmp & ~mask);  // clear bit mask 
	}
/////////////////////////////////////////////////////////////////////
//?    ?:??RC522?ISO14443??? //????:Command[IN]:RC522??? 
						 
						 //          pInData[IN]:??RC522???????? 
//          InLenByte[IN]:????????? 
//          pOutData[OUT]:??????????
//          *pOutLenBit[OUT]:????????  
///////////////////////////////////////////////////////////////////// 
char PcdComMF522(uint Command,uint *pInData,uint InLenByte,uint *pOutData,uint *pOutLenBit)
{     
	char xdata status = MI_ERR;   
  uint xdata irqEn   = 0x00;   
  uint xdata waitFor = 0x00;    
	uint xdata lastBits;   
  uint xdata n;  
	uint xdata i;   
	switch (Command)   
	{          case PCD_AUTHENT:    irqEn   = 0x12;    waitFor = 0x10;    break;  
             case PCD_TRANSCEIVE:    irqEn   = 0x77;    waitFor = 0x30;    break;  
              default:    break;  
   }    
	 WriteRawRC(ComIEnReg,irqEn|0x80);    
	 ClearBitMask(ComIrqReg,0x80);   
   WriteRawRC(CommandReg,PCD_IDLE);    
   SetBitMask(FIFOLevelReg,0x80);  
	 for (i=0; i<InLenByte; i++)
      {   WriteRawRC(FIFODataReg, pInData[i]);    } 
    WriteRawRC(CommandReg, Command);   
      if (Command == PCD_TRANSCEIVE) 
    {    SetBitMask(BitFramingReg,0x80);  }           //i = 600;//????????,??M1???????25ms 
		i = 2000;   
		do 
			 {    
           n = ReadRawRC(ComIrqReg);   
   				 i--;    
				 }  
		while ((i!=0) && !(n&0x01) && !(n&waitFor));  
				 ClearBitMask(BitFramingReg,0x80);   
    if (i!=0)   
			{          
				if(!(ReadRawRC(ErrorReg)&0x1B))     
					{              status = MI_OK;            
         						if (n & irqEn & 0x01)      
											{   status = MI_NOTAGERR;   }   
											if (Command == PCD_TRANSCEIVE)    
                      {   n = ReadRawRC(FIFOLevelReg);     
												lastBits = ReadRawRC(ControlReg) & 0x07;           
												if (lastBits)                
        													{   *pOutLenBit = (n-1)*8 + lastBits;   }  
                        else      {   *pOutLenBit = n*8;   }      
                         if (n == 0)                  {   n = 1;    }          
														if (n > MAXRLEN)                  {   n = MAXRLEN;   }         
													for (i=0; i<n; i++)             
                           {   pOutData[i] = ReadRawRC(FIFODataReg);    }    
												 }}      
             else          {   status = MI_ERR;   }            
  }         
  SetBitMask(ControlReg,0x80);        	// stop timer now     
	WriteRawRC(CommandReg,PCD_IDLE);    
  return status;
 }   