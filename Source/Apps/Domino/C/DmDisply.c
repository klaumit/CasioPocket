/******************************************************************************
*   Program Title	Domino
*   Module			Display Game
*	Author  		J. Marcos Matilla
*	Date    		01/2001 
*	Rev     		1.2
*	MMSoft 2001 - Freeware
*	Notes:	
*   
******************************************************************************/
#include    <stdrom.h>
#include	<define.h>
#include	<libc.h>
#include	<domino.h>

static void PlaceDominoR(int);  /* Interface to place a token at Right*/
static void PlaceDominoL(int);  /* Interface to place a token at Left*/
static int CheckScroll(int);
static void DrawPoints(int,int,int,int);
static void DrawDots(int,int,int,int);
static void TchShadow(void);


/******************************************************************************
*	[Title]			PlaceDomino
*	[Arguments]		int: token number
*	[ReturnVAL]		
*   [Description] 	
*
******************************************************************************/
void PlaceDomino(int token,int side)
{ int val1,val2;
  
  val1=Domino[token].pts1;
  val2=Domino[token].pts2;

  if (side==LEFT && (Board[LEFT].end==val1 || Board[LEFT].end==val2)) {
    PlaceDominoL (token);
    Board[LEFT].dom[Board[LEFT].len]=token;    
    Board[LEFT].len++; /* dominoes counter */
    return;
    }
  if (side==RIGHT && (Board[RIGHT].end==val1 || Board[RIGHT].end==val2)) {
    PlaceDominoR (token);
    Board[RIGHT].dom[Board[RIGHT].len]=token;
    Board[RIGHT].len++; /* dominoes counter */
    return;
    }

/* Invalid domino */
   LibPutMsgDlg("[%1d-%1d] %s",val1,val2,GetMsg(4)); 
}  

