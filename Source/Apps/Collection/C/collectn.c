/******************************************************************************
*	
*	[Collection]	Menu and Link Program using foreign public Sources
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.01
*
*	Copyright (C) 2001 Thomas Gaertner - thomas.gaertner@t-online.de
******************************************************************************/#include <stdrom.h>
#include "libc.h"
#include "collectn.h" 

/* own control defines */
#define TCH_BATTSTAT		0xc008
#define TCH_BATTLE		0xc009
#define TCH_FRANKTR		0xc002
#define TCH_CHARPR		0xc003
#define TCH_PPLOT		0xc004
#define TCH_METRO		0xc005
#define TCH_MATHE		0xc006
#define TCH_PERIODIC		0xc007
#define TCH_SETUP		0xc001

void 	initMainTchStack();
void 	initSetupTchStack();


/* button defintions */ /*absolute Koordinaten !!!!*/
TCHTBL far TTTchMainScreenButtons[]={
	2,2,29,12,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_SETUP,
	0x0000,

	1,1,49,39,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_BATTSTAT,
	0x0000,

	50,1,99,39,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_BATTLE,
	0x0000,

	100,1,149,39,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_MATHE,
	0x0000,

	1,40,49,79,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_CHARPR,
	0x0000,

	50,40,99,79,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_PPLOT,
	0x0000,

	100,40,149,79,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_FRANKTR,
	0x0000,

	1,80,49,119,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_METRO,
	0x0000,

	50,80,99,119,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_PERIODIC,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};


void main()

	{
	TCHSTS	tsts;
	bool	loop=TRUE;

  
	initMainTchStack();

	LibInitDisp();
	LibPutDisp();

	
	LibPutGraph (5,1,battstatIcon);
	LibPutGraph (55,1,battleshipIcon);
	LibPutGraph (105,1,matheIcon);
	
	LibPutGraph (5,40,charprintIcon);
	LibPutGraph (55,40,pplotIcon);
	LibPutGraph (105,40,frankaIcon);
	
	LibPutGraph (5,80,metronomIcon);
	/*LibPutGraph (55,80,periodicIcon);
	LibPutGraph (105,80,collectionIcon);
	
	LibPutGraph (5,120,collectionIcon);
	LibPutGraph (55,120,collectionIcon);
	LibPutGraph (105,120,collectionIcon);*/
	
	
	LibStringDsp("BattStat",5,30,150,IB_PFONT1);
	LibStringDsp("BattleShip",55,30,150,IB_PFONT1);
	LibStringDsp("Mathe",105,30,150,IB_PFONT1);
	
	LibStringDsp("CharPring",5,70,150,IB_PFONT1);
	LibStringDsp("PPlot",55,70,150,IB_PFONT1);
	LibStringDsp("Franka",105,70,150,IB_PFONT1);
	
	LibStringDsp("Metronom",5,110,150,IB_PFONT1);
	/*LibStringDsp("Periodic",55,110,150,IB_PFONT1);
	LibStringDsp("Unused",105,110,150,IB_PFONT1);
	
	LibStringDsp("Unused",5,150,150,IB_PFONT1);
	LibStringDsp("Unused",55,150,150,IB_PFONT1);
	LibStringDsp("Unused",105,150,150,IB_PFONT1);*/
	
	LibPutDisp();
	
	

	while(loop)
		{
		LibTchWait(&tsts);		/* wait for action */
		switch(tsts.obj){

			case TCH_BATTSTAT:			
				BattStat();
				break;

			case TCH_BATTLE:
				battlemain();
				break;
			
			case TCH_FRANKTR:
				frankTR();
				break;

			case TCH_CHARPR:
				charprint();
				break;

			case TCH_PPLOT:
				pplot();
				break;

			case TCH_METRO:
				metromain();
				break;

			case TCH_MATHE:
				mathe();
				break;
			
			/*case TCH_PERIODIC:
				periodicmain();*/
				break;


			default:
				break;
		}
	}
	LibTchRelease();
	LibJumpMenu();
}


void initMainTchStack(){
	LibTchStackClr();			/* Clear stack contents */
	LibTchStackPush(NULL);		
	LibTchStackPush(TchHardIcon);		/* Hardware Icons */
	LibTchStackPush(TTTchMainScreenButtons);	/* Setup Button */
	LibTchInit();	
}			/* init touch display */
