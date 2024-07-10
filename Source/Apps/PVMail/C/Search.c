/******************************************************************************
*	
*	[Title]	 Routines for mail Edit-mode
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

#include    <stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_libc.h"

#include	"Common.h"
#include    "Graphics.h"
#include	"Flash.h"
#include	"PVMail.h"


/*** Structures ***/

/* Main window touch areas */
static TCHTBL far TchSearchViewIcon[] = {
	71, 0, 99, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_CANCEL,
	0x0000,

	100, 0, 126, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_OK,
	0x0000,
	
	53, 74, 63, 85,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_SRCH_BYITEM,
	0x0000,

	108, 74, 118, 85,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_SRCH_ALL,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};
static T_ICON far TiconCanc  = { &TchSearchViewIcon[0], NULL, NULL, 0x00 };
static T_ICON far TiconOK = { &TchSearchViewIcon[1], NULL, NULL, 0x00 };
static T_ICON far TiconSrchByItem  = { &TchSearchViewIcon[2], NULL, NULL, 0x00 };
static T_ICON far TiconSrchAll = { &TchSearchViewIcon[3], NULL, NULL, 0x00 };

static word xFFFE = 0xfffe;

extern TXTP        txtBox;
extern SLW_TBL    *ktb_s;
extern BK_SMPL_TCH *bk;
extern byte       *MailFileName;
extern FILE_BUF    mail_fb;
extern TPvFile     hMailFile;
extern int         CurMail;     /* Mail message currently selected from list */
extern int         MailNum; /* Number of mail messages loaded in the list */
extern int         SourceMail;
extern byte        MailBox;  /* Current mail box (In / Out) */

extern TCache      Cache;
extern TMailRecord MailRecord;

extern TCHTBL     *TchTxtBuf;  /* Text touch area table */
extern T_SCR_POS   ComScr;     /* Scroll bar position information */
extern byte *keywd_0;
extern byte *keywd_1;
extern byte *keywd_2;

/* Used in searching */
extern byte  SearchMsg[];  /* Global buffer to keep the search string */
extern int   SearchMode;   /* SRCH_ALL or SRCH_ITEM - PRESENTLY DISABLED */

/************ Function prototypes *************/
/* Main menu */
extern void GmPdwnSet_Edit(void);
extern bool GmPdwnFnc();
extern bool PdwnFncSys(word sel);
extern bool PdwnFncEdt(word sel);
extern void SplitItems(byte *m, TMailMessageItems *mi);
extern void PVMailTitle(void);
extern void TxtItemIconDsp(TXTP *txt);
extern bool NewMessage(int new_type);

/**********************************************************
*  [Title] InitSearchBuffer
*  [Descr] Reset Text buffer used by search
**********************************************************/
void InitSearchBuffer(void){

	if (SearchMode == SRCH_ITEM)
		sprintf(SearchMsg,
		    "\x0FE" /* Subject */
            "\x0FE" /* From */
            "\x0FE" /* To */
            "\x0FE" /* Body */
            "\x0FE" /* Date */
            "\x0FE" /* Folder */
            "\x0FE" /* CC */
            "\x000" /* BCC */ );
	else
		SearchMsg[0]='\0';

}

