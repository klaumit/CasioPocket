#include "xymodem.h"


bool CRC_CHECKSUM_MODE;


/* manages the CRC-Handshake
   and receives the first(0) block with the file-information
   and returns the information in fi.
   It also sends the ACK and C according to the first block.
   It sets the variable CRC_CHECKSUM_MODE accordingly.
  Parameters :
  FILEINFO *fi      returns information about the file 

  Return values:
  byte              if any error occours it will be returned here
                    0 = no error
                    1 = sending not possible
                    2 = no response
               
*/
byte InitReceive(FILEINFO *fi)
{
  byte state=1;
  byte reps=0;
  byte secondsget=0;
  byte secondsact=0;
  byte std=0;
  byte min=0;
  byte diff=0;
  byte in=0;
  bool type=0;
  bool first=TRUE;
  
  while(state!=0) {
    switch (state) {
      case 1 : /* Send "C" */
        if (LibSrlSendByte(C, IB_FOLLOW_BUSY) == IW_SRL_TRSERR)
          return(1);
        state = 2;
        first = TRUE;
      break;
      case 2 : /* Wait 3 seconds for response (SOH or STX) */
        if (first) {
          LibGetTime2(&std, &min, &secondsget);
          reps++;
          first=FALSE;
        }

        LibGetTime2(&std, &min, &secondsact);
        diff = secondsact-secondsget;
        if (diff > 100) diff += 60;
        if (diff >= 4) { /* this is not exact, but has a minimum 
                            of 3 seconds */
          if (reps == 4) {
            state = 3;
            reps = 0;
            first = TRUE;
            break;
          }
          state = 1;
        }
        
        /* receive STX or SOH */
        if (LibSrlRecvByte(&in) != IW_SRL_NODATA) {
          if ((in == SOH) | (in == STX)) {
            CRC_CHECKSUM_MODE = CRCMODE;
            state = 4;
            first = TRUE;
            reps = 0;
          }
        }
      break;
      case 3 : /* Send NAK (8-Bit-Checksummode) */
      break;
      case 4 : /* receive first(0) block */
        switch (in) {
          case SOH : type = B128; break;
          case STX : type = KB1; break;
        }
        ReceiveBlock(type, buffer);
      break;
      case 5 : /* Wait 10 Seconds for response */
      break;
    } /* switch */
  } /* while */
  return(0);
}

byte ReceiveBlock(bool size, void *block)
{ /* one second time out during data receive */
  bool block_completed=FALSE;
  word actpos=0;
  word bytes_recv=0;
  word buffersize;
  byte std=0;
  byte min=0;
  byte secondsact=0;
  byte secondsget=0;
  byte diff=0;
  bool first=TRUE;

  switch (size) {
    case KB1  : buffersize = 1024; break;
    case B128 : buffersize = 128; break;
  }
  switch (CRC_CHECKSUM_MODE) {
    case CRCMODE : buffersize += 2; break;
    case CHECKSUMMODE : buffersize += 1; break;
  }
  while (!block_completed) {
    
    if (LibSrlRecvBlock(buffer+actpos, buffersize, &bytes_recv) == IW_SRL_TRSERR) {
      /* buffer is empty */
      if (first) {
        LibGetTime2(&std, &min, &secondsget);
        break;
      }
      LibGetTime2(&std, &min, &secondsact);
      diff = secondsact-secondsget;
      if (diff > 100) diff += 60;
      if (diff >= 2) { /* this is not exact, but has a minimum 
                          of 1 seconds */
        /* 1 second time out */
        return(1);
      }
    }
    else {
      /* bytes_recv bytes received */
      actpos += bytes_recv;
      if (actpos >= buffersize) break;
    } /* if */
  } /* while */
  return(0);
}

void FinishFile(void)
{
}