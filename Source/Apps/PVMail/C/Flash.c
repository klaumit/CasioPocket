/******************************************************************************
*	
*	[Title]	  PVMail
*             Flash File Operations 
*             (Many parts copied and changed from the Kino program,
*             by Wittawatt Wamyong)
* 
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

******************************************************************************/

#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"l_libc.h"

#include    "Flash.h"
#include    "Common.h"

/********** Exernal variables **************/
extern word    far RecListFP[MAX_MAIL];
extern TMailRecord MailRecord;
extern TCache      Cache; 
extern FILE_BUF    mail_fb;
extern TPvFile     hMailFile;
extern byte        MailFileName[];
extern int         SourceMail;
extern int         CurMail;
extern int         MailNum;
extern byte        MailBox;
extern bool        bSort;
extern byte        SearchMsg[];

extern void SplitItems(byte *m, TMailMessageItems *mi);

/******
 Functionality to be implemented yet 
extern void far_qsort(char far *base,int nel,int width, int (*compare)());
******/

/*******************************************************************
	[Title]	PvOpenFile (copied from Kino)
	        Open a file from Flash
    [Arguments]	filename: Name of the file
                TPvFile: structure with file infos
		        fmode: 0 (open an old file)
			           FM_CREATE (create a new one)
			           FM_SECRET (open / create in secret mode)     
				       FM_NEW (re-create the file)
	[Returns] FALSE if any error occurred, or file doesn't esist 
	          TRUE otherwise
 *******************************************************************/
bool PvOpenFile(char *filename, TPvFile *f, int fmode)
{
	if (((fmode & FM_NEW) == FM_NEW) &&
        LibSubEntrySearch(filename,&f->sub) == TRUE)
        return (FALSE); /* Error: file exists */
	if (fmode & FM_CREATE) {
		if (LibSubEntrySave(filename, &f->sub) == FALSE)
			return (FALSE); /* Error: file exists */
	} else {
		if (LibSubEntrySearch(filename,&f->sub) == FALSE)
			return (FALSE); /* File not found */
	}
	
	/* File found */
	LibGetAllEntry(filename,&f->main,&f->sub);
	if (fmode & FM_SECRET) {
        f->area = FILE_SECRET_MODE;
	} else {
		f->area = FILE_OPEN_MODE;
	}
	
	return (TRUE);
}

/******************************************************************************
	[Title]	initRecord
******************************************************************************/
void initRecord(TMailRecord *m)
{
	memset(m,0,sizeof(TMailRecord));
}

/******************************************************************************
	[Title]	getRecordSize
******************************************************************************/
word getRecordSize(TMailRecord *m)
{
    word len;
    
    /* Variable size, depending on TMailRecord structure, and string lenghts */
	len = sizeof(byte)                              /* New           */
	    + sizeof(byte)                              /* MailBox       */
        + sizeof(long)                              /* Datasize      */
	    + sizeof(byte)*(MAIL_ENTRYID_SIZE+1)        /* EntryID       */
		+ sizeof(byte)*(strlen(m->Data)+1);         /* Data (variable size) */
		    
	return (len);
}

/******************************************************************************
	[Title]	Sorting criteria
******************************************************************************/
int SortAsc(word far *r1, word far *r2){

    byte Msg1[MAIL_DATA_SIZE + 1];
    byte Msg2[MAIL_DATA_SIZE + 1];
    TMailMessageItems mi1;
    TMailMessageItems mi2;
	FILE_INFO fi;
    
    fi.kind = FILE_KIND_BIN;
    		
    /* Load and split message data for sorting */
	fi.fp = *r1;
	LibFileRead(&mail_fb,&fi);
    memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
    strcpy(Msg1, MailRecord.Data);
	SplitItems(Msg1, &mi1);
	   
	fi.fp = *r2;
	LibFileRead(&mail_fb,&fi);
    memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
    strcpy(Msg2, MailRecord.Data);
	SplitItems(Msg2, &mi2);
	
    return (strcmp(mi1.Date,mi2.Date));
}

int SortDesc(word far *r1, word far *r2){

    byte Msg1[MAIL_DATA_SIZE + 1];
    byte Msg2[MAIL_DATA_SIZE + 1];
    TMailMessageItems mi1;
    TMailMessageItems mi2;
	FILE_INFO fi; 
    
    fi.kind = FILE_KIND_BIN;
    		
    /* Load and split message data for sorting */
	fi.fp = *r1;
	LibFileRead(&mail_fb,&fi);
    memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
    strcpy(Msg1, MailRecord.Data);
	SplitItems(Msg1, &mi1);
	   
	fi.fp = *r2;
	LibFileRead(&mail_fb,&fi);
    memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
    strcpy(Msg2, MailRecord.Data);
	SplitItems(Msg2, &mi2);
	    
	/* Predifined sort -> by date */
    return (strcmp(mi2.Date,mi1.Date));
    
}

