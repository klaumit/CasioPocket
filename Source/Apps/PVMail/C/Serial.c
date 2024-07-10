/*****************************************************
 * Communication Functions

     Copyright (C) 2001 Fabio Fumi
	f_fumi@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *****************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_libc.h"

#include	"Flash.h"
#include    "Serial.h"
#include    "Common.h"

extern TMailRecord MailRecord;
extern TCache      Cache;
extern int         CurMail; 
extern int         MailNum;
extern FILE_BUF   mail_fb;
extern FILE_INFO  mail_fi;
extern TPvFile    hMailFile;
extern byte       MailFileName[];
extern BK_SMPL_TCH bk[2];
extern long       SerialError;

bool   UserBreak = FALSE;


/******************************************************************************
	[Title]		Break Key Sample check (from Kino)
	[Arguments]	
	[ReturnVAL]	
******************************************************************************/
static byte MyBkSampleCheck(bool *done)
{
	struct { byte main,sub,scrt; } fb;
	FILE_INFO fi;
	int Esc;

	/* dummy call to flash bios. without this, no touch screen scanning */
	fb.main = 0x10;
	fb.sub = 0;
	fb.scrt = 0x80;
	fi.fp = 0xffff;
	fi.kind = 0;
	LibFileFindNext((const FILE_BUF *)&fb,&fi,0);

    Esc = LibBkSampleCheck(done);
    
    if (Esc != OFF)
        UserBreak = TRUE; /* Set a global var */
    
	return (Esc);
}

/******************************************************************************
*	[Title]		Wait for a sync byte in the serial input buffer
*	[Arguments]	None
*	[ReturnVAL]	Sync byte ASCII value; -1 if break by user
******************************************************************************/
static byte WaitByte(void) {
	byte buf;
	bool done;

	buf = '\0';
	while (TRUE) {
	
		/* *** A TIME-OUT CHECK SHOULD BE ADDED !!! *** */
		
		/* Check the 'esc' press */
		if (MyBkSampleCheck(&done) != OFF) {
			LibWinIcnMsg(ICON_BIKKURI,387,2);
			/* Send a break (SERVER MODE ONLY) ^/
			if (LibSrlSendByte(IB_FOLLOW_BUSY, BRK) != IW_SRL_NOERR)
			*/
			SerialError += 1;
			return (-1);
		}

		if(LibSrlRecvByte(&buf) == IW_SRL_NOERR)
			return (buf);
			
	}

}

/******************************************************************************
*	[Title]		Wait for an ACK byte in the serial input buffer
*	[Arguments]	None
*	[ReturnVAL]	TRUE when ACK arrived, FALSE if anything else arrived
******************************************************************************/
static bool WaitACK(void){

	if (WaitByte() != ACK) {
		SerialError += 5;
		return (FALSE);
	}

	return(TRUE);

}

/******************************************************************************
*	[Title]		Receive
*               Receive a string from serial port
*               Terminates when 'size' is reached
*	[Arguments]	size: exact number of expected bytes
                str: pointer to the string buffer to be filled up
*	[Return]    TRUE: no error
*               FALSE: interrupted by user, or size exceeded
******************************************************************************/
static bool Receive(byte *str, int size){
	bool  done;
	int   i = 0;
	word  n_read = 0;
	byte  tmpbuf[SER_BUF_SIZE];

    while (TRUE){
    	while (LibSrlRecvBlock(tmpbuf, SER_BUF_SIZE, &n_read) != IW_SRL_NOERR) {
    		/* Test ESC key touch */
        	/* *** A TIME-OUT CHECK SHOULD BE ADDED !!! *** */
    		if (MyBkSampleCheck(&done) != OFF) {
    			LibWinIcnMsg(ICON_BIKKURI,387,2);
    			/* *** IF IN 'SERVER' MODE, SHOULD SEND A 'BRK' ALSO *** */
    		    SerialError += 1;
    			return (FALSE); /* Interrupted by user */
    		}
    	}
	
	   	if ((i+n_read) > size){
	        SerialError += 2;
			return (FALSE); /* Size exceeded maximum */
		} 
	    
	    /* Copy just loaded bytes */
		memcpy(&str[i], tmpbuf, n_read);
		i += n_read;
		
		if (i == size)
		    return (TRUE); /* All expected bytes were received */
    }	

}

