/******************************************************************************
*	[Title]         PVToday
*	[Model] 	PocketViewer
*	[Version]	Ver 2.10
*
*	Copyright (C) 2003 Jean-Michel GIRARD . All rights reserved.
*       http://pocketcasio.pocketviewer.de
******************************************************************************/
#ifdef __HITACHI__

#include    <hitachi.h>
#include    <stdio.h>
#include    "libc.h"

#define FAR /* nothing */
#define NEAR /* nothing */
#define far_strcpy strcpy
#define far_strncpy strncpy
#define far_strcmp strcmp
#define far_strncmp strncmp
#define far_memmove memmove
#define	IB_SMMAIL_OUTBOX	0x06		/*Define Mail Outbox for old PV model */

#else

#include    <stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#define FAR far
#define NEAR near

#endif

#include	"PVToday.h"
#include	"PVToDGraData.h"
#include	"PVToDFlash.h"
#include	"shared.h"

/***  ***/
static bool PVTodayPullDownEvent(word sel);
static bool PVTodayPullDownMenu(void);
static void PVTodayPullDownSetUp(void);
static void PVTodayShowCode(void);
static void SwapEdit(TCHSTS *tsts,byte bModeAct,byte *bModeNext);
static void PVTodayShowPersonnal(void);
static int PVTodayEditOptMail(int STARTX, int STARTY, int XSIZE, int YSIZE);
static void PVTodayShowPrefs(void);
static void PVTodayShowAbout(void);

static bool PVTodayCalcDay(word *wYear, byte *bMonth, byte *bDay, int iDelta);

static void DispBatt( void );
static void ConvertTimetoZone(byte bHour,byte bMinut, char *time);
static void Num2( byte *p, byte n );
static void DispTimer( void );
static void DispInvertDay(byte *pszDate);

static void PVTodayTchSet(void);
static void PVTodayInit( void );
static void PVTodayDisplay(void);
static void PVTodayDisplayInfo(char * pszDate);
static void PVTodayGetMail(char * szValueSent);
static void PVTodayGetRemind(char *pszDateAct, char * szValue);
static void PVTodayGetTask(char *pszDateAct, char * szValue);
static void PVTodayGetSchedule(char *pszDateAct, char * szValue, char * szTime, byte *bInLate);
static void PVTodayGetDate(char *pszDateAct, char * szValue);

static void PVTodayShowLstSchedule(char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE, byte bStart, bool *bAffVal);
static void PVTodayShowLstTasks(char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE, byte bStart, bool *bAffVal);
static void PVTodayListTaskOfDay(byte bTypeList, char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE);
static void StrToNum(word *dt_buff, byte *buff);

/* */
INFOSHOW  stPVTodayRecord;                      /* struct for SHARED library flash file */
FILE_BUF  pzlfb;                                /* FLASH data buffer (binaryBuffer 3KB)*/
FILE_INFO pzlfinf;                              /* FLASH data information */
TEXTAREA    TextArea[2];                        /* Ptr to Textbuffer struct */
byte blnInfoHere[I_MAXINFO+1];                  /* To select correct info */
byte blnInfoCoord[I_MAXINFO+1];                 /* To display correct info */

#define FILERECTODAY "PVTodayV2Data"

/***  ***/
static word wYearGen;                           /* Global year */
static byte bMonthGen,bDayGen;                  /* Global month and day */
static bool bActualDay;                         /* TRUE if the current day */

static TCHTBL TchFullScreen[] =
{
  { 0,0,159,159,
    ACT_MAKE | ACT_MOVE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK | ACT_500MSEC,
    0x8000,0x0000
  },

  { 0,0,0,0,ACT_NONE,OBJ_END,0x0000 }
};

static TCHTBL TchDispPref[] =
{
  { 6,140,6+39-1,140+14-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_POK, 0x0000
  },

  { 49,140,49+49-1,140+14-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PCANCEL, 0x0000
  },

  { 3,30+14,3+9,30+14+(I_MAXINFO+2)*11,
    /*ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | */ACT_BREAK_IN,
    OBJ_IC_PCHECK, 0x0000
  },

  { 3+11+45+3, 30+14+11*4,3+11+45+3+9, 30+14+11*4+5,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PMAIL, 0x0000
  },

  { 0, 0, 0, 0,ACT_NONE,OBJ_END,0x0000 }
};

T_ICON FAR TiconPOk  = { &TchDispPref[0], NULL, NULL, 0x00 };
T_ICON FAR TiconPEsc = { &TchDispPref[1], NULL, NULL, 0x00 };
T_ICON FAR TiconPMail= { &TchDispPref[3], NULL, NULL, 0x00 };

static TCHTBL TchDispPerso[] =
{
  { 6,73,6+39-1,73+14-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PEOK, 0x0000
  },

  { 49,73,49+49-1,73+14-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PECANCEL, 0x0000
  },

  { 3,/*15*/25,3+150-1,/*15*/25+10-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PETXT1, 0x0000
  },

  { 3,/*36*/46,3+150-1,/*36*/46+10-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PETXT2, 0x0000
  },

  { 0, 0, 0, 0,ACT_NONE,OBJ_END,0x0000 }
};

T_ICON FAR TiconPrOk  = { &TchDispPerso[0], NULL, NULL, 0x00 };
T_ICON FAR TiconPrEsc = { &TchDispPerso[1], NULL, NULL, 0x00 };

static TCHTBL TchChoice[] =
{
  { 1,I_YDEBINFO+2,159,I_YDEBINFO+I_YINFOSIZE*I_MAXINFO,
    ACT_MAKE | ACT_MOVE_IN |ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_CHOICE, 0x0000,
  },

  { 0, 0, 0, 0,ACT_NONE,OBJ_END,0x0000 }
};

static TCHTBL TchDispRefresh[] =
{
  { 2+10+50,0,2+10+50+9-1,0+11-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_REFRESH, 0x0000
  },

  { 2+10+50+10+3,0,2+10+50+10+3+6,0+11-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_PREVDAY, 0x0000
  },

  { 2+10+50+10+3+10*8,0,2+10+50+10+3+10*8+6,0+11-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_NEXTDAY, 0x0000
  },

  { 2+10+50+10+3+7+0*10+1,0,2+10+50+10+3+7+7*10-1,0+11-1,
    ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
    OBJ_IC_CHANDAY, 0x0000
  },

  { 0, 0, 0, 0,ACT_NONE,OBJ_END,0x0000 }
};

word MPdEdt[] =
{
  PDNTBLEND,
};

word MPdSys[] =
{
  PDNTBLEND,
};

word MPdOpt[] =
{
  19,            /* Option */
  196,           /* Personnal */
  401,           /* Version */
  
  PDNTBLEND,
};


/***V2.0  ****************Select Mail List ***********************************/
static TCHTBL TchLstSelWinMail[] =
{
  { 0, 0, 0, 0,
    ACT_MAKE | ACT_BREAK_IN, OBJ_AREA,
    0x0000
  },

  { 0, 0, 0, 0,ACT_NONE,OBJ_END,0x0000 }
};

/******************************************************************************
	[Title]		PVTodayPullDownEvent
	[Arguments]	word Sel(select number)
	[ReturnVAL]	bool TRUE=Rediplay
******************************************************************************/
static bool PVTodayPullDownEvent(word sel)
{
  bool blnMenuClick = FALSE;
  switch(sel)
  {
    /* Set options */
    case PDWN_OPT_OPTIONS:
      PVTodayShowPrefs();
      break;
    /* Show Personnal */
    case PDWN_OPT_PERSONA:
      PVTodayShowPersonnal();
      break;
    /* Show About */
    case PDWN_OPT_VERSION:
      PVTodayShowAbout();
      break;
    default:
      break;
  }
  return(blnMenuClick);
}

/******************************************************************************
	[Title]		PVTodayPullDownMenu
	[Arguments]	None
	[ReturnVAL]	bool TRUE=Redisplay
******************************************************************************/
static bool PVTodayPullDownMenu(void)
{
  word wSelect;
  bool blnMenuClick = FALSE;

  wSelect = LibPullDown();		/* PullDownMenu */
  if(wSelect & PULLDOWN_OPTION)
    blnMenuClick = PVTodayPullDownEvent(wSelect);
  return(blnMenuClick);
}

/******************************************************************************
	[Title]  PVTodayPullDownSetUp
	[Arguments]	None
	[ReturnVAL]	None
******************************************************************************/
static void PVTodayPullDownSetUp(void)
{
  word wPullOpt;

  /* MenuBAR Init */
  LibPullDownInit(MPdEdt,MPdSys,MPdOpt);

  /* [Option] only */
  wPullOpt =  PULLDOWN_OPTION | 0x0100;
  LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_OPTION, wPullOpt);
}

/******************************************************************************
*	[Titel]         SwapEdit
*	[Arguments]     None
*	[ReturnVAL]	None
*       Go to the next field in Edit mode
******************************************************************************/
static void SwapEdit(TCHSTS *tsts, byte bModeAct,byte *bModeNext)
{
  LibTxtDsp(&(TextArea[bModeAct].EventPrm));
  LibPutDisp();
  LibTxtInit(&(TextArea[*bModeNext].EventPrm));
  LibTxtTchSet(&(TextArea[*bModeNext].EventPrm));
}