/******************************************************************************
*	[Title]			PlaceDominoR
*	[Arguments]		int: token number
*	[ReturnVAL]		
*   [Description] 	
*       Place a token at "Right" side			   
*   The reference to position is the left-top corner
******************************************************************************/
static void PlaceDominoR(int token)
{ int posId;  /* Identifier for position type in the board */
  int x,y;

  x=Board[RIGHT].x;
  y=Board[RIGHT].y;

  /* Draw Domino at actual position */
  /* Check for a "double" domino direction */  
    if (Domino[token].pts1!=Domino[token].pts2) {  /* "NORMAL" => Horizontal token */
       if (Board[RIGHT].dir==RIGHT) 	/* RIGTH Direction */
         /* Checks how the domino fits at right*/
         if (x+DOM_WIDTH<BOARD_X2) posId=0;  /* The token fits NEXT to lastest */
         else if (x+DOM_HEIGHT<BOARD_X2) posId=1;  /* The token fits NEXT BUT VERTICAL */  
              else posId=2; /* The token only fits OVER the lastest*/  
       else                        	/* LEFT Direction */
         /* Checks how the domino fits at left*/
         if (x-DOM_WIDTH>BOARD_X1) posId=10; /* the token fits NEXT to lastest */
         else if (x-DOM_HEIGHT>BOARD_X1) posId=11;  /* The token fits NEXT BUT VERTICAL */  
              else posId=12;  /* Piece only fits OVER the lastest*/  
       } /* end IF NORMAL */  
    else {            /* "DOUBLE" => Vertical token */
       if (Board[RIGHT].dir==RIGHT) 	/* RIGTH Direction */
         /* Checks how the domino fits at right*/    
         if (x-DOM_HEIGHT<BOARD_X1) posId=20; /* SPECIAL CASE: starts with a "double" */
	     else if (x+DOM_WIDTH<BOARD_X2) posId=21; /* The token fits NEXT to lastest */  
       	      else if (x+DOM_HEIGHT<BOARD_X2) posId=22; /* The token fits NEXT but at corner */  
             	   else  posId=23; /* The token only fits OVER the lastest*/  
       else                        	/* LEFT Direction */       
         /* Checks how the domino fits at left*/    
         if (x+DOM_HEIGHT>BOARD_X2) posId=30; /* SPECIAL CASE: starts with a "double" */
         else if (x-DOM_WIDTH>BOARD_X1) posId=31; /* The token fits next to lastest */  
       	      else if (x-DOM_HEIGHT>BOARD_X1) posId=32; /* The token fits NEXT but end side */  
                   else  posId=33; /* The token only fits over the lastest*/  
       } /* end else IF DOUBLE */  

/* Draw Domino position based in */
   switch (posId) {
     case 0:  /* RIGHT dir: Horizontal token next to lastest   */       
       DrawDomino(x,y,RIGHT,token,RIGHT);
       Board[RIGHT].x=x+DOM_WIDTH;  /* Next X position */
       break; 
     case 1:  /* RIGHT dir: Vertical token next to lastest     */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x,y+DOM_HEIGHT-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=LEFT;       /* Change direction */           
       Board[RIGHT].x=x+DOM_HEIGHT; /* Next X position */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next X position */           
       break; 
     case 2:  /* RIGHT dir: Vertical token OVER lastest       */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x-DOM_HEIGHT,y-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=LEFT;       /* Change direction */           
       Board[RIGHT].x=x-DOM_HEIGHT; /* Next X position  */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next X position  */           
       break; 
     case 10: /* LEFT dir: Horizontal token next to lastest    */
       DrawDomino(x,y,LEFT,token,RIGHT);
       Board[RIGHT].x=x-DOM_WIDTH;  /* Next X position */
       break; 
     case 11: /* LEFT dir: Vertical token next to lastest      */  
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x-DOM_HEIGHT,y+DOM_HEIGHT-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=RIGHT;      /* Change direction */           
       Board[RIGHT].x=x-DOM_HEIGHT; /* Next X position  */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next X position  */           
       break; 
     case 12: /* LEFT dir: Vertical token OVER lastest        */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x,y-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=RIGHT;      /* Change direction */           
	   Board[RIGHT].x=x+DOM_HEIGHT; /* Next X position  */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next X position  */           
       break; 
     case 20: /* RIGHT dir: Hor.DOUBLE is the first at left  */
       DrawDomino(x,y,RIGHT,token,RIGHT);
       Board[RIGHT].x=x+DOM_WIDTH;  /* Next X position */
       break; 
     case 21: /* RIGHT dir: Vertical DOUBLE next to lastest    */
       y=y+CheckScroll(y-6);
       DrawDomino(x,y-6,DOWN,token,RIGHT);
       Board[RIGHT].x=x+DOM_HEIGHT; /* Next X position */                
       break; 
     case 22: /* RIGHT dir: Vertical DOUBLE is the last        */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x,y+DOM_HEIGHT-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].x=x+DOM_HEIGHT; /* Next X position */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next Y position */
       Board[RIGHT].dir=LEFT;       /* Change direction */           
       break; 
     case 23: /* RIGHT dir: Vertical DOUBLE OVER the lastest  */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x-DOM_HEIGHT,y-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=LEFT;       /* Change direction */           
       Board[RIGHT].x=x-DOM_HEIGHT; /* Next X position */
       Board[RIGHT].y=y-DOM_WIDTH;  /* Next X position */           
       break; 
     case 30: /* LEFT dir: Hor.DOUBLE is the first at right   */
       DrawDomino(x,y,LEFT,token,RIGHT);
       Board[RIGHT].x=x-DOM_WIDTH;  /* Next X position */
       break;  
     case 31: /* LEFT dir: Vertical DOUBLE next to lastest     */
       y=y+CheckScroll(y-6);
       DrawDomino(x-DOM_HEIGHT,y-6,DOWN,token,RIGHT);
       Board[RIGHT].x=x-DOM_HEIGHT;  /* Next X position */
       break; 
     case 32: /* LEFT dir: Vertical DOUBLE is the last         */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x-DOM_HEIGHT,y+DOM_HEIGHT-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].x=x-DOM_HEIGHT; /* Next X position */
       Board[RIGHT].y=y-DOM_WIDTH;   /* Next Y position */
       Board[RIGHT].dir=RIGHT;       /* Change direction */           
       break; 
     case 33: /* LEFT dir: Vertical DOUBLE OVER the lastest   */
       y=y+CheckScroll(y-DOM_WIDTH);
       DrawDomino(x,y-DOM_WIDTH,DOWN,token,RIGHT);
       Board[RIGHT].dir=RIGHT;       /* Change direction */           
       Board[RIGHT].x=x+DOM_HEIGHT;  /* Next X position */
       Board[RIGHT].y=y-DOM_WIDTH;   /* Next X position */           
       break; 
     }                
  if (Board[RIGHT].end==Domino[token].pts1) 
    Board[RIGHT].end=Domino[token].pts2;
  else  
    Board[RIGHT].end=Domino[token].pts1;  
      
}  /* end PlaceDominoR */


