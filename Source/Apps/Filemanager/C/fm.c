#include "fm.h"

FILEINFO      far *fileinfo;
LFILE_BUF far finfofilebuf;
FILE_INFO     finfofileinfo;
LFILE_BUF far fdatafilebuf;                            /* FLASH data buffer (binaryBuffer 30720Byte)*/
FILE_INFO     fdatafileinfo;                           /* FLASH data information */
DATAFILEINFO  far *datafileinfo;

byte          m_entry_info;                            /* File Main Entry for INFO-File */
byte          s_entry_info;                            /* File Sub Entry for INFO-File */
byte          m_entry_data;
byte          s_entry_data;

byte          datafilename[15+1] = "FM-Datafile";      /* Datei für Daten */
byte          infofilename[15+1] = "FM-Infofile";      /* Zugriffsinformationen für Daten */
byte          far rootname[] = {"Main"};

word startentry;
word openentrys; /* value used by every chain-run to add it's number of folders */
bool countshow;
byte ebene;
bool deleteeverythingfound;


word GetLastEntryofChain(word entry)
{
  while ((fileinfo[entry].typeattr & BITS[4]) == 0) {
    entry = fileinfo[entry].nextentry;
  }
  return(entry);
}

void SaveInfo(byte *string, bool disp)
{
  LibOpenWindow(0,0,160,160);
  LibPutFarData(15,54,107);
  LibPutProStr(IB_PFONT3,(160-LibGetProStrSize(IB_PFONT3, string))/2,74 , string, LibGetProStrSize(IB_PFONT3, string));
  LibPutDisp();

  /* set fp to first data in info file */
  FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info, 
                0, 0xffff);
  if (LibLFileFindNext(&finfofilebuf,&finfofileinfo,0x00)==FALSE) 
    Error(FILE_SYSTEM_ERROR, NO_DATA_FOUND,m_entry_info,s_entry_info, finfofileinfo.fp);

  /* Alles ins info-file schreiben (alle entrys)*/
  FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info,
                1024+sizeof(DATAFILEINFO)+sizeof(FILEINFO)*datafileinfo->entryno, finfofileinfo.fp);
  if (LibLFileWrite(&finfofilebuf, &finfofileinfo)==FALSE)
    Error(FILE_SYSTEM_ERROR, WRITE_ERROR, finfofilebuf.fsb_main_entry_,
          finfofilebuf.fsb_sub_entry_, finfofileinfo.fp);      
  LibCloseWindow();
  if (disp) LibPutDisp();
}

void Initfmdata(void)
{
  datafileinfo = ((DATAFILEINFO far*)&finfofilebuf.fbuf.bin.bin_buf[1024]);
  fileinfo = ((FILEINFO far*)&finfofilebuf.fbuf.bin.bin_buf[1028]);
}

void InitFileSystem()
{
  /* Try to register necessary files */
  if(LibSubEntrySave(infofilename,&m_entry_info)==FALSE) Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,1);    
  if (SubEntryStat==IB_SERR_RNEW) /* INFO-File have to be initialized and
                                       DATA File have to be created or rebuild */
    { 
      LibGetAllEntry(infofilename,&m_entry_info,&s_entry_info); /* Main/Sub Entry Get */  
      /* Create Datafile */
      if(LibSubEntrySave(datafilename,&m_entry_data)==FALSE) Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,2);
      if(SubEntryStat==IB_SERR_RALDY) /* DATA File have to be a new one if the 
                                         INFO-File is rebuild */
        {
          if (LibSubEntryDel(datafilename, &m_entry_data)==FALSE) 
            Error(FILE_SYSTEM_ERROR, DATA_DELETE_FAILED,0,0,3);
          if(LibSubEntrySave(datafilename,&m_entry_data)==FALSE) /* Create new DATA-File */
            Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,4);
          LibGetAllEntry(datafilename, &m_entry_data, &s_entry_data);
        }
      else
        LibGetAllEntry(datafilename, &m_entry_data, &s_entry_data);
      /* Init data for the Info File (entrys to Data file) */
      datafileinfo[0].m_entry = m_entry_data;
      datafileinfo[0].s_entry = s_entry_data;
      datafileinfo[0].entryno = 1;
      /* Root entry */
       /* Declaration of fileinfo :
             fileinfo = ((FILEINFO far*)&finfofilebuf.fbuf.bin.bin_buf)+1028;  */
      memfarcpy(rootname ,fileinfo[0].name, 5); /* pointer to buffer */
