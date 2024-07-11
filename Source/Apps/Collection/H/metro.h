/******************************************************************************
*	[Titel]		Metronom				      	      *
*	[Model] 	PocketViewer PV-250X, PV-450X und PV-750X.	      *
*	[Version]	Ver 1.00	 				      *
*	[Datei Name]	metro.h					      	      *
*	Copyright (C)   2000 Nikolaus Thiessen.				      *
*	E-Mail		nikolaust@aol.com				      *
******************************************************************************/
#include <define.h>
#include <libc.h>
#include <l_libc.h>

void HauptMenu(void);
void Direkt(TCHSTS *tsts);
void Info(void);
void Metronom(void);
void Piep(void);
void Init(void);
void Zzeit(void);
void Zahlzeit(void);
void TextZZeit(int x, int y);
void Position(TCHSTS *tsts, int x, int y);
void Hilfe(void);
bool Pruefen(int eingabe);
static byte SetZahlzeit(void);

void PollEvent(TCHSTS far* tsts, byte event_mask);

static word far TabelleBPM[] = 
{
	   30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90, 95,  100, 105,
	  110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205,
	  210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 270, 275, 280, 285, 290, 295, 300, 305,
};

static word far WerteTabelle[] =
{
          505, 426, 370, 325, 286, 256, 230, 210, 195, 180, 164, 154, 146, 134, 124, 114,
	  107, 100,  94,  87,  86,  84,  79,  72,  66,  65,  60,  58,  54,  52,  50,  49,  45,  41,  39,  38,
	   38,  36,  32,  32,  31,  26,  25,  25,  24,  24,  22,  20,  19,  19,  18,  18,  17,  16,  16,  16,
};