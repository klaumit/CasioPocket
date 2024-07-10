/******************************************************************************
*	[Title]		BlackJack
*	[Model]	 	PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#define		OBJ_IC_YES0			0x9000
#define		OBJ_IC_NO0			0x9001
#define		OBJ_IC_HIT1			0x9002
#define		OBJ_IC_STAND1		0x9003
#define		OBJ_IC_SPLIT1		0x9004
#define		OBJ_IC_HIT2			0x9005
#define		OBJ_IC_STAND2		0x9006
#define		OBJ_IC_SPLIT2		0x9007
#define		OBJ_IC_HIT3			0x9008
#define		OBJ_IC_STAND3		0x9009
#define		OBJ_IC_SPLIT3		0x900A
#define		OBJ_IC_HIT4			0x900B
#define		OBJ_IC_STAND4		0x900C
#define		OBJ_IC_YES1			0x900D
#define		OBJ_IC_NO1			0x900E
#define		OBJ_IC_YES2			0x900F
#define		OBJ_IC_NO2			0x9010
#define		OBJ_IC_YES3			0x9011
#define		OBJ_IC_NO3			0x9012
#define		OBJ_IC_YES4			0x9013
#define		OBJ_IC_NO4			0x9014
#define		OBJ_IC_SET			0x9015
#define		OBJ_IC_CLR			0x9016
#define		OBJ_STRING			0x9017
#define		OBJ_IC_GAMEAREA	0x9018

#define		GAME_STOP	0
#define		GAME_RUN	1
#define		GAME_OVER	2

#define		CARD_SIZE_X		29
#define		CARD_SIZE_Y		40

#define		LCD_X_DOT		160
#define		LCD_Y_DOT		160

#define		START			0

#define		NAME_MAX	6
#define		CARD_MAX	52
#define		CARD_START_DX	0
#define		CARD_START_DY	18
#define		CARD_START_Y1X	0
#define		CARD_START_Y1Y	60
#define		CARD_START_Y2X	0
#define		CARD_START_Y2Y	110

struct CARD {		/* struct for every card */
	int	color;
	int	number;
};

struct work_area {
	struct CARD loc[CARD_MAX];	/* all cards */
	int symb[3];				/* symbol status */
	int sts;					/* GameStatus */
};

byte *Num2Char[] = {		/* card number change to char */
	"JOKER","A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
};