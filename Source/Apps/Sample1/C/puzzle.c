/******************************************************************************
*	
*	[Title]	
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.00
*
*	Copyright (C) 1997 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"puzzle.h"
#include	"gdata.h"

/***  ***/
static void PuzzleInit( void );
void PuzzleDisplay(void);
static void PuzzleTchSet(void);
static void DataInit( struct work_area *p );
static bool DataFileOpen(void);
static void HighScoreClr(void);
static bool HighScoreCk(void);
static void DispHSData(void);
static void HighScoreInput(byte *EditString);
static void HighScoreSave(byte *name);
static void DspFileOpenErr(void);
static void DispHighScore(void);
static void DataRead(void);
static void Restart( void );
static void Disp( struct work_area *p );
static void DispCard( int x, int y, int n, bool f );
static void Move( TCHSTS *tsts );
static bool JudgeEnd( void );
static int GetSeed( void );
static void Shuffle( struct work_area *p );
static void TimerInc( void );
static void DispTimer( void );
static void Num2( byte *p, int n );
static void TimeChg(byte *time,int hightime);
static byte LevelSet(void);
void GmPdwnSet(void);
bool PdwnFncSys(word sel);
bool GmPdwnFnc();


TCHTBL far TchHeaderIcon[] = {
	99, 0, 127, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_NEW,
	0x0000,

	71, 0, 98, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_LEVEL,
	0x0000,

	103, 13,159, 24,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_HIGH,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconNew = { &TchHeaderIcon[0], NULL, NULL, 0x00 };
T_ICON far TiconLevel = { &TchHeaderIcon[1], NULL, NULL, 0x00 };
T_ICON far TiconHigh = { &TchHeaderIcon[2], NULL, NULL, 0x02 };


TCHTBL far TchHeaderIcon2[] = {
	99, 0, 127, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_SET,
	0x0000,

	71, 0, 98, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_CLR,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconSet = { &TchHeaderIcon2[0], NULL, NULL, 0x00 };
T_ICON far TiconClr = { &TchHeaderIcon2[1], NULL, NULL, 0x00 };


TCHTBL far TchDispHigh[] = {

	106,135,135,150,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_HOK,
	0x0000,

	24,135,53,150,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_HCLR,
	0x0000,

	16, 27,16+128,27+128,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_HBOX,
	0x0000,

	0,0,159,159,
	ACT_MAKE,
	OBJ_IC_DSP_NONE,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconHOk = { &TchDispHigh[0], NULL, NULL, 0x02 };
T_ICON far TiconHClr = { &TchDispHigh[1], NULL, NULL, 0x02 };


TCHTBL far TchGameArea1[] = {     /* GameArea Level1 */
	((LCD_X_DOT-CARD_SIZE_X*3)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*3)/2)+30,
	((LCD_X_DOT+CARD_SIZE_X*3)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*3)/2)+30+CARD_SIZE_Y*3,
	ACT_MAKE | ACT_MOVE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK | ACT_500MSEC,
	OBJ_IC_GAMEAREA,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};
TCHTBL far TchGameArea2[] = {     /* GameArea Level2 */
	((LCD_X_DOT-CARD_SIZE_X*4)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*4)/2)+30,
	((LCD_X_DOT+CARD_SIZE_X*4)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*4)/2)+30+CARD_SIZE_X*4,
	ACT_MAKE | ACT_MOVE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK | ACT_500MSEC,
	OBJ_IC_GAMEAREA,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};
TCHTBL far TchGameArea3[] = {     /* GameArea Level3 */
	((LCD_X_DOT-CARD_SIZE_X*5)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*5)/2)+30,
	((LCD_X_DOT+CARD_SIZE_X*5)/2),
	((LCD_Y_DOT-30-CARD_SIZE_X*5)/2)+30+CARD_SIZE_X*5,
	ACT_MAKE | ACT_MOVE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK | ACT_500MSEC,
	OBJ_IC_GAMEAREA,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
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


struct work_area	puzzleinfo;