/******************************************************************************
*	[Titel]         PVTodayShowPersonnal
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayShowPersonnal(void)
{
  TCHSTS      tsts;
  int         iSize;
  bool        bLoopEnd;
  char        szEditString[I_MAXNAME+1];             /* Name */
  char        szEditString1[I_MAXNAME+1];            /* Phone */
  byte        bKeyChk,bMode,bModeSvg;
  word        GdTbl[1];                              /* Guidance comment table */

  /* Init text buffer for Owner */
  GdTbl[0] = 0xffff;                            /* No Guidance comment */
  bMode = 0;                                    /* 1st text editing */
  TextArea[0].EventPrm.st_x = 3;                /* Start coordinate (X) of text display */
  TextArea[0].EventPrm.st_y = 25;               /* Start coordinate (Y) of text display */
  TextArea[0].EventPrm.ed_x = 145;              /* End coordinate (X) of text display  */
  TextArea[0].EventPrm.it_y = 9;                /* Text display line spacing (Y) */
  TextArea[0].EventPrm.MAXGYO = 1;              /* Number of text display lines  */
  TextArea[0].EventPrm.font = IB_PFONT1;        /* Display font type  */
  TextArea[0].EventPrm.csen = TRUE;             /* Cursor display enabled(Keyword registration enabled) */
  TextArea[0].EventPrm.rtnen = 0x02;            /* CR code display enabled  */
  TextArea[0].EventPrm.maxmj = I_MAXNAME;       /* Maximum number of allowable input characters. */
  TextArea[0].EventPrm.txtobj = OBJ_IC_PETXT1;     /* Object when text area is touched.  */
  TextArea[0].EventPrm.sbrobj = NULL;           /* Object when the scroll bar is touched. */
  TextArea[0].EventPrm.tchtb = TextArea[0].TchTxtBuf;       /* Pointer for text scroll bar area  */
  TextArea[0].EventPrm.s_pos = &(TextArea[0].ComScr);         /* Pointer for text and scroll bar information */
  TextArea[0].EventPrm.gdcmt = GdTbl;           /* Guidance comment table =none */
  TextArea[0].EventPrm.txbf = (byte *) szEditString;     /* Designation of text buffer address  */

  TextArea[1].EventPrm.st_x = 3;                /* Start coordinate (X) of text display */
  TextArea[1].EventPrm.st_y = 46;               /* Start coordinate (Y) of text display */
  TextArea[1].EventPrm.ed_x = 145;              /* End coordinate (X) of text display  */
  TextArea[1].EventPrm.it_y = 9;                /* Text display line spacing (Y) */
  TextArea[1].EventPrm.MAXGYO = 1;              /* Number of text display lines  */
  TextArea[1].EventPrm.font = IB_PFONT1;        /* Display font type  */
  TextArea[1].EventPrm.csen = TRUE;             /* Cursor display enabled(Keyword registration enabled) */
  TextArea[1].EventPrm.rtnen = 0x02;            /* CR code display enabled  */
  TextArea[1].EventPrm.maxmj = I_MAXNAME;       /* Maximum number of allowable input characters. */
  TextArea[1].EventPrm.txtobj = OBJ_IC_PETXT2;     /* Object when text area is touched.  */
  TextArea[1].EventPrm.sbrobj = NULL;           /* Object when the scroll bar is touched. */
  TextArea[1].EventPrm.tchtb = TextArea[1].TchTxtBuf;       /* Pointer for text scroll bar area  */
  TextArea[1].EventPrm.s_pos = &(TextArea[1].ComScr);         /* Pointer for text and scroll bar information */
  TextArea[1].EventPrm.gdcmt = GdTbl;           /* Guidance comment table  */
  TextArea[1].EventPrm.txbf = (byte *) szEditString1;    /* Designation of text buffer address  */

  /* Open a new windows */
  LibOpenWindow(0, 0, 159, 159);

  /* Draw window */
  LibPutGraph( 0, 0+1, bmpLeft);
  iSize = LibGetProStrSize(IB_PFONT2,(byte *) SZ_PERSTITLE);
  LibPutProStr(IB_PFONT2,80-iSize/2, 0+3, (byte *) SZ_PERSTITLE, iSize);
  LibPutGraph( 0+159-2, 0+1, bmpRight);
  LibReverse(0+2, 0+1, 159-2-2, 11);
  LibGdsBox(0,0+12,0+159-1,0+159);
  LibPutProStr(IB_PFONT2,3, 15, (byte *) SZ_PROWNER, 150);
  LibPutLineSub(3,33,150,33,IB_GDS_MESH );
  LibPutProStr(IB_PFONT2,3, 36, (byte *) SZ_PRTEL, 150);
  LibPutLineSub(3,54,150,54,IB_GDS_MESH );

  /* Draw buttons  */
  PVGraphDrawButton(6,73,39,14,SZ_OKBUTTON);
  PVGraphDrawButton(49,73,49,14,SZ_CANCELBUTTON);

  /* Init text buffer, text input and text area PUSH */
  strcpy(szEditString,stPVTodayRecord.szInfoOwner);
  strcpy(szEditString1,stPVTodayRecord.szInfoPhone);
  LibPutProStr(IB_PFONT1,3, 25, (byte *) szEditString, 140);
  LibPutProStr(IB_PFONT1,3, 46, (byte *) szEditString1, 140);
  LibPutDisp();

  /* Init current touch scan */
  LibTchStackPush(NULL);
  LibTchStackPush( TchHardIcon );
  LibTchStackPush( TchDispPerso );
  LibTxtInit(&(TextArea[bMode].EventPrm));
  LibTxtTchSet(&(TextArea[bMode].EventPrm));

  /* do while touch set */
  LibDispKey();
  bLoopEnd = FALSE;
  while(bLoopEnd==FALSE)
  {
    LibTxtDsp(&(TextArea[bMode].EventPrm));
    LibPutDisp();
    bKeyChk = LibGetKeyM(&tsts);
    /* If not Keyboard */
    if(bKeyChk == KEY_NONE)
    {
      switch(tsts.obj)
      {
        case OBJ_LPSW_PUSH:
        case OBJ_IC_PEOK:
          if (LibIconClick2( &TiconPrOk, &tsts ) == TRUE)
          {
            strcpy(stPVTodayRecord.szInfoOwner,szEditString);
            strcpy(stPVTodayRecord.szInfoPhone,szEditString1);
            if (!sfSaveRecord((char *) FILERECTODAY,sizeof(stPVTodayRecord),(byte*)&stPVTodayRecord))
            {
              LibPutMsgDlg((byte *) SZ_SHAREDERROR1);
              LibJumpMenu();
            }
            bLoopEnd = TRUE;
          };
          break;
        case OBJ_IC_PECANCEL:
          bLoopEnd = (LibIconClick2( &TiconPrEsc, &tsts ) == TRUE);
          break;
        case OBJ_HIC_ESC:
          bLoopEnd = TRUE;
          break;
        /* case for cliking in textbox (much nicer than next touch */
        case OBJ_IC_PETXT1:
          if (bMode !=0)
          {
            bModeSvg=bMode;
            bMode = 0;
            SwapEdit(&tsts,bModeSvg,&bMode);
          }
          break;
        case OBJ_IC_PETXT2:
          if (bMode != 1)
          {
            bModeSvg=bMode;
            bMode = 1;
            SwapEdit(&tsts,bModeSvg,&bMode);
          }
          break;
        default:
          LibRepOff();
          break;
      }
    }
    else
    /* Next on keyboard, go from textbox 1 -> 2 or 2 -> 1 */
    if(bKeyChk == KEY_NEXT)
    {
      bModeSvg=bMode;
      bMode = (bMode==0?1:0);
      SwapEdit(&tsts,bModeSvg,&bMode);
    }

    /* Check input */
    LibTxtInp(bKeyChk,&tsts,&(TextArea[bMode].EventPrm));
  }

  /* Quit safely this form */
  while(LibTchStackPop()!=NULL);
  LibTchInit();
  LibCloseWindow();
  LibPutDisp();
}

