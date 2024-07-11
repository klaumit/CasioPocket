/******************************************************************************
*  Program Title  Escoba
*  Module         References
*  Author         J. Marcos Matilla (marcosjo@wanadoo.es)
*  Date           12/2001
*  Rev            1.0
*  MMSoft 2000 - 2001 Freeware
*  Notes:   
*
******************************************************************************/
#include    <define.h>

/* #define DEBUG */

/* Cards Objects */
#define TCH_END {0,0,0,0,ACT_NONE,OBJ_END,0x0000}

#define OBJ_WINMSG      0xA000   /* Touch in Window message area  */
#define OBJ_PLAY0       0xA001   /*   "   in card 0 (left)        */
#define OBJ_PLAY1       0xA002   /*   "   in card 1 (middle)      */
#define OBJ_PLAY2       0xA003   /*   "   in card 2 (right)       */

#define OBJ_TBL0        0xA010   /*   "   in table card 0         */ 
#define OBJ_TBL1        0xA011   /*   "   in table card 1         */ 
#define OBJ_TBL2        0xA012   /*   "   in table card 2         */ 
#define OBJ_TBL3        0xA013   /*   "   in table card 3         */ 
#define OBJ_TBL4        0xA014   /*   "   in table card 4         */ 
#define OBJ_TBL5        0xA015   /*   "   in table card 5         */ 
#define OBJ_TBL6        0xA016   /*   "   in table card 6         */ 
#define OBJ_TBL7        0xA017   /*   "   in table card 7         */ 
#define OBJ_TBL8        0xA018   /*   "   in table card 8         */ 
#define OBJ_TBL9        0xA019   /*   "   in table card 9         */ 
#define OBJ_TBL10       0xA01A   /*   "   in table card 10        */ 
#define OBJ_TBL11       0xA01B   /*   "   in table card 11        */ 

#define OBJ_DISC        0xA020   /*   "   in discard area         */    

/* Settings objects */
#define OBJ_SET_LEV1    0xA030   /*   "   level 1 option          */ 
#define OBJ_SET_LEV2    0xA031   /*   "   level 2 option          */ 
#define OBJ_SET_LEV3    0xA032   /*   "   level 3 option          */ 
#define OBJ_SET_DEMO    0xA033   /*   "   demo option             */ 
#define OBJ_SET_PLY2    0xA034   /*   "   2 players option        */ 
#define OBJ_SET_PLY3    0xA035   /*   "   3 players option        */ 
#define OBJ_SET_YES     0xA036   /*   "   YES button              */ 
#define OBJ_SET_ESC     0xA037   /*   "   ESC button              */ 

/* Global constants */
#define MAXPLAYERS      3        /* Player's number                        */
#define CARDS_IN_PLAYER 3        /* Cards for each player                  */
#define CARDS_IN_TABLE  12       /* Maximum cards number on the table      */
#define PL_ME           0        /* player number for human                */
#define PL_RIGHT        1        /* Player number for right position       */
#define PL_LEFT         2        /* Player number for left position        */

#define NOCARD          -1

/* reference for coordinates */
#define LEFT            0
#define TOP             1
#define RIGHT           2
#define BOTTOM          3

/* discard & Selected cards area */
#define  DISCARD_L      31                   /* discard area left coordinate           */ 
#define  DISCARD_T      98                   /* discard area top coordinate            */
#define  DISCARD_R      128                  /* discard area right coordinate          */
#define  DISCARD_B      127                  /* discard area bottom coordinate         */

/* Decks */
#define  DECK0_L        130                  /* Player 0 Deck X coordinate             */
#define  DECK0_T        133                  /* Player 0 Deck Y coordinate             */
#define  DECK1_L        130                  /* Player 1 Deck X coordinate             */
#define  DECK1_T        10                   /* Player 1 Deck Y coordinate             */
#define  DECK2_L        1                    /* Player 2 Deck X coordinate             */
#define  DECK2_T        10                   /* Player 2 Deck Y coordinate             */
#define  DECKT_L        2                    /* Table    Deck X coordinate             */
#define  DECKT_T        129                  /* Table    Deck Y coordinate             */

