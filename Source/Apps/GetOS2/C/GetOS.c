/******************************************************************************
*	
*	[Title]		GetOS2
*
*	[Model] 	PocketViewer S-Model
*
*	[Version]	Ver1.20
*
*	[Author]        J. Steingraeber
*
*       Released to the Public Domain
*
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include        "sx.h"

/* global variables for sx_getblock */
static int iSendStatus;
static long lBytesSent;

/*
 * SwitchBank
 *
 * makes RAM or FLASH memory accessible at certain bank
 *
 * addr A31-A16 of source address (must be even)
 *      use 0000 - 00FE for RAM (only 0000 will work)
 *      use 0100 - 010E for program flash (1st MB, PVOS)
 *      use 0110 - 011E for program flash (2nd MB, Add-In)
 *      use 0120 - 013E for data flash (4MB data)
 *
 * bank destination bank
 *      use 0 for address space 0000:0000 (forbidden)
 *      use 1 for address space 2000:0000
 *      use 2 for address space 4000:0000
 *      use 3 for address space 6000:0000
 *      use 4 for address space 8000:0000 (forbidden)
 *      use 5 for address space A000:0000
 *      use 6 for address space C000:0000 (forbidden)
 *
 * remarks:
 *   you can not switch bank 0, 4 and 6, because that will crash your
 *   PV; bank 0 contains the interrupt vectors, and every program
 *   expects to find RAM here; bank 4 contains your Add-In, so
 *   switching this would make your Add-In vanishing; bank 6 contains
 *   the BIOS, your PV can not operate without it
 *
 * examples:
 *
 *   SwitchBank(0x0000,3);
 *     will make 128kB RAM accessible at address 6000:0000 so RAM can
 *     be accessed at 0000:0000 and 6000:0000 in the same way (which
 *     is possible but stupid, you still have only 128kB of RAM)
 *
 *   SwitchBank(0x0104,5);
 *     will make Fonts accessible at address A000:0000 and Graphics
 *     accessible at B000:0000
 *
 *   SwitchBank(0x0120,1);
 *     will make first 128kB of data flash accessible at 2000:0000
 *
 */
void _asm_switchbank(char *,int,int);
#define SwitchBank(addr,bank) _asm_switchbank("\n MOV BH,AL\n MOV AL,AH\n MOV AH,BL\n OR AH,80h\n INT 0C8h\n MOV AL,BH\n MOV AH,BL\n INT 0C8h\n",addr,bank);

void _asm_STI();
void _asm_CLI();
#define STI() _asm_STI("\n POPF\n")
#define CLI() _asm_CLI("\n PUSHF\n CLI\n")

long getdata(unsigned char * pBuffer, long lLen);

/*
  txfree
  
  the missing library function ;-)
  calculates free space in transmit buffer by looking directly
  onto system variables
  this is valid for all known PVOS versions (250X,450X,250S,450S,750,750+)
*/
unsigned int txfree()
{
  unsigned int far * pTop;
  unsigned int far * pBottom;
  unsigned int result;

  pTop = MK_FP(0x0000,0x21A8);
  pBottom = MK_FP(0x0000,0x21AA);

  CLI();
  result = *pTop - *pBottom;
  STI();
  
  result &= 0x00FF;
  result = 256 - result;

  return result;
}

/* callback routines for XY-MODEM protocol */
unsigned char sx_getchar(int iTimeout);
void sx_sendchar(unsigned char c);
int sx_getblock(unsigned char * pBuffer, int iBlockLen);

unsigned char sx_getchar(int iTimeout)
{
  unsigned char c;
  unsigned int iNum, iFlag;
  byte hour, minute, second;
  byte lastsecond;
  
  LibGetTime2(&hour,&minute,&second);
  lastsecond = second;
  
  while (iTimeout != 0)
  {
    LibGetTime2(&hour,&minute,&second);
    if (second != lastsecond)
    {
      lastsecond = second;
      --iTimeout;
    }
    
    iNum = iFlag = 0;
    LibSrlGetDteStat(&iNum,&iFlag);
    
    if (iNum != 0)
    {
      LibSrlRecvByte(&c);
      if (c != 0) return c;
    }
  }
  
  return 0; /* timeout */
}

