/******************************************************************************
*   Program Title	Domino
*   Module			Main game
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		01/2001 
*	Rev     		1.2
*	MMSoft 2001 - Freeware
*	Notes:	
*
*   How to play?
*   The game is a domino competition for 4 players, playing alone (no teams)
*   You can select a name and a picture for each player. See Menu->Option->Name
*   This option will be restored every time you start the game.
*
*   Also you can select the game level (Menu->Option->level1 or 2)
*
*   To play a domino (if you can) you must select a valid domino and touch the
*   last domino placed at the same side. The domino will be placed next to the last
*   and it doesn't matter the position; the program turn the domino to the correct
*   position.
*
*   The game ends when the first player finishes his dominoes or when is not possible
*   to place more (game closed!).
*   At this time, each player adds the dominoes points to his total score.
*
*   When the first player reaches 100 or more points, the game finishes.
*   The player with less points is the winner. 
*   Note: If some players draws the score, the best is the more times winner.
*   
*   You can save the game whatever you want (Menu->Edit->Save). The next time 
*   you start the game, you'll be asked to resume the saved game.
*   This includes the names, scores, dominoes and level game.
*
*   If you select a new game (Menu->Edit->New) the scores will be to zero and a new
*   game will start. The last names and pictures will stay, but the others settings
*   will be cleared.
*
*   If you exit the game without save it, you can continue the last saved game later.
*   
*   Enjoy
*               J. Marcos
*
*
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
******************************************************************************/
#include	<stdrom.h>
#include	<define.h>
#include	<l_define.h>
#include	<libc.h>
#include	<l_libc.h>
#include	<domino.h>



static int GetSeed(void);
static void InitProgram(void);
static void InitGame(void);
static void PlayGame (void);
static int RandomDomino(void);
static bool CheckEndGame(int);
static bool CheckFinalScore(void);
static void EndGame(void);
static void Ranking(void);

/* Define Touch area for Player Dominoes  */
TCHTBL far TchHuman[]=
 {30,135,42,159,
  ACT_BREAK_IN,
  OBJ_DOM0,
  0x0000,
  45,135,57,159,
  ACT_BREAK_IN,
  OBJ_DOM1,
  0x0000,
  60,135,72,159,
  ACT_BREAK_IN,
  OBJ_DOM2,
  0x0000,
  75,135,87,159,
  ACT_BREAK_IN,
  OBJ_DOM3,
  0x0000,
  90,135,102,159,
  ACT_BREAK_IN,
  OBJ_DOM4,
  0x0000,
  105,135,117,159,
  ACT_BREAK_IN,
  OBJ_DOM5,
  0x0000,
  120,135,132,159,
  ACT_BREAK_IN,
  OBJ_DOM6,
  0x0000,  
  TCH_END};
  
/* Define Touch Area in Board */
TCHTBL far TchBoard[]= {
  BOARD_X1,BOARD_Y1,BOARD_X2,BOARD_Y2,
  ACT_BREAK_IN,
  OBJ_BOARD,
  0x0000,  
  TCH_END};

/* Define Touch for YES-NO buttons */
TCHTBL far TchYesNo[]={
/* YES Button */	
  50,143,79,158,
  ACT_ICON,
  OBJ_IC_YES,
  0x0000,	
/* NO Button */  
  90,143,119,158,
  ACT_ICON,
  OBJ_IC_NO,
  0x0000,	
/* End */  
  TCH_END};  

/* Idle event */
TCHTBL far  TchIdle[]={
  0,0,1,1,
  ACT_500MSEC,
  OBJ_500MSEC,
  0x0000,
  TCH_END};	
   
/* Define YES-NO buttons */
static T_ICON far IcnYes={ &TchYesNo[0],NULL,NULL,0x00};
static T_ICON far IcnNo ={ &TchYesNo[1],NULL,NULL,0x00};
  
  bool hisc;
  
