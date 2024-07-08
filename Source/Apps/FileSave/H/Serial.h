
/******************************************************************************
*	[Title]		Comm header
*	[Model] 	PocketViewer
*	[Version]	Ver1.00
*
*	Copyright (C) 1997 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/

#ifndef  SERIAL_H
#define  SERIAL_H


/*********** Defines ****************/
#define ACK	1 /* Acknowledge */
#define BRK	2 /* Break signal */
#define ERR 3 /* Unrecoverable error */
#define CHK 4 /* Checksum error */
#define NXT 5 /* Next block request */
#define STP 6 /* Download complete with no errors */
#define RCV 7 /* Sync byte to set up Receive mode */
#define SND 8 /* Sync byte to set up Send mode */
#define BUF 9 /* Send next buffer (part of a block) */

#define MAXTRY	3
#define SER_BUF_SIZE 512 /* internal serial buffer size */

#define C_OPEN	0x10
#define C_CLOSE	0x11
#define C_DATE	0x12
#define C_REC	0x13
#define	C_TIME	0x14
#define C_TEXT  0x15
#define C_NOTE  0x16
#define C_ENDREC 0x18

#define B_COMM	1
#define B_PROTO 2
#define B_CANCEL 3

#define A_OK	0
#define A_ERR	1
#define A_WAIT  2
#define A_WARN	3


/*********** Types ****************/
typedef enum { sOpen,sClose,sDate,sRec,sData,sError,sBreak } TState;


/*********** Variables ***************/
static TState State;


/*********** Prototypes ***************/
byte MyBkSampleCheck(bool *onesec);
int ListenPort(void);
int ReceiveHeader(int i);
int ReceiveBlock(void);
int SendHeader(int i);
int SendBlock(byte *block, int size);
int ReceiveData(byte *block, int *size);
int ReceiveText(byte *block, int *size);

/*
static TState processRecord(byte packet, byte len, byte *buf);
static TState processDate(char *buf);
static int download();
*/

#endif /* SERIAL_H */