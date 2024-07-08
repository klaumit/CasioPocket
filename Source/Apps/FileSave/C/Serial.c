/*****************************************************
 * Communication Functions
 *****************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"Trm.h"
#include	"Flash.h"
#include    "Serial.h"
#include    "Common.h"

/*********** Variables ***********/
extern BK_SMPL_TCH bk[2];
extern long  FileSize;
extern long  totlen;
extern byte  block[BLOCK_SIZE + 1];
extern int   blocklen;
extern TFileIndex IndexArray[MAX_FILES];   /* Array of indexes to files */
extern TRecord  Record;   /* Single file record */



/******************************************************************************
*	[Title]		Wait for a sync byte in the serial input buffer
*	[Arguments]	None
*	[ReturnVAL]	Sync byte ASCII value
******************************************************************************/
int WaitSync(void) {
	byte buf;
	bool  onesec;
	int k = 0;

	buf = '\0';
	while (1) {

		/* Check the 'esc' press */
		if (MyBkSampleCheck(&onesec) != OFF) {
			LibWinIcnMsg(ICON_BIKKURI,387,2);
			/* Send a break */
			if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
				OutputTrm("\nBRK send err");
			return (-1);
		}

		if(LibSrlRecvByte(&buf) == IW_SRL_NOERR)
			return (buf);
			
	}

	return (0);

}

/******************************************************************************
*	[Title]		Wait for an ACK byte in the serial input buffer
*	[Arguments]	None
*	[ReturnVAL]	0 when ACK arrived, else 1
******************************************************************************/
int WaitACK(void){

	if (WaitSync() != ACK) {
		return (-1);
	}

	return(0);

}

/******************************************************************************
*	[Title]		Open serial port and wait for a sync byte
*	[Arguments]	None
*	[ReturnVAL]	mode command (could be either SND or RCV)
******************************************************************************/
int ListenPort(void){

	SRL_STAT s;

	bk[0].x1 = TchHardIcon[7].x1;
	bk[0].y1 = TchHardIcon[7].y1;
	bk[0].x2 = TchHardIcon[7].x2;
	bk[0].y2 = TchHardIcon[7].y2;
	bk[1] = bk[0];
	LibBkSampleInitSub(bk,IX_ESCBRK | IX_BLD1MSG);

	s.port = IB_SRL_COM2;
	s.speed = IB_SRL_38400BPS; /* 38400 */
	s.parit = IX_SRL_NONE;
	s.datab = IX_SRL_8DATA;
	s.stopb = IX_SRL_1STOP;
	s.fctrl = IX_SRL_RSCS; /* IX_SRL_XONOFF for Xon/Xoff control */

	LibELHandle(IB_ELP_OFF); /* Turn off light */
	if (LibSrlPortOpen(&s) != IW_SRL_NOERR) {
		LibWinIcnMsg(ICON_BADTZ,386,2);
		return (-1);
	}

	LibDspWinMessage(25,388,0,IB_MWIN_NONE);
	LibPutDisp();

	/* Starting Synchronization */
	/* wait for the starting sync byte */
	return (WaitSync());

}

/******************************************************************************
*	[Title]		Send header to serial port
*	[Arguments]	index in the array of file indexes
*	[ReturnVAL]	
******************************************************************************/
int SendHeader(int i){
	byte  buf[HEADER_LEN + 1];
	SRL_STAT s;

	bk[0].x1 = TchHardIcon[7].x1;
	bk[0].y1 = TchHardIcon[7].y1;
	bk[0].x2 = TchHardIcon[7].x2;
	bk[0].y2 = TchHardIcon[7].y2;
	bk[1] = bk[0];
	LibBkSampleInitSub(bk,IX_ESCBRK | IX_BLD1MSG);

	s.port = IB_SRL_COM2;
	s.speed = IB_SRL_38400BPS;
	s.parit = IX_SRL_NONE;
	s.datab = IX_SRL_8DATA;
	s.stopb = IX_SRL_1STOP;
	s.fctrl = IX_SRL_RSCS; /* IX_SRL_XONOFF for Xon/Xoff control */

	LibELHandle(IB_ELP_OFF); /* Turn off light */
	if (LibSrlPortOpen(&s) != IW_SRL_NOERR) {
		LibWinIcnMsg(ICON_BADTZ,386,2);
		return (-1);
	}

	/* Send a starting acknowledge byte */
	if (LibSrlSendByte(IB_FOLLOW_BUSY, RCV) != IW_SRL_NOERR) {
		OutputTrm("\nRCV send err");
		return (-1);
	}

	/* Wait for response */
	if (WaitSync() != SND){
		return (-1);
	}

	/* Prepare and send file name */
	if (LibSrlSendBlock(IndexArray[i].FileName, 
		strlen(IndexArray[i].FileName)) !=	IW_SRL_NOERR) {
		OutputTrm("\nFName send err");
		return (-1);
	}

	/* Wait for response */
	if (WaitACK()){
		return (-1);
	}

	/* Prepare and send file size */
	sprintf(buf, "%d", IndexArray[i].Size);
	if (LibSrlSendBlock(buf, strlen(buf)) != IW_SRL_NOERR) {
		OutputTrm("\nFSize send err");
		return (-1);
	}

	/* Wait for response */
	return (WaitACK());

}

