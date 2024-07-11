/******************************************************************************
*	[Title]		PVTetris 
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#ifndef EXP_DEF
#define EXP_DEF

#include    "define.h"

#define OBJ_IC_Larrow    0x8089
#define OBJ_IC_Rarrow    0x8090
#define OBJ_IC_arrow     0x8091
#define OBJ_IC_START		 0x8092	
#define OBJ_IC_dw		    0x8093
#define OBJ_IC_LEVEL     0x8094
#define OBJ_IC_GAME      0x8095
#define OBJ_IC_PAUSE		 0x8096
#define ROW_COUNT        20    /* 19 row   */
#define COLUM_COUNT		 11    /* 10 colum */

extern bool boxdsp[ROW_COUNT][COLUM_COUNT]; /* Block Array */

/************************/
/*   TCHTBL List        */
/************************/
extern T_ICON far TiconLevel;
extern TCHTBL far control[];
extern TCHTBL far tchstart[];
extern TCHTBL far TchHeaderIcon[];
/* Shape List           */
extern byte far bigleft[];
extern byte far bigright[];
extern byte far bigdown[];
extern byte far bigarrow[];
extern byte far sblock[];
extern byte far block[];
extern byte far la1[];
extern byte far la2[];
extern byte far la3[];
extern byte far la4[];
extern byte far lb1[];
extern byte far lb2[];
extern byte far lb3[];
extern byte far lb4[];
extern byte far t1[];
extern byte far t2[];
extern byte far t3[];
extern byte far t4[];
extern byte far sa1[];
extern byte far sa2[];
extern byte far sb1[];
extern byte far sb2[];
extern byte far line1[];
extern byte far line2[];
extern byte far o[];
extern byte far sc1[];
extern byte far sc2[];
extern byte far sd1[];
extern byte far sd2[];
extern byte far u1[];
extern byte far u2[];
extern byte far u3[];
extern byte far u4[];
extern byte far blank[];
extern byte far blankblock[];
extern byte far pause1[];
/* Icon List            */
extern byte far startgm[];
extern byte far Level[];
extern byte far game1[];

/************************/
/*    Help Function     */
/************************/
static int GetSeed( void );
void PlyGm();
void DspBk();
void Draw(int shape,int pos,int i,int j,bool clear);
int  makeshape();
int  makepos(int);
bool test(int ,int,int,int);
void test2(int shape,int pos,int i,int j);
void shownew();
void setblock(int shape,int pos,int i,int j);
bool findright(int shape,int pos,int i,int j,int *right);
void drawblank(int n);
static byte LevelSet();
static byte LevelSetgame();
void setgame();
#endif
