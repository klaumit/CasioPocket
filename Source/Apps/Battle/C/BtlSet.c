/******************************************************************************
*   Program Title	Battle Ship
*   Module			Board settings
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2000 
*	Rev     		1.3
*	MMSoft 2000 - Freeware
*	Notes:	
*		There are two areas: "board" and "table".
*		At begining the ships are in the table.
*		The player must "cut" the ship form the table and paste it to board
*		To correct a ship the player can select from the board and paste it
*		to other position or return it to the table.
*		The trash button clears the board
*		The Autofill button prepares a random board
******************************************************************************/
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_libc.h>
#include <battle.h>

/* Buttons for Ok, Rotate & Auto_Fill */
/* Button codes */
#define OBJ_ROTATE_BTN 0xA010
#define OBJ_FILL_BTN   0xA011
#define OBJ_OK_BTN     0xA012
#define OBJ_CLR_BTN    0xA013

/* Button touch areas */
static TCHTBL far TchOption[]={	
/* Rotate button */  
  129,75,149,95,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_ROTATE_BTN,
  0x0000,
/* Autofill */  
  110,3,130,23,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_FILL_BTN,
  0x0000,  
/* Ok Button */	
  135,3,155,23,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_OK_BTN,
  0x0000,  
/* Trash Button */	
  85,3,105,23,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_CLR_BTN,
  0x0000,  
/* End */  
  TCH_END
  };  

/* Button graphics (icons) */
byte far GrpRotate[]={  /* ROTATE Icon button */
 GSIZE(20, 20),
 0x00, 0x00, 0x0F, 0x00, 0x00, 0x0F, 0x3F, 0x55, 0x6F,
 0x21, 0x00, 0x2F, 0x21, 0x04, 0x0F, 0x21, 0x04, 0x0F,
 0x21, 0x00, 0x2F, 0x3F, 0x55, 0x6F, 0x21, 0x00, 0x0F,
 0x21, 0x01, 0x0F, 0x21, 0x03, 0x8F, 0x21, 0x01, 0x0F,
 0x3F, 0x01, 0x0F, 0x21, 0x02, 0x0F, 0x21, 0x24, 0x0F,
 0x21, 0x78, 0x0F, 0x21, 0x20, 0x0F, 0x3F, 0x00, 0x0F,
 0x00, 0x00, 0x0F,0x00, 0x00, 0x0F
}; 

byte far GrpFill[]={  /* FILL Icon button */
 GSIZE(20, 20),
 0xFF, 0xFF, 0xEF, 0x80, 0x00, 0x3F, 0x81, 0xF8, 0x3F,
 0x82, 0x3C, 0x3F, 0x86, 0x1E, 0x3F, 0x87, 0x9E, 0x3F,
 0x87, 0x9E, 0x3F, 0x83, 0x3C, 0x3F, 0x80, 0x38, 0x3F,
 0x80, 0x70, 0x3F, 0x80, 0x60, 0x3F, 0x80, 0x60, 0x3F,
 0x80, 0x00, 0x3F, 0x80, 0x60, 0x3F, 0x80, 0xF0, 0x3F,
 0x80, 0xF0, 0x3F, 0x80, 0x60, 0x3F, 0x80, 0x00, 0x3F,
 0xFF, 0xFF, 0xFF,0x7F, 0xFF, 0xFF
}; 

byte far GrpOk[]={  /* Ok Icon button */
 GSIZE(20, 20),
 0xFF, 0xFF, 0xEF, 0x80, 0x00, 0x3F, 0x80, 0x00, 0x3F,
 0x80, 0x00, 0x3F, 0x83, 0x80, 0x3F, 0xF4, 0x7F, 0x3F,
 0xD9, 0x40, 0xBF, 0xD1, 0x7F, 0xBF, 0xD1, 0x4F, 0x3F,
 0xD0, 0xF8, 0x3F, 0xD1, 0x10, 0x3F, 0xD0, 0xF0, 0x3F,
 0xD9, 0x20, 0x3F, 0xFF, 0xE0, 0x3F, 0xF7, 0xC0, 0x3F,
 0x80, 0x00, 0x3F, 0x80, 0x00, 0x3F, 0x80, 0x00, 0x3F,
 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF
}; 
byte far GrpTrash[]={  /* Trash Icon button */
 GSIZE(20, 20),
 0xFF, 0xFF, 0xEF, 0x83, 0xF8, 0x3F, 0x8C, 0x06, 0x3F,
 0x98, 0xE3, 0x3F, 0xB1, 0xF1, 0xBF, 0xB8, 0xA3, 0xBF,
 0xBC, 0x07, 0xBF, 0x9F, 0xFF, 0x3F, 0x97, 0xFD, 0x3F,
 0x98, 0x03, 0x3F, 0x8E, 0xDA, 0x3F, 0x8C, 0x92, 0x3F,
 0x8C, 0x92, 0x3F, 0x8D, 0xB6, 0x3F, 0x89, 0x26, 0x3F,
 0x85, 0x24, 0x3F, 0x85, 0x24, 0x3F, 0x83, 0xF8, 0x3F,
 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF
}; 