/******************************************************************************
*  [Title]			Puzzle(main)
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void) {
	TCHSTS	tsts;
	bool	end_flag;
	byte	mes_ans;

	sc = (SCORE *)&pzlfb.fbuf.bin.bin_buf;
	gamelevel = 1;				/* Level Initial*/

	PuzzleInit();  				/* Initial */
	Disp( &puzzleinfo );

	if(DataFileOpen()==FALSE){		/* DataFile open */
		DspFileOpenErr();			/* Open Error Disp */
		memset(sc,0xff,sizeof(SCORE));
	}else
		DataRead();					/* HighScore Data Read */

	LibTchInit();
	end_flag=FALSE;
	for(;;){
		LibTchWait( &tsts );
		if( tsts.ext==EXT_CODE_500MS ) TimerInc();
		switch( tsts.obj ) {
		case  OBJ_IC_NEW:			/* [NEW] */
			if(LibBlockIconClick( &TiconNew, &tsts , B_ICON_RIGHT ) == TRUE) {
				if(end_flag==TRUE){
					PuzzleDisplay(); 	/* Initial & Display */
				}
				Restart();
			}
			break;

		case  OBJ_IC_LEVEL:			/* [LEVEL] */
			if(LibBlockIconClick( &TiconLevel, &tsts , B_ICON_LEFT ) == TRUE) {
				mes_ans = LevelSet();
				if(mes_ans < 3) {
					gamelevel = mes_ans;
					PuzzleTchSet();
					if(end_flag==TRUE){
						PuzzleDisplay(); 	/* Initial & Display */
					}
					Restart();
				}
			}
			break;


		case  OBJ_IC_HIGH:			/* [HighScore] */
			if(LibIconClick( &TiconHigh, &tsts ) == TRUE) {
				if(entry_cond == FALSE){
					DspFileOpenErr();			/* Open Error Disp */
					break;
				}
				DispHighScore();
			}
			break;

		case  OBJ_IC_GAMEAREA:
			if(puzzleinfo.sts != GAME_RUN)  break;
			Move( &tsts );				/* Move */
			end_flag=JudgeEnd();
			break;

		/** M-BAR **/
			case OBJ_HIC_MBAR:
				GmPdwnSet();				/* PullDownMenu */
				if( GmPdwnFnc() == TRUE ){
					PuzzleInit();	  		/* Initial */
					Disp( &puzzleinfo );
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
*	[Title]		Level Chang Setting
*	[Arguments]	None
*	[ReturnVAL]	byte	0:LV1 1:LV2 2:LV3
******************************************************************************/
static byte LevelSet(void)
{
	byte    rtv[3+1] = {0,1,2,0xff};
	byte    cmnt[3*(7+1)],np;
	SLW2_ST win_prm;
	byte    sel_ret;
	byte    t_tbl[3][7+1];
	int     i,k,len;

	/* data */
	strcpy(t_tbl[0],"Level1");
	strcpy(t_tbl[1],"Level2");
	strcpy(t_tbl[2],"Level3");

	/* initial */
	np = gamelevel;

	/* disp Buff Making */
	for(i=k=0;i<3;i++,k++){
	    len = strlen(t_tbl[i]);
	    memcpy(&cmnt[k],t_tbl[i],len);
	    k += len;
	    if(i<1) cmnt[k] = 0xfe;
	    else    cmnt[k] = 0x00;     /* End Data */
	}

	/* select window */
	win_prm.x    =  70;
	win_prm.y    =  12;
	win_prm.xs   =  37;
	win_prm.ity  =   9;
	win_prm.np   = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv  = rtv;
	win_prm.t_xs =   4;

	sel_ret = LibSelWindowExt(&win_prm);
	return(sel_ret);
}

/******************************************************************************
*	[Title]		HighScore Data Raed
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DataRead(void){

	pzlfb.fsb_main_entry_ = fm_entry;		/* Main */
	pzlfb.fsb_sub_entry_  = fs_entry;		/* Sub */
	pzlfb.fsb_scrt_info_  = 0x80;			/* Open Mode */

	pzlfinf.fp      = 0xffff;		/* first Data */
	pzlfinf.kind    = FILE_KIND_BIN;/* Bin */

	if(entry_cond == FALSE)
		return;
	if(LibFileFindNext(&pzlfb,&pzlfinf,0x00)==TRUE){
	    LibFileRead(&pzlfb,&pzlfinf);     			/* Read */
	}else{
		memset(sc,0xff,sizeof(SCORE));
	}
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
*	[Title]		Data File Open
*	[Arguments]	None
*	[ReturnVAL]	bool TRUE=OK FLASE=NG
******************************************************************************/
static bool DataFileOpen(void){

	if(LibSubEntrySave(filename,&fs_entry)==FALSE){			/* Filename SAVE */
		fm_entry = 0xff;
		fs_entry = 0xff;
		entry_cond = FALSE;
		return(FALSE);
	}
	LibGetAllEntry(filename,&fm_entry,&fs_entry);	/* Main/Sub Entry Get */
	entry_cond = TRUE;
	return(TRUE);
}


/******************************************************************************
*	[Title]		Initial
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PuzzleInit( void ) {

	PuzzleDisplay();
	PuzzleTchSet();
	srand( GetSeed() );			/* Randomize */
	DataInit( &puzzleinfo );	/* DataInitial */
}

/******************************************************************************
*	[Title]		Touch Stack Push
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void PuzzleTchSet(void){

	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( TchHeaderIcon );

	if(gamelevel == 0)
		LibTchStackPush( TchGameArea1 );
	else if(gamelevel == 1)
		LibTchStackPush( TchGameArea2 );
	else
		LibTchStackPush( TchGameArea3 );

	LibTchStackPush( TchModeIcon );		/* Pop Up Tools */
}


/******************************************************************************
*	[Title]		InitialDisplay
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void PuzzleDisplay(void){

	LibClrDisp();

	LibPutGraph(0,0,Graph15Puzzle);

	LibPutFarData(71,0,8);
	LibGdsBox(103,13,158,23);
	LibGdsLine(104,24,158,24);
	LibGdsLine(159,14,159,24);

	LibPutProStr(IB_PFONT1,104,2,"New",30);				/* New */
	LibPutProStr(IB_PFONT1,74,2,"Level",30);			/* Level */
	LibPutProStr(IB_PFONT1,108,15,"High score",50);		/* High score */

	LibGdsBox(TIME_LU_X, TIME_LU_Y,TIME_LU_X+34, TIME_LU_Y+14);

	LibModeIconDsp();		/* Pop Up Tools */

	LibPutDisp();
}