/******************************************************************************
*	[Title]			PlaceDominoL
*	[Arguments]		int: token number
*	[ReturnVAL]		
*   [Description] 	
*       Place a pice at "Left" side
*   The reference to position is the right-bottom corner
******************************************************************************/
static void PlaceDominoL(int token)
{ int posId;  /* Identifier for position type in the board */
  int x,y;

  x=Board[LEFT].x;
  y=Board[LEFT].y;

  /* Draw Domino at actual position */
  /* Check for a "double" domino direction */  
    if (Domino[token].pts1!=Domino[token].pts2) {  /* "NORMAL" => Horizontal token */
       if (Board[LEFT].dir==RIGHT) 	/* RIGTH Direction */
         /* Checks how the domino fits at right*/
         if (x+DOM_WIDTH<BOARD_X2) posId=0;  /* The token fits NEXT to lastest */
         else if (x+DOM_HEIGHT<BOARD_X2) posId=1;  /* The token fits NEXT BUT VERTICAL */  
              else posId=2; /* The token only fits OVER the lastest*/  
       else                        	/* LEFT Direction */
         /* Checks how the domino fits at left*/
         if (x-DOM_WIDTH>BOARD_X1) posId=10; /* the token fits NEXT to lastest */
         else if (x-DOM_HEIGHT>BOARD_X1) posId=11;  /* The token fits NEXT BUT VERTICAL */  
              else posId=12;  /* Piece only fits OVER the lastest*/  
       } /* end IF NORMAL */  
    else {            /* "DOUBLE" => Vertical token */
       if (Board[LEFT].dir==RIGHT) 	/* RIGTH Direction */
         /* Checks how the domino fits at right*/    
         if (x-DOM_HEIGHT<BOARD_X1) posId=20; /* SPECIAL CASE: starts with a "double" */
	     else if (x+DOM_WIDTH<BOARD_X2) posId=21; /* The token fits NEXT to lastest */  
       	      else if (x+DOM_HEIGHT<BOARD_X2) posId=22; /* The token fits NEXT but at corner */  
             	   else  posId=23; /* The token only fits OVER the lastest*/  
       else                        	/* LEFT Direction */       
         /* Checks how the domino fits at left*/    
         if (x+DOM_HEIGHT>BOARD_X2) posId=30; /* SPECIAL CASE: starts with a "double" */
         else if (x-DOM_WIDTH>BOARD_X1) posId=31; /* The token fits next to lastest */  
       	      else if (x-DOM_HEIGHT>BOARD_X1) posId=32; /* The token fits NEXT but end side */  
                   else  posId=33; /* The token only fits over the lastest*/  
       } /* end else IF DOUBLE */  

/* Draw Domino position based in */
   switch (posId) {
     case 0:  /* RIGHT dir: Horizontal token next to lastest   */       
       DrawDomino(x,y,RIGHT,token,LEFT);
       Board[LEFT].x=x+DOM_WIDTH;  /* Next X position */
       break; 
     case 1:  /* RIGHT dir: Vertical token next to lastest     */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x+DOM_HEIGHT,y-DOM_HEIGHT+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=LEFT;       /* Change direction */           
       Board[LEFT].x=x+DOM_HEIGHT; /* Next X position */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next X position */           
       break; 
     case 2:  /* RIGHT dir: Vertical token UNDER lastest       */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x,y+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=LEFT;       /* Change direction */           
       Board[LEFT].x=x-DOM_HEIGHT; /* Next X position  */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next X position  */           
       break; 
     case 10: /* LEFT dir: Horizontal token next to lastest    */
       DrawDomino(x,y,LEFT,token,LEFT);
       Board[LEFT].x=x-DOM_WIDTH;  /* Next X position */
       break; 
     case 11: /* LEFT dir: Vertical token next to lastest      */  
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x,y-DOM_HEIGHT+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=RIGHT;      /* Change direction */           
       Board[LEFT].x=x-DOM_HEIGHT; /* Next X position  */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next X position  */           
       break; 
     case 12: /* LEFT dir: Vertical token UNDER lastest        */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x+DOM_HEIGHT,y+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=RIGHT;      /* Change direction */           
	   Board[LEFT].x=x+DOM_HEIGHT; /* Next X position  */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next X position  */           
       break; 
     case 20: /* RIGHT dir: Hor.DOUBLE is the first at left  */
       DrawDomino(x,y,RIGHT,token,LEFT);
       Board[LEFT].x=x+DOM_WIDTH;  /* Next X position */
       break; 
     case 21: /* RIGHT dir: Vertical DOUBLE next to lastest    */
       y=y-CheckScroll(y+6);
       DrawDomino(x+DOM_HEIGHT,y+6,UP,token,LEFT);
       Board[LEFT].x=x+DOM_HEIGHT; /* Next X position */                
       break; 
     case 22: /* RIGHT dir: Vertical DOUBLE is the last        */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x+DOM_HEIGHT,y-DOM_HEIGHT+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].x=x+DOM_HEIGHT;            /* Next X position */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next Y position */
       Board[LEFT].dir=LEFT;       /* Change direction */           
       break; 
     case 23: /* RIGHT dir: Vertical DOUBLE UNDER the lastest  */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x,y+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=LEFT;       /* Change direction */           
       Board[LEFT].x=x-DOM_HEIGHT; /* Next X position */
       Board[LEFT].y=y+DOM_WIDTH;  /* Next X position */           
       break; 
     case 30: /* LEFT dir: Hor.DOUBLE is the first at right   */
       DrawDomino(x,y,LEFT,token,LEFT);
       Board[LEFT].x=x-DOM_WIDTH;  /* Next X position */
       break;  
     case 31: /* LEFT dir: Vertical DOUBLE next to lastest     */
       y=y-CheckScroll(y+6);
       DrawDomino(x,y+6,UP,token,LEFT);
       Board[LEFT].x=x-DOM_HEIGHT;  /* Next X position */
       break; 
     case 32: /* LEFT dir: Vertical DOUBLE is the last         */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x,y-DOM_HEIGHT+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].x=x-DOM_HEIGHT; /* Next X position */
       Board[LEFT].y=y+DOM_WIDTH;   /* Next Y position */
       Board[LEFT].dir=RIGHT;       /* Change direction */           
       break; 
     case 33: /* LEFT dir: Vertical DOUBLE UNDER the lastest   */
       y=y-CheckScroll(y+DOM_WIDTH);
       DrawDomino(x+DOM_HEIGHT,y+DOM_WIDTH,UP,token,LEFT);
       Board[LEFT].dir=RIGHT;       /* Change direction */           
       Board[LEFT].x=x+DOM_HEIGHT;  /* Next X position */
       Board[LEFT].y=y+DOM_WIDTH;   /* Next X position */           
       break; 
     }           

  if (Board[LEFT].end==Domino[token].pts1) 
    Board[LEFT].end=Domino[token].pts2;
  else  
    Board[LEFT].end=Domino[token].pts1;  

}  /* end PlaceDominoL */