/******************************************************************************
	[Title]	collectMail (copied from Kino)
	        Read all mail records from flash to fill the vector
		    of record information, and then process it (sort, filter, etc.)
******************************************************************************/
void collectMail(void)
{
	int i, N;
	FILE_INFO fi;
	word fp;
	
	Cache.fp = 0xffff; /* Reset cache */
	
	ASSIGN_BUF(mail_fb, hMailFile);
	/* Count total file records */
	N = LibGetFileCnt(&mail_fb);
	if (N > MAX_MAIL)
		N = MAX_MAIL; /* This error that should never happen !! */
	if (!N) {
	    MailNum = 0;
	    return;
	}
	
	fi.fp = 0xffff;
	fi.kind = FILE_KIND_BIN;
	
	/* Search file Header (TO BE IMPLEMENTED) ^/
	if (LibFileFindNext(&mail_fb,&fi,0) == FALSE)
		fatalError("collectMail: 0"); /^ Empty file ! ^/
	*/
		
	/* Scan the rest of file and load record pointers */		
	MailNum = 0;
	for (i=0; i <= N - 1; i++) {
		if (!LibFileFindNext(&mail_fb,&fi,0))
		    fatalError("collectMail");
		
		LibFileRead(&mail_fb,&fi);
        memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
        if (MailBox == MailRecord.MailBox){
	  		RecListFP[MailNum] = fi.fp;
			MailNum++;
		}
	}
	
	if (MailNum){
    	/* Reverse the array (to display most recent on top) */
    	for (i=0; i<(MailNum/2); i++) {
    	     fp = RecListFP[MailNum-i-1];
    	     RecListFP[MailNum-i-1]=RecListFP[i];
    	     RecListFP[i]=fp;
    	}
    
    
    	/*   Apply sorting (only if explicitly required)                      */
    	/*** NOTE 1: IT CRASHED FOR VERY LONG LISTS (because of recursion)    ***/
    	/*** NOTE 2: It took several (3-5) seconds to sort a 50-element list  ***/
/******
Functionality to be implemented yet 
    	if (bSort){
            far_qsort ((char far*)RecListFP,
                MailNum,
        	    sizeof(word),
        	    &SortDesc);
    	}
******/

	}

}


/******************************************************************************
	[Title]	LoadMail (copied from Kino)
	        Point the 'Cache' structure to the mail 'idx' of the current list
    [Arguments]	idx: index to the mail messages to retreive
******************************************************************************/
void LoadMail(int idx)
{
	FILE_INFO fi;

	if (idx > MailNum)
		return; /* Error! */
	else if (idx == MailNum) {
        /* If it's last in the list ... ??? ^/
		m = (TMailRecord *) BINBUF(mail_fb);
		if (SourceMail == -1) {
			initRecord(m);
			BINBUF_SIZE(mail_fb) = getRecordSize(m);
		} else {
			ASSIGN_BUF(mail_fb, hMailFile);
			fi.fp = RecListFP[SourceMail];
			fi.kind = FILE_KIND_BIN;
			LibFileRead(&mail_fb,&fi);
		}
		*/
		Cache.fp = 0xffff;
	} else {
		if (RecListFP[idx] == Cache.fp) /* Already here! */
			return;
		ASSIGN_BUF(mail_fb, hMailFile);
		fi.fp = RecListFP[idx];
		fi.kind = FILE_KIND_BIN;
		LibFileRead(&mail_fb,&fi);
		Cache.fp = RecListFP[idx];
	}

	/* Update the GLOBAL memory record */
    memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
	/* Set cache to point to mail record fields */
	Cache.Data     = MailRecord.Data;
    Cache.DataSize = &(MailRecord.DataSize);
	Cache.EntryID  = MailRecord.EntryID;
	Cache.New      = &(MailRecord.New);
	Cache.MailBox  = &(MailRecord.MailBox);
	Cache.fp       = fi.fp ;
		
}

