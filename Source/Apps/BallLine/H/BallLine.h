/******************************************************************************
*	[Title]		PVBallLines
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#define		OBJ_IC_NEW		   	0x9000
#define		OBJ_IC_GAMEAREA		0x9001
#define		OBJ_IC_LEVEL	   	0x9002
#define		OBJ_STRING				0x9003
#define		OBJ_IC_SET				0x9004
#define		OBJ_IC_CLR				0x9005
#define		OBJ_IC_HIGH				0x9006
#define		OBJ_IC_HOK				0x9007
#define		OBJ_IC_HCLR				0x9008
#define		OBJ_IC_HBOX				0x9009
#define		OBJ_IC_DSP_NONE		0x900A
#define		OBJ_IC_MESSAGE			0x900B

#define		GAME_STOP	0
#define		GAME_RUN		1
#define		GAME_OVER	2

#define		NAME_MAX		7
#define		BOARD_MAX	5

#define		CARD_SIZE_X	 24
#define		CARD_SIZE_Y	 24

#define		LCD_X_DOT	160
#define		LCD_Y_DOT	160

#define		TIME_LU_X	  62
#define		TIME_LU_Y	  14

typedef struct SCORE{
	byte	lv1[NAME_MAX+1];			/* Level1 HighScore Name */
	int		lv1score;					/*                  Time */

	byte	lv2[NAME_MAX+1];			/* Level2 HighScore Name */
	int		lv2score;					/*                  Time */

	byte	lv3[NAME_MAX+1];			/* Level3 HighScore Name */
	int		lv3score;					/*                  Time */
}SCORE;

SCORE		*sc;
FILE_BUF	pzlfb;						/* FLASH data buffer (binaryBuffer 3KB)*/
FILE_INFO	pzlfinf;					/* FLASH data information */


struct work_area {
	int sts;
};


byte	gamelevel;						/* Level */
int		board_size;					/* BOARD SIZE */

byte	fm_entry;						/* File Main Entry */
byte	fs_entry;						/* File Sub Entry */
byte 	filename[15+1] = "PV BallLines";/* FileName */
byte	entry_cond;						/* File Entry condition */


#define	CARD_SIZE		16
#define	CARD_NUM			9
#define	CIR_R				7
#define	BOARD_START_X	8
#define	BOARD_START_Y	14
int	far	Cir_num;
int	far	Data[CARD_NUM+2][CARD_NUM+2];
int	far	Data1[CARD_NUM+2][CARD_NUM+2];
int	far 	deleteIt[CARD_NUM*CARD_NUM+1][2];
int	far	cx,cy,sx,sy,dx,dy,tx,ty;
bool	far	click;
static int	score;