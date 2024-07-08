/* sx.h

   some definitions for X-MODEM and Y-MODEM protocol

   this is a very(!) simple implementation, sending part only

   Author: J. Steingraeber

   Released to the Public Domain
*/

#ifndef SX_INCLUDED
#define SX_INCLUDED

/* control characters */
#define SOH 1
#define STX 2
#define EOT 4
#define ACK 6
#define NAK 21
#define CAN 24

#define SX_MAXTRY_BLOCK 10
#define SX_TIMEOUT 20

/* protocol type */
#define SX_XMODEM 0
#define SX_XMODEM1K 1
#define SX_YMODEM 2
#define SX_YMODEM1K 3

#define SX_SENDPATH 0
#define SX_SENDDATA 1
#define SX_SENDFINISH 2
#define SX_NULL 3

#define SX_SENDNORMAL 0
#define SX_STOPAFTERFIRSTBLOCK 1

/*
  main routine to send data via XMODEM or YMODEM
  
  callback routines
    extern unsigned char sx_getchar(int iTimeout);
    extern void sx_sendchar(unsigned char c);
    extern int sx_getblock(unsigned char * pBuffer);
  must be provided by caller

  iProto is one of SX_XMODEM, SX_XMODEM1K, SX_YMODEM, SX_YMODEM1K
  iStop is used for last block of YMODEM protocol
  
  returns 0 if transmission succeeded
  retuns CAN if transmission was canceled by receiver
  returns NAK if transmission failed for some other reason
*/
int sx(int iProto, int iStop);

#endif