/******************************************************************************
*	[Title]			main
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 			
*      Main program	   
******************************************************************************/

void main(void)
{ 

  InitProgram();
  hisc=FALSE;
  while (!hisc) {
    PlayGame();
    if (newgame==FALSE) EndGame();
    if (!hisc) InitGame();
    }
   Ranking(); 
   LibJumpMenu();  
}

/******************************************************************************
*	[Title]			InitProgram
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 				   
*     Prepares main variables
*     Restores previous saved infrmation (names, etc)
******************************************************************************/
static void InitProgram ()
{ int i,j;  /* Indexes */
  int token;
  

 /* Randomize time */
  srand(GetSeed());

/* Set Pulldown menu */
  GmPdwnSet();
 
/* Assign points to dominoes */
  token=0;
  for (i=0;i<7;i++) 
  	for (j=i;j<7;j++) {
  	  Domino[token].pts1=i;
  	  Domino[token].pts2=j;
  	  token++;
  	} 

/* Restore names and last game (if exist) */
  LoadGame();
  if (resume==FALSE)  /* Data was not previously saved: Default settings */
  	InitGame();       /* Prepare a new Game */
  if (newgame==TRUE) levelgame=2;  /* Level game: Hard */
  
/* Game Touch Areas */
  LibTchStackClr();  
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchHuman);
  LibTchStackPush(TchBoard);
  LibTchInit();


}    


/******************************************************************************
*	[Title]			InitGame
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 				   
*     Init a new game. Reset Scores and board position for dominoes.
******************************************************************************/
static void InitGame ()
{ int i,j;  /* Indexes */
  int pl,token;
  
  
  LibClrDisp(); 
/* Init Board information */ 
  Board[RIGHT].x=86;
  Board[RIGHT].y=63;
  Board[RIGHT].dir=RIGHT; 
  Board[RIGHT].tchX1=Board[RIGHT].x-DOM_HEIGHT;
  Board[RIGHT].tchY1=Board[RIGHT].y-6;
  Board[RIGHT].tchX2=Board[RIGHT].x;
  Board[RIGHT].tchY2=Board[RIGHT].y-6+DOM_WIDTH; 
  Board[RIGHT].end=6;
  Board[RIGHT].len=0;  
  
  Board[LEFT].x=Board[RIGHT].x;
  Board[LEFT].y=Board[RIGHT].y+DOM_HEIGHT;
  Board[LEFT].dir=LEFT;
  Board[LEFT].tchX1=Board[RIGHT].tchX1;
  Board[LEFT].tchY1=Board[RIGHT].tchY1;
  Board[LEFT].tchX2=Board[RIGHT].tchX2;
  Board[LEFT].tchY2=Board[RIGHT].tchY2; 
  Board[LEFT].end=6;
  Board[LEFT].len=0;  
 
/* Reset free dominoes */
  token=0;
  for (i=0;i<7;i++) 
  	for (j=i;j<7;j++) {
  	  Domino[token].used=FALSE;
  	  token++;
  	}   	
  	
/* Deal */  	
  for (pl=0;pl<N_PLAYERS;pl++) {
    for (i=0;i<7;i++) {
      Player[pl].token[i].domino=RandomDomino();
      Player[pl].token[i].used=FALSE;
  	  }
  	DrawPlayerDomino(pl);
    if (newgame==TRUE) {
      Player[pl].score=0; /* Reset scores */
  	  Player[pl].nwins=0;      
  	  } 	
  	}
  if (newgame==TRUE) resume=FALSE;
}