/**********************************************************
*  [Title] InitTxtSearch
*  [Descr] Setup initial values for the preview txt box
**********************************************************/
static void InitTxtSearch(void)
{
	txtBox.st_x = TXTSEARCH_XLEFT; /* Start (X) of text display */
	txtBox.st_y = TXTSEARCH_YTOP;  /* Start  (Y) of text display */
	txtBox.ed_x = LCD_X_DOT - TXTSCROLLBARWIDTH ; /* End (X) of text display */
	txtBox.it_y = TXTSEARCH_ROWHEIGHT; /* Text display line spacing (Y) */
	txtBox.MAXGYO = TXTSEARCH_ROWNUM;  /* Number of text display lines */
	txtBox.font = IB_PFONT1;     /* Font type */
	txtBox.csen = TRUE;          /* Cursor enabled */
	txtBox.rtnen = TRUE;         /* CR code display enabled */
	txtBox.maxmj = MAIL_SEARCH_SIZE - 1; /* Maximum number of allowable characters */
	txtBox.txtobj = OBJ_TXTTCH;  /* Object when text area is touched */
	txtBox.sbrobj = OBJ_SCR_BAR; /* No scroll bar */
	txtBox.tchtb = TchTxtBuf;    /* Pointer for text scroll bar area */
	txtBox.s_pos = &ComScr;      /* Pointer for text and scroll bar information */
	txtBox.kwb_0 = keywd_0;      /* Keyword saving buffer 0 */
	txtBox.kwb_1 = keywd_1;      /* Keyword saving buffer 1 */
	txtBox.kwb_2 = keywd_2;      /* Keyword saving buffer 2 */
	txtBox.gdcmt = &xFFFE;       /* No guidance table  */
	if (SearchMode == SRCH_ITEM)   /* Guidance comment strings */
		txtBox.gdcmt2 = TXT_GUIDE;
	else
		txtBox.gdcmt2 = TXT_GUIDE_SRCH;

}

/**********************************************************
*  [Title] DrawtxtBox
*  [Descr] Show a display-only txt box based on 'data'
**********************************************************/
static void DrawtxtBox(byte * data)
{
    
    /* Set the text buffer pointer to actual data string */
	txtBox.txbf = data;
	
    /* Draw text box and scroll bar frames */
    LibClrBox(0, TXTSEARCH_YTOP - 2,  
        LCD_X_DOT, TXTSEARCH_YHEIGHT + 3);
    LibBox(0, TXTSEARCH_YTOP - 2,
        LCD_X_DOT, TXTSEARCH_YHEIGHT + 3, 1);
	LibPutDisp();
	LibLine(LCD_X_DOT - TXTSCROLLBARWIDTH + 1 , TXTSEARCH_YTOP - 1,
	    1, TXTSEARCH_YHEIGHT + 2, 1);

    /* (Re)-initialization of the text-box data */
	LibTxtInit(&txtBox);
    if(LibTxtDsp(&txtBox) == TRUE)
        LibPutDisp();   
}


/******************************************************************************
*	[Title]		TxtItemIconDspSrch
*   [Descr]     Draw the text-box icon
******************************************************************************/
static void TxtItemIconDspSrch(TXTP *txt){
    LibClrBox( txt->st_x - TXTICON_WIDTH + 1, txt->st_y - 1,  
    	TXTICON_WIDTH - 1, txt->it_y*txt->MAXGYO);
	LibPutGraph(txt->st_x - TXTICON_WIDTH + 1, txt->st_y - 1,
		SearchIcon);
}

/******************************************************************************
*	[Title]		EditInit
*               Init Display-mode screen
******************************************************************************/
static void SearchInit(void){

	LibClrDisp();
	PVMailTitle();	

	/* Buttons */
	LibPutFarData(71,0,149); /* DUE PULSANTI */
	LibPutProStr(IB_PFONT1,76,2,"Canc",30);	
	LibPutProStr(IB_PFONT1,104,2,"Srch",30);
	
	/* mail box label */
	if (MAIL_INBOX == MailBox)
    	LibPutProStr(IB_PFONT1,1,16,"Search in the 'Inbox' list",150);
	else
	    LibPutProStr(IB_PFONT1,1,16,"Search in the 'Outbox' list",150);
	
			/**********************
			MODE-SWITCH DISABLED 
	/^ Radio-buttons, and labels ^/
	if (SearchMode == SRCH_ITEM){
		LibPutFarData(54, 75,12);
		LibPutFarData(109,75,13);
	} else {
		LibPutFarData(54, 75,13);
		LibPutFarData(109,75,12);
	}
	LibPutProStr(IB_PFONT1,65, 75,"By Item",40);	
	LibPutProStr(IB_PFONT1,120,75,"All msg",40);
             *********************/
    	
	/* Init and show mail text-box and keyboard */
	InitTxtSearch();
	DrawtxtBox(SearchMsg /* Cache.Data */);
	if (SearchMode == SRCH_ITEM)
	    TxtItemIconDsp(&txtBox);
	else
		TxtItemIconDspSrch(&txtBox);
	LibKeyInit(); /* Initializes keyboard */
    LibDispKey(); /* Keyboard display */ 
	LibPutDisp();

}

