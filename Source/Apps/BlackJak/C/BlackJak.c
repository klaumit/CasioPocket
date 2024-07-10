/*******************************************************************
*	[Title]		BlackJack
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"BlackJak.h"
#include	"gdata.h"

/***  ***/
void GameInit( void );
void DataInit( struct work_area *p );
void BetDisplay( bool f );
void GameTchSet( bool f );
void Restart( void );
void Disp( void );
void DispCard( byte m, byte n );
void DispState( byte n );
void DispWinLost( long int n );
void DispPoint( void );
bool JudgeBC( byte n );
bool JudgeBust( byte n );
bool JudgeDouble( byte n );
bool JudgeDealerAdd( void );
void JudgeWin( void );
int CalPoint( byte n, bool a );
static int GetSeed( void );
static void Shuffle( struct work_area *p );
void BetSet( byte *EditString );
void BetCompare( void );
void GameStop( void );
void GameOver( byte n );
static void DispIllust( byte n );
static void DispBC( byte n );
void ReverseIcon( byte n );

TCHTBL far TchHeaderIcon[] = {
	
	110, 49, 130, 56,
	ACT_MAKE,
	OBJ_IC_YES0,
	0x0000,

	135, 49, 155, 56,
	ACT_MAKE,
	OBJ_IC_NO0,
	0x0000,	
	
	0, 101, 17, 108,
	ACT_MAKE,
	OBJ_IC_HIT1,
	0x0000,

	19, 101, 50, 108,
	ACT_MAKE,
	OBJ_IC_STAND1,
	0x0000,

	52, 101, 78, 108,
	ACT_MAKE,
	OBJ_IC_SPLIT1,
	0x0000,
	
	0, 151, 17, 158,
	ACT_MAKE,
	OBJ_IC_HIT2,
	0x0000,

	19, 151, 50, 158,
	ACT_MAKE,
	OBJ_IC_STAND2,
	0x0000,

	52, 151, 78, 158,
	ACT_MAKE,
	OBJ_IC_SPLIT2,
	0x0000,
	
	81, 101, 98, 108,
	ACT_MAKE,
	OBJ_IC_HIT3,
	0x0000,

	100, 101, 131, 108,
	ACT_MAKE,
	OBJ_IC_STAND3,
	0x0000,

	133, 101, 159, 108,
	ACT_MAKE,
	OBJ_IC_SPLIT3,
	0x0000,
	
	81, 151, 98, 158,
	ACT_MAKE,
	OBJ_IC_HIT4,
	0x0000,

	100, 151, 131, 158,
	ACT_MAKE,
	OBJ_IC_STAND4,
	0x0000,

	15, 88, 35, 96,
	ACT_MAKE,
	OBJ_IC_YES1,
	0x0000,

	45, 88, 65, 96,
	ACT_MAKE,
	OBJ_IC_NO1,
	0x0000,

	15, 138, 35, 146,
	ACT_MAKE,
	OBJ_IC_YES2,
	0x0000,

	45, 138, 65, 146,
	ACT_MAKE,
	OBJ_IC_NO2,
	0x0000,

	95, 88, 115, 96,
	ACT_MAKE,
	OBJ_IC_YES3,
	0x0000,

	125, 88, 145, 96,
	ACT_MAKE,
	OBJ_IC_NO3,
	0x0000,

	95, 138, 115, 146,
	ACT_MAKE,
	OBJ_IC_YES4,
	0x0000,

	125, 138, 145, 146,
	ACT_MAKE,
	OBJ_IC_NO4,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

TCHTBL far TchHeaderIcon2[] = {
	84, 68, 112, 79,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_SET,
	0x0000,

	56, 68, 83, 79,
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

TCHTBL far TchGameArea[] = {
	0, 0, 159, 159,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_GAMEAREA,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};


/*****************************************************************************/
struct work_area	gameinfo;
TCHSTS	tsts;
long int Score;
long int Bet[5],Insure[5];	/* Bet[0] id totol bet,Insure[0] not to use*/
int BetScore;
byte *num="        ";
byte Player;
int Card[5][CARD_MAX][2];	/* who, no Card, number and color */
int HandOutCount;	/* totol hand out Card's number */
int CardCount[5];	/* each Card's number */
bool Yes[5],No[5];	/* Insure or Double */
bool Hit[5],Stand[5];	/* Hit or Stand */
bool Split[5],SplitYes[5],SplitA[5];	/* Split,Split*[0] and [4] not to use */
bool CardOver[5];	/* if need Card next*/
bool Select[5];		/* if Selected Hit or Stand */
bool Bust[5],BC[5];	/* if Bust or BC*/
bool DoubleYes[5],DoubleNo[5];		/* if Double, Double[0] not to use*/
static int Illust[10][5]={{0,0,0,50,100},{80,100,0,0,0},{0,0,50,90,100},{0,0,40,90,100},
			{0,0,50,90,100},{0,0,60,30,100},{40,90,100,0,0},{30,70,100,0,0},
			{20,50,100,0,0},{0,10,90,100,0}};

/******************************************************************************
*  [Title]			BlackJack
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void) {
	byte i;
	Score=1000;
	Bet[0]=0;
	
	LibPutGraph( 0, 0, GraphIllust[ILLUST_IMAGE]);	/*Display Image*/
	LibPutDisp( );
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	GameInit( );  				/* Initial */
	Restart( );
	for(;;){
		LibTchWait( &tsts );
		
		switch( tsts.obj ) {
		case  OBJ_IC_YES0:			/* [YES0] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Yes[0]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 0 );
			Score=Score-Insure[1];	/*Lost Insure*/
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
				
			Yes[0]=FALSE;	No[0]=FALSE;
			LibGdsClr( 105, 19, 159, 60 );
			LibPutDisp();
				
			LibGdsClr( CARD_START_Y1X+1, CARD_START_Y1Y+16, CARD_START_Y1X+96, CARD_START_Y1Y+23 );
			LibPutProStr(IB_PFONT1,CARD_START_Y1X+10,CARD_START_Y1Y+17,"INSURED",100);
			LibReverse( CARD_START_Y1X+1, CARD_START_Y1Y+16, 95, 8);
				
			LibGdsClr( CARD_START_DX+1, CARD_START_DY+16, CARD_START_DX+96, CARD_START_DY+23 );
			if(JudgeBC( 0 )){
				Score=Score+Bet[0]+Insure[1];	/*Win 0*/
				DispWinLost( 0 );
				
				DispBC( 1 );	/*Dealer win by BC*/
				GameStop( );
			}else{
				LibPutProStr(IB_PFONT1,CARD_START_DX+10,CARD_START_DY+17,"NON BLACKJACK",100);
				LibReverse( CARD_START_DX+1, CARD_START_DY+16, 95, 8);
				LibPutDisp();
				LibWait(IB_1SWAIT);
				LibWait(IB_1SWAIT);
					
				DispIllust( 2 );	/*Dealer is NON BLACKJACK*/
					
				DispCard( 0, Player );	/*in order to delete Dealer's state*/
				Yes[1]=FALSE;	No[1]=FALSE;
				DispCard( 1, Player );
				DispState( 1 );
			}
			LibPutDisp();
			break;
			
		case  OBJ_IC_NO0:			/* [NO0] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(No[0]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 1 );
			Yes[0]=FALSE;	No[0]=FALSE;
			LibGdsClr( 105, 19, 159, 60 );
				
			LibGdsClr( CARD_START_DX+1, CARD_START_DY+16, CARD_START_DX+96, CARD_START_DY+23 );
			if(JudgeBC( 0 )){
				DispWinLost( -Bet[1] );		/*Lost Bet[1]*/
					
				DispBC( 1 );	/*Dealer win by BC*/
				if(Score<=0)	{GameOver( 1 );	break;}
				GameStop( );
			}else{
				LibPutProStr(IB_PFONT1,CARD_START_DX+10,CARD_START_DY+17,"NON BLACKJACK",100);
				LibReverse( CARD_START_DX+1, CARD_START_DY+16, 95, 8);
				LibPutDisp();
				LibWait(IB_1SWAIT);
				LibWait(IB_1SWAIT);
					
				DispIllust( 2 );	/*Dealer is NON BALCKJACK*/
					
				DispCard( 0, Player );	/*in order to delete Dealer's state*/
				Yes[1]=FALSE;	No[1]=FALSE;
				DispCard( 1, Player );
				DispState( 1 );
			}
			break;
			
		case  OBJ_IC_HIT1:			/* [HIT1] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Hit[1]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 2 );
			Card[1][CardCount[1]][0]=gameinfo.loc[HandOutCount].number;
			Card[1][CardCount[1]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[1]++;
			DispCard( 1, Player );
			DispState( 1 );
				
			BC[1]=JudgeBC( 1 );
			if(BC[1]==FALSE)		Bust[1]=JudgeBust( 1 );
			if(Hit[1]==TRUE){
				Hit[1]=FALSE; Stand[1]=FALSE;
				LibMesh(0, 101, 17, 8);		/*make Hit1 Mesh*/
				LibMesh(19, 101, 50, 8);	/*make Stand1 Mesh*/
			}
			if(Split[1]==TRUE){
				Split[1]=FALSE;
				LibMesh(52, 101, 26, 8);	/*make Split1 Mesh*/
			}
			LibPutDisp();
			Select[1]=TRUE;
			break;

		case  OBJ_IC_STAND1:			/* [STAND1] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Stand[1]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 3 );
			DispCard( 1, Player );
			DispState( 1 );
				
			if(Hit[1]==TRUE){
				Hit[1]=FALSE; Stand[1]=FALSE;
				LibMesh(0, 101, 17, 8);		/*make Hit1 Mesh*/
				LibMesh(19, 101, 31, 8);	/*make Stand1 Mesh*/
			}
			if(Split[1]==TRUE){
				Split[1]=FALSE;
				LibMesh(52, 101, 26, 8);		/*make Split Mesh*/
			}
			LibPutDisp();
			Select[1]=TRUE;
			CardOver[1]=TRUE;
			break;

		case  OBJ_IC_SPLIT1:			/* [SPLIT] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Split[1]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 4 );
			Player++;
			Bet[Player]=Bet[0];
			Score=Score-Bet[Player];	/*Sub Bet[Player]*/
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
			
			Card[Player][0][0]=Card[1][1][0];
			Card[Player][0][1]=Card[1][1][1];
			Card[1][1][0]=gameinfo.loc[HandOutCount].number;
			Card[1][1][1]=gameinfo.loc[HandOutCount].color;
				
			HandOutCount++;
			Card[Player][1][0]=gameinfo.loc[HandOutCount].number;
			Card[Player][1][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[Player]=2;
			SplitYes[1]=TRUE;	Split[1]=FALSE;	Select[Player]=FALSE;	CardOver[Player]=FALSE;
			if((Card[1][0][0]==Card[1][1][0])&&(Score>=Bet[1])){
				Split[1]=TRUE;	SplitYes[1]=FALSE;
			}
			for(i=1;i<=Player;i++){
				DispCard( i, Player );
				DispState( i );
			}
			LibPutDisp();
			if(SplitA[1]==TRUE){
				Hit[1]=FALSE; Stand[1]=FALSE; CardOver[1]=TRUE;	Select[1]=TRUE;
				if(BC[1]==FALSE)	DispCard( 1, Player );
				Hit[Player]=FALSE; Stand[Player]=FALSE; CardOver[Player]=TRUE;	Select[Player]=TRUE;
				if(BC[Player]==FALSE)	DispCard( Player, Player );
			}
			break;

		case  OBJ_IC_YES1:			/* [YES] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Yes[1]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 13 );
			Score=Score-Bet[1];
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
			Bet[1]=Bet[1]*2;
			LibGdsClr( 50, 8, 150, 17 );		/*Change Bet*/
			LibPutProStr(IB_PFONT1,50,10,"Bet",30);
			ltoa( Bet[1], num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibReverse( 49, 9, 101, 9);
				
			Yes[1]=FALSE;	No[1]=FALSE;	DoubleYes[1]=TRUE;
				
			Card[1][CardCount[1]][0]=gameinfo.loc[HandOutCount].number;
			Card[1][CardCount[1]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[1]++;
			DispCard( 1, Player );
			Hit[1]=FALSE; Stand[1]=FALSE; CardOver[1]=TRUE;	Select[1]=TRUE;
			for(i=1;i<=Player;i++){
				if((Yes[i]==TRUE)||(Split[i]==TRUE)){
					DispCard( i, Player );
					DispState( i );
				}
			}
			break;

		case  OBJ_IC_NO1:			/* [NO] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(No[1]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 14 );
			Yes[1]=FALSE;	No[1]=FALSE;	DoubleNo[1]=TRUE;
			DispCard( 1, Player );
			DispState( 1 );
			DispCard( 2, Player );
			DispState( 2 );
			break;
			
		case  OBJ_IC_HIT2:			/* [HIT2] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Hit[2]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 5 );
			Card[2][CardCount[2]][0]=gameinfo.loc[HandOutCount].number;
			Card[2][CardCount[2]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[2]++;
			DispCard( 2, Player );
			DispState( 2 );
				
			BC[2]=JudgeBC( 2 );
			if(BC[2]==FALSE)		Bust[2]=JudgeBust( 2 );
			if(Hit[2]==TRUE){
				Hit[2]=FALSE; Stand[2]=FALSE;
				LibMesh(0, 151, 17, 8);		/*make Hit2 Mesh*/
				LibMesh(19, 151, 31, 8);	/*make Stand2 Mesh*/
			}
			if(Split[2]==TRUE){
				Split[2]=FALSE;
				LibMesh(52, 151, 26, 8);	/*make Split2 Mesh*/
			}
			LibPutDisp();
			Select[2]=TRUE;
			break;

		case  OBJ_IC_STAND2:			/* [STAND2] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Stand[2]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 6 );
			DispCard( 0, Player );	/*in order to delete Dealer's state*/
			DispCard( 2, Player );
			DispState( 2 );
				
			if(Hit[2]==TRUE){
				Hit[2]=FALSE; Stand[2]=FALSE;
				LibMesh(0, 151, 17, 8);		/*make Hit2 Mesh*/
				LibMesh(19, 151, 31, 8);	/*make Stand2 Mesh*/
			}
			if(Split[2]==TRUE){
				Split[2]=FALSE;
				LibMesh(52, 151, 26, 8);		/*make Split2 Mesh*/
			}
			LibPutDisp();
			Select[2]=TRUE;
			CardOver[2]=TRUE;
			break;

		case  OBJ_IC_SPLIT2:			/* [SPLIT2] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Split[2]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 7 );
			Player++;
			Bet[Player]=Bet[0];
			Score=Score-Bet[Player];	/*Sub Bet[Player]*/
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
				
			Card[Player][0][0]=Card[2][1][0];
			Card[Player][0][1]=Card[2][1][1];
			Card[2][1][0]=gameinfo.loc[HandOutCount].number;
			Card[2][1][1]=gameinfo.loc[HandOutCount].color;
				
			HandOutCount++;
			Card[Player][1][0]=gameinfo.loc[HandOutCount].number;
			Card[Player][1][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[Player]=2;
			SplitYes[2]=TRUE;	Split[2]=FALSE;	Select[Player]=FALSE;	CardOver[Player]=FALSE;
			if((Card[2][0][0]==Card[2][1][0])&&(Score>=Bet[2])){
				Split[2]=TRUE;	SplitYes[2]=FALSE;
			}
			for(i=1;i<=Player;i++){
				DispCard( i, Player );
				DispState( i );
			}
			LibPutDisp();
			if(SplitA[2]==TRUE){
				Hit[2]=FALSE; Stand[2]=FALSE; CardOver[2]=TRUE;	Select[2]=TRUE;
				if(BC[2]==FALSE)	DispCard( 2, Player );
				Hit[Player]=FALSE; Stand[Player]=FALSE; CardOver[Player]=TRUE;	Select[Player]=TRUE;
				if(BC[Player]==FALSE)	DispCard( Player, Player );
			}
			break;

		case  OBJ_IC_YES2:			/* [YES] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Yes[2]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 15 );
			Score=Score-Bet[2];
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
				
			Bet[2]=Bet[2]*2;
			LibGdsClr( 50, 8, 150, 17 );		/*Change Bet*/
			LibPutProStr(IB_PFONT1,50,10,"Bet",30);
			ltoa( Bet[2], num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibReverse( 49, 9, 101, 9);
				
			Yes[2]=FALSE;	No[2]=FALSE;	DoubleYes[2]=TRUE;
				
			Card[2][CardCount[2]][0]=gameinfo.loc[HandOutCount].number;
			Card[2][CardCount[2]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[2]++;
			DispCard( 2, Player );
			Hit[2]=FALSE; Stand[2]=FALSE; CardOver[2]=TRUE;	Select[2]=TRUE;
			for(i=1;i<=Player;i++){
				if((Yes[i]==TRUE)||(Split[i]==TRUE)){
					DispCard( i, Player );
					DispState( i );
				}
			}
			break;

		case  OBJ_IC_NO2:			/* [NO] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(No[2]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 16 );
			Yes[2]=FALSE;	No[2]=FALSE;	DoubleNo[2]=TRUE;
			DispCard( 2, Player );
			DispState( 2 );
			DispCard( 3, Player );
			DispState( 3 );
			break;
			
		case  OBJ_IC_HIT3:			/* [HIT3] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Hit[3]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 8 );
			Card[3][CardCount[3]][0]=gameinfo.loc[HandOutCount].number;
			Card[3][CardCount[3]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[3]++;
			DispCard( 3, Player );
			DispState( 3 );
				
			BC[3]=JudgeBC( 3 );
			if(BC[3]==FALSE)		Bust[3]=JudgeBust( 3 );
			if(Hit[3]==TRUE){
				Hit[3]=FALSE; Stand[3]=FALSE;
				LibMesh(81, 101, 17, 8);	/*make Hit3 Mesh*/
				LibMesh(100, 101, 31, 8);	/*make Stand3 Mesh*/
			}
			if(Split[3]==TRUE){
				Split[3]=FALSE;
				LibMesh(133, 101, 26, 8);	/*make Split3 Mesh*/
			}
			LibPutDisp();
			Select[3]=TRUE;
			break;

		case  OBJ_IC_STAND3:			/* [STAND3] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Stand[3]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 9 );
			DispCard( 3, Player );
			DispState( 3 );
				
			if(Hit[3]==TRUE){
				Hit[3]=FALSE; Stand[3]=FALSE;
				LibMesh(81, 101, 17, 8);	/*make Hit3 Mesh*/
				LibMesh(100, 101, 31, 8);	/*make Stand3 Mesh*/
			}
			if(Split[3]==TRUE){
				Split[3]=FALSE;
				LibMesh(133, 101, 26, 8);	/*make Split3 Mesh*/
			}
			LibPutDisp();
			Select[3]=TRUE;
			CardOver[3]=TRUE;
			break;

		case  OBJ_IC_SPLIT3:			/* [SPLIT3] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Split[3]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 10 );
			Bet[4]=Bet[0];
			Score=Score-Bet[4];	/*Sub Bet[4]*/
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
				
			Card[4][0][0]=Card[3][1][0];
			Card[4][0][1]=Card[3][1][1];
			Card[3][1][0]=gameinfo.loc[HandOutCount].number;
			Card[3][1][1]=gameinfo.loc[HandOutCount].color;
				
			HandOutCount++;
			Card[4][1][0]=gameinfo.loc[HandOutCount].number;
			Card[4][1][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[4]=2;
			Player++;
			Split[3]=FALSE;	SplitYes[3]=TRUE;	Select[4]=FALSE;	CardOver[4]=FALSE;
			if((Card[3][0][0]==Card[3][1][0])&&(Score>=Bet[3])){
				Split[3]=TRUE;	SplitYes[3]=FALSE;
			}
			for(i=1;i<=Player;i++){
				DispCard( i, Player );
				DispState( i );
			}
			LibPutDisp();
			if(SplitA[3]==TRUE){
				Hit[3]=FALSE; Stand[3]=FALSE; CardOver[3]=TRUE;	Select[3]=TRUE;
				if(BC[3]==FALSE)	DispCard( 3, Player );
				Hit[Player]=FALSE; Stand[Player]=FALSE; CardOver[Player]=TRUE;	Select[Player]=TRUE;
				if(BC[Player]==FALSE)	DispCard( Player, Player );
			}
			break;

		case  OBJ_IC_YES3:			/* [YES] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Yes[3]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 17 );
			Score=Score-Bet[3];
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
				
			Bet[3]=Bet[3]*2;
			LibGdsClr( 50, 8, 150, 17 );		/*Change Bet*/
			LibPutProStr(IB_PFONT1,50,10,"Bet",30);
			ltoa( Bet[3], num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibReverse( 49, 9, 101, 9);
				
			Yes[3]=FALSE;	No[3]=FALSE;	DoubleYes[3]=TRUE;
				
			Card[3][CardCount[3]][0]=gameinfo.loc[HandOutCount].number;
			Card[3][CardCount[3]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[3]++;
			DispCard( 3, Player );
			Hit[3]=FALSE; Stand[3]=FALSE; CardOver[3]=TRUE;	Select[3]=TRUE;
			for(i=1;i<=Player;i++){
				if((Yes[i]==TRUE)||(Split[i]==TRUE)){
					DispCard( i, Player );
					DispState( i );
				}
			}
			break;

		case  OBJ_IC_NO3:			/* [NO] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(No[3]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 18 );
			Yes[3]=FALSE;	No[3]=FALSE;	DoubleNo[3]=TRUE;
			DispCard( 3, Player );
			DispState( 3 );
			DispCard( 4, Player );
			DispState( 4 );
			break;
			
		case  OBJ_IC_HIT4:			/* [HIT4] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Hit[4]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 11 );
			Card[4][CardCount[4]][0]=gameinfo.loc[HandOutCount].number;
			Card[4][CardCount[4]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[4]++;
			DispCard( 4, Player );
			DispState( 4 );
				
			BC[4]=JudgeBC( 4 );
			if(BC[4]==FALSE)		Bust[4]=JudgeBust( 4 );
			if(Hit[4]==TRUE){
				Hit[4]=FALSE; Stand[4]=FALSE;
				LibMesh(81, 151, 17, 8);	/*make Hit4 Mesh*/
				LibMesh(100, 151, 31, 8);	/*make Stand4 Mesh*/
			}
			if(Split[4]==TRUE){
				Split[4]=FALSE;
				LibMesh(133, 151, 26, 8);	/*make Split4 Mesh*/
			}
			LibPutDisp();
			Select[4]=TRUE;
			break;

		case  OBJ_IC_STAND4:			/* [STAND4] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Stand[4]==FALSE)	break;	/*Now can't operate*/
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 12 );
			if(Hit[4]==TRUE){
				Hit[4]=FALSE; Stand[4]=FALSE;
				LibMesh(81, 151, 17, 8);	/*make Hit4 Mesh*/
				LibMesh(100, 151, 31, 8);	/*make Stand4 Mesh*/
			}
			if(Split[4]==TRUE){
				Split[4]=FALSE;
				LibMesh(133, 151, 26, 8);	/*make Split4 Mesh*/
			}
			LibPutDisp();
			CardOver[4]=TRUE;	Select[4]=TRUE;
			break;

		case  OBJ_IC_YES4:			/* [YES] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(Yes[4]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 19 );
			Score=Score-Bet[4];
			ltoa( Score, num, 10);
			LibGdsClr( 80, 0, 150, 7 );
			LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);	/*Change Score*/
			
			Bet[4]=Bet[4]*2;
			LibGdsClr( 50, 8, 150, 17 );		/*Change Bet*/
			LibPutProStr(IB_PFONT1,50,10,"Bet",30);
			ltoa( Bet[4], num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibReverse( 49, 9, 101, 9);
				
			Yes[4]=FALSE;	No[4]=FALSE;	DoubleYes[4]=TRUE;
				
			Card[4][CardCount[4]][0]=gameinfo.loc[HandOutCount].number;
			Card[4][CardCount[4]][1]=gameinfo.loc[HandOutCount].color;
			HandOutCount++;
			CardCount[4]++;
			DispCard( 4, Player );
			Hit[4]=FALSE; Stand[4]=FALSE; CardOver[4]=TRUE;	Select[4]=TRUE;
			for(i=1;i<=Player;i++){
				if((Yes[i]==TRUE)||(Split[i]==TRUE)){
					DispCard( i, Player );
					DispState( i );
				}
			}
			break;

		case  OBJ_IC_NO4:			/* [NO] */
			if(gameinfo.sts != GAME_RUN)  break;
			if(No[4]==FALSE)	break;
			if(tsts.act!=ACT_MAKE)	break;
			ReverseIcon( 20 );
			Yes[4]=FALSE;	No[4]=FALSE;	DoubleNo[4]=TRUE;
			DispCard( 4, Player );
			DispState( 4 );
			break;
		
		default:
			break;
		}
		
		BetCompare();
	}
}

/******************************************************************************
*	[Title]		Initial
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void GameInit( void ) {

  	gameinfo.sts=GAME_STOP;
	BetDisplay( TRUE );
	GameTchSet( FALSE );
	srand( GetSeed() );		/* Randomize */
}

/******************************************************************************
*	[Title]		Touch Stack Push
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void GameTchSet( bool f ){

	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	if(f==FALSE)	LibTchStackPush( TchHeaderIcon );
	else	LibTchStackPush( TchGameArea );
	LibTchInit();
}


/******************************************************************************
*	[Title]		BetGraph Display
*	[Arguments]	TRUE:display "How much you bet?"
*	[ReturnVAL]	None
******************************************************************************/
void BetDisplay( bool f ){
	
	char	*num="        ";
	byte	money[NAME_MAX+1];		/* Buffer for text input */
	
	LibClrDisp();
	
	LibGdsBoxCmp(1,1,48,17);		/*D's Box*/
	LibPutProStr(IB_PFONT1,50,1,"SCORE",80);	/* SCORE */
	LibPutProStr(IB_PFONT1,50,10,"BET",30);		/* BET */
	
	ltoa(Score, num, 10);
	LibPutProStr(IB_PFONT1,150-5*strlen(num),1,num,30);

	if(f){
		LibPutProStr(IB_PFONT3,23,4,"P",20);	/* P */
		LibPutProStr(IB_PFONT3,20,35,"How much you bet?",130);	
		LibPutFarData(56,68,8);
		LibPutProStr(IB_PFONT1,64,70,"Clr",18);				/* Clr */
		LibPutProStr(IB_PFONT1,91,70,"Set",18);				/* Set */	
		BetSet( money );
		LibPutDisp();
	}
	else{
		LibPutProStr(IB_PFONT3,23,4,"D",20);	/* D */
		ltoa( Bet[0], num, 10);
		LibPutProStr(IB_PFONT1,150-5*strlen(num),10,num,30);
	}
	LibPutDisp();
}

/******************************************************************************
*	[Title]		BouadInitial
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
static void BetSet(byte *EditString){

TXTP		EventPrm;				/* Declaration of structure for text input */
TCHTBL 		TchTxtBuf[3];			/* Text touch area table */
T_SCR_POS	ComScr;					/* Scroll bar position information */
word		GdTbl[1];				/* Guidance comment table */

byte		key_cd;
TCHSTS		tsts;
byte		input_end;

	GdTbl[0]		= 0xffff;			/* No Guidance comment */
	EventPrm.st_x	= 119;				/* Start coordinate (X) of text display */
	EventPrm.st_y	= 11;				/* Start coordinate (Y) of text display */
	EventPrm.ed_x	= 150;				/* End coordinate (X) of text display  */
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
				case OBJ_IC_SET:
					if(LibBlockIconClick( &TiconSet, &tsts , B_ICON_RIGHT ) == TRUE){
						Bet[0]=atol( EditString );
						if((Bet[0]>Score)||(Bet[0]==0)){
							EditString[0] = 0;
							LibTxtInit(&EventPrm);
							break;
						}
						
						while(LibTchStackPop()!=NULL)
							LibTchInit();
							
						input_end = TRUE;
						Score=Score-Bet[0];	/*Change Score*/
						BetDisplay( FALSE );
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
		
		if( ((key_cd >= '0') && (key_cd <= '9')) || (key_cd == 0xf1) ) 
			LibTxtInp(key_cd,&tsts,&EventPrm);
	}
}

/******************************************************************************
*	[Title]		RestartProcedure
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void Restart( void ) {
	
	BetDisplay( FALSE );	/*display Score and Bet*/
	DataInit( &gameinfo );
	gameinfo.sts=GAME_RUN;
	Disp( );	/*display dealer and Play's Card and State*/
}

