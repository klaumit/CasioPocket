/******************************************************************************
*   Program Title	Battle Ship
*   Module			Board & Display
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2000 
*	Rev     		1.3
*	MMSoft 2000 - Freeware
*	Notes:	
*	
******************************************************************************/
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <battle.h>

static void ShowBoardCell(byte,int,int);


/******************************************************************************
*	[Title]			BattleScreen
*	[Arguments]		
*	[ReturnVAL]	
*   [Description] 	
*		Draw the main screen: Boards, title, scores, etc
******************************************************************************/
void BattleScreen(void)
{ byte pl;
    
  LibClrDisp(); /* Clear screen */
  LibPutGraph(0,0,Logo);
  for (pl=PL_CASIO;pl<=PL_HUMAN;pl++) 
    DrawBoard(Board[pl].x,Board[pl].y);

   /* Title */
  LibPutFarData(25,5,83);   /* Border */
  LibPutProStr(IB_PFONT2,45,7,"BATTLE SHIP",110);

/* Players */   
  LibPutProStr(IB_PFONT2,Board[PL_CASIO].x+10,Board[PL_CASIO].y-17,GetMsg(0),80);
  LibPutProStr(IB_PFONT2,Board[PL_HUMAN].x+10,Board[PL_HUMAN].y-17,GetMsg(1),80);  

/* Show all boards */
  ShowBoardAll(PL_CASIO,FALSE);
  ShowBoardAll(PL_HUMAN,TRUE);
  MsgScore(PL_CASIO);
  MsgScore(PL_HUMAN);  
}


/******************************************************************************
*	[Title]			DrawBoard
*	[Arguments]		int xorig: board coord. x 
*					int yorig: board coord. y	
*	[ReturnVAL]	
*   [Description] 	
*		Draw a board
*  		The Boards have 64 cells of CELL_SIZE x CELL_SIZE pixels each
******************************************************************************/
void DrawBoard(int xorig,int yorig)
{ int i;
 /* Draw board */
 for (i=0;i<9;i++) {
  LibLine(xorig,yorig+CELL_SIZE*i,8*CELL_SIZE,1,1); /* Horizontal Lines */
  LibLine(xorig+CELL_SIZE*i,yorig,1,8*CELL_SIZE,1); /* Vertical Lines   */
  }
  
  LibLine (xorig+8*CELL_SIZE,yorig+2,1,8*CELL_SIZE+1,3);/* Vertical frame */
  LibLine (xorig+2,yorig+8*CELL_SIZE+1,8*CELL_SIZE-1,1,2);  /* Horizontal frame */

  LibPutGraph(xorig+3,yorig-6,SymbolCoor);
  if (xorig>5)
    for (i=0;i<8;i++)  /* row & column references */
      LibPut35Font('1'+i,xorig-5,yorig+CELL_SIZE*i+2);
}


/******************************************************************************
*	[Title]			ClearBoard
*	[Arguments]		byte pl: player
*	[ReturnVAL]	
*   [Description] 	
*		Clear (initializes) the contents of player (pl) board
*  		Fill the array with ID_NONE values
******************************************************************************/
void ClearBoard(byte pl)
{ int row,col;

/* Initialize board cells to NONE */    
  for (col=0;col<8;col++)
    for(row=0;row<8;row++)
      Board[pl].cell[col][row]=ID_NONE;
}   /* End ClearBoard */   


/******************************************************************************
*	[Title]			ShowBoardCell
*	[Arguments]		byte pl: player
*					int row: row board
*					int col: col board
*	[ReturnVAL]	
*   [Description] 	
*		Shows the contents of a board cell with a symbol
* 		Each one can be ID_FAIL => symbol SY_FAIL
*						ID_HIT  => symbol SY_HIT
*						ID_SUNK => symbol SY_SUNK
*						ID_SHIP => symbol SY_SHIP (not discovered yet)
*						ID_NONE => symbol SY_NONE
*
*		Symbols are defined in BtlData
******************************************************************************/
static void ShowBoardCell(byte pl,int col,int row)
{ int xpos,ypos;
  byte cellvalue;  
  byte ship_status;
  int symbol;

 /* Define coordinates start area */
   xpos=Board[pl].x+col*CELL_SIZE+1;
   ypos=Board[pl].y+row*CELL_SIZE+1;
 
   cellvalue=Board[pl].cell[col][row];
   ship_status=cellvalue-cellvalue%10;
   switch (ship_status) {
    case ID_FAIL:
     symbol=SY_FAIL;
     break;
   case ID_HIT:
     symbol=SY_HIT;
     break;     
   case ID_SUNK:
     symbol=SY_SUNK;
     break;
   case ID_SHIP:
     symbol=SY_SHIP;
     break;     
   default:  
     symbol=SY_NONE;
     break;
   }
   LibPutGraph(xpos,ypos,SymbolData[symbol]);
}     
	

/******************************************************************************
*	[Title]			ShowBoardAll
*	[Arguments]		byte pl: player
*					bool option: TRUE to display all ships (reveal ships)
*								 FALSE to display ships but not discovered.
*	[ReturnVAL]	
*   [Description] 	
*		Show a complete player board: CASIO or human PLAYER
******************************************************************************/
void ShowBoardAll(byte pl, bool option)
{ int row;
  int col;

  /* Print cell values */   
  for (col=0;col<8;col++)
    for(row=0;row<8;row++)
      if (Board[pl].cell[col][row]>=ID_FAIL || option==TRUE)
        ShowBoardCell(pl,col,row);      
  LibPutDisp();    
}  /* end ShowBoardAll */