/******************************************************************************
*	[Title]		ReceiveMail
*               Receive mail messages from Outlook.
*               Terminates if either last mail (END), or a BRK signal
*               is received.
*	[Arguments] none
*	[Return]    TRUE: no error
*               FALSE: Any error (message managed inside)
******************************************************************************/
static bool ReceiveMail(void){
	byte        cmd;
	int         n_cmd = 0;
	byte        NumBuf[NUMBUF_SIZE];
	int         EntryIDlen = 0;
    /* TMailRecord *Msg = (TMailRecord *) BINBUF(mail_fb); */
	FILE_INFO   fi;
	
	fi.kind = FILE_KIND_BIN;
	ASSIGN_BUF(mail_fb, hMailFile);
	 
    /* main loop on received commands */
    while(TRUE) {
        
        n_cmd ++;

	    LibWait(IB_125MWAIT);	 
	    if ((cmd = WaitByte()) == -1){
	        SerialError += 400;
		    return (FALSE);
		}
		if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
	        SerialError += 500;
		    return (FALSE);
    	}

		switch (cmd){
		    case CREATE: /* Receive a new message to save */
		    
		    	/* First check if aReceiveMailny more messages can be received */
				if (MailNum == MAX_MAIL - 1){
				    SerialError += 550;
		            return(FALSE);
				}
		    
		        /* Receive inbox/outbox flag */
				if (!WaitACK()) {
    		        SerialError += 600;
		            return(FALSE);
    	        }		
			    if (!Receive(NumBuf, NUMBUF_SIZE)){ 
			        SerialError += 605;
			        return (FALSE);
				}
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 610;
            		return (FALSE);
            	}
		        MailRecord.MailBox = atol(NumBuf);

		    	/* Wait until server is ready to send, then receive EntryID length 
			       (Length of message EntryID; from the Outlook mail item) */
				if (!WaitACK()) {
    		        SerialError += 615;
		            return(FALSE);
    	        }		
			    if (!Receive(NumBuf, NUMBUF_SIZE)){ 
			        SerialError += 620;
			        return (FALSE);
				}
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 625;
            		return (FALSE);
            	}
		        EntryIDlen = atol(NumBuf);
		
				/* actual EntryID string */
				if (!WaitACK()) {
    		        SerialError += 630;
		            return(FALSE);
    	        }		
			    if (!Receive(MailRecord.EntryID, EntryIDlen + 1)){ 
			        SerialError += 640;
			        return (FALSE);
				}
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 650;
            		return (FALSE);
            	}
		        /* Check consistency (lengths) */
            	if (strlen(MailRecord.EntryID) != EntryIDlen){
            	    SerialError += 660;
		    		LibPutMsgDlg("%d %d",
				        strlen(MailRecord.EntryID),
					    EntryIDlen);
            		return (FALSE);		
    		    }

	            /* Wait until server is ready to send Data Size*/
				if (!WaitACK()) {
    		        SerialError += 670;
		            return(FALSE);
    	        }     
		        /* get Data Size */	
			    if (!Receive(NumBuf, NUMBUF_SIZE)){	    
  			        SerialError += 680;
    		        return (FALSE);
				}
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 690;
            		return (FALSE);
            	}
		        MailRecord.DataSize = atol(NumBuf);
                
	            /* Wait until server is ready to send actual Data*/
				if (!WaitACK()) {
    		        SerialError += 700;
		            return(FALSE);
    	        }   		
			    /* Get actual message Data */ 
				if (!Receive(MailRecord.Data, MailRecord.DataSize + 1)){	    
  			        SerialError += 710;
    		        return (FALSE);
				}
				/* Check consistency */
				if ( MailRecord.DataSize != strlen(MailRecord.Data) ){
  			        SerialError += 720;
    		        return (FALSE);
				}
				
				/* Set up proper flags */
				if (MAIL_INBOX == MailRecord.MailBox)
				    MailRecord.New = TRUE;
				else if  (MAIL_OUTBOX == MailRecord.MailBox)
				    MailRecord.New = FALSE;
				else {
			        SerialError += 725;
				    return (FALSE);					    
				}

	            /* Append new message to Flash file */
	            if (!saveMail(0xffff)) {
				    LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
                    fatalError("FATAL\rReceiveMail\rSave failed");
		  	    } else {
			        /* OK - msg received and saved - reply with ACK to ask for the next one */
        		    if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
			            SerialError += 740;
				        return (FALSE);
				    }
				}
				
				break;
				
			case DELETE:
		        /* Receive the flash pointer of record to delete */
			    if (!Receive(NumBuf, NUMBUF_SIZE)){ 
			        SerialError += 750;
			        return (FALSE);
				}
				
			    /* Delete this record from file */
		        fi.fp = atol(NumBuf);
			    if (LibFileRemove(&mail_fb,&fi) != TRUE){
				    LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
				    fatalError("FATAL\rReceiveMail\rRemove failed");
			    }
			
			    /* Answer: OK, done! */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 760;
            		return (FALSE);
            	}
			    
			    break;				

			case ALL_SENT:
	            return (TRUE);
		        break;
			
		    default:
				/* IF IN SERVER MODE, SHOULD SEND A 'BRK' ALSO */
				SerialError += 770;
				return (FALSE); 
				
		} /* switch cmd */
    } /* while */
    

}


