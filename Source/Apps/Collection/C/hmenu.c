/******************************************************************************
*	[Titel]		Metronom				      	      *
*	[Model] 	PocketViewer PV-250X, PV-450X und PV-750X.	      *
*	[Version]	Ver 1.00	 				      *
*	[Datei Name]	hmenu.c					      	      *
*	Copyright (C)   2000 Nikolaus Thiessen.				      *
*	E-Mail		nikolaust@aol.com				      *
******************************************************************************/
#include <define.h>
#include <l_define.h>
#include <libc.h>
#include <l_libc.h>

#include <metro.h>
#include <bilder.h>
#include <hmenu.h>

/* Folgende variablen sind in der metro.c deklariert */
extern bool ton;
extern bool ZZ;
extern int BPM;
extern int zge;
extern int UZahl;

TCHSTS tsts;

/****************************************************************
*		Das Hauptmenu					*
****************************************************************/
void HauptMenu()
{
	byte ende = FALSE;
	byte z[2]="";
	int m = 10;					/* Mulpiplikator */
	int Zahl = 0;					/* Eingabe Variable */
	float zw;

	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( TastFeldD );
	LibTchStackPush( TastFeldS );
	LibTchStackPush( TastFeldT );
	LibTchInit();	

while(ende != TRUE)
	{
	LibTchWait(&tsts);
		switch(tsts.obj)
		{
		case TAST_1:
			if(LibBlockIconClick( &T00, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 1;}
			break;
		case TAST_2:
			if(LibBlockIconClick( &T01, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 2;}
			break;
		case TAST_3:
			if(LibBlockIconClick( &T02, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 3;}
			break;
		case TAST_4:
			if(LibBlockIconClick( &T03, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 4;}
			break;
		case TAST_5:
			if(LibBlockIconClick( &T04, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 5;}
			break;
		case TAST_6:
			if(LibBlockIconClick( &T06, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 6;}
			break;
		case TAST_7:
			if(LibBlockIconClick( &T07, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 7;}
			break;
		case TAST_8:
			if(LibBlockIconClick( &T08, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 8;}
			break;
		case TAST_9:
			if(LibBlockIconClick( &T09, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * m + 9;}
			break;
		case TAST_0:
			if(LibBlockIconClick( &T10, &tsts , B_ICON_CENTER ) == TRUE){Zahl = Zahl * 10;}
			break;
		case TAST_DEL:
			if(LibBlockIconClick( &T05, &tsts , B_ICON_CENTER ) == TRUE){Zahl = 0;}
			break;
		case TAST_OK:
			if(LibBlockIconClick( &T11, &tsts , B_ICON_CENTER ) == TRUE)
			{
				if( Pruefen(Zahl) )
				{
					LibOpenWindowS(OFF, 0, 0, 160, 160);
					Metronom();
					LibCloseWindow();
					Zahl = BPM;
				break;
				}
			}
			break;
		case TAST_START:
			if(LibBlockIconClick( &S00, &tsts , B_ICON_CENTER ) == TRUE)
			{
				if( Pruefen(Zahl) )
				{
					LibOpenWindowS(OFF, 0, 0, 160, 160);
					Metronom();
					LibCloseWindow();
					Zahl = BPM;
				break;
				}
			}
			break;
		case TAST_HILFE:
			if(LibBlockIconClick( &S01, &tsts , B_ICON_CENTER ) == TRUE){Hilfe();}
			break;
		case TAST_INFO:
			if(LibBlockIconClick( &S02, &tsts , B_ICON_CENTER ) == TRUE){Info();}
			break;
		case TAST_DIREKT:
				Direkt(&tsts);
				Zahl = BPM;
			break;
		case TAST_MODI:
			if(ton){ton = FALSE; LibPutGraph(9, 95, Aus);}
			else{ ton = TRUE; LibPutGraph(9, 95, Ein);}

				LibPutDisp();
				LibTchInit();
			break;
		case TAST_ZZEIT:
			if(LibBlockIconClick( &D00, &tsts , B_ICON_CENTER ) == TRUE){Zahlzeit();}
			break;
		}
		
		if( Zahl > 999 ){zw = (Zahl / 10.0 - BPM) * 10;Zahl = zw;}
			BPM = Zahl;

			LibNumToStr(z, Zahl,3);		/* BPM - Ausgabe */
			LibPutProStr(IB_PFONT2, 68, 94 , z, 20);
			LibPutDisp();
	}
}

/****************************************************************
*	Die Wahl über die Tabelle				*
****************************************************************/
void Direkt(TCHSTS *tsts)
{
	int i, w;
	int sx, sy;
	int xa, ya, xe, ye;

	xa = 1;
	ya = 12;
	xe = 158;
	ye = 70;

	for(sx = -1,i = 0; i < 8; i++)
	{
		w = xa + 20 * i;
		if( (w <= tsts->x) && (tsts->x < w + 20) )
		{
			sx = i;
			break;
		}
	}
	if(sx == -1)  return ;

	for(sy = -1, i = 0; i < 3; i++)
	{
		w = ya + 20 * i;
		if( (w <= tsts->y) && (tsts->y < w + 20) )
		{
			sy = i;
			break;
		}
	}
	if(sy == -1)  return ;

	BPM = BPMdata[sy][sx];
}

/********************************************************************
*	Zahlzeit						    *
********************************************************************/
void Zzeit()
{
	byte a[4];
	byte print[5];
	byte auswahl;

	auswahl = SetZahlzeit();	/* aufruf des Menus */

	if(auswahl == 0)
	{
		ZZ = FALSE;
		LibPutProStr(IB_CG57FONT, 7, 115, "Aus ", 50);
		LibPutDisp();
		return;
	}
		else
		{
			zge = (int)auswahl + 1;
		}
		
	ZZ = TRUE;
	LibNumToStr(a, zge,2);
	if(a[0] == '0')
	{
		print[0] = 0x20;
		print[1] = a[1];
		print[2] = 0x20;
		print[3] = 0x1f;
	}
		else
		{
			print[0] = a[0];
			print[1] = a[1];
			print[2] = 0x20;
			print[3] = 0x1f;
		}
LibPutProStr(IB_CG57FONT, 6, 115 , print, 50);
LibPutDisp();
}

/**************************************************************************
*	Ermitlung von Zahlzeit						  *
**************************************************************************/
void Zahlzeit()
{
	int x, y;
	x = 2;
	y = 2;

	LibOpenWindowS(ON, x, y, 156, 60);
	TextZZeit(x, y);
	Zzeit();
	LibCloseWindow();
}

/************************************************************************
* 		Das Tacktart Menu					*
************************************************************************/
static byte SetZahlzeit(void)
{
	byte    rtv[10+1] = {0,1,2,3,4,5,6,7,8,9,0xff};
	byte    cmnt[10*(5+1)],np;
	SLW2_ST win_prm;
	byte    sel_ret;
	byte    t_tbl[10][5+1];
	int     i,k,len;

	/* Menu data */
	strcpy(t_tbl[0]," Aus ");
	strcpy(t_tbl[1],"  2  ");
	strcpy(t_tbl[2],"  3  ");
	strcpy(t_tbl[3],"  4  ");
	strcpy(t_tbl[4],"  5  ");
	strcpy(t_tbl[5],"  6  ");
	strcpy(t_tbl[6],"  7  ");
	strcpy(t_tbl[7],"  8  ");
	strcpy(t_tbl[8],"  9  ");
	strcpy(t_tbl[9]," 10  ");
	/* Standart position ( Aus ) */
	np = 0;

	for(i=k=0;i<10;i++,k++){
	    len = strlen(t_tbl[i]);
	    memcpy(&cmnt[k],t_tbl[i],len);
	    k += len;
	    if(i<1) cmnt[k] = 0xfe;
	    else    cmnt[k] = 0x00;    
	}

	/* Auswahl Fenster */
	win_prm.x    =  33;
	win_prm.y    =  62;
	win_prm.xs   =  35;
	win_prm.ity  =   9;
	win_prm.np   = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv  = rtv;
	win_prm.t_xs =   4;

	sel_ret = LibSelWindowExt(&win_prm);
	if(sel_ret > 10) sel_ret = 0;
	return(sel_ret);
}

/******************************************************
*	Beschreibung von Zählzait		      *
******************************************************/
void TextZZeit(int x, int y) 
{
	int i = 0;
	y = y - 2;

	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," Hier besteht die M³glichkeit,",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," die Taktart bzw. die Z°hlzeiten",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," zu w°hlen. F´r einen 4/4 Takt",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," beisp., ist die 4 einzugeben.",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," Beim Starten z°hlt d. Metronom",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," bis 4, wobei die 1. Z°hlzeit ",156);
	i++;
	LibPutProStr(IB_PFONT1,2 + x,5 +  y + i * 8," zus°tzlich betont wird.",156);

LibPutDisp();
}

/********************************************************
*	Prüfen der eingabe (BPM)		        *
********************************************************/
bool Pruefen(int eingabe)
{
	int x, y, t;
	int z;

	if((eingabe < 30) || (eingabe > 305))
	{
		LibPutMsgDlg("Die Tempoeingabe liegt \r\rau­erhalb des erlaubten\r\r Bereiches! (30 - 305) ");
		return 0;    
	}

	for(t = -1,x = 0; x < 56; x++)
	{
		if(TabelleBPM[x] == BPM){UZahl = WerteTabelle[x]; return 1;}
	}
		if(t == -1)
		{
			for(x = 0; x < 55; x++)
			{
				if((TabelleBPM[x] < eingabe ) && ( eingabe < TabelleBPM[x + 1]))
				{
					for(y = 1; y < 5; y++)
					{
						if(TabelleBPM[x] + y == eingabe)
						{
							z = (WerteTabelle[x] - WerteTabelle[x + 1])/5;
							z = z * y;
							UZahl = WerteTabelle[x] + z;
							return 1;
						}
					}
				}
			}
		}
return 0;
}