/******************************************************************************
*	[Title]		BouadInitial
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
static void DataInit( struct work_area *p ) {

	int i,j;

	if(gamelevel == 0)
		board_size = 3;
	else if(gamelevel == 1)
		board_size = 4;
	else
		board_size = 5;

	for(i=0; i<board_size; i++)
		for(j=0;j<board_size;j++)
			p->loc[i][j] = i*board_size+j+1;

	p->loc[board_size-1][board_size-1]=0;
	p->fx=(board_size-1);
	p->fy=(board_size-1);
	p->sts=GAME_STOP;
	p->timests=TIMER_STOP;
	p->timer=0;
}

/******************************************************************************
*	[Title]		RestartProcedure
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void Restart( void ) {

	DataInit( &puzzleinfo );
	Shuffle( &puzzleinfo );
	puzzleinfo.sts=GAME_RUN;
	Disp( &puzzleinfo );
}

/******************************************************************************
*	[Title]		BouadInitialDisplay
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
static void Disp( struct work_area *p ) {

	int		i,j;
	int		size_x,size_y,x1,y1;

	size_x = CARD_SIZE_X * board_size;
	size_y = CARD_SIZE_Y * board_size;
	x1 = ((LCD_X_DOT-size_x)/2);
	y1 = ((LCD_Y_DOT-30-size_y)/2)+30;

	LibGdsClr( 18,30,159,159);

	LibGdsBox( x1-2, y1-2, x1+size_x, y1+size_y);

	for(i=0; i<board_size; i++)
		for(j=0; j<board_size; j++)
			DispCard( j, i, p->loc[i][j], FALSE );

	LibPutDisp();
	DispTimer();
}

/******************************************************************************
*	[Title]		ChipsDisplay
*	[Arguments]	int  x, y  --- DrawPosition(0Å`3)
*				int  n  --- ChipNumber(0Å`15 :0=Space)
*				bool f  --- ( TRUE:Display )
*	[ReturnVAL]	None
******************************************************************************/
static void DispCard( int x, int y, int n, bool f ) {

	int		x1,y1;

	x1 = ((LCD_X_DOT-(CARD_SIZE_X * board_size))/2);
	y1 = ((LCD_Y_DOT-30-(CARD_SIZE_Y * board_size))/2)+30;

	if( n!=0 )
		LibPutGraph( x*CARD_SIZE_X+x1, y*CARD_SIZE_Y+y1, Graphs[n-1] );
	else
		LibClrBox( x*CARD_SIZE_X+x1, y*CARD_SIZE_Y+y1,CARD_SIZE_X, CARD_SIZE_Y );

	if(f)
		LibPutDispBox( x*CARD_SIZE_X+x1, y*CARD_SIZE_Y+y1,CARD_SIZE_X, CARD_SIZE_Y );
}

