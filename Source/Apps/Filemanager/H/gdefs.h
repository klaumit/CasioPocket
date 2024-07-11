#ifndef GDEFSH
#define GDEFSH

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

/******************************************************************************
  Definition of PROCEDURES
******************************************************************************/
extern void Init(void);
extern void Display(void);
extern void InitTouch(void);
extern void InitData(void);
extern void InitFileSystem(void);
extern void PerformFileInfoCheck(void);
extern void Error(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3);
extern void MessageOK(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3);
extern byte SetTypeAttr(bool filedir, bool viewable, bool protected, bool entryused, 
                        bool lastentry, bool nodeopen, bool read, bool firstentry);
extern void ShowTree(void);
extern bool MessageBox(byte *string);
extern void RunChain(void);
extern byte filemenu(void);
extern void area1select(byte line);
extern void area2select(byte line);
extern void globalselect(bool area, byte line);
extern void filemenuaction(byte action);
extern void ClearArea1();
extern void AdjustSelection(byte oldpos, byte newpos);
extern void Options(void);
extern void miscmenuaction(byte action);
extern void DrawOpPage();
extern void ExitSubTch(void);
extern void SaveInfo(byte *string, bool disp);
extern void InitOpPageTch();

/******************************************************************************
  Definition of DEFINES
******************************************************************************/
/* Standart dialog boxes */

/* small */
#define OBJ_STDDLGBOXSXSIZE  128
#define OBJ_STDDLGBOXSYSIZE  50
#define OBJ_STDDLGBOXSXORG   (160-OBJ_STDDLGBOXSXSIZE)/2
#define OBJ_STDDLGBOXSYORG   (160-OBJ_STDDLGBOXSYSIZE)/2
/* middle */
#define OBJ_STDDLGBOXMXSIZE  128
#define OBJ_STDDLGBOXMYSIZE  62
#define OBJ_STDDLGBOXMXORG   (160-OBJ_STDDLGBOXMXSIZE)/2
#define OBJ_STDDLGBOXMYORG   (160-OBJ_STDDLGBOXMYSIZE)/2
/* Large */
#define OBJ_STDDLGBOXLXSIZE  128
#define OBJ_STDDLGBOXLYSIZE  76
#define OBJ_STDDLGBOXLXORG   (160-OBJ_STDDLGBOXLXSIZE)/2
#define OBJ_STDDLGBOXLYORG   (160-OBJ_STDDLGBOXLYSIZE)/2


#define SCRBAR1X      149
#define SCRBAR1Y      16
#define SCRBAR1SIZE   75
 
#define SCRBAR2X      149
#define SCRBAR2Y      96
#define SCRBAR2SIZE   61

/* defines for messagebox */
#define  MB_BUTTONXSIZE   30
#define  MB_BUTTONYSIZE   12
#define  MB_BXSIZE        128
#define  MB_BYSIZE        76
#define  MB_BEDGE         4
#define  MB_BXORG         (160-MB_BXSIZE)/2
#define  MB_BYORG         (160-MB_BYSIZE)/2
#define  MB_YESXORG       MB_BXORG+MB_BEDGE+10
#define  MB_YESYORG       MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1
#define  MB_NOXORG        MB_BXORG+MB_BXSIZE-MB_BEDGE-MB_BUTTONXSIZE-10
#define  MB_NOYORG        MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1

#define  MB_OKXORG        (160-MB_BUTTONXSIZE)/2
#define  MB_OKYORG        MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1


/* Folder defines */
#define FLD_XORG     2            /* Origin to draw the first Foldericon (AREA1) */
#define FLD_YORG     15           
#define FLD_SQRSIZE  11           /* Size of a virtual grid to which the FilderIcons are aligned */
#define FLD_PMXORG   1            /* Plus and Minus-Icons's origin within a virtual grid (11x11) */
#define FLD_PMYORG   2
#define FLD_NAMEXORG 1            /* Origin of Foldernames within the (first used) grid */
#define FLD_NAMEYORG 1
#define FLD_MAXXGRID 13           /* Value of folders that fit on the screen in horzontal
                                     direction. This value depends on the FLD_SQRSIZE.
                                     The maximum of this value in horizontal direction should 
                                     never be used because there won't fit any foldernames on
                                     the screen.
                                     The value is used to calculate the length of names to be
                                     shown on the screen */