/******************************************************************************
*	[Title]		Send one block to serial port
*	[Arguments]	None
*	[ReturnVAL]	0: OK to send next block; else error while sending
******************************************************************************/
int SendBlock(byte *block, int size){
	bool  onesec;
	int   i;

	/* Send the block, byte by byte */
	for (i = 0; i < size; i++){
		/* wait until the serial buffer can receive new data */
		while (LibSrlSendByte(IB_FOLLOW_BUSY, block[i]) != IW_SRL_NOERR){

			/* Check the 'esc' press */
			if (MyBkSampleCheck(&onesec) != OFF) {
				LibWinIcnMsg(ICON_BIKKURI,387,2);
				/* Send a break */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
					OutputTrm("\nBRK send err");
				return (BRK);
			}
		}
	}

	/* A full block was sent; now wait for a response */
	/* (Assume receiver is aware in advance of the block size) */
	return (WaitSync());

}

/******************************************************************************
*	[Title]		Receive one block of text from serial port
*               Terminates when 'size' is reached
*	[Arguments]	INPUT size (number of expected bytes to get from the port)
*	[ReturnVAL]	0: OK to receive next block; BRK: interrupted by user
******************************************************************************/
int ReceiveData(byte *block, int *size){
	bool  onesec;
	int   i;

	/* Send the block, byte by byte */
	for (i = 0; i < *size; i++){
		/* wait until the serial buffer has new data */
		while (LibSrlRecvByte(&block[i]) != IW_SRL_NOERR){
			/* Check the 'esc' press */
			if (MyBkSampleCheck(&onesec) != OFF) {
				LibWinIcnMsg(ICON_BIKKURI,387,2);
				/* Send a break */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
					OutputTrm("\nBRK send err");
				return (BRK);
			}
		}
	}

	
	*size = i + 1;
	return (0);

}


/******************************************************************************
*	[Title]		Receive one block of text from serial port
*               Terminates if either 'size' is reached or a '\0' is received
*	[Arguments]	INPUT size (number of expected bytes to get from the port)
*	[ReturnVAL]	0: OK to receive next block; BRK: interrupted by user
******************************************************************************/
int ReceiveText(byte *block, int *size){
	bool  onesec;
	int   i;

	/* Send the block, byte by byte */
	for (i = 0; i < *size; i++){
		/* wait until the serial buffer has new data */
		while (LibSrlRecvByte(&block[i]) != IW_SRL_NOERR){
			/* Check the 'esc' press */
			if (MyBkSampleCheck(&onesec) != OFF) {
				LibWinIcnMsg(ICON_BIKKURI,387,2);
				/* Send a break */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
					OutputTrm("\nBRK send err");
				return (BRK);
			}
		}

		if (block[i] == '\0') /* String terminator */
			break;
	}

	
	*size = i + 1;
	return (0);

}

