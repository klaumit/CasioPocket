/******************************************************************************
*	
*	[Title]	        File Save
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.00
*
*	Copyright (C) 1997 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/

#include    <stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"Common.h"
#include	"Serial.h"
#include	"Flash.h"
#include	"ListBox.h"
#include	"Trm.h"
#include	"FileSave.h"

/*** Structures ***/

/* Main window touch areas */
TCHTBL far TchHeaderIcon[] = {
	71, 0, 98, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_SEND,
	0x0000,

	100, 0, 126, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_RECEIVE,
	0x0000,

	128, 0, 154, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_DELETE,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconSend = { &TchHeaderIcon[0], NULL, NULL, 0x00 };
T_ICON far TiconReceive = { &TchHeaderIcon[1], NULL, NULL, 0x00 };
T_ICON far TiconDelete = { &TchHeaderIcon[2], NULL, NULL, 0x00 };

/* Message-Box touch areas */
TCHTBL far TchMsg[] = {
	106, 137, 134, 151,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_MSG_OK,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconMsgOk = { &TchMsg[0], NULL, NULL, 0x02 };

/* List-Box touch areas */
static TCHTBL far tchList[] = {
	TCHTBL_LB(LB_X,LB_Y,LB_W,LB_IH,LB_IPP,LB_LT,OBJ_LB),
	TCHTBL_END
};

/*****************************************************************************/
word	MPdEdt[] ={
			PDNTBLEND,
		};

word	MPdSys[] = {
			38,			/* Set_date/time		*/
			33,			/* Sound				*/
			39,			/* Format				*/
			34,			/* Capacity				*/
			35,			/* Memory_management	*/
/*			36,			/* Language				*/
			37,			/* Contrast				*/
			40,			/* Touch_Panel_Alignment*/
/*			41,			/* Data_communication	*/
			PDNTBLEND,
		};

word	MPdOpt[] = {
			PDNTBLEND,
		};

SLW_TBL ktb_s[3] = {{312,0},{313,1},{314,2}};


/*************** Global Variables ************************/
byte far SIcon[] = {
	GSIZE(12, 12),
	0xC0, 0x1F,
	0x20, 0x0F,
	0xA3, 0xEF,
	0xA4, 0x1F,
	0xA5, 0xDF,
	0xBD, 0xDF,
	0xBD, 0xDF,
	0x25, 0xDF,
	0xC4, 0x1F,
	0x84, 0x1F,
	0xF3, 0xEF,
	0x10, 0x0F,
};

FILE_BUF   fb;           /* Flash file read/write buffer */
FILE_INFO  fi;           /* keeps the pointer to current file position */
FILE_BUF   idx_fb;       /* Flash INDEX file read/write buffer */
FILE_INFO  idx_fi;       /* keeps the pointer to current INDEX file position */
TPvFile    hStorage;     /* Storage Flash file handle */
TPvFile    hIndex;       /* Index Flash file handle */
/* word       DataFP[MAX_RECORD];  /^ Array of saved records (for random access) */
TFileIndex Header;       /* File header */
TRecord    Record;       /* Single file record */
TFileIndex IndexArray[MAX_FILES];   /* Array of indexes to files */
TIndexList *IndexList;  /**** NOT USED !!! ****/

BK_SMPL_TCH bk[2];            /* ESC touch icon */
byte  block[BLOCK_SIZE + 1];  /* Block read from the serial port */
long  FileSize;        /* Total file size to download */
long  totlen = 0;      /* Total size currently downloaded */
int   blocklen = 0;    /* Lenght of the currently downloaded block */
byte  FileName[] = FILE_NAME;
byte  IndexName[] = INDEX_NAME;
char  ProgTitle[] = "FileSave"; /* Program name */
int   CurFile = 0;              /* File currently selected from list */
int   FileNum = 0;              /* Number of files loaded in the list */
static TListBox lb;

/******** Local function prototypes ************/
static void DspFileOpenErr(void);
void GmPdwnSet(void);
bool PdwnFncSys(word sel);
bool GmPdwnFnc();
void WaitOKPress(void);

static void drawInfo();
int  FileSend(void);
int  FileReceive(void);
void FileSaveInit(void);
void FileSaveTouchSet(void);
int  LoadList(void);
void ShowList(void);
void FreeList(TIndexList  *list);
TIndexList  * GetListItemIdx(TIndexList  *list, int i);

/******************************************************************************
*  [Title]			File Save (MAIN)
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void) {
    TCHSTS	tsts;
    bool    temp;
    byte    buf[256];

    /* Clear screen and init objects on the main window */
    FileSaveInit();
    FileSaveTouchSet();
    LibTchInit();

	for(;;){

		LibTchWait( &tsts );

		switch( tsts.obj ) {
		
		/** BUTTONS **/

			case  OBJ_IC_RECEIVE:
				if(LibBlockIconClick( &TiconReceive, &tsts , B_ICON_RIGHT ) == TRUE){
					/* Receive and save a file */
					FileReceive();

					FileSaveTouchSet();
					LibTchInit();
				}
			    break;

			case  OBJ_IC_SEND:
				if(LibBlockIconClick( &TiconSend, &tsts , B_ICON_RIGHT ) == TRUE){
					/* Read and send a file */
					FileSend();
					
					FileSaveTouchSet();
					LibTchInit();
				}
			    break;

		/** LIST-BOX Events **/
            case OBJ_LB:
		        temp = LBClicked(&tsts,&lb);
		        if (temp == LB_UP || temp == LB_DOWN || temp == LB_MOVE) {
                    CurFile = getLBCurIdx(&lb);
			        drawInfo();
		        } else if (temp == LB_SELECT) {
                    sprintf(buf,"NOP");
                    /*
			        gotoDisplayMode();
			        retcode = -1;
                    */
		        }
		        break;

            case OBJ_LPSW_UP:
		        if (scrollListBox(&lb,LB_UP) != -1) {
                    CurFile = getLBCurIdx(&lb);
			        drawInfo();
		        }
		        break;

	        case OBJ_LPSW_DOWN:
		        if (scrollListBox(&lb,LB_DOWN) != -1) {
                    CurFile = getLBCurIdx(&lb);
			        drawInfo();
		        }
		        break;

	        case OBJ_LPSW_PUSH:
                /*
		        if (MovieNum > 0) {
			        gotoDisplayMode();
			        retcode = -1;
		        }
                */
                sprintf(buf,"NOP");
		        break;

		/** MENU-BAR **/

			case OBJ_HIC_MBAR:
				GmPdwnSet();				/* PullDownMenu */
				if( GmPdwnFnc() == TRUE ){
					FileSaveInit();
					LibTchInit();
				};
				break;

		default:
			break;
		}

		if(tsts.act == ACT_ALM)
			LibAlarm();	/* AlarmProc */
	}

}

/******************************************************************************
*	[Title]		Init screen
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void FileSaveInit(void){

	LibClrDisp();

	/* Buttons */
	/* LibPutFarData(71,0,8);   /^ DUE PULSANTI */
	LibPutFarData(71,0,149); /* TRE PULSANTI */

	LibPutProStr(IB_PFONT1,74,2,"Send",30);	
	LibPutProStr(IB_PFONT1,104,2,"Recv",30);
	/* LibPutProStr(IB_PFONT1,128,2,"Del",30);	*/

    /* Load and show the list of files already loaded into system */
	ShowList();

	LibPutDisp();

}

