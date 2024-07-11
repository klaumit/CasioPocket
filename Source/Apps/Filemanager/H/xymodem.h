#ifndef XYMODEM
#define XYMODEM

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>
#include <fm.h>
#include <stdprocs.h> /* buffer[2048] */
/* #include <timing.h> */

#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define CAN 0x18
#define C   0x43
#define G   0x47

/* size */
#define KB1  TRUE
#define B128 FALSE

/*  CRC_CHECKSUM_MODE */
#define CRCMODE      TRUE
#define CHECKSUMMODE FALSE

extern bool CRC_CHECKSUM_MODE;

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
                    1 = no response
*/
extern byte InitReceive(FILEINFO *fi);

/* sends C (in the case of no answer it will be repeated 4 times
   with timeout seconds between it) and checks for an STX or SOH.
   
  Parameters:
  byte timeout     seconds between the 4 retrys
                   SHOULD BE 3 TO BE YMODEM COMPATIBLE

  Return values:
  byte             returns an error if no answer is send
                   0
                   1 = Timed out(no response after 4 retrys)   

extern byte RequestCRCTransfer(byte timeout); */

/* receives a single block checks the crc(checksum) and sends a 
   ACK, checks if the ACK is correct answered. If the procedure
   receives the last block and ACK correct it will Preread for
   an EOT and return, to continue with FinishFile.

  Parameters:
  bool size        returns the size of the received block(1KB, 128B)
  byte *block      pointer to an 1KB block

  Return values:
  byte             returns timeout
                   0 = no time out
                   1 = one second time out occured
*/
extern byte ReceiveBlock(bool size, void *block);

/* FinishFile ACK the EOT (EOT, NAK, EOT, ACK)

  no Parameters.
*/
extern void FinishFile(void);

#endif