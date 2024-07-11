/******************************************************************************
*   Program Title	Domino
*   Module			
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		01/2001
*	Rev     		1.2
*	MMSoft 2001 - Freeware
*	Notes:	
*
******************************************************************************/
#include    <define.h>


#define N_PLAYERS 4     /* Maximum players in game     */
#define N_DOMS    7     /* Maximum dominoes for player */
#define NOWIN    -1

#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define NODIR 9

#define DOM_WIDTH  25    /* Domino width      */
#define DOM_HEIGHT 12    /* Domino height     */

#define BOARD_X1 5
#define BOARD_X2 154
#define BOARD_Y1 5
#define BOARD_Y2 133

#define NAME_MAX 9
#define DATAFILENAME "DOMINO"

/* Pulldown menu options */
#define		PDWN_EDT_NEW	PULLDOWN_EDIT	| 0x0001    /* New game				*/
#define		PDWN_EDT_SAVE	PULLDOWN_EDIT	| 0x0002    /* Save game			*/

#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time		*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound				*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format				*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity				*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_LANG	PULLDOWN_SYSTEM | 0x0020	/* Language selection	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0040	/* Contrast				*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0080	/* Touch_Panel_Alignment*/
#define 	PDWN_SYS_COMM	PULLDOWN_SYSTEM | 0x0100	/* Communication data 	*/

#define		PDWN_OPT_LEV1	PULLDOWN_OPTION	| 0x0001    /* Level 1				*/
#define		PDWN_OPT_LEV2	PULLDOWN_OPTION	| 0x0002    /* Level 2				*/
#define		PDWN_OPT_SCORE	PULLDOWN_OPTION	| 0x0004    /* View scores			*/
#define		PDWN_OPT_NAME	PULLDOWN_OPTION	| 0x0008    /* Edit names 			*/


/* Touch Table Objects */
#define OBJ_DOM0   0xA000
#define OBJ_DOM1   0xA001
#define OBJ_DOM2   0xA002
#define OBJ_DOM3   0xA003
#define OBJ_DOM4   0xA004
#define OBJ_DOM5   0xA005
#define OBJ_DOM6   0xA006
#define OBJ_IDLE   0xA007
#define OBJ_BOARD  0xA010
#define OBJ_IC_YES 0xA011
#define OBJ_IC_NO  0xA012

#define TCH_END {0,0,0,0,ACT_NONE,OBJ_END,0x0000}


typedef struct {
  int x,y;          /* Coords for next piece          */
  int dir;          /* Next piece direction           */ 
  int end;          /* Points to match next piece     */
  int tchX1;  		/* Touch area to point new domino */
  int tchY1;
  int tchX2;
  int tchY2;
  int len;
  byte dom[28];
} T_SIDE;
  	
typedef struct {
  byte pts1;
  byte pts2;
  bool used;
} T_FICHA;
	  	
typedef struct {
  byte domino;  /* Info for each domino */
  bool used;   /* Domino was used? */  
} T_DOMINO;

typedef struct {
  char name[NAME_MAX+1];  /* Player name */
  byte pict;              /* Picture for face */ 
  T_DOMINO token[N_DOMS]; /* token info  */
  int score;              /* score       */
  int nwins;			  /* times the player was winner (only for draw)*/	
} T_PLAYER;   

/* BitMaps */
extern byte far ImgPlayer0[];
extern byte far ImgPlayer1[];
extern byte far ImgPlayer2[];
extern byte far ImgPlayer3[];
extern byte far ImgPlayer4[];
extern byte far ImgPlayer5[];
extern byte far ImgPlayer6[];
extern byte far ImgPlayer7[];
extern byte far *Image[];
extern byte far ShapeL[];
extern byte far ShapeR[];
extern byte far Copa[];
extern byte far Logo[];


extern T_SIDE Board[2];
extern T_PLAYER Player[N_PLAYERS];
extern T_FICHA Domino[28];

extern bool newgame;     /* Global variable to start the game again */
extern int levelgame;    /* Level */
extern bool resume;     /* The game can resume if previously saved */

extern void PlaceDomino(int,int);  /* Interface to place a piece */
extern void DrawDomino(int,int,int,int,int);
extern void DrawPlayerDomino(int);
extern void GmPdwnSet(void);
extern void GmPdwnFnc(void);
extern void LoadGame(void);
extern char *GetMsg(int);
extern void MsgPlayer(int,int);
extern bool ComputerSelect(int,int*,int*);
extern bool HumanSelect(int*,int*);
