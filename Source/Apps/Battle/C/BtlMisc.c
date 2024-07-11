/******************************************************************************
*   Program Title	Battle Ship
*   Module			Miscellaneous functions
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


/******************************************************************************
*	[Title]			InitPlayerBoard
*	[Arguments]		TCHSTS tsts: touched position
*					byte pl: player board for references
*					int *col: column board returned
*					int *row: row board returned
*	[ReturnVAL]	
*   [Description] 	
*		Returns the touched cell X & Y position in CASIO's area
******************************************************************************/
void GetCell(TCHSTS tsts,byte pl,int *col,int *row)
{ *row=(tsts.y-Board[pl].y-1)/CELL_SIZE;
  *col=(tsts.x-Board[pl].x-1)/CELL_SIZE; 
}  /* end GetCell */


/******************************************************************************
*	[Title]			TouchCell
*	[Arguments]		byte pl: player board for references
*					int col: column board 
*					int row: row board 
*	[ReturnVAL]	
*   [Description] 	
*		Simulates a press & release operation in the pointed cell
******************************************************************************/
void TouchCell (byte pl,int col,int row)
{ int xp;
  int yp;
  int i;
  
   xp=Board[pl].x+col*CELL_SIZE+1;
   yp=Board[pl].y+row*CELL_SIZE+1;
   for (i=0;i<4;i++){
     LibReverse(xp,yp,xp+CELL_SIZE,yp+CELL_SIZE);
     LibWait (IB_500MWAIT);
     LibPutDispBox(xp,yp,CELL_SIZE,CELL_SIZE);
     }  
}   


/******************************************************************************
*	[Title]			CheckShot
*	[Arguments]		int pl: player board for references
*					int col: column board 
*					int row: row board 
*	[ReturnVAL]	    byte: shot result:
*					 SH_FAIL: for a failed shot (water)
*					 SH_HIT: For a hit (touched)					 	
*					 SH_SUNK: The ship has sunked
*					 SH_NULL: for an invalid shot
*   [Description] 	
*		Returns shot's result for a player and show the info messages
*
******************************************************************************/
byte CheckShot(byte pl,int col,int row)
{ int shotinfo;
  byte other;  /* Player Opponent */
  int xp,yp;
  int nship;
  int cell;
  int nhits;
  
  other=(pl==PL_CASIO?PL_HUMAN:PL_CASIO);  /* Select opponent for check shot in his board */
  TouchCell(other,col,row); /* Press and release effect */
  
  cell=Board[other].cell[col][row];
/* Check if was a valid shot => not discovered cell */ 
  if (cell>=ID_FAIL)
    shotinfo=SH_NULL;  /* Invalid shot */
  else { 
/* Checks a valid shot */     
	Board[pl].shots++;        
	if (cell!=ID_NONE){  	/* Hit?		      */
      Board[pl].hits++;  	/* Hits counter   */
      nship=GetShipId(cell);/* What ship was? */
      Board[other].cell[col][row]=nship+ID_HIT;  /* rewrites old ID with ID_HIT */
    
/* Checks SUNK Ship (all ships with same ID) */
      nhits=0;
      for (xp=0;xp<8;xp++)  /* Count "hitted" cells  with same ID */
       for (yp=0;yp<8;yp++)
         if (Board[other].cell[xp][yp]==nship+ID_HIT)
           nhits++;
      if (nhits==ShipInfo[nship].size) { 
        shotinfo=SH_SUNK;         /* SUNK ship=> rewrites old ID with ID_SUNK  */
        for (xp=0;xp<8;xp++)  
          for (yp=0;yp<8;yp++)
           if (Board[other].cell[xp][yp]==nship+ID_HIT)
             Board[other].cell[xp][yp]=nship+ID_SUNK;
        }
      else
        shotinfo=SH_HIT;  /* Not sunk yet */
      }  /* end if */
    else {
      Board[other].cell[col][row]=ID_FAIL;      /* Fail */
      shotinfo=SH_FAIL;
      }  /* end else */ 
  } /* end if */

/* Select next player with shotinfo value */
  switch (shotinfo) { /* Shot result for player */
    case SH_FAIL:  /* Failed */
      /* Selects the other player */
      break;
    case SH_HIT:  /* Hit    */
      other=pl;   /* Player doesn't change. Repeat shot */
      break;
    case SH_SUNK:  /* Sunk    */
      /* Player doesn't change. Repeat shot */
      if (pl==PL_CASIO) { /* fills non discovered cells around sunk ship */
        FillAround(other,Board[other].cell[col][row]);
        }
      other=pl;    
      break;
    default:  /* Invalid */    
      /* Player doesn't change. Repeat shot */
      other=pl;    
    break;
  }  /* end switch */ 
  if (pl==PL_HUMAN)
    ShowBoardAll(PL_CASIO,FALSE);  /* Show casio's board */  
  else  
    ShowBoardAll(PL_HUMAN,TRUE);   /* Show player's board */  
  MsgScore(pl);                   /* Show score */  
  MsgShotInfo(shotinfo,col,row);   /* Shot info message */        
  MsgPlayerTurn(other);	
  return(other);
}          


