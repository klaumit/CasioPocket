/******************************************************************************
*       
*       [Title] 	Franka
*       [Model]         PocketViewer
*       [Version]       Ver1.10
*       [Copyright]	2000 Frank Tusche All rights reserved.
*
******************************************************************************/

#include <math.h>

/* touch ids */
#define		OBJ_IC_Prog		   	0x901D
#define		OBJ_IC_C			0x901E
#define		OBJ_IC_MR		   	0x901F
#define		OBJ_IC_MC			0x9020
#define		OBJ_IC_MPlus			0x9021
#define		OBJ_IC_MMinus			0x9022
#define		OBJ_IC_Back			0x9023
#define		OBJ_IC_Plus			0x9024
#define		OBJ_IC_Minus			0x9025
#define		OBJ_IC_Mal			0x9026
#define		OBJ_IC_Teilen			0x900A
#define		OBJ_IC_Result			0x900B
#define		OBJ_IC_KAuf			0x900C
#define		OBJ_IC_KZu			0x900D
#define		OBJ_IC_Function			0x900E
#define		OBJ_IC_PlusMinus		0x900F
#define		OBJ_IC_0			0x9010
#define		OBJ_IC_1			0x9011
#define		OBJ_IC_2			0x9012
#define		OBJ_IC_3			0x9013
#define		OBJ_IC_4			0x9014
#define		OBJ_IC_5			0x9015
#define		OBJ_IC_6			0x9016
#define		OBJ_IC_7			0x9017
#define		OBJ_IC_8			0x9018
#define		OBJ_IC_9			0x9019
#define		OBJ_IC_Dot			0x901A
#define		OBJ_IC_Trig			0x901B
#define		OBJ_IC_Off			0x901C

/* positions and sizes */
#define		KEY_LOX		17
#define		KEY_LOY		62
#define		KEY_SIZE_X	23
#define		KEY_SIZE_Y	17
#define		KEY_DIST_X	25
#define		KEY_DIST_Y	19

#define		LCD_X_DOT	160
#define		LCD_Y_DOT	160

#define		FRAME_X		15
#define		FRAME_Y		32
#define		FRAME_W		128
#define		FRAME_H		128

#define		DISPLAY_X	15
#define		DISPLAY_Y	13
#define		DISPLAY_W	128
#define		DISPLAY_H	48

/* System menu */
#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time	*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound		*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format		*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity		*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0020	/* Contrast		*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0040	/* Touch_Panel_Alignment*/

/*word		MPdSys[] = {/*
			/*38,	/* Set_date/time */
			/*33,	/* Sound */
			/*39,	/* Format */
			/*34,	/* Capacity */
			/*35,	/* Memory_management */
			/*37,	/* Contrast */
			/*40,	/* Touch_Panel_Alignment */
			/*PDNTBLEND,
		};*/

/* Edit menu */
/*word		MPdEdt[] = {PDNTBLEND};*/

/* Options menu */
/*word		MPdOpt[] = {PDNTBLEND};*/

/* texts to be displayed on calculator buttons */
byte		*keytexts[5][5]	= {
		{"MC", "MR", "M+", "M-", "\xd0"},
		{"7", "8", "9", "\xc4", "C"},
		{"4", "5", "6", "+", "-"},
		{"1", "2", "3", "\xc2", "\xc3"},
		{"0", ".", "=", "(", ")"}
		};

