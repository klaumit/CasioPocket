#include "gdefs.h"

/******************************************************************************
  Definition of PROCEDURES
******************************************************************************/

void Init(void);
void Display(void);
void InitTouch(void);
void InitData(void);
void InitFileSystem(void);
void PerformFileInfoCheck(void);
void Error(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3);
void MessageOK(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3);
void FileBufInfSet(LFILE_BUF far *fbuf, FILE_INFO *fi,
                   byte mentry, byte sentry, word size, word fp);
byte SetTypeAttr(bool filedir, bool viewable, bool protected, bool entryused, 
                 bool lastentry, bool nodeopen, bool read, bool firstentry);
void ShowTree(void);
void memfarcpy(const void far *source, void far *dest, word len);
void memfartonearcpy(const void far *source,void *dest,word len);
void memneartofarcpy(const void *source,void far *dest,word len);
void memfarfill(const void far *ptr,word len, byte value);
bool MessageBox(byte *string);
void RunChain(void);
byte filemenu(void);
void area1select(byte line);
void area2select(byte line);
void globalselect(bool area, byte line);
void filemenuaction(byte action);
word GetLastEntryofChain(word entry);
void ClearArea1();
void AdjustSelection(byte oldpos, byte newpos);
void PutSelection(byte xpos, byte ypos, byte xwidth, byte anz, byte selected);
word RedirectValues(word value,word *redirectvalues, byte anz);
void Options(void);
void miscmenuaction(byte action);
byte miscmenu(void);
void DrawOpPage();
void ExitSubTch(void);
void SaveInfo(byte *string, bool disp);
void InitOpPageTch();
/* void SelWindowOptionTypes(); */

/* DEBUG procedures */
void TestFile(void);
void TestFile1(void);
void RomCreationDate(void);
void PrintProStrSize();

/******************************************************************************
  Definition of const DATA (Icons, Touchtables, Messages)
******************************************************************************/

const byte far CornerIcon[] = {
	GSIZE(11, 11),
	0x1C, 0x1F,
	0x22, 0x1F,
	0x7F, 0xDF,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x7F, 0xDF,
};

const byte far Folder[] = {
	GSIZE(10, 10),
	0x1C, 0x3F,
	0x22, 0x3F,
	0x7F, 0xBF,
	0x80, 0x7F,
	0x80, 0x7F,
	0x80, 0x7F,
	0x80, 0x7F,
	0x80, 0x7F,
	0x80, 0x7F,
	0x7F, 0xBF,
};

const byte far MainFolder[] = {
	GSIZE(10, 10),
	0x1C, 0x3F,
	0x2A, 0x3F,
	0x7F, 0xBF,
	0xD5, 0x7F,
	0xAA, 0xFF,
	0xD5, 0x7F,
	0xAA, 0xFF,
	0xD5, 0x7F,
	0xAA, 0xFF,
	0x7F, 0xBF,
};

const byte far Plus[] = {
	GSIZE(7, 7),
	0x7D, 
	0x83, 
	0x93, 
	0xBB, 
	0x93, 
	0x83, 
	0x7D, 
};

const byte far Minus[] = {    
	GSIZE(7, 7),
	0x7D, 
	0x83, 
	0x83, 
	0xBB, 
	0x83, 
	0x83, 
	0x7D, 
};