/******************************************************************************
*	[Titel]         PVTodayEditOptMail
*	[Arguments]     None
*	[ReturnVAL]     iIndex = mail option selected
*       Select a new selection of mail programm
******************************************************************************/
static int PVTodayEditOptMail(int STARTX, int STARTY, int XSIZE, int YSIZE)
{
  TCHSTS	tsts;
  int	iMailSel;
  bool	end_flag = FALSE;

  /* touch area */
  TchLstSelWinMail[0].x1=STARTX+1;
  TchLstSelWinMail[0].y1=STARTY+1;
  TchLstSelWinMail[0].x2=STARTX+XSIZE;
  TchLstSelWinMail[0].y2=STARTY+YSIZE;

  /* set up & down flag and current position */
  LibOpenWindow(STARTX, STARTY, XSIZE+2+2, YSIZE+2+2);

  /* display background */
  LibGdsLine(STARTX, STARTY+1, STARTX, STARTY+YSIZE+2-1);  /*left line */
  LibGdsLine(STARTX+1, STARTY, STARTX+XSIZE+2-1, STARTY);  /* up line */
  LibGdsLine(STARTX+1, STARTY+YSIZE+2, STARTX+XSIZE+2-1, STARTY+YSIZE+2);      /* upderline */
  LibGdsLine(STARTX+2, STARTY+YSIZE+2+1, STARTX+XSIZE+2-2, STARTY+YSIZE+2+1);
  LibGdsLine(STARTX+XSIZE+2, STARTY+1, STARTX+XSIZE+2, STARTY+YSIZE+2-1);      /* right line */
  LibGdsLine(STARTX+XSIZE+2+1, STARTY+2, STARTX+XSIZE+2+1, STARTY+YSIZE+2-2);
  LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2,(byte *) SZ_PRMAILOPT1,70);
  LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2+11*1,(byte *) SZ_PRMAILOPT2,70);
  LibPutDisp();

  /* Init touchs */
  LibTchStackPush(NULL);
  LibTchStackPush(TchLstSelWinMail);
  LibTchInit();

  /* While true */
  while(end_flag == FALSE)
  {
    /* Wait for a touch */
    LibTchWait( &tsts );
    switch( tsts.obj )
    {
      /* Icon select */
      case  OBJ_AREA:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        iMailSel = (tsts.y-STARTY) / 11;
        if( (iMailSel<0) || (iMailSel>=2) ) break;
        LibReverse(STARTX+2, STARTY+2+iMailSel*11, 70, 11);
        LibPutDisp();
        LibWait(IB_250MWAIT);
        LibReverse(STARTX+2, STARTY+2+iMailSel*11, 70, 11);
        LibPutDisp();
        iMailSel++; /* To make it in 1..2 */
        end_flag = TRUE;
        break;
      default:
         LibRepOff();
         break;
    }
  }

  /* Free touch buffer, close window and redraw screen */
  while (LibTchStackPop()!=NULL)
    LibTchInit();
  LibCloseWindow();
  LibPutDisp();

  return (iMailSel);
}

/******************************************************************************
*	[Titel]         PVTodayShowPrefs
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayShowPrefs(void)
{
  TCHSTS  tsts;
  bool    bLoopEnd;
  int     iSize,iBcl,iOption;

  /* Open a new windows */
  LibOpenWindow(0, 30, 159, 129);

  /* Draw window */
  LibPutGraph( 0, 30+1, bmpLeft);
  iSize = LibGetProStrSize(IB_PFONT2,(byte *) SZ_PREFTITLE);
  LibPutProStr(IB_PFONT2,80-iSize/2, 30+3, (byte *) SZ_PREFTITLE, iSize);
  LibPutGraph( 0+159-2, 30+1, bmpRight);
  LibReverse(0+2, 30+1, 159-2-2, 11);
  LibGdsBox(0,30+12,0+159-1,30+129);

  /* Draw all options */
  LibPutProStr(IB_PFONT1,3+11, 30+14, (byte *) SZ_PRCHKDATE,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11, (byte *) SZ_PRCHKOWNER,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*2, (byte *) SZ_PRCHKCALEND,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*3, (byte *) SZ_PRCHKTASK,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*4, (byte *) SZ_PRCHKMAIL,45);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*5,(byte *) SZ_PRCHKSYSTEM,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*6, (byte *) SZ_PRCHKTIME,140);
  LibPutProStr(IB_PFONT1,3+11, 30+14+11*7,(byte *) SZ_PRCHKTSKACT,140);
  for (iBcl=0;iBcl<I_MAXINFO;iBcl++)
  {
    LibPutFarData(3,30+14+iBcl*11,(stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]] >= 1?17:16));
  }
  LibPutFarData(3,30+14+11*6,(stPVTodayRecord.bChkTime == 1?17:16));
  LibPutFarData(3,30+14+11*7,(stPVTodayRecord.bChkSched == 1?17:16));

  /* V 2.0 Diffrent mail supports */
  LibPutFarData(3+11+45+3,30+14+11*4,187);
  if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] >= 1)
  {
    LibPutProStr(IB_PFONT1,3+11+45+3+9+3, 30+14+11*4,(stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]]==1? (byte *) SZ_PRMAILOPT1 : (byte *) SZ_PRMAILOPT2),70);
  }
  else
  {
    LibPutProStr(IB_PFONT1,3+11+45+3+9+3, 30+14+11*4, (byte *) SZ_PRCHKMAIL,70);
    LibMesh(3+11+45+3+9+3, 30+14+11*4,  70, 11);
  }

  /* Draw buttons  */
  PVGraphDrawButton(6,140,39,14,SZ_OKBUTTON);
  PVGraphDrawButton(49,140,49,14,SZ_CANCELBUTTON);
  LibPutDisp();

  /* Init current touch scan */
  LibTchStackPush(NULL);
  LibTchStackPush( TchHardIcon );
  LibTchStackPush( TchDispPref );
  LibTchInit();

  /* do while touch set */
  bLoopEnd = FALSE;
  while(bLoopEnd==FALSE)
  {
    LibTchWait( &tsts );
    switch(tsts.obj)
    {
      case OBJ_IC_PMAIL:
        /* If mail is checked */
        if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] >= 1)
        {
          if (LibIconClick2( &TiconPMail, &tsts ) == TRUE)
          {
            stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] = PVTodayEditOptMail(3+11+45+3+9+3, 30+14+11*4, 70,11*2);
            LibClrBox(3+11+45+3+9+3, 30+14+11*4,  70, 11);
            LibPutProStr(IB_PFONT1,3+11+45+3+9+3, 30+14+11*4,(stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]]==1? (byte *) SZ_PRMAILOPT1 : (byte *) SZ_PRMAILOPT2),70);
            LibPutDisp();
          }
        }
        break;
      case OBJ_LPSW_PUSH:
      case OBJ_IC_POK:
        if (LibIconClick2( &TiconPOk, &tsts ) == TRUE)
        {
          if (!sfSaveRecord((char *) FILERECTODAY,sizeof(stPVTodayRecord),(byte*)&stPVTodayRecord))
          {
            LibPutMsgDlg((byte *) SZ_SHAREDERROR1);
            LibJumpMenu();
          }
          bLoopEnd = TRUE;
        };
        break;
      case OBJ_IC_PCHECK:
        iBcl = ((tsts.y-(30+14)) / 11);
        if (iBcl<I_MAXINFO)
        {
          if (iBcl == 4)
            stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]] = (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]] >= 1?0:1);
          else
            stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]] = (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]] == 1?0:1);
          iOption = stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[iBcl]];
          LibClrBox(3+11+45+3+9+3, 30+14+11*4,  70, 11);
          if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] >= 1)
          {
            LibPutProStr(IB_PFONT1,3+11+45+3+9+3, 30+14+11*4,(stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]]==1? (byte *) SZ_PRMAILOPT1 : (byte *) SZ_PRMAILOPT2),70);
          }
          else
          {
            LibPutProStr(IB_PFONT1,3+11+45+3+9+3, 30+14+11*4, (byte *) SZ_PRCHKMAIL,70);
            LibMesh(3+11+45+3+9+3, 30+14+11*4,  70, 11);
          }
        }
        else
        {
          if (iBcl == I_MAXINFO)
          {
            stPVTodayRecord.bChkTime = (stPVTodayRecord.bChkTime == 1?0:1);
            iOption = stPVTodayRecord.bChkTime;
          }
          else
          {
            iBcl = I_MAXINFO+1;
            stPVTodayRecord.bChkSched = (stPVTodayRecord.bChkSched == 1?0:1);
            iOption = stPVTodayRecord.bChkSched;
          }
        }
        LibPutFarData(3,30+14+iBcl*11,(iOption == 1?17:16));
        LibPutDisp();
        break;
      case OBJ_IC_PCANCEL:
        bLoopEnd = (LibIconClick2( &TiconPEsc, &tsts ) == TRUE);
        break;
      case OBJ_HIC_ESC:
        bLoopEnd = TRUE;
        break;
      default:
        LibRepOff();
        break;
    }
  }

  /* Quit safely this form */
  while(LibTchStackPop()!=NULL);
  LibTchInit();
  LibCloseWindow();
  LibPutDisp();
}

