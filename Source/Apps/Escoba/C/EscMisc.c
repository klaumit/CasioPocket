/******************************************************************************
*  Program Title  Escoba
*  Module         Miscellaneous functions and menu options
*  Author         J. Marcos Matilla (marcosjo@wanadoo.es)
*  Date           12/2001
*  Rev            1.0
*  MMSoft 2000 - 2001 Freeware
*  Notes:   
*
******************************************************************************/
#include <escoba.h>
#include <stdrom.h>
#include <define.h>
#include <libc.h>

/*****************************************************************************/
word MPdEdt[] ={
	   14,  	 	/* New game             	*/
	   PDNTBLEND,};
word MPdSys[] = {
		38,	 	/* Set_date/time		 		*/
		33,	 	/* Sound				 			*/
		36,		/* Language						*/
		37,	 	/* Contrast			 			*/
		40,	 	/* Touch_Panel_Alignment	*/
		PDNTBLEND,
		};
word	MPdOpt[] = {
	   437,		/* Swap							*/
       10,	  	/* Pop Up tools				*/
 	  	472,    	/* Calculator					*/ 
		PDNTBLEND,};
/*****************************************************************************/

TCHTBL far TchWait[]=    
 { /* card 1 area */
 	0,0,1,1,ACT_500MSEC,OBJ_NONE,0x0000,
   TCH_END}; 


TCHTBL far TchSetLevel[]=    
 { /* Level 1*/
   SETL1_L,SETL1_T,SETL1_R,SETL1_B,ACT_BREAK_IN,OBJ_SET_LEV1,0x0000,
 	/* Level 2*/
   SETL2_L,SETL2_T,SETL2_R,SETL2_B,ACT_BREAK_IN,OBJ_SET_LEV2,0x0000,
 	/* Level 3*/
   SETL3_L,SETL3_T,SETL3_R,SETL3_B,ACT_BREAK_IN,OBJ_SET_LEV3,0x0000,
   TCH_END};

TCHTBL far TchSetPlayers[]=    
 { /* 2 players game */
   SETPL2_L,SETPL2_T,SETPL2_R,SETPL2_B,ACT_BREAK_IN,OBJ_SET_PLY2,0x0000,
 	/* 3 players game */
   SETPL3_L,SETPL3_T,SETPL3_R,SETPL3_B,ACT_BREAK_IN,OBJ_SET_PLY3,0x0000,
   TCH_END};

TCHTBL far TchSetDemo[]=    
 { /* Demo option */
   SETDEMO_L,SETDEMO_T,SETDEMO_R,SETDEMO_B,ACT_BREAK_IN,OBJ_SET_DEMO,0x0000,
   TCH_END};

TCHTBL far TchSetButtons[]=    
 { /* Ok Button */
   SETB1_L,SETB1_T,SETB1_R,SETB1_B,ACT_ICON,OBJ_SET_YES,0x0000,
 	/* ESC Button */
   SETB2_L,SETB2_T,SETB2_R,SETB2_B,ACT_ICON,OBJ_SET_ESC,0x0000,
   TCH_END};

T_ICON far ButtonYES={&TchSetButtons[0],NULL,NULL,0x00};
T_ICON far ButtonESC={&TchSetButtons[1],NULL,NULL,0x00};


int DealCard (void);
static void GmPdwnSet(void);
static void PdwnFncEdt(word);
static void PdwnFncSys(word);
static void PdwnFncOpt(word);

/******************************************************************************
*  [Title]        GetSeed
*  [Arguments]    
*  [ReturnVAL]    int: Seed value
*  [Description]  
*   Obtains a number based on actual time          
******************************************************************************/
int GetSeed(void)
{ 	byte stime[7];
  	int s;
  	int i;

  LibGetDateTimeM(stime);
  s=0;
  for (i=0;i<7;i++)
  	s+=stime[i];
  return(s%0xFFFF);
}  


