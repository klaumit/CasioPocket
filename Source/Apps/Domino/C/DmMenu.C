/******************************************************************************
*   Program Title	Domino
*   Module			Menu Options and settings
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


static void PdwnFncEdt(word);
static void PdwnFncSys(word);
static void PdwnFncOpt(word);
static void Scores(void);
static void SetPlayerNames(void);
static void SavePlayerNames(void);
static void SaveGame(void);
static bool GetName(char *,int,int);
static int  GetImage(void);
static bool LoadData(void);
static bool SaveData(void);

#define		OBJ_IC_SET			0xA020
#define		OBJ_IC_CLR			0xA021
#define		OBJ_IC_ESC			0xA022
#define     OBJ_IC_NAME0        0xA025 
#define     OBJ_IC_NAME1        0xA026
#define     OBJ_IC_NAME2        0xA027
#define     OBJ_IC_NAME3        0xA028
#define		OBJ_STRING			0xA029

#define     OBJ_IC_IMAGE0       0xA030
#define     OBJ_IC_IMAGE1       0xA031
#define     OBJ_IC_IMAGE2       0xA032
#define     OBJ_IC_IMAGE3       0xA033
#define     OBJ_IC_IMAGE4       0xA034
#define     OBJ_IC_IMAGE5       0xA035
#define     OBJ_IC_IMAGE6       0xA036
#define     OBJ_IC_IMAGE7       0xA037

#define     NO_IMAGE            -1
TCHTBL far TchHeaderIcon[] =
{ 40, 0, 68, 11,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_SET,
  0x0000,
  69, 0, 96, 11,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_CLR,
  0x0000,
  97, 0, 123, 11,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_ESC,
  0x0000,
  TCH_END
  };

TCHTBL far TchNames[]=
{ 50,62,106,74,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_NAME0,
  0x0000,
  92,42,148,54,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_NAME1,
  0x0000,
  50,20,106,32,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_NAME2,
  0x0000,
  4,42,60,54,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_NAME3,
  0x0000,
  TCH_END
  };


TCHTBL far TchImages[]=
{ 0,79,39,119,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE0,
  0x0000,
  40,79,79,119,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE1,
  0x0000,
  80,79,119,119,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE2,
  0x0000,
  120,79,159,119,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE3,
  0x0000,
  0,120,40,159,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE4,
  0x0000,
  40,120,79,159,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE5,
  0x0000,
  80,120,119,159,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE6,
  0x0000,
  120,120,159,159,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_IMAGE7,
  0x0000,
  TCH_END
  };

/* Define Touch for YES-NO buttons */
TCHTBL far TchYesNo1[]={
/* YES Button */	
  50,140,79,155,
  ACT_ICON,
  OBJ_IC_YES,
  0x0000,	
/* NO Button */  
  90,140,119,155,
  ACT_ICON,
  OBJ_IC_NO,
  0x0000,	
/* End */  
  TCH_END};  

/* Define YES-NO buttons */
static T_ICON far IcnYes={ &TchYesNo1[0],NULL,NULL,0x00};
static T_ICON far IcnNo ={ &TchYesNo1[1],NULL,NULL,0x00};


T_ICON far TiconSet = { &TchHeaderIcon[0], NULL, NULL, 0x00 };
T_ICON far TiconClr = { &TchHeaderIcon[1], NULL, NULL, 0x00 };
T_ICON far TiconEsc = { &TchHeaderIcon[2], NULL, NULL, 0x00 };

/* Structure for info: */
struct {
	bool flsaved;
	bool level;
	bool resume;
	char name[N_PLAYERS][NAME_MAX+1];
	byte pict[N_PLAYERS];
	T_PLAYER Player[N_PLAYERS];
	T_SIDE Board[2];
    } FileInfo;


/*****************************************************************************/
word MPdEdt[] ={
	    14,  /* New game             */
	    477, /* Save                 */
	    PDNTBLEND,};