void sx_sendchar(unsigned char c)
{
  byte hour, minute, second;
  byte lastsecond;
  byte timeout = 3;

  LibGetTime2(&hour,&minute,&second);
  lastsecond = second;

      /* wait for at least 10 free bytes in tx buffer */
  while (timeout && (txfree() < 10))
  {
    LibGetTime2(&hour,&minute,&second);
    if (second != lastsecond)
    {
      --timeout;
      lastsecond = second;
    }
  }
  
  LibSrlSendByte(IB_FOLLOW_BUSY,c);
}

int sx_getblock(unsigned char * pBuffer, int iBlockLen)
{
  int i;
  long l;
  long lTotal = 16*17 + 16*4096*45 + 13;
  
  if (iSendStatus == SX_SENDPATH)
  {
    iSendStatus = SX_SENDDATA;
        /*
          create first block exactly 128 bytes long
          containing name and length
        */
    sprintf(pBuffer,"getos.hex\n%ld",lTotal);
    i = strlen(pBuffer);
    pBuffer[9] = 0;
    while (i < 128)
    {
      pBuffer[i] = 0;
      ++i;
    }
    return 128;
  }

  if (iSendStatus == SX_SENDDATA)
  {
    l = lTotal - lBytesSent;
    if (l == 0)
    {
      iSendStatus = SX_SENDFINISH;
      return 0;
    }
    if (l > iBlockLen)
    {
      l = iBlockLen;
    }
    
    l = getdata(pBuffer,l);
    lBytesSent += l;
    
    return l;
  }
  
    
  if (iSendStatus == SX_SENDFINISH)
  {
    iSendStatus = SX_NULL;
    for (i=0;i<128;++i)
    {
      pBuffer[i] = 0;
    }
    return 128;
  }

  return 0;
}


static long lHexPos;
static int iHexOffset;
static int iTextLen;

/*
  getdata

  fill in hex file portion into pBuffer of length iLen
  returns number of bytes written into pBuffer
*/
long getdata(unsigned char * pBuffer, long lLen)
{
  static unsigned char text[80];
  int iBank;
  unsigned char cChecksum;
  int i;
  unsigned int iResult = 0;
  unsigned int iSegment;
  unsigned int iOffset;
  unsigned char far * pData;
  unsigned char c;
  
  while (lLen)
  {
    while (lLen && (iHexOffset < iTextLen))
    {
          /* transfer bytes left in text */
      pBuffer[iResult] = text[iHexOffset];
      ++iHexOffset;
      ++iResult;
      --lLen;
    }

    if (lLen)
    {
          /* fill text with new string */
      lHexPos += 16;
      iSegment = (lHexPos & 0x000F0000L) >> 4;
      iOffset = lHexPos;
      iHexOffset = 0;
      iTextLen = 0;

      if (lHexPos == 0x00100000L)
      {
            /* 1MEG boundary reached */
            /* end record */
        sprintf(text,":00000001FF\r\n");
        iTextLen = 13;
      }
      else
      {
        if (iOffset == 0)
        {
              /* new segment record */
          sprintf(text,":02000002%04X",iSegment);
          cChecksum = 0x04 + (iSegment >> 8) + iSegment;
          cChecksum = -cChecksum;
          sprintf(&text[13],"%02X\r\n",cChecksum);
          iTextLen = 17;
        }
            /* data record */
        sprintf(&text[iTextLen],":10%04X00",iOffset);
        iTextLen += 9;
        cChecksum = 0x10 + (iOffset >> 8) + iOffset;
        
        iBank = iSegment >> 12;
        if (iBank & 1)
        {
          pData = MK_FP(0x7000,0x0000);
        }
        else
        {
          pData = MK_FP(0x6000,0x0000);
        }
        SwitchBank((iBank & 0x1E) + 0x0100,3);
        for (i=0;i<16;++i)
        {
          c = pData[iOffset+i];
          sprintf(&text[iTextLen],"%02X",c);
          cChecksum += c;
          iTextLen += 2;
        }
            /* not strictly necessary because PVOS
               switches back automatically if fonts data is requested
            */
        SwitchBank(0x0104,3); /* Fonts */
        cChecksum = -cChecksum;
        sprintf(&text[iTextLen],"%02X\r\n",cChecksum);
        iTextLen += 4;
      }
    }
  }
  
  return (long)iResult;
}