/*      memfartonearcpy(fileinfo[0].name, name, 5);
      /* "Main" shows up */
/*      memfartonearcpy(((byte far*)finfofilebuf.fbuf.bin.bin_buf+1028), name, 5);
      /* Nothing shows up */     
/*      memfartonearcpy(((byte far*)&finfofilebuf.fbuf.bin.bin_buf+1028), name, 5);
      /* Nothing shows up 

        So far:
         -memfarcpy and memfartonearcpy work 
         -data written to fileinfo[0].name can be read again 
         -but it's not the same as:
           ((byte far*)finfofilebuf.fbuf.bin.bin_buf+1028)    OR
           ((byte far*)&finfofilebuf.fbuf.bin.bin_buf+1028)
       */
/*      memfartonearcpy(((FILEINFO far*)&finfofilebuf.fbuf.bin.bin_buf[1028]), name, 5);
       "Main" shows up /**/
      
/*      LibClrDisp();
      LibPutProStr(IB_PFONT1, 0, 0, name, 100);
      LibPutDisp(); /**/
      /* info from the beginning */
      serial[0].port           = IB_SRL_COM2;
      serial[0].speed          = IB_SRL_38400BPS;
      serial[0].parit          = IX_SRL_NONE;
      serial[0].datab          = IX_SRL_7DATA;
      serial[0].stopb          = IX_SRL_1STOP;
      serial[0].fctrl          = IX_SRL_XONOFF;
      /* first info after the 1KB */
      fileinfo[0].size         = 0;
      fileinfo[0].treesubentry = 0;
      fileinfo[0].preventry    = 0;
      fileinfo[0].nextentry    = 0;
      fileinfo[0].typeattr     = 
        SetTypeAttr(FT_DIR, FT_NOTVIEWABLE, FT_NOTPROTECTED, FT_ENTRYUSED, FT_LASTENTRY,FT_NODEOPEN, FT_READ, FT_FIRSTENTRY);
      fileinfo[0].pointer      = 0;