/******************************************************************************
*	[Title]		Send
*               Send a string to serial port
*               Terminates when 'size' is reached
*	[Arguments]	size: exact number of bytes to send
                str: pointer to the string buffer to be sent
*	[Return]    TRUE: no error
*               FALSE: interrupted by user
******************************************************************************/
bool Send(byte *str, int size){
    bool done;
    int i = 0;
    
    for (i=0; i<size; i++){
    
        /* Loop until a call give no error */
        while(LibSrlSendByte(IB_FOLLOW_BUSY, str[i]) != IW_SRL_NOERR){
	    	if (MyBkSampleCheck(&done) != OFF){
    	    LibWinIcnMsg(ICON_BIKKURI,387,2);
    		SerialError += 1;
    		return (FALSE); /* Interrupted by user */
    	    }
	    }
	
    }
       
    return(TRUE); /* All bytes sent */
}


/******************************************************************************
*	[Title]		DeleteMail
*               Receive from Outlook which messages have to be deleted
*               Terminates if either last mail (END), or a BRK signal
*               is received.
*	[Arguments] none
*	[Return]    TRUE: no error
*               FALSE: Any error (message managed inside)
******************************************************************************/
static bool ReceiveDeleteMail(void){
	byte        cmd;
	int         n_cmd = 0;
	byte        NumBuf[NUMBUF_SIZE];
	byte        buf[50];
	FILE_INFO   fi;

	fi.kind = FILE_KIND_BIN;
	ASSIGN_BUF(mail_fb, hMailFile);
	 
    /* main loop on received commands */
    while(TRUE) {
        n_cmd ++;
        
		LibWait(IB_125MWAIT);	 
	    if ((cmd = WaitByte()) == -1){
	        SerialError += 400;
		    return (FALSE);
		}
		if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
	        SerialError += 500;
		    return (FALSE);
    	}
	
	    switch(cmd){
	    
			case DELETE:
		        /* Receive the Flash pointer of record to delete */
			    if (!Receive(NumBuf, NUMBUF_SIZE)){ 
			        SerialError += 650;
			        return (FALSE);
				}
				
			    /* Delete record from file */
		        fi.fp = atol(NumBuf);
			    if (LibFileRemove(&mail_fb,&fi) != TRUE){
			        sprintf (buf, "FATAL\rDeleteMail\r%s", NumBuf);
				    /* Answer: Error */
				    LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
				    fatalError(buf);
			    }
			
			    /* Answer: OK, done! */
				if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
            	    SerialError += 700;
            		return (FALSE);
            	}
			    
			    break;
			   
			case ALL_SENT:
		
		        /* This is last communication action (then port is closed) */
				if (!WaitACK()) {
    	            SerialError += 810;
        		    return(FALSE);
            	}
	    
		        return (TRUE);
		        break;
			
		    default:
				SerialError += 750;
				return (FALSE); 
				
		} /* switch cmd */
    } /* while */

}