word MPdSys[] = {
		38,	 /* Set_date/time		 */
		33,	 /* Sound				 */
		39,	 /* Format				 */
		34,	 /* Capacity		 	 */
		35,	 /* Memory_management	 */
 		36,	 /* Language			 */
		37,	 /* Contrast			 */
		40,	 /* Touch_Panel_Alignment*/
/*		41,	 /* Data_communication	 */
		PDNTBLEND,
		};
word	MPdOpt[] = {
	    312, /* Level1               */
	    313, /* Level2               */
	    398, /* Score                */
	    213, /* Name                 */
		PDNTBLEND,};
/*****************************************************************************/

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
void GmPdwnFnc(void)
{  word	sel;
	
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
		newgame=TRUE;
		LibWinIcnMsg(ICON_OK,66,1);
		break;
	case PDWN_EDT_SAVE:		 /* Save game */
		SaveGame();
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
		LibFuncLang();
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
	case PDWN_OPT_LEV1:		/* Level 1 */
	   levelgame=1;
		break;
	case PDWN_OPT_LEV2:		/* Level 2 */
	   levelgame=2;
		break;
	case PDWN_OPT_SCORE:	/* Scores  */
		Scores();
		break;
	case PDWN_OPT_NAME:		/* Names   */
		SetPlayerNames();
		break;
	default:
		break;
	}
}


/******************************************************************************
*	[Title]			Scores
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*      MENU OPTION SCORES
*      Shows the scores
******************************************************************************/
static void Scores ()
{ TCHSTS tsts;
  int pl;
  char texto[30];
	
  LibOpenWindow(16,10,128,62);
  LibPutFarData(16,10,108);       /* rectangle 62 pixels height */
  LibClrBox(18,12,124,58);
  
  LibPutMessage(311+levelgame,22,15,IB_PFONT2); /* Level */
  LibPutMessageRight(398,140,15,IB_CG57FONT);     /* Score */
  for (pl=0;pl<N_PLAYERS;pl++) {
    sprintf(texto,"%10s   %4d",Player[pl].name,Player[pl].score);
    LibPutProStr(IB_CG57FONT,20,26+10*pl,texto,150);
    }
  LibPutDisp();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  LibTchWait(&tsts);  /* Wait for a key */
  LibTchRelease();
  LibCloseWindow();
  LibPutDisp();
}  	      