/******************************************************************************
	[Title]	SearchMail
	        Look for first message containing a string.
		    Search only over messages of the currently shown list.
		    Ignore upper/lower case.
    [Arguments]	*mi: message to look for.
               *idx: index of message to start looking from (in current 'box').
	           mode: SRCH_ITEM to make an item-by-item comaprison.
		             SRCH_ALL to compare mi->Subject with all of each message.
    [Return] TRUE: message found, FALSE: not found.
             *idx: if found, index to FP array for searched message (CurMail).
	         Cache global variable pointing to found message.
******************************************************************************/
int SearchMail(int *idx, int mode)
{

    TMailMessageItems mi1; 
    TMailMessageItems mi2; 
	int i, j;
	FILE_INFO fi;
	bool found;

 /* fi.fp = 0xffff   - Per iniziare sempre dal primo della lista */
	fi.kind = FILE_KIND_BIN;
	
	/* Scan list */
	for (i=*idx; i < MailNum; i++) {
		fi.fp = RecListFP[i];
		LibFileRead(&mail_fb,&fi);
		memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
		if (MailRecord.MailBox == MailBox){
		    found = FALSE;
		    /* Capitalize strings, before comparison */
		    strupr(MailRecord.Data);
		    strupr(SearchMsg);
		    switch (mode){
		      case SRCH_ITEM:
    		    /* We want to compare item by item */
		    
		      /**** PRESENTLY DISABLED MODE; 
		          This search mode cannot work as it is now, since
		          the mi1 and mi2 items are not zero-terminated strings!
			      Maybe such a feature is not needed at all.
		      *****/
		    
    		    SplitItems (MailRecord.Data, &mi1);
		        SplitItems (SearchMsg, &mi2);     
    		    found=((   mi2.Subject[0]=='\0'
		                || strstr(mi1.Subject, mi2.Subject) != NULL )
		            && (   mi2.From[0]=='\0'
		                || strstr(mi1.From, mi2.From) != NULL )
			        && (   mi2.To[0]=='\0'
		                || strstr(mi1.To, mi2.To) != NULL )
			        && (   mi2.Body[0]=='\0'
		                || strstr(mi1.Body, mi2.Body) != NULL )
			        && (   mi2.Date[0]=='\0'
		                || strstr(mi1.Date, mi2.Date) != NULL )
			        && (   mi2.Folder[0]=='\0'
		                || strstr(mi1.Folder, mi2.Folder) != NULL )
			        && (   mi2.CC[0]=='\0'
		                || strstr(mi1.CC, mi2.CC) != NULL ) 
					&& (   mi2.BCC[0]=='\0'
		                || strstr(mi1.BCC, mi2.BCC) != NULL ));
			    break;
			    
		      case SRCH_ALL:
			    /* We want to search in any part of the message */
			  /*****
			      NOTE: This search mode have crashed the PV.
			      TO INVESTIGATE ON...
			      Does a 'search' library function exist?
			  *****/
			    found = ( strstr(MailRecord.Data, SearchMsg) != NULL );
			    break;
			    
			  default: /* This should never happen ! */
			    return(FALSE);
			    
			} /* switch */
			    
		    if (found){
		        /* Load found record */
                *idx = i;
				LibFileRead(&mail_fb,&fi);
        		memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
            	Cache.fp       = fi.fp;
                Cache.Data     = MailRecord.Data;
                Cache.DataSize = &(MailRecord.DataSize);
                Cache.EntryID  = MailRecord.EntryID;
                Cache.New      = &(MailRecord.New);
                Cache.MailBox  = &(MailRecord.MailBox);
		        /* Exit with 'Found' */
                return (TRUE);	
		    }
		  
	    } /* if (mailbox) */
	} /* for */
	
	/* Re-load originally selected record */
	fi.fp = RecListFP[*idx];
	LibFileRead(&mail_fb,&fi);
	memcpy(&MailRecord, BINBUF(mail_fb), BINBUF_SIZE(mail_fb));
	Cache.fp       = fi.fp;
    Cache.Data     = MailRecord.Data;
    Cache.DataSize = &(MailRecord.DataSize);
    Cache.EntryID  = MailRecord.EntryID;
    Cache.New      = &(MailRecord.New);
    Cache.MailBox  = &(MailRecord.MailBox);
	    
	/* Exit with 'Not found' */
    return (FALSE);		
}