/******************************************************************************
*	[Title]			PlayGame
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 				   
*     Main loop for play the game
*     If was not a resumed game (new game) is neccesary to look for the player
*     who has the double 6. Then, the game starts.
*     The game ends when a player place all his dominoes or when isn't possible
*     to place more (game closed).
******************************************************************************/
static void PlayGame()
{  int pl;
   int dom;
   int side;
   int nogame;
   int win;   


   if (resume==FALSE) { /* Search for 6-Double */
    /* Where's the DOUBLE SIX ? */
     pl=0;
     dom=0;
     while (Player[pl].token[dom].domino!=27)
       if(dom++>=N_DOMS) {
     	  dom=0;
     	  pl++; 
     	  }

     PlaceDomino(27,LEFT);   
     Player[pl].token[dom].used=TRUE;
     DrawPlayerDomino(pl);  
     MsgPlayer(pl,0); /* Start */
     }

   resume=TRUE;
   newgame=FALSE;
   nogame=0;
   win=NOWIN;
   while (nogame<4 && win==NOWIN) {  /* Main loop */
     if(++pl>=N_PLAYERS) pl=0;  /* Next player */
     
     if (pl==0)
       if (HumanSelect(&dom,&side)==TRUE) {
       	 if (newgame==TRUE) return;       	 	
       	 PlaceDomino(dom,side);
         DrawPlayerDomino(pl);
         nogame=0;
         }
       else {
       	 if (newgame==TRUE) return;
       	 if (nogame++<3) MsgPlayer(pl,3);
       	 else MsgPlayer(pl,1);
         }
     else
       if (ComputerSelect(pl,&dom,&side)==TRUE) {
    	 PlaceDomino(dom,side);
         DrawPlayerDomino(pl);
         nogame=0;
         }
       else {
       	 if (nogame++<3) MsgPlayer(pl,3);
       	 else MsgPlayer(pl,1);
         }
     if (CheckEndGame(pl)==TRUE) win=pl;
     }  

    resume=FALSE;    
    if (win!=NOWIN) { 
      Player[win].nwins++;    	
      MsgPlayer(win,2);
      }
}	
	

/******************************************************************************
*	[Title]			GetSeed
*	[Arguments]		
*	[ReturnVAL]		int: Seed value
*   [Description] 	
*     Obtains a number based on actual time			   
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
*	[Title]			RandomDomino
*	[Arguments]		
*	[ReturnVAL]		int: a free domino number
*   [Description] 
*      Select a not used random domino
******************************************************************************/
static int RandomDomino()
{ int i;
  bool loop;

  i=rand()%28;
  loop=FALSE;  /* Avoid searching forever*/
  while (Domino[i].used==TRUE) {
    i++;
    if (i>27) {
      i=0;
      if (loop==FALSE) loop=TRUE;  /* Looking for other turn */
      else return(0);              /* No more free dominoes  */
      }
    }  
  Domino[i].used=TRUE;
  return(i);
}  	


/******************************************************************************
*	[Title]			CheckEnd
*	[Arguments]		pl: player number
*	[ReturnVAL]		TRUE: The player was used all his dominoes.
*                   FALSE: The player has more pieces.
*   [Description] 
*      Check for the player's end. That is when the player has no more pieces.
******************************************************************************/
static bool CheckEndGame(int pl)
{ int i;

  for (i=0;i<N_DOMS;i++)
    if (Player[pl].token[i].used==FALSE) return (FALSE);
  return (TRUE);
}    


/******************************************************************************
*	[Title]			CheckFinalScore
*	[Arguments]		
*	[ReturnVAL]		TRUE: Some player have reached the max. score
*                   FALSE: The game continues
*   [Description] 
*      Check if some player was more than 100 Points
******************************************************************************/
static bool CheckFinalScore()
{ int pl;

  for (pl=0;pl<N_PLAYERS;pl++)
    if (Player[pl].score>=100)  return (TRUE);
  return (FALSE);
}    
	
	
/******************************************************************************
*	[Title]			EndGame
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      End of game. Shows the dominoes of each player, points, level, etc
*      At the end, waits for a key. At this moment you can save the game
*      to continue later or quit to PV menu.
******************************************************************************/

static void EndGame ()