/******************************************************************************
*	[Title]		BouadInitial
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
*******************************************************************************/
void DataInit( struct work_area *p ) {

	int i, j;
	
	/* Initial array and variable*/
	for(i=0;i<5;i++){
		CardCount[i]=0;
		Yes[i]=FALSE;	No[i]=FALSE;
		Hit[i]=FALSE;	Stand[i]=FALSE;
		Split[i]=FALSE;	SplitYes[i]=FALSE;	SplitA[i]=FALSE;
		Select[i]=TRUE;	CardOver[i]=TRUE;
		Bust[i]=FALSE;	BC[i]=FALSE;
		Insure[i]=0;	DoubleYes[i]=FALSE;	DoubleNo[i]=FALSE;
	}
	HandOutCount=4;	CardCount[0]=2;	CardCount[1]=2;	Select[0]=FALSE;	Select[1]=FALSE;
	Bet[1]=Bet[0];	Insure[1]=(Bet[1]+1)/2;	Bet[2]=0;	Bet[3]=Bet[2];	Bet[4]=Bet[2];
	Player=1;	CardOver[0]=FALSE;	CardOver[1]=FALSE;
	
	if(Score==0)	BetScore=0;
	if(Bet[0]>=Score)	BetScore=1;
	else BetScore=2;
	
	/* make cards */
	for(i=0; i<13; i++){
		for(j=0;j<4;j++) {
			p->loc[i*4+j].number = i+1;
			p->loc[i*4+j].color = j;
		}
	}
	
	/* shuffle the cards */
	Shuffle(p);
	
	/* initial Card[][][] */
	for(i=0; i<5; i++){
		for(j=2;j<CARD_MAX;j++){
			Card[i][j][0] = 0;
			Card[i][j][1] = -1;
		}
	}
	Card[1][0][0]=p->loc[0].number;
	Card[1][0][1]=p->loc[0].color;
	Card[0][0][0]=p->loc[1].number;
	Card[0][0][1]=p->loc[1].color;
	Card[1][1][0]=p->loc[2].number;
	Card[1][1][1]=p->loc[2].color;
	Card[0][1][0]=p->loc[3].number;
	Card[0][1][1]=p->loc[3].color;
	
	p->sts=GAME_RUN;
}