/******************************************************************************
*	[Title]		MoveChips
*	[Arguments]	TCHSTS *tsts : Touch Status
*	[ReturnVAL]	None
*******************************************************************************/
static void Move( TCHSTS *tsts ) {

	int		i, w;
	int		sx, sy, x1, y1;

	x1 = ((LCD_X_DOT-(CARD_SIZE_X * board_size))/2);
	y1 = ((LCD_Y_DOT-30-(CARD_SIZE_Y * board_size))/2)+30;

	if( puzzleinfo.timests==TIMER_STOP )
		puzzleinfo.timests=TIMER_RUN;
	for(sx=-1,i=0; i<board_size; i++) {
		w = x1 + CARD_SIZE_X*i;
		if( (w <= tsts->x) && (tsts->x < w + CARD_SIZE_X) ) {
			sx = i;
			break;
		}
	}
	if(sx == -1)  return;
	for(sy=-1,i=0; i<board_size; i++) {
		w = y1 + CARD_SIZE_Y*i;
		if( (w <= tsts->y) && (tsts->y < w + CARD_SIZE_Y) ) {
			sy = i;
			break;
		}
	}
	if(sy == -1)  return;
	if( puzzleinfo.fx==sx && puzzleinfo.fy==sy ) return;

	if( puzzleinfo.fx==sx ) {
		if( puzzleinfo.fy<sy )
			for( i=puzzleinfo.fy; i<sy; i++ )
				DispCard( sx, i, puzzleinfo.loc[i][sx]=puzzleinfo.loc[(i+1)][sx], TRUE );
		else
			for( i=puzzleinfo.fy; i>sy; i-- )
				DispCard( sx, i, puzzleinfo.loc[i][sx]=puzzleinfo.loc[(i-1)][sx], TRUE );
	} else if( puzzleinfo.fy==sy ){
		if( puzzleinfo.fx<sx )
			for( i=puzzleinfo.fx; i<sx; i++ )
				DispCard( i, sy, puzzleinfo.loc[sy][i]=puzzleinfo.loc[sy][(i+1)], TRUE );
		else
			for( i=puzzleinfo.fx; i>sx; i-- )
				DispCard( i, sy, puzzleinfo.loc[sy][i]=puzzleinfo.loc[sy][(i-1)], TRUE );
	} else
		return;
	DispCard( sx, sy, puzzleinfo.loc[sy][sx]=0, TRUE );
	puzzleinfo.fx=sx;
	puzzleinfo.fy=sy;
}


/******************************************************************************
*	[Title]		High Score Input
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void HighScoreInput(byte *EditString){

TXTP		EventPrm;				/* Declaration of structure for text input */
TCHTBL 		TchTxtBuf[3];			/* Text touch area table */
T_SCR_POS	ComScr;					/* Scroll bar position information */
word		GdTbl[1];				/* Guidance comment table */