/******************************************************************************
*  [Title]        Shuffle
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Shuffle the deck
******************************************************************************/
void Suffle ()
{  byte TmpDeck[MAXCARDS];
   int calc;
   int crd;

   srand(GetSeed());
   /*Initializing cards status */
   for (crd=0;crd<MAXCARDS;crd++)
      TmpDeck[crd]=TRUE;
     
   for (crd=0;crd<MAXCARDS;crd++) {
      calc=rand()%MAXCARDS;               /* Random card number               */
      while (TmpDeck[calc]!=TRUE){        /* Search for a non assigned card   */
         calc++;
         calc%=MAXCARDS;                  /* Only between the 40 first ones   */
         }
      deck.card[crd]=calc;                /* Assign then cards to the deck    */
      TmpDeck[calc]=FALSE;
      }  
   deck.remain=MAXCARDS;                  /* Remaining cards in deck          */
}

 
/******************************************************************************
*  [Title]        DealPlayer
*  [Arguments]    nplayer
*  [ReturnVAL]    
*  [Description]  
*  Deal three cards for each player
******************************************************************************/
void DealPlayers ()
{  int pl;
   int crd;
   char texto[40];
   
   LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1); /* Clear area */
   strcpy(texto,GetMsg(3));
   LibPutProStr(IB_CG57FONT,1+(DISCARD_L+DISCARD_R-LibGetProStrSize(IB_CG57FONT,texto))/2,DISCARD_T+5,texto,96);  /* deal */
  	LibPutDisp();	
	CheckHrdIcn(1);
   	
   for (crd=0;crd<CARDS_IN_PLAYER;crd++) 
      for (pl=PL_ME;pl<nPlayers;pl++) {
         player[pl].card[crd].Id=DealCard();
         switch(pl) {
         	case PL_ME:
         		SlideCard(DECKT_L,DECKT_T,Pl0Coords[crd][LEFT],Pl0Coords[crd][TOP],CARD_BACK);
         		break;
         	case PL_RIGHT:
         		SlideCard(DECKT_L,DECKT_T,Pl1Coords[crd][LEFT],Pl1Coords[crd][TOP],CARD_BACK);
         		break;
         	case PL_LEFT:
         		SlideCard(DECKT_L,DECKT_T,Pl2Coords[crd][LEFT],Pl2Coords[crd][TOP],CARD_BACK);
         		break;
         	}			
         DispPlayerCards(pl);    
     		CheckHrdIcn(1);
      	}
	LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1);      
}        


/******************************************************************************
*  [Title]        DealTable
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Deal's the first 4 cards on the table at the begining
******************************************************************************/
void DealTable ()
{  int crd;
   
   for (crd=0;crd<4;crd++) {
      table[crd].Id=DealCard();
  		SlideCard(DECKT_L,DECKT_T,TblCoords[crd][LEFT],TblCoords[crd][TOP],CARD_BACK);
      DispTableCards();
  		CheckHrdIcn(1);
   	}
}        
      
      
/******************************************************************************
*  [Title]        DealCard
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Draw the next card from deck and decreases the remaining cards
******************************************************************************/
int DealCard ()
{  
   if (deck.remain>0){
      deck.remain--;
      DispTableDeck();
      return(deck.card[deck.remain]);
      }
   else
      return(NOCARD);      
}     
   

/******************************************************************************
*	[Title]			GmPdwnSet
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 	
*		Prepares the pulldown menu			   
******************************************************************************/
void GmPdwnSet(void)
{
	LibPullDownInit(MPdEdt,MPdSys,MPdOpt);		/* MenuBAR Init */
}


/******************************************************************************
*	[Title]			GmPdwnFnc
*	[Arguments]		
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown option selected			   
******************************************************************************/
void GmPdwnFnc(void)
{  word	sel;
	
	GmPdwnSet();
	sel = LibPullDown();		/* PullDownMenu */
	if(sel & PULLDOWN_EDIT)	  PdwnFncEdt(sel);
	if(sel & PULLDOWN_SYSTEM) PdwnFncSys(sel);
	if(sel & PULLDOWN_OPTION) PdwnFncOpt(sel);
}


/******************************************************************************
*	[Title]			PdwnFncEdt
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu EDIT		   
******************************************************************************/
void PdwnFncEdt(word sel)
{ 
  switch(sel){
	case PDWN_EDT_NEW:       /* New game */
		Initialize=TRUE;
		break;
	default:
		break;
	}
}

