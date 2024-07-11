/******************************************************************************
*   Program Title	Battle Ship
*   Module			Library reference
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2000 
*	Rev     		1.3
*	MMSoft 2000 - Freeware
*	Notes:	
*
******************************************************************************/
#include    <define.h>
#define TCH_END {0,0,0,0,ACT_NONE,OBJ_END,0x0000}

#define PL_CASIO 0
#define PL_HUMAN 1
#define PL_ABORT 2

#define N_SHIPS    8
#define CELL_SIZE  9                /* Square Cell size in pixels */
#define TOTAL_HITS 4+3+3+2+2+1+1+1 /* Cell sum for all ships */
#define MSG_LEN    255

/* SHIP Directions */
#define SD_VER 0   /* vertical ship */
#define SD_HOR 1   /* horizontal ship */

/* Ship & cell status */
#define ID_NONE  0   /* Cell_Id for a non discovered & empty   */
#define ID_SHIP 10   /* Cell_Id for a non discovered ship      */
#define ID_FAIL 20   /* Cell_Id for a discovered & failed shot */ 
#define ID_HIT  30   /* Cell_Id for a discovered ship (hit)    */
#define ID_SUNK 40   /* Cell_Id for a destroyed ship           */

/* Shot results */
#define SH_NULL 0    /* Invalid shot */
#define SH_FAIL 1    /* Fail shot    */
#define SH_HIT  2    /* Hit shot     */
#define SH_SUNK 3    /* Sunk shot    */

/* Ship status in preset */
#define ST_BOARD   0
#define ST_TABLE   1
#define ST_SELECT  2

/* Symbol references */
#define SY_NONE   0
#define SY_FAIL   1
#define SY_HIT    2
#define SY_SUNK   3
#define SY_SHIP   4

/* Touch Table Objects */
#define OBJ_BD_HUMAN   0xA000
#define OBJ_IC_HYES    0xA001
#define OBJ_IC_HNO     0xA002

/* Pulldown menu options */
#define		PDWN_EDT_NEW	PULLDOWN_EDIT	| 0x0001    /* New game				*/

#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time		*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound				*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format				*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity				*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_LANG	PULLDOWN_SYSTEM | 0x0020	/* Language selection	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0040	/* Contrast				*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0080	/* Touch_Panel_Alignment*/
#define 	PDWN_SYS_COMM	PULLDOWN_SYSTEM | 0x0100	/* Communication data 	*/

#define		PDWN_OPT_SAVE	PULLDOWN_OPTION	| 0x0001    /* Save game			*/
#define		PDWN_OPT_LOAD	PULLDOWN_OPTION	| 0x0002    /* Restore game			*/

typedef struct {
  int x;          /* X origin for board area  */
  int y;          /* Y origin for board area  */
  int xs;         /* X origin for score area  */
  int ys;         /* Y origin for score area  */
  int shots;      /* shots counter            */
  int hits;       /* hits counter             */
  int cell[8][8]; /* Column & row cell info   */
} T_BOARD;
  
/* Struct to control each ship in the table */  
typedef struct {
  int size;     /* Ship size in cells       */
  int stat;     /* Ship status in the table */
  int direc;    /* ship direction to cut paste operations */
} T_SHIP;

extern T_SHIP far ShipInfo[N_SHIPS]; /* Common Ship data */  	
extern T_BOARD far Board[2];         /* Player's Board info */
extern byte far Logo[];              /* Logo Graph */
extern byte far *SymbolData[];       /* Symbol data table to show guns & ships */
extern byte far SymbolCoor[];

extern TCHTBL far TchBoard[];        /* Touch area definitions */
extern char *helpmsg[1];

/* Board & screen functions */
extern void BattleScreen(void);
extern void ShowBoardAll(byte,bool);
extern void ClearBoard(byte pl);
extern void DrawBoard(int,int);

/* Miscelaneous functions */
extern void GetCell(TCHSTS,byte,int *,int *);
extern byte CheckShot(byte,int,int);
extern bool CheckEnd(byte);
extern void ComputerShot(int *,int *);
extern int GetShipId(int);
extern void FillAround (byte,int);

/* Language functions */
extern void MsgShotInfo (int,int,int);
extern void MsgPlayerTurn (byte);
extern void MsgScore(byte);
extern byte MsgEndGame();
extern char *GetMsg(int);

/* Ship settings functions */
extern void InitPlayerBoard(void);
extern void RandomBoard(byte);

