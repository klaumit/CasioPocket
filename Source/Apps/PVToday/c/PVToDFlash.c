/******************************************************************************
*	[Title]		PVToday Flash functions
*	[Model] 	PocketViewer
*	[Version]	Ver 2.10
*
*	Copyright (C) 2003 Jean-Michel GIRARD . All rights reserved.
*       http://pocketcasio.free.fr
******************************************************************************/
#ifdef __HITACHI__
#include <hitachi.h>
#include "libc.h"
#define FAR /* nothing */
#define NEAR /* nothing */
#define far_strcpy strcpy
#define far_strncpy strncpy
#define far_strcmp strcmp
#define far_strncmp strncmp
#else
#include        <stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"
#define FAR far
#define NEAR near
#endif

#include        "PVToday.h"

/* Structs for PVMail compatibility */
#define MAIL_RECORD_SIZE       3000
#define MAIL_ENTRYID_SIZE      200
#define MAIL_DATA_SIZE         MAIL_RECORD_SIZE - ( MAIL_ENTRYID_SIZE + 1 + 1 + 1 + 4 )
typedef struct {
        byte    main,sub;
        byte    area;
} TPvFile;
typedef struct {
        byte MailBox;                         /* can be either MAIL_INBOX or MAIL_OUTBOX */
        byte New;                             /* Flag TRUE/FALSE */
    long DataSize;                        /* Lunghezza messagio in 'Data' (compreso 0x00 finale) */
    byte EntryID[MAIL_ENTRYID_SIZE + 1];  /* Identificatore univoco (da Outlook) + 0x00 */
    byte Data[MAIL_DATA_SIZE + 1];        /* Tutti i campi del messaggio (separati da 0xFE) + 0x00 */
} TMailRecord;

/******************************************************************************
*	[Title]         DataPVMailRead
*	[Arguments]	None
*	[ReturnVAL]	int : #number of unsent messages
*  Read mails from PVMail
******************************************************************************/
int DataPVMailRead(void)
{
  #define MAIL_FILENAME  "PVMailData"  /* Global file name for the storage Flash file */
  byte szMailFileName[15+1] = MAIL_FILENAME;
  TPvFile fMailFile;
  FILE_BUF mail_fb;
  int iNbMails, iBcl, iNbMailsFlash;
  FILE_INFO fi;
  TMailRecord MailRecord;

  /* 0 unsent mail for default value */
  iNbMails = 0;

  /* If we can reach the save flash file of PV MAIL */
  if (LibSubEntrySearch(szMailFileName,&(fMailFile.sub)) == TRUE)
  {
    /* Init reading of flash file */
    LibGetAllEntry(szMailFileName,&(fMailFile.main),&(fMailFile.sub));
    fMailFile.area = FILE_OPEN_MODE;
    mail_fb.fsb_main_entry_ = fMailFile.main;
    mail_fb.fsb_sub_entry_ = fMailFile.sub;
    mail_fb.fsb_scrt_info_ = fMailFile.area;

    /* Count total file records */
    iNbMailsFlash = LibGetFileCnt(&mail_fb);
    if (!iNbMailsFlash)
    {
      return(0);
    }

    /* Start from 1st record */
    fi.fp = 0xffff;
    fi.kind = FILE_KIND_BIN;

    /* Scan the rest of file and load record pointers */
    for (iBcl=0; iBcl <= iNbMailsFlash - 1; iBcl++)
    {
      /* If can't read , saw it ... */
      if (!LibFileFindNext(&mail_fb,&fi,0))
        LibPutMsgDlg((byte *) "Read PVMail 2.0.4 Mails failed");

      /* Read the current mail record */
      LibFileRead(&mail_fb,&fi);
      memcpy(&MailRecord, mail_fb.fbuf.bin.bin_buf, mail_fb.fbuf.bin.char_num);
      if (MailRecord.MailBox == 0x02 /* MAIL_OUTBOX*/)
      {
        iNbMails++;
      }
    }
  } /* LibSubEntrySearch ... */
  
  /* Quit with number of mail */
  return (iNbMails);
}

/******************************************************************************
*	[Title]         DataPVMailRead
*	[Arguments]	None
*	[ReturnVAL]	int : #number of unsent messages
*  Read mails from PV-750 mail addin
******************************************************************************/
int DataPV750MailRead(void)
{
  FILE_BUF fb;
  FILE_INFO finf;
  int iNbMails;

  /* 0 unsent mail for default value */
  iNbMails = 0;

  /* Get infos from tasks , and Common sub mode */
  fb.fsb_main_entry_ = 0x60;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xffff;  /* Search from top */
  finf.kind = FILE_KIND_BIN;
  fb.fsb_sub_entry_ = 0x00;

  /* Specify searching by date 0x10 in LibFileFindNext */
  while (LibFileFindNextExt(&fb,&finf,0x00) != FALSE)
  {
    if (LibFileRead(&fb,&finf)!=TRUE)
      LibPutMsgDlg((byte *) "Read PV750 Mails failed");
    else
    {
      /* Inc. nbre of mails */
      iNbMails++;
    }
  }
  
  /* Quit with number of mail */
  return (iNbMails);
}