/******************************************************************************
	[Title]	saveMail (copied from Kino)
	        Save the mail record to Flash file
    [Input] global structure MailRecord
	[Return] FALSE if any error occurred - TRUE otherwise
******************************************************************************/
bool saveMail(word fp)
{
	FILE_INFO fi;

	fi.fp = fp;
	fi.kind = FILE_KIND_BIN;

	ASSIGN_BUF(mail_fb, hMailFile);
	BINBUF_SIZE(mail_fb) = getRecordSize(&MailRecord);
	
	/* Save record found in the GLOBAL memory store area */
    memcpy(BINBUF(mail_fb), &MailRecord, BINBUF_SIZE(mail_fb));
	if (LibFileWrite(&mail_fb,&fi) == FALSE) {
	        LibWinIcnMsg(ICON_BADTZ,324,2); /* It's a bad error! */
	        return (FALSE);
	}
	
	/* Add to list only if current mailbox is the same of the message */
        /* and message is new - bmf*/
	if ( (MailBox == MailRecord.MailBox) && (fp==0xffff)){
    	RecListFP[CurMail] = fi.fp;
	    if (CurMail == MailNum)
	        MailNum++;
	}

	Cache.fp = fi.fp;

	return (TRUE);
	
}

/******************************************************************************
	[Title]	DeleteMail (copied from Kino)
	        Remove the mail record from Flash file
    [Input] index of the mail record to remove
    [Return] FALSE if any error occurred - TRUE otherwise
******************************************************************************/
bool DeleteMail(int idx)
{
	FILE_INFO fi;

	if (idx < 0 || idx >= MailNum || !MailNum)
		return (FALSE);
	
	/* Deleting! */
	LibDspWinMessage(24,329,0,IB_MWIN_NONE | IX_MWIN_CENTER);
    LibPutDisp();
				
	fi.fp = RecListFP[idx];
	fi.kind = FILE_KIND_BIN;
	if (LibFileRemove(&mail_fb,&fi) != TRUE)
		fatalError("DeleteMail");
	
	collectMail();
	/*
	if (Header.Movie > idx) {
		Header.Movie--;
		saveHeader();
	} else if (Header.Movie == idx) {
		Header.Movie = 0;
		saveHeader();
	}
	*/
	    
	return (TRUE);
}

/******************************************************************************
	[Title]		Fatal error message and exit function
******************************************************************************/
void fatalError(char *msg)
{
	LibPutMsgDlg(msg);
	LibJumpMenu();
	/* never returns */

	/* Use OFF to get out of this */

}


/* turned of by Marat Bakirov because this code used in debug mode only and is not needed really :) */
#if 0
/******************************************************************************
	[Title]	 CreateDummyRecords
	         **** USED ONLY FOR DEBUG *****
	         When a file is first created, to fill it with some dummy records
******************************************************************************/
bool CreateDummyRecords(void)
{  /***** ONLY FOR DEBUG ****/
   int i;
   TMailRecord  m;

	/* Please wait! */
	LibDspWinMessage(24,381,0,IB_MWIN_NONE | IX_MWIN_CENTER);
	LibPutDisp();
	
    i = MailNum + 5; /* message to create + 1*/
    for (MailNum; MailNum <= i; CurMail++) { 
    
        /* Prepare a record with dummy data */    
        strcpy(m.EntryID,
	        "08AFE830FACE830CFE0A9DF0202"
    		"0DA0BC920EDBF93B93BBD90830F"
		    "ACE83E830CFE020200CFE042DB7");
		    
        sprintf(m.Data, 
            "Subject msg #%d\x0FE"
            "From%d\x0FE"
            "To%d\x0FE"
            "Corpo del messaggio #%d "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body "
     "Test Big Body Test Big Body Test Big Body Test Big Body"
    	    "\x0FE"
            "Date%d\x0FE"
            "Folder%d\x0FE"
            "CC%d\x0FE"
            "BCC%d\x000",
	    	 MailNum,MailNum,MailNum,MailNum,
		     MailNum,MailNum,MailNum,MailNum);	   
		      
        m.DataSize = strlen(m.Data);
	    m.New      = TRUE;
	    m.MailBox  = MAIL_INBOX;
	
	    /* Copy new dummy to GLOBAL memory record */
	    memcpy(&MailRecord, &m, getRecordSize(&m));
	
    	/* Set cache to point to new record (serve davvero?) */
    	Cache.Data     = MailRecord.Data;
        Cache.DataSize = &(MailRecord.DataSize);
    	Cache.EntryID  = MailRecord.EntryID;
    	Cache.New      = &(MailRecord.New);
    	Cache.MailBox  = &(MailRecord.MailBox);
    	Cache.fp       = 0xffff;
    
        /* Save the new mail message mail */
        if (!saveMail(0xffff))
            return FALSE;
	}
    
    return TRUE;
}


#endif