/******************************************************************************
*	[Title]			DrawPlayerDomino
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*     Draw player's tokens				   
******************************************************************************/
void DrawPlayerDomino(int pl)
{  int i;
   int x,y;
   int token;

   switch (pl) {
     case 0:  /* Player 0 => Human */
       x=30;
       y=BOARD_Y2+2;
       for (i=0;i<N_DOMS;i++) {
          LibClrBox(x+i*(DOM_HEIGHT+3),y,DOM_HEIGHT,DOM_WIDTH);
       	  if (Player[pl].token[i].used==FALSE) {
       	    token=Player[pl].token[i].domino;
       	    DrawDomino(x+i*(DOM_HEIGHT+3),y,DOWN,token,NODIR);
       	    }
       	  } 
       break;     
     case 1:  /* Player 1 */
       x=BOARD_X2+2;
       y=20;
       for (i=0;i<N_DOMS;i++) {
          LibClrBox(x,y+i*(DOM_HEIGHT+3),4,DOM_HEIGHT);
       	  if (Player[pl].token[i].used==FALSE) {
         	LibBox(x+1,y+i*(DOM_HEIGHT+3),3,DOM_HEIGHT,1);
         	LibLine(x,y+i*(DOM_HEIGHT+3),1,DOM_HEIGHT,1);
       	    }
       	  }
       break;     
     case 2:  /* Player 2 */	
       x=30;
       y=0;
       for (i=0;i<N_DOMS;i++) {
          LibClrBox(x+i*(DOM_HEIGHT+3),y,DOM_HEIGHT,4);
       	  if (Player[pl].token[i].used==FALSE) {
       	    LibBox(x+i*(DOM_HEIGHT+3),y,DOM_HEIGHT,3,1);
       	    LibLine(x+i*(DOM_HEIGHT+3),y+3,DOM_HEIGHT,1,1);
       	    }
		  }
       break;
     default: /* Player 3 */	
       x=0;
       y=20;
       for (i=0;i<N_DOMS;i++) {
          LibClrBox(x,y+i*(DOM_HEIGHT+3),4,DOM_HEIGHT);
       	  if (Player[pl].token[i].used==FALSE) {
       	  	LibBox(x,y+i*(DOM_HEIGHT+3),3,DOM_HEIGHT,1);
       	    LibLine(x+3,y+i*(DOM_HEIGHT+3),1,DOM_HEIGHT,1);
       	    }
          }
       break;
     }  
 LibPutDisp();    
}