int sendfile(int iProto)
{
  int iError = 0;
  
  LibSrlTxBufClr();
  LibSrlRxBufClr();
  
  LibSrlSendBlock("\r\nStart your ",13);

  switch (iProto)
  {
      case SX_XMODEM:
        LibSrlSendBlock("XMODEM",6);
        break;
      case SX_XMODEM1K:
        LibSrlSendBlock("XMODEM-1K",9);
        break;
      case SX_YMODEM:
        LibSrlSendBlock("YMODEM",6);
        break;
      case SX_YMODEM1K:
        LibSrlSendBlock("YMODEM-1K",9);
        break;
  }
  
  LibSrlSendBlock(" receiver\r\n",11);
  
  if ((iProto == SX_XMODEM) || (iProto == SX_XMODEM1K))
  {
    iSendStatus = SX_SENDDATA;
  }
  else
  {
    iSendStatus = SX_SENDPATH;
  }
        
  lHexPos = -16;
  iHexOffset = 0;
  iTextLen = 0;
  lBytesSent = 0;

  iError = sx(iProto,SX_SENDNORMAL);

  if (iError == CAN)
  {
    LibSrlSendBlock("\r\nTransfer aborted by receiver\r\n",32);
    return iError;
  }
  else if (iError)
  {
    LibSrlSendBlock("\r\nTransfer aborted (protocol error)\r\n",37);
    return iError;
  }
  
  if ((iProto == SX_YMODEM) || (iProto == SX_YMODEM1K))
  {
        /* send last block of YMODEM protocol */
    iError = sx(iProto,SX_STOPAFTERFIRSTBLOCK);
  }

  if (iError == CAN)
  {
    LibSrlSendBlock("\r\nTransfer aborted by receiver\r\n",32);
    return iError;
  }
  else if (iError)
  {
    LibSrlSendBlock("\r\nTransfer aborted (protocol error)\r\n",37);
    return iError;
  }
  else
  {
    LibSrlSendBlock("\r\nTransfer finished\r\n",21);
  }
  
  return iError;
}