/* Board & ship areas */
static TCHTBL far TchSetting[]={
/* Board */	
  10,30,81,111,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN,
  0x0000,  	
/* Ship #1 */
  25,118,34,154,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+1,
  0x0000,
/* Ship #2 */
  39,118,48,145,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+2,
  0x0000,
/* Ship #3 */
  53,118,62,145,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+3,
  0x0000,
/* Ship #4 */
  67,118,76,136,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+4,
  0x0000,
/* Ship #5 */
  81,118,90,136,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+5,
  0x0000,
/* Ship #6 */
  95,118,104,127,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+6,
  0x0000,
/* Ship #7 */
  109,118,118,127,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+7,
  0x0000,
/* Ship #8 */
  123,118,132,127,
  ACT_BREAK_IN,
  OBJ_BD_HUMAN+8,
  0x0000,
/* End */  
  TCH_END
  };  

/* Icons */
static T_ICON far RotateBtn={&TchOption[0],GrpRotate,NULL,0x00};
static T_ICON far FillBtn={&TchOption[1],GrpFill,NULL,0x00};
static T_ICON far OkBtn={&TchOption[2],GrpOk,NULL,0x00};
static T_ICON far ClrBtn={&TchOption[3],GrpTrash,NULL,0x00};

/* Funcions */
static void SettingInit(void);
static void DrawTableShips(void);
static void CutFromTable(int);
static void PasteToTable(void);
static void CutFromBoard(int,int);
static void PasteToBoard(int,int);

/* Clipboard BattleShip for Cut & Paste operations between Table & Board */
typedef struct {
  bool data;  /* Has the clipboard data?              */
  int x;      /* X origin for clipboard display       */
  int y;      /* Y origin for clipboard display       */
  int ship;   /* Ship Id in the clipboard             */
} T_CLIP;  

/* Global variable to Cut & Paste ships */
T_CLIP Clip;