/******************************************************************************
*	[Title]		BouadInitialDisplay
*	[Arguments]	None
*	[ReturnVAL]	None
*******************************************************************************/
void Disp( void ) {
	
	LibGdsClr( 0, 18, 159, 159 );
	DispCard( 0, Player );	/*Display Dealer's Card*/
	DispCard( 1, Player );	/*Display Play1's Card*/
	DispState( 1 );			/*Display Play1's State*/
	BC[1]=JudgeBC(1);
	if(BC[1]==TRUE){
		LibGdsClr( 0, 101, 159, 109);	/*Delete Player1's State*/
		LibPutDisp( );
		Score=Score+Bet[0]+Bet[1]+(Bet[1]+1)/2;
		LibWait(IB_1SWAIT);
		LibWait(IB_1SWAIT);
		
		if(Score>=999999)	GameOver( 0 );
		else{
			DispBC( 0 );	/*Player win by BC*/
			GameStop();
		}
	}else{
		DispState( 0 );			/*Display Dealer's State*/
		if(BC[0]==TRUE){
			Hit[1]=FALSE; Stand[1]=FALSE; Yes[1]=FALSE; No[1]=FALSE; Split[1]=FALSE;
			DispCard( 1, Player );
			LibWait(IB_1SWAIT);
			LibWait(IB_1SWAIT);
		
			DispBC( 1 );	/*Dealer win by BC*/
			if(Score<=0)	GameOver( 1 );
			else	GameStop();
		}
	}
}