byte		key_cd;
TCHSTS		tsts;
byte		input_end;

	GdTbl[0]		= 0xffff;			/* No Guidance comment */
	EventPrm.st_x	= 79;				/* Start coordinate (X) of text display */
	EventPrm.st_y	= 61;				/* Start coordinate (Y) of text display */
	EventPrm.ed_x	= 137;				/* End coordinate (X) of text display  */
	EventPrm.it_y	= 9;				/* Text display line spacing (Y) */
	EventPrm.MAXGYO	= 1;				/* Number of text display lines  */
	EventPrm.font	= IB_PFONT1;		/* Display font type  */
	EventPrm.csen	= TRUE;				/* Cursor display enabled(Keyword registration enabled) */
	EventPrm.rtnen	= 0x02;				/* CR code display enabled  */
	EventPrm.maxmj	= NAME_MAX;			/* Maximum number of allowable input characters. */
	EventPrm.txbf	= EditString;		/* Designation of text buffer address  */
	EventPrm.gdcmt	= GdTbl;			/* Guidance comment table  */
	EventPrm.txtobj	= OBJ_STRING;		/* Object when text area is touched.  */
	EventPrm.sbrobj	= NULL;				/* Object when the scroll bar is touched. */
	EventPrm.tchtb	= TchTxtBuf;		/* Pointer for text scroll bar area  */
	EventPrm.s_pos	= &ComScr;			/* Pointer for text and scroll bar information */

	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( TchHeaderIcon2 );

	EditString[0] = 0;					/* Initialization of text buffer */
	LibTxtInit(&EventPrm);				/* Initialization of text input */
	LibTxtTchSet(&EventPrm);			/* Text touch area PUSH */

    LibDispKey();

	input_end = FALSE;
	while(input_end == FALSE){

		LibTxtDsp(&EventPrm);
		LibPutDisp();

		key_cd = LibGetKeyM(&tsts);
		if(key_cd == KEY_NONE){					/* Not keyboard */
			switch(tsts.obj){
				case OBJ_HIC_MBAR:
					LibEditPullDown();
					break;

				case OBJ_IC_SET:
					if(LibBlockIconClick( &TiconSet, &tsts , B_ICON_RIGHT ) == TRUE){
						while(LibTchStackPop()!=NULL)
						LibTchInit();
						input_end = TRUE;
					}
					break;

				case OBJ_IC_CLR:
					if(LibBlockIconClick( &TiconClr, &tsts , B_ICON_LEFT ) == TRUE){
						EditString[0] = 0;
						LibTxtInit(&EventPrm);
					}
					break;
			}
		}
		LibTxtInp(key_cd,&tsts,&EventPrm);
	}
}


/******************************************************************************
*	[Title]		EndingJudge
*	[Arguments]	None
*	[ReturnVAL]	bool TRUE=End
******************************************************************************/
static bool JudgeEnd( void ) {
	char t[6];
	int  i,j, f;
	byte	name[NAME_MAX+1];		/* Buffer for text input */

	f=0;
	for( i=0; i<board_size; i++ ){
		for(j=0;j<board_size;j++){
			if( puzzleinfo.loc[i][j]!=(i*board_size+j+1)%(board_size*board_size) ){
				f=1;
				break;
			}
		}
	}
	if( f==1 ) return FALSE;

	LibWait(IB_500MWAIT);
	if(HighScoreCk()==TRUE){
		LibOpenWindow(0,0,160,160);
		LibPutGraph(0,0,Graph15Puzzle);

		LibPutFarData(71,0,8);
		LibPutProStr(IB_PFONT1,79,2,"Clr",18);				/* Clr */
		LibPutProStr(IB_PFONT1,106,2,"Set",18);				/* Set */
		LibPutFarData((LCD_X_DOT-128)/2,27,107);
		LibPutFarData(75,58,135);

		LibPutProStr(IB_PFONT3,33,33,"Congratulations!",110);	/* Congratulations! */
		LibPutProStr(IB_PFONT3,26,46,"Time",50);				/* Time */
		LibPutProStr(IB_PFONT3,26,59,"Name",50);				/* Name */

		Num2( &t[0], puzzleinfo.timer/120);
		t[2]=':';
		Num2( &t[3], (puzzleinfo.timer%120)/2);
		t[5]='\0';
		LibPutProStr(IB_PFONT3,97,46,t,34);

		HighScoreInput(name);		/* High Score Input */
		LibCloseWindow();
		HighScoreSave(name);		/* High Score Save */

	}else{
		LibGdsClr(((LCD_X_DOT-128)/2)-1,27-1,((LCD_X_DOT-128)/2)+128+1,27+50+1);
		LibPutFarData((LCD_X_DOT-128)/2,27,107);

		LibPutProStr(IB_PFONT3,33,38,"Congratulations!",110);			/* Congratulations! */
		LibPutProStr(IB_PFONT3,((LCD_X_DOT-128)/2)+10,56,"Time",50);	/* Time */

		Num2( &t[0], puzzleinfo.timer/120);
		t[2]=':';
		Num2( &t[3], (puzzleinfo.timer%120)/2);
		t[5]='\0';
		LibPutProStr(IB_PFONT3,97,56,t,34);
	}
	LibPutDisp();
	puzzleinfo.sts=GAME_STOP;
	puzzleinfo.timests=TIMER_STOP;
	return TRUE;
}

