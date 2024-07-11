/******************************************************************************
*  Program Title	Escoba
*  Module			Language functions
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2001
*	Rev     		1.0
*	MMSoft 2000 - Freeware
******************************************************************************/
#include <escoba.h>
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

#define WINMSG_L 16
#define WINMSG_T 30
#define WINMSG_R WINMSG_L+128
#define WINMSG_B WINMSG_T+50

TCHTBL far TchWinMsg[]=    
 { /* card 1 area */
 	WINMSG_L,WINMSG_T,WINMSG_R,WINMSG_B,ACT_BREAK_IN|ACT_500MSEC,OBJ_WINMSG,0x0000,
   TCH_END}; 

char LangMsgOut[80];  /* Global variable for return messages in function GetMsg*/

/******************************************************************************
*	[Title]		GetMsg
*	[Arguments]	int id: Identifier for message
*	[ReturnVAL] a pointer to string LangMsgOut containing the message
*  [Description]
*  Selects a message from an array.
*  The main index is the language
*	The second index is the message number.
*	You can rewrite the messages to your own game
*  Please, be carefully with commas (,) CR's (\r) and SPACES in each message
*  Put attention to length messages too.
******************************************************************************/
char *GetMsg(int id)
{  int lang;

/* Language messages */
char *msg[5][30]={
/* Deutch   */ 
/* Msg00 */  {"Sie sind dran...",
/* Msg01 */   "Rechter spieler",
/* Msg02 */   "Linker spieler",
/* Msg03 */   "Verteilen karten",
/* Msg04 */   "\rESCOBA!!",
/* Msg05 */   "\rGame Over",
/* Msg06 */   "\rSee scores",
/* Msg07 */   "  IHRE KARTEN  ",
/* Msg08 */   " KART.RECHTER SP.",
/* Msg09 */   " KART. LINKEN SP.",
/* Msg10 */	  " Karten:  0  0  0",
/* Msg11 */	  "   Oros:  0  0  0",
/* Msg12 */	  "    7's:  0  0  0",
/* Msg13 */	  " 7 Oros:         ",
/* Msg14 */	  "Escobas:  0  0  0",
/* Msg15 */	  "  TOTAL:  0  0  0",
/* Msg16 */   "\rSpielbeginn...\rLevel  ",
/* Msg17 */   "Um eine karte zu\rspielen, m\xB4ssen sie\rdiese erst\rvorher ausw\x0B0hlen",
/* Msg18 */   "keine freieren\rpl\x0B0tze mehr",
/* Msg19 */	  "W\x0B0hlen sie eine\rkarte und klicken\rSie in den Bereich\rum sie zu spielen",	
/* Msg20 */	  "LEVEL",	
/* Msg21 */	  "Lev.1",	
/* Msg22 */	  "Lev.2",	
/* Msg23 */	  "Lev.3",	
/* Msg24 */	  "SPIELER",	
/* Msg25 */	  "2 Spi.",	
/* Msg26 */	  "3 Spi.",
/* Msg27 */	  "DEMO MODUS",
/* Msg28 */	  "DEMO AKTIV\rBer\x0B4hren sie den\rbildschirm um fortz.\rESC f\x0B4r einstellungen",
/* Msg29 */	  "TOTAL"
             },   
/* English  */ 
/* Msg00 */  {"Your turn...",
/* Msg01 */   "Right player",
/* Msg02 */   "Left player",
/* Msg03 */   "Deal...",
/* Msg04 */   "\rESCOBA!!",
/* Msg05 */   "\rGame Over",
/* Msg06 */   "\rSee scores",
/* Msg07 */   "   YOUR CARDS   ",
/* Msg08 */   " RIGHT PL.CARDS ",
/* Msg09 */   " LEFT  PL.CARDS ",
/* Msg10 */	  "  Cards:  0  0  0",
/* Msg11 */	  "   Oros:  0  0  0",
/* Msg12 */	  "    7's:  0  0  0",
/* Msg13 */	  " 7 Oros:         ",
/* Msg14 */	  "Escobas:  0  0  0",
/* Msg15 */	  "  TOTAL:  0  0  0",
/* Msg16 */   "\rStarting game...\rLevel  ",
/* Msg17 */   "\rChoose a card\rbefore throwing it",
/* Msg18 */   "No more free positions",
/* Msg19 */	  "Select a card\rand touch area\rto throw it",	
/* Msg20 */	  "LEVEL",	
/* Msg21 */	  "Lev.1",	
/* Msg22 */	  "Lev.2",	
/* Msg23 */	  "Lev.3",	
/* Msg24 */	  "PLAYERS",	
/* Msg25 */	  "2 Ply.",	
/* Msg26 */	  "3 Ply.",
/* Msg27 */	  "DEMO MODE",
/* Msg28 */	  "DEMO ACTIVE\r\rTouch to continue\rESC to settings",
/* Msg29 */	  "TOTAL"
             },   
/* Español  */   
/* Msg00 */  {"Su turno...",
/* Msg01 */   "Jugad. derecha",
/* Msg02 */   "Jugad. izquierda",
/* Msg03 */   "Reparto...",
/* Msg04 */   "\r\x08F\x08FESCOBA!!",
/* Msg05 */   "\rFin del juego",
/* Msg06 */   "\rVer puntuaci\x093n",
/* Msg07 */   " 	SUS CARTAS   ",
/* Msg08 */   " CARTAS JUG.DCHA.",
/* Msg09 */   " CARTAS JUG.IZDA.",
/* Msg10 */	  " Cartas:  0  0  0",
/* Msg11 */	  "   Oros:  0  0  0",
/* Msg12 */	  " Sietes:  0  0  0",
/* Msg13 */	  " 7 oros:         ",
/* Msg14 */	  "Escobas:  0  0  0",
/* Msg15 */	  "  TOTAL:  0  0  0",
/* Msg16 */   "\rIniciando juego...\rNivel  ",
/* Msg17 */   "Toque una carta\rantes de tirarla\rsobre la mesa.",
/* Msg18 */   "No hay mas\respacio libre",
/* Msg19 */	  "Seleccione carta\ry su\x091ltela sobre\rla zona marcada",	
/* Msg20 */	  "NIVEL",	
/* Msg21 */	  "Niv.1",	
/* Msg22 */	  "Niv.2",	
/* Msg23 */	  "Niv.3",	
/* Msg24 */	  "JUGAD.",	
/* Msg25 */	  "2 Jug.",	
/* Msg26 */	  "3 Jug.",
/* Msg27 */	  "MODO DEMO",
/* Msg28 */	  "DEMO ACTIVADA\r\rToque para seguir\rESC para ajustes",
/* Msg29 */	  "TOTAL"
	            },   
/* Francais */
/* Msg00 */  {"V\x09Dtre tour...",
/* Msg01 */   "Joueur droit ",
/* Msg02 */   "Joueur gauche",	
/* Msg03 */   "R\x091partition...",
/* Msg04 */   "\rESCOBA!!",
/* Msg05 */   "\rFinale du jeu",
/* Msg06 */   "\rVoyez ponctuation",
/* Msg07 */   "   V\x08DTRE CARTES  ",
/* Msg08 */   " CARTES JOU.DRO. ",
/* Msg09 */   " CARTES JOU.GAU. ",
/* Msg10 */	  " Cartes:  0  0  0",
/* Msg11 */	  "   Oros:  0  0  0",
/* Msg12 */	  "    7's:  0  0  0",
/* Msg13 */	  " 7 Oros:         ",
/* Msg14 */	  "Escobas:  0  0  0",
/* Msg15 */	  "  TOTAL:  0  0  0",
/* Msg16 */   "\rInitialisation du jeu\rNiveau  ",
/* Msg17 */   "Choisissez une carte\rpremi\x091re de la jeter",
/* Msg18 */   "Plus de positions\rlibres",
/* Msg19 */	  "Choisissez carte \ret touchez dans\rla zone marqu\x091e",	
/* Msg20 */	  "NIVEAU",	
/* Msg21 */	  "Niv.1",	
/* Msg22 */	  "Niv.2",	
/* Msg23 */	  "Niv.3",	
/* Msg24 */	  "JOUEURS",	
/* Msg25 */	  "2 Jou.",	
/* Msg26 */	  "3 Jou.",
/* Msg27 */	  "MODO DEMO",
/* Msg28 */	  "DEMO ACTIV\x081\r\rTouchez pour continuer\rESC pour arrange",
/* Msg29 */	  "TOTAL"
				},
   /* Italian */ 
/* Msg00 */  {"Vostra girata",
/* Msg01 */   "Gioc. destra ",
/* Msg02 */   "Gioc.sinistra",
/* Msg03 */   "Deal...",
/* Msg04 */   "\rESCOBA!!",
/* Msg05 */   "\rFine di gioco",
/* Msg06 */   "\rPunteggio",
/* Msg07 */   "      CARTE     ",
/* Msg08 */   " CARTE GIOC.DES.",
/* Msg09 */   " CARTE GIOC.SIN.",
/* Msg10 */	  "  Carte:  0  0  0",
/* Msg11 */	  "   Oros:  0  0  0",
/* Msg12 */	  "    7's:  0  0  0",
/* Msg13 */	  " 7 Oros:         ",
/* Msg14 */	  "Escobas:  0  0  0",
/* Msg15 */	  " TOTALE:  0  0  0",
/* Msg16 */   "\rInizio dei gioco\rLivello  ",
/* Msg17 */   "Toccare la carta\rprima da liberare",
/* Msg18 */   "Nessuna posizione\rlibera",
/* Msg19 */	  "Toccare l'area\rdopo di carte\rper liberare",	
/* Msg20 */	  "LIVELLO",	
/* Msg21 */	  "Liv.1",	
/* Msg22 */	  "Liv.2",	
/* Msg23 */	  "Liv.3",	
/* Msg24 */	  "GIOCATORE",	
/* Msg25 */	  "2 Gio.",	
/* Msg26 */	  "3 Gio.",
/* Msg27 */	  "MODO DEMO",
/* Msg28 */	  "DEMO ACTIVATA\r\rToccare per continuare\rESC per opzionni",
/* Msg29 */	  "TOTALE"
       		 }
  };
   lang=LibGetLang();
   strcpy(LangMsgOut,msg[lang][id]);
   return(LangMsgOut);
}