/******************************************************************************
*	[Title]		DispCard
*	[Arguments]	m(Dealer and Players) , n(Player's number)
*	[ReturnVAL]	None
*******************************************************************************/
void DispCard( byte m, byte n ) {
	
	int i,j;
				
	switch	(m){
		
		case	0:
			/* display Dealer's cards */
			LibGdsClr(0,CARD_START_DY,159,CARD_START_Y1Y-1);
			j = (LCD_X_DOT-CARD_SIZE_X)/CardCount[0];
			if(CardOver[0]==FALSE){
				LibPutGraph( CARD_START_DX, CARD_START_DY, GraphCards[4]);	/*Display Dealer's first card*/
				for(i=1; i<CardCount[0]; i++){
  					LibPutGraph(CARD_START_DX+i*j, CARD_START_DY, GraphCards[Card[0][i][1]]);
					LibPutProStr(IB_PFONT3, CARD_START_DX+i*j+3, CARD_START_DY+3, Num2Char[Card[0][i][0]], 15);
				}
			}else{
				for(i=0; i<CardCount[0]; i++){
  					LibPutGraph(CARD_START_DX+i*j, CARD_START_DY, GraphCards[Card[0][i][1]]);
					LibPutProStr(IB_PFONT3, CARD_START_DX+i*j+3, CARD_START_DY+3, Num2Char[Card[0][i][0]], 15);
				}
			}
			break;
			
		case	1:
			/* display Player1's cards */
			if(n<=2){
				LibGdsClr(0,CARD_START_Y1Y-1,159,CARD_START_Y2Y-1);
				j = (LCD_X_DOT-CARD_SIZE_X)/CardCount[1];
			}
			else{
				LibGdsClr(0,CARD_START_Y1Y-1,79,CARD_START_Y2Y-1);
			  	j = (LCD_X_DOT/2-CARD_SIZE_X)/CardCount[1];
			}
			for(i=0; i<CardCount[1]; i++){
  				LibPutGraph(CARD_START_Y1X+i*j, CARD_START_Y1Y, GraphCards[Card[1][i][1]]);
				LibPutProStr(IB_PFONT3, CARD_START_Y1X+i*j+3, CARD_START_Y1Y+3, Num2Char[Card[1][i][0]], 15);
			}
			break;
			
		case	2:
			/* display Player2's cards */
			if(CardCount[2]==0) break;
			if(n<=2){
				LibGdsClr(0,CARD_START_Y2Y-1,159,159);
				j = (LCD_X_DOT-CARD_SIZE_X)/CardCount[2];
			}
			else{
				if(CardCount[4]==0)		LibGdsClr(0,CARD_START_Y2Y-1,159,159);
				else	LibGdsClr(0,CARD_START_Y2Y-1,79,159);
			  	j = (LCD_X_DOT/2-CARD_SIZE_X)/CardCount[2];
			}
			for(i=0; i<CardCount[2]; i++){
  				LibPutGraph(CARD_START_Y2X+i*j, CARD_START_Y2Y, GraphCards[Card[2][i][1]]);
				LibPutProStr(IB_PFONT3, CARD_START_Y2X+i*j+3, CARD_START_Y2Y+3, Num2Char[Card[2][i][0]], 15);
			}
			break;
			
		case	3:
			/* display Player3's cards */
			if(CardCount[3]==0) break;
			if(n<=2){
				LibGdsClr(80,CARD_START_Y1Y-1,159,CARD_START_Y2Y-1);
				j = (LCD_X_DOT-CARD_SIZE_X)/CardCount[3];
			}
			else{
				LibGdsClr(80,CARD_START_Y1Y-1,159,CARD_START_Y2Y-1);
			  	j = (LCD_X_DOT/2-CARD_SIZE_X)/CardCount[3];
			}
			for(i=0; i<CardCount[3]; i++){
  				LibPutGraph(CARD_START_Y1X+80+i*j, CARD_START_Y1Y, GraphCards[Card[3][i][1]]);
				LibPutProStr(IB_PFONT3, CARD_START_Y1X+80+i*j+3, CARD_START_Y1Y+3, Num2Char[Card[3][i][0]], 15);
			}
			break;
			
		case	4:
			/* display Player4's cards */
			if(CardCount[4]==0) break;
			if(n<=2){
				LibGdsClr(80,CARD_START_Y2Y-1,159,159);
				j = (LCD_X_DOT-CARD_SIZE_X)/CardCount[4];
			}
			else{
				LibGdsClr(80,CARD_START_Y2Y-1,159,159);
			  	j = (LCD_X_DOT/2-CARD_SIZE_X)/CardCount[4];
			}
			for(i=0; i<CardCount[4]; i++){
  				LibPutGraph(CARD_START_Y2X+80+i*j, CARD_START_Y2Y, GraphCards[Card[4][i][1]]);
				LibPutProStr(IB_PFONT3, CARD_START_Y2X+80+i*j+3, CARD_START_Y2Y+3, Num2Char[Card[4][i][0]], 15);
			}
			break;
	}
	
	LibPutDisp();
}

