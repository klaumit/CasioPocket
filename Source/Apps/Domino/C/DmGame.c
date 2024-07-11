/******************************************************************************
*   Program Title	Domino
*   Module			Domino selections; player and computer.
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		01/2001 
*	Rev     		1.2
*	MMSoft 2001 - Freeware
*	Notes:	
*
******************************************************************************/
#include	<stdrom.h>
#include	<define.h>
#include	<l_define.h>
#include	<libc.h>
#include	<l_libc.h>
#include	<domino.h>

void BlinkSelection(int,int);

/******************************************************************************
*	[Title]			HumanSelect
*	[Arguments]		
*	[ReturnVAL]		FALSE if not possible to place none domino
*                   TRUE if yes
*                   *dom: returned domino number
*                   *side: player side selected (LEFT or RIGHT)
*   [Description] 
*   If was not possible to match some domino, the functions returns FALSE and
*   the player can't select none.
*   if is possible, the computer selects THE FIRST (not the best) domino 
*   to play it. The human player can change this selection; but cannot place
*   a non valid piece in the wrong side.
******************************************************************************/
bool HumanSelect(int *dom,int *side)
#define NO_SELECT -1
{ TCHSTS tsts;
  int x;
  int x1,y1,x2,y2;
  int pos;
  int ficha;
  bool match;


 match=FALSE; 
 /* Check if some human's domino matches in the board */
 for (pos=0;pos<N_DOMS && match==FALSE;pos++) {
   if (Player[0].token[pos].used==FALSE) {   	
     ficha=Player[0].token[pos].domino;
     if (Domino[ficha].pts1==Board[RIGHT].end || Domino[ficha].pts2==Board[RIGHT].end)
        match=TRUE;
     if (Domino[ficha].pts1==Board[LEFT].end || Domino[ficha].pts2==Board[LEFT].end) 
        match=TRUE;
     }
   }   
 if (match==FALSE) 
   return(match);  
 pos--;  
 x=30+pos*(DOM_HEIGHT+3);
 LibCngeBoxSub(x+1,136,x+DOM_HEIGHT-2,136+DOM_WIDTH-3,IB_GDS_REV);
 LibPutDisp();
   
 while (TRUE) {   
   LibTchWait(&tsts);
   switch (tsts.obj) {   	 
     case OBJ_HIC_ESC:  /* ESC Key */
           LibJumpMenu();       	   
           break;
     case OBJ_HIC_MBAR: /* Menu bar */
      	   GmPdwnFnc();
    	   if (newgame==TRUE)
    	     return (FALSE);
      	   break;
   	 case OBJ_DOM0:  /* Domino 1 */
   	 case OBJ_DOM1:  /* Domino 2 */
   	 case OBJ_DOM2:  /* Domino 3 */
   	 case OBJ_DOM3:  /* Domino 4 */
   	 case OBJ_DOM4:  /* Domino 5 */
   	 case OBJ_DOM5:  /* Domino 6 */
   	 case OBJ_DOM6:  /* Domino 7 */
 	   pos=tsts.obj-OBJ_DOM0;
   	   if (Player[0].token[pos].used==FALSE) {
         DrawPlayerDomino(0);
         x=30+pos*(DOM_HEIGHT+3);
    	 LibCngeBoxSub(x+1,136,x+DOM_HEIGHT-2,136+DOM_WIDTH-3,IB_GDS_REV);
   	     LibPutDisp();
   	     }
   	   break;
   	 case OBJ_BOARD:
   	   if (pos!=NO_SELECT) {
     	  ficha=Player[0].token[pos].domino;     /* Domino number ot total tokens */
   	   /* Check the RIGHT side */
  	 	  x1=Board[RIGHT].tchX1;
  		  y1=Board[RIGHT].tchY1;
  		  x2=Board[RIGHT].tchX2;
  		  y2=Board[RIGHT].tchY2;
   	      if (tsts.x>x1 && tsts.x<x2 && tsts.y>y1 && tsts.y<y2) { /* Touched domino at RIGHT */	
   	        if (Domino[ficha].pts1==Board[RIGHT].end || Domino[ficha].pts2==Board[RIGHT].end) {    	      
    	      *dom=ficha;
   	          *side=RIGHT;
   	          Player[0].token[pos].used=TRUE;
   	          return (TRUE);
   	          }
   	        else    	          
   	          MsgPlayer(0,4);
   	        }
   	   /* Check the LEFT side */
  	 	  x1=Board[LEFT].tchX1;
  		  y1=Board[LEFT].tchY1;
  		  x2=Board[LEFT].tchX2;
  		  y2=Board[LEFT].tchY2;
   	      if (tsts.x>x1 && tsts.x<x2 && tsts.y>y1 && tsts.y<y2) {/* Touched domino at LEFT */
       	    if (Domino[ficha].pts1==Board[LEFT].end || Domino[ficha].pts2==Board[LEFT].end) {
     	      *dom=ficha;
   	          *side=LEFT;
   	          Player[0].token[pos].used=TRUE;
   	          return (TRUE);
   	          }
   	        else 
   	          MsgPlayer(0,4);
   	        }
   	     }   	    
   	   break;
   	 }  
  } 	 
}   
   