/******************************************************************************
*	[Titel]         PVTodayShowAbout
*	[Arguments]	None
*	[ReturnVAL]	None
*       Display About window
******************************************************************************/
static void PVTodayShowAbout(void)
{
  int     iSize;
  word  code,status;
#ifdef __HITACHI__
  byte  *icon;
  byte  *ofs;
#else
  byte far *icon;
  word seg,ofs;
#endif
  
  /* Open a new windows */
  LibOpenWindow(0, 0, 160, 160);

  /* Draw window */
  LibPutGraph( 0, 0+1, bmpLeft);
  iSize= LibGetProStrSize(IB_PFONT3,(byte *) SZ_ABOUTTITLE);
  LibPutProStr(IB_PFONT2,80-iSize/2, 0+3, (byte *) SZ_ABOUTTITLE, iSize);
  LibPutGraph( 0+160-2, 0+1, bmpRight);
  LibReverse(0+2, 0+1, 160-2-2, 11);
  LibGdsBox(0,0+12,0+160-1,0+160-1);
  LibPutProStr(IB_CG57FONT,3,150,(byte *) "V",20);
  LibPut35Str(9,152,(byte *) SZ_ABOUT1);
  iSize= LibGetProStrSize(IB_PFONT3,(byte *) SZ_TITLE);
  LibPutProStr(IB_PFONT3,55,28,(byte *) SZ_TITLE,104);
  iSize= LibGetProStrSize(IB_PFONT1,(byte *) SZ_ABOUT2);
  LibPutProStr(IB_PFONT1,80-iSize/2,55,(byte *) SZ_ABOUT2,iSize);
  iSize= LibGetProStrSize(IB_PFONT1,(byte *) SZ_ABOUT3);
  LibPutProStr(IB_PFONT1,80-iSize/2,70,(byte *) SZ_ABOUT3,iSize);
  iSize= LibGetProStrSize(IB_PFONT1,(byte *) SZ_ABOUT4);
  LibPutProStr(IB_PFONT1,80-iSize/2,85,(byte *) SZ_ABOUT4,iSize);
  iSize= LibGetProStrSize(IB_PFONT1,(byte *) SZ_ABOUT4);
  LibPutProStr(IB_PFONT1,80-iSize/2,100,(byte *) SZ_ABOUT5,iSize);
  iSize= LibGetProStrSize(IB_PFONT1,(byte *) SZ_ABOUT4);
  LibPutProStr(IB_PFONT1,80-iSize/2,115,(byte *) SZ_ABOUT6,iSize);

  /* Get Application Icon and draw it */
#ifdef __HITACHI__
  LibGetMode((word *) &code,(word *) &status,(void *) &ofs);
  LibGetMenuIcon(&icon, code >> 8, code & 0xff);
#else
  LibGetMode(&code,&status,&seg,&ofs);
  LibGetMenuIcon(&icon, code >> 8, code & 0xff);
#endif
  LibPutGraph(5,20,icon);

  /* Wait for OK */
  PVGraphWaitForOK();
  
  /* Quit safely this form */
  while(LibTchStackPop()!=NULL);
  LibCloseWindow();
  LibPutDisp();
}

/******************************************************************************
*	[Titel]         PVTodayCalcDay
*	[Arguments]	wYear,bMonth and bDay : Actual and futur year, month day
*                 iDelta : +/- about the current date
*	[ReturnVAL]	None
* Compute the new day regarding the delta
******************************************************************************/
static bool PVTodayCalcDay(word *wYear, byte *bMonth, byte *bDay, int iDelta)
{
  word wPrecMonth[2],wActMonth[2],wYearCurr;
  byte bPrecNbdays,bActNbDays,bDayCurr,bMonthCurr;

  /* Get the current day to the return code */
  LibGetDate2(&wYearCurr,&bMonthCurr,&bDayCurr);

  /* Check the precedent month and compute the number of days of actual month */
  /*  and last month */
  wPrecMonth[0]  = (*bMonth == 1?*wYear-1:*wYear);
  wPrecMonth[1]  = (*bMonth == 1?12:*bMonth-1);
  wActMonth[0]  = *wYear;
  wActMonth[1]  = *bMonth;
  bPrecNbdays = LibGetDays(wPrecMonth);
  bActNbDays= LibGetDays(wActMonth);

  /* If delta<0 */
  if (iDelta<0)
  {
    /* Check for prec. month */
    if (*bDay<=-iDelta)
    {
      *bDay = bPrecNbdays + (*bDay+iDelta);
      *wYear= (*bMonth == 1?*wYear-1:*wYear);
      *bMonth= (*bMonth == 1?12:*bMonth-1);
    }
    else
      *bDay+=iDelta;
  }
  else
  {
    /* Check for next month */
    if (*bDay+iDelta>bActNbDays)
    {
      *bDay = (*bDay+iDelta)-bActNbDays;
      *wYear= (*bMonth == 12?*wYear+1:*wYear);
      *bMonth= (*bMonth == 12?1:*bMonth+1);
    }
    else
      *bDay+=iDelta;
  }

  /* Return with TRUE if day = actual day */
  return((*wYear==wYearCurr) && (*bMonth==bMonthCurr) && (*bDay == bDayCurr) );
}

/******************************************************************************
*	[Titel]         DispBatt
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
int GetBatt();                          /* To make compile OK ! */
static void DispBatt( void )
{
#ifdef __HITACHI__
#define GETBATTADDR 0x8C02E35C
#define GETBATTTARGET 0x2FE62FD6
#else
#define GETBATTADDR 0xC00006D1
#define GETBATTTARGET 0x100C6AE4
#endif

  union
  {
    unsigned int (FAR  * GetBattValue)();
    unsigned long FAR  * p;
  } u;
  unsigned int iVal,iMemory;
  char szMemo[7+1],szBatt[7+1];
  int iBcl;

  /* test if GetBattValue will reach expected code */
  u.p = (unsigned long  FAR *) GETBATTADDR;
  if (*u.p != GETBATTTARGET)
    /* error: unexpected code */
    iVal = 0;
  else
    iVal = u.GetBattValue();

  /* Change value to percent */
  /* Reactualize fact (should be 372 if 1023 is  100%. But we assume 927 (3.0V) = 100%) */
  /*  and just to be sure ... if something ugly happens */
  iVal=iVal-651;
  if (iVal < 0) { iVal=0; }
  iVal = iVal/2.76;
  if (iVal > 100) { iVal=100; }
  if (iVal <   0) { iVal = 0; }
  /*iVal = (int) (100*((float) iVal / 1023));*/

  /* Get the percent free of flash memory */
  if (LibGetFlash() == 0)
    iMemory =0;
  else
    iMemory = (int) (100*((float) LibGetFreeBlock() / (float) LibGetFlash()));

  /* print value */
  sprintf(szMemo,"%3.0d%%",iMemory);
  sprintf(szBatt,"%3.0d%%",iVal);
  LibGdsBoxClr(19,1+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],20+28*4,11+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5]-1-1);
  LibGdsBox   (19,1+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],20+28*4,11+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5]-1-1);
  LibGdsBoxClr(19,1+12+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],20+28*4,11+12+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5]-1-1);
  LibGdsBox   (19,1+12+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],20+28*4,11+12+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5]-1-1);
  for (iBcl = 0; iBcl<28; iBcl++)
  {
    if(iBcl<((iVal*28)/100))
      LibPutGraph(20+iBcl*4,I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],bmpProgress);
    if(iBcl<((iMemory*28)/100))
      LibPutGraph(20+iBcl*4,12+I_YDEBINFO+1+I_YINFOSIZE*blnInfoCoord[5],bmpProgress);
  }
  LibPutProStr(IB_PFONT1,20+28*4+2,I_YDEBINFO+2+I_YINFOSIZE*blnInfoCoord[5],(byte *) szBatt,35);
  LibPutProStr(IB_PFONT1,20+28*4+2,12+I_YDEBINFO+2+I_YINFOSIZE*blnInfoCoord[5],(byte *) szMemo,35);
  LibPutDisp();
}

/******************************************************************************
*	[Titel]         ConvertTimetoZone
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void Num2( byte *p, byte n )
{
  *p=(byte)(n/10)+'0';
  *(p+1)=(byte)(n%10)+'0';
}

static void ConvertTimetoZone(byte bHour,byte bMinut, char *time)
{
  /* Add a or p for Time in English */
  /* and correct time between 0..12 */
  time[5] = ' ';
  time[6] = ' ';
  if ((stPVTodayRecord.bChkTime == 1))
  {
    time[5] ='A';
    time[6] ='M';
    if (bHour>12)
    {
      time[5] ='P';
      bHour -=12;
    }
  }
  Num2((byte *) &time[0], bHour);
  time[2]=':';
  Num2((byte *) &time[3], bMinut);
  time[7]='\0';
}

/******************************************************************************
*	[Titel]         DispTimer
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DispTimer( void )
{
  byte t[7+1];
  byte bHour,bMinut,bSecond;

  /* Display time now */
  if (bActualDay)
  {
    /* Get normal time (not in BCD) */
    LibGetTime2(&bHour, &bMinut, &bSecond );

    /* Call for convert time to correct version  V1.1 06/04/2002 */
    ConvertTimetoZone(bHour,bMinut,(char *) t);
    LibPutProStr(IB_PFONT1,I_XTIMELU,I_YDEBINFO+2+I_YINFOSIZE*stPVTodayRecord.bInfoOrder[0]+I_YINFOSIZE/2,t,45);
  }
  else
    LibPutProStr(IB_PFONT1,I_XTIMELU,I_YDEBINFO+2+I_YINFOSIZE*stPVTodayRecord.bInfoOrder[0]+I_YINFOSIZE/2,(byte *) SZ_SHOWNOTIME,159-I_XTIMELU);
  LibPutDispBox( I_XTIMELU, I_YDEBINFO+2+I_YINFOSIZE*stPVTodayRecord.bInfoOrder[0]+I_YINFOSIZE/2,159-I_XTIMELU,15);
}

