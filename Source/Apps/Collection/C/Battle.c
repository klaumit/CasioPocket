/******************************************************************************
*   Program Title	Battle Ship
*   Module			Main game
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2000 
*	Rev     		1.3
*	MMSoft 2000 - Freeware
* Rev 1.1: Italian messages translation corrected. What happens in French?
*     1.2: The game can start without all ships in the board. (Thanks to Francesco Aldrovandi!)
*     1.3: Bad clear ship in setup phase. Sometimes clears a part of ship.
* 
*	Notes:	
*     For best translation messages to other languages, you can edit
*	  the BtlLang.c module.
*
* 				How to play?
*	Battle ship is the known ships game based, for two players: 
*	  The Human player (you) and the computer (CASIO)		
*	There are two boards one for each player
*	There are 8 ships described bellow:
*			1 with 4 cells size
*			2 with 3 cells size
*			2 with 2 cells size
*			3 with 1 cell size
*				
*	You must place the ships in your board, but following next rules:
*	The ships only can be horizontal or vertical cells.
*	The ship must fit in the area
*	A ship can't touch other ship cells.
*
*	This program is free software; you can redistribute it and/or modify	   
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*		
*	
*		Enjoy.
*				J.Marcos
******************************************************************************/
#include	<stdrom.h>
#include	<define.h>
#include	<l_define.h>
#include	<libc.h>
#include  	<l_libc.h>
#include	<battle.h>

/* Touch area for main game */
TCHTBL far TchGame[]=
 {2,40,73,111,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN,
  0x0000,
  TCH_END};    

static int GetSeed(void);
static void InitGame(void);
static void InitTchGame(void);
static byte PlayGame();
static void GmPdwnSet(void);
static bool GmPdwnFnc(void);
static bool PdwnFncEdt(word);
static bool PdwnFncSys(word);
static bool PdwnFncOpt(word);


/*****************************************************************************/
word MPdEdt[] ={
	    14, /* New game				*/
	    PDNTBLEND,};
word MPdSys[] = {
		38,	/* Set_date/time		*/
		33,	/* Sound				*/
		39,	/* Format				*/
		34,	/* Capacity		 		*/
		35,	/* Memory_management	*/
 		36,	/* Language				*/
		37,	/* Contrast				*/
		40,	/* Touch_Panel_Alignment*/
/*		41,	/* Data_communication	*/
		PDNTBLEND,
		};
word	MPdOpt[] = {
/*	    477, /* Save    (future options?)     		*/
/*	    445, /* Game-1	(future options?)			*/
		PDNTBLEND,};
/*****************************************************************************/

/******************************************************************************
*	[Title]			main
*	[Arguments]		
*	[ReturnVAL]	
*   [Description] 	
*		Main game function:
*		Initializes game
*		Prepare player ships for game
*		Play the game
*		Ask for continue or end game
******************************************************************************/

void battlemain(void)
{ byte win;

  InitGame();
  while (TRUE) {
     InitPlayerBoard();
     RandomBoard(PL_CASIO);
     InitTchGame();
     BattleScreen();  /* Initialize screen */  

     win=PlayGame(); 
     if (win!=PL_ABORT)       
       if (MsgEndGame(win)==FALSE)
      LibJumpMenu();
    }  
}


/******************************************************************************
*	[Title]			PlayGame
*	[Arguments]		
*	[ReturnVAL]		byte: winner PL_CASIO if computer wins
*								 PL_HUMAN if player wins	
*								 PL_ABORT if the player aborts the game	
*   [Description] 				   
*		Game main loop until end.
*		Executes code for each player 
*		and captures menu options
******************************************************************************/
static byte PlayGame()
{ TCHSTS tsts;       /* Variable for touch */
  byte pl;           /* Player turn.. */
  int  row,col;      /* Cell reference for shot and check */
  bool endg=FALSE;   /* The game begins... */
 

  LibTchInit();      /* Clear touch informations */
  col=0;
  row=0;    
  pl=PL_HUMAN;       /* HUMAN player starts the game */
  MsgPlayerTurn(pl);	

  /* Game main loop */
  while (endg==FALSE){
    if (pl==PL_CASIO){ /* CASIO's turn */
       ComputerShot(&col,&row); /* Computer aims */
        pl=CheckShot(pl,col,row);
       }
    else {
       LibTchWait(&tsts);          /* Wait for a touch */
       if (tsts.obj==OBJ_BD_HUMAN)
         GetCell(tsts,PL_CASIO,&col,&row);
       switch (tsts.obj) {
       	 case OBJ_HIC_ESC:  /* ESC Key */
           LibJumpMenu();       	   
         case OBJ_HIC_MBAR: /* Menu bar */
      	   GmPdwnSet();
    	   if (GmPdwnFnc()==TRUE)
    	     return(PL_ABORT);
    	   break;
    	 case OBJ_BD_HUMAN:  /* Player board */
           pl=CheckShot(pl,col,row);
           break;
         default:
           break;  
    	 }  /* end switch */
       }  /* end else */
  	endg=CheckEnd(pl);       
    }  /* end while */    
   return(pl); /* End game. pl Player wins */   
}


/******************************************************************************
*	[Title]			InitGame
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 				   
*		Initializes the game:
*		Randomize the number generator
*		loads ship sizes in the ship array
******************************************************************************/
static void InitGame()
{ int nship; 
  int ship_size;

/* randomize time */
   srand(GetSeed());
   
/* Ship sizes */   
   	for (nship=0;nship<N_SHIPS;nship++) {
     switch (nship) {
       case 0:
        ship_size=4;
        break;
       case 1:
       case 2:
        ship_size=3;
        break;
       case 3:
       case 4:
        ship_size=2;
        break;
       default:
        ship_size=1;
        break;
       } /* end switch */
     ShipInfo[nship].size=ship_size;
	} /* end for */
} /* End InitGame */


