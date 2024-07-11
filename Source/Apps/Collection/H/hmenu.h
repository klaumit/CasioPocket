/******************************************************************************
*	[Titel]		Metronom				      	      *
*	[Model] 	PocketViewer PV-250X, PV-450X und PV-750X.	      *
*	[Version]	Ver 1.00	 				      *
*	[Datei Name]	hmenu.h					      	      *
*	Copyright (C)   2000 Nikolaus Thiessen.				      *
*	E-Mail		nikolaust@aol.com				      *
******************************************************************************/
#include <stdrom.h>
#include <define.h>
#include <l_define.h>
#include <libc.h>
#include <l_libc.h>

#define TAST_1			0x8000
#define TAST_2			0x8001
#define TAST_3			0x8002
#define TAST_4			0x8003
#define TAST_5			0x8004
#define TAST_DEL		0x8005
#define TAST_6			0x8006
#define TAST_7			0x8007
#define TAST_8			0x8008
#define TAST_9			0x8009
#define TAST_0			0x8010
#define TAST_OK			0x8011
	
#define TAST_START		0x8012
#define TAST_HILFE		0x8013
#define	TAST_INFO		0x8014

#define	TAST_DIREKT		0x8015
#define	TAST_MODI		0x8016
#define TAST_ZZEIT		0x8017



#define xsize  37		/* Anfangs koordinaten der Tastatur */
#define ysize 127

static TCHTBL far TastFeldT[] =		/* Tasten der 10-ner Tastatur */
{
	 xsize + 0,  ysize + 0,  xsize + 13, ysize + 15, ACT_ICON, TAST_1,    0x00,
	  xsize + 14, ysize + 0,  xsize + 27, ysize + 15, ACT_ICON, TAST_2,    0x00,
	   xsize + 28, ysize + 0,  xsize + 41, ysize + 15, ACT_ICON, TAST_3,    0x00,
	    xsize + 42, ysize + 0,  xsize + 55, ysize + 15, ACT_ICON, TAST_4,    0x00,
	     xsize + 56, ysize + 0,  xsize + 69, ysize + 15, ACT_ICON, TAST_5,    0x00,
	      xsize + 70, ysize + 0,  xsize + 83, ysize + 15, ACT_ICON, TAST_DEL,  0x00,
	       xsize + 0,  ysize + 15, xsize + 13, ysize + 30, ACT_ICON, TAST_6,    0x00,
	       	xsize + 14, ysize + 15, xsize + 27, ysize + 30, ACT_ICON, TAST_7,    0x00,
	         xsize + 28, ysize + 15, xsize + 41, ysize + 30, ACT_ICON, TAST_8,    0x00,
	          xsize + 42, ysize + 15, xsize + 55, ysize + 30, ACT_ICON, TAST_9,    0x00,
	           xsize + 56, ysize + 15, xsize + 69, ysize + 30, ACT_ICON, TAST_0,    0x00,
	            xsize + 70, ysize + 15, xsize + 83, ysize + 30, ACT_ICON, TAST_OK,   0x00,
};
static T_ICON far T00 = {&TastFeldT[0],  NULL, NULL, 0x00}; /* 1 */
static T_ICON far T01 = {&TastFeldT[1],  NULL, NULL, 0x00}; /* 2 */
static T_ICON far T02 = {&TastFeldT[2],  NULL, NULL, 0x00}; /* 3 */
static T_ICON far T03 = {&TastFeldT[3],  NULL, NULL, 0x00}; /* 4 */
static T_ICON far T04 = {&TastFeldT[4],  NULL, NULL, 0x00}; /* 5 */
static T_ICON far T05 = {&TastFeldT[5],  NULL, NULL, 0x00}; /* DEL */
static T_ICON far T06 = {&TastFeldT[6],  NULL, NULL, 0x00}; /* 6 */
static T_ICON far T07 = {&TastFeldT[7],  NULL, NULL, 0x00}; /* 7 */
static T_ICON far T08 = {&TastFeldT[8],  NULL, NULL, 0x00}; /* 8 */
static T_ICON far T09 = {&TastFeldT[9],  NULL, NULL, 0x00}; /* 9 */
static T_ICON far T10 = {&TastFeldT[10], NULL, NULL, 0x00}; /* 0 */
static T_ICON far T11 = {&TastFeldT[11], NULL, NULL, 0x00}; /* OK */

static TCHTBL far TastFeldS[] =		/* Tasten: Start,Info und Hilfe */
{
	116,  75, 155, 106, ACT_ICON, TAST_START,  0x00,
	   2, 136,  31, 149, ACT_ICON, TAST_HILFE,  0x00,
	  127, 136, 156, 149, ACT_ICON, TAST_INFO,   0x00,
};
static T_ICON far S00 = {&TastFeldS[0], NULL, NULL, 0x00}; /* Start */
static T_ICON far S01 = {&TastFeldS[1], NULL, NULL, 0x00}; /* Hilfe */
static T_ICON far S02 = {&TastFeldS[2], NULL, NULL, 0x00}; /* Info */

static TCHTBL far TastFeldD[] =		/* Direkte wahl 40 - 270 ,Ton ,Zählzeit*/
{
	1, 12, 159,  71, ACT_ICON, TAST_DIREKT,  0x00,
	 4, 74,  25, 107, ACT_ICON, TAST_MODI,    0x00,
	  2,112,  33, 124, ACT_ICON, TAST_ZZEIT,   0x00,
};
static T_ICON far D00 = {&TastFeldD[2], zzeittaste, NULL, 0x00}; /* Zählzeit */

static word far BPMdata[3][8] = /* Daten für die direkte wahl über die Tabelle*/
{
	{ 30,  40,  50,  60,  70,  80,  90, 100},
	{110, 120, 130, 140, 150, 160, 170, 180},
	{190, 200, 210, 220, 230, 240, 250, 260},
};

static word far XData[2][6] =
{
	{1, 2, 3,  4,  5,  6},
	{7, 8, 9, 10, 11, 12},
};

