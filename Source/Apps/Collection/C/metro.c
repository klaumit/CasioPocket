/******************************************************************************
*	[Titel]		Metronom  				      	      *
*	[Model] 	PocketViewer PV-250X, PV-450X und PV-750X.	      *
*	[Version]	Ver 1.00	 				      *
*	[Datei Name]	metro.c					      	      *
*	Copyright (C)   2000 Nikolaus Thiessen.				      *
*	E-Mail		nikolaust@aol.com				      *
******************************************************************************/
#include <define.h>
#include <l_define.h>
#include <libc.h>
#include <l_libc.h>
#include  "stdrom.h"

#include <metro.h>
#include <bilder.h>
#include <hmenu.h>

#define EVENT_TCH       1
#define EVENT_CRADLE    4
#define EVENT_BLD1      8

static byte far punkt[] =
{
	GSIZE(1, 2),
	0x80,
	0x80,
};

byte Buffer[17][33];		
byte far HilfeD[17][33] =
{
	"   *********************         ",
	"   * HILFE ZU METRONOM  *        ",
	"   *********************         ",
	"                                 ",
	" Zur Tempoeingabe in bpm         ",
	" (beats per minute) benutzen     ",
	" Sie bitte die oben angeordnete  ",
	" Tabelle oder die Tastatur unten.",
	" Gestartet wird mit START oder   ",
	" der Enter-Taste.                ",
	" Mit einem Klick auf das         ",
	" Lautsprechersymbol wird der     ",
	" Ton aus- bzw. eingeschaltet.    ",
	" Weitere Infos in der readme.txt.",
	"     Viel Gl´ck beim ´ben.       ",
	"                                 ",
	"  Bitte den Bildschirm ber´hren! ",
};        

bool ton;
bool ZZ;
int UZahl;
int BPM;	/* Beats per Minute. Global */
int zg = 1;
int zge;
/***************************************************************
*	Main Modul					       *
***************************************************************/
void metromain()
{
	Init();
	HauptMenu();

LibJumpMenu();
}

/**************************************************************
*		Initillisirung vor dem Start		      *
**************************************************************/
void Init()
{
	TCHSTS tsts;
	bool ende = TRUE;
	int i, j;

	LibClrDisp();
	LibPutGraph(0, 0, metronom);		/* Begrühsungs Bild */
	LibPutDisp();

	for(j = 0; j < 119; j++)
	{
		LibPutProStr(IB_PFONT2,27,151,"by Nikolaus Thiessen",j);
		LibPutDisp();
	for(i = 0; i < 10; i++){}
	}

	LibTchInit();						/* Warte schleife */
	i = 0;
	do
	{
		PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE);
                               	if (tsts.obj != 0x5000 ||tsts.x != -1 || tsts.y != -1){ende = FALSE;break;}
				if (tsts.y > 160) break;
	if(i == 400) ende = FALSE;
	i++;
	}
	while(ende);

	LibClrDisp();
	LibPutGraph(0, 0, Hauptbild);
	LibPutGraph(9, 95, Ein);
	LibIconPrint(&D00);
	LibPutProStr(IB_CG57FONT, 7, 115, "Aus ", 50);
	LibPutDisp();
	ton = TRUE;
	ZZ = FALSE;
}

/**************************************************************
*		Das Info Fanster			      *
**************************************************************/
void Info()
{
	TCHSTS tsts;
	bool ende = TRUE;
	int x, y, x1, y1;

	x = 5;
	y = 27;
	x1 = 150;
	y1 = 106;

	LibOpenWindowS(ON, x, y, x1, y1);
	LibPutProStr(IB_PFONT3,x + 10,y + 5,    "Metronom",160);
	LibPutProStr(IB_PFONT1,x + 80,y + 8,    "Version 1.00",160);
	LibPutProStr(IB_CG57FONT,x + 10,y + 25, "Copyright (c) 2000 by",160);
	LibPutProStr(IB_CG57FONT,x + 13,y + 38, "Nikolaus Thiessen.",160);
	LibPutProStr(IB_CG57FONT,x + 13,y + 50, "eMail:",160);
	LibPutProStr(IB_CG57FONT,x + 13,y + 63, "nikolaust@aol.com",160);
	LibPutProStr(IB_PFONT1,x + 10,y + 79,   "Besten Dank an:",160);
	LibPutProStr(IB_PFONT2,x + 10,y + 88,   "Helmut Scholz!!",160);

	LibPutDisp();

	LibTchInit();						/* Warte schleife */
	do
	{
		 PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE);
                               	if (tsts.obj != 0x5000 ||tsts.x != -1 || tsts.y != -1){ende = FALSE;break;}
				if (tsts.y > 160) break;
	}
	while(ende);
	LibCloseWindow();
	LibTchInit();
}