/******************************************************************************
*	[Title]			ComputerSelect
*	[Arguments]		
*	[ReturnVAL]		TRUE if a domino was selected
* 		            FALSE if none domino could be selected
* 	                *dom= domino selected number
* 	                side LEFT or RIGHT side to put  it
*   [Description] 
* 	Select the best domino for this turn.
*   Each domino has a score based on side.
*    - If domino matches, Sum the points (More points-> Best piece)
*    - A double value scores 10 points more
*   If the leves is easiest (level 1), selects the first possible domino 
*   If the level is hardest (level 2), selects the domino with high score.
******************************************************************************/
bool ComputerSelect(int pl,int *dom,int *side)  
{ int pts1,pts2;
  int d,s;
  int ficha;
  int best_domino;
  int best_side;
  int best_score;
  int score[2][N_DOMS];
     	

  for (d=0;d<N_DOMS;d++) {
   	 ficha=Player[pl].token[d].domino;
     pts1=Domino[ficha].pts1;
     pts2=Domino[ficha].pts2;
     for (s=LEFT;s<=RIGHT;s++)
     /* Check matching for right side */
   	    if (Player[pl].token[d].used==FALSE && (pts1==Board[s].end||pts2==Board[s].end)) {
   	 	  score[s][d]=pts1+pts2;                  /* Domino matches: Sum of points  */   	 	  
   	 	  if (pts1==pts2) score[s][d]+=10;  /* double value:   8 points */
   	      }
   	    else 
   	      score[s][d]=0;  /* Invalid Domino: 0 Points  */
     }

/* Select the first (level 1) or the best (level 2) */
  best_side=RIGHT;
  best_score=0;
  best_domino=0;
  for (d=0;d<N_DOMS;d++) {  	
    for (s=LEFT;s<=RIGHT;s++)    
      if (score[s][d]>best_score) {
      	 best_side=s;
      	 best_score=score[s][d];
      	 best_domino=d;
      	 }
    if (levelgame==1 && best_score>0)  /* Level=1 => select the first possible */
      break;
    }

  Player[pl].token[best_domino].used=TRUE;     	
  *dom=Player[pl].token[best_domino].domino;
  *side=best_side;
  if (best_score>0) {
    BlinkSelection(pl,best_domino);
    return (TRUE);   /* A domino was found */
    }
  else
    return (FALSE);  /* A domino was not found */
}  	

/******************************************************************************
*	[Title]			BlinkSelection
*	[Arguments]		pl: player number
*                   dom: domino number
*	[ReturnVAL]		
*   [Description] 
*      Blink the selected domino for a player. (Visual effect)
*     The blink period is 500msec. The number is asigned in NBLINKS definition
******************************************************************************/
void BlinkSelection(int pl,int dom)
#define NBLINKS 2
{  int x,y;   
   int blink;

   switch (pl) {
     case 0:  /* Player 0 => Human */
       break;     
     case 1:  /* Player 1 */
       x=BOARD_X2+2;
       y=20+dom*(DOM_HEIGHT+3);
       for (blink=0;blink<NBLINKS;blink++) {
          LibClrBox(x,y,4,DOM_HEIGHT);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
          LibBox(x+1,y,3,DOM_HEIGHT,1);
          LibLine(x,y,1,DOM_HEIGHT,1);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
        	  }
       break;     
     case 2:  /* Player 2 */	
       x=30+dom*(DOM_HEIGHT+3);
       y=0;
       for (blink=0;blink<NBLINKS;blink++) {
          LibClrBox(x,y,DOM_HEIGHT,4);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
       	  LibBox(x,y,DOM_HEIGHT,3,1);
       	  LibLine(x,y+3,DOM_HEIGHT,1,1);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
 		  }
       break;
     default: /* Player 3 */	
       x=0;
       y=20+dom*(DOM_HEIGHT+3);
       for (blink=0;blink<NBLINKS;blink++) {
          LibClrBox(x,y,4,DOM_HEIGHT);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
       	  LibBox(x,y,3,DOM_HEIGHT,1);
       	  LibLine(x+3,y,1,DOM_HEIGHT,1);
          LibPutDisp();
       	  LibWait(IB_500MWAIT); /* 500ms wait */
          }
       break;
     }  
}