/******************************************************************************
*	[Title]			DrawDomino
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*     The reference for token is allways the Top-Left corner 
******************************************************************************/
void DrawDomino(int xpos,int ypos,int dir,int token,int side)  
{ int x,y; 
  int val1,val2;


  if (Board[side].end==Domino[token].pts1) {
    val1=Domino[token].pts1;
    val2=Domino[token].pts2;
    }
  else {
  	val1=Domino[token].pts2;
    val2=Domino[token].pts1;
    }


  /* Draw token */
  switch (dir) {
    case UP:
      if (side==LEFT) {
      	x=xpos-DOM_HEIGHT;
      	y=ypos-DOM_WIDTH;
      	}
      else {
      	x=xpos;
      	y=ypos;
      }
      LibClrBox(x,y,DOM_HEIGHT,DOM_WIDTH);
      LibBox(x,y,DOM_HEIGHT,DOM_WIDTH,1);           /* Frame */
      LibMeshLine(x+1,y+DOM_WIDTH/2,DOM_HEIGHT-2,1);
      DrawDots (x,y,val1,UP);
      DrawDots (x,y+DOM_WIDTH-12,val2,UP);
      LibPutDispBox(x,y,DOM_HEIGHT,DOM_WIDTH);
      if (side!=NODIR) {
        Board[side].tchX1=x;
        Board[side].tchY1=y;
        Board[side].tchX2=x+DOM_HEIGHT;
        Board[side].tchY2=y+DOM_WIDTH;
        }
      break;
    case DOWN:   
      if (side==LEFT) {      	
      	x=xpos-DOM_HEIGHT;
      	y=ypos;
      	}
      else {
      	x=xpos;
      	y=ypos;
      }
      LibClrBox(x,y,DOM_HEIGHT,DOM_WIDTH);      
      LibBox(x,y,DOM_HEIGHT,DOM_WIDTH,1);           /* Frame */
      LibMeshLine(x+1,y+DOM_WIDTH/2,DOM_HEIGHT-2,1);
      DrawDots (x,y,val2,DOWN);
      DrawDots (x,y+DOM_WIDTH-12,val1,UP);
      LibPutDispBox(x,y,DOM_HEIGHT,DOM_WIDTH);
      if (side!=NODIR) {      
        Board[side].tchX1=x;
        Board[side].tchY1=y;
        Board[side].tchX2=x+DOM_HEIGHT;
        Board[side].tchY2=y+DOM_WIDTH;
        }
      break;
    case LEFT:
      if (side==LEFT) {
        x=xpos-DOM_WIDTH;
        y=ypos-DOM_HEIGHT;
        }
      else {
        x=xpos-DOM_WIDTH;
      	y=ypos;
        }
      LibClrBox(x,y,DOM_WIDTH,DOM_HEIGHT);
      LibBox(x,y,DOM_WIDTH,DOM_HEIGHT,1);           /* Frame */
      LibMeshLine(x+DOM_WIDTH/2,y+1,1,DOM_HEIGHT-2);
      DrawDots (x,y,val2,RIGHT);
      DrawDots (x+DOM_WIDTH-12,y,val1,RIGHT);
      LibPutDispBox(x,y,DOM_WIDTH,DOM_HEIGHT);
      if (side!=NODIR) {
        Board[side].tchX1=x;
        Board[side].tchY1=y;
        Board[side].tchX2=x+DOM_WIDTH;      
        Board[side].tchY2=y+DOM_HEIGHT;
        }
  	  break;
    case RIGHT:
      if (side==LEFT) {
        x=xpos;
        y=ypos-DOM_HEIGHT;
        }
      else {
        x=xpos;      	
      	y=ypos;
        }
      LibClrBox(x,y,DOM_WIDTH,DOM_HEIGHT);
      LibBox(x,y,DOM_WIDTH,DOM_HEIGHT,1);           /* Frame */
      LibMeshLine(x+DOM_WIDTH/2,y+1,1,DOM_HEIGHT-2);
      DrawDots (x,y,val1,RIGHT);
      DrawDots (x+DOM_WIDTH-12,y,val2,RIGHT);
      LibPutDispBox(x,y,DOM_WIDTH,DOM_HEIGHT);
      if (side!=NODIR) {
        Board[side].tchX1=x;
        Board[side].tchY1=y;
        Board[side].tchX2=x+DOM_WIDTH;      
        Board[side].tchY2=y+DOM_HEIGHT;
        }
      break;
    }  
}  