/******************************************************************************
*	[Title]			ComputerShot
*	[Arguments]		int *col: column shot returned
*					int *row: row shot returned
*	[ReturnVAL]	    
*   [Description] 	
*		The soul of game (I think)
*		The computer calculates a (valid) shot based in the previous status
*		and chance
*		Method to calculate cell:
*		1.- Check for a previous hitted ship (ID=HIT)
*		2.- If not exist a hitted ship, try a random but not discovered cell.
*       	If exist, checks adjacent cell to stablish direction ship (vert. or horiz.)
*		3.- If the direction was stablished, continues the same direction.
*			If was not stablished, tries a random and possible direction,
*		 	checking limits and not discovered adjacent cells.
******************************************************************************/
void ComputerShot(int *col,int *row)
{ int xp,yp;
  int newxp, newyp;
  int direc;  /* ship direction=>  0:none 1:vertical 2:horizontal    */
  byte ship_id;
  bool found;
  byte pl;  /* player Id*/
  int i;
 
  
  pl=PL_HUMAN;
  /* 1st step */
  /* Looks for the first "hitted" but not sunk ship, if exists */  
  /* exits with found=TRUE and first hitted ship in (xp,yp)    */
  /*    or with found=FALSE                                    */
   xp=0;
   yp=0;
   found=FALSE;
   while (yp<8 && found==FALSE) {
      ship_id=Board[pl].cell[xp][yp];
      if (ship_id>=ID_HIT && ship_id<ID_SUNK)  /* Hit, but not sunk? */
        found=TRUE;
      else
        if (++xp>7) {
          xp=0;   /* returns to first column */
          yp++;   /* advances to next row    */
          }	
       }  /* end while */
        
   /* 2nd step */    
    if (found==FALSE){  /* A hitted ship was not found */
      i=0;
      xp=rand()%8;      
      yp=rand()%8;         
    /* Try an aleatory and not discovered cell (max 10 times) */
      while (Board[pl].cell[xp][yp]>=ID_FAIL && i<10) { 
        xp=rand()%8;      
        yp=rand()%8;         
        i++;
        }  
  /* Search the first not discovered cell starting at (xp,yp) */
      while (Board[pl].cell[xp][yp]>=ID_FAIL)
        if (++xp>7) {
          xp=0;
          if (++yp>7)
            yp=0;
          }  /* end if */
      *row=yp;
      *col=xp;
      return; /* ends the function */
     }  /* end if */  

    /* Continues if "found" was TRUE */	
    /* Found some "hitted" ship. Now must determine ship direction */
    /* Checking adjacent cells (vertical & horizontal)             */
    /* with the same ship_id                                       */ 
    if (Board[pl].cell[xp][yp+1]==ship_id) {     /* Is a vertical ship?     */
      newxp=xp;         /* stores the first cell to check the bottom of ship */
      newyp=yp+1;
      while (newyp<8 && Board[pl].cell[newxp][newyp]==ship_id)
        newyp++;       /* searchs the end of ship */
      if (newyp<8 && Board[pl].cell[newxp][newyp]<ID_FAIL) { /* Continue down? */
        *row=newyp;  /* returns the last row not discovered */
        *col=newxp;  /* and the same column                 */
        return;
        } /* end if */
      else { /* Can't continue down. Try upside  */
        *row=yp-1;   /* returns the previous row */
        *col=newxp;  /* and the same column      */
        return;
        } /* end else */
      } /* end if is a vertical ship */
      
      
    if (Board[pl].cell[xp+1][yp]==ship_id) { /* Is an horizontal ship?  */
      newxp=xp+1;         /* stores the first cell to check the right of ship  */
      newyp=yp;
      while (newxp<8 && Board[pl].cell[newxp][newyp]==ship_id)
        newxp++;       /* searchs the end of ship */
      if (newxp<8 && Board[pl].cell[newxp][newyp]<ID_FAIL) { /* Continue right? */
        *col=newxp;   /* returns the last column not discovered */
        *row=newyp;   /* and the same row                       */
        return;
        } /* end if */
      else {  /* Can't continue right. Try leftside */
        *col=xp-1;   /* returns the previous column */
        *row=newyp;  /* and the same row            */
        return;
        } /* end else */
      } /* end if is a horizontal ship */

    /* 3rd step */    
    /* Can't establish ship direction (yet) Try an aleatory dir. */
    while (TRUE) {   /* repeat until the function returns a valid row,column */
      direc=rand()%4+1;  /* first, select a random direction   */
      switch (direc){
        case 1: /* up */
          if (yp>0 && Board[pl].cell[xp][yp-1]<ID_FAIL) {
            *row=yp-1;
            *col=xp;
            return;
            } /* end if */
          break;   
        case 2: /* down */              
          if (yp<7 && Board[pl].cell[xp][yp+1]<ID_FAIL) {
            *row=yp+1;
            *col=xp;
            return;
            } /* end if */
          break;   
        case 3: /* left */
          if (xp>0 && Board[pl].cell[xp-1][yp]<ID_FAIL) {
            *row=yp;
            *col=xp-1;
            return;
            } /* end if */
          break;   
        default: /* right */
          if (xp<7 && Board[pl].cell[xp+1][yp]<ID_FAIL) {
            *row=yp;
            *col=xp+1;
            return;
            } /* end if */
          break;   
        }  /* end switch */
      } /* end while */  
}  /* end ComputerShot */