/******************************************************************************
*	[Title]		LoadGame
*	[Arguments]	None
*	[ReturnVAL]	None
*   [Description] 
*       Restore the saved game if exist.
*       First, checks if exist a file called DOMINO
*       If file exists, load previous names and pictures.
*       Later, check if exists a game data saved (Field flsaved=TRUE)
*       If data exists, reload the game and shows it.
******************************************************************************/
void LoadGame()
{ char PlayerDefault[][NAME_MAX+1]={"PLAYER 1","PLAYER 2","PLAYER 3","PLAYER 4"};  
  TCHSTS tsts;
  int pl;
  int side,d,l;
  bool input_end;

  LibClrDisp();
  LibPutProStr(IB_PFONT3,50,10,"D O M I N O",100);
  LibPutGraph(11,35,Logo);  /* Logo */  
  LibWinIcnMsg(ICON_SYNC,337,0);

  if (LoadData()==FALSE) { /* No files saved */
    for (pl=0;pl<4;pl++) {
      Player[pl].pict=pl;  						 /* Default Image */
      strcpy(Player[pl].name,PlayerDefault[pl]); /* Default Name  */
      }
    LibCloseWindow();
    LibWinIcnMsg(ICON_BADTZ,323,0);
    SavePlayerNames();
    LibCloseWindow();    
    LibWinIcnMsg(ICON_OK,342,1);    
    resume=FALSE;
    newgame=TRUE;
	return;
    }

/* Read Player's names */
   for (pl=0;pl<N_PLAYERS;pl++) {
     strcpy(Player[pl].name,FileInfo.name[pl]);
     Player[pl].pict=FileInfo.pict[pl];
     }
    
  if (FileInfo.flsaved!=0xFF) { /* No games saved previously */
    LibCloseWindow();
    LibWinIcnMsg(ICON_BIKKURI,323,1);
    resume=FALSE;
    newgame=TRUE;
	return;
   }
   
  /* Ask for resume previous game */ 
  LibClrDisp();
  LibPutProStr(IB_PFONT3,50,10,"D O M I N O",100);
  LibPutGraph(11,35,Logo);  /* Logo */
  l=LibGetProStrSize(IB_PFONT1,GetMsg(13));
  LibPutProStr(IB_PFONT1,80-l/2,110,GetMsg(13),l);
  l=LibGetProStrSize(IB_PFONT1,GetMsg(14));
  LibPutProStr(IB_PFONT1,80-l/2,125,GetMsg(14),l);
  LibPutFarData(TchYesNo1[0].x1,TchYesNo1[0].y1,113);
  LibPutFarData(TchYesNo1[1].x1,TchYesNo1[1].y1,113);
  LibPutMessageCenter(43,50,79,143,IB_CG57FONT);  /* Yes */
  LibPutMessageCenter(44,90,119,143,IB_CG57FONT); /* No */
  LibPutDisp();   

  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchYesNo1);
  LibTchInit();
  input_end=FALSE;
  while (!input_end) {
    LibTchWait(&tsts);  /* Wait for a key */
    switch (tsts.obj) {
    	case OBJ_IC_YES:
    	     if (LibIconClick(&IcnYes,&tsts)==TRUE) {
    	       input_end=TRUE;	
      	       }
      	  break;  
    	case OBJ_IC_NO:
    	     if (LibIconClick(&IcnNo,&tsts)==TRUE) {
               resume=FALSE;
               newgame=TRUE;
               return;
      	       }
      	  break;  
     	case OBJ_HIC_ESC:  /* ESC Key */
          LibTchRelease();
          LibTchInit();
          LibJumpMenu();
      	  break;  
      	} 
     } 	  

  memcpy(Player,&FileInfo.Player[0],sizeof(Player));
  memcpy(Board,&FileInfo.Board[0],sizeof(Board));
	
/* Restore game */	
  levelgame=FileInfo.level;
  resume=FileInfo.resume;
  newgame=FALSE;

  if (resume) {
/* Init Board information */ 
    Board[RIGHT].x=86;
    Board[RIGHT].y=63;
    Board[RIGHT].dir=RIGHT; 
    Board[RIGHT].tchX1=Board[RIGHT].x-DOM_HEIGHT;
    Board[RIGHT].tchY1=Board[RIGHT].y-6;
    Board[RIGHT].tchX2=Board[RIGHT].x;
    Board[RIGHT].tchY2=Board[RIGHT].y-6+DOM_WIDTH; 
    Board[RIGHT].end=6;
  
    Board[LEFT].x=Board[RIGHT].x;
    Board[LEFT].y=Board[RIGHT].y+DOM_HEIGHT;
    Board[LEFT].dir=LEFT;
    Board[LEFT].tchX1=Board[RIGHT].tchX1;
    Board[LEFT].tchY1=Board[RIGHT].tchY1;
    Board[LEFT].tchX2=Board[RIGHT].tchX2;
    Board[LEFT].tchY2=Board[RIGHT].tchY2; 
    Board[LEFT].end=6;
  	  	
	LibCloseWindow();	  	
    LibClrDisp();	
    for (side=LEFT;side<=RIGHT;side++) {
      l=Board[side].len;
	  Board[side].end=6;
	  Board[side].len=0;
	  for (d=0;d<l;d++) 
	    PlaceDomino(Board[side].dom[d],side);
   	  for (pl=0;pl<N_PLAYERS;pl++)  /* Draw Player's dominoes */
	    DrawPlayerDomino(pl);  	
	  }  
	}  /* End restore */	  
  LibTchRelease();
  LibTchInit();
  LibWinIcnMsg(ICON_OK,342,1);
  return;
}  