/******************************************************************************
*	[Title]		Read the header from the serial port
*	[Arguments]	index in the array of file indexes
*	[ReturnVAL]	
******************************************************************************/
int ReceiveHeader(int i){
	byte  buf[MAX_SER_BUF + 1];
	byte  sTerm[256];
	int   len;
	SRL_STAT s;

	bk[0].x1 = TchHardIcon[7].x1;
	bk[0].y1 = TchHardIcon[7].y1;
	bk[0].x2 = TchHardIcon[7].x2;
	bk[0].y2 = TchHardIcon[7].y2;
	bk[1] = bk[0];
	LibBkSampleInitSub(bk,IX_ESCBRK | IX_BLD1MSG);

	s.port = IB_SRL_COM2;
	s.speed = IB_SRL_9600BPS; /* 38400 */
	s.parit = IX_SRL_NONE;
	s.datab = IX_SRL_8DATA;
	s.stopb = IX_SRL_1STOP;
	s.fctrl = IX_SRL_RSCS; /* IX_SRL_XONOFF for Xon/Xoff control */

	LibELHandle(IB_ELP_OFF); /* Turn off light */
	if (LibSrlPortOpen(&s) != IW_SRL_NOERR) {
		LibWinIcnMsg(ICON_BADTZ,386,2);
		return (-1);
	}

   	InitTrm();

	LibDspWinMessage(25,388,0,IB_MWIN_NONE);
	LibPutDisp();


	/* Wait for the starting sync byte */
	if (WaitSync() != RCV) {
		return(-1);
	}
	
	/* Send the acknowledge byte */
	if (LibSrlSendByte(IB_FOLLOW_BUSY, SND) != IW_SRL_NOERR) {
		return (-1);
	}

	len = FILENAME_LEN;
	/* Read the file name */
	if (ReceiveText(buf, &len) == IW_SRL_NOERR){
		sprintf(sTerm, "\n%s", buf);
		OutputTrm(sTerm);
		/* Put filename in the flash file header buffer */
		strcpy(IndexArray[i].FileName, buf);
	} else {
		return (-1);
	}

	/* Answer with an acknowledge byte */
	if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR) {
		return (-1);
	}

	len = FILESIZE_LEN;
	/* Read the file size */
	if (ReceiveText(buf, &len) == IW_SRL_NOERR){
		sprintf(sTerm, "/n%s/n", buf);
		OutputTrm(sTerm);
		FileSize = atol(buf);
	} else {
		return (-1);
	}

	/* Answer with an acknowledge byte */
	if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR) {
		OutputTrm("\nACK answer err");
		return (-1);
	}

	LibWait(IB_125MWAIT);
}


/******************************************************************************
*	[Title]		Read one block from serial port
*	[Arguments]	None
*	[ReturnVAL]	BRK if transfer was interrupted
*               STP if transfer finished
*               -1  if any erro occurred
******************************************************************************/
int ReceiveBlock_OLD(void){
	byte  ackbyte;
	byte  sTerm[256];
	int   len, i;

	bool  onesec;

	blocklen = 0;
	Record.Checksum = 0;
	for (;;){

		/* Check the 'esc' press */
		if (MyBkSampleCheck(&onesec) != OFF) {
			LibWinIcnMsg(ICON_BIKKURI,387,2);
			if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
				OutputTrm("\nBRK answer err");
			return (BRK);
		}

		/* Read data from serial port, and append it to the flash file buffer*/
		LibSrlRecvBlock(&(Record.Data[blocklen]), MAX_SER_BUF, &len);

		if (len != 0) {  /* data found */

			/* Update checksum */
			for (i = 0; i < len; i++)
				Record.Checksum += Record.Data[blocklen + i];

			blocklen += len;
			Record.Size = blocklen;
			totlen += len;
			sprintf(sTerm, "%d ", totlen);
			/* OutputTrm(sTerm); */

			if ((blocklen == BLOCK_SIZE)
				|| (totlen == FileSize)) {

				/* One full block received, or EOF reached */
				OutputTrm("*");

				/* send the 'block received' signal */
				if (totlen == FileSize)
					ackbyte = STP;
				else
					ackbyte = NXT;
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ackbyte) != IW_SRL_NOERR) {
					OutputTrm("\nBlock ack answer err");
					return (-1);
				}

				/* ... Receive checksum, and check it (TO DO) */
				/* if it's not correct:                          */
				/*       totlen-=blocklen;                       */
				/*       blocklen = 0;                           */
				/*       Send a CHK error (to re-transmit block) */
				/*       break;                                  */

				if (totlen == FileSize) {
					OutputTrm("\nFile completed");
					return (STP); /* File completed with no errors */
				}

				return (0); /* Block completed with no errors */

			} else if (totlen > FileSize) {
				sprintf (sTerm, "\nSize error"
					"\ntolen %d", totlen);
				OutputTrm(sTerm);
				/* Send an 'Error' signal */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ERR) != IW_SRL_NOERR) 
					OutputTrm("\nERR answer err");
				return (-1);
			}
		}