/******************************************************************************
*	[Title]			InitPlayerBoard
*	[Arguments]		
*	[ReturnVAL]	
*   [Description] 	
*		Main loop to preset player ships
******************************************************************************/
void InitPlayerBoard()
{ byte pl;
  int row,col;
  TCHSTS tsts;
  int nship;
  bool set_end;

  pl=PL_HUMAN; /* All player references are for PL_HUMAN */
  SettingInit();
  
  /* Main loop selection */
  ClearBoard(pl);
  LibPutGraph(0,0,Logo);  
  while (TRUE) {
    DrawTableShips();
    ShowBoardAll(pl,TRUE);
    LibTchWait(&tsts);
    switch (tsts.obj) {
      case OBJ_HIC_ESC:     /* ESC Key */
        LibJumpMenu();
      case OBJ_OK_BTN:      /* Ok Button */
        if (LibIconClick(&OkBtn,&tsts)==TRUE){
          set_end=TRUE;
          for (nship=0;nship<N_SHIPS;nship++)
            if (ShipInfo[nship].stat!=ST_BOARD)
              set_end=FALSE;
          if (set_end==TRUE) {
            Board[PL_HUMAN].shots=0;
            Board[PL_HUMAN].hits=0;       
            return;          
            }  /* end if */
          else 
            LibPutMsgDlg (GetMsg(19));
        }  /* end if */
        break;
        
      case OBJ_CLR_BTN:  /* Clear Button */
        if (LibIconClick(&ClrBtn,&tsts)==TRUE){
      /* Initialize & draw ship data */      
          for (nship=0;nship<N_SHIPS;nship++)	{
            ShipInfo[nship].stat=ST_TABLE;   /* None Ship put         */       
            ShipInfo[nship].direc=SD_VER;    /* Default Vertical ship */
            }  /* end for */
          Clip.data=FALSE;
          ClearBoard(pl);
        }  /* end if */
        break;

      case OBJ_ROTATE_BTN:  /* Rotate Button */
        if (LibIconClick(&RotateBtn,&tsts)==TRUE && Clip.data==TRUE)
          if (ShipInfo[Clip.ship].direc==SD_VER)
            ShipInfo[Clip.ship].direc=SD_HOR;
          else  
            ShipInfo[Clip.ship].direc=SD_VER;            
        break;
        
      case OBJ_FILL_BTN:  /* Auto fill Button */
      if (LibIconClick(&FillBtn,&tsts)==TRUE){
      	for (nship=0;nship<N_SHIPS;nship++)  /* Clear the ships on table */
      	  ShipInfo[nship].stat=ST_BOARD;     
       	Clip.data=FALSE;                     /* Clear Clipboard data */ 
      	RandomBoard(pl);
      	}  /* end if */
        break;
        
      case OBJ_BD_HUMAN:    /* Board area */        
        if (tsts.act==ACT_BREAK_IN) {
          GetCell(tsts,pl,&col,&row);
          /* Check for a selected ship */
          if (Board[pl].cell[col][row]==ID_NONE)  /* Paste in a empty cell */
            PasteToBoard(col,row); /* Paste data from Clipboard */
          else /* Copy from the board */
            CutFromBoard(col,row); 
          }  /* end if */  
        break;
        
       default:
         if (tsts.obj>OBJ_BD_HUMAN && tsts.obj<=OBJ_BD_HUMAN+N_SHIPS)
            CutFromTable(tsts.obj-OBJ_BD_HUMAN-1); /* Copy touched ship */
         break;     
       } /* end switch */  
    } /* end while */ 
}  /* end PresetShips */
 


/******************************************************************************
*	[Title]			SettingInit
*	[Arguments]		
*	[ReturnVAL]	
*   [Description] 	
*		Draw preset screen and prepare active areas
******************************************************************************/
static void SettingInit()
{ byte pl;
  int nship;
  
   pl=PL_HUMAN; /* All next player references are for PL_HUMAN */
/* Initialize coordinates origin for each board */   
   Board[pl].x=10;               /* Start coordinate X for HUMAN player */
   Board[pl].y=30;               /* Start coordinate Y for HUMAN player */

/* Title */
   LibPutFarData(25,5,83);   /* Border */
   LibClrDisp(); /* Clear screen */
   LibPutProStr(IB_PFONT2,Board[pl].x+10,Board[pl].y-17,GetMsg(1),80);
   
/* Initialize coordinates origin for clipboard */   
   Clip.x=110;   /* corner reference for left side  */
   Clip.y=50;    /* corner reference for upper side */

/* Draw player board */      
   DrawBoard(Board[pl].x,Board[pl].y);
    
/* Draw Clipboard area */
   LibPutProStr(IB_CG57FONT,105,42,GetMsg(9),100);
   LibBox(Clip.x,Clip.y,41,47,1);  	/* Border */
   LibLine(112,97,39,1,2);    	/* Horizontal border */
   LibLine(151,52,1,47,2);		/* Vertical border */

/* Draw table area */
   LibBox(20,108,120,49,1);  	/* Table Border */
   LibLine(22,157,118,1,2);    	/* Horizontal border */
   LibLine(140,110,1,49,2);		/* Vertical border */
   for (nship=0;nship<N_SHIPS;nship++) {
     /* Border area for ship */
     LibPut35Font(nship+'1',28+14*nship,111);  /* Ship id's */
     LibBox(25+14*nship,118,CELL_SIZE+1,ShipInfo[nship].size*CELL_SIZE+1,1);  /* Border ship */
     }
   
/* Initialize ship data */      
   for (nship=0;nship<N_SHIPS;nship++)	{
     ShipInfo[nship].stat=ST_TABLE;   /* None Ship put         */       
     ShipInfo[nship].direc=SD_VER;    /* Default Vertical ship */
    }	    
  
/* Active screen areas */
   LibTchStackClr();             /* Clear all touch areas  */
   LibTchStackPush(NULL);        /* Empty stack terminator */
   LibTchStackPush(TchHardIcon); /* Register hard icons    */
   LibTchStackPush(TchSetting);  /* Register Casio's board */
   LibTchStackPush(TchOption);   /* Register Buttons       */
   
/* Option Buttons */   
   
   LibIconPrint(&RotateBtn);
   LibIconPrint(&FillBtn);
   LibIconPrint(&ClrBtn);      
   LibIconPrint(&OkBtn);   
   LibPutDisp();   
}   