/******************************************************************************
*	[Title]		SaveGame
*	[Arguments]	None
*	[ReturnVAL]	None
*   [Description] 
*      MENU EDIT SAVE
*      Save a game to restore later
*      Sets the flsaved to TRUE.
******************************************************************************/
static void SaveGame()
{ int pl;
	
	LibWinIcnMsg(ICON_COFFEE,381,0);
	for (pl=0;pl<N_PLAYERS;pl++)  /* Player's name */
	  strcpy(FileInfo.name[pl],Player[pl].name);

    FileInfo.flsaved=0xFF;    /* Info data saved */
    FileInfo.level=levelgame;
    FileInfo.resume=resume;
    memcpy(&FileInfo.Player[0],Player,sizeof(Player));
    memcpy(&FileInfo.Board[0],Board,sizeof(Board));
	SaveData();
	LibCloseWindow();
	LibWinIcnMsg(ICON_OK,341,1);
}  


/******************************************************************************
*	[Title]		SavePlayerNames
*	[Arguments]	None
*	[ReturnVAL]	None
*   [Description] 
*      Save the player names to restore later.
*      Load the previous saved data and replaces the NAME fields 
******************************************************************************/
static void SavePlayerNames()
{ int pl;
  bool info;

	info=LoadData();
	if (info) LibWinIcnMsg(ICON_COFFEE,381,0);  /* Data exists */
	for (pl=0;pl<N_PLAYERS;pl++) {
	  strcpy(FileInfo.name[pl],Player[pl].name);
	  FileInfo.pict[pl]=Player[pl].pict;
	  }
	SaveData();
	if (info) {
	 LibCloseWindow();
	 LibWinIcnMsg(ICON_OK,341,1);
	 }
	
}  


/******************************************************************************
*	[Title]		SetPlayerNames
*	[Arguments]	None
*	[ReturnVAL]	None
*   [Description] 
*      Sets the player's names and selects the picture asociated to each one
******************************************************************************/
static void SetPlayerNames()
{ TCHSTS tsts;
  bool input_end;
  bool modify;
  int img,i;
  int xp,yp;
   
  LibOpenWindow(0,0,160,160);

  LibBox(0,15,160,66,2);
  LibReverse(2,17,156,62);
  LibMesh(2,17,156,62); 
  
  LibPutFarData(50,62,135);  /* Player 0 area */
  LibPutProStr(IB_CG57FONT,55,65,Player[0].name,100);
  LibPutFarData(92,42,135);  /* Player 1 area */  
  LibPutProStr(IB_CG57FONT,97,45,Player[1].name,100);  
  LibPutFarData(50,20,135);  /* Player 2 area */
  LibPutProStr(IB_CG57FONT,55,23,Player[2].name,100);
  LibPutFarData(4,42,135);   /* Player 3 area */
  LibPutProStr(IB_CG57FONT,9,45,Player[3].name,100);    
  LibPutDisp();

  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon );
  LibTchStackPush(TchNames); 
  LibTchInit();
  
  input_end=FALSE;
  modify=FALSE;
  while (input_end==FALSE){
   i=LibGetProStrSize(IB_CG57FONT,GetMsg(10));
   LibPutProStr(IB_CG57FONT,80-i/2,3,GetMsg(10),150);     
   LibPutDisp();

   LibTchWait(&tsts);
   switch (tsts.obj) {
   	  case OBJ_IC_NAME0:
   	    xp=50;
   	    yp=62;
        if (GetName(Player[0].name,xp+5,yp+3)==TRUE) {
           modify=TRUE;
           LibPutFarData(xp,yp,135);  /* Player 0 area */
           LibPutProStr(IB_CG57FONT,xp+5,yp+3,Player[0].name,100);
           LibPutDisp();
           img=GetImage();
           if (img!=NO_IMAGE) Player[0].pict=img;
           }
        break;
   	  case OBJ_IC_NAME1:
   	    xp=92;
   	    yp=42;
        if (GetName(Player[1].name,xp+5,yp+3)==TRUE) {
           modify=TRUE;
           LibPutFarData(xp,yp,135);  /* Player 1 area */
           LibPutProStr(IB_CG57FONT,xp+5,yp+3,Player[1].name,100);
           LibPutDisp();
           img=GetImage();
           if (img!=NO_IMAGE) Player[1].pict=img;
           }
        break;
   	  case OBJ_IC_NAME2:
   	    xp=50;
   	    yp=20;
        if (GetName(Player[2].name,xp+5,yp+3)==TRUE) {
           modify=TRUE;
           LibPutFarData(xp,yp,135);  /* Player 2 area */
           LibPutProStr(IB_CG57FONT,xp+5,yp+3,Player[2].name,100);
           LibPutDisp();
           img=GetImage();
           if (img!=NO_IMAGE) Player[2].pict=img;
           }
        break;
   	  case OBJ_IC_NAME3:
   	    xp=4;
   	    yp=42;
        if (GetName(Player[3].name,xp+5,yp+3)==TRUE) {
           modify=TRUE;
           LibPutFarData(xp,yp,135);  /* Player 3 area */
           LibPutProStr(IB_CG57FONT,xp+5,yp+3,Player[3].name,100);
           LibPutDisp();
           img=GetImage();
           if (img!=NO_IMAGE) Player[3].pict=img;
           }
        break;
      case OBJ_HIC_ESC:
		input_end = TRUE;
		break;
      }  
    }  
  if (modify==TRUE)
     SavePlayerNames();
  LibTchRelease();
  LibTchInit();
  LibCloseWindow();
  LibPutDisp();  
}  
	