/******************************************************************************
*	[Title]		HighScore Save
*	[Arguments]	None
*	[ReturnVAL] None
******************************************************************************/
static void HighScoreSave(byte *name){

	bool    f_handle;

	if(entry_cond == FALSE)
		return;

	pzlfb.fsb_main_entry_ = fm_entry;			/* Main */
	pzlfb.fsb_sub_entry_  = fs_entry;			/* Sub */
	pzlfb.fsb_scrt_info_  = 0x80;				/* Open Mode */

	pzlfinf.fp      = 0xffff;					/* first Data */
	pzlfinf.kind    = FILE_KIND_BIN;			/* Bin */

	if(LibFileFindNext(&pzlfb,&pzlfinf,0x00)==FALSE){
		pzlfinf.fp  = 0xffff;					/* first Data */
	}

	if(gamelevel == 0){
		strcpy(sc->lv1,name);
		sc->lv1time = puzzleinfo.timer;
	}else if(gamelevel == 1){
		strcpy(sc->lv2,name);
		sc->lv2time = puzzleinfo.timer;
	}else{
		strcpy(sc->lv3,name);
		sc->lv3time = puzzleinfo.timer;
	}
	pzlfb.fsb_main_entry_ = fm_entry;			/* Main */
	pzlfb.fsb_sub_entry_  = fs_entry;			/* Sub */
	pzlfb.fsb_scrt_info_  = 0x80;				/* Open Mode */

	pzlfinf.kind    = FILE_KIND_BIN;			/* Bin */

	pzlfb.fbuf.bin.char_num = sizeof(SCORE);	/* BinaryDataCapacity */

	f_handle = LibFileWrite(&pzlfb, &pzlfinf);	/* Save */
	if(f_handle == FALSE){
		LibErrorDisp(FlashStatus);				/* Error Disp */
		DataRead();								/* HighScore Data Read */
	}
}

/******************************************************************************
*	[Title]		HighScore Check
*	[Arguments]	None
*	[ReturnVAL]	bool TRUE=HighScore
******************************************************************************/
static bool HighScoreCk(void){

	int		time;

	if(entry_cond == FALSE)
		return(FALSE);

	if(gamelevel == 0)
		time = sc->lv1time;
	else if(gamelevel == 1)
		time = sc->lv2time;
	else
		time = sc->lv3time;

	if(time == 0xffff || time > puzzleinfo.timer)
		return(TRUE);

	return(FALSE);
}


/******************************************************************************
*	[Title]		Disp HighScore Main
*	[Arguments]	None
*	[ReturnVAL] None
******************************************************************************/
#define		DSP_HS_X	16
#define		DSP_HS_Y	27

static void DispHighScore(void){

TCHSTS	tsts;
byte	loop_end;

	LibOpenWindow(DSP_HS_X-1,DSP_HS_Y-1,130,130);

	DispHSData();

	LibTchStackPush(NULL);
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( TchDispHigh );
	LibTchStackPush( TchActionKey );

	LibTchInit();

	loop_end = FALSE;
	while(loop_end==FALSE){
		LibTchWait(&tsts);
		switch(tsts.obj){

			case OBJ_LPSW_PUSH:
			case OBJ_IC_HOK:
				if(LibIconClick( &TiconHOk, &tsts ) == TRUE)
					loop_end = TRUE;
				break;

			case OBJ_IC_HCLR:
				if(LibIconClick( &TiconHClr, &tsts ) == TRUE){
					HighScoreClr();
					DispHSData();
				}
				break;

			case OBJ_HIC_ESC:
			case OBJ_IC_DSP_NONE:
				loop_end = TRUE;
			default:
				LibRepOff();
				break;
		}
	}
	while(LibTchStackPop()!=NULL)
	LibTchInit();
	LibCloseWindow();
	LibPutDisp();
}

/******************************************************************************
*	[Title]		HighScore Data Clr
*	[Arguments]	None
*	[ReturnVAL] None
******************************************************************************/
static void HighScoreClr(void){

	if(entry_cond == FALSE)
		return;

	if(sc->lv1time == 0xffff && sc->lv2time == 0xffff && sc->lv3time == 0xffff)
		return;

	pzlfb.fsb_main_entry_ = fm_entry;		/* Main */
	pzlfb.fsb_sub_entry_  = fs_entry;		/* Sub */
	pzlfb.fsb_scrt_info_  = 0x80;			/* Open Mode */
	LibFileRemoveAll(&pzlfb);				/* Sub Mode Data Delete */
	memset(sc,0xff,sizeof(SCORE));
}