/******************************************************************************
*	[Title]		SendMail
*               Send mail messages to Outlook.
*               Consider only messages which were never sent, so that
*               the EntryID field is still empty. After sending it, update
*               the EntryID to avoid sending again it.
*	[Arguments] none
*	[Return]    TRUE: no error
*               FALSE: Any error (message managed inside)
******************************************************************************/
static bool SendMail(void){
	byte        SizeBuf[NUMBUF_SIZE];
	byte        FmtBuf[6];
	word        EntryIDlen;
	FILE_INFO   fi;
    int i, N;

	ASSIGN_BUF(mail_fb, hMailFile);
	/* Count total file records */
	N = LibGetFileCnt(&mail_fb);
	fi.fp = 0xffff;
	fi.kind = FILE_KIND_BIN;
	
	/* Scan all Flash file records */		
	for (i=0; i <= N - 1; i++) {
		if (!LibFileFindNext(&mail_fb,&fi,0)){
		    LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
			fatalError("FATAL\rSendMail"); /* File is corrupted! */
		}
		LibFileRead(&mail_fb,&fi);
        memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
		Cache.Data     = MailRecord.Data;
	    Cache.DataSize = &(MailRecord.DataSize);
		Cache.EntryID  = MailRecord.EntryID;
		Cache.New      = &(MailRecord.New);
		Cache.MailBox  = &(MailRecord.MailBox);
		Cache.fp       = fi.fp ;
		
		if (*(Cache.New) && (MAIL_OUTBOX == *(Cache.MailBox))){	
		/* Send OUTBOX NEW (=never sent) messages */
		
		    /* Wait until server is ready to receive a new message */
		    if (!WaitACK()) {
    	        SerialError += 600;
    		    return(FALSE);
        	}

            /* Tell server to wait for a new message */
	    	if (LibSrlSendByte(IB_FOLLOW_BUSY, CREATE) != IW_SRL_NOERR){
	            SerialError += 610;
		        return (FALSE);
            }
		    if (!WaitACK()) {
    	        SerialError += 620;
    		    return(FALSE);
        	}
	    
	    	/* Send Data size */
	        sprintf(FmtBuf, "%%%d.%dd", NUMBUF_SIZE -1, NUMBUF_SIZE-1);
	        sprintf(SizeBuf, FmtBuf, strlen(Cache.Data));
		    if (!Send(SizeBuf, NUMBUF_SIZE)){	    
  			    SerialError += 630;
    		    return (FALSE);
			}
			if (!WaitACK()) {
    		    SerialError += 640;
		        return(FALSE);
    	    }
		    
			/* Send Actual Data */ 
			if (!Send(Cache.Data, strlen(Cache.Data))){	    
  			    SerialError += 650;
    		    return (FALSE);
			}

		    /* Wait until server is ready to send the EntryID */
			if (!WaitACK()) {
    		    SerialError += 655;
		        return(FALSE);
    	    }		
			if (!Receive(SizeBuf, NUMBUF_SIZE)){ 
			   SerialError += 660;
			   return (FALSE);
			}
			if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
               SerialError += 665;
               return (FALSE);
            }    
			EntryIDlen = atol(SizeBuf);
			
		    /* actual EntryID string */
			if (!WaitACK()) {
    		   SerialError += 670;
		       return(FALSE);
    	    }		
			if (!Receive(Cache.EntryID, EntryIDlen + 1)){ 
			   SerialError += 675;
			   return (FALSE);
			}
			if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
		        SerialError += 680;
            	return (FALSE);
            }
		    /* Check consistency (lengths) */
            if (strlen(Cache.EntryID) != EntryIDlen){
                SerialError += 685;
		    	LibPutMsgDlg("%d %d",
				    strlen(Cache.EntryID),
				    EntryIDlen);
            	return (FALSE);		
    		}
		    
		    /* Update message flags (now it has been sent) */
		    *(Cache.New) = FALSE;
		    if (!saveMail(Cache.fp)){
  			    SerialError += 690;
    		    return (FALSE);
		    }
		    
		} else if ( strlen(Cache.EntryID) ) {
		/* Tell server a new ID/fp will be sent */
		
		    /* Wait until server is ready to receive a new EntryID */
		    if (!WaitACK()) {
    	        SerialError += 600;
    		    return(FALSE);
        	}
		
	    	if (LibSrlSendByte(IB_FOLLOW_BUSY, SAVE_ID) != IW_SRL_NOERR){
	            SerialError += 700;
		        return (FALSE);
            }
		    if (!WaitACK()) {
    	        SerialError += 705;
    		    return(FALSE);
        	}
		
			/* Send EntryID size */
			sprintf(FmtBuf, "%%%d.%dd", NUMBUF_SIZE-1, NUMBUF_SIZE-1);
	        sprintf(SizeBuf, FmtBuf, strlen(Cache.EntryID));
		    if (!Send(SizeBuf, NUMBUF_SIZE)) {	    
  			    SerialError += 710;
    		    return (FALSE);
			}
			if (!WaitACK()) {
    		    SerialError += 715;
		        return(FALSE);
    	    }
		    
			/* Send EntryID */ 
			if (!Send(MailRecord.EntryID, strlen(Cache.EntryID))){	    
  			    SerialError += 720;
    		    return (FALSE);
			}
			if (!WaitACK()) {
    		    SerialError += 725;
		        return(FALSE);
    	    }
			
			/* Send Flash 'fp' pointer */
	        sprintf(SizeBuf, "%8.8x", fi.fp);
			if (!Send(strupr(SizeBuf), 8+1)){
  			    SerialError += 730;
    		    return (FALSE);
			}
			if (!WaitACK()) {
    		    SerialError += 735;
		        return(FALSE);
    	    }
	    
	    
		}
	}
	
	
	if (!WaitACK()) {
        SerialError += 740;
        return(FALSE);
    }
    /* Terminate sending */
	if (LibSrlSendByte(IB_FOLLOW_BUSY, ALL_SENT) != IW_SRL_NOERR){
	    SerialError += 750;
	    return (FALSE);
    }
	
    return (TRUE);
				
}