#define FLD_MAXYGRID 7            /* Value of folders that fit on the screen in vertical 
                                     direction. This value depends on the FLD_SQRSIZE. */
#define FLD_XLINE    4            /* used to draw lines(pixels) that connect the icons */
#define FLD_YLINE    5


#define OBJ_FILEICONXORG     20
#define OBJ_FILEICONYORG     0
#define OBJ_FILEICONXSIZE    30
#define OBJ_FILEICONYSIZE    12

#define OBJ_MISCICONXORG     OBJ_FILEICONXORG+OBJ_FILEICONXSIZE+3
#define OBJ_MISCICONYORG     OBJ_FILEICONYORG
#define OBJ_MISCICONXSIZE    30
#define OBJ_MISCICONYSIZE    OBJ_FILEICONYSIZE

#define OBJ_ABOUTOKXSIZE     30
#define OBJ_ABOUTOKYSIZE     12
#define OBJ_ABOUTOKXORG      (160-OBJ_ABOUTOKXSIZE)/2 
#define OBJ_ABOUTOKYORG      OBJ_STDDLGBOXLYORG+OBJ_STDDLGBOXLYSIZE-OBJ_ABOUTOKYSIZE-4-2

/* Defs for options */
#define OBJ_OPTCANCELXSIZE   OBJ_FILEICONXSIZE
#define OBJ_OPTCANCELYSIZE   OBJ_FILEICONYSIZE
#define OBJ_OPTCANCELXORG    160-OBJ_OPTCANCELXSIZE-2
#define OBJ_OPTCANCELYORG    160-OBJ_OPTCANCELYSIZE-2

#define OBJ_OPTOKXSIZE       OBJ_FILEICONXSIZE
#define OBJ_OPTOKYSIZE       OBJ_FILEICONYSIZE
#define OBJ_OPTOKXORG        OBJ_OPTCANCELXORG-OBJ_OPTOKXSIZE-2
#define OBJ_OPTOKYORG        OBJ_OPTCANCELYORG

#define OBJ_OPTCBXORG        0
#define OBJ_OPTCBYORG        0
#define OBJ_OPTCBXSIZE       105
#define OBJ_OPTCBYSIZE       12

/* Objekte */
#define OBJ_SCRBAR1          0x8000
#define OBJ_SCRBAR2          0x8001
#define OBJ_AREA1            0x8002
#define OBJ_AREA2            0x8003
#define OBJ_RESETICON        0x8004
#define OBJ_BUTTON_YES       0x8005
#define OBJ_BUTTON_NO        0x8006
#define OBJ_BUTTON_OK        0x8007
#define OBJ_FILEICON         0x8008
#define OBJ_LIGHT            0x8009
#define OBJ_BUTTON_CANCEL    0x800A
#define OBJ_BUTTON_RCRECEIVE 0x800B
#define OBJ_BUTTON_RCCANCEL  0x800C
#define OBJ_MISCICON         0x800D
#define OBJ_ABOUTOKICON      0x800E
#define OBJ_OPTOK            0x800F
#define OBJ_OPTCANCEL        0x8010
#define OBJ_OPTCB            0x8011
#define OBJ_OPTP1CB1         0x8012 /* Options Page 1 Combobox 1 BPS */
#define OBJ_OPTP1CB2         0x8013 /* Options Page 1 Combobox 2 Flow control*/
#define OBJ_OPTP1Parity      0x8014
#define OBJ_OPTP1DBL         0x8015 /* Options Page 1 Data bit length */
#define OBJ_OPTP1SBL         0x8016 /* Options Page 1 Stop bit length */


/* Error codes */
#define UNKNOWN_ERROR        0
/*#define FILE_SYSTEM_ERROR    1*/
#define SERIAL_ERROR         2
#define MEMORY_ERROR         3