/******************************************************************************
*	[Title]		DispState
*	[Arguments]	Dealer and Players's number
*	[ReturnVAL]	None
*******************************************************************************/
void DispState( byte n ) {
				
	switch	(n){
		case	0:	/*Display Dealer's State*/
			if((Card[0][1][0]==1)&&(CardCount[0]==2)&&(Score>=Insure[1])){	/* I state */
				LibPutProStr(IB_PFONT3,106,20,"Do you",80);
				LibPutProStr(IB_PFONT3,115,32,"insure?",80);
				/*display Yes0 and No0*/
				LibPutProStr(IB_PFONT1,112,50,"YES",30);
				LibReverse( 110, 49, 20, 8);
				LibPutProStr(IB_PFONT1,140,50,"NO",30);
				LibReverse( 135, 49, 20, 8);
				Yes[0]=TRUE; No[0]=TRUE;
				Hit[1]=FALSE; Stand[1]=FALSE; Yes[1]=FALSE; No[1]=FALSE; Split[1]=FALSE;
				DispCard( 1, Player );
				break;
			}
			BC[0]=JudgeBC( 0 );
			if(BC[0]==TRUE){
				DispWinLost( -Bet[1] );
				break;
			}
			if((BC[0]==FALSE)&&(Card[0][1][0]>10)){
				LibGdsClr( CARD_START_DX+1, CARD_START_DY+16, CARD_START_DX+96, CARD_START_DY+23 );
				LibPutProStr(IB_PFONT1,CARD_START_DX+10,CARD_START_DY+17,"NON BLACKJACKER",100);
				LibReverse( CARD_START_DX+1, CARD_START_DY+16, 95, 8);
				DispCard( 1, Player );
				LibWait(IB_1SWAIT);
				LibWait(IB_1SWAIT);
				
				DispIllust( 2 );	/*Dealer is NON BLACKJACK*/
				
				DispCard( 0, Player );
				DispState( 1 );
			}
			break;
		
		case	1:	/*Display Player1's State*/
			if(JudgeDouble(1)==TRUE){	/* D Stete*/
				LibGdsClr(CARD_START_Y1X+10,CARD_START_Y1Y+15,CARD_START_Y1X+70,CARD_START_Y1Y+38);
				LibPutProStr(IB_PFONT3,CARD_START_Y1X+17,CARD_START_Y1Y+16,"Double?",80);
				/*display Yes1 and No1*/
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+17,CARD_START_Y1Y+29,"YES",30);
				LibReverse( CARD_START_Y1X+15,CARD_START_Y1Y+28,20,8 );
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+49,CARD_START_Y1Y+29,"NO",30);
				LibReverse( CARD_START_Y1X+45,CARD_START_Y1Y+28,20,8 );
				LibPutDisp();
				if(Yes[0]==FALSE){
					Yes[1]=TRUE; No[1]=TRUE;
				}
				else{
					Yes[1]=FALSE; No[1]=FALSE;
					LibMesh( CARD_START_Y1X+15,CARD_START_Y1Y+10,20,8 );	/*make Yes1 Mesh*/
					LibMesh( CARD_START_Y1X+45,CARD_START_Y1Y+10,20,8 );	/*make No1 Mesh*/				
				}
				if(Hit[1]==TRUE){
					Hit[1]=FALSE; Stand[1]=FALSE;
					LibMesh(0, 101, 17, 8);		/*make Hit1 Mesh*/
					LibMesh(19, 101, 31, 8);	/*make Stand1 Mesh*/
				}
				if(Split[1]==TRUE){
					Split[1]=FALSE;
					LibMesh(52, 101, 26, 8);	/*make Split1 Mesh*/
				}
				break;
			}

			/* N state */
			LibPutProStr(IB_PFONT1,2,102,"HIT",30);
			LibReverse( 0, 101, 17, 8);
			LibPutProStr(IB_PFONT1,20,102,"STAND",30);
			LibReverse( 19, 101, 31, 8);
			if((Yes[0]==FALSE)&&(Select[1]==FALSE)){
				Hit[1]=TRUE; Stand[1]=TRUE;
			}
			else{
				Hit[1]=FALSE; Stand[1]=FALSE;
				LibMesh(0, 101, 17, 8);		/*make Hit1 Mesh*/
				LibMesh(19, 101, 31, 8);	/*make Stand1 Mesh*/
			}
			
			if((Score>=Bet[1])&&(Card[1][0][0]==Card[1][1][0])&&(CardCount[1]==2)
			  &&(SplitYes[1]==FALSE)){/* S state */
				if(Card[1][0][0]==1)	SplitA[1]=TRUE;
				LibPutProStr(IB_PFONT1,53,102,"SPLIT",30);
				LibReverse( 52, 101, 26, 8);
				if((Yes[0]==FALSE)&&(Select[1]==FALSE)){
					Split[1]=TRUE;
				}
				else{
					Split[1]=FALSE;
					LibMesh(52, 101, 26, 8);	/*make Split1 Mesh*/		
				}
			}
			LibPutDisp();
			break;
		
		case	2:	/*Display Player2's State*/
			if(CardCount[2]==0)	break;	/*not Player2*/
			
			if(JudgeDouble(2)==TRUE){	/* D Stete*/
				LibGdsClr(CARD_START_Y2X+10,CARD_START_Y2Y+15,CARD_START_Y2X+70,CARD_START_Y2Y+38);
				LibPutProStr(IB_PFONT3,CARD_START_Y2X+17,CARD_START_Y2Y+16,"Double?",80);
				/*display Yes2 and No2*/
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+17,CARD_START_Y2Y+29,"YES",30);
				LibReverse( CARD_START_Y2X+15,CARD_START_Y2Y+28,20,8 );
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+49,CARD_START_Y2Y+29,"NO",30);
				LibReverse( CARD_START_Y2X+45,CARD_START_Y2Y+28,20,8 );
				LibPutDisp();
				if(Yes[0]==FALSE){
					Yes[2]=TRUE; No[2]=TRUE;
				}
				else{
					Yes[2]=FALSE; No[2]=FALSE;
					LibMesh( CARD_START_Y2X+15,CARD_START_Y2Y+10,20,8 );	/*make Yes2 Mesh*/
					LibMesh( CARD_START_Y2X+45,CARD_START_Y2Y+10,20,8 );	/*make No2 Mesh*/				
				}
				if(Hit[2]==TRUE){
					Hit[2]=FALSE; Stand[2]=FALSE;
					LibMesh(0, 151, 17, 8);		/*make Hit2 Mesh*/
					LibMesh(19, 151, 31, 8);	/*make Stand2 Mesh*/
				}
				if(Split[2]==TRUE){
					Split[2]=FALSE;
					LibMesh(52, 151, 26, 8);	/*make Split2 Mesh*/
				}
				break;
			}

			/* N state */
			LibPutProStr(IB_PFONT1,2,152,"HIT",30);
			LibReverse( 0, 151, 17, 8);
			LibPutProStr(IB_PFONT1,20,152,"STAND",30);
			LibReverse( 19, 151, 31, 8);
			if((Yes[0]==FALSE)&&(Select[2]==FALSE)){
				Hit[2]=TRUE; Stand[2]=TRUE;
			}
			else{
				Hit[2]=FALSE; Stand[2]=FALSE;
				LibMesh(0, 151, 17, 8);		/*make Hit1 Mesh*/
				LibMesh(19, 151, 31, 8);	/*make Stand1 Mesh*/
			}
			
			if((Score>=Bet[2])&&(Card[2][0][0]==Card[2][1][0])&&(CardCount[2]==2)
			  &&(SplitYes[2]==FALSE)){/* S state */
				if(Card[1][0][0]==1)	SplitA[2]=TRUE;
				LibPutProStr(IB_PFONT1,53,152,"SPLIT",30);
				LibReverse( 52, 151, 26, 8);
				if((Yes[0]==FALSE)&&(Select[2]==FALSE)){
					Split[2]=TRUE;
				}
				else{
					Split[2]=FALSE;
					LibMesh(52, 151, 26, 8);	/*make Split2 Mesh*/		
				}
			}
			LibPutDisp();
			break;
		
		case	3:	/*Display Player3's State*/
			if(CardCount[3]==0)	break;	/*not Player3*/
			
			if(JudgeDouble(3)==TRUE){	/* D Stete*/
				LibGdsClr(CARD_START_Y1X+90,CARD_START_Y1Y+15,CARD_START_Y1X+150,CARD_START_Y1Y+38);
				LibPutProStr(IB_PFONT3,CARD_START_Y1X+97,CARD_START_Y1Y+16,"Double?",80);
				/*display Yes3 and No3*/
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+97,CARD_START_Y1Y+29,"YES",30);
				LibReverse( CARD_START_Y1X+95,CARD_START_Y1Y+28,20,8 );
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+129,CARD_START_Y1Y+29,"NO",30);
				LibReverse( CARD_START_Y1X+125,CARD_START_Y1Y+28,20,8 );
				LibPutDisp();
				Yes[3]=TRUE; No[3]=TRUE;
				if(Hit[3]==TRUE){
					Hit[3]=FALSE; Stand[3]=FALSE;
					LibMesh(81, 101, 17, 8);	/*make Hit3 Mesh*/
					LibMesh(100, 101, 31, 8);	/*make Stand3 Mesh*/
				}
				if(Split[3]==TRUE){
					Split[3]=FALSE;
					LibMesh(133, 101, 26, 8);	/*make Split3 Mesh*/
				}
				break;
			}

			/* N state */
			LibPutProStr(IB_PFONT1,82,102,"HIT",30);
			LibReverse( 80, 101, 17, 8);
			LibPutProStr(IB_PFONT1,100,102,"STAND",30);
			LibReverse( 99, 101, 31, 8);
			if((Yes[0]==FALSE)&&(Select[3]==FALSE)){
				Hit[3]=TRUE; Stand[3]=TRUE;
			}
			else{
				Hit[3]=FALSE; Stand[3]=FALSE;
				LibMesh(80, 101, 17, 8);	/*make Hit3 Mesh*/
				LibMesh(99, 101, 31, 8);	/*make Stand3 Mesh*/
			}
			
			if((Score>=Bet[3])&&(Card[3][0][0]==Card[3][1][0])&&(CardCount[3]==2)
			  &&(SplitYes[3]==FALSE)){/* S state */
				if(Card[1][0][0]==1)	SplitA[3]=TRUE;
				LibPutProStr(IB_PFONT1,133,102,"SPLIT",30);
				LibReverse( 132, 101, 26, 8);
				if((Yes[0]==FALSE)&&(Select[3]==FALSE)){
					Split[3]=TRUE;
				}
				else{
					Split[3]=FALSE;
					LibMesh(132, 101, 26, 8);	/*make Split3 Mesh*/		
				}
			}
			LibPutDisp();
			break;
		
		case	4:	/*Display Player4's State*/
			if(CardCount[4]==0)	break;	/*not Player4*/
			if(Select[4]==TRUE)	break;
			
			if(JudgeDouble(4)==TRUE){	/* D Stete*/
				LibGdsClr(CARD_START_Y2X+90,CARD_START_Y2Y+15,CARD_START_Y2X+150,CARD_START_Y2Y+38);
				LibPutProStr(IB_PFONT3,CARD_START_Y2X+97,CARD_START_Y2Y+16,"Double?",80);
				/*display Yes4 and No4*/
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+97,CARD_START_Y2Y+29,"YES",30);
				LibReverse( CARD_START_Y2X+95,CARD_START_Y2Y+28,20,8 );
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+129,CARD_START_Y2Y+29,"NO",30);
				LibReverse( CARD_START_Y2X+125,CARD_START_Y2Y+28,20,8 );
				LibPutDisp();
				Yes[4]=TRUE; No[4]=TRUE;
				if(Hit[4]==TRUE){
					Hit[4]=FALSE; Stand[4]=FALSE;
					LibMesh(80, 151, 17, 8);	/*make Hit4 Mesh*/
					LibMesh(99, 151, 31, 8);	/*make Stand4 Mesh*/
				}
				break;
			}

			/* N state */
			LibPutProStr(IB_PFONT1,82,152,"HIT",30);
			LibReverse( 80, 151, 17, 8);
			LibPutProStr(IB_PFONT1,100,152,"STAND",30);
			LibReverse( 99, 151, 31, 8);
			Hit[4]=TRUE; Stand[4]=TRUE;
			LibPutDisp();
			break;
	}
}