/******************************************************************************
*	[Title]		GetName
*	[Arguments]	name: old name to modify
*               x,y: name position to write.
*	[ReturnVAL]	TRUE If name was modified 
*               FALSE if not
*               name: the new player's name
*                
*   [Description] 
*      Get the player's name.
******************************************************************************/
static bool GetName(char *name,int x,int y)
{
TXTP		EventPrm;				/* Declaration of structure for text input */
TCHTBL 		TchTxtBuf[3];			/* Text touch area table */
T_SCR_POS	ComScr;					/* Scroll bar position information */
word		GdTbl[1];				/* Guidance comment table */
byte		key_cd;
TCHSTS		tsts;
byte		input_end;
bool 		modify;
char 		EditString[NAME_MAX+1];          /* Input buffer */
 

    LibClrBox(0,0,160,12); /* Button area */
	LibPutFarData(40,0,149);
	LibPutProStr(IB_CG57FONT,42,2,GetMsg(5),28); /* Set */
	LibPutProStr(IB_CG57FONT,72,2,GetMsg(6),28); /* Clr */	
	LibPutProStr(IB_CG57FONT,103,2,GetMsg(7),28); /* ESC */	

	LibTchStackPush(NULL);
	LibTchStackPush(TchHardIcon);
	LibTchStackPush(TchHeaderIcon); 

    strcpy (EditString,name);           /* Init string buffer */
 
	GdTbl[0]		= 0xffff;			/* No Guidance comment */
	EventPrm.st_x	= x;				/* Start coordinate (X) of text display */
	EventPrm.st_y	= y;				/* Start coordinate (Y) of text display */
	EventPrm.ed_x	= 56+x;				/* End coordinate (X) of text display  */
	EventPrm.it_y	= 9;				/* Text display line spacing (Y) */
	EventPrm.MAXGYO	= 1;				/* Number of text display lines  */
	EventPrm.font	= IB_CG57FONT;		/* Display font type  */
	EventPrm.csen	= TRUE;				/* Cursor display enabled(Keyword registration enabled) */
	EventPrm.rtnen	= 0x02;				/* CR code display enabled  */
	EventPrm.maxmj	= NAME_MAX; 		/* Maximum number of allowable input characters. */
	EventPrm.txbf	= EditString;		/* Designation of text buffer address  */
	EventPrm.gdcmt	= GdTbl;			/* Guidance comment table  */
	EventPrm.txtobj	= OBJ_STRING;		/* Object when text area is touched.  */
	EventPrm.sbrobj	= NULL;				/* Object when the scroll bar is touched. */
	EventPrm.tchtb	= TchTxtBuf;		/* Pointer for text scroll bar area  */
	EventPrm.s_pos	= &ComScr;			/* Pointer for text and scroll bar information */

    
	LibTxtInit(&EventPrm);				/* Initialization of text input */
	LibTxtTchSet(&EventPrm);			/* Text touch area PUSH */

    LibDispKey();
	input_end = FALSE;
    modify=FALSE;
	while(input_end==FALSE){
	  LibTxtDsp(&EventPrm);
	  LibPutDisp();
	  key_cd = LibGetKeyM(&tsts);
	  if(key_cd == KEY_NONE)					/* Not keyboard */
		switch(tsts.obj){
		  case OBJ_HIC_MBAR:
			LibEditPullDown();
			break;
          case OBJ_HIC_ESC:
            modify=FALSE; 
			input_end=TRUE;
			break;					
  		  case OBJ_IC_SET:
			if(LibBlockIconClick(&TiconSet,&tsts,B_ICON_LEFT)==TRUE){
			  if (EditString[0]!='\0') {
				strcpy (name,EditString);
				input_end=TRUE;
                modify=TRUE;						  
				}
			  else
                LibWinIcnMsg(ICON_BADTZ,326,1);  /* Invalid name */	  	  						   
			  }
			break;
		  case OBJ_IC_CLR:
			if(LibBlockIconClick(&TiconClr,&tsts,B_ICON_CENTER)==TRUE){
			  EditString[0]='\0';
			  LibTxtInit(&EventPrm);
			  }
			break;
	      case OBJ_IC_ESC:
			if(LibBlockIconClick(&TiconEsc,&tsts,B_ICON_RIGHT)==TRUE){
              modify=FALSE;						
			  input_end=TRUE;
			  }
			break;
		  }
	  LibTxtInp(key_cd,&tsts,&EventPrm);
	  }
  LibClrBox(0,0,160,12); /* Clear buttons */
  LibClrBox(0,88,160,72);/* Clear keyboard */
  LibTchRelease();
  LibTchInit();
  return (modify);
}