/******************************************************************************
*	[Title]		WinMsgBox
*	[Arguments]	msg	: Message string
*					icon	: Window icon
*					wsec	: seconds to wait before close window (0=waits for a touch)
*
*	[ReturnVAL] 
*  [Description]
*  Show a window messaging with 3 lines centered
* 	The newline character is \r
******************************************************************************/
void WinMsgBox(char *msg,int icon,int wait)
{ 	TCHSTS tsts;
	int sec;		
	int nlines;
	char *p1;
	char msgcpy[80];

	LibOpenWindow(WINMSG_L,WINMSG_T,128,50);
	/* Window */
	LibPutFarData(WINMSG_L,WINMSG_T,107);
	/* Icon classification */	
	switch (icon) {
		case ICON_OK:					/* Ok		      	*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,46);
			break;
		case ICON_BADTZ:				/* NO		      	*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,10);
			break;
		case ICON_BIKKURI:			/* i		      	*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,11);
			break;
		case ICON_COFFEE:				/* Coffee cup  	*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,24);
			break;
		case ICON_TRASH:				/* Trash box		*/	
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,28);
			break;
		case ICON_SIGN:				/* Hand				*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,55);
			break;
		case ICON_SYNC:				/* Communicating 	*/
			LibPutFarData(WINMSG_L+5,WINMSG_T+10,25);
			break;
		default:
			break;
		}		
	strcpy(msgcpy,msg);		
	nlines=0;
	while (strlen(msgcpy)!=0) { /* Display message  */
		p1=strchr(msgcpy,'\r');
		if (p1==NULL){
			LibPutProStr(IB_PFONT1,WINMSG_L+74-LibGetProStrSize(IB_PFONT1,msgcpy)/2,WINMSG_T+6+10*nlines,msgcpy,100);
			break;
			}
		else {	
			*p1='\0';	
			p1++;		
			LibPutProStr(IB_PFONT1,WINMSG_L+74-LibGetProStrSize(IB_PFONT1,msgcpy)/2,WINMSG_T+6+10*nlines,msgcpy,100);
			strcpy(msgcpy,p1);
			nlines++;
			}
		}		
	LibPutDispBox(WINMSG_L,WINMSG_T,128,50);	
	LibTchStackPush(TchWinMsg);	/* Window area */
	LibTchInit();
	if (wait!=0)
		sec=wait*10;
	else
		sec=5;	
	do {		
 		LibTchWait(&tsts);
		if (tsts.act==ACT_500MSEC) {
			if  (wait!=0)sec-=5;	
			}
		else
		switch (tsts.obj) {
   		case OBJ_HIC_ESC:   	/* ESC key*/
   			if (GameDemo==TRUE){
   				LibTchStackPop();					/* Restore touch 	*/
   				Settings();
  					LibTchStackPush(TchWinMsg);	/* Window area 	*/
	         	sec=0;
  					}
   			else	
      			LibJumpMenu();
         	break;
       	case OBJ_HIC_MBAR:  	/* MENU Key*/       		
  				LibTchStackPop();					/* Restore touch 	*/
        		GmPdwnFnc();
				LibTchStackPush(TchWinMsg);	/* Window area 	*/
         	sec=0;
         	break;
         case OBJ_WINMSG:		/* Touch in window area */
         	sec=0;
         	break;	
       	default:
       		break;
       	}	  
	} while (sec>0);
   LibTchStackPop();		/* restore tch area */
	LibCloseWindow();
	LibPutDispBox(WINMSG_L,WINMSG_T,128,50);	
}	