/* define card's area for player 0 */
#define ME_CARD0_L      60                   /* player's CARD 0 left coordinate        */
#define ME_CARD0_T      131                  /* player's CARD 0 top coordinate         */
#define ME_CARD0_R      ME_CARD0_L+18        /* player's CARD 0 right position         */
#define ME_CARD0_B      ME_CARD0_T+28        /* player's CARD 0 bottom position        */

#define ME_CARD1_L      ME_CARD0_R+4         /* player's CARD 1 left coordinate        */
#define ME_CARD1_T      ME_CARD0_T           /* player's CARD 1 top coordinate         */
#define ME_CARD1_R      ME_CARD1_L+18        /* player's CARD 1 right position         */
#define ME_CARD1_B      ME_CARD0_B           /* player's CARD 1 bottom position        */

#define ME_CARD2_L      ME_CARD1_R+4         /* player's CARD 2 left coordinate        */
#define ME_CARD2_T      ME_CARD0_T           /* player's CARD 2 top coordinate         */
#define ME_CARD2_R      ME_CARD2_L+18        /* player's CARD 2 right position         */
#define ME_CARD2_B      ME_CARD0_B           /* player's CARD 2 bottom position        */

/* define card's area to select in the table */
#define TABLE0_L        36                   /* table's card 1 left coordinate         */
#define TABLE0_T        1                    /* table's card 1 top coordinate          */
#define TABLE0_R        TABLE0_L+22          /* table's card 1 right position          */
#define TABLE0_B        TABLE0_T+32          /* table's card 1 bottom position         */

#define TABLE1_L        TABLE0_R             /* table's card 2 left coordinate         */
#define TABLE1_T        TABLE0_T             /* table's card 2 top coordinate          */
#define TABLE1_R        TABLE1_L+22          /* table's card 2 right position          */
#define TABLE1_B        TABLE1_T+32          /* table's card 2 bottom position         */

#define TABLE2_L        TABLE1_R             /* table's card 3 left coordinate         */
#define TABLE2_T        TABLE0_T             /* table's card 3 top coordinate          */
#define TABLE2_R        TABLE2_L+22          /* table's card 3 right position          */
#define TABLE2_B        TABLE2_T+32          /* table's card 3 bottom position         */

#define TABLE3_L        TABLE2_R             /* table's card 4 left coordinate         */
#define TABLE3_T        TABLE0_T             /* table's card 4 top coordinate          */
#define TABLE3_R        TABLE3_L+22          /* table's card 4 right position          */
#define TABLE3_B        TABLE3_T+32          /* table's card 4 bottom position         */

#define TABLE4_L        TABLE0_L             /* table's card 5 left coordinate         */
#define TABLE4_T        TABLE0_B             /* table's card 5 top coordinate          */
#define TABLE4_R        TABLE4_L+22          /* table's card 5 right position          */
#define TABLE4_B        TABLE4_T+32          /* table's card 5 bottom position         */

#define TABLE5_L        TABLE1_L             /* table's card 5 left coordinate         */
#define TABLE5_T        TABLE1_B             /* table's card 5 top coordinate          */
#define TABLE5_R        TABLE5_L+22          /* table's card 5 right position          */
#define TABLE5_B        TABLE5_T+32          /* table's card 5 bottom position         */

#define TABLE6_L        TABLE2_L             /* table's card 6 left coordinate         */
#define TABLE6_T        TABLE2_B             /* table's card 6 top coordinate          */
#define TABLE6_R        TABLE6_L+22          /* table's card 6 right position          */
#define TABLE6_B        TABLE6_T+32          /* table's card 6 bottom position         */

#define TABLE7_L        TABLE3_L             /* table's card 7 left coordinate         */
#define TABLE7_T        TABLE3_B             /* table's card 7 top coordinate          */
#define TABLE7_R        TABLE7_L+22          /* table's card 7 right position          */
#define TABLE7_B        TABLE7_T+32          /* table's card 7 bottom position         */