/******************************************************************************
*	[Title]		GetImage
*	[Arguments]	
*	[ReturnVAL]	The image number selected
*   [Description] 
*       Select a picture for identify a player. Attached to.
*       Are 8 pictures (male and female) 40x40 dots each one.
*       You can modify these and rewrite them in the Global data module.
******************************************************************************/
static int GetImage()
{ TCHSTS tsts;  
  int sel;
  int i;
	
  LibClrBox(0,0,160,10);  /* Title */
  i=LibGetProStrSize(IB_CG57FONT,GetMsg(11));
  LibPutProStr(IB_CG57FONT,80-i/2,3,GetMsg(11),150);
  
  LibClrBox(0,80,160,80); /* Pictures */
  for (i=0;i<4;i++) {
  	LibPutGraph(40*i,80,Image[i]);
  	LibLine(40*i,80,40,1,1); /* square */
  	LibLine(40*i,80,1,40,1);
  	LibLine(40*i+39,80,1,40,1);
  	LibLine(40*i,119,40,1,1);
  	LibPutGraph(40*i,120,Image[4+i]);
  	LibLine(40*i,120,40,1,1); /* square */
  	LibLine(40*i,120,1,40,1);
  	LibLine(40*i+39,120,1,40,1);
  	LibLine(40*i,159,40,1,1);
    }
  LibPutDisp();
  
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchImages);
  LibTchInit();
  sel=10;
  while (sel==10) {
    LibTchWait (&tsts);
    if (tsts.obj>=OBJ_IC_IMAGE0 && tsts.obj<=OBJ_IC_IMAGE7)
      sel=tsts.obj-OBJ_IC_IMAGE0;
    }  
    if (tsts.obj==OBJ_HIC_ESC)
      sel=NO_IMAGE;	
   LibClrBox(0,0,160,10);  /* Clear title */
   LibClrBox(0,80,160,80); /* Clear Pictures */
   LibPutDisp();
   LibTchRelease();
   LibTchInit();
   return(sel);
}