/******************************************************************************
*	[Title]		DispWinLost
*	[Arguments]	n(n Bet),end(Display Dealer's first Card
*	[ReturnVAL]	None
*******************************************************************************/
void DispWinLost( long int n ) {
		
	LibGdsClr( 50, 8, 150, 17 );	/*Change Bet*/
	if(n==0)	LibPutProStr(IB_PFONT1,52,9,"Tie",100);
	else{
		if(n>0){
			ltoa( n, num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibPutProStr(IB_PFONT1,52,9,"You won",100);
		}else{
			ltoa( -n, num, 10);
			LibPutProStr(IB_PFONT1,150-5*strlen(num),9,num,30);
			LibPutProStr(IB_PFONT1,52,9,"You lost",100);
		}
	}
	LibReverse( 50, 8, 100, 9);
	
	LibPutDisp();
}

/******************************************************************************
*	[Title]		JudgeDealerAdd
*	[Arguments]	Judge if Dealer add Card?
*	[ReturnVAL]	TRUE or FALSE
*******************************************************************************/
bool JudgeDealerAdd( void ) {
	
	int	i,j;
	
	/*Judge if hand out a card to Dealer*/
	i=CalPoint( 0, TRUE );
	if(i<=16){
		if((Bust[1]==TRUE)&&(CardCount[2]==0)&&(CardCount[3]==0)&&(CardCount[4]==0))	return FALSE;
		if((Bust[1]==TRUE)&&(Bust[2]==TRUE)&&(CardCount[3]==0)&&(CardCount[4]==0))	return FALSE;
		if((Bust[1]==TRUE)&&(Bust[2]==TRUE)&&(Bust[3]==TRUE)&&(CardCount[4]==0))	return FALSE;
		if((Bust[1]==TRUE)&&(Bust[2]==TRUE)&&(Bust[3]==TRUE)&&(Bust[4]==TRUE))	return FALSE;
		
		Card[0][CardCount[0]][0]=gameinfo.loc[HandOutCount].number;
		Card[0][CardCount[0]][1]=gameinfo.loc[HandOutCount].color;
		CardCount[0]++;
		HandOutCount++;
		Bust[0]=JudgeBust(0);
		if(Bust[0]==TRUE){
			for(j=1;j<=Player;j++){
				if(Hit[j]==TRUE){
					Hit[i]=FALSE; Stand[i]=FALSE;
					switch(i){
						case	1:
							LibMesh(0, 101, 17, 8);		/*make Hit1 Mesh*/
							LibMesh(19, 101, 31, 8);	/*make Stand1 Mesh*/
							break;
						case	2:
							LibMesh(0, 151, 17, 8);		/*make Hit2 Mesh*/
							LibMesh(19, 151, 31, 8);	/*make Stand2 Mesh*/
							break;
						case	3:
							LibMesh(80, 101, 17, 8);	/*make Hit3 Mesh*/
							LibMesh(99, 101, 31, 8);	/*make Stand3 Mesh*/
							break;
						case	4:
							LibMesh(80, 151, 17, 8);	/*make Hit4 Mesh*/
							LibMesh(99, 151, 31, 8);	/*make Stand4 Mesh*/
							break;
					}		
				}
			}
		}else	DispCard( 0,Player );
		return TRUE;
	}else	return FALSE;
}

/******************************************************************************
*	[Title]		JudgeWin
*	[Arguments]	Judge who win
*	[ReturnVAL]	None
*******************************************************************************/
void JudgeWin( void ) {
		
	int	Point[5];
	long int win,lost;
	int i;
	
	win=0;lost=0;
	for(i=0;i<=Player;i++){
		Point[i]=CalPoint(i,TRUE);
		if(Point[i]>21)	Point[i]=0;
	}
	for(i=1;i<=Player;i++){
		if(DoubleYes[i]==TRUE)	Score=Score+Bet[i]/2;
		if(SplitYes[i]==TRUE)	Score=Score+Bet[i];
		if(Point[i]>Point[0]){
			if(BC[i]==TRUE)	win=win+Bet[i]+(Bet[i]+1)/2;
			else	win=win+Bet[i];
		}else{
			if(Point[i]<Point[0])	lost=lost+Bet[i];
		}
	}
	
	Score=(Score+Bet[0])+win-lost;
	DispWinLost( (win-lost) );
	DispPoint( );
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	
	if(((win-lost)>0)&&(BetScore==0))	{DispIllust( 3 );	return;}
	if(((win-lost)>0)&&(BetScore==1))	{DispIllust( 4 );	return;}
	if(((win-lost)>0)&&(BetScore==2))	{DispIllust( 5 );	return;}
	if(((win-lost)<0)&&(BetScore==0))	{DispIllust( 6 );	return;}
	if(((win-lost)<0)&&(BetScore==1))	{DispIllust( 7 );	return;}
	if(((win-lost)<0)&&(BetScore==2))	{DispIllust( 8 );	return;}
	if((win-lost)==0)	DispIllust( 9 );	return;
}

/******************************************************************************
*	[Title]		CalPoint
*	[Arguments]	Dealer and Players
*	[ReturnVAL]	Point
*******************************************************************************/
int CalPoint( byte n, bool f ) {

	int temp[CARD_MAX][2];
	int i,j,k;
	
	j=0;	k=0;
	for(i=0;i<CardCount[n];i++){
		if(Card[n][i][0]==1)	k=k+1;
		if(Card[n][i][0]>10)	temp[i][0]=10;
		else	temp[i][0]=Card[n][i][0];
		j=j+temp[i][0];
	}	/*j is min Point*/
	if(f==FALSE)	return j;
			
	for(i=0;i<k;i++){
		if((j+10)<=21)	j=j+10;
	}
	return j;
}

/******************************************************************************
*	[Title]		JudgeBC
*	[Arguments]	Dealer You1 or You2
*	[ReturnVAL]	TRUE or FALSE
*******************************************************************************/
bool JudgeBC( byte n ) {
	int j;
	long int k;
	
	if((Card[n][0][0]!=1)&&(Card[n][1][0]!=1))	return FALSE;
	if((Card[n][0][0]<=10)&&(Card[n][1][0]<=10))	return FALSE;
	j=CalPoint( n, TRUE );
	if((j==21)&&(CardCount[n]==2)){
		CardOver[n]=TRUE;	Select[n]=TRUE;
		k=Bet[n]+(Bet[n]+1)/2;
		switch(n){
			case	0:
				DispCard( 0, Player );
				LibPutProStr(IB_PFONT1,CARD_START_DX+10,CARD_START_DY+17,"BLACK JACKER",100);
				LibReverse( CARD_START_DX+1, CARD_START_DY+16, 95, 8);
				LibPutDisp( );
				break;
			case	1:
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+2,CARD_START_Y1Y+17,"BLACK JACKER",100);
				LibReverse( CARD_START_Y1X+1, CARD_START_Y1Y+16, 75, 8);
				DispWinLost( k );
				break;
			case	2:
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+2,CARD_START_Y2Y+17,"BLACK JACKER",100);
				LibReverse( CARD_START_Y2X+1, CARD_START_Y2Y+16, 75, 8);
				DispWinLost( k );
				break;
			case	3:
				LibPutProStr(IB_PFONT1,CARD_START_Y1X+82,CARD_START_Y1Y+17,"BLACK JACKER",100);
				LibReverse( CARD_START_Y1X+81, CARD_START_Y1Y+16, 75, 8);
				DispWinLost( k );
				break;
			case	4:
				LibPutProStr(IB_PFONT1,CARD_START_Y2X+82,CARD_START_Y2Y+17,"BLACK JACKER",100);
				LibReverse( CARD_START_Y2X+81, CARD_START_Y2Y+16, 75, 8);
				DispWinLost( k );
				break;
		}
		return TRUE;
	}else	return FALSE;
}