/******************************************************************************
*	[Title]			CutFromTable
*	[Arguments]		int nship: ship number to cut
*	[ReturnVAL]	
*   [Description] 	
*		Select a ship from table and copy it to ClipBoard 
*		Only select a ship if previously was in the table
******************************************************************************/


static void CutFromTable(int nship)
{ int i;
 
  /* Check if ship was in the table */
  if (ShipInfo[nship].stat==ST_BOARD)
    return;

/* If ship was previos selected, returns it to table */
  if (ShipInfo[nship].stat==ST_SELECT) {
    ShipInfo[nship].stat=ST_TABLE;
    Clip.data=FALSE;
    return;
    }
      
  /* Deselect all selected ships */    
  for (i=0;i<N_SHIPS;i++) 
    if (ShipInfo[i].stat==ST_SELECT)
      ShipInfo[i].stat=ST_TABLE;
      
  /* Selects the touched ship only */    
  ShipInfo[nship].stat=ST_SELECT;
      
  /* Copy ship to ClipBoard */
  Clip.ship=nship;      /* Copy the ship number to clipboard */
  Clip.data=TRUE;       /* Clipboard with data               */
} /* end CutFromTable */


/******************************************************************************
*	[Title]			PasteToTable
*	[Arguments]		
*	[ReturnVAL]	
*   [Description] 	
*		Returns the clipbard ship to table and clears the ClipBoard 
*		Only works if the Clipboard have data
******************************************************************************/
static void PasteToTable()
{ int nship;
 
  if (Clip.data==FALSE)  /* No data to paste */
   return;
   
  nship=Clip.ship; 
  ShipInfo[nship].stat=ST_TABLE;
  Clip.data=FALSE;
} /* end PasteToTable */


/******************************************************************************
*	[Title]			CutFromBoard
*	[Arguments]		int col: column board selected 
*					int row: row board selected
*	[ReturnVAL]	
*   [Description] 	
*		Select a ship from Board and copy it to ClipBoard 
*		Only works if the Clipboard is empty
******************************************************************************/
static void CutFromBoard(int col,int row)
{ int nship;
  int i;
  
  if (Clip.data==TRUE) {
    LibPutMsgDlg (GetMsg(22));  
    return;
    }
 
  nship=GetShipId(Board[PL_HUMAN].cell[col][row]);
  /* Is there a ship in cell? */
  if (nship==-1)  /* Ship not found in board.*/ 
    return;  
    
  /* Points to first cell of ship and clears it*/  
  if (ShipInfo[nship].direc==SD_VER) {    /* Vertical ship? */
    while (Board[PL_HUMAN].cell[col][row]!=ID_NONE)  /* Searchs for first cell */
      row--;
    row++;  
    for (i=0;i<ShipInfo[nship].size;i++)  /* Clears the vertical ship in the board */
      Board[PL_HUMAN].cell[col][row+i]=ID_NONE;
    }  
  else {                                   /* Horizontal ship? */
    while (Board[PL_HUMAN].cell[col][row]!=ID_NONE) /* Searchs for first cell */
      col--;
    col++;      
    for (i=0;i<ShipInfo[nship].size;i++)  /* Clears the horizontal ship in the board */
      Board[PL_HUMAN].cell[col+i][row]=ID_NONE;
    }
     
  /* Deselect all selected ships */    
  for (i=0;i<N_SHIPS;i++) 
    if (ShipInfo[i].stat==ST_SELECT)
      ShipInfo[i].stat=ST_TABLE;
      
  /* Cut the ship from table */    
  ShipInfo[nship].stat=ST_SELECT;  
  
  /* Copy ship to ClipBoard */
  Clip.ship=nship;                  /* Copy the shipId to clipboard */
  Clip.data=TRUE;                   /* Clipboard with data */
} /* end CutFromBoard */


