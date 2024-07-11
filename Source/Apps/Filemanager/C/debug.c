#include "debug.h"

TCHTBL far TchPPSS[] = 
    {
	  /* Input */
	  0,0,10,10,
	  ACT_ICON,
      0x8000,
      0x0000,

      /* END */
      0, 0, 0, 0,
      ACT_NONE,
      OBJ_END,
      0x0000
    };


void PrintProStrSize()
{
  byte name[30+1];
  byte number[5+1];
  word size;
  TCHSTS tsts;
  
  LibClrDisp();
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);  
  LibTchStackPush(TchPPSS);
  LibTchInit();
  while(TRUE) {
    LibTchWait(&tsts);
    switch(tsts.obj) {
      case 0x8000 :
        Input(name,  30, "", "");
        size = LibGetProStrSize(IB_PFONT1, name);
        LibNumToStr(number, size, 5);
        number[5] = 0;
        LibPutProStr(0,0,IB_PFONT1, number, 50);
        LibPutDisp();
      break;
    }
  }
}

void ROMCreationDate(void)
{
  byte version[12+1];

  LibClrDisp();
  LibGetVersion(version);
  LibPutProStr(IB_PFONT1, 0, 0, version, 160);

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
  LibWait(IB_1SWAIT);
  
}

void TestFile1(void)
{
	bool    f_handle;
    byte name[30+1];  

	if(LibSubEntrySave(name,&s_entry_info)==FALSE){			/* Filename SAVE */
		m_entry_info = 0xff;
		s_entry_info = 0xff;
        LibJumpMenu();
	}
	LibGetAllEntry(name,&m_entry_info,&s_entry_info);	/* Main/Sub Entry Get */
	
	


	finfofilebuf.fsb_main_entry_ = m_entry_info;			/* Main */
	finfofilebuf.fsb_sub_entry_  = s_entry_info;			/* Sub */
	finfofilebuf.fsb_scrt_info_  = 0x80;				/* Open Mode */

	finfofileinfo.fp      = 0xffff;					/* first Data */
	finfofileinfo.kind    = FILE_KIND_BIN;			/* Bin */

	if(LibLFileFindNext(&finfofilebuf,&finfofileinfo,0x00)==FALSE){
		finfofileinfo.fp  = 0xffff;					/* first Data */
	}


	finfofilebuf.fsb_main_entry_ = m_entry_info;			/* Main */
	finfofilebuf.fsb_sub_entry_  = s_entry_info;			/* Sub */
	finfofilebuf.fsb_scrt_info_  = 0x80;				/* Open Mode */

	finfofileinfo.kind    = FILE_KIND_BIN;			/* Bin */

	finfofilebuf.fbuf.bin.char_num = 256;

	f_handle = LibLFileWrite(&finfofilebuf, &finfofileinfo);	/* Save */
	if(f_handle == FALSE){
		LibErrorDisp(FlashStatus);				/* Error Disp */
		LibJumpMenu();
	}
  LibJumpMenu();
}

void TestFile(void)
{
  word i;
  bool correct = TRUE;
  bool f_handel;  
  byte name[30+1];
  LibClrDisp();  
  /* Create File */
  if (LibSubEntrySave(name, &s_entry_info) == FALSE) Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,0);
  if (LibGetAllEntry(name, &m_entry_info, &s_entry_info)== FALSE) Error(FILE_SYSTEM_ERROR, SubEntryStat,0,0,0);


  finfofilebuf.fsb_main_entry_ = m_entry_info;
  finfofilebuf.fsb_sub_entry_  = s_entry_info; 
  finfofilebuf.fsb_scrt_info_  = FILE_OPEN_MODE;
/*  finfofilebuf.fsb_todo_chek_  = 0xFF;
  finfofilebuf.fsb_todo_rank_  = 0xFF;
  finfofilebuf.fsb_ararm_chk_  = 0x00; */
  finfofileinfo.fp   = 0xffff;
  finfofileinfo.kind = FILE_KIND_BIN;

  if (LibLFileFindNext(&finfofilebuf, &finfofileinfo, 0x00) == FALSE)
    finfofileinfo.fp = 0xffff;

  finfofilebuf.fsb_main_entry_ = m_entry_info;
  finfofilebuf.fsb_sub_entry_  = s_entry_info; 
  finfofilebuf.fsb_scrt_info_  = FILE_OPEN_MODE;
  finfofileinfo.kind = FILE_KIND_BIN;

  for (i=0;i<256;i++)   
    finfofilebuf.fbuf.bin.bin_buf[i] = i; 
  finfofilebuf.fbuf.bin.char_num = 256;

  f_handel = LibLFileWrite(&finfofilebuf, &finfofileinfo);

  if (f_handel == FALSE) {
/*    LibErrorDisp(FlashStatus);
    LibPutDisp();
    LibWait(IB_1SWAIT);
    LibWait(IB_1SWAIT);
    LibWait(IB_1SWAIT);
    LibWait(IB_1SWAIT);
    LibWait(IB_1SWAIT);
    LibWait(IB_1SWAIT); /**/
    Error(FILE_SYSTEM_ERROR, WRITE_ERROR,m_entry_info,s_entry_info, FlashStatus);
  }

  for (i=0;i<256;i++)
    finfofilebuf.fbuf.bin.bin_buf[i] = 0;

  correct = TRUE;  
  for(i=0;i<256;i++) if (finfofilebuf.fbuf.bin.bin_buf[i] != i) correct = FALSE;
  if (correct)
    LibPutProStr(IB_PFONT1, 0, 0, "1. OK", 160);
  else
    LibPutProStr(IB_PFONT1, 0, 0, "1. FALSE", 160);


  finfofilebuf.fbuf.bin.char_num = 256;
  finfofilebuf.fsb_main_entry_   = m_entry_info;
  finfofilebuf.fsb_sub_entry_    = s_entry_info; 
  finfofilebuf.fsb_scrt_info_    = FILE_OPEN_MODE;
  finfofileinfo.fp   = 0xffff;
  finfofileinfo.kind = FILE_KIND_BIN;

  if (LibLFileFindNext(&finfofilebuf, &finfofileinfo, 0x00) == FALSE)
    finfofileinfo.fp = 0xffff;

  finfofilebuf.fsb_main_entry_ = m_entry_info;
  finfofilebuf.fsb_sub_entry_  = s_entry_info; 
  finfofilebuf.fsb_scrt_info_  = FILE_OPEN_MODE;
  finfofileinfo.kind = FILE_KIND_BIN;
  
  LibLFileRead(&finfofilebuf, &finfofileinfo);

  correct = TRUE;  
  for(i=0;i<256;i++) if (finfofilebuf.fbuf.bin.bin_buf[i] != i) correct = FALSE;
  if (correct)
    LibPutProStr(IB_PFONT1, 0, 10, "Final OK", 160);
  else
    LibPutProStr(IB_PFONT1, 0, 10, "Final FALSE", 160);

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
  LibWait(IB_1SWAIT);
  LibJumpMenu();
}