TCHTBL far TchList[] = 
{
	/* Scroll Bar 1 */
	SCRBAR1X, SCRBAR1Y,SCRBAR1X+SCRBAR1SIZE-1,SCRBAR1Y+SCRBAR1SIZE-1,
	ACT_SCR_BAR,
	OBJ_SCRBAR1,
	0x0000,

	/* Scroll Bar 2 */
	SCRBAR2X, SCRBAR2Y,SCRBAR2X+SCRBAR2SIZE-1,SCRBAR2Y+SCRBAR2SIZE-1,
	ACT_SCR_BAR,
	OBJ_SCRBAR2,
	0x0000,

	/* oberes Fenster */
	2, 14, 146, 92,
	ACT_ICON,
	OBJ_AREA1,
	0x0000,

	/* unteres Fenster */
	2, 94, 146, 158,
	ACT_ICON,
	OBJ_AREA2,
	0x0000,

    /* reset Icon */
	0, 0, 10, 10,
	ACT_ICON,
	OBJ_RESETICON,
	0x0000,

    /* File menu */
    OBJ_FILEICONXORG, OBJ_FILEICONYORG, 
    OBJ_FILEICONXORG+OBJ_FILEICONXSIZE-1, OBJ_FILEICONYORG+OBJ_FILEICONYSIZE-1, 
    ACT_ICON,
    OBJ_FILEICON,
    0x0000,

    /* Misc menu */
    OBJ_MISCICONXORG, OBJ_MISCICONYORG, 
    OBJ_MISCICONXORG+OBJ_MISCICONXSIZE-1, OBJ_MISCICONYORG+OBJ_MISCICONYSIZE-1, 
    ACT_ICON,
    OBJ_MISCICON,
    0x0000,
    
    /* Light */     
    11, 0, 20, 10,
    ACT_ICON,
    OBJ_LIGHT,
    0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far fileicon = {&TchList[5], NULL, NULL, 0x02 };
T_ICON far miscicon = {&TchList[6], NULL, NULL, 0x02 };
T_ICON far reseticon = {&TchList[4], NULL, NULL, 0x03 };

TCHTBL far TchYESNO[] = 
  {
	/* YES */
	MB_YESXORG, MB_YESYORG, MB_YESXORG+MB_BUTTONXSIZE-1, MB_YESYORG+MB_BUTTONYSIZE-1,
	ACT_ICON,
	OBJ_BUTTON_YES,
	0x0000,

	/* NO */
	MB_NOXORG, MB_NOYORG, MB_NOXORG+MB_BUTTONXSIZE-1, MB_NOYORG+MB_BUTTONYSIZE-1,
	ACT_ICON,
	OBJ_BUTTON_NO,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

TCHTBL far TchOK[] = 
  {
	/* OK */
	MB_OKXORG, MB_OKYORG, MB_OKXORG+MB_BUTTONXSIZE-1, MB_OKYORG+MB_BUTTONYSIZE-1,
	ACT_ICON,
	OBJ_BUTTON_OK,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far MB_ICON_YES = {&TchYESNO[0], NULL, NULL, 0x02 };
T_ICON far MB_ICON_NO  = {&TchYESNO[1], NULL, NULL, 0x02 };

T_ICON far MB_ICON_OK  = {&TchOK[0], NULL, NULL, 0x02 };

TCHTBL far TchReceiveCancel[] = 
  {
	/* Receive */
	MB_BXORG+14-2, 
    MB_BYORG+MB_BYSIZE-18-2,
    MB_BXORG+51,
    MB_BYORG+MB_BYSIZE-9,
	ACT_ICON,
	OBJ_BUTTON_RCRECEIVE,
	0x0000,

	/* Cancel */
	MB_BXORG+MB_BXSIZE-45,
    MB_BYORG+MB_BYSIZE-20,
    MB_BXORG+MB_BXSIZE-11,
    MB_BYORG+MB_BYSIZE-9,
	ACT_ICON,
	OBJ_BUTTON_RCCANCEL,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};


T_ICON far RCRECEIVEICON = {&TchReceiveCancel[0], NULL, NULL, 0x02 };
T_ICON far RCCANCELICON = {&TchReceiveCancel[1], NULL, NULL, 0x02 };

TCHTBL far TchAboutOk[] = 
  {
	/* About Ok */
    OBJ_ABOUTOKXORG, 
    OBJ_ABOUTOKYORG, 
    OBJ_ABOUTOKXORG+OBJ_ABOUTOKXSIZE-1,
    OBJ_ABOUTOKYORG+OBJ_ABOUTOKYSIZE-1, 
	ACT_ICON,
	OBJ_ABOUTOKICON,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far aboutokicon = {&TchAboutOk[0], NULL, NULL, 0x02 };

TCHTBL far TchOptions[] = 
  {
	/* Combobox */
    OBJ_OPTCBXORG, 
    OBJ_OPTCBYORG, 
    OBJ_OPTCBXORG+OBJ_OPTCBXSIZE-1,
    OBJ_OPTCBYORG+OBJ_OPTCBYSIZE-1, 
	ACT_ICON,
	OBJ_OPTCB,
	0x0000,

	/* Ok */
    OBJ_OPTOKXORG, 
    OBJ_OPTOKYORG, 
    OBJ_OPTOKXORG+OBJ_OPTOKXSIZE-1,
    OBJ_OPTOKYORG+OBJ_OPTOKYSIZE-1, 
	ACT_ICON,
	OBJ_OPTOK,
	0x0000,

	/* Esc (Cancel) */
    OBJ_OPTCANCELXORG, 
    OBJ_OPTCANCELYORG, 
    OBJ_OPTCANCELXORG+OBJ_OPTCANCELXSIZE-1,
    OBJ_OPTCANCELYORG+OBJ_OPTCANCELYSIZE-1, 
	ACT_ICON,
	OBJ_OPTCANCEL,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far optcbicon = {&TchOptions[0], NULL, NULL, 0x03 };
T_ICON far optokicon = {&TchOptions[1], NULL, NULL, 0x02 };
T_ICON far optcancelicon = {&TchOptions[2], NULL, NULL, 0x02 };

TCHTBL far TchOptionsPage1[] = 
  {
	/* Combobox BPS */
    10,24,10+OBJ_OPTCBXSIZE-1,24+OBJ_OPTCBYSIZE-1,
	ACT_ICON,
	OBJ_OPTP1CB1,
	0x0000,

    /* Parity */
    10,53,10+40*3,53+9,
	ACT_ICON,
	OBJ_OPTP1Parity,
	0x0000,

    /* Data bit length */
    10,78,10+40*2,78+9,
	ACT_ICON,
	OBJ_OPTP1DBL,
	0x0000,
    
    /* Stop bit length */
    10,103,10+40*2,103+9,
	ACT_ICON,
	OBJ_OPTP1SBL,
	0x0000,

    /* Combobox Flow control */
    10,127,10+OBJ_OPTCBXSIZE-1,127+OBJ_OPTCBYSIZE-1,
	ACT_ICON,
	OBJ_OPTP1CB2,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far optp1cb1icon = {&TchOptionsPage1[0], NULL, NULL, 0x03 };
T_ICON far optp1cb2icon = {&TchOptionsPage1[4], NULL, NULL, 0x03 };


TCHTBL far TchOptionsPage2[] = 
  {

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

TCHTBL far TchOptionsPage3[] = 
  {

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};


byte errcodemessages[4][20+1] = {
  "UNKNOWN ERROR",
  "FILE SYSTEM ERROR",
  "SERIAL ERROR",
  "MEMORY ERROR",
};
/* max. lines 3!! */
byte errnomessages[3][16][60+1] = {
{
  "Illegal file name\r",
  "No sub entry\r",
  "Sub-Entry number is\routside of range\r",
  "Already used file\rname\r",
  "The file name is\rnot registered\r",
  "Illegal input\rcondition\r",
  "Faild to delete\rDATA-File\r",
  "UNKOWN ERROR\r",
  "Data found, but\rnone expected\r",
  "Can't write data\r(Memory full)\r",
  "No data found\r",
  "Can't read data\r(Filestructure unexpected)\r",
  "Treeflow error.\rThe tree-structure\rhas been destroyed.\r",
  "There are no more\rfree entrys.\r",
  "The entry is not\rused.\r",
  "The entry linked\ris not marked\ras used.\r",
}, {
  /* Serial error messages */
  "Can't open port\r",
}, {
  "malloc failed\r",
}
};

byte Message[3][75] = {
  "Datafile not found.\rRebuild Infofile?\rAll Information will be\rlost!\r",
  "All subdirectories\rwill be deleted\rincluding all files!.\r",
  "The Main folder\rcan't be deleted\r",
};

byte cb1string[3][21+1] = {
  "Serial options",
  "View options",
  "Registered file types",
};

word BITS[9] = {1,2,4,8,16,32,64,128,256};



byte flowcontrolstr[4][15+1] = {
  "No flow control",
  "XON/XOFF",
  "RS/CS(Hardware)",
  "XON/XOFF+RS/CS",
};

byte BPSstring[8][5+1] = {
  "00300",
  "00600",
  "01200",
  "02400",
  "04800",
  "09600",
  "19200",
  "38400",
};

byte MiscMenue[4][11+1] = {
  " Options ",
  " Item info ",
  " Info ",
  " About ",
};

byte receiveoptions[3][13+1] = {
  " 1. X/YMODEM ",
  " 2. Program ",
  " 3. DCC ",
};

word    parityredirect[3] = {IX_SRL_ODD, IX_SRL_EVEN, IX_SRL_NONE };
word    databitredirect[2] = {IX_SRL_7DATA, IX_SRL_8DATA };
word    stopbitredirect[2] = {IX_SRL_1STOP, IX_SRL_2STOP };

SRL_STAT      far *serial;

word          fp_first_infofile;

byte attstrings[8][3+1] = {
  "DR:",
  "VA:",
  "PR:",
  "EU:",
  "LE:",
  "NO:",
  "RE:",
  "FE:",
};

BK_SMPL_TCH BkCancel[] = {
  MB_BXORG+MB_BXSIZE-45,
  MB_BYORG+MB_BYSIZE-20,
  MB_BXORG+MB_BXSIZE-11,
  MB_BYORG+MB_BYSIZE-9,

  MB_BXORG+MB_BXSIZE-45,
  MB_BYORG+MB_BYSIZE-20,
  MB_BXORG+MB_BXSIZE-11,
  MB_BYORG+MB_BYSIZE-9,
};