/******************************************************************************
*	[Title]			PasteToBoard
*	[Arguments]		int col: column board selected 
*					int row: row board selected
*	[ReturnVAL]	
*   [Description] 	
*		Paste a ship to Board
*		Only works if the Clipboard have data
******************************************************************************/
static void PasteToBoard(int col,int row)
{ int nship;
  int xp,yp;
  int i;
  int newrow,newcol;
  bool fit;  /* Ship fits in empty space     */
  bool touch; /* Ship is touching other ships */
  
  /* Has the clipboard Data? */  
  if (Clip.data==FALSE) {
     LibPutMsgDlg (GetMsg(18));  
     return;
     }

  nship=Clip.ship;  /* Gets ship number */
  /* Fits the ship in the board? */
  fit=TRUE;
  touch=FALSE;
  newrow=row;
  newcol=col;
  /* Cell's ship are empty? */
  for (i=0;i<ShipInfo[nship].size;i++){
    if (ShipInfo[nship].direc==SD_VER) 
      newrow=row+i;  /* Vertical ship */  	
    else
      newcol=col+i;  /* Horizontal ship */  	
    if (newcol>7||newrow>7||Board[PL_HUMAN].cell[newcol][newrow]!=ID_NONE)
        fit=FALSE;
    else    
    /* Cell's ship are touching other ships? */
      for (xp=-1;xp<2;xp++)
        for (yp=-1;yp<2;yp++)
          if (newcol+xp>=0 && newcol+xp<=7 && newrow+yp>=0 && newrow+yp<=7 && Board[PL_HUMAN].cell[newcol+xp][newrow+yp]!=ID_NONE)
            touch=TRUE;
    }  /* end for */  

    if (fit==FALSE)
      LibPutMsgDlg(GetMsg(20));

    if (touch==TRUE)
      LibPutMsgDlg(GetMsg(21));
     
    if (fit==TRUE && touch==FALSE) {
      for (i=0;i<ShipInfo[nship].size;i++){
       if (ShipInfo[nship].direc==SD_VER)   /* Vertical ship */  	
        newrow=row+i;
       else                                 /* Horizontal ship */
        newcol=col+i;
       Board[PL_HUMAN].cell[newcol][newrow]=nship+ID_SHIP;  /* Symbol for ship */
       } /* end for */
      ShipInfo[nship].stat=ST_BOARD;
      Clip.data=FALSE;
      }
}  


/******************************************************************************
*	[Title]			PasteToBoard
*	[Arguments]		int col: column board selected 
*					int row: row board selected
*	[ReturnVAL]	
*   [Description] 	
*		Draw all ships in the table and clipboard contents
******************************************************************************/
static void DrawTableShips()
{ int nship;
  int xp,yp;
  byte symbol;
  byte pl;
  int i;
  
   pl=PL_HUMAN; /* All player references are for PL_HUMAN */
  /* Draw ships to select in the table */
   for (nship=0;nship<N_SHIPS;nship++) {
     xp=25+nship*(5+CELL_SIZE);   /* corner reference is the same as Touch area */
     yp=118;
     /* Draw cell's ship */
     switch (ShipInfo[nship].stat) {
       case ST_TABLE:   /* Ship on table yet */  
         symbol=SY_SHIP;       /* Ship symbol */
         break;
       case ST_SELECT:  /* Ship selected but not put */
         symbol=SY_SUNK; /* Sunk symbol */
         break;     
       default:         /* Ship on Board: remove from table */  
         symbol=SY_NONE;
         break;
       } /* end switch */
     for(i=0;i<ShipInfo[nship].size;i++)
       LibPutGraph(xp+1,yp+1+i*CELL_SIZE,SymbolData[symbol]);
     }  /* end for */
      
   /* Draws the Clipbard state */   
   xp=Clip.x+1;   /* corner reference from right side of display*/
   yp=Clip.y+1;
   LibClrBox(xp,yp,3+4*CELL_SIZE,9+CELL_SIZE); /* Clear clipboard area */
   LibClrBox(xp,yp,3+CELL_SIZE,9+4*CELL_SIZE); /* Clear clipboard area */
      
   if (Clip.data==TRUE) { /* Are data in the clipboard? */           
     LibPut35Font(Clip.ship+'1',xp+2*CELL_SIZE,yp+1);  /* Ship number */
     xp=xp+1;
     yp=yp+7;
     if (ShipInfo[Clip.ship].direc==SD_VER) { /* Is a vertical ship? */
        LibGdsBox(xp,yp,xp+CELL_SIZE,yp+ShipInfo[Clip.ship].size*CELL_SIZE);
        for (i=0;i<ShipInfo[Clip.ship].size;i++)  /* Writes the ship in clipboard area */
          LibPutGraph(xp+1,yp+1+i*CELL_SIZE,SymbolData[SY_SHIP]);
        }
       else { /* Horizontal ship */
        LibGdsBox(xp,yp,xp+ShipInfo[Clip.ship].size*CELL_SIZE,yp+CELL_SIZE);
        for (i=0;i<ShipInfo[Clip.ship].size;i++)  /* Writes the ship in clipboard area */
          LibPutGraph(xp+1+i*CELL_SIZE,yp+1,SymbolData[SY_SHIP]);
        }        
      }  
  LibPutDisp(); 
}