/******************************************************************************
*	[Title]         DispInvertDay
*	[Arguments]	None
*	[ReturnVAL]	None
* Invert the day specified by the date
******************************************************************************/
static void DispInvertDay(byte *pszDate)
{
  byte bDow;

  /* Get the day */
  bDow = LibGetDow(pszDate);
  
  /* Invert the rect of the day */
  LibReverse(2+10+50+10+3+7+bDow*10+1, 1, 9, 9);
  LibPutDispBox( 2+10+50+10+3+7, 1,7*10,9);
}

/******************************************************************************
*	[Title]         PVTodayTchSet
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayTchSet(void)
{
  LibTchStackClr();
  LibTchStackPush( NULL );
  LibTchStackPush( TchHardIcon );
  
  LibTchStackPush( TchFullScreen );
  LibTchStackPush( TchChoice );
  LibTchStackPush( TchDispRefresh );
}

/******************************************************************************
*	[Title]         PVTodayInit
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayInit( void )
{
  byte bBcl;
  	
  /* Init Data Flash Buffer */
  /* If not find SHARED record, make it */
  if (!sfLoadRecord((char *) FILERECTODAY,sizeof(stPVTodayRecord),(byte*)&stPVTodayRecord))
  {
    for (bBcl=0;bBcl<I_MAXINFO;bBcl++)
    {
      stPVTodayRecord.blnInfoShow[bBcl] = 1;		/* Differents showed infos */
      stPVTodayRecord.bInfoOrder[bBcl] = bBcl;			/* infos order */
    }
    strcpy(stPVTodayRecord.szInfoOwner,SZ_OWNER);
    strcpy(stPVTodayRecord.szInfoPhone,"-- / -- / --");
    stPVTodayRecord.bChkTime = 0;
    stPVTodayRecord.bChkSched = 0;
    if (!sfSaveRecord((char *) FILERECTODAY,sizeof(stPVTodayRecord),(byte*)&stPVTodayRecord))
    {
      LibPutMsgDlg((byte *) SZ_SHAREDERROR1);
      LibJumpMenu();
    }
  }
  if (!sfLoadRecord((char *) FILERECTODAY,sizeof(stPVTodayRecord),(byte*)&stPVTodayRecord))
  {
      LibPutMsgDlg((byte *) SZ_SHAREDERROR2);
      LibJumpMenu();
  }

  /* Init display */
  PVTodayDisplay();
  PVTodayTchSet();
}

/******************************************************************************
*	[Title]         PVTodayDisplay
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayDisplay(void)
{
  byte bBcl;
  char szShortDay[3];
  
  /* Show Window Title */
  LibClrDisp();
  LibPutProStr(IB_PFONT2,6, 3, (byte *) SZ_TITLE, 60);
  LibPutGraph( 0, 1, bmpLeft);
  LibPutGraph( 2+6+50, 1, bmpRight);
  LibReverse(2, 1, 2+4+50, 11);
  LibPutLineSub(0,12,159-1,12,IB_GDS_OR);
  
  /* V1.2 06/05/2002 Display Refresh Button */
  LibPutGraph( 2+10+50, 0, bmpRefresh);

  /* V2.0 show the week days */
  LibPutFarData(2+10+50+10+3,3,198);
  LibPutFarData(2+10+50+10+3+10*8,3,213);
  for (bBcl=0;bBcl<7;bBcl++)
  {
    LibGdsBox(2+10+50+10+3+7+bBcl*10,0,2+10+50+10+3+7+(bBcl+1)*10,10);
    LibReadMessage(186+bBcl,(byte *) szShortDay);
    LibPutProStr(IB_PFONT1,2+10+50+10+3+7+bBcl*10+2, 2, (byte *) szShortDay, 8);
  }
  
  /* Really show infos */
  LibPutDisp();
}

/******************************************************************************
*	[Title]         PVTodayGetMail
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayGetMail(char * szValueSent)
{
  int iNbMailSend=0;

  /* Init varables */
  strcpy(szValueSent,SZ_NOUNSENDMAIL);

  /* Get unsent mails from correct addin */
  if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] == 1)
  {
   iNbMailSend= DataPV750MailRead();
  }
  else
  {
   iNbMailSend= DataPVMailRead();
  }

  /*  if one mail or more, changemessage */
  if (iNbMailSend>0)
  {
    sprintf(szValueSent,SZ_SHOWUNSENDMAIL,iNbMailSend);
  }
}

/******************************************************************************
*	[Title]         PVTodayGetRemind
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayGetRemind(char *pszDateAct, char * szValue)
{
  FILE_BUF fb;
  FILE_INFO finf;
  int iNbRemind=0;
  char szScheDate[8+1];

  /* Init variables */
  strcpy(szValue,SZ_NOREMIND);
  szScheDate[8]='\0';

  /* Get infos from tasks , and Common sub mode */
  fb.fsb_main_entry_ = 0x40;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xffff;  /* Search from top */
  finf.kind = FILE_KIND_TEXT;
  fb.fsb_sub_entry_ = 0x03;

    
  /* Init initial date search */
  memcpy(fb.fsb_date_srch_,pszDateAct,8);

  /* Specify searching by date 0x10 in LibFileFindNext */
  while (LibFileFindNextExt(&fb,&finf,0x10) != FALSE)
  {
    if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
      LibPutMsgDlg((byte *) "Read failed");
    else
    {
      /* if date is correct */
      memcpy(szScheDate,fb.fsb_date1_buf_,8);
      if (strcmp(pszDateAct,szScheDate) == 0)
        /* Inc. nbre of active reminds */
        iNbRemind++;
    }
  }

  /*  if one task or more, changemessage */
  if (iNbRemind>0)
  {
    sprintf(szValue,SZ_SHOWREMIND,iNbRemind);
  }
}

/******************************************************************************
*	[Title]         PVTodayGetTask
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayGetTask(char *pszDateAct, char * szValue)
{
  FILE_BUF fb;
  FILE_INFO finf;
  int iNbTask=0,iBcl;
  char szScheDate[8+1];

  /* Init variables */
  strcpy(szValue,SZ_NOTASK);
  szScheDate[8]='\0';

  /* for all rank */
  for (iBcl=0;iBcl<3;iBcl++)
  {
    /* Get infos from tasks , and Common sub mode */
    fb.fsb_main_entry_ = 0x40;
    fb.fsb_scrt_info_ = 0x80;
    finf.fp = 0xffff;  /* Search from top */
    finf.kind = FILE_KIND_TEXT;
    fb.fsb_sub_entry_ = 0x08;

    fb.fsb_todo_chek_ = 0x04; /* Uncheck task only */

    /* Init initial date search */
    memcpy(fb.fsb_date_srch_,pszDateAct,8);

    if (iBcl==2) {iBcl=3;}     /* no mode == 2, only 0,1,3 for C,B,A */
    fb.fsb_todo_rank_ = iBcl; /* Check actual rank */
    /* Specify searching by date 0x10 in LibFileFindNext */
    while (LibFileFindNextExt(&fb,&finf,0x10) != FALSE)
    {
      if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
        LibPutMsgDlg((byte *) "Read failed");
      else
      {
        /* if date is correct V1.1 06/04/2002 */
        memcpy(szScheDate,fb.fsb_date1_buf_,8);
        if (strcmp(pszDateAct,szScheDate) == 0)
          /* Inc. nbre of active tasks */
          iNbTask++;
      }
    }
  }

  /*  if one task or more, changemessage */
  if (iNbTask>0)
  {
    sprintf(szValue,SZ_SHOWTASK,iNbTask);
  }
}