/*		
		} else if (len != 0) {
			sprintf (sTerm, "\nReceive error"
				"\ntolen %d len %d", totlen, len);
			OutputTrm(sTerm);
			/^ Send an 'Error' signal ^/
			if (LibSrlSendByte(IB_FOLLOW_BUSY, ERR) != IW_SRL_NOERR) 
				OutputTrm("\nERR answer err");
			return (-1);
		}
*/
	
	}	/* loop to next reading */


}

/******************************************************************************
*	[Title]		Read one block from serial port
*	[Arguments]	None
*	[ReturnVAL]	BRK if transfer was interrupted
*               STP if transfer finished
*               -1  if any erro occurred
******************************************************************************/
int ReceiveBlock(void){
	byte  ackbyte;
	byte  databyte;
	byte  buf[CHECKSUM_LEN];
	byte  sTerm[256];
	int   len;

	bool  onesec;

	blocklen = 0;
	Record.Checksum = 0;

	for (;;) {
		/* Wait data from serial port, and append it to the flash file buffer*/
		while (LibSrlRecvByte(&databyte)){
			/* Check the 'esc' press */
			if (MyBkSampleCheck(&onesec) != OFF) {
				LibWinIcnMsg(ICON_BIKKURI,387,2);
				if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
					OutputTrm("\nBRK answer err");
				return (BRK);
			}
		}

		/* one byte received */
		Record.Data[blocklen] = databyte;

		/* Update checksum */
		Record.Checksum += databyte;

		blocklen += 1;
		totlen += 1;
		Record.Size = blocklen;

		if ((blocklen == BLOCK_SIZE)
			|| (totlen == FileSize)) {

			/* Either one full block was received, or EOF was reached */
			OutputTrm("*");

			/* Send an ACK */
			if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR) {
				OutputTrm("\nBlock ACK answer err");
				return (-1);
			}

			/* Receive checksum, and check it */
			len = CHECKSUM_LEN;
			ReceiveText(buf, &len);

			sprintf(sTerm,"\n%d\n%s", Record.Checksum, buf);
			OutputTrm(sTerm);
			
			if (atol(buf) != Record.Checksum)
			{ /* Error in checksum */
				totlen-=blocklen;
				blocklen = 0;
				/* Send a CHK error (to re-transmit block) */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, CHK) != IW_SRL_NOERR) {
					OutputTrm("\nBlock CHK answer err");
					return (-1);
				}
			    break;
			} 

			/* Checksum OK: send the 'block received' signal */
			if (totlen == FileSize)
				ackbyte = STP;
			else
				ackbyte = NXT;

			if (LibSrlSendByte(IB_FOLLOW_BUSY, ackbyte) != IW_SRL_NOERR) {
				OutputTrm("\nBlk NXT/STP answer err");
				return (-1);
			}


			if (totlen == FileSize) {
				return (STP); /* File completed with no errors */
			}

			return (0); /* Block completed with no errors */

		} else if (totlen > FileSize) {
			/* Error in file size; send an 'Error' signal */
			LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
			return (-1);
		}
	
	}	/* loop to next reading */


}

/******************************************************************************
	[Title]		Break Key Sample check
	[Arguments]	
	[ReturnVAL]	
******************************************************************************/
byte MyBkSampleCheck(bool *onesec)
{
	struct { byte main,sub,scrt; } fb;
	FILE_INFO fi;

	/* dummy call to flash bios. without this, no touch screen scanning */
	fb.main = 0x10;
	fb.sub = 0;
	fb.scrt = 0x80;
	fi.fp = 0xffff;
	fi.kind = 0;
	LibFileFindNext((const FILE_BUF *)&fb,&fi,0);

	return (LibBkSampleCheck(onesec));
}