/******************************************************************************
*	[Title]		FileSaveTouchSet
*	[Arguments]	None
*	[ReturnVAL]	None
*   [Descr]     Init touch info
******************************************************************************/
void FileSaveTouchSet(void){

	/* Touch init */
	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( TchHeaderIcon );
    LibTchStackPush( tchList );

}

/**********************************************************
* drawLBItem
* Load from mem array, and draw one list item
**********************************************************/
static void drawLBItem(const TListBox *l, const TLBItemInfo *li)
{
    int i = 0;

	LibClrBox(li->x,li->y,l->w2,l->ItemHeight);
	if (li->idx >= 0 && li->idx < FileNum) {

        LibStringDsp(IndexArray[li->idx].FileName,
            li->x+1, li->y+1, l->w2 - 2,
            IB_PFONT1);
        /*
        LibPutProStr(IB_PFONT1,li->x+1,li->y+1,
            IndexArray[li->idx].FileName,l->w2 - 2);
        */

		if (li->reverse)
			LibReverse(li->x,li->y,l->w2,l->ItemHeight);
	}
}

/**********************************************************
*  [Title] toggleLBItem
*  [Descr] Reverse selected list item
**********************************************************/
static void toggleLBItem(const TListBox *l, const TLBItemInfo *li)
{
	if (li->idx >= 0 && li->idx < FileNum) {
		LibReverse(li->x,li->y,l->w2,l->ItemHeight);
	} else {
		LibClrBox(li->x,li->y,l->w2,l->ItemHeight);
	}
}

