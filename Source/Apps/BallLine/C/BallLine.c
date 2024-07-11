/******************************************************************************
*	[Title]		PVBallLines
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"BallLine.h"
#include	"gdata.h"

/***  ***/
static void GameInit( void );
void TitleDisplay(void);
static void GameTchSet( bool f );
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
static void DispFrame( void );
static void DispCirStr( void );
static void Move( TCHSTS *tsts );
static bool JudgeEnd( void );
static int GetSeed( void );
static void Shuffle( struct work_area *p,int n, bool f );
static void DispTimer( void );
static byte LevelSet(void);
static void Push(int x, int y, bool resetPoint);
static	void Disapear( void );


TCHTBL far TchHeaderIcon[] = {
	30, 0, 58, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_NEW,
	0x0000,

	2, 0, 29, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_LEVEL,
	0x0000,

	60, 0,115, 11,
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

TCHTBL far TchGameArea[] = {     /* GameArea */
	0,15,159,159,
	ACT_MAKE,
	OBJ_IC_GAMEAREA,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

TCHTBL far TchMessage[] = {     /* Message */
	0,0,159,159,
	ACT_MAKE,
	OBJ_IC_MESSAGE,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

struct work_area	Gameinfo;
struct	DRAWCIRCLE_PACS draw_prm;
TCHSTS	tsts;

/******************************************************************************
*  [Title]			ALine(main)
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void) {
	bool	end_flag;
	byte	mes_ans;

	sc = (SCORE *)&pzlfb.fbuf.bin.bin_buf;
	gamelevel = 0;				/* Level Initial*/

	GameInit();  				/* Initial */
	Disp( &Gameinfo );

	if(DataFileOpen()==FALSE){		/* DataFile open */
		DspFileOpenErr();			/* Open Error Disp */
		memset(sc,0xff,sizeof(SCORE));
	}else
		DataRead();					/* HighScore Data Read */
	end_flag=FALSE;
	for(;;){
		LibTchWait( &tsts );
		switch( tsts.obj ) {
		case  OBJ_IC_NEW:			/* [NEW] */
			if(LibBlockIconClick( &TiconNew, &tsts , B_ICON_RIGHT ) == TRUE) {
				if(end_flag==TRUE){
					TitleDisplay(); 	/* Initial & Display */
				}
				Restart();
			}
			break;

		case  OBJ_IC_LEVEL:			/* [LEVEL] */
			if(LibBlockIconClick( &TiconLevel, &tsts , B_ICON_LEFT ) == TRUE) {
				mes_ans = LevelSet();
				if(mes_ans < 3) {
					gamelevel = mes_ans;
					GameTchSet( FALSE );
					if(end_flag==TRUE){
						TitleDisplay(); 	/* Initial & Display */
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
			if(Gameinfo.sts != GAME_RUN)  break;
			Move( &tsts );				/* Move */
			end_flag=JudgeEnd();
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
	win_prm.x    =  1;
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
*	[Title]		HighScore Data Read
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

	LibPutProStr(IB_PFONT1,30,35,"PV application memory",110);
	LibPutProStr(IB_PFONT1,30,44,"is full!",110);
	LibPutProStr(IB_PFONT1,30,53,"See your readme file",110);
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
static void GameInit( void ) {
	
	GameTchSet( FALSE );
	srand( GetSeed() );			/* Randomize */
	DataInit( &Gameinfo );	/* DataInitial */
	TitleDisplay();
}

/******************************************************************************
*	[Title]		Touch Stack Push
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void GameTchSet( bool f ){

	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	if(f==FALSE){
		LibTchStackPush( TchHeaderIcon );
		LibTchStackPush( TchGameArea );
	}else	LibTchStackPush( TchMessage );
	LibTchInit();
}


/******************************************************************************
*	[Title]		Display Title
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void TitleDisplay(void){

	char	*num2="    ";

	LibClrDisp();

	LibPutFarData(2,0,8);				/*Display New and Level Box*/
	LibGdsBox(60,0,114,10);			/*Display High score Box*/
	LibGdsLine(61,11,115,11);
	LibGdsLine(115,0,115,11);

	LibPutProStr(IB_PFONT1,36,2,"New",30);				/* New */
	LibPutProStr(IB_PFONT1,4,2,"Level",30);			/* Level */
	LibPutProStr(IB_PFONT1,64,2,"High score",50);		/* High score */
	LibNumToStr(num2, score, 4);	/*display score*/
	LibPutProStr(IB_PFONT3,123,2,num2,34);

	LibPutDisp();
}


/******************************************************************************
*	[Title]		BouadInitial
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
static void DataInit( struct work_area *p ) {

	int i,j;

	score=0;
	for(j=0;j<=CARD_NUM+1;j++){
		Data[0][j]=-1;
		Data[CARD_NUM+1][j]=-1;
	}
	for(i=0;i<=CARD_NUM+1;i++){
		Data[i][0]=-1;
		Data[i][CARD_NUM+1]=-1;
	}
	for(i=1;i<=CARD_NUM;i++){
		for(j=1;j<=CARD_NUM;j++){
			Data[i][j]=0;
			Data1[i][j]=0;
		}
	}

	switch(gamelevel)	{
		case	0:
			Cir_num=6;
			break;
		case	1:
			Cir_num=7;
			break;
		case	2:
			Cir_num=8;
			break;
	}

	click=0;
	p->sts=GAME_STOP;
}

/******************************************************************************
*	[Title]		RestartProcedure
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void Restart( void ) {

	DataInit( &Gameinfo );
	Shuffle( &Gameinfo, 3, 1 );
	Gameinfo.sts=GAME_RUN;
	TitleDisplay();
	Disp( &Gameinfo );
}

/******************************************************************************
*	[Title]		BouadInitialDisplay
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
static void Disp( struct work_area *p ) {
	
	LibGdsClr( 0, 14, 159, 159);
	DispFrame();			/*Display Frame*/
	DispCirStr( );			/*Display Card*/
}

/******************************************************************************
*	[Title]		Display Frame
*	[Arguments]	None
*	[ReturnVAL]	None
*******************************************************************************/
static void DispFrame( void ) {

	int	i;
	int	x1,x2,y1,y2;	

	LibGdsClr( 0,14,159,159);

	for(i=0;i<=CARD_NUM;i++){
		x1=BOARD_START_X;
		y1=BOARD_START_Y+i*CARD_SIZE;
		x2=BOARD_START_X+CARD_NUM*CARD_SIZE;
		y2=y1;
		LibGdsLine(x1,y1,x2,y2);

		x1=BOARD_START_X+i*CARD_SIZE;
		y1=BOARD_START_Y;
		x2=x1;
		y2=y1+CARD_NUM*CARD_SIZE;
		LibGdsLine(x1,y1,x2,y2);
	}

	LibPutDisp();
}


/******************************************************************************
*	[Title]		ChipsDisplay
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void DispCirStr( void ) {
	int	i,j,n;
	int	x1,y1;

	/*Draw Graph*/
	for(i=1;i<=CARD_NUM;i++){
		for(j=1;j<=CARD_NUM;j++){
			x1=BOARD_START_X+(i-1)*CARD_SIZE+1;
			y1=BOARD_START_Y+(j-1)*CARD_SIZE+1;
			if((Data[i][j]>0)&&(Data[i][j]<Cir_num+1)){
				n=Data[i][j];
				LibPutGraph( x1, y1, Graphs[n-1] );
			}
			else{
				if((Data[i][j]>=10)&&(Data[i][j]<=Cir_num*10)){
					n=Data[i][j]/10;
					LibPutGraph( x1, y1, Graphs[n-1] );
					LibReverse(x1, y1, CARD_SIZE-1, CARD_SIZE-1 );
				}
			}
		}
	}
	LibPutDisp();
}


/******************************************************************************
*	[Title]		MoveChips
*	[Arguments]	TCHSTS *tsts : Touch Status
*	[ReturnVAL]	None
*******************************************************************************/
static void Move( TCHSTS *tsts ) {

	int	i,j,k;
	int	tmp;
	int	dx,dy;

	cx=(tsts->x-BOARD_START_X)/CARD_SIZE+1;
	cy=(tsts->y-BOARD_START_Y)/CARD_SIZE+1;
	if((cx>CARD_NUM)||(cy>CARD_NUM)||(cx<1)||(cy<1))	/*out gamearea*/
		return;

	click=(click+1)%2;	/*is used to judge select or move ball*/

	if(click){			/*select ball*/
		if((Data[cx][cy]==FALSE)||((Data[cx][cy]>=10)&&(Data[cx][cy]<=Cir_num*10))){	/*selected blank or Next ball*/			
			click=(click+1)%2;
			return;
		}
		else{		/*Selected ball*/			
			sx=cx;	sy=cy;
			LibGdsMesh(BOARD_START_X+(sx-1)*CARD_SIZE+1,BOARD_START_Y+(sy-1)*CARD_SIZE+1
				,BOARD_START_X+sx*CARD_SIZE-1,BOARD_START_Y+sy*CARD_SIZE-1);
			LibPutDisp();
		}
	}
	else{			/*move ball*/
		if(((Data[cx][cy]>0)&&(Data[cx][cy]<=Cir_num))==TRUE){	/*selected ball again*/
			LibClrBox(BOARD_START_X+(sx-1)*CARD_SIZE+1,BOARD_START_Y+(sy-1)*CARD_SIZE+1
				,CARD_SIZE-2,CARD_SIZE-2);
			LibPutGraph( BOARD_START_X+(sx-1)*CARD_SIZE+1, BOARD_START_Y+(sy-1)*CARD_SIZE+1, Graphs[Data[sx][sy]-1]);
			LibGdsMesh(BOARD_START_X+(cx-1)*CARD_SIZE+1,BOARD_START_Y+(cy-1)*CARD_SIZE+1
				,BOARD_START_X+cx*CARD_SIZE-1,BOARD_START_Y+cy*CARD_SIZE-1);
			LibPutDisp();
			sx=cx;	sy=cy;
			click=(click+1)%2;
			return;
		}
		else{
			/* get the continuous block */
			Push(sx, sy, TRUE);
			i = 0; j= 0;
			while(deleteIt[i][0] != -1){
   				dx = deleteIt[i][0];
	   			dy = deleteIt[i][1];
	
				if ((dx == cx) && ( dy == cy))	break;
				if ((Data[dx][dy-1] == 0)||(Data[dx][dy-1]>=10))
					Push(dx, dy-1, FALSE);
				if ((Data[dx][dy+1] == 0)||(Data[dx][dy+1]>=10))
					Push(dx, dy+1, FALSE);
				if	((Data[dx-1][dy] == 0)||(Data[dx-1][dy]>=10))
					Push(dx-1, dy, FALSE);
				if ((Data[dx+1][dy] == 0)||(Data[dx+1][dy]>=10))
					Push(dx+1, dy, FALSE);
				i++;
			}

			if (deleteIt[i][0] == -1){
				click=(click+1)%2;
				return;		/*can't mov*/
			}

			tmp=Data[cx][cy];
			Data[cx][cy]=Data[sx][sy];
			Data[sx][sy]=0;
			Disapear();		/*Disapear ball*/
			if(JudgeEnd()==TRUE)	return;
			
			if((tmp>=10)&&(tmp<=Cir_num*10)){		/*selected Next ball*/	
				do{
					i=rand()%CARD_NUM+1;
					j=rand()%CARD_NUM+1;
				}while(Data[i][j]!=0);

				k=rand()%Cir_num+1;
				Data[i][j]=k;				/*gengrate a ball*/
			}

			for(i=1;i<=CARD_NUM;i++){		/*Next ball change to ball*/
				for(j=1;j<=CARD_NUM;j++){
					if((Data[i][j]>=10)&&(Data[i][j]<=Cir_num*10))
						Data[i][j]=Data[i][j]/10;
				}
			}
			Disapear();		/*Disapear ball*/

			k=0;
			for(i=1;i<=CARD_NUM;i++){		/*generate next ball*/
				for(j=1;j<=CARD_NUM;j++){
					if(Data[i][j]==FALSE)	k=k+1;
				}
			}
			if(k>=3){
				Shuffle( &Gameinfo, 3, 0 );
			}
			else{
				Shuffle( &Gameinfo, k, 0 );					
			}

		Disp( &Gameinfo );
		}
	}
}


/******************************************************************************
*	[Title]		Push
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void Push(int x, int y, bool resetPoint) {
	static int stackPoint = 0;
   int	i;

	if (resetPoint) {
	   stackPoint = 0;
	 	deleteIt[0][0] = -1;
		deleteIt[0][1] = -1;
   }

   i = 0;
   while(deleteIt[i][0] != -1){
		if ((deleteIt[i][0] == x) && (deleteIt[i][1] == y)) return;
      i++;
	}

  	deleteIt[stackPoint][0] = x;
   deleteIt[stackPoint][1] = y;
	stackPoint++;
  	deleteIt[stackPoint][0] = -1;
   deleteIt[stackPoint][1] = -1;
}


/******************************************************************************
*	[Title]		Disapear
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
static void Disapear( ) {

		int	i,j,k,m;
		char	*num2="    ";

		
		for(i=1;i<=CARD_NUM;i++){
			for(j=1;j<=CARD_NUM;j++){
				Data1[i][j]=0;
			}
		}

		/*Disapear horizontal ball*/
		for(m=1;m<=CARD_NUM;m++){
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[m][i+j]<=0)||(Data[m][i+j]>Cir_num))	break;	/*not ball*/
					if(Data[m][i+j]!=Data[m][i+j+1])	break;	/*left not equal to right*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[m][j+k]=1;
					}
				}
			}
		}

		/*Disapear vertical ball*/
		for(m=1;m<=CARD_NUM;m++){
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[i+j][m]<=0)||(Data[i+j][m]>Cir_num))	break;	/*not ball*/
					if(Data[i+j][m]!=Data[i+j+1][m])	break;	/*up not equal to down*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[j+k][m]=1;
					}
				}
			}
		}

		/*Disapear inclined and right ball*/
		for(m=0;m<=CARD_NUM-1;m++){
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[m+i+j][i+j]<=0)||(Data[m+i+j][i+j]>Cir_num))	break;	/*not ball*/
					if(Data[m+i+j][i+j]!=Data[m+i+j+1][i+j+1])	break;	/*left not equal to right*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[m+j+k][j+k]=1;
					}
				}
			}
			
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[i+j-m][i+j]<=0)||(Data[i+j-m][i+j]>Cir_num))	break;	/*not ball*/
					if(Data[i+j-m][i+j]!=Data[i+j+1-m][i+j+1])	break;	/*left not equal to right*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[j+k-m][j+k]=1;
					}
				}
			}
		}

		/*Disapear inclined and left ball*/
		for(m=0;m<=CARD_NUM-1;m++){
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[i+j][CARD_NUM+1-(m+i+j)]<=0)||(Data[i+j][CARD_NUM+1-(m+i+j)]>Cir_num))	break;	/*not ball*/
					if(Data[i+j][CARD_NUM+1-(m+i+j)]!=Data[i+j+1][CARD_NUM+1-(m+i+j+1)])	break;	/*left not equal to right*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[j+k][CARD_NUM+1-(m+j+k)]=1;
					}
				}
			}
			
			i=0;
			for(j=i;j<=CARD_NUM-1;j++){
				k=1;
				for(i=1;i<=CARD_NUM;i++){
					if((Data[i+j][CARD_NUM+1-(i+j-m)]<=0)||(Data[i+j][CARD_NUM+1-(i+j-m)]>Cir_num))	break;	/*not ball*/
					if(Data[i+j][CARD_NUM+1-(i+j-m)]!=Data[i+j+1][CARD_NUM+1-(i+j+1-m)])	break;	/*left not equal to right*/
					k=k+1;
				}
				if(k>=5){
					for(;k>0;k--){
						Data1[j+k][CARD_NUM+1-(j+k-m)]=1;
					}
				}
			}
		}

	
		k=0;
		for(i=1;i<=CARD_NUM;i++){
			for(j=1;j<=CARD_NUM;j++){
				if(Data1[i][j]==1){
					Data[i][j]=0;
					k=k+1;
				}
			}
		}
		if(k>=5){
			score=score+k*(k-4);
			LibNumToStr(num2, score, 4);
			LibClrBox(116,0,30,14);
			LibPutProStr(IB_PFONT3,123,2,num2,34);
		}
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

	int i,j,k;
	char *num2="    ";
	byte	name[NAME_MAX+1];		/* Buffer for text input */

	k=0;
	for(i=1;i<=CARD_NUM;i++){
		for(j=1;j<=CARD_NUM;j++){
			if((Data[i][j]>=1)&&(Data[i][j]<=Cir_num)){
				k=k+1;
			}
		}
	}

	if((k==CARD_NUM*CARD_NUM)||(k==0)){
		LibWait(IB_500MWAIT);
		if(HighScoreCk()==TRUE){
			LibOpenWindow(0,0,160,160);
			LibPutGraph(0,0,GraphALine);
	
			LibPutFarData(71,0,8);
			LibPutProStr(IB_PFONT1,79,2,"Clr",18);				/* Clr */
			LibPutProStr(IB_PFONT1,106,2,"Set",18);			/* Set */
			LibPutFarData((LCD_X_DOT-128)/2,27,107);
			LibPutFarData(75,58,135);

			LibPutProStr(IB_PFONT3,33,33,"Congratulations!",110);	/* Congratulations! */
			LibPutProStr(IB_PFONT3,26,46,"Score",50);				/* Time */
			LibPutProStr(IB_PFONT3,26,59,"Name",50);				/* Name */

			LibNumToStr(num2, score, 4);
			LibPutProStr(IB_PFONT3,97,46,num2,34);

			HighScoreInput(name);		/* High Score Input */
			LibCloseWindow();
			HighScoreSave(name);		/* High Score Save */
			LibPutDisp();

		}else{
			LibGdsClr(((LCD_X_DOT-128)/2)-1,27-1,((LCD_X_DOT-128)/2)+128+1,27+50+1);
			LibPutFarData((LCD_X_DOT-128)/2,27,107);

			LibPutProStr(IB_PFONT3,48,38,"Game Over!",110);			/* Congratulations! */
			LibPutProStr(IB_PFONT3,((LCD_X_DOT-128)/2)+20,56,"Score",50);	/* Time */
			LibNumToStr(num2, score, 4);
			LibPutProStr(IB_PFONT3,97,56,num2,34);
			LibPutDisp();
			GameTchSet( TRUE );
			for(;;){
				LibTchWait( &tsts );
				if( (tsts.act & ACT_MAKE) != 0x00000000L)
				break;
			}
			GameInit();  				/* Initial */
			Disp( &Gameinfo );
		}
		Gameinfo.sts=GAME_STOP;
		return TRUE;
	}
	else
		return FALSE;
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
		sc->lv1score = score;
	}else if(gamelevel == 1){
		strcpy(sc->lv2,name);
		sc->lv2score = score;
	}else{
		strcpy(sc->lv3,name);
		sc->lv3score = score;
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

	int		score1;

	if(entry_cond == FALSE)
		return(FALSE);

	if(gamelevel == 0)
		score1 = sc->lv1score;
	else if(gamelevel == 1)
		score1 = sc->lv2score;
	else
		score1 = sc->lv3score;

	if((score1 == 0xffff || score1 < score ) && score > 0)
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

	if(sc->lv1score == 0xffff && sc->lv2score == 0xffff && sc->lv3score == 0xffff)
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

	char *num2="    ";

	LibPutFarData(DSP_HS_X,DSP_HS_Y,106);

	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+22,135);		/* Level1 Name */
	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+57,135);		/* Level2 Name */
	LibPutFarData(DSP_HS_X+56,DSP_HS_Y+92,135);		/* Level3 Name */

	LibPutFarData(DSP_HS_X+8,DSP_HS_Y+108,113);		/* for Clr */
	LibPutFarData(DSP_HS_X+90,DSP_HS_Y+108,114);	/* for OK */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+4,"Level1",50);			/* Level1 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+13,"Score",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+25,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+39,"Level2",50);		/* Level2 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+48,"Score",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+60,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT2,DSP_HS_X+48,DSP_HS_Y+74,"Level3",50);		/* Level3 */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+83,"Score",30);			/* Time */
	LibPutProStr(IB_PFONT1,DSP_HS_X+9,DSP_HS_Y+95,"Name",30);			/* Name */

	LibPutProStr(IB_PFONT1,DSP_HS_X+17,DSP_HS_Y+112,"Clr",30);			/* Clr */
	LibPutProStr(IB_PFONT1,DSP_HS_X+99,DSP_HS_Y+112,"OK",30);			/* OK */

	if(sc->lv1score != 0xffff){
		LibNumToStr(num2, sc->lv1score, 4);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+13,num2,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+25,sc->lv1,58);
	}

	if(sc->lv2score != 0xffff){
		LibNumToStr(num2, sc->lv2score, 4);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+48,num2,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+60,sc->lv2,58);
	}

	if(sc->lv3score != 0xffff){
		LibNumToStr(num2, sc->lv3score, 4);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+83,num2,34);
		LibPutProStr(IB_PFONT1,DSP_HS_X+60,DSP_HS_Y+95,sc->lv3,58);
	}
	LibPutDisp();
}


/******************************************************************************
*	[Title]		Shuffle
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
******************************************************************************/
static void Shuffle( struct work_area *p, int	n, bool f ) {
	
	int	i,j;
	int	x,y;
	
	if(f){
		/*get three random number*/
		for(j=0;j<3;j++){	
			do{
				x=rand()%CARD_NUM+1;
				y=rand()%CARD_NUM+1;
			}while(Data[x][y]!=0);
	
			i=rand()%Cir_num+1;
			Data[x][y]=i;
		}
	}
	
	/*get Next three random number*/
	for(j=0;j<n;j++){
		do{
			x=rand()%CARD_NUM+1;
			y=rand()%CARD_NUM+1;
		}while(((Data[x][y]>=1)&&(Data[x][y]<=Cir_num))||((Data[x][y]>=10)&&(Data[x][y]<=Cir_num*10)));

		i=(rand()%Cir_num+1)*10;
		Data[x][y]=i;
	}
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