/***************************************************************************
*		Das Hilfe Fenster					   *
***************************************************************************/
void Hilfe()
{
	TCHSTS tsts;
	bool ende = TRUE;
	int x, y, x1, y1;
	int i;
	x = 0;
	y = 0;
	x1 = 160;
	y1 = 160;

	LibOpenWindowS(ON, x, y, x1, y1);

	for(i = 0;i < 17;i++)				/* Der Hilfetext wird ausgegeben */
	{
		far_strcpy(Buffer[i],HilfeD[i]);
		LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 9,Buffer[i],155);
		LibPutDisp();
	}
	
	LibTchInit();
	do					/* Warte schleife */
	{
		 PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE);
                               	if (tsts.obj != 0x5000 ||tsts.x != -1 || tsts.y != -1){ende = FALSE;break;}
				if (tsts.y > 160) break;
	}
	while(ende);
	LibCloseWindow();
	LibTchInit();
}

/***************************************************************************
*	Metronom Simulation						   *
***************************************************************************/
void Metronom()
{
	TCHSTS tsts;
	bool e = FALSE;
	int z;
	int x,y; 

	LibTchInit();
	LibGdsBoxMesh(21, 138, 138, 160);
	LibGdsBoxMesh(29, 28, 132, 48);
	LibPutProStr(IB_CG57FONT, 45, 140 ,"Zum Beenden,", 160);
	LibPutProStr(IB_CG57FONT, 23, 150 ,"Bildschirm ber´hren", 160);
	LibPutDisp();

	while(e != TRUE)
	{
		x = 31; y = 30;
		for(z = 0; z <= UZahl; z++) 
		{
		 LibPutGraph(x+z, y, punkt);
		 LibPutDispBox(x+z, y, 1, 2);
		 if(z+x == 130){y += 3; x -= 100;}

		 PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE);
                               	if (tsts.obj != 0x5000 ||tsts.x != -1 || tsts.y != -1){e = TRUE; zg = 1; break;}
				if (tsts.y > 160) break;
		}
	LibClrBox(31, 30, 100, 17);
	Piep();
	}
zg = 1;
}

/**************************************************************************
*		Ton Ausgabe						  *
**************************************************************************/
void Piep()
{
	int i;
	byte s[2];

	if(ZZ)
	{
		LibPutGraph(71, 8, fenster);
		LibPutDispBox(71, 8, 17, 15);
		if(ton)
		{
			if(zg == 1)
			{	
				LibNumToStr(s, zg, 2);
				
				LibPutProStr(IB_PFONT3, 73, 10 , s, 15);
				LibPutDisp();
				zg++;	
				LibBuzzerOn(IB_BEEP0_SET);
				for(i = 0; i < 5; i++){/* */} 
				LibPutMsgDlg2("            \r            \r            ");
				LibBuzzerOff();
			return;
			}

		LibNumToStr(s, zg, 2);

		LibPutProStr(IB_PFONT3, 73, 10 , s, 15);
		LibPutDisp();
		zg++;
		if(zg > zge){zg = 1;}
		LibBuzzerOn(IB_BEEP0_SET);
		LibPutMsgDlg2("************************\r************************");
		LibBuzzerOff();
		return;
		}
		if(!ton)
		{
			if(zg == 1)
			{
				LibNumToStr(s, zg,2);

				LibPutProStr(IB_PFONT3, 73, 10 , s, 15);
				LibPutDisp();
				zg++;

				LibPutMsgDlg2("            \r            \r            ");
			return;
			}

		LibNumToStr(s, zg,2);

		LibPutProStr(IB_PFONT3, 73, 10 , s, 15);
		LibPutDisp();
		zg++;
		if(zg > zge){zg = 1;}

			LibPutMsgDlg2("************************\r************************");
		return;
		}
	return;
	}
	if(ton)
	{
		LibBuzzerOn(IB_BEEP0_SET);
		LibPutMsgDlg2("************************\r************************");
		LibBuzzerOff();
	return;
	}
LibPutMsgDlg2("************************\r************************");
}

/**************************************************************************
*	Funktion von  Wittawat Yamwong zur Touch abfrage		  *
**************************************************************************/
void PollEvent(TCHSTS far* tsts, byte event_mask)
{
	union REGS reg;

	reg.x.ax = 0x0200 | event_mask;
	reg.x.di = FP_OFF(tsts);
	reg.x.es   = FP_SEG(tsts);
	int86(0x50,&reg,&reg);
}