/******************************************************************************
*	[Title]         PVTodayGetSchedule
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PVTodayGetSchedule(char *pszDateAct, char * szValue, char * szTime, byte *bInLate)
{
  FILE_BUF fb;
  FILE_INFO finf;
  char szScheDate[8+1];
  char szTimeDeb1[7+1],szTimeEnd1[7+1];
  byte bHour,bMinut,bSecond;
  int iHourActDeb,iMinActDeb,iHourActFin,iMinActFin;
  bool bOk;

  /* Get normal time (not in BCD) */
  /* V2.0, if not current day, put time to midnight */
  if (bActualDay)
    LibGetTime2(&bHour, &bMinut, &bSecond );
  else
  {
    bHour = 0; bMinut = 0; bSecond = 0;
  }
  szTimeDeb1[5]='\0';szTimeEnd1[5]='\0';
  szTimeEnd1[2]=':';
  szScheDate[8]='\0';
  strcpy(szValue,SZ_NOSCHEDULE);
  strcpy(szTime,"");

  /* Get infos from schedule , and Common sub mode */
  fb.fsb_main_entry_ = 0x40;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xffff;  /* Search from top */
  finf.kind = FILE_KIND_TEXT /*FILE_KIND_BIN*/;
  fb.fsb_sub_entry_ = 0x01;  /* 0x06 schedule information */

  /* Init initial date search */
  memcpy(fb.fsb_date_srch_,pszDateAct,8);

  /* Specify searching by date 0x10 in LibFileFindNext */
  while (LibFileFindNextExt(&fb,&finf,0x10) != FALSE)
  {
    if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
      LibPutMsgDlg((byte *) "Read failed");
    else
    {
      /* Get Task time */
      memcpy(szTimeDeb1,fb.fsb_time1_buf_,2);
      szTimeDeb1[2]='\0';
      szTimeEnd1[2]='\0';
      iHourActDeb = atoi(szTimeDeb1);
      memcpy(&szTimeDeb1,fb.fsb_time1_buf_+2,2);
      iMinActDeb = atoi(szTimeDeb1);
      memcpy(szTimeDeb1,fb.fsb_time2_buf_,2);
      iHourActFin = atoi(szTimeDeb1);
      memcpy(&szTimeDeb1,fb.fsb_time2_buf_+2,2);
      iMinActFin = atoi(szTimeDeb1);
      bOk = FALSE;
      /* if date is correct V1.1 06/04/2002 */
      *bInLate=0;
      memcpy(szScheDate,fb.fsb_date1_buf_,8);
      if (strcmp(pszDateAct,szScheDate) == 0)
      /* end correction if date is correct V1.1 06/04/2002 */
      {
        /* Check for time */
        if (stPVTodayRecord.bChkSched == 1)
        {
          bOk = ((iHourActFin*100+iMinActFin)>=(bHour*100+bMinut)) && ((iHourActDeb*100+iMinActDeb)<=(bHour*100+bMinut));
          if (bOk) {*bInLate=1;}
        }
        if (!bOk)
        {
          bOk = ((iHourActDeb*100+iMinActDeb)>=(bHour*100+bMinut));
        }
      }
      /* if current time after actual time ,store it and leave proc. */
      if (bOk)
      {
        /* V1.1 07/04/2002 , convert time to 12 or 24 hours for start and end time */
        memcpy(szTimeDeb1,fb.fsb_time1_buf_,2);
        iHourActDeb = atoi(szTimeDeb1);
        memcpy(&szTimeDeb1,fb.fsb_time1_buf_+2,2);
        iMinActDeb = atoi(szTimeDeb1);
        ConvertTimetoZone(iHourActDeb,iMinActDeb, szTimeDeb1);
        memcpy(szTimeEnd1,fb.fsb_time2_buf_,2);
        iHourActDeb = atoi(szTimeEnd1);
        memcpy(&szTimeEnd1,fb.fsb_time2_buf_+2,2);
        iMinActDeb = atoi(szTimeEnd1);
        ConvertTimetoZone(iHourActDeb,iMinActDeb, szTimeEnd1);
        sprintf(szValue,"%-54.54s",fb.fbuf.text.fsb_text_buf_);
        sprintf(szTime,"%-6.6s - %-6.6s",szTimeDeb1,szTimeEnd1);
        if (strcmp(szTime,"      -      ")==0)
        {
          strcpy(szTime,SZ_NOTASKTIME);
        }
        break;
      }
    }
  }
}

/******************************************************************************
*	[Title]         PVTodayGetDate
*	[Arguments]     pszDateAct : date to show 'YYYYMMDD'
*	[ReturnVAL]     szValue : New dateformated
*  Get the curret date and time
******************************************************************************/
static void PVTodayGetDate(char *pszDateAct, char * szValue)
{
  char szYear[4+1],szDay[2+1], szMon[2+1], szMonth[25+1], szDayName[25+1];

  /* Read current date and transform to variables */
  szYear[4]='\0';szDay[2]='\0';szMon[2]='\0';
  strncpy(szDay,pszDateAct+6,2);
  strncpy(szMon,pszDateAct+4,2);
  strncpy(szYear,pszDateAct,4);

  /* Get day from FLASH Messages 426 = Sunday and */
  /*  get month from FLASH Messages 281 = January */
  LibReadMessage(426+LibGetDow((byte *) pszDateAct) ,(byte *) szDayName);
  LibReadMessage(281-1+atoi(szMon),(byte *) szMonth);

  /* Return value regarding country display */
  sprintf(szValue,SZ_SHOWDATE,szDayName,szMonth,szDay,szYear);
}

/******************************************************************************
*	[Title]         PVTodayDisplayInfo
*	[Arguments]     pszDate : current date
*	[ReturnVAL]	None
*  Display all availables informations
******************************************************************************/
static void PVTodayDisplayInfo(char *pszDate)
{
  int iBcl,iBcl1=0, iIncY;           /* Coord */
  char szInfoOwner[55+1];    /* Info #1 */
  char szInfoOwner1[155+1];   /* Info #2 */
  byte bLate=0;

  /* Show differents infos */
  LibGdsClr(0,13,159,159);
  iIncY=0;
  for (iBcl=0;iBcl<I_MAXINFO;iBcl++)
  {
    blnInfoHere[iBcl] = 0xff;
    blnInfoCoord[iBcl] = iBcl;
  }
  for (iBcl=0;iBcl<I_MAXINFO;iBcl++)
  {
    if (stPVTodayRecord.blnInfoShow[iBcl] >= 1)
    {
      LibPutGraph( 1, I_YDEBINFO+2+I_YINFOSIZE*iIncY, pbmpGraphs[stPVTodayRecord.bInfoOrder[iBcl]] );
      /* Select info to display */
      blnInfoHere[iBcl1++] = stPVTodayRecord.bInfoOrder[iBcl];
      blnInfoCoord[iBcl] = iIncY;
      switch (stPVTodayRecord.bInfoOrder[iBcl])
      {
        case 0: /* Show date */
          PVTodayGetDate(pszDate,szInfoOwner);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          break;
        case 1: /* Show owner */
          sprintf(szInfoOwner,SZ_SHOWOWNER,stPVTodayRecord.szInfoOwner);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          sprintf(szInfoOwner,SZ_SHOWTEL,stPVTodayRecord.szInfoPhone);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+11+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          break;
        case 2: /* Show schedule */
          PVTodayGetSchedule(pszDate,szInfoOwner,szInfoOwner1,&bLate);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+11+I_YINFOSIZE*iIncY,(byte *) szInfoOwner1,139);
          LibPutFarData(150,I_YDEBINFO+2+I_YINFOSIZE*iIncY,187);
          if (bLate == 1)
            LibPutGraph( 150, 11+I_YDEBINFO+2+I_YINFOSIZE*iIncY, bmpCalendLate );
          break;
        case 3: /* Show task and remind */
          PVTodayGetTask(pszDate,szInfoOwner);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          LibPutFarData(150,I_YDEBINFO+2+I_YINFOSIZE*iIncY,187);
          PVTodayGetRemind(pszDate,szInfoOwner);
          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+11+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          LibPutFarData(150,I_YDEBINFO+2+11+I_YINFOSIZE*iIncY,187);
          break;
        case 4: /* Show mail */
          /* Support only for PVS-750 il this mail support is selected */
          if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] == 1)
          {
            if (LibGetModelType() == 14392)
            {
              PVTodayGetMail(szInfoOwner);
            }
            else
              strcpy(szInfoOwner,SZ_MAILOPENERR1);
          }
          else
          if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] == 2)
          {
            PVTodayGetMail(szInfoOwner);
          }
          else
            strcpy(szInfoOwner,SZ_MAILOPENERR2);

          LibPutProStr(IB_PFONT1,20,I_YDEBINFO+2+I_YINFOSIZE*iIncY,(byte *) szInfoOwner,139);
          break;
        case 5: /* Show memory status  */
          DispBatt();
          break;
      }
      /* Prepare next row */
      LibPutLineSub(0,I_YDEBINFO+I_YINFOSIZE*(iIncY+1),159-1,I_YDEBINFO+I_YINFOSIZE*(iIncY+1),IB_GDS_MESH );
      iIncY++;
    }
  }

  /* Really show infos */
  LibPutDisp();
}