/******************************************************************************
*	[Title]			RandomBoard
*	[Arguments]		byte pl: player
*	[ReturnVAL]	
*   [Description] 	
*		Generates a random board with ship rules
*		To create a ship must be necessary 3 conditions:
*		  - The ship start cell must be an empty cell
*		  - Get a random direction for extend the ship and check if fits
*		  - Fill with ID_FAIL's around the new ship to reserve space
******************************************************************************/
void RandomBoard(byte pl)
{ int row,col;
  int direc;         /* Ship direction   0:vertical 1: horizontal */
  int nship;
  int ship_size;
  bool valid;
  int i;

  LibWinIcnMsg(ICON_COFFEE,381,0x00);  /* Please wait */
  LibPutDisp(); 
  ClearBoard(pl);
  
  for (nship=0;nship<N_SHIPS;nship++) {
    /* Ship size */	
    ship_size=ShipInfo[nship].size;
   /* Gets a random position */
    row=rand()%8;
    col=rand()%8;
    direc=rand()%2;
     
    valid=FALSE;
  /* Look for a valid cell */
    while (valid==FALSE) {
      
  /*1st condition: empty cell */
  /* Search the first empty cell starting at (row,col) */
       while (Board[pl].cell[col][row]!=ID_NONE)
         if (++col>7) {
           col=0;
           if (++row>7)
      	      row=0;
           }
       valid=TRUE;      
       
  /*2nd condition: Have enought space?  */        
       if (direc==SD_VER) {  /*  vertical ship?   */
           if (1+row-ship_size>=0) { /* Have enought space? */
             for (i=1;i<ship_size && valid==TRUE;i++)
               if (Board[pl].cell[col][row-i]!=ID_NONE)
                 valid=FALSE; 
             }    
           else
             valid=FALSE;     
           }                   
       else { /* horizontal ship  */
           if (1+col-ship_size>=0) { /* Have enought space? */
             for (i=1;i<ship_size && valid==TRUE;i++)
               if (Board[pl].cell[col-i][row]!=ID_NONE)
                 valid=FALSE;
             }    
           else
             valid=FALSE;                    
         }  
               
       if (valid==TRUE)  { /* The ship fits=> Paste ship in board */
         for (i=0;i<ship_size;i++)
           if (direc==SD_VER) /*  vertical ship */
               Board[pl].cell[col][row-i]=nship+ID_SHIP;
           else          /* horizontal ship */
               Board[pl].cell[col-i][row]=nship+ID_SHIP;
         FillAround(pl,nship+ID_SHIP);      
         ShipInfo[nship].direc=direc;
         }
       else
      /* Search the next empty cell starting at (row,col) */
         if (++col>7) {
           col=0;
           if (++row>7)
             row=0;
           }   
       }   /* end while (valid==FALSE) */       
    }   /* end for */
    
  /* Remove all "FAILS" from cells and rewrites ID_SUNK with ID_SHIP*/
  for (row=0;row<8;row++)
    for (col=0;col<8;col++)
       if (Board[pl].cell[col][row]==ID_FAIL)
          Board[pl].cell[col][row]=ID_NONE;
  /* Init counters */        
  Board[pl].shots=0;
  Board[pl].hits=0;
  LibCloseWindow(); 
}   /* end function */