/******************************************************************************
*	[Title]		HighScore Data Disp
*	[Arguments]	None
*	[ReturnVAL] None
******************************************************************************/
static void DispHSData(void){

	byte t[6];

	LibPutFarData(DSP_HS_X,DSP_HS_Y,106);

	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+22,135);		/* Level1 Name */
	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+57,135);		/* Level2 Name */
	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+92,135);		/* Level3 Name */

	LibPutFarData(DSP_HS_X+8,DSP_HS_Y+108,113);		/* for Clr */
	LibPutFarData(DSP_HS_X+90,DSP_HS_Y+108,114);	/* for OK */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+4,"Level1",50);			/* Level1 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+13,"Time",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+25,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+39,"Level2",50);		/* Level2 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+48,"Time",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+60,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+74,"Level3",50);		/* Level3 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+83,"Time",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+95,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT1,DSP_HS_X+17,DSP_HS_Y+112,"Clr",30);			/* Clr */
	LibPutProStr(IB_PFONT1,DSP_HS_X+99,DSP_HS_Y+112,"OK",30);			/* OK */

	if(sc->lv1time != 0xffff){
		TimeChg(t,sc->lv1time);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+13,t,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+25,sc->lv1,58);
	}

	if(sc->lv2time != 0xffff){
		TimeChg(t,sc->lv2time);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+48,t,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+60,sc->lv2,58);
	}

	if(sc->lv3time != 0xffff){
		TimeChg(t,sc->lv3time);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+83,t,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+95,sc->lv3,58);
	}
	LibPutDisp();
}

static void TimeChg(byte *time,int hightime){
	Num2( &time[0], hightime/120);
	time[2]=':';
	Num2( &time[3], (hightime%120)/2);
	time[5]='\0';
}


/******************************************************************************
*	[Title]		Shuffle
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
******************************************************************************/
static void Shuffle( struct work_area *p ) {
	int i, j,ii,w,m,ff;

	for(m=0;m<board_size*2;m++){
		ff=0;
		for(i=0;i<board_size;i++){
			if(p->loc[m][i]==0){ff=1;break;}
			if(p->loc[i][m]==0){ff=1;break;}
		}
		if(ff==1)	continue;
		i=rand()%board_size;
		do{ii=rand()%board_size;}while(i==ii);
		w=p->loc[m][i];
		p->loc[m][i]=p->loc[m][ii];
		p->loc[m][ii]=w;
		w=p->loc[i][m];
		p->loc[i][m]=p->loc[ii][m];
		p->loc[ii][m]=w;
	}
	for( i=0; i<board_size; i++ ){
		for(j=0;j<board_size;j++){
			if( p->loc[i][j]==0) {
				p->fx = j;
				p->fy = i;
				break;
			}
		}
	}
}

/******************************************************************************
*	[Title]		Timer++
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void TimerInc( void ) {

	if( puzzleinfo.timests==TIMER_RUN ) {
		if( (++(puzzleinfo.timer))>7198 ) puzzleinfo.timests=TIMER_OVER;
		if( !(puzzleinfo.timer&1) ) DispTimer();
	}
}

/******************************************************************************
*	[Titel]		TimerDisplay
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DispTimer( void ) {
	byte t[6];
	int time;

	time=puzzleinfo.timer;
	Num2( &t[0], time/120);
	t[2]=':';
	Num2( &t[3], (time%120)/2);
	t[5]='\0';
	LibPutProStr(IB_PFONT3,TIME_LU_X+2,TIME_LU_Y+2,t,34);
	LibPutDispBox( TIME_LU_X, TIME_LU_Y,35,15);
}

static void Num2( byte *p, int n ) {
	*p=(byte)(n/10)+'0';
	*(p+1)=(byte)(n%10)+'0';
}


/******************************************************************************
*	[Title]		RandomSeed(0<=VAL<=32768)
*	[Arguments]	None
*	[ReturnVAL]	RandomSeed
******************************************************************************/
static int GetSeed( void ) {
	int   s;
	long  hh, mm, ss;
	byte  stime[7];

	LibGetDateTimeM( stime );
	hh = (long)( (10*(stime[1]&0x0F) + (stime[2]&0x0F)) * 3600 );
	mm = (long)( (10*(stime[3]&0x0F) + (stime[4]&0x0F)) * 60 );
	ss = (long)( (10*(stime[5]&0x0F) + (stime[6]&0x0F)) );
	s = (int)( (hh+mm+ss)%32768 );
	return( s );
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