/******************************************************************************
*	[Title]		JudgeBust
*	[Arguments]	Dealer You1 or You2
*	[ReturnVAL]	TRUE or FALSE
*******************************************************************************/
bool JudgeBust( byte n ) {
	
	int j;
	
	j=CalPoint( n, FALSE );
	if(j>21){
		CardOver[n]=TRUE;	Select[n]=TRUE;
		switch(n){
			case	0:
			DispCard( 0, Player );
			LibPutProStr(IB_PFONT1,CARD_START_DX+10,CARD_START_DY+17,"BUST",100);
			LibReverse( CARD_START_DX+1, CARD_START_DY+16, 95, 8);
			LibPutDisp( );
			break;
			case	1:
			LibPutProStr(IB_PFONT1,CARD_START_Y1X+2,CARD_START_Y1Y+17,"BUST",100);
			LibReverse( CARD_START_Y1X+1, CARD_START_Y1Y+16, 75, 8);
			DispWinLost( -Bet[1] );
			break;
			case	2:
			LibPutProStr(IB_PFONT1,CARD_START_Y2X+2,CARD_START_Y2Y+17,"BUST",100);
			LibReverse( CARD_START_Y2X+1, CARD_START_Y2Y+16, 75, 8);
			DispWinLost( -Bet[2] );
			break;
			case	3:
			LibPutProStr(IB_PFONT1,CARD_START_Y1X+82,CARD_START_Y1Y+17,"BUST",100);
			LibReverse( CARD_START_Y1X+81, CARD_START_Y1Y+16, 75, 8);
			DispWinLost( -Bet[3] );
			break;
			case	4:
			LibPutProStr(IB_PFONT1,CARD_START_Y2X+82,CARD_START_Y2Y+17,"BUST",100);
			LibReverse( CARD_START_Y2X+81, CARD_START_Y2Y+16, 75, 8);
			DispWinLost( -Bet[4] );
			break;
		}
		return TRUE;
	}else
		return	FALSE;
}

/******************************************************************************
*	[Title]		JudgeDouble
*	[Arguments]	Dealer Players
*	[ReturnVAL]	TRUE or FALSE
*******************************************************************************/
bool JudgeDouble( byte n ) {
	
	int i,j;
			
	if(Card[n][0][0]==1)	i=11;
	else{
		if(Card[n][0][0]>10)	i=10;
		else	i=Card[n][0][0];
	}		
	if(Card[n][1][0]==1)	j=11;
	else{
		if(Card[n][1][0]>10)	j=10;
		else	j=Card[n][1][0];
	}
	if(((i+j)==11)&&(CardCount[n]==2)&&(Bet[n]<=Score)
		&&(DoubleYes[n]==FALSE)&&(DoubleNo[n]==FALSE))	return	TRUE;
	else	return	FALSE;
}


