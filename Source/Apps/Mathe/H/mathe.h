#include "stdrom.h"
#include "string.h"
#include "define.h"
#include "libc.h"
#include "l_libc.h"

/* Prototypen */
int Titel();
int Auswertung();
int rechne();
int ZahlenEingabe();
int ClearScreen();
int Rahmen();
int DialogBox();
int DisplaySetup();
int DispKeyboard();
int onscreen();
int GetSeed();
int GetTime();
int randomg(int max);
int Aufgabe();
void VarReset();

/* Globale Variablen */
int versuche=0;		/* speicher f�r anzahl der versuche */
int operation;		/* 0 Addition, 1 Subtraktion */
int zufallzahl=0;	/* speicher f�r die Zufallzahl */
int eingabe=0;		/* speicher f�r gerade eingegebene zahl */
int exit=0;		/* dient dazu um return im touchscreen zu erkennen */
int h=0;			/* speicher f�r hunderter */
int z=0;		/* speicher f�r zehner */
int e=0;		/* speicher f�r einer */
int a=0;		/* speicher f�r den z�hler zur erkennung der stelle h,z,e */
int tipp=0;		/* speicher f�r den eingegebenen wert */
int ergebnis=0;
int sec=0;		/* speicher um zu erkennen ob die eingegebene zahl weniger als 3 stellen hat */
int gesaufgaben=4;	/* Anzahl der Aufgaben pro Runde */
int l;			/* Laufvariable */

int startzeit;
int geszeit;
int level=0;
int punkte=0;
bool richtig=FALSE;
bool verkehrt=FALSE;
bool gameover=FALSE;
	
	
        
#define		OBJ_1		   	0x9000
#define		OBJ_2			0x9001
#define		OBJ_3	 	  	0x9002
#define		OBJ_4			0x9003
#define		OBJ_5			0x9004
#define		OBJ_6			0x9005
#define		OBJ_7			0x9006
#define		OBJ_8			0x9007
#define		OBJ_9			0x9008
#define		OBJ_0			0x9009
#define		OBJ_links		0x900A
#define		OBJ_rechts		0x900B
#define		OBJ_del			0x900C
#define		OBJ_next		0x900D

/* keys*/

TCHTBL far Tastiera[] = {
	50, 80, 70, 95,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_7,
	0x0000,

	71, 80, 90, 95,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_8,
	0x0000,

	91, 80, 110, 95,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_9,
	0x0000,
	
	50, 96, 70, 110,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_4,
	0x0000,
	
	71, 96, 90, 110,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_5,
	0x0000,
	
 	91, 96, 110, 110,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_6,
	0x0000,

 	50, 111, 70, 125,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_1,
	0x0000,

 	71, 111, 90, 125,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_2,
	0x0000,
	
	91, 111, 110, 125,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_3,
	0x0000,
	
 	50, 126, 70, 140,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_0,
	0x0000,

 	71, 126, 90, 140,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_links,
	0x0000,

 	91, 126, 110, 140,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_rechts,
	0x0000,
	
	50, 141, 80, 155,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_del,
	0x0000,
	
 	50, 141, 110, 155,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_next,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};