/******************************************************************************
*	[Title]		Handshake
*               Initial communication handshake and synchronization
*	[Arguments]	None
*	[ReturnVAL]	TRUE = No error
******************************************************************************/
static bool Handshake(void) {

	    /* Outlook counterpart is :
        Serial.Output = Chr$(ACK)
        sInStr = Receive()
        If sInStr <> Chr$(ACK) Then Exit Sub
    	*/
	    if (!WaitACK()) {
    		SerialError += 100;
		    return(FALSE);
    	}
    	if (LibSrlSendByte(IB_FOLLOW_BUSY, ACK) != IW_SRL_NOERR){
    	    SerialError += 200;
		    return(FALSE);
	    }
	    
	    return TRUE;
}


/******************************************************************************
*	[Title]		SyncMail
*	[Arguments]	None
*	[ReturnVAL]	TRUE = No error
******************************************************************************/
bool SyncMail(void){
    bool retval;
    SRL_STAT    s;
	SerialError = 0; /* Global var keeping error reason */
	UserBreak = FALSE; /* Global var set to TRUE when 'Esc' is pressed */

	/*******************/
	/* Start-up stuff  */
	
	s.port  = IB_SRL_COM2;     /* Open cradle serial port */
	s.speed = IB_SRL_38400BPS;
	s.parit = IX_SRL_NONE;
	s.datab = IX_SRL_8DATA;
	s.stopb = IX_SRL_1STOP;
	s.fctrl = IX_SRL_RSCS;     /* IX_SRL_XONOFF for Xon/Xoff control */

    /* Show the Communication Picture */
    LibOpenWindow(0, 0, 160, 160);
    LibClrDisp();
    LibPutFarData(0,0,271);
    LibDspWinMessage(25,388,0,IB_MWIN_NONE);
    LibPutDisp();
    
    /* Turn off light */
	LibELHandle(IB_ELP_OFF);
	
    /* Init the 'ESC' key sampling */
    bk[0].x1 = TchHardIcon[7].x1;
	bk[0].y1 = TchHardIcon[7].y1;
	bk[0].x2 = TchHardIcon[7].x2;
	bk[0].y2 = TchHardIcon[7].y2;
	bk[1] = bk[0];
   	LibBkSampleInitSub(bk,IX_ESCBRK | IX_BLD1MSG);

    /****************************/
	/* Open port and do the job */
   	SerialError = 1000;
	if (retval = (LibSrlPortOpen(&s)==IW_SRL_NOERR)) {
	    LibSrlRxBufClr();
        LibSrlTxBufClr();
	
	    /* 0-th step - Communication handshake */
        if (retval = Handshake()){

            /* - 1nd step - */
            /* Receive mail messages (both to create as new unread,
	           or remove since deleted on the PC server */
        	SerialError = 3000;
            if (retval = SendMail()){

                /* - 2nd step - */
			    /* Send new messages to the PC server */
                SerialError = 4000;
                if (retval = ReceiveMail()){
		
	                /* - 3rd step - */
    			    /* Receive from server the list of messages to delete */
			        SerialError = 5000;
		            retval = ReceiveDeleteMail();
		
		        }
		    }
	    }
    }
    
    /******************/
    /* Close-up stuff */
    /* Show the error code (only if it's not due to 'Esc' pressed) */
    if (!retval && !UserBreak){
        LibSrlSendByte(IB_FOLLOW_BUSY, ERR);
        LibPutMsgDlg("Serial Error\r\r(code #%d)", SerialError);
	}

    /* Clear and close serial port */
    LibSrlRxBufClr();
    LibSrlTxBufClr();
    LibSrlPortFClose();
    
    LibCloseWindow();
    LibPutDisp();
	return (retval);
    
}