/**********************************************************
*  [Title] drawListBG
*  [Descr] Draw list-box background
**********************************************************/
static void drawListBG()
{
 /*
    LibClrDisp();
	LibModeIconDsp();

	LibPutFarData(icNew.tch->x1,icNew.tch->y1,27);
	LibPutMessageCenter(14,icNew.tch->x1,icNew.tch->x2,
			    icNew.tch->y1+2,IB_PFONT1);
*/
	LibPutGraph(0,0,SIcon);
	LibPutProStr(IB_PFONT3,14,1,ProgTitle,114);

/*
    LibPutFarData(145,14,115); /^ Pulsante della 'combo box' ^/
	drawButtonFrame(&icDate);
	printDate();

	drawInfoFrame(L_X,L_Y,L_W,L_H);
*/

	initListBox(&lb,LB_IH,LB_IPP,lbltDot);
	lb.drawItem = drawLBItem;
	lb.toggleItem = toggleLBItem;
	lb.x = LB_X;
	lb.y = LB_Y;
	lb.w = LB_W;
}

/**********************************************************
*  [Title] drawInfo
*  [Descr] Draw information for selected LB item
**********************************************************/
static void drawInfo()
{
    int  x,y,w;

    y = L_Y + 2;
	x = L_X + 2;
	w = L_W - 4;
	LibClrBox(x, y, w, L_H-3);

	if (FileNum == 0)
		return;

    /* Centered printing of File Name
	/* p = IndexArray[CurFile].FileName;
	LibPutProStr(IB_PFONT1,
            (x + w - LibGetProStrSize(IB_PFONT1,p))/2,
            y,p,w);
	LibMeshLine(x,y + 9,w,1);			y += 11;
    */

    /* Show comment */
	LibStringDsp(IndexArray[CurFile].Comment,
            x,y,w,IB_PFONT1);	

    /* from Kino ...
    y += 9;
	w = (LCD_WIDTH-4)/3 - 6;
	for (i=0; i < TIME_PER_REC; i++) if (Cache.time[i][0] != 0xff) {
		Time2Str(Cache.time[i],str);
		printTime(str, x + 52*(i%3) + 4, y + (i/3)*9, w, IB_PFONT2);
	}
    */

	LibPutDisp();
}

/******************************************************************************
*	[Title]		ShowList
*   [Descr]		Retreive and show the list-box of files
******************************************************************************/
void ShowList(void){

	/* Open the flash index file */
	if (!initIndexFile()) {
		LibWinIcnMsg(ICON_BIKKURI,324,2);
		/* never returns */
		LibJumpMenu();
	}

    /* Retrieve index data from Flash and load an array */
	FileNum = LoadList();

    /* Draw list box background */
    drawListBG();

    /* Draw list items */
    drawListBox(&lb);
    LibPutDisp();    
    
    /* Init some LB vars */
    setLBItemsCount(&lb,FileNum);

    /* Show selected row infos */
    CurFile = getLBCurIdx(&lb);
	drawInfo();
}

/******************************************************************************
*	[Title]		LoadList
*	[Return]	Number of loaded file indexes
*	[Descr]		Load list of indexes from Flash memory
******************************************************************************/	
int LoadList(void){
    int i = 0;

	/* Load previously saved index records
       into a linked list */
	while(loadIndexRecord(&IndexArray[i]))
		i++;

    return(i);
}