{ int pl,dom,token;
  int counter;
  int n,l;
  char texto[30];
  TCHSTS tsts;
	   
  LibPutFarData(16,10,107);       /* rectangle 50 pixels height */
  LibPutFarData(16,65,109);       /* rectangle 62 pixels height */
  LibClrBox(18,67,124,72);
  
  LibPutMessage(311+levelgame,60,4,IB_CG57FONT); /* Level */
  LibPutMessageRight(398,100,72,IB_CG57FONT);     /* Score */
  LibPutMessage(263,106,72,IB_CG57FONT);          /* Total */
  
  for (pl=0;pl<N_PLAYERS;pl++) {
    LibClrBox(18,12,124,46);
    LibPutGraph(96,14,Image[Player[pl].pict]);
    LibPutProStr(IB_PFONT3,20,43,Player[pl].name,100);    
    LibPutProStr(IB_CG57FONT,20,85+10*pl,Player[pl].name,100);
    LibPutDisp();
    LibWait(IB_1SWAIT);
  	counter=0;
  	n=0;
    for (dom=0;dom<N_DOMS;dom++) 
    	if (Player[pl].token[dom].used==FALSE) {
    	  token=Player[pl].token[dom].domino;
    	  DrawDomino(30+n*(DOM_HEIGHT+3),15,DOWN,token,NODIR);
    	  n++;
    	  counter=counter+Domino[token].pts1+Domino[token].pts2;
    	  Player[pl].score=Player[pl].score+counter;    	  
    	  sprintf(texto,"%3d %5d",counter,Player[pl].score);
    	  LibPutProStr(IB_CG57FONT,80,85+10*pl,texto,100); 
  	      LibPutDisp();
  	      LibWait(IB_1SWAIT);  	  	      
  	      }
      sprintf(texto,"%3d %5d",counter,Player[pl].score);
   	  LibPutProStr(IB_CG57FONT,80,85+10*pl,texto,100);   	    
      LibPutDisp();
  	}

  hisc=CheckFinalScore();
  if (hisc==TRUE) {  /* Final game High score reached */
    l=LibGetProStrSize(IB_PFONT2,GetMsg(8));    
    LibPutProStr(IB_PFONT2,80-l/2,130, GetMsg(8),l);
    LibPutDisp();
    LibTchStackPush(NULL);
    LibTchStackPush(TchHardIcon);
    LibTchStackPush (TchAllDsp);
    LibTchInit();
    LibTchWait(&tsts);
    return;
    }
  
  LibPutFarData(TchYesNo[0].x1,TchYesNo[0].y1,113);
  LibPutFarData(TchYesNo[1].x1,TchYesNo[1].y1,113);
  l=LibGetProStrSize(IB_PFONT1,GetMsg(12));
  LibPutProStr(IB_PFONT1,80-l/2,130,GetMsg(12),l);
  LibPutMessageCenter(43,50,79,147,IB_CG57FONT);  /* Yes */
  LibPutMessageCenter(44,90,119,147,IB_CG57FONT); /* No */
  LibPutDisp();   
    
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchYesNo);
  LibTchInit();
  
  while (TRUE) {
    LibTchWait(&tsts);  /* Wait for a key */
    switch (tsts.obj) {
    	case OBJ_IC_YES:
    	     if (LibIconClick(&IcnYes,&tsts)==TRUE) {
               LibTchRelease();
               LibTchInit();
      	       return;
      	       }
      	  break;  
    	case OBJ_IC_NO:
    	     if (LibIconClick(&IcnNo,&tsts)==TRUE) {
               LibTchRelease();
               LibTchInit();
               LibJumpMenu();
      	       }
      	  break;  
     	case OBJ_HIC_ESC:  /* ESC Key */
          LibTchRelease();
          LibTchInit();
          LibJumpMenu();
      	  break;  
        case OBJ_HIC_MBAR: /* Menu bar */
      	  GmPdwnFnc();      	  
          break;
      	}  
     } 	      	
}  	      