/******************************************************************************
*  [Title]              PVTodayShowLstTasks
*  [Arguments]          None
*  [ReturnVAL]		    None
*******************************************************************************/
static void PVTodayShowLstTasks(char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE, byte bStart, bool *bAffVal)
{
  byte bBcl=0,bNbVal=0;	
  FILE_BUF fb;
  FILE_INFO finf;
  int iBcl;
  char szScheDate[8+1], szDispTask[70+1];

  /* Init variables */
  szScheDate[8]='\0';
  *bAffVal=FALSE;
  LibClrBox(STARTX+1, STARTY+1, XSIZE-1, YSIZE-1);

  /* for all rank */
  for (iBcl=0;((iBcl<3) && (bNbVal<4));iBcl++)
  {
    /* Get infos from tasks , and Common sub mode */
    fb.fsb_main_entry_ = 0x40;
    fb.fsb_scrt_info_ = 0x80;
    finf.fp = 0xffff;  /* Search from top */
    finf.kind = FILE_KIND_TEXT /*FILE_KIND_BIN*/;
    fb.fsb_sub_entry_ = 0x08;
    fb.fsb_todo_chek_ = 0x04; /* Uncheck task only */

    /* Init initial date search */
    memcpy(fb.fsb_date_srch_,pszDateAct,8);

    if (iBcl==2) {iBcl=3;}     /* no mode == 2, only 0,1,3 for C,B,A */
    fb.fsb_todo_rank_ = iBcl; /* Check actual rank */
    /* Specify searching by date 0x10 in LibFileFindNext */
    while ((LibFileFindNextExt(&fb,&finf,0x10) != FALSE) && (bNbVal<4))
    {
      if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
        LibPutMsgDlg((byte *) "Read failed");
      else
      {
        /* if date is correct */
        memcpy(szScheDate,fb.fsb_date1_buf_,8);
        if (strcmp(pszDateAct,szScheDate) == 0)
        {
          /* Get Schedule if we are in the correct interval */
          bBcl++;
          if (bBcl>bStart)
          {
  	         /* Take care about displaying more than 4 lines */
             *bAffVal=TRUE;
	         bNbVal++;
	         sprintf(szDispTask,"%-70.70s",fb.fbuf.text.fsb_text_buf_);
             LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2+(bNbVal-1)*11,(byte *) szDispTask,130);
          }
        }
      }
    }
  }
  if (*bAffVal == FALSE)
    LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2,(byte *) SZ_NODISPLAY,130);
}

/******************************************************************************
*  [Title]              PVTodayShowLstSchedule
*  [Arguments]          None
*  [ReturnVAL]		    None
*******************************************************************************/
static void PVTodayShowLstSchedule(char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE, byte bStart, bool *bAffVal)
{
  byte bBcl=0,bNbVal=0;	
  FILE_BUF fb;
  FILE_INFO finf;
  char szScheDate[8+1], szDispTask[70+1];
  
  /* Get infos from schedule , and Common sub mode */
  fb.fsb_main_entry_ = 0x40;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xffff;  /* Search from top */
  finf.kind = FILE_KIND_TEXT /*FILE_KIND_BIN*/;
  fb.fsb_sub_entry_ = 0x01;  /* 0x06 schedule information */

  /* Init initial date search */
  memcpy(fb.fsb_date_srch_,pszDateAct,8);
  *bAffVal=FALSE;
  szScheDate[8]='\0';
  LibClrBox(STARTX+1, STARTY+1, XSIZE-1, YSIZE-1);

  /* Specify searching by date 0x10 in LibFileFindNext */
  while ((LibFileFindNext(&fb,&finf,0x10) != FALSE) && (bNbVal<4))
  {
    if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
      LibPutMsgDlg((byte *) "Read failed");
    else
    {
      /* if date is correct */
      memcpy(szScheDate,fb.fsb_date1_buf_,8);
      if (strcmp(pszDateAct,szScheDate) == 0)
      {
        /* Get Schedule if we are in the correct interval */
        bBcl++;
        if (bBcl>bStart)
        {
	       /* Take care about displaying more than 4 lines */
           *bAffVal=TRUE;
	       bNbVal++;
	       sprintf(szDispTask,"%2.2s:%2.2s - %-50.50s",fb.fsb_time1_buf_,fb.fsb_time1_buf_+2,fb.fbuf.text.fsb_text_buf_);
           LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2+(bNbVal-1)*11,(byte *) szDispTask,130);
        }
      }  
    }
  }
  if (*bAffVal == FALSE)
    LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2,(byte *) SZ_NODISPLAY,130);
}

/******************************************************************************
*  [Title]              PVTodayShowLstTasks
*  [Arguments]          None
*  [ReturnVAL]		    None
*******************************************************************************/
static void PVTodayShowLstReminds(char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE, byte bStart, bool *bAffVal)
{
  byte bBcl=0,bNbVal=0;	
  FILE_BUF fb;
  FILE_INFO finf;
  char szScheDate[8+1], szDispTask[70+1];

  /* Init variables */
  szScheDate[8]='\0';
  *bAffVal=FALSE;
  LibClrBox(STARTX+1, STARTY+1, XSIZE-1, YSIZE-1);

  /* Get infos from tasks , and Common sub mode */
  fb.fsb_main_entry_ = 0x40;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xffff;  /* Search from top */
  finf.kind = FILE_KIND_TEXT;
  fb.fsb_sub_entry_ = 0x03;
    
  /* Init initial date search */
  memcpy(fb.fsb_date_srch_,pszDateAct,8);

  /* Specify searching by date 0x10 in LibFileFindNext */
  while ((LibFileFindNextExt(&fb,&finf,0x10) != FALSE) && (bNbVal<4))
  {
    if (LibFileRead(&fb,&finf)!=TRUE)    /* Read */
      LibPutMsgDlg((byte *) "Read failed");
    else
    {
      /* if date is correct */
      memcpy(szScheDate,fb.fsb_date1_buf_,8);
      if (strcmp(pszDateAct,szScheDate) == 0)
      {
        /* Get Schedule if we are in the correct interval */
        bBcl++;
        if (bBcl>bStart)
        {
          /* Take care about displaying more than 4 lines */
          *bAffVal=TRUE;
          bNbVal++;
          sprintf(szDispTask,"%-70.70s",fb.fbuf.text.fsb_text_buf_);
          LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2+(bNbVal-1)*11,(byte *) szDispTask,130);
        }
      }
    }
  }
  if (*bAffVal == FALSE)
    LibPutProStr(IB_PFONT1,STARTX+2, STARTY+2,(byte *) SZ_NODISPLAY,130);
}

/******************************************************************************
*  [Title]              PVTodayListTaskOfDay
*  [Arguments]          None
*  [ReturnVAL]		    None
*******************************************************************************/
static void  PVTodayListTaskOfDay(byte bTypeList, char *pszDateAct, int STARTX, int STARTY, int XSIZE, int YSIZE)
{
  TCHSTS tsts;
  bool end_flag = FALSE, bOk;
  byte bNbVal=0;

  /* touch area */
  TchLstSelWinMail[0].x1=STARTX+1;
  TchLstSelWinMail[0].y1=STARTY+1;
  TchLstSelWinMail[0].x2=STARTX+XSIZE;
  TchLstSelWinMail[0].y2=STARTY+YSIZE;

  /* set up & down flag and current position */
  LibOpenWindow(STARTX, STARTY, XSIZE+2+2, YSIZE+2+2);

  /* display background */
  LibGdsLine(STARTX, STARTY+1, STARTX, STARTY+YSIZE+2-1);  /*left line */
  LibGdsLine(STARTX+1, STARTY, STARTX+XSIZE+2-1, STARTY);  /* up line */
  LibGdsLine(STARTX+1, STARTY+YSIZE+2, STARTX+XSIZE+2-1, STARTY+YSIZE+2);      /* upderline */
  LibGdsLine(STARTX+2, STARTY+YSIZE+2+1, STARTX+XSIZE+2-2, STARTY+YSIZE+2+1);
  LibGdsLine(STARTX+XSIZE+2, STARTY+1, STARTX+XSIZE+2, STARTY+YSIZE+2-1);      /* right line */
  LibGdsLine(STARTX+XSIZE+2+1, STARTY+2, STARTX+XSIZE+2+1, STARTY+YSIZE+2-2);
  if (bTypeList==0)
    PVTodayShowLstSchedule(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, 0, &bOk);
  else
  if (bTypeList==1)
    PVTodayShowLstTasks(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, 0, &bOk);
  else
    PVTodayShowLstReminds(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, 0, &bOk);
  if (bOk)
    LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,184);
  else
    LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,183);  
  LibPutFarData(STARTX+XSIZE-7-1,STARTY+2,256);
  LibPutDisp();

  /* Init touchs */
  LibTchStackPush(NULL);
  LibTchStackPush(TchLstSelWinMail);
  LibTchInit();

  /* While true */
  while(end_flag == FALSE)
  {
    /* Wait for a touch */
    LibTchWait( &tsts );
    switch( tsts.obj )
    {
      /* Icon select */
      case  OBJ_AREA:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        /* Test if down pressed */
        if (bOk && ((tsts.x>STARTX+XSIZE-7-1) && (tsts.y>STARTY+YSIZE-8-1)))
        {
          bNbVal+=4;
          if (bTypeList==0)
            PVTodayShowLstSchedule(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          else
          if (bTypeList==1)
            PVTodayShowLstTasks(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          else
            PVTodayShowLstReminds(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          LibPutFarData(STARTX+XSIZE-7-1,STARTY+2,257);
          if (bOk)
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,184);
          else
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,183);  
          LibPutDisp();
        }
        else
        /* Test if up pressed */
        if ((bNbVal>0) && ((tsts.x>STARTX+XSIZE-7-1) && (tsts.y<STARTY+8+1)))
        {
          bNbVal-=4;
          if (bTypeList==0)
            PVTodayShowLstSchedule(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          else
          if (bTypeList==1)
            PVTodayShowLstTasks(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          else
            PVTodayShowLstReminds(pszDateAct, STARTX, STARTY, XSIZE, YSIZE, bNbVal, &bOk);
          if (bNbVal>0)
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+2,257);
          else
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+2,256);
          if (bOk)
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,184);
          else
            LibPutFarData(STARTX+XSIZE-7-1,STARTY+YSIZE-8-1,183);  
          LibPutDisp();
        }
        else
          end_flag = TRUE;
        break;
      default:
         LibRepOff();
         break;
    }
  }

  /* Free touch buffer, close window and redraw screen */
  while (LibTchStackPop()!=NULL)
    LibTchInit();
  LibCloseWindow();
  LibPutDisp();
}