/******************************************************************************
*	[Title]		Receive a File
*	[Return]	0 if no error
******************************************************************************/
int FileReceive(void){
	int retval;
	long totlen;

	if (FileNum == MAX_FILES + 1)
		return(0); /* Too many files -> no action */

	LibOpenWindow(0,0, 160, 160);

    /* 'Data transmit' message */
	LibDspWinMessage(25,388,0,IB_MWIN_NONE);
	LibPutDisp();

	/* Open Flash for the storage of received data ^/
       ALREADY DONE WHEN INDEX LIST IS LOADED
	if (!initStorageFile()) {
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return(-1);
	}

    IndexArray[FileNum + 1].Size = 0;
	IndexArray[FileNum + 1].nRecord = 0;
    IndexArray[FileNum + 1].fp = 0;
	if (!saveHeader(FileNum + 1)) {
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return(-1);
	}

    */
    
    /* Receive general info (filename, size, comment, etc.)
       about the file to be received */
	ReceiveHeader(FileNum + 1);

	totlen = 0;
	retval = 0;
	while(retval != STP){

        /* Receive one block of data */
		retval = ReceiveBlock();

		if (retval == BRK) {
			/* Stopped */
			LibDspWinMessage(11,387,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return (0);
		}
		
		if (retval != 0 && retval != STP) {
			/* Communication error */
			LibDspWinMessage(11,386,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return (-1);
		}
		
		IndexArray[FileNum + 1].Size += Record.Size;
		IndexArray[FileNum + 1].nRecord ++;
		if (IndexArray[FileNum + 1].nRecord > MAX_RECORD)
		{
			/* Communication error */
			LibDspWinMessage(11,386,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return(-1);
		}

		if (!saveRecord()){
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return(-1);
		}
	}

	/* update index with the new file */
	if (!saveHeader(FileNum + 1)) {
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return(-1);
	}

	/* Data stored */
	LibDspWinMessage(46,341,1,IB_MWIN_OK /* | IX_MWIN_CENTER */ );
	LibPutDisp();
	WaitOKPress();
	LibCloseWindow();
	/* LibCloseWindow(); this is to close the LOADING message */
	LibPutDisp();
	LibSrlPortClose();
	return (0);
}

/******************************************************************************
*	[Title]		Send selected file
*	[Return]	0 only if file was sent with no errors
******************************************************************************/
int FileSend(void) {
	int i, retval = 0;
	byte   sTerm[256];

	if (FileNum == 0)
		return(0); /* No file no action */
    
    LibOpenWindow(0,0, 160, 160);
	InitTrm();

	LibDspWinMessage(24,381,0,IB_MWIN_NONE /* | IX_MWIN_CENTER */ ); /* Wait */
	LibPutDisp();

	/* Open Flash file */
	if (PvOpenFile(FileName, &hStorage, 0) == 0)
		ASSIGN_BUF(fb,hStorage);
	else {
		/* Data not found! */
		LibDspWinMessage(11,323,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
		LibPutDisp();
		WaitOKPress();
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return (-1);
	}

	loadHeader(CurFile);

	if ((strlen(IndexArray[CurFile].FileName) == 0) ||
		(IndexArray[CurFile].Size) == 0){
		/* Data not found! */
		LibDspWinMessage(11,323,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
		LibPutDisp();
		WaitOKPress();
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return (-1);
	}

	sprintf(sTerm, "Sending...\n%s\nsize: %d\n",
		IndexArray[CurFile].FileName,
		IndexArray[CurFile].Size);
	OutputTrm(sTerm);

	LibDspWinMessage(25,388,0,IB_MWIN_NONE); /* Data transmit */
	LibPutDisp();
	/* Send to serial stream */
	if (SendHeader(CurFile)){
		LibCloseWindow();
		LibPutDisp();
		LibSrlPortClose();
		return (-1);
	}

	i = 0;
	/* Send file, block by block */
	while(1) {
	/*for (i = 1; i <= Header.nRecord; i++) { */
		i++;
		loadRecord();
		retval = SendBlock(Record.Data, Record.Size);
		
		if (retval == BRK){
			/* Stopped by the receiver */
			LibDspWinMessage(11,387,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return (-1);

		} else if (retval == NXT){
			continue; /* Next block */

		} else if ((retval == STP) && (i != Header.nRecord)){
			/* Communication error (incomplete file sent) */
			LibDspWinMessage(11,386,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return (-1);

		} else if (retval == STP){
			break; /* file finished */

		} else if (!retval) {
			/* Communication error (general error) */
			LibDspWinMessage(11,386,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
			LibPutDisp();
			WaitOKPress();
			LibCloseWindow();
			LibPutDisp();
			LibSrlPortClose();
			return (-1);
		}

	}

	/* Data stored */
	LibDspWinMessage(46,341,1,IB_MWIN_OK /* | IX_MWIN_CENTER */) ;
	LibPutDisp();
	WaitOKPress();
	LibCloseWindow();
	LibPutDisp();
	LibSrlPortClose();
	return(0);


}


/******************************************************************************
*	[Title]		File Open Error Disp
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DspFileOpenErr(void){

	LibOpenWindow((LCD_X_DOT-128)/2-1,27-1,130,52);
	LibPutFarData((LCD_X_DOT-128)/2,27,107);

	LibPutProStr(IB_PFONT1,30,35,"PV aplication memory",110);
	LibPutProStr(IB_PFONT1,30,44,"is full!",110);
	LibPutProStr(IB_PFONT1,30,53,"See your User's Guide",110);
	LibPutProStr(IB_PFONT1,30,62,"for more infomation.",110);

	LibPutDisp();
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibCloseWindow();
	LibPutDisp();
}

/******************************************************************************
*	[Title]		File Open Error Disp
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DspMsgBox(byte * line1, byte * line2, byte * line3){
	byte tmp[120];

	LibOpenWindow((LCD_X_DOT-128)/2-1,27-1,130,52);
	LibPutFarData((LCD_X_DOT-128)/2,27,107);

	LibPutProStr(IB_PFONT1,30,35,tmp,110);

	LibPutProStr(IB_PFONT1,30,44,line1,110);
	LibPutProStr(IB_PFONT1,30,53,line2,110);
	LibPutProStr(IB_PFONT1,30,62,line3,110);

	LibPutDisp();
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibCloseWindow();
	LibPutDisp();
}


/******************************************************************************
	[Title]		PullDownMenuSetup
	[Arguments]	None
	[ReturnVAL]	None
******************************************************************************/
void GmPdwnSet(void)
{
	word	p_sys;

	LibPullDownInit(MPdEdt,MPdSys,MPdOpt);		/* MenuBAR Init */

 /* [System] */
	p_sys =  PULLDOWN_SYSTEM | 0x0100;
	LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_SYSTEM, p_sys);

}

/******************************************************************************
	[Title]		PullDownMenu Functions
	[Arguments]	None
	[ReturnVAL]	bool TRUE=Redisplay
******************************************************************************/
bool GmPdwnFnc()
{
	word	sel;
	bool	jdg = FALSE;

	sel = LibPullDown();		/* PullDownMenu */
	if(sel & PULLDOWN_SYSTEM)
		jdg = PdwnFncSys(sel);
	return(jdg);
}

/******************************************************************************
	[Title]		PullDownMenuFunction(system)
	[Arguments]	word Sel(select number)
	[ReturnVAL]	bool TRUE=Rediplay
******************************************************************************/
bool PdwnFncSys(word sel)
/*TAIL*/
{
	bool	jdg = FALSE;
	switch(sel){

		case PDWN_SYS_DTTM:		/* Set_date/time		*/
			LibFuncDateTime();
			break;

		case PDWN_SYS_SOUND:	/* Sound				*/
			LibFuncSound(0);
			break;

		case PDWN_SYS_FORMT:	/* Format				*/
			LibFuncFormat();
			break;

		case PDWN_SYS_CAPCT:	/* Capacity				*/
			LibFuncCapa();
			break;

		case PDWN_SYS_MMANE:	/* Memory_management	*/
			LibFuncMemoryManagement();
			break;

/*		case PDWN_SYS_LANG:		/* Language				*/
/*			if(LibFuncLang()==TRUE){					*/
/*				jdg = TRUE;								*/
/*			}											*/
/*			break;										*/

		case PDWN_SYS_CNTRS:	/* Contrast				*/
			LibFuncContrast();
			break;

		case PDWN_SYS_TPA:		/* Touch_Panel_Alignment*/
			LibFuncDigitizer();
			break;

		default:
			break;
	}
	return(jdg);
}


/******************************************************************************
	[Title]		Wait OK press in a message box
	[Arguments]	
	[ReturnVAL]	
******************************************************************************/
void WaitOKPress(void) {
	TCHSTS	tsts;

	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchMsg );
	LibTchInit();

	for(;;){
	LibTchWait( &tsts );

	switch( tsts.obj ) {
		
		case  OBJ_IC_MSG_OK:
			if(LibIconClick( &TiconMsgOk, &tsts ) == TRUE){
				/* Exit from subroutine*/
				return;
			}
			break;

		default:
			break;
		}
	}

}