/******************************************************************************
*	[Title]			DrawDots
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      I was used dots for points becase the boxes doesn't worked fine. Why?
******************************************************************************/
static void DrawDots(int x,int y,int val,int dir)
{ 
  switch (val) {   						 /* First value */
    case 1:    
       LibDotOn(x+5,y+5);  /* Point 4 */
       LibDotOn(x+6,y+5);
       LibDotOn(x+5,y+6);
       LibDotOn(x+6,y+6);
       break;
    case 2:
	   if (dir==UP||dir==DOWN) {  /* Vertical points */    
         LibDotOn(x+8,y+2);  /* Point 3 */
         LibDotOn(x+9,y+2);
         LibDotOn(x+8,y+3);
         LibDotOn(x+9,y+3);
         LibDotOn(x+2,y+8);  /* Point 5 */
         LibDotOn(x+3,y+8);
         LibDotOn(x+2,y+9);
         LibDotOn(x+3,y+9);
         }
	   else {                     /* Horizontal points */       
         LibDotOn(x+2,y+2);  /* Point 1 */
         LibDotOn(x+3,y+2);
         LibDotOn(x+2,y+3);
         LibDotOn(x+3,y+3);
         LibDotOn(x+8,y+8);  /* Point 7 */
         LibDotOn(x+9,y+8);
         LibDotOn(x+8,y+9);
         LibDotOn(x+9,y+9);
         }
       break;
    case 3:
	   if (dir==UP||dir==DOWN) {  /* Vertical points */    
         LibDotOn(x+8,y+2);  /* Point 3 */
         LibDotOn(x+9,y+2);
         LibDotOn(x+8,y+3);
         LibDotOn(x+9,y+3);
         LibDotOn(x+5,y+5);  /* Point 4 */
         LibDotOn(x+6,y+5);
         LibDotOn(x+5,y+6);
         LibDotOn(x+6,y+6);
         LibDotOn(x+2,y+8);  /* Point 5 */
         LibDotOn(x+3,y+8);
         LibDotOn(x+2,y+9);
         LibDotOn(x+3,y+9);
         }
	   else {                     /* Horizontal points */                
         LibDotOn(x+2,y+2);  /* Point 1 */
         LibDotOn(x+3,y+2);
         LibDotOn(x+2,y+3);
         LibDotOn(x+3,y+3);
         LibDotOn(x+5,y+5);  /* Point 4 */
         LibDotOn(x+6,y+5);
         LibDotOn(x+5,y+6);
         LibDotOn(x+6,y+6);
         LibDotOn(x+8,y+8);  /* Point 7 */
         LibDotOn(x+9,y+8);
         LibDotOn(x+8,y+9);
         LibDotOn(x+9,y+9);
         }
       break;
    case 4:
       LibDotOn(x+2,y+2);  /* Point 1 */
       LibDotOn(x+3,y+2);
       LibDotOn(x+2,y+3);
       LibDotOn(x+3,y+3);
       LibDotOn(x+8,y+2);  /* Point 3 */
       LibDotOn(x+9,y+2);
       LibDotOn(x+8,y+3);
       LibDotOn(x+9,y+3);
       LibDotOn(x+2,y+8);  /* Point 5 */
       LibDotOn(x+3,y+8);
       LibDotOn(x+2,y+9);
       LibDotOn(x+3,y+9);
       LibDotOn(x+8,y+8);  /* Point 7 */
       LibDotOn(x+9,y+8);
       LibDotOn(x+8,y+9);
       LibDotOn(x+9,y+9);
       break;
    case 5:
       LibDotOn(x+2,y+2);  /* Point 1 */
       LibDotOn(x+3,y+2);
       LibDotOn(x+2,y+3);
       LibDotOn(x+3,y+3);
       LibDotOn(x+8,y+2);  /* Point 3 */
       LibDotOn(x+9,y+2);
       LibDotOn(x+8,y+3);
       LibDotOn(x+9,y+3);
       LibDotOn(x+5,y+5);  /* Point 4 */
       LibDotOn(x+6,y+5);
       LibDotOn(x+5,y+6);
       LibDotOn(x+6,y+6);
       LibDotOn(x+2,y+8);  /* Point 5 */
       LibDotOn(x+3,y+8);
       LibDotOn(x+2,y+9);
       LibDotOn(x+3,y+9);
       LibDotOn(x+8,y+8);  /* Point 7 */
       LibDotOn(x+9,y+8);
       LibDotOn(x+8,y+9);
       LibDotOn(x+9,y+9);
       break;
    case 6:
	   if (dir==UP||dir==DOWN) {  /* Vertical points */    
         LibDotOn(x+2,y+2);  /* Point 1 */
         LibDotOn(x+3,y+2);
         LibDotOn(x+2,y+3);
         LibDotOn(x+3,y+3);
         LibDotOn(x+8,y+2);  /* Point 3 */
         LibDotOn(x+9,y+2);
         LibDotOn(x+8,y+3);
         LibDotOn(x+9,y+3);
         LibDotOn(x+2,y+5);  /* Point 4a */
         LibDotOn(x+2,y+6);
         LibDotOn(x+3,y+5);
         LibDotOn(x+3,y+6);
         LibDotOn(x+8,y+5);  /* Point 4b */
         LibDotOn(x+8,y+6);
         LibDotOn(x+9,y+5);
         LibDotOn(x+9,y+6);
         LibDotOn(x+2,y+8);  /* Point 5 */
         LibDotOn(x+3,y+8);
         LibDotOn(x+2,y+9);
         LibDotOn(x+3,y+9);
         LibDotOn(x+8,y+8);  /* Point 7 */
         LibDotOn(x+9,y+8);
         LibDotOn(x+8,y+9);
         LibDotOn(x+9,y+9);
         }
	   else {                     /* Horizontal points */                
         LibDotOn(x+2,y+2);  /* Point 1 */
         LibDotOn(x+3,y+2);
         LibDotOn(x+2,y+3);
         LibDotOn(x+3,y+3);
         LibDotOn(x+5,y+2);  /* Point 2 */
         LibDotOn(x+6,y+2);
         LibDotOn(x+5,y+3);
         LibDotOn(x+6,y+3);
         LibDotOn(x+8,y+2);  /* Point 3 */
         LibDotOn(x+9,y+2);
         LibDotOn(x+8,y+3);
         LibDotOn(x+9,y+3);
         LibDotOn(x+2,y+8);  /* Point 5 */
         LibDotOn(x+3,y+8);
         LibDotOn(x+2,y+9);
         LibDotOn(x+3,y+9);
         LibDotOn(x+5,y+8);  /* Point 6 */
         LibDotOn(x+6,y+8);
         LibDotOn(x+5,y+9);
         LibDotOn(x+6,y+9);
         LibDotOn(x+8,y+8);  /* Point 7 */
         LibDotOn(x+9,y+8);
         LibDotOn(x+8,y+9);
         LibDotOn(x+9,y+9);
         }
       break;   
    default:
       break;   
    }
}    