/* calcuator buttons */
TCHTBL far keys[] = {
	KEY_LOX+0*KEY_DIST_X,KEY_LOY+0*KEY_DIST_Y,KEY_LOX+0*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+0*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_MC,
	0x0000,

	KEY_LOX+1*KEY_DIST_X,KEY_LOY+0*KEY_DIST_Y,KEY_LOX+1*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+0*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_MR,
	0x0000,

	KEY_LOX+2*KEY_DIST_X,KEY_LOY+0*KEY_DIST_Y,KEY_LOX+2*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+0*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_MPlus,
	0x0000,

	KEY_LOX+3*KEY_DIST_X,KEY_LOY+0*KEY_DIST_Y,KEY_LOX+3*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+0*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_MMinus,
	0x0000,

	KEY_LOX+4*KEY_DIST_X,KEY_LOY+0*KEY_DIST_Y,KEY_LOX+4*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+0*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Back,
	0x0000,

	KEY_LOX+0*KEY_DIST_X,KEY_LOY+1*KEY_DIST_Y,KEY_LOX+0*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+1*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_7,
	0x0000,

	KEY_LOX+1*KEY_DIST_X,KEY_LOY+1*KEY_DIST_Y,KEY_LOX+1*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+1*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_8,
	0x0000,

	KEY_LOX+2*KEY_DIST_X,KEY_LOY+1*KEY_DIST_Y,KEY_LOX+2*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+1*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_9,
	0x0000,

	KEY_LOX+3*KEY_DIST_X,KEY_LOY+1*KEY_DIST_Y,KEY_LOX+3*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+1*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_PlusMinus,
	0x0000,

	KEY_LOX+4*KEY_DIST_X,KEY_LOY+1*KEY_DIST_Y,KEY_LOX+4*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+1*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_C,
	0x0000,

	KEY_LOX+0*KEY_DIST_X,KEY_LOY+2*KEY_DIST_Y,KEY_LOX+0*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+2*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_4,
	0x0000,

	KEY_LOX+1*KEY_DIST_X,KEY_LOY+2*KEY_DIST_Y,KEY_LOX+1*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+2*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_5,
	0x0000,

	KEY_LOX+2*KEY_DIST_X,KEY_LOY+2*KEY_DIST_Y,KEY_LOX+2*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+2*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_6,
	0x0000,

	KEY_LOX+3*KEY_DIST_X,KEY_LOY+2*KEY_DIST_Y,KEY_LOX+3*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+2*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Plus,
	0x0000,

	KEY_LOX+4*KEY_DIST_X,KEY_LOY+2*KEY_DIST_Y,KEY_LOX+4*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+2*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Minus,
	0x0000,

	KEY_LOX+0*KEY_DIST_X,KEY_LOY+3*KEY_DIST_Y,KEY_LOX+0*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+3*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_1,
	0x0000,

	KEY_LOX+1*KEY_DIST_X,KEY_LOY+3*KEY_DIST_Y,KEY_LOX+1*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+3*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_2,
	0x0000,

	KEY_LOX+2*KEY_DIST_X,KEY_LOY+3*KEY_DIST_Y,KEY_LOX+2*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+3*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_3,
	0x0000,

	KEY_LOX+3*KEY_DIST_X,KEY_LOY+3*KEY_DIST_Y,KEY_LOX+3*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+3*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Mal,
	0x0000,

	KEY_LOX+4*KEY_DIST_X,KEY_LOY+3*KEY_DIST_Y,KEY_LOX+4*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+3*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Teilen,
	0x0000,

	KEY_LOX+0*KEY_DIST_X,KEY_LOY+4*KEY_DIST_Y,KEY_LOX+0*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+4*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_0,
	0x0000,

	KEY_LOX+1*KEY_DIST_X,KEY_LOY+4*KEY_DIST_Y,KEY_LOX+1*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+4*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Dot,
	0x0000,

	KEY_LOX+2*KEY_DIST_X,KEY_LOY+4*KEY_DIST_Y,KEY_LOX+2*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+4*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Result,
	0x0000,

	KEY_LOX+3*KEY_DIST_X,KEY_LOY+4*KEY_DIST_Y,KEY_LOX+3*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+4*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_KAuf,
	0x0000,

	KEY_LOX+4*KEY_DIST_X,KEY_LOY+4*KEY_DIST_Y,KEY_LOX+4*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+4*KEY_DIST_Y+KEY_SIZE_Y,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_KZu,
	0x0000,

	0,0,0,0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far iMC = {
	&keys[ 0],NULL,NULL,0x03
};
T_ICON far iMR = {
	&keys[ 1],NULL,NULL,0x03,
};
T_ICON far iMPlus = {
	&keys[ 2],NULL,NULL,0x03,
};
T_ICON far iMMinus = {
	&keys[ 3],NULL,NULL,0x03,
};
T_ICON far iBack = {
	&keys[ 4],NULL,NULL,0x03
};
T_ICON far i7 = {
	&keys[ 5],NULL,NULL,0x03,
};
T_ICON far i8 = {
	&keys[ 6],NULL,NULL,0x03,
};
T_ICON far i9 = {
	&keys[ 7],NULL,NULL,0x03,
};
T_ICON far iPlusMinus = {
	&keys[ 8],NULL,NULL,0x03,
};
T_ICON far iC = {
	&keys[ 9],NULL,NULL,0x03
};
T_ICON far i4 = {
	&keys[10],NULL,NULL,0x03,
};
T_ICON far i5 = {
	&keys[11],NULL,NULL,0x03,
};
T_ICON far i6 = {
	&keys[12],NULL,NULL,0x03,
};
T_ICON far iPlus = {
	&keys[13],NULL,NULL,0x03,
};
T_ICON far iMinus = {
	&keys[14],NULL,NULL,0x03
};
T_ICON far i1 = {
	&keys[15],NULL,NULL,0x03,
};
T_ICON far i2 = {
	&keys[16],NULL,NULL,0x03,
};
T_ICON far i3 = {
	&keys[17],NULL,NULL,0x03,
};
T_ICON far iMal = {
	&keys[18],NULL,NULL,0x03,
};
T_ICON far iTeilen = {
	&keys[19],NULL,NULL,0x03
};
T_ICON far i0 = {
	&keys[20],NULL,NULL,0x03,
};
T_ICON far iDot = {
	&keys[21],NULL,NULL,0x03,
};
T_ICON far iResult = {
	&keys[22],NULL,NULL,0x03,
};
T_ICON far iKAuf = {
	&keys[23],NULL,NULL,0x03,
};
T_ICON far iKZu = {
	&keys[24],NULL,NULL,0x03
};

/* popup menu buttons */
TCHTBL far tbtns[] = {
	DISPLAY_X+59,47,DISPLAY_X+87,58,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Prog,
	0x0000,

	DISPLAY_X+30,47,DISPLAY_X+58,58,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Function,
	0x0000,

	DISPLAY_X+2,47,DISPLAY_X+30,58,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Trig,
	0x0000,

	DISPLAY_X+DISPLAY_W-2-30,47,DISPLAY_X+DISPLAY_W-2-1,58,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_Off,
	0x0000,

	0,0,0,0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far iProg = {
	&tbtns[0],NULL,NULL,0x00
};

T_ICON far iFunction = {
	&tbtns[1],NULL,NULL,0x00
};

T_ICON far iTrig = {
	&tbtns[2],NULL,NULL,0x00
};

T_ICON far iOff = {
	&tbtns[3],NULL,NULL,0x00
};

/* menus */
byte funcrtv[] = {1,2,7,6,3,4,5,0xff};
byte funccmnt[] = {"exp\xfeln\xfe\x31\x30^x\xfelog\xfex^y\xfex\xc6\xfe\xd1"};
byte trigrtv[] = {11,12,13,14,15,16,17,18,0xff};
byte trigcmnt[] = {"Pi\xfesin\xfe\x63os\xfetan\xfe\x63ot\xfe\x61sin\xfe\x61\x63os\xfe\x61tan"};
byte progrtv[] = {1,2,3,4,5,6,7,8,0xff};
byte progcmnt[] = {"Lrn 1\xfeLrn 2\xfeLrn 3\xfeRun 1\xfeRun 2\xfeRun 3\xfeSave\xfeLoad"};

/* for calculator */
#define		maxInput	14	/* max. length of display */
#define		maxStack	20	/* max. depth of number stack */
#define		maxOps		20	/* max. depth of operator stack */

byte		*_Error		="Error";   /* string for display */

word 		lkey;			/* last key */
byte		opString[maxOps];	/* operator stack */
byte		nops;			/* depth of operator stack */
double		Stack[maxStack];	/* number stack */
byte		StackSize;		/* depth of number stack */
double		memory;			/* memory */
double		lastResult;		/* display content as double */
byte		dispString[maxInput*2+1]; /* display content as string */

/* for macros */
bool		learning;	/* learn mode */
bool		running;	/* run mode */
byte		prognr;		/* program to learn/run */

#define		maxCmd		20	/* max. number of commands per program */

typedef struct {		/* a command */
	word	key;
	byte	subkey;
} cmd;

cmd		commands[3][maxCmd];	/* the three programs */
byte		cmdLen[3];		/* # of commands of the three programs */
byte		cmdPos;			/* # of command to be executed/learned */

cmd		command;		/* command to be executed */

/* for fileaccess */
byte		fm_entry;			/* File Main Entry */
byte		fs_entry;			/* File Sub Entry */
byte 		filename[15+1] = "Calc-Progs";	/* FileName */
byte		entry_cond;			/* File Entry condition */
FILE_BUF	pzlfb;				/* FLASH data buffer (binaryBuffer 3KB)*/
FILE_INFO	pzlfinf;			/* FLASH data information */

/*** function prototypes ***/
static void init(void);
void showDisplay(void);
void initDisplay(void);
void setDisp(double);
void error(void);
bool isError(void);
void clear(void);
bool checkDot(void);
void push(void);
double pop(void);
void pushOp(byte);
byte popOp(void);
byte peekOp(void);
void addCmd(word, byte);
bool nextCommand(void);
void execute(byte);
void execFunc(byte);
byte selFunction(bool manual, byte xp, byte *rtv, byte *cmnt);
void execProg(byte);
void DataRead(void);
void DataSave(void);
static void tchSet(void);
void GmPdwnSet(void);
bool PdwnFncSys(word sel);
bool GmPdwnFnc();