/******************************************************************************
*	[Title]			FillAround
*	[Arguments]		byte pl: player
*					int cell_id: cell value (ship number) to fill around it.
*	[ReturnVAL]	    
*   [Description] 	
*		Fills around cells with "cell_id" ships with FAIL cells.
*		This is to prevent shots close to ships and save shots
******************************************************************************/
void FillAround (byte pl,int cell_id)
{ int row,col;
  int x,y;

  /* Look for each cell same as ship_id and fills the ID_NONE with ID_FAIL */
  for (col=0;col<8;col++)
   for (row=0;row<8;row++)  
    if (Board[pl].cell[col][row]==cell_id) /* Is the selected ship? */
    /* Fill the adjacent and not discovered cells with ID_FAIL */
     for (x=-1;x<2;x++)
      for (y=-1;y<2;y++)
       if (col+x>=0 && col+x<8 && row+y>=0 && row+y<8 && Board[pl].cell[col+x][row+y]==ID_NONE)
         Board[pl].cell[col+x][row+y]=ID_FAIL;
}  /* end FillAround */


/******************************************************************************
*	[Title]			CheckEnd
*	[Arguments]		byte pl: player
*	[ReturnVAL]	    TRUE if end of game
*					FALSE if not.
*   [Description] 	
*		Check the end of game for a player => hits=TOTAL_HITS
******************************************************************************/
bool CheckEnd(byte pl)
{ if (Board[pl].hits==TOTAL_HITS)
    return (TRUE);
  else  
    return (FALSE);
}        

/******************************************************************************
*	[Title]			GetShipId
*	[Arguments]		int cell: cell pointed (shot)
*	[ReturnVAL]	    The ship number (ship Id) 
*					-1 if was an empty cell (no ship).
*   [Description] 	
*		Returns the Ship number from a cell.
******************************************************************************/
int GetShipId(int cell)
{ 
  if (cell==ID_NONE || cell==ID_FAIL) /* No ship in cell */
    return(-1);  
  return (cell%10);
} 