/*      for(i=0;i<1024;i++)
        finfofilebuf.fbuf.bin.bin_buf[i] = i % 256;
      
      /* Save initial information */
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info, 
                    0, 0xffff);
                     
      /* search for first free data */
      if (LibLFileFindNext(&finfofilebuf,&finfofileinfo,0x00)==TRUE) 
        Error(FILE_SYSTEM_ERROR, DATA_FOUND_NONE_EXPECTED,m_entry_info,s_entry_info, finfofileinfo.fp);
      fp_first_infofile = finfofileinfo.fp;
      
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info,
                    1024+sizeof(FILEINFO)+sizeof(DATAFILEINFO), finfofileinfo.fp);

      if (LibLFileWrite(&finfofilebuf, &finfofileinfo)==FALSE)
        Error(FILE_SYSTEM_ERROR, WRITE_ERROR, finfofilebuf.fsb_main_entry_,
                                              finfofilebuf.fsb_sub_entry_, finfofileinfo.fp);
    } /* Info-File hat mind. den Rooteintrag und das Data-File existiert und ist "geöffnet" */
  else /* Info-File auslesen und Data-File initialisieren */
    {
      LibGetAllEntry(infofilename,&m_entry_info,&s_entry_info); /* Main/Sub Entry Get */
      /* Data-file öffnen */
      if(LibSubEntrySave(datafilename,&m_entry_data)==FALSE) 
        Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,5);
      
      /* set fp to first data in info file */
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info, 
                    0, 0xffff);
      if (LibLFileFindNext(&finfofilebuf,&finfofileinfo,0x00)==FALSE) 
        Error(FILE_SYSTEM_ERROR, NO_DATA_FOUND,m_entry_info,s_entry_info, finfofileinfo.fp);
      fp_first_infofile = finfofileinfo.fp;
      
      /* DATAFILEINFO aus info-file auslesen */
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info,
                    1024+sizeof(DATAFILEINFO), finfofileinfo.fp);
      if (LibLFileRead(&finfofilebuf, &finfofileinfo)==FALSE)
        Error(FILE_SYSTEM_ERROR, READ_ERROR, finfofilebuf.fsb_main_entry_,
                                             finfofilebuf.fsb_sub_entry_, finfofileinfo.fp);


      /* READ ALL */

      /* set fp to first data in info file */
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info, 
                    0, 0xffff);
      if (LibLFileFindNext(&finfofilebuf,&finfofileinfo,0x00)==FALSE) 
        Error(FILE_SYSTEM_ERROR, NO_DATA_FOUND,m_entry_info,s_entry_info, finfofileinfo.fp);

      /* Alles aus info-file auslesen (alle entrys)*/
      FileBufInfSet(&finfofilebuf, &finfofileinfo, m_entry_info, s_entry_info,
                    1024+sizeof(DATAFILEINFO)+sizeof(FILEINFO)*datafileinfo->entryno, finfofileinfo.fp);


      if (LibLFileRead(&finfofilebuf, &finfofileinfo)==FALSE)
        Error(FILE_SYSTEM_ERROR, READ_ERROR, finfofilebuf.fsb_main_entry_,
                                             finfofilebuf.fsb_sub_entry_, finfofileinfo.fp);
      
      /* Check if Data-File exist's */
      if (LibSubEntrySave(datafilename, &s_entry_data)==FALSE) {
        Error(FILE_SYSTEM_ERROR, SubEntryStat, 0, 0, 6);
      }
      if (SubEntryStat==IB_SERR_RNEW) { /* Data-File have been deleted */
        LibGetAllEntry(datafilename, &m_entry_data, &s_entry_data);
        /* Update Info-File information */
        if (MessageBox(Message[0])) {
          /* Clear complete buffer and rebuild it */          
          /* the fill marks every entry as unused */
          memfarfill(fileinfo[0].name,30720-1024, 0);
          datafileinfo[0].m_entry = m_entry_data;
          datafileinfo[0].s_entry = s_entry_data;
          datafileinfo[0].entryno = 1;
          memfarcpy(rootname ,fileinfo[0].name, 5); /* pointer to buffer */
          fileinfo[0].size         = 0;
          fileinfo[0].treesubentry = 0;
          fileinfo[0].preventry    = 0;
          fileinfo[0].nextentry    = 0;
          fileinfo[0].typeattr     = 
            SetTypeAttr(FT_DIR, FT_VIEWABLE, FT_NOTPROTECTED, FT_ENTRYUSED, FT_LASTENTRY,FT_NODEOPEN, FT_READ, FT_FIRSTENTRY);
          fileinfo[0].pointer      = 0;
      
        }
        
        
      }
        
      /* Check if entry's just read match real Data-File entry's */
      
      /* Set Data-file entry according to info read from info-file */
      m_entry_data = finfofilebuf.fsb_main_entry_;
      s_entry_data = finfofilebuf.fsb_sub_entry_;
      
  /*    memfartonearcpy(fileinfo[0].name, name, 5); 

      LibClrDisp();
      LibPutProStr(IB_PFONT1, 0, 0, name, 100);
      name[5] = 0;
      LibNumToStr(name, m_entry_data, 4);
      LibPutProStr(IB_PFONT1, 0, 10, name, 100);
      LibNumToStr(name, s_entry_data, 4);
      LibPutProStr(IB_PFONT1, 0, 20, name, 100);
      for (i=0;i<13;i++) {
        LibNumToStr(name, finfofilebuf.fbuf.bin.bin_buf[i], 4);
        LibPutProStr(IB_PFONT1, 0, 30+i*10, name, 100);
      }
      LibPutDisp();
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT); /**/

    }
}

void fmdelete(word entry)
{
  if (entry == 0) {
    MessageBox(Message[2]);
    return;
  }
  if (!MessageBox(Message[1])) return;

  /* mark entry as unused */
  if ((fileinfo[entry].typeattr & BITS[3]) == BITS[3]) 
    fileinfo[entry].typeattr -= BITS[3];
  else
    Error(FILE_SYSTEM_ERROR, ENTRY_NOT_USED,entry,0,0);
  /* if DIR : if theres a sub chain, delete all sub entrys */
  if ((fileinfo[entry].typeattr & BITS[0]) == BITS[0]) {
    /* Subentry? */
    if (fileinfo[entry].treesubentry != 0) {
      countshow  = COUNT;
      startentry = fileinfo[entry].treesubentry;
      deleteeverythingfound = TRUE;
      RunChain();
    }
  }
  else { /* file */
    /* delete data in data file */
    /* ************************************************************************************
       ************************************************************************************
       ************************************************************************************ */
  }
  /* last entry of chain ? (only last entry not first and last) */
  if (((fileinfo[entry].typeattr & BITS[4]) == BITS[4]) && 
       (fileinfo[entry].typeattr & BITS[7]) != BITS[7]) {
    /* last entry */
    /* set previous entry to last entry */
    fileinfo[fileinfo[entry].preventry].typeattr += BITS[4];
  }
  else {
    /* not last entry */
    
    if ((fileinfo[entry].typeattr & BITS[7]) == BITS[7]) {
      /* its the first entry of a sub-chain */

      if ((fileinfo[entry].typeattr & BITS[4]) == BITS[4]) {
       /* set the new treesubentry */
       /* it's the first entry and the last entry so treesubentry of preventry have to be 0 */
       fileinfo[fileinfo[entry].preventry].treesubentry = 0;
      }
      else { /* it's only the first entry, so a nextentry does exist's */
        /* set the new treesubentry */
        fileinfo[fileinfo[entry].preventry].treesubentry = fileinfo[entry].nextentry;
        /* set the new first entry */
        fileinfo[fileinfo[entry].nextentry].typeattr += BITS[7];
      }
    }
    else {
     /* set nextentry of previous entry to nextentry of the deletet entry */
      fileinfo[fileinfo[entry].preventry].nextentry = 
        fileinfo[entry].nextentry;
    }
  }
}


