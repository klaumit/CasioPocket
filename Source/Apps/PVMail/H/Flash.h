/******************************************************************************
*
*       [Title]         Flash file operations
*               (Copied and changed from the Kino program,
*               by Wittawatt Wamyong)
*
*       [Model]         PocketViewer
*       [Version]       Ver1.00
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

#ifndef  FLASH_H
#define  FLASH_H

/*********** Macros ************/

/* Flash Files */
#define MAIL_FILENAME  "PVMailData"  /* Global file name for the storage Flash file */

#define FILE_NAME  "FileSaveData"    /* Global file name for the storage Flash file */
#define INDEX_NAME "FileSaveIndex"   /* Global File Name for the index Flash file */
#define BINBUF(fb)      ((fb).fbuf.bin.bin_buf)
#define BINBUF_SIZE(fb) ((fb).fbuf.bin.char_num)

#define ASSIGN_BUF(fb,f) do {                   \
        (fb).fsb_main_entry_ = (f).main;        \
        (fb).fsb_sub_entry_ = (f).sub;          \
        (fb).fsb_scrt_info_ = (f).area;         \
 } while (0)

#define FM_CREATE               1
#define FM_SECRET               2
#define FM_NEW                  (FM_CREATE | 4)

/* Serial & blocks */
#define MAX_SER_BUF    1024 /* maximum size for each serial reading */
#define MAX_RECORD     100  /* max records per file */
#define HEADER_LEN     256
#define BLOCK_SIZE     100
#define COMMENT_LEN    128
#define FILENAME_POS   0
#define FILENAME_LEN   128
#define FILESIZE_LEN   12
#define CHECKSUM_LEN   12

/* Mail record field sizes (used in allocating a TMailRecord variable).
   Even if theoretical max is 9600 (Flash record size),
   a 'Stack Overflow' occurs if a total size larger than about 3000 is used.
   Maybe some more optimization is needed. */
#define MAIL_RECORD_SIZE       3000
#define MAIL_ENTRYID_SIZE      200
#define MAIL_DATA_SIZE         MAIL_RECORD_SIZE - \
                               ( MAIL_ENTRYID_SIZE + 1 + \
                                 1 + 1 + 4 )
                        
#define MAIL_SEARCH_SIZE           100 /* Max buffer size used in search */             
                                 
#define SORT_LEN               5 /* Length of the string used to sort list ( 8 -> crashed! ) */                  
                               
#define NUMBUF_SIZE            6 + 1 /* Size of the buffer used to receive numbers */

/*********** Structs ************/
typedef struct {
        byte    main,sub;
        byte    area;
} TPvFile;

typedef struct {
        byte    FileName[FILENAME_LEN + 1];
        long    Size;
        int     nRecord;
} THeader;

/* Mail record structure (and its pointer cache) */
/* NOTE: Any change here change getRecordSize() also */
typedef struct {
        byte MailBox;                         /* can be either MAIL_INBOX or MAIL_OUTBOX */
        byte New;                             /* Flag TRUE/FALSE */
    long DataSize;                        /* Lunghezza messagio in 'Data' (compreso 0x00 finale) */
    byte EntryID[MAIL_ENTRYID_SIZE + 1];  /* Identificatore univoco (da Outlook) + 0x00 */
    byte Data[MAIL_DATA_SIZE + 1];        /* Tutti i campi del messaggio (separati da 0xFE) + 0x00 */
} TMailRecord;

/* Pointers to items of the 'Data' member of TmailRecord struct */
typedef struct {
    byte * Subject ;
    byte * From ;
    byte * To ;
    byte * Body ;
    byte * Date ;
    byte * Folder ;
    byte * CC ;
    byte * BCC ;  
} TMailMessageItems;

typedef struct {
        byte *EntryID;
        byte *New;
        byte *MailBox;
    byte *Data;
    long *DataSize;
        word fp;
} TCache;


/*********** Function Prototypes ************/
bool PvOpenFile(char *filename, TPvFile *f, int fmode);
bool saveMail(word fp);
bool DeleteMail(int idx);
bool initMailFile(void);
void collectMail(void);
void LoadMail(int idx);
int  SearchMail(int *idx, int mode);
word getRecordSize(TMailRecord *m);
void initRecord(TMailRecord *m);
/* bool CreateDummyRecords(void); **** ONLY FOR DEBUG ****/
void fatalError(char *msg);


#endif /* FLASH_H */