/******************************************************************************
*	[Title]			CheckScroll
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
******************************************************************************/
static int CheckScroll(int newY)
{ int scrsize;

   if (newY<BOARD_Y1+1) {   /* Check for Scroll Down */
     scrsize=BOARD_Y1-newY+2;
     LibGrpDwn(BOARD_X1+1,BOARD_Y1+1,BOARD_X2-BOARD_X1-1,BOARD_Y2-BOARD_Y1-1,scrsize);
     LibPutDisp();
     Board[RIGHT].y=Board[RIGHT].y+scrsize;
     Board[RIGHT].tchY1=Board[RIGHT].tchY1+scrsize;
     Board[RIGHT].tchY2=Board[RIGHT].tchY2+scrsize;
     Board[LEFT].y=Board[LEFT].y+scrsize;  
     Board[LEFT].tchY1=Board[LEFT].tchY1+scrsize;
     Board[LEFT].tchY2=Board[LEFT].tchY2+scrsize;
     return (scrsize);
     }

   if (newY>BOARD_Y2-1) {    /* Check for Scroll Up */
     scrsize=newY-BOARD_Y2+2;
     LibGrpUp(BOARD_X1+1,BOARD_Y1+1,BOARD_X2-BOARD_X1-1,BOARD_Y2-BOARD_Y1-1,scrsize);
     LibPutDisp();
     Board[RIGHT].y=Board[RIGHT].y-scrsize;
     Board[RIGHT].tchY1=Board[RIGHT].tchY1-scrsize;
     Board[RIGHT].tchY2=Board[RIGHT].tchY2-scrsize;
     Board[LEFT].y=Board[LEFT].y-scrsize;  
     Board[LEFT].tchY1=Board[LEFT].tchY1-scrsize;
     Board[LEFT].tchY2=Board[LEFT].tchY2-scrsize;
     return (scrsize);
     }
  return(0);  /* Default */  
}    

