#ifndef FMH
#define FMH

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

#include <gdefs.h>

#include "mem.h"

/* This h-file provides access to a file structure */

/******************************************************************************
  Defines
******************************************************************************/

#define FILE_SYSTEM_ERROR    1

/* FILE_SYSTEM_ERRORs */
#define DATA_DELETE_FAILED        0x00
#define DATA_FOUND_NONE_EXPECTED  0x01
#define WRITE_ERROR               0x02
#define NO_DATA_FOUND             0x03
#define READ_ERROR                0x04
#define TREE_FLOW_ERROR           0x05
#define NO_FREE_ENTRYS            0x06
#define ENTRY_NOT_USED            0x07
#define NOT_USED_ENTRY_LINKED     0x08
/*
#define IB_SERR_INJUST       0xF0
#define IB_SERR_NOFILE       0xFB
#define IB_SERR_ALDYFL       0xFC
#define IB_SERR_NGSUBC       0xFD
#define IB_SERR_NOSUBC       0xFE
#define IB_SERR_FOPEN        0xFF

/* Filetype attributes */
#define FT_DIR           TRUE
#define FT_FILE          FALSE
#define FT_VIEWABLE      TRUE
#define FT_NOTVIEWABLE   FALSE    
#define FT_PROTECTED     TRUE
#define FT_NOTPROTECTED  FALSE
#define FT_ENTRYUSED     TRUE
#define FT_ENTRYNOTUSED  FALSE
#define FT_LASTENTRY     TRUE
#define FT_NOTLASTENTRY  FALSE
#define FT_NODEOPEN      TRUE
#define FT_NOTECLOSED    FALSE
#define FT_READ          TRUE
#define FT_NOTREAD       FALSE
#define FT_FIRSTENTRY    TRUE
#define FT_NOTFIRSTENTRY FALSE
/******************************************************************************
  Definition of STRUCTS
******************************************************************************/

typedef struct DATAFILEINFO{ /*This struct is placed at the beginning of the Info-File to 
                               access the Datafile */
  byte   m_entry;
  byte   s_entry;
  word   entryno;          /* number of entrys to read out of the Info-File 
                              includes unused entrys (pointer to the last entry)
                            */
}DATAFILEINFO;


typedef struct FILEINFO{ /* due to the size of this struct and the usage of only one LFILE_BUF
                            for info storrage, it is possible to store 1023 FILEINFO structs
                            (files or dirs) */
  byte   name[15+1];
  dword  date;          /* Jahr    0-4095       12 Bits
                           Monat   1-12          4 Bits
                           Tag     1-31          5 Bits
                           Stunde  0-23          5 Bits
                           Minute  0-59          6 Bits
                           Sekunde 0-99          7 Bits
                           
                           
                               0         1         2         3         4
                           Bit 01234567890123456789012345678901234567890
                               |   Jahr   ||  ||Tag||   ||Min.||     |
                                           Monat    Stunde     Sekunde
                        */
  dword  size;
  word   treesubentry;  /* for dirs only 1-1023(0=not used; points to the next tree(FILEINFO)
                           which contains the files for the subentry )*/
  word   preventry;     /* previous entry of the current tree */
  word   nextentry;     /* next member of the current tree */
  word   typeattr;      /* Type of the File(file or Dir) and attributes
                           Bit
                            0      0 = File    1 = Dir
                            1      File is viewable (a fitting procedure has been developed)
                                   this will be set by the receive procedure
                            2      file is protected, system password is needed to access file
                                   note that the DATA and INFO-Files are stored in an unprotected
                                   area
                            3      ENTRY USED(0=not used;1=used)
                            4      last entry of a chain
                            5      node open(inicates wheter a node(treesubentry,next-preventry)
                                   is shown on screen, or is not shown when it's closed)   
                            6      for e-Mail: 1 = read; 0 = not yet read
                            7      first entry of a chain
                            8-15   reserved(0)
                        */
  
  word   pointer;       /* FilePointer in the DATA-File */
}FILEINFO;

/******************************************************************************
  Definition of variables
******************************************************************************/

extern FILEINFO      far *fileinfo;
extern LFILE_BUF far finfofilebuf;
extern FILE_INFO     finfofileinfo;
extern LFILE_BUF far fdatafilebuf;                            /* FLASH data buffer (binaryBuffer 30720Byte)*/
extern FILE_INFO     fdatafileinfo;                           /* FLASH data information */
extern DATAFILEINFO  far *datafileinfo;

extern byte          m_entry_info;                            /* File Main Entry for INFO-File */
extern byte          s_entry_info;                            /* File Sub Entry for INFO-File */
extern byte          m_entry_data;
extern byte          s_entry_data;

extern byte          datafilename[15+1];                      /* Datei für Daten */
extern byte          infofilename[15+1];                      /* Zugriffsinformationen für Daten */
extern byte          far rootname[];

extern word startentry;
extern word openentrys; /* value used by every chain-run to add it's number of folders */
extern bool countshow;
extern byte ebene;
extern bool deleteeverythingfound;

/******************************************************************************
  Definition of the procedures
******************************************************************************/

/* returns the number of the last entry in a chain started with "entry"
  Parameters :
  
  word entry      this value can point to every entry and indicates 
                  an entry-point from where the scan should start
  
  Return values :
  word            this value points to the last entry in the chain 
                  started with "entry"
*/
word GetLastEntryofChain(word entry);

/* saves the whole Infofile 

  Parameters :

  byte *string    points to a zero-terminated character string
                  with a message that will be shown in a
                  dialog box (e.g. Saving . . . )
  bool disp       indicates wheter LibPutDisp should be called at the end or not
*/
void SaveInfo(byte *string, bool disp);


/* initializes pointer to the file-structures
*/
void Initfmdata(void);

/* initializes the file-system :
   the necessarry files are loaded or created if they don't exist
   the options for the filemanager will be initialized 
   if the files will be created
   
   - refers to Error-procedures
   needs the following additional defines :
     SRL_STAT      far *serial;
     word          fp_first_infofile;
*/
void InitFileSystem(void);

/* deletes entry and data in datafile(if folder)
  - refers to Error-procedures

  Parameters :
  
  word entry       entry that will be deleted
*/
void fmdelete(word entry);

/* creates a new folder in the first found unused entry
  Parameters ;

  byte *name       sz with the name (if length of string is 0 prpcedure aborts)
  word entry       entry-no of the folder in which the new folder should be created
*/
void fmNewFolder(byte *name,word entry);

/* NOT YET IMPLEMENTED */
void PerformFileInfoCheck(void);

/* sets filebuffer and fileinfo according to parameters

  Parameters :

  LFILE_BUF far *fbuf  pointer to file buffer
  FILE_INFO *fi        pointer to info buffer
  byte mentry          main entry values
  byte sentry          sub entry
  word size            value for fbuf->fbuf.bin.char_num
  word fp              fi->fp
*/
void FileBufInfSet(LFILE_BUF far *fbuf,FILE_INFO *fi,byte mentry,byte sentry, word size, word fp);

#endif