/******************************************************************************
*	[Title]		Shuffle
*	[Arguments]	struct work_area *p
*	[ReturnVAL]	None
******************************************************************************/
static void Shuffle( struct work_area *p ) {
	int i, j, pos1, pos2;
	struct CARD tmpcard;

	for( i=0; i<CARD_MAX; i++ ){

		for(j=0; j<10; j++)		
			pos1=rand()%CARD_MAX;

		for(j=0; j<10; j++)		
			pos2=rand()%CARD_MAX;

		tmpcard = p->loc[pos1];
		p->loc[pos1] = p->loc[pos2];
		p->loc[pos2] = tmpcard;
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

/******************************************************************************
*	[Title]		BetCompare
*	[Arguments]	None
*	[ReturnVAL]	
******************************************************************************/
void BetCompare( void ) {
		
	int i;
	
	if(gameinfo.sts==GAME_STOP)	return;
	if((Select[1]==TRUE)&&(Select[2]==TRUE)&&(Select[3]==TRUE)&&
	  (Select[4]==TRUE)&&(CardOver[0]==FALSE)){
		if(JudgeDealerAdd()==FALSE){
			CardOver[0]=TRUE;
			if((CardOver[1]==TRUE)&&(CardOver[2]==TRUE)&&(CardOver[3]==TRUE)
			  &&(CardOver[4]==TRUE)&&(Bust[0]==FALSE))	DispCard( 0,Player );
		}else{
			if(Bust[0]==TRUE){	/*Dealer Bust*/
				JudgeWin( );
				if(Score>=999999)	{GameOver( 0 );	return;}
				if(Score<=0)	{GameOver( 1 );	return;}
				GameStop( );
				return;
			}
				
			if((CardOver[1]==TRUE)&&(CardOver[2]==TRUE)&&(CardOver[3]==TRUE)
			  &&(CardOver[4]==TRUE)){
				while(JudgeDealerAdd()==TRUE)	JudgeDealerAdd();
				CardOver[0]=TRUE;
				if(Bust[0]==FALSE)	DispCard( 0, Player );
			}
		}
		for(i=1;i<=Player;i++){
			if(CardOver[i]==FALSE){
				Select[i]=FALSE;
				DispCard( i, Player);
				DispState(i);
			}
		}
	}
	
	if((CardOver[0]==TRUE)&&(CardOver[1]==TRUE)&&(CardOver[2]==TRUE)
		&&(CardOver[3]==TRUE)&&(CardOver[4]==TRUE)){
			if(Bust[0]==FALSE)	DispCard( 0, Player );
				JudgeWin();
				if(Score>=999999)	{GameOver( 0 );	return;}
				if(Score<=0)	{GameOver( 1 );	return;}
				GameStop( );
				return;
	}
	if(CardOver[0]==TRUE){
		for(i=1;i<=Player;i++){
			if(CardOver[i]==FALSE){
				Select[i]=FALSE;
				DispCard( i, Player );
				DispState(i);
			}
		}
	}
}

/******************************************************************************
*	[Title]		GameStop
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void GameStop( void ){
	
	GameTchSet( TRUE );
	for(;;){
		LibTchWait( &tsts );
		if( (tsts.act & ACT_MAKE) != 0x00000000L)
			break;
	}
	gameinfo.sts = GAME_STOP;
	BetDisplay( TRUE );
	Restart( );
	GameTchSet( FALSE );
}

/******************************************************************************
*	[Title]		GameOver
*	[Arguments]	0(Score=0) or 1(Score>999999)
*	[ReturnVAL]	None
******************************************************************************/
void GameOver( byte n ){
			
	if(n==0){
		LibOpenWindow(0,0,160,160);
		LibPutGraph( 0, 0, GraphIllust[ILLUST_IMAGE1] );	/*Display Image1*/
		LibPutDisp( );
		LibWait(IB_1SWAIT);
		LibWait(IB_1SWAIT);
		LibWait(IB_1SWAIT);
		LibCloseWindow();
	}
	Score=1000;	
	LibPutGraph( 0, 0, GraphIllust[ILLUST_IMAGE]);	/*Display Image*/
	LibPutDisp( );
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	GameInit( );  				/* Initial */
	Restart( );	
	LibTchInit();
}

/******************************************************************************
*	[Title]		DispPoint
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void DispPoint( void ){
	
	int i;
	
	LibGdsClr(2,2,47,15);		/*D's Box*/
	i=CalPoint( 0, TRUE );
	ltoa( i, num, 10);
	LibPutProStr(IB_PFONT1,3,6,"DEALER",40);
	LibPutProStr(IB_PFONT1,37,6,num,30);
	
	LibGdsClr( 0, 101, 159, 108 );
	i=CalPoint( 1, TRUE );
	ltoa( i, num, 10);
	LibPutProStr(IB_PFONT1,11,102,"YOU",30);
	LibPutProStr(IB_PFONT1,40,102,num,30);
	
	if(CardCount[2]!=0){
		LibGdsClr( 0, 151, 159, 158 );
		i=CalPoint( 2, TRUE );
		ltoa( i, num, 10);
		LibPutProStr(IB_PFONT1,11,152,"YOU",30);
		LibPutProStr(IB_PFONT1,40,152,num,30);
	}
	
	if(CardCount[3]!=0){
		i=CalPoint( 3, TRUE );
		ltoa( i, num, 10);
		LibPutProStr(IB_PFONT1,91,102,"YOU",30);
		LibPutProStr(IB_PFONT1,120,102,num,30);
	}
	
	if(CardCount[4]!=0){
		i=CalPoint( 4, TRUE );
		ltoa( i, num, 10);
		LibPutProStr(IB_PFONT1,91,152,"YOU",30);
		LibPutProStr(IB_PFONT1,120,152,num,30);
	}
	LibPutDisp();
}

/******************************************************************************
*	[Title]		DispIllust
*	[Arguments]	n
*	[ReturnVAL]	None
******************************************************************************/
void DispIllust( byte n ){
	
	int i,j;
	
	i=(rand()%100)+1;
	for(j=0;j<5;j++){
		if(i<=Illust[n][j])	break;
	}
	
	LibOpenWindow(18,33,124,95);
	LibGdsBox( 18, 33, 142, 127);
	LibGdsBox( 19, 34, 141, 126);
	LibPutGraph( 20, 35, GraphBrow);	/*Display Brow*/
	LibPutGraph( 56, 63, GraphIllust[j]);	/*Display Illust*/
	LibPutDisp();
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibPutGraph( 20, 35, GraphBrow);	/*Display Brow*/
	LibPutDisp();
	LibWait(IB_1SWAIT);
	LibCloseWindow();
	LibPutDisp();
}

/******************************************************************************
*	[Title]		DispBC
*	[Arguments]	n(Dealer or Player)
*	[ReturnVAL]	None
******************************************************************************/
void DispBC( byte n ){
	
	int i,j;
	
	i=(rand()%100)+1;
	for(j=0;j<5;j++){
		if(i<=Illust[n][j])	break;
	}

	LibOpenWindow(0,0,160,160);
	LibPutGraph( 0, 0, GraphIllust[ILLUST_IMAGE]);	/*Display Image*/
	LibPutGraph( 56, 69, GraphIllust[j]);
	LibPutDisp( );
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
	LibPutGraph( 0, 0, GraphIllust[ILLUST_IMAGE]);	/*Display Image*/
	LibPutDisp( );
	LibWait(IB_1SWAIT);
	LibCloseWindow();
	LibPutDisp( );
}

/******************************************************************************
*	[Title]		ReverseIcon
*	[Arguments]	n(what icon?)
*	[ReturnVAL]	None
******************************************************************************/
void ReverseIcon( byte n ){
	int x1,y1,x2,y2;
	
	switch(n){
		case	0:
			x1=110; y1=49; x2=20; y2=8;
			break;
		case	1:
			x1=135; y1=49; x2=20; y2=8;
			break;
		case	2:
			x1=0; y1=101; x2=17; y2=8;
			break;
		case	3:
			x1=19; y1=101; x2=31; y2=8;
			break;
		case	4:
			x1=52; y1=101; x2=26; y2=8;
			break;
		case	5:
			x1=0; y1=151; x2=17; y2=8;
			break;
		case	6:
			x1=19; y1=151; x2=31; y2=8;
			break;
		case	7:
			x1=52; y1=151; x2=26; y2=8;
			break;
		case	8:
			x1=81; y1=101; x2=27; y2=8;
			break;
		case	9:
			x1=100; y1=101; x2=31; y2=8;
			break;
		case	10:
			x1=133; y1=101; x2=26; y2=8;
			break;
		case	11:
			x1=81; y1=151; x2=27; y2=8;
			break;
		case	12:
			x1=100; y1=151; x2=31; y2=8;
			break;
		case	13:
			x1=15; y1=88; x2=20; y2=8;
			break;
		case	14:
			x1=45; y1=88; x2=20; y2=8;
			break;
		case	15:
			x1=15; y1=138; x2=20; y2=8;
			break;
		case	16:
			x1=45; y1=138; x2=20; y2=8;
			break;
		case	17:
			x1=95; y1=88; x2=20; y2=8;
			break;
		case	18:
			x1=125; y1=88; x2=20; y2=8;
			break;
		case	19:
			x1=95; y1=138; x2=20; y2=8;
			break;
		case	20:
			x1=125; y1=138; x2=20; y2=8;
			break;
	}
	LibReverse( x1, y1, x2, y2);
	LibPutDisp( );
	LibWait(IB_500MWAIT);
	LibReverse( x1, y1, x2, y2);
	LibPutDisp( );
}