/******************************************************************************
*	[Title]			TchShadow
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      Shadows the touch area in both (Left & Right) limits to check them
*      Used only for debug
******************************************************************************/
static void TchShadow(void)
{ int x1,y1,x2,y2;

/* Right side */
  x1=Board[RIGHT].tchX1+1;
  y1=Board[RIGHT].tchY1+1;
  x2=Board[RIGHT].tchX2-1;
  y2=Board[RIGHT].tchY2-1;
  LibGdsReverse(x1,y1,x2,y2);
  LibGdsMesh(x1,y1,x2,y2);
  LibGdsMesh(x1,y1,x2,y2);
  
/* Left side */
  x1=Board[LEFT].tchX1+1;
  y1=Board[LEFT].tchY1+1;
  x2=Board[LEFT].tchX2-1;
  y2=Board[LEFT].tchY2-1;
  LibGdsReverse(x1,y1,x2,y2);
  LibGdsMesh(x1,y1,x2,y2);
  LibGdsMesh(x1,y1,x2,y2);
  LibPutDisp();
}

/******************************************************************************
*	[Title]			MsgPlayer
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      Check end game
******************************************************************************/
void MsgPlayer(int pl,int msg)
{ int l;
  int x1,y1;
  int x2,y2;
  TCHSTS tsts;
  	
  l=LibGetProStrSize(IB_PFONT1,GetMsg(msg))-5; /* Message size to shape length */  	
  switch (pl) {
  	case 0: 
      x1=50;
      y1=75;
      break;
  	case 1:
      x1=111-l;
      y1=50;
      break;
  	case 2:
      x1=50;
      y1=15;
     break;
  	case 3:
      x1=13;
      y1=50;
     break;
    } 

  x2=x1+30;
  y2=y1-12;
  /* Face area to restore */  
  LibOpenWindow(x1,y1,40,40);                  
  /* Name area to restore */
  LibOpenWindow(x1-12,y1+40,64,12);
  LibPutFarData(x1-12,y1+40,135);  /* name area */
  /* Shape area to restore */
  LibOpenWindow(x2,y2,14+l,12);                
  
/* Print Face  */  	    
  LibPutGraph(x1,y1,Image[Player[pl].pict]);               
/* Print Shape */  
  LibPutGraph(x2,y2,ShapeL);                               
  LibLine(x2+10,y2,l,1,1);
  LibLine(x2+10,y2+10,l,1,1);
  LibPutGraph(x2+10+l,y2,ShapeR);
/* Print Text Message */       
  LibPutProStr (IB_PFONT1,x2+5,y2+2,GetMsg(msg),l+6);      
/* print player name  */  
  l=LibGetProStrSize(IB_CG57FONT,Player[pl].name);
  LibPutProStr(IB_CG57FONT,x1+20-l/2,y1+43,Player[pl].name,l);  
  LibPutDisp();	
  
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  LibTchWait(&tsts);  /* Wait for a key */
  LibTchRelease();
  LibTchInit();

  LibCloseWindow();
  LibCloseWindow();  
  LibCloseWindow();  
  LibPutDisp();	
}