/******************************************************************************
*  [Title]		StrToNum
*  [Arguments]          dt_buff : output buffer with date converted,
*                       buff : input buffer with char date
*  [ReturnVAL]		None
*******************************************************************************/
static void StrToNum(word *dt_buff, byte *buff)
{
  int i;
  word	par;
  byte	num;

  par = 1000;
  dt_buff[0]=0;
  for(i=0;i<4;i++)
  {
    num = buff[i];
    if(num == 0)
      num = 0x30;
    dt_buff[0] += (num-0x30)*par;
    par /= 10;
  }
  par = 10;
  dt_buff[1]=0;
  for(i=0;i<2;i++)
  {
    num = buff[i+4];
    if(num == 0)
      num = 0x30;
    dt_buff[1] += (num-0x30)*par;
    par /= 10;
  }
  par = 10;
  dt_buff[2]=0;
  for(i=0;i<2;i++)
  {
    num = buff[i+6];
    if(num == 0)
      num = 0x30;
    dt_buff[2] += (num-0x30)*par;
    par /= 10;
  }
}

/******************************************************************************
*  [Title]		PVToday(main)
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void)
{
  TCHSTS tsts;
  bool   bLoopEnd,bOK;
  int    iBcl;
  int    iNbRefresh=0,iNbRef2=0;
  char   szDayAct[8+1];                      /* Current day */
  byte   bName[16];
  byte   bSubMode;
  word	date_b[6];

  /* Initial */
  PVTodayInit();

  /* Invert the box of the current day */
  bActualDay = TRUE;
  LibGetDate2(&wYearGen,&bMonthGen,&bDayGen);
  sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
  DispInvertDay((byte *) szDayAct);

  /* Display infos and Init touch */
  PVTodayDisplayInfo(szDayAct);
  LibTchInit();

  /* While allway ...*/
  bLoopEnd = FALSE;
  while(bLoopEnd==FALSE)
  {
    LibTchWait( &tsts );
    if ( tsts.ext==EXT_CODE_500MS )
    {
      if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[0]] == 1) { DispTimer(); }
      iNbRefresh++;
      iNbRef2++;
      if (iNbRef2>60)
      {
        iNbRef2 = 0;
        /* V2.1.1 Stupid bug ! */
        if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[5]] == 1) { DispBatt(); }
      }
      if (iNbRefresh>2*60*5)
      {
        iNbRefresh=0;
        PVTodayDisplayInfo(szDayAct);
      }
    }
    switch( tsts.obj )
    {
      /* V2.0 11/08/2002 Previous day button */
      case OBJ_IC_PREVDAY:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        DispInvertDay((byte *) szDayAct);
        bActualDay = PVTodayCalcDay(&wYearGen, &bMonthGen, &bDayGen, -7);
        sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
        DispInvertDay((byte *) szDayAct);
        PVTodayDisplayInfo(szDayAct);
        break;
      /* V2.0 11/08/2002 Next day button */
      case OBJ_IC_NEXTDAY:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        DispInvertDay((byte *) szDayAct);
        bActualDay = PVTodayCalcDay(&wYearGen, &bMonthGen, &bDayGen, +7);
        sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
        DispInvertDay((byte *) szDayAct);
        PVTodayDisplayInfo(szDayAct);
        break;
      /* V2.0 11/08/2002 Day of week button */
      case OBJ_IC_CHANDAY:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        DispInvertDay((byte *) szDayAct);
        iBcl = ((tsts.x-(2+10+50+10+3+7+0*10+1)) / 10)-LibGetDow((byte *) szDayAct);
        bActualDay = PVTodayCalcDay(&wYearGen, &bMonthGen, &bDayGen, iBcl);
        sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
        DispInvertDay((byte *) szDayAct);
        PVTodayDisplayInfo(szDayAct);
        break;
      /* V1.2 06/05/2002 Refresh button */
      case OBJ_IC_REFRESH:
        if ( (tsts.act & ACT_MAKE) == 0x00000000L) break;
        DispInvertDay((byte *) szDayAct);
        bActualDay = TRUE;
        LibGetDate2(&wYearGen,&bMonthGen,&bDayGen);
        sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
        DispInvertDay((byte *) szDayAct);
        PVTodayDisplayInfo(szDayAct);
        break;
      /* If click on menu option, show Options available */
      case OBJ_HIC_MBAR:
        PVTodayPullDownSetUp(); /* PullDownMenu */
        if( PVTodayPullDownMenu() == TRUE )
          LibTchInit();
        PVTodayDisplayInfo(szDayAct);
        break;
      /* If click on a task, Jump to specific task */
      case OBJ_IC_CHOICE:
        iBcl = ((tsts.y-I_YDEBINFO) / I_YINFOSIZE);
        if (iBcl >= I_MAXINFO-1) { iBcl = I_MAXINFO-1; }
        switch (blnInfoHere[iBcl])
        {
          case 0:
            /* Cancel invert box, change date and reinvert the new day */
            DispInvertDay((byte *) szDayAct);
            LibFuncDateTime();
            bActualDay = TRUE;
            LibGetDate2(&wYearGen,&bMonthGen,&bDayGen);
            sprintf(szDayAct,"%04d%02d%02d",wYearGen,bMonthGen,bDayGen);
            DispInvertDay((byte*) szDayAct);
            PVTodayDisplayInfo(szDayAct);
            break;
          case 1:
            PVTodayShowPersonnal();
            PVTodayDisplayInfo(szDayAct);
            break;
          case 2:
            /* IF show the whole task day is clicked */
            if ((tsts.x>150) && (tsts.y-I_YDEBINFO-iBcl*I_YINFOSIZE<11))
            {
              /* DIsplay alls the schedule of the day */
              PVTodayListTaskOfDay(0,szDayAct,4, tsts.y-I_YDEBINFO, 150, 11*4);
            }
            /* ELSE */
            else
            {
              /* Change the day to the select one */
              StrToNum( date_b,(byte *) szDayAct);
              far_memmove(&SYS_CLD_DATE, date_b, 6);
              /* Jump to shedule mode */
              LibModeJump(IW_MSCHD | IB_SMSCHD_DAY,0x00);
            }
            break;
          case 3:
            /* IF show the whole task/reminds day is clicked */
            if (tsts.x>150)
            {
              if ((tsts.y-I_YDEBINFO-iBcl*I_YINFOSIZE)<11)
                /* DIsplay alls the tasks */
                PVTodayListTaskOfDay(1,szDayAct,4, tsts.y-I_YDEBINFO, 150, 11*4);
              else
              if ((tsts.y-11-I_YDEBINFO-iBcl*I_YINFOSIZE)<11)
                /* DIsplay alls the reminds */
                PVTodayListTaskOfDay(2,szDayAct,4, tsts.y-I_YDEBINFO, 150, 11*4);
            }
            /* ELSE */                
            else
            {
              if ((tsts.y-I_YDEBINFO-iBcl*I_YINFOSIZE)<11)
                /* Jump to todo task mode */
                LibModeJump(IW_MSCHD | IB_SMTODO,0x00);
              else
                /* Jump to reminds mode */
                LibModeJump(IW_MSCHD | IB_SMREMI,0x00);
            }
            break;
          case 4:
            /* If we want the PV-750 mail */
            if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] == 1)
            {
              /* Support only for PVS-750 */
              if (LibGetModelType() == 14392)
              {
                LibModeJump(IW_MMAIL | IB_SMMAIL_OUTBOX,0x00);
              }
            }
            /* Other is PV Mail 2.0.4 only ;-( */
            else
            if (stPVTodayRecord.blnInfoShow[stPVTodayRecord.bInfoOrder[4]] == 2)
            {
              bOK=FALSE;
              for (bSubMode=1;bSubMode<16;++bSubMode)
              {
                if ((LibGetProgramName(bName,IB_MADDIN,bSubMode)) && (far_strcmp("PVMail",bName) == 0))
                {
                  bOK = TRUE;
                  LibModeJump(IW_MADDIN + bSubMode,0x00);
                }
              }
              if (!bOK)
                PVGraphPutMsgDlg(SZ_MAILOPENERR3);
            }
            else
            {
              PVGraphPutMsgDlg(SZ_MAILOPENERR1);
            }
            break;
          case 5: /* Nothing to do with the system representation */
            break;
          default:
            break;
        }
        break;
      case OBJ_HIC_ESC:
        bLoopEnd = TRUE;
        break;
      default:
        LibRepOff();
        break;
    }
  }
  LibJumpMenu();
}