/******************************************************************************
*	[Title]			LoadData
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*     Restore a previous save game from the flash memory
*     The structure is:
*	bool flsaved        TRUE is the data have a game info.
*	bool level;         Level number.
*	bool resume;        The game was saved at the end or in the middle
*	char name[N_PLAYERS][NAME_MAX+1];  Name of players
*	byte pict[N_PLAYERS];              Pictures asociated to each player
*	T_PLAYER Player[N_PLAYERS];        Dominoes, scores, etc for each player
*	T_SIDE Board[2];                   Dominoes used in the board
*
******************************************************************************/
static bool LoadData()
{  FILE_BUF domfb;
   FILE_INFO domfinf;
   byte fs_entry;
   byte fm_entry;
   char filename[10];   
	
	strcpy(filename,DATAFILENAME);
	if (LibSubEntrySave(filename,&fs_entry)==FALSE)   /* Filename Save */
	  return (FALSE);
	  
	LibGetAllEntry(filename,&fm_entry,&fs_entry);

	domfb.fsb_main_entry_=fm_entry;  /* Main entry */
	domfb.fsb_sub_entry_=fs_entry;   /* Sub entry  */
	domfb.fsb_scrt_info_=0x80;       /* Open Mode  */
	
	domfinf.fp=0xffff;                /* first data */
	domfinf.kind=FILE_KIND_BIN;       /* Binary mode */		
	
	if (LibFileFindNext(&domfb,&domfinf,0x00)==TRUE) {
     	LibFileRead(&domfb,&domfinf);
 	    memcpy(&FileInfo,&domfb.fbuf.bin.bin_buf,sizeof(FileInfo));     	
        return (TRUE);
        }
	else {
	  LibErrorDisp(FlashStatus);
	  return (FALSE);
	  }		
}


/******************************************************************************
*	[Title]			SaveData
*	[Arguments]		
*	[ReturnVAL]		
*   [Description] 
*     Save the game data to the flash memory
*     The structure is:
*	bool flsaved        TRUE is the data have a game info.
*	bool level;         Level number.
*	bool resume;        The game was saved at the end or in the middle
*	char name[N_PLAYERS][NAME_MAX+1];  Name of players
*	byte pict[N_PLAYERS];              Pictures asociated to each player
*	T_PLAYER Player[N_PLAYERS];        Dominoes, scores, etc for each player
*	T_SIDE Board[2];                   Dominoes used in the board
******************************************************************************/
static bool SaveData(void)
{  FILE_BUF domfb;
   FILE_INFO domfinf;
   byte fs_entry;
   byte fm_entry;
   char filename[10];


    strcpy(filename,DATAFILENAME);
	if (LibSubEntrySave(filename,&fs_entry)==FALSE)   /* Filename Save */
	  return (FALSE);
	  
	LibGetAllEntry(filename,&fm_entry,&fs_entry);

	domfb.fsb_main_entry_=fm_entry;  /* Main entry */
	domfb.fsb_sub_entry_=fs_entry;   /* Sub entry  */
	domfb.fsb_scrt_info_=0x80;       /* Open Mode  */
	
	domfinf.fp=0xffff;                /* first data */
	domfinf.kind=FILE_KIND_BIN;       /* Binary mode */		
	
	if (LibFileFindNext(&domfb,&domfinf,0x00)==FALSE) 
	   domfinf.fp=0xffff;                /* first data */
		
	domfinf.kind=FILE_KIND_BIN;
	
	memcpy(&domfb.fbuf.bin.bin_buf,&FileInfo,sizeof(FileInfo)); /* Info */
	domfb.fbuf.bin.char_num=sizeof(FileInfo);                   /* Size */
	
	if (LibFileWrite(&domfb,&domfinf)==TRUE) 
	  return (TRUE);
	else {
	  LibErrorDisp(FlashStatus);
	  return (FALSE);
	  }	
}	