/******************************************************************************
*	[Title]		 Ranking
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      Shows the final ranking. Put the players in decreasing order of points.
*      If more than one player has the same points, the player who has more 
*      winnings is the best.
******************************************************************************/

static void Ranking()  
{ TCHSTS tsts;
  int tick;
  bool img=FALSE;
  int i,j;
  T_PLAYER dummy;
  int l;
  char texto[10];
	   	   
  LibClrDisp();
  l=LibGetProStrSize(IB_PFONT3,GetMsg(9));
  LibPutProStr(IB_PFONT3,80-l/2,5,GetMsg(9),l);
 /* Draw Podium */  
  LibLine(0,145,1,-45,2);
  LibLine(0,100,50,1,2);  
  LibLine(50,101,1,-30,2);
  LibLine(50,70,60,1,2);
  LibLine(110,70,1,50,2);
  LibLine(110,120,50,1,2);
  LibLine(158,120,1,25,2);
  LibLine(159,145,-160,1,2);
  
  
  /* Sort for best scores (bubble method) */
  for (i=0;i<N_PLAYERS-1;i++)
    for (j=i;j<N_PLAYERS;j++) {
      if (Player[j].score<Player[i].score) {  /* new best score */
    	memcpy(&dummy,&Player[i],sizeof(dummy));
    	memcpy(&Player[i],&Player[j],sizeof(dummy));
    	memcpy(&Player[j],&dummy,sizeof(dummy));
    	}
      if (Player[j].score==Player[i].score && Player[j].nwins>Player[i].nwins){
      	 /* draw: check nwins */
    	memcpy(&dummy,&Player[i],sizeof(dummy));
    	memcpy(&Player[i],&Player[j],sizeof(dummy));
    	memcpy(&Player[j],&dummy,sizeof(dummy));
   		}
	  }    		 
 
/* Show Best Players */
/* 1st position */
  LibPutGraph(50,28,Image[Player[0].pict]);
  l=LibGetProStrSize(IB_CG57FONT,Player[0].name);
  LibPutProStr(IB_CG57FONT,80-l/2,75,Player[0].name,60);
  sprintf(texto,"%3d pts",Player[0].score);
  LibPutProStr(IB_PFONT2,65,85,texto,100); 
/* 2nd position */
  LibPutGraph(5,58,Image[Player[1].pict]);
  l=LibGetProStrSize(IB_CG57FONT,Player[1].name);
  LibPutProStr(IB_CG57FONT,30-l/2,105,Player[1].name,60);
  sprintf(texto,"%3d pts",Player[1].score);
  LibPutProStr(IB_PFONT2,10,115,texto,100); 
/* 3rd position */  
  LibPutGraph(115,78,Image[Player[2].pict]);
  l=LibGetProStrSize(IB_CG57FONT,Player[2].name);
  LibPutProStr(IB_CG57FONT,128-l/2,125,Player[2].name,60);
  sprintf(texto,"%3d pts",Player[2].score);
  LibPutProStr(IB_PFONT2,115,135,texto,100); 
/* Credits */  
  LibPutProStr(IB_PFONT1,20,150,"DOMINO \x0dc 2000 J. Marcos",150);
  LibPutDisp(); 
    
  LibTchStackClr();  
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  LibTchStackPush(TchIdle);
  LibTchInit();
  tick=0;
  while (TRUE) {   
    LibTchWait(&tsts);  /* Wait for a key */
    tick++;
    if (tsts.obj!=OBJ_500MSEC) { /* Stylus action */    
      LibTchRelease();
      LibTchInit();
      return;
      }
   
    if (tick>6) { /* 2 seconds */
      tick=0;
      if (img==TRUE) LibClrBox(90,18,33,50);
      else LibPutGraph(90,18,Copa);
      img=!img;
      LibPutDispBox(90,18,33,50);    
      }
    }  
}  	      


