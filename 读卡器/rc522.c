#include<reg52.h>           
#include<intrins.h>
sbit  spi_cs=P1^6;
sbit  spi_ck=P1^5;
sbit  spi_mosi=P1^4;
sbit  spi_miso=P1^3;
sbit  spi_rst=P1^2;
 
#define SET_SPI_CS  spi_cs=1
#define CLR_SPI_CS  spi_cs=0
#define SET_SPI_CK  spi_ck=1
#define CLR_SPI_CK  spi_ck=0
#define SET_SPI_MOSI  spi_mosi=1
#define CLR_SPI_MOSI  spi_mosi=0
#define STU_SPI_MISO  spi_miso
#define SET_RC522RST  spi_rst=1
#define CLR_RC522RST  spi_rst=0
#define  nop()  _nop_()
void delay_ns(unsigned int data ns)
{
  unsigned int xdata i;
  for(i=0;i>0;i++)
  {
    nop();
    nop();
    nop();
  }
}
 
unsigned char SPIReadByte(void)
{
  unsigned char idata SPICount;                   // Counter used to clock out the data
  
  unsigned char idata SPIData;                  
  SPIData = 0;
  for (SPICount = 0; SPICount < 8; SPICount++)  // Prepare to clock in the data to be read
  {
    SPIData <<=1;                               // Rotate the data
    CLR_SPI_CK; nop();nop();                    // Raise the clock to clock the data out of the MAX7456
   if(STU_SPI_MISO)
   {
     SPIData|=0x01;
   }
    SET_SPI_CK;   nop();nop();                  // Drop the clock ready for the next bit
  }                                                             // and loop back
  return (SPIData);                             // Finally return the read data
} 
 
void SPIWriteByte(unsigned char data SPIData)
{
  unsigned char idata SPICount;                 // Counter used to clock out the data
  for (SPICount = 0; SPICount < 8; SPICount++)
  {
    if (SPIData & 0x80)
    {
      SET_SPI_MOSI;
    }
    else
    {
      CLR_SPI_MOSI;
    } nop();nop();
    CLR_SPI_CK;nop();nop();
    SET_SPI_CK;nop();nop();
    SPIData <<= 1;
  }          
}                    
 char PcdRequest(unsigned char data req_code,unsigned char *pTagType)
{ 	char idata status;  
		unsigned int idata unLen;
		unsigned char xdata ucComMF522Buf[MAXRLEN]; 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(BitFramingReg,0x07);
		SetBitMask(TxControlReg,0x03);
		ucComMF522Buf[0] = req_code;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
		if ((status == MI_OK) && (unLen == 0x10))
		{    
			*pTagType     = ucComMF522Buf[0];
			*(pTagType+1) = ucComMF522Buf[1];
		}
		else
		{  
				status = MI_ERR;   }  
				return status;
}
char PcdSelect(unsigned char *pSnr)
		{
				char xdata status;
				unsigned char xdata i;
				unsigned int xdata unLen;
				unsigned char xdata ucComMF522Buf[MAXRLEN];  
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
				if ((status == MI_OK) && (unLen == 0x18))
						{   status = MI_OK;  }
				else
						{   status = MI_ERR;    }
				return status;
	}

char PcdWrite(unsigned char  data addr,unsigned char *pData)
{
    char idata status;
    unsigned int idata unLen;
    unsigned char idata  ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
 
    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pData, 16);
     //   for (i=0; i<16; i++)
      //  {    
     //    ucComMF522Buf[i] = *(pData+i);   
     //   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   } }
    return status;
}

char PcdHalt(void)
{
    char idata status;
    unsigned int idata unLen;
    unsigned char idata ucComMF522Buf[MAXRLEN]; 
 
    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
 
    return MI_OK;
}

void CalulateCRC(unsigned char *pIndata,unsigned char data len,unsigned char *pOutData)
{
    unsigned char idata i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}
char PcdReset(void)
{
//PORTD|=(1<<RC522RST);
		SET_RC522RST;
    delay_ns(10);
//PORTD&=~(1<<RC522RST);
		CLR_RC522RST;
    delay_ns(10);
//PORTD|=(1<<RC522RST);
		SET_RC522RST;
    delay_ns(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_ns(10);
    
    WriteRawRC(ModeReg,0x3D);         //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
 
		WriteRawRC(TxAutoReg,0x40);//必须要
   
    return MI_OK;
}

unsigned char ReadRawRC(unsigned char data Address)
{
    unsigned char xdata ucAddr;
    unsigned char xdata ucResult=0;
		CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E)|0x80;
 
		SPIWriteByte(ucAddr);
		ucResult=SPIReadByte();
		SET_SPI_CS;
   return ucResult;
}

void WriteRawRC(unsigned char data Address, unsigned char data value)
{  
    unsigned char xdata ucAddr;
 
		CLR_SPI_CS;
    ucAddr = ((Address<<1)&0x7E);
 
		SPIWriteByte(ucAddr);
		SPIWriteByte(value);
		SET_SPI_CS;
}

char PcdComMF522(unsigned char data Command, 
                 unsigned char *pInData, 
                 unsigned char data InLenByte,
                 unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char data status = MI_ERR;
    unsigned char data irqEn   = 0x00;
    unsigned char data waitFor = 0x00;
    unsigned char data lastBits;
    unsigned char data n;
    unsigned int data i;
    switch (Command)
    {
        case PCD_AUTHENT:
				irqEn   = 0x12;
				waitFor = 0x10;
				break;
				case PCD_TRANSCEIVE:
				irqEn   = 0x77;
				waitFor = 0x30;
				break;
				default:
				break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    //i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
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
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);
               lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
   
    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}

char PcdValue(unsigned char data dd_mode,unsigned char data addr,unsigned char  *pValue)
{
    char data status;
    unsigned int data  unLen;
    unsigned char idata  ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
 
    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        memcpy(ucComMF522Buf, pValue, 4);
       //  for (i=0; i<16; i++)
        // {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
 
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}
 