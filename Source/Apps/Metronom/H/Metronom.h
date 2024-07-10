/******************************************************************************
*	[Title]		Metronome Head File
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include    "define.h"
/************************/
/************************/
extern byte far GraphNumKey[];
extern byte far *GraphPricker[];
extern byte far GraphTime[];

#define	OBJ_IC_NUMKEY0		0x9000
#define	OBJ_IC_NUMKEY7		0x9001
#define	OBJ_IC_NUMKEY8		0x9002
#define	OBJ_IC_NUMKEY9		0x9003
#define	OBJ_IC_NUMKEY4		0x9004
#define	OBJ_IC_NUMKEY5		0x9005
#define	OBJ_IC_NUMKEY6		0x9006
#define	OBJ_IC_NUMKEY1		0x9007
#define	OBJ_IC_NUMKEY2		0x9008
#define	OBJ_IC_NUMKEY3		0x9009
#define	OBJ_IC_NUMKEYBACK	0x900A
#define	OBJ_IC_NUMKEYSET	0x900B
#define	OBJ_IC_NUMKEYAC	0x900C

#define	OBJ_IC_START		0x900D
#define	OBJ_IC_NUMSET		0x900E
#define	OBJ_IC_TIMESET		0x900F

#define	PRICKER_X		22
#define	PRICKER_Y		42
#define	PRICKER_X_SIZE		114
#define	PRICKER_Y_SIZE		66
#define	NUMKEYSTART_X	4
#define	NUMKEYSTART_Y	60
#define	NUMKEYSIZE_X	21
#define	NUMKEYSIZE_Y	16

static int objtonum[13]={0,7,8,9,4,5,6,1,2,3,-1,-1,-1};
#define	STRING_LEN_FREQ		3
#define	STRING_LEN_TIME		1