void fmNewFolder(byte *name,word entry)
{
  word  i, useentry;

  if (strlen(name) > 0) {
    /* Search for first unused entry */
    useentry = 0;
    for (i=0;i<=1023;i++) /* Scan in all entrys */
      if ((fileinfo[i].typeattr & BITS[3]) == 0) { /* Entry free */
        useentry = i;
        break;
       }
    if (useentry == 0) Error(FILE_SYSTEM_ERROR, NO_FREE_ENTRYS, 0, 0, 0);
    /* set new top entry (datafileinfo->entryno) if it is one */
    if (useentry > datafileinfo[0].entryno-1) datafileinfo[0].entryno = useentry+1;
      
    /* Set up info */
      memneartofarcpy(name, fileinfo[useentry].name, 16);
      fileinfo[useentry].size = 0;
      fileinfo[useentry].treesubentry = 0;
      fileinfo[useentry].nextentry = 0;
      fileinfo[useentry].pointer = 0;
      /* no subentry there: create new one */
          
      /* area12line is used in assumption that this part is only called if area12 is set
         to area1 because there are only folders */

      if (fileinfo[entry].treesubentry == 0) {
/*        fileinfo[useentry].preventry = GetLastEntryofChain(entry); */
        fileinfo[useentry].preventry = entry;
        fileinfo[entry].treesubentry = useentry;
        /* it's the first entry */
        fileinfo[useentry].typeattr =
          SetTypeAttr(FT_DIR, FT_NOTVIEWABLE, FT_NOTPROTECTED, FT_ENTRYUSED, FT_LASTENTRY,FT_NODEOPEN, FT_READ, FT_FIRSTENTRY);

      }
      else { 
        fileinfo[useentry].preventry = 
         /* Run chain of treesubentry (currently selected) */
         GetLastEntryofChain(fileinfo[entry].treesubentry);
       /* Set new end of chain BIT[4] (clear bit 4 of prev entry) 
          the new entry is set as lastentry */
       if ((fileinfo[fileinfo[useentry].preventry].typeattr & BITS[4]) == BITS[4])
         fileinfo[fileinfo[useentry].preventry].typeattr -= BITS[4];
       /* set the nextentry property */
       fileinfo[fileinfo[useentry].preventry].nextentry = useentry;
      fileinfo[useentry].typeattr =
        SetTypeAttr(FT_DIR, FT_NOTVIEWABLE, FT_NOTPROTECTED, FT_ENTRYUSED, FT_LASTENTRY,FT_NODEOPEN, FT_READ, FT_NOTFIRSTENTRY);
      }
  }
}

void PerformFileInfoCheck(void)
{ 
/* Possible errors :
   -Dirs have a size of 0
   -Dirs are not viewable with any procedure
   -Dirs's Pointer is 0
   -Files's treesubentry is unused (0)
   -the pointer of dirs is 0
   -A dir with the name "Root" have to exist 
   -a chain can't contain a unused entry(FILEINFO)
   -no cross linked entry's are allowed
   -data reffered by size and pointer must be valid 
   
*/
}

void FileBufInfSet(LFILE_BUF far *fbuf,FILE_INFO *fi,byte mentry,byte sentry, word size, word fp)
{
  fbuf->fsb_main_entry_ = mentry;
  fbuf->fsb_sub_entry_  = sentry;
  fbuf->fsb_scrt_info_  = FILE_OPEN_MODE;

  fi->kind = FILE_KIND_BIN;
  fi->fp   = fp;

  fbuf->fbuf.bin.char_num = size;
}