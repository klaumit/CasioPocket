/*
  sx.c

  send some bytes with X-MODEM protocol

  this is a very(!) simple implementation, sending part only

  Author: J. Steingraeber

  Released to the Public Domain
*/

#include "crcxm.h" /* thank you, Paul Edwards, this helps a lot */
#include "sx.h"

/* these are the routines your main program must provide */
/* they are sort of callback routines */

/* get one character from serial line (or whatever from)
   but wait max iTimeout seconds
   return 0 if timeout was reached */
extern unsigned char sx_getchar(int iTimeout);

/* send one character to serial line, main program is responsible for
   handling of serial line errors */
extern void sx_sendchar(unsigned char c);

/* put next 128 byte into data buffer and return number of bytes in buffer
   return 0 if no further data is available */
extern int sx_getblock(unsigned char * pBuffer, int iBlockLen);


/*
  implementation
*/

/*
  main routine to send data via XMODEM or YMODEM
  returns 0 if transmission succeeded
  retuns CAN if transmission was canceled by receiver
  returns NAK if transmission failed for some other reason
*/


int sx(int iProto, int iStop)
{
  unsigned char cLastChar;
  unsigned char data[1024];
  int iBlockLen;
  int iTry;
  unsigned char cBlockNum;
  unsigned char cCheckSum;
  int i;
  CRCXM iCRC;
  int iUseCRC = 0;
  int iAllowedBlockLen;

  iAllowedBlockLen = 128;
  if ((iProto == SX_XMODEM1K) || (iProto == SX_YMODEM1K))
  {
    iAllowedBlockLen = 1024;
  }
  
  cLastChar = 0;

  while ((cLastChar != NAK) && (cLastChar != 'C'))
  {
    cLastChar = sx_getchar(30);
  
    if (cLastChar == CAN) return CAN;
    if (cLastChar == 0) return NAK;
  }

  if (cLastChar == 'C')
  {
    iUseCRC = 1;
  }

  cBlockNum = 0;
    
  if ((iProto == SX_XMODEM) || (iProto == SX_XMODEM1K))
  {
    cBlockNum = 1;
  }
  
  iBlockLen = 1;
  
  while (iBlockLen != 0)
  {
    iTry = SX_MAXTRY_BLOCK;

    iBlockLen = sx_getblock(data,iAllowedBlockLen);
    
    if (iBlockLen == 0)
    {
      break;
    }

    i = 128;
    if (iBlockLen > 128)
    {
      i = 1024;
    }
    
    while (iBlockLen < i)
    {
          /* fill block with ^Z */
      data[iBlockLen] = 26;
      ++iBlockLen;
    }

    while ((iBlockLen != 0) && (iTry != 0))
    {
      if (iBlockLen == 128)
      {
        sx_sendchar(SOH);
      }
      else
      {
        sx_sendchar(STX);
      }
      
      sx_sendchar(cBlockNum);
      sx_sendchar(-cBlockNum-1);
      
      cCheckSum = 0;
      crcxmInit(&iCRC);
      
      for (i=0;i<iBlockLen;++i)
      {
        sx_sendchar(data[i]);
        cCheckSum += data[i];
        crcxmUpdate(&iCRC,data[i]);
      }
      
      if (iUseCRC)
      {
        sx_sendchar(crcxmHighbyte(&iCRC));
        sx_sendchar(crcxmLowbyte(&iCRC));
      }
      else
      {
        sx_sendchar(cCheckSum);
      }
      
      cLastChar = sx_getchar(SX_TIMEOUT);
      
      if (cLastChar == 0) return NAK;

      if (cLastChar == CAN)
      {
        cLastChar = sx_getchar(SX_TIMEOUT);
        if (cLastChar == 0) return NAK;
      }

      if (cLastChar == CAN) return CAN;

      if (cLastChar == ACK)
      {
        if (iStop == SX_STOPAFTERFIRSTBLOCK)
        {
          return 0;
        }
        ++cBlockNum;
        break;
      }
      
      --iTry;
    }
  }

  sx_sendchar(EOT);
  cLastChar = sx_getchar(SX_TIMEOUT);
  if (cLastChar != ACK)
  {
    sx_sendchar(EOT);
    cLastChar = sx_getchar(SX_TIMEOUT);
  }
  
  if (cLastChar != ACK) return NAK;

  return 0;
}