/******************************************************************************
*	[Title]			InitTchGame
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 				   
*		Initializes the active areas for the game
******************************************************************************/
void InitTchGame()
{
    /* Initialize coordinates origin for each board */   
   Board[PL_CASIO].x=1;            	/* Start coordinate X for CASIO board */
   Board[PL_CASIO].y=40;         	/* Start coordinate Y for CASIO board */  
   Board[PL_CASIO].xs=84;           /* Start coordinate X for CASIO score */
   Board[PL_CASIO].ys=120; 	        /* Start coordinate Y for CASIO score */  
   Board[PL_HUMAN].x=84;          	/* Start coordinate X for HUMAN board */
   Board[PL_HUMAN].y=40;          	/* Start coordinate Y for HUMAN board */
   Board[PL_HUMAN].xs=2;            /* Start coordinate X for HUMAN score */
   Board[PL_HUMAN].ys=120;          /* Start coordinate Y for HUMAN score */

  /* Active screen areas */
   LibTchStackClr();             /* Clear all touch areas  */
   LibTchStackPush(NULL);        /* Empty stack terminator */
   LibTchStackPush(TchHardIcon); /* Register hard icons    */
   LibTchStackPush(TchGame);     /* Register Casio's board */
   LibModeIconDsp();
} /* End InitTchGame



/******************************************************************************
*	[Title]			GetSeed
*	[Arguments]		int: obtains a number based on actual time
*	[ReturnVAL]		
*   [Description] 				   
******************************************************************************/
static int GetSeed(void)
{ byte stime[7];
  long hh,mm,ss;
  int s;

  LibGetDateTimeM(stime);
  hh=(long)((10*stime[1]&0x0F+(stime[2]&0x0F)) *3600);
  mm=(long)((10*stime[3]&0x0F+(stime[4]&0x0F)) *60);
  ss=(long)((10*stime[5]&0x0F+(stime[6]&0x0F)));  
  s=(int) ((hh+mm+ss)%32768);
  return(s);
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
	word	p_sys;

	LibPullDownInit(MPdEdt,MPdSys,MPdOpt);		/* MenuBAR Init */

 /* [System] */
	p_sys =  PULLDOWN_EDIT | 0x0000;
	LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_EDIT, p_sys);
	p_sys =  PULLDOWN_SYSTEM | 0x0000;
	LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_SYSTEM, p_sys);
	p_sys =  PULLDOWN_OPTION | 0x0000;	
	LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_OPTION, p_sys);

}


/******************************************************************************
*	[Title]			GmPdwnFnc
*	[Arguments]		
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown option selected			   
******************************************************************************/
bool GmPdwnFnc(void)
{
	word	sel;
	bool	endg=FALSE;
	
	sel = LibPullDown();		/* PullDownMenu */
	if(sel & PULLDOWN_EDIT)
		endg = PdwnFncEdt(sel);
	if(sel & PULLDOWN_SYSTEM)
		endg = PdwnFncSys(sel);
	if(sel & PULLDOWN_OPTION)
		endg = PdwnFncOpt(sel);
	return(endg);
}


/******************************************************************************
*	[Title]			PdwnFncEdt
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu EDIT		   
******************************************************************************/
bool PdwnFncEdt(word sel)
/*TAIL*/
{ bool endg = FALSE;
  switch(sel){
	case PDWN_EDT_NEW:		/* Set_date/time */
		endg=TRUE;
		break;
	default:
		break;
	}
	return(endg);
}

/******************************************************************************
*	[Title]			PdwnFncSys
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu SYSTEM		   
******************************************************************************/
bool PdwnFncSys(word sel)
/*TAIL*/
{ bool endg = FALSE;
  switch(sel){
	case PDWN_SYS_DTTM:		/* Set_date/time */
		LibFuncDateTime();
		break;
	case PDWN_SYS_SOUND:	/* Sound */
		LibFuncSound(0);
		break;
	case PDWN_SYS_FORMT:	/* Format */
		LibFuncFormat();
		break;
	case PDWN_SYS_CAPCT:	/* Capacity */
		LibFuncCapa();
		break;
	case PDWN_SYS_MMANE:	/* Memory_management */
		LibFuncMemoryManagement();
		break;
	case PDWN_SYS_LANG:		/* Language */				
		if(LibFuncLang()==TRUE){
          BattleScreen();  /* Initialize screen */  
		  MsgScore(PL_CASIO);
		  MsgScore(PL_HUMAN);		  
		  MsgPlayerTurn(PL_HUMAN);
		  }					
		break;					
	case PDWN_SYS_CNTRS:	/* Contrast */
		LibFuncContrast();
		break;
	case PDWN_SYS_TPA:		/* Touch_Panel_Alignment*/
		LibFuncDigitizer();
		break;
	default:
		break;
	}
	return(endg);
}


/******************************************************************************
*	[Title]			PdwnFncOpt
*	[Arguments]		word Sel: selected option
*	[ReturnVAL]		bool: TRUE if neccesary redisplay the screen
*   [Description] 	
*		Executes the pulldown menu OPTIONS		   
******************************************************************************/
bool PdwnFncOpt(word sel)
/*TAIL*/
{ bool endg = FALSE;
  switch(sel){
	case PDWN_OPT_SAVE:		/* Save game */
		LibPutMsgDlg("Saving...");
		endg=TRUE;
		break;
	case PDWN_OPT_LOAD:		/* Load game */
		LibPutMsgDlg("Restoring...");
		endg=TRUE;
		break;
	default:
		break;
	}
	return(endg);
}