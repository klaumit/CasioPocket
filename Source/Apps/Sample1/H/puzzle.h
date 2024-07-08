/******************************************************************************
*	[Title]		Puzzle
*	[Model] 	PocketViewer
*	[Version]	Ver1.00
*
*	Copyright (C) 1997 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/


#define		OBJ_IC_NEW		   	0x9000
#define		OBJ_IC_GAMEAREA		0x9001
#define		OBJ_IC_LEVEL	   	0x9002
#define		OBJ_STRING			0x9003
#define		OBJ_IC_SET			0x9004
#define		OBJ_IC_CLR			0x9005
#define		OBJ_IC_HIGH			0x9006
#define		OBJ_IC_HOK			0x9007
#define		OBJ_IC_HCLR			0x9008
#define		OBJ_IC_HBOX			0x9009
#define		OBJ_IC_DSP_NONE		0x900A


#define		GAME_STOP	0
#define		GAME_RUN	1
#define		GAME_OVER	2

#define		TIMER_STOP	0
#define		TIMER_RUN	1
#define		TIMER_OVER	2

#define		NAME_MAX	7

#define		BOARD_MAX	5

#define		CARD_SIZE_X	 24
#define		CARD_SIZE_Y	 24

#define		LCD_X_DOT	160
#define		LCD_Y_DOT	160

#define		TIME_LU_X	  62
#define		TIME_LU_Y	  14


#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time		*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound				*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format				*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity				*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0020	/* Contrast				*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0040	/* Touch_Panel_Alignment*/


typedef struct SCORE{
	byte	lv1[NAME_MAX+1];			/* Level1 HighScore Name */
	int		lv1time;					/*                  Time */

	byte	lv2[NAME_MAX+1];			/* Level2 HighScore Name */
	int		lv2time;					/*                  Time */

	byte	lv3[NAME_MAX+1];			/* Level3 HighScore Name */
	int		lv3time;					/*                  Time */
}SCORE;

SCORE		*sc;
FILE_BUF	pzlfb;						/* FLASH data buffer (binaryBuffer 3KB)*/
FILE_INFO	pzlfinf;					/* FLASH data information */


struct work_area {
	int loc[BOARD_MAX][BOARD_MAX];		/*BouadSize*/
	int fx, fy;							/*SpacePosition */
	int sts;							/* GameStatus */
	int timests;						/* TimerStatus */
	int timer;							/* Timer */
};


byte	gamelevel;						/* Level */
int		board_size;						/* BOARD SIZE */

byte	fm_entry;						/* File Main Entry */
byte	fs_entry;						/* File Sub Entry */
byte 	filename[15+1] = "Game-C Data";	/* FileName */
byte	entry_cond;						/* File Entry condition */