/******************************************************************************
*	[Title]			PdwnFncSys
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu SYSTEM		   
******************************************************************************/
void PdwnFncSys(word sel)
{ 
  switch(sel){
	case PDWN_SYS_DTTM:		/* Set_date/time */
		LibFuncDateTime();
		break;
	case PDWN_SYS_SOUND:		/* Sound */
		LibFuncSound(0);
		break;
	case PDWN_SYS_LANG:		/* Language */
		LibFuncLang();
		break;	
	case PDWN_SYS_CNTRS:		/* Contrast */
		LibFuncContrast();
		break;
	case PDWN_SYS_TPA:		/* Touch_Panel_Alignment*/
		LibFuncDigitizer();
		break;
	default:
		break;
	}
}


/******************************************************************************
*	[Title]			PdwnFncOpt
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu OPTIONS		   
******************************************************************************/
void PdwnFncOpt(word sel)
{ 
  switch(sel){
	case PDWN_OPT_SET:
		Settings();
		break;	
	case PDWN_OPT_TOOL:
		LibFuncPtool();	
		break;
	case PDWN_OPT_CALC:
		LibCalWin(OFF);	
		break;
	default:
		break;
	}
}


/******************************************************************************
*	[Title]			Settings
*	[Arguments]		
*	[ReturnVAL]		
*  [Description] 	
*		
******************************************************************************/
void Settings (void)
{	int Win_L=16;
	int Win_T=16;
	int pl;
	int l;
	int xp,yp;
	int oldLevel=GameLevel;
	int oldDemo=GameDemo;
	int oldPlayers=nPlayers;
	bool exit;
	TCHSTS tsts;
	
	LibOpenWindow(Win_L,Win_T,128,128);
	LibPutFarData(Win_L,Win_T,106);  /* Square 128x128 */
	LibPutProStr(IB_PFONT3,Win_L+30,Win_T+6,"E S C O B A",128);  /* Title */
	
	/* Level area */
	LibBox(SETL1_L-3,SETL1_T-9,60,40,1);
	LibPutProStr(IB_PFONT1,SETL1_L,SETL1_T-12,GetMsg(20),55);	/* LEVEL 	*/
	LibPutProStr(IB_CG57FONT,SETL1_L+15,SETL1_T,GetMsg(21),55);	/* Level 1 	*/
	LibPutProStr(IB_CG57FONT,SETL2_L+15,SETL2_T,GetMsg(22),55);	/* Level 2 	*/
	LibPutProStr(IB_CG57FONT,SETL3_L+15,SETL3_T,GetMsg(23),55);	/* Level 3 	*/
	
	/* Players number */
	LibBox(SETPL2_L-3,SETPL2_T-9,60,40,1);
	LibPutProStr(IB_PFONT1,SETPL2_L,SETPL2_T-12,GetMsg(24),55);		/* PLAYERS		*/
	LibPutProStr(IB_CG57FONT,SETPL2_L+15,SETPL2_T,GetMsg(25),55);	/* 2 PLayers	*/
	LibPutProStr(IB_CG57FONT,SETPL3_L+15,SETPL3_T,GetMsg(26),55);	/* 3 PLayers	*/

	/* Demo mode Area */
	LibPutProStr(IB_PFONT1,SETDEMO_L+13,SETDEMO_T+1,GetMsg(27),80); /* DEMO MODE */

	/* Buttons area */
	LibPutFarData(SETB1_L,SETB1_T,114); 
	LibPutMessageCenter(60,SETB1_L,SETB1_R,SETB1_T+4,IB_CG57FONT); /* SET */
	LibPutFarData(SETB2_L,SETB2_T,113); 
	LibPutMessageCenter(109,SETB2_L,SETB2_R,SETB2_T+4,IB_CG57FONT); /* ESC */
					
	LibTchStackPush(TchSetLevel);
	LibTchStackPush(TchSetPlayers);
	LibTchStackPush(TchSetDemo);
	LibTchStackPush(TchSetButtons);
	
	exit=FALSE;	
	do {  /* Loop until ESC, OK or CANCEL */
		for (l=1;l<4;l++) {		/* Display actual level */
			switch (l){
				case 1:			
					xp=SETL1_L;
					yp=SETL1_T;
					break;
				case 2:
					xp=SETL2_L;
					yp=SETL2_T;
					break;
				case 3:
					xp=SETL3_L;
					yp=SETL3_T;
					break;
				}					
			if (l==GameLevel)
				LibPutFarData(xp,yp,12);
			else	
				LibPutFarData(xp,yp,13);
			}			
		if (nPlayers==2) {
			LibPutFarData(SETPL2_L,SETPL2_T,12);
			LibPutFarData(SETPL3_L,SETPL3_T,13);
			}
		else {
			LibPutFarData(SETPL2_L,SETPL2_T,13);
			LibPutFarData(SETPL3_L,SETPL3_T,12);
			}			
		if (GameDemo==TRUE)
			LibPutFarData(SETDEMO_L,SETDEMO_T,17);
		else	
			LibPutFarData(SETDEMO_L,SETDEMO_T,16);
		LibPutDispBox(Win_L,Win_T,128,128);	
			
		LibTchWait(&tsts);
		switch (tsts.obj) { 
			case OBJ_HIC_ESC:
				GameLevel=oldLevel;
				GameDemo=oldDemo;
				nPlayers=oldPlayers;
				exit=TRUE;
				break;
			case OBJ_SET_LEV1:
				GameLevel=1;
				break;
			case OBJ_SET_LEV2:
				GameLevel=2;
				break;
			case OBJ_SET_LEV3:
				GameLevel=3;
				break;
			case OBJ_SET_PLY2:
				nPlayers=2;
				break;
			case OBJ_SET_PLY3:
				nPlayers=3;
				break;
			case OBJ_SET_DEMO:
				if (GameDemo==TRUE)
					GameDemo=FALSE;
				else
					GameDemo=TRUE;
				break;
			case OBJ_SET_YES:				
				if (LibIconClick(&ButtonYES,&tsts)==TRUE)
					exit=TRUE;
				break;	
			case OBJ_SET_ESC:
				if (LibIconClick(&ButtonESC,&tsts)==TRUE){
					GameLevel=oldLevel;		/* Restore old settings */
					GameDemo=oldDemo;
					nPlayers=oldPlayers;
					exit=TRUE;
					}
				break;	
			default:
				break;
			}	/* end switch */	
	} while (exit==FALSE);	/* repeat until exit option */
		
		
	LibTchStackPop();
	LibTchStackPop();
	LibTchStackPop();
	LibTchStackPop();
	LibCloseWindow();		
	LibPutDispBox(Win_L,Win_T,128,128);	

/* New settings made */
	if (GameDemo!=oldDemo)		/* New mode to play? -> Reset Game 		*/
		Initialize=TRUE;
	if (nPlayers!=oldPlayers)	/* New players in the game -> Reset Game*/
		Initialize=TRUE;
	if (Initialize==FALSE & GameLevel!=oldLevel)
		for (pl=PL_ME;pl<nPlayers;pl++)
			DispPlayerDeck(pl);
}	
	

/******************************************************************************
*	[Title]			CheckHrdIcn
*	[Arguments]		wait: time to wait in 500msec units
*	[ReturnVAL]		
*  [Description] 	
*	Wait a time interval, checking the Hard Icons for ESC or MENU	
******************************************************************************/
	
void CheckHrdIcn (int wait)
{	TCHSTS tsts;
	
	LibTchStackPush(TchWait);  /* 500 msec. wait */
	while (wait>0) {
		LibTchWait(&tsts);
		if (tsts.act==ACT_500MSEC) wait--;
		else	
			switch (tsts.obj) {
				case OBJ_HIC_ESC:
   				LibTchStackPop();					/* Restore touch 	*/
	   			if (GameDemo==TRUE){
   					Settings();
   					return;
   					}
					else			
						LibJumpMenu();
					break;
				case OBJ_HIC_MBAR:
						LibTchStackPop();				/* 500 msec. wait */
						GmPdwnFnc();
						return;
					break;
				default:
					break;
				}	
		}
	LibTchStackPop();				/* 500 msec. wait */
}	
	
	
	
	
	
	