/* FILE_SYSTEM_ERRORs */
/*#define DATA_DELETE_FAILED        0x00
#define DATA_FOUND_NONE_EXPECTED  0x01
#define WRITE_ERROR               0x02
#define NO_DATA_FOUND             0x03
#define READ_ERROR                0x04
#define TREE_FLOW_ERROR           0x05
#define NO_FREE_ENTRYS            0x06
#define ENTRY_NOT_USED            0x07
#define NOT_USED_ENTRY_LINKED     0x08*/
/*
#define IB_SERR_INJUST       0xF0
#define IB_SERR_NOFILE       0xFB
#define IB_SERR_ALDYFL       0xFC
#define IB_SERR_NGSUBC       0xFD
#define IB_SERR_NOSUBC       0xFE
#define IB_SERR_FOPEN        0xFF
*/
/* Serial Errors */
#define SR_CANT_OPEN_PORT         0x00

/* MEMORY_ERRORS */
#define MALLOC_FAILED             0x00

/* defines for messagebox */
#define  MB_BXSIZE        128
#define  MB_BYSIZE        76
#define  MB_BEDGE         4
#define  MB_BXORG         (160-MB_BXSIZE)/2
#define  MB_BYORG         (160-MB_BYSIZE)/2
#define  MB_YESXORG       MB_BXORG+MB_BEDGE+10
#define  MB_YESYORG       MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1
#define  MB_NOXORG        MB_BXORG+MB_BXSIZE-MB_BEDGE-MB_BUTTONXSIZE-10
#define  MB_NOYORG        MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1

#define  MB_OKXORG        (160-MB_BUTTONXSIZE)/2
#define  MB_OKYORG        MB_BYORG+MB_BYSIZE-MB_BUTTONYSIZE-MB_BEDGE-1

/* ShowTree */
#define  SHOW    TRUE
#define  COUNT   FALSE

/* Area selection */
#define AREA1     TRUE
#define AREA2     FALSE

/* Input cancel button */
#define  IP_CANCELXORG    (160-30)/2
#define  IP_CANCELYORG    60

/******************************************************************************
  Definition of STRUCTS
******************************************************************************/

typedef struct AREA12LINE{
  word entryno;  /* number of entry in the info-file */
  byte x;        /* number of field where the Foldericon is located(ebene) */
  byte length;   /* 1+Length of name (LibGetProStrSize) */
  bool used;
}AREA12LINE;

/******************************************************************************
  Definition of const DATA (Icons, Touchtables, Messages)
******************************************************************************/

extern const byte far CornerIcon[];

extern const byte far Folder[];

extern const byte far MainFolder[];

extern const byte far Plus[];

extern const byte far Minus[];

extern TCHTBL far TchList[];

extern T_ICON far fileicon;
extern T_ICON far miscicon;
extern T_ICON far reseticon;

extern TCHTBL far TchYESNO[];

extern TCHTBL far TchOK[];

extern T_ICON far MB_ICON_YES;
extern T_ICON far MB_ICON_NO;

extern T_ICON far MB_ICON_OK;

extern TCHTBL far TchReceiveCancel[];

extern T_ICON far RCRECEIVEICON;
extern T_ICON far RCCANCELICON;

extern TCHTBL far TchAboutOk[];

extern T_ICON far aboutokicon;

extern TCHTBL far TchOptions[];

extern T_ICON far optcbicon;
extern T_ICON far optokicon;
extern T_ICON far optcancelicon;

extern TCHTBL far TchOptionsPage1[];

extern T_ICON far optp1cb1icon;
extern T_ICON far optp1cb2icon;


extern TCHTBL far TchOptionsPage2[];

extern TCHTBL far TchOptionsPage3[];


extern byte errcodemessages[4][20+1];
/* max. lines 3!! */
extern byte errnomessages[3][16][60+1];

extern byte Message[3][75];

extern byte cb1string[3][21+1];

extern word BITS[9];

extern byte flowcontrolstr[4][15+1];

extern byte BPSstring[8][5+1];

extern word    parityredirect[3];
extern word    databitredirect[2];
extern word    stopbitredirect[2];

extern SRL_STAT      far *serial;

extern word          fp_first_infofile;

extern byte MiscMenue[4][11+1];

extern byte receiveoptions[3][13+1];

extern byte attstrings[8][3+1];

extern BK_SMPL_TCH BkCancel[2];

#endif