/******************************************************************************
*	[Title]		EditTouchSet
*   [Descr]     Init touch info
******************************************************************************/
static void SearchTouchSet(void){

	/* Touch init */
	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTxtTchSet(&txtBox);
	LibTchStackPush( TchHardIcon );
    LibTchStackPush( TchActionKey );	
	LibTchStackPush( TchSearchViewIcon );

}


/******************************************************************************
*  [Title]       SearchLoop
*  [Desc]        Main loop for entering the search string
*                Returns TRUE when a search is to be started 
*                (which is actually performed in the calling function)
*******************************************************************************/
int SearchLoop(void){
    TCHSTS	tsts;
    byte    keycd;

    /* Clear screen and init objects for the 'Search' view */
    SearchInit();
    SearchTouchSet();

	for(;;){

        /* Text-box update */
		if(LibTxtDsp(&txtBox) == TRUE){
			if (SearchMode == SRCH_ITEM)
			    TxtItemIconDsp(&txtBox);
			else
				TxtItemIconDspSrch(&txtBox);
            LibPutDisp();
	    }
		
        keycd = LibGetKeyM(&tsts); /* Waiting for software key */
	
        if(keycd==KEY_NONE){
		    switch( tsts.obj ) {
		
		/** ACTION KEY & ESCAPE**/
		/*
		    case OBJ_LPSW_UP:
		    case OBJ_LPSW_DOWN:
		*/
		    case OBJ_HIC_ESC:
			case OBJ_LPSW_PUSH:
			    return FALSE;
			    break;
			    
		/** BUTTONS **/
			case  OBJ_IC_CANCEL:
				if(LibBlockIconClick( &TiconCanc, &tsts , B_ICON_RIGHT ) == TRUE)
					/* Go back to list-view mode and do nothing */
					return FALSE;
				break;

			case  OBJ_IC_OK:
				if(LibBlockIconClick( &TiconOK, &tsts , B_ICON_RIGHT ) == TRUE){
				    /* Go back to the calling routine, 
				       which will actually perform search */
				    strcpy(SearchMsg, txtBox.txbf);
				    return TRUE;
    			}
			    break;

			/**********************
			MODE-SWITCH DISABLED 
			case  OBJ_IC_SRCH_BYITEM:
				if (SearchMode != SRCH_ITEM)
					if(LibIconClick( &TiconSrchByItem, &tsts ) == TRUE){
						SearchMode = SRCH_ITEM;
						InitSearchBuffer();
					    SearchInit();
					    SearchTouchSet();
			    	}
			 	break;	
			case  OBJ_IC_SRCH_ALL:
				if (SearchMode != SRCH_ALL)
					if(LibIconClick2( &TiconSrchAll, &tsts ) == TRUE){
						SearchMode = SRCH_ALL;
						InitSearchBuffer();
					    SearchInit();
					    SearchTouchSet();
				    }
				break;
		   *************************/

		/** PULL-DOWN MENU-BAR ESCLUSO ^^/
			case OBJ_HIC_MBAR:
				GmPdwnSet_Edit();
				if( GmPdwnFnc() == TRUE ){
					EditInit();
					LibTchInit();
					return;
				};
				break;
		**/

		    default:
			    break;
		    }
		}
		
		/* Text-box touch processing */
        LibTxtInp(keycd,&tsts,&txtBox);

		if(tsts.act == ACT_ALM)
			LibAlarm();	/* AlarmProc */
	}

}