#define TABLE8_L        TABLE0_L             /* table's card 8 left coordinate         */
#define TABLE8_T        TABLE4_B             /* table's card 8 top coordinate          */
#define TABLE8_R        TABLE8_L+22          /* table's card 8 right position          */
#define TABLE8_B        TABLE8_T+32          /* table's card 8 bottom position         */

#define TABLE9_L        TABLE1_L             /* table's card 9 left coordinate         */
#define TABLE9_T        TABLE5_B             /* table's card 9 top coordinate          */
#define TABLE9_R        TABLE9_L+22          /* table's card 9 right position          */
#define TABLE9_B        TABLE9_T+32          /* table's card 9 bottom position         */
   
#define TABLE10_L       TABLE2_L             /* table's card 10 left coordinate        */
#define TABLE10_T       TABLE6_B             /* table's card 10 top coordinate         */
#define TABLE10_R       TABLE10_L+22         /* table's card 10 right position         */
#define TABLE10_B       TABLE10_T+32         /* table's card 10 bottom position        */

#define TABLE11_L       TABLE3_L             /* table's card 11 left coordinate        */
#define TABLE11_T       TABLE7_B             /* table's card 11 top coordinate         */
#define TABLE11_R       TABLE11_L+22         /* table's card 11 right position         */
#define TABLE11_B       TABLE11_T+32         /* table's card 11 bottom position        */

/* Settings window areas */
#define SETL1_L         21                   /* Settings window Level 1 left pos.      */
#define SETL1_T         50                   /* Settings window Level 1 top pos.       */
#define SETL1_R         SETL1_L+8            /* Settings window Level 1 right pos.     */
#define SETL1_B         SETL1_T+8            /* Settings window Level 1 bottom pos.    */

#define SETL2_L         SETL1_L              /* Settings window Level 2 left pos.      */
#define SETL2_T         SETL1_T+10           /* Settings window Level 2 top pos.       */
#define SETL2_R         SETL1_R              /* Settings window Level 2 right pos.     */
#define SETL2_B         SETL2_T+8            /* Settings window Level 2 bottom pos.    */

#define SETL3_L         SETL1_L              /* Settings window Level 3 left pos.      */
#define SETL3_T         SETL2_T+10           /* Settings window Level 3 top pos.       */
#define SETL3_R         SETL1_R              /* Settings window Level 3 right pos.     */
#define SETL3_B         SETL3_T+8            /* Settings window Level 3 bottom pos.    */

#define SETPL2_L        83                   /* Settings 2 player option left pos.     */
#define SETPL2_T        50                   /* Settings 2 player option top pos.      */
#define SETPL2_R        SETPL2_L+8           /* Settings 2 player option right pos.    */
#define SETPL2_B        SETPL2_T+8           /* Settings 2 player option bottom pos.   */

#define SETPL3_L        SETPL2_L             /* Settings 3 player option left pos.     */
#define SETPL3_T        SETPL2_T+10          /* Settings 3 player option top pos.      */
#define SETPL3_R        SETPL3_L+8           /* Settings 3 player option right pos.    */
#define SETPL3_B        SETPL3_T+8           /* Settings 3 player option bottom pos.   */

#define SETDEMO_L       40                   /* Demo option left position              */
#define SETDEMO_T       90                   /* Demo option top  position              */
#define SETDEMO_R       SETDEMO_L+8          /* Demo option right position             */
#define SETDEMO_B       SETDEMO_T+8          /* Demo option bottom position            */

#define SETB1_L         50                   /* Yes button left position               */
#define SETB1_T         120                  /* Yes button top  position               */
#define SETB1_R         SETB1_L+29           /* Yes button right position              */
#define SETB1_B         SETB1_T+15           /* Yes button bottom position             */
   
#define SETB2_L         90                   /* ESC button left position               */
#define SETB2_T         SETB1_T              /* ESC button top  position               */
#define SETB2_R         SETB2_L+29           /* ESC button right position              */
#define SETB2_B         SETB2_T+15           /* ESC button bottom position             */


#define MAXCARDS        40                   /* Number of cards for deal, count, etc   */
#define CARD_BACK       MAXCARDS             /* Card number to card's back picture     */
#define CARD_BACKL      MAXCARDS+1           /* Card number to long card's back pict.  */