void main(void)
{
  TCHSTS tsts;
  
  dword dwAction;

  SRL_STAT serinit;
  word wTimeout;

  int iProto = SX_YMODEM1K;

  serinit.port = IB_SRL_COM2;
  serinit.speed = IB_SRL_19200BPS;
  serinit.parit = IX_SRL_NONE;
  serinit.datab = IX_SRL_8DATA;
  serinit.stopb = IX_SRL_1STOP;
  serinit.fctrl = IX_SRL_RSCS;

      /* Clear Screen */
  LibInitDisp();

      /* Output some text */
  LibStringDsp("GetOS V2.0",5,1,160,IB_PFONT2);

  LibLine(0,10,160,1,1);
  LibStringDsp("Use baudrate 19200",5,20,160,IB_PFONT1);
  LibStringDsp("(will last 26 minutes)",5,28,160,IB_PFONT1);
  LibLine(0,46,160,1,1);
  LibStringDsp("Use baudrate 38400",5,56,160,IB_PFONT1);
  LibStringDsp("(will last 13 minutes)",5,64,160,IB_PFONT1);
  LibLine(0,82,160,1,1);
  LibStringDsp("Protocol: YMODEM-1K",5,92,160,IB_PFONT1);
  LibLine(0,110,160,1,1);
  LibStringDsp("Start transfer",5,120,160,IB_PFONT1);
  LibLine(0,138,160,1,1);

  LibTchStackClr();
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  
  LibTchInit();

  tsts.obj = 0;
    
  while (tsts.obj == 0)
  {
        /* display text */
    LibPutDisp();
        /* wait for touchpad */
    LibTchWait( &tsts );
        /* set default text */
    LibClrBox(5,120,154,8);
    LibStringDsp("Start transfer",5,120,160,IB_PFONT1);

    dwAction = tsts.act;
    
    if ((tsts.obj == 0x9FFF) && (tsts.act == ACT_BREAK)
        && (tsts.y > 10) && (tsts.y < 138))
    {

          /* adjust baudrate */
      if (tsts.y < 46)
      {
        if (serinit.speed != IB_SRL_19200BPS)
        {
          serinit.speed = IB_SRL_19200BPS;
        }
      }
      else if (tsts.y < 82)
      {
        if (serinit.speed != IB_SRL_38400BPS)
        {
          serinit.speed = IB_SRL_38400BPS;
        }
      }
      else if (tsts.y < 110)
      {
            /* change protocol */
        if (iProto == SX_YMODEM1K)
        {
          iProto = SX_XMODEM;
        }
        else
        {
          ++iProto;
        }
        
        switch (iProto)
        {
            case SX_XMODEM:
              LibStringDsp("Protocol: XMODEM   ",5,92,160,IB_PFONT1);
              break;
            case SX_XMODEM1K:
              LibStringDsp("Protocol: XMODEM-1K",5,92,160,IB_PFONT1);
              break;
            case SX_YMODEM:
              LibStringDsp("Protocol: YMODEM   ",5,92,160,IB_PFONT1);
              break;
            case SX_YMODEM1K:
              LibStringDsp("Protocol: YMODEM-1K",5,92,160,IB_PFONT1);
              break;
        }
      }
      
      if (tsts.y < 82)
      {
            /* open port with actual settings */
        LibSrlTxBufClr();
        LibSrlRxBufClr();
        LibSrlPortOpen(&serinit);
        
            /* send teststring */
        LibSrlSendBlock("\r\nGetOS",7);

            /* close port */
        wTimeout = 8000;
        while (wTimeout && (LibSrlPortClose() != IW_SRL_NOERR))
        {
          --wTimeout;
        }
            /* forced close */
        LibSrlPortFClose();
        
            /* don't leave while loop */
        tsts.obj = 0;
      }

      if ((tsts.y > 110) && (tsts.y < 138))
      {
            /* do the transfer */
        LibClrBox(5,120,154,8);
        LibStringDsp("Transfer started...",5,120,160,IB_PFONT1);
        LibPutDisp();

            /* open port with actual settings */
        LibSrlTxBufClr();
        LibSrlRxBufClr();
        LibSrlPortOpen(&serinit);
        
        LibClrBox(5,120,154,8);
        switch (sendfile(iProto))
        {
            case 0:
              LibStringDsp("Transfer finished",5,120,160,IB_PFONT1);
              break;
            case CAN:
              LibStringDsp("Transfer aborted by receiver",5,120,160,IB_PFONT1);
              break;
            default:
              LibStringDsp("Transfer aborted (protocol error)",5,120,160,IB_PFONT1);
        }
        
            /* close port */
        wTimeout = 8000;
        while (wTimeout && (LibSrlPortClose() != IW_SRL_NOERR))
        {
          --wTimeout;
        }
            /* forced close */
        LibSrlPortFClose();
        
      }
      
      tsts.obj = 0;
    }

    else
    {
      tsts.obj = 0;
    }
    
    
  }

      /* close port */
  wTimeout = 8000;
  while (wTimeout && (LibSrlPortClose() != IW_SRL_NOERR))
  {
    --wTimeout;
  }
      /* forced close */
  LibSrlPortFClose();

  LibJumpMenu();
  
}