#define T_OROS          0
#define T_COPAS         1
#define T_ESPADAS       2
#define T_BASTOS        3

/* Pulldown menu options */
#define  PDWN_EDT_NEW   PULLDOWN_EDIT   | 0x0001   /* New game             */

#define  PDWN_SYS_DTTM  PULLDOWN_SYSTEM | 0x0001   /* Set_date/time        */
#define  PDWN_SYS_SOUND PULLDOWN_SYSTEM | 0x0002   /* Sound                */
#define  PDWN_SYS_LANG  PULLDOWN_SYSTEM | 0x0004   /* Language             */
#define  PDWN_SYS_CNTRS PULLDOWN_SYSTEM | 0x0008   /* Contrast             */
#define  PDWN_SYS_TPA   PULLDOWN_SYSTEM | 0x0010   /* Touch_Panel_Alignment*/

#define  PDWN_OPT_SET   PULLDOWN_OPTION | 0x0001   /* Game Settings        */
#define  PDWN_OPT_TOOL  PULLDOWN_OPTION | 0x0002   /* Pop Up Tools         */
#define  PDWN_OPT_CALC  PULLDOWN_OPTION | 0x0004   /* Calculator           */

/* Macro functions */
#define PrintCard(x,y,cardId) LibPutGraph(x,y,ImgCard[cardId])	/* Print a card 	*/
#define GetValue(cardId)      (1+cardId%10)							/* Card's value 	*/
#define GetSuit(cardId)       (cardId/10)								/* Card's Suit		*/ 

typedef struct {
   byte Id;                /* card number			                     */
   bool select;            /* The card is selected             		*/
   int coord[4];           /* card's coordinates							*/
} TCard; 

typedef struct {           /* Estructura de control de cada jugador  */
   TCard card[3];          /* Numero de las cartas del jugador       */
   byte history[MAXCARDS]; /* Historial de cartas ganadas            */
   byte ncards;            /* Cartas conseguidas                     */
   byte escobas;           /* Escobas conseguidas                    */
} TPlayer;  

typedef struct {
   byte npoints;           /* Escobas conseguidas                    */
   byte ncards;
   byte ngolds;
   byte nseven;
   bool gold7;
} TScore ;  
  
typedef struct {           /* Deck struct                            */ 
   byte remain;            /* Total cards remaining in deck          */
   byte card[MAXCARDS];    /* Cards in deck after shuffle            */
} TDeck;                   

typedef struct {           /* Struct for valid combinations          */
   int score;
   byte card[CARDS_IN_TABLE+2];
} TComb;

/* Global variables */   
/* Cards coordinates to draw them */
extern int Pl0Coords[CARDS_IN_PLAYER][4];
extern int Pl1Coords[CARDS_IN_PLAYER][4];
extern int Pl2Coords[CARDS_IN_PLAYER][4];
extern int TblCoords[CARDS_IN_TABLE][4];

extern byte far logo[];
extern byte far *ImgCard[MAXCARDS+2];  /* Card's pictures */
extern byte far Asigna[];
extern TCard table[CARDS_IN_TABLE];
extern TPlayer player[MAXPLAYERS];     /* Player's structure   */
extern TDeck deck;                     /* Deck cards           */
extern bool Initialize;                   /* Initialize game         */
extern int GameLevel;                  /* Global for level     */
extern bool GameDemo;
extern int nPlayers;

/* Global functions */
extern int  GetSeed(void);
extern void Suffle(void);
extern void DealPlayers(void);
extern void DealTable(void);
extern void DispPlayerCards(int); 
extern void DispPlayerDeck(int);
extern void DispTableCards(void); 
extern void DispTableDeck(void);
extern void DispDiscardArea(int);
extern void ShowScores(void);
extern char *GetMsg(int);
extern void GmPdwnFnc(void);
extern void WinMsgBox (char *,int,int);
extern void Settings (void);
extern void SlideCard(int,int,int,int,byte);
extern void CheckHrdIcn(int);
