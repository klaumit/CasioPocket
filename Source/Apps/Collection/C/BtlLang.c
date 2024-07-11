/******************************************************************************
*   Program Title	Battle Ship
*   Module			Language functions
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		12/2000 
*	Rev     		1.3
*	MMSoft 2000 - Freeware
*	Notes:	
*		Thanks to Karin Biederlack for her translation to German language
*                 Francesco Aldrovandi for his help to Italian messages
*		The rest of translations was using the Internet
******************************************************************************/
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_libc.h>
#include <battle.h>

/* Define Touch area for YES-NO buttons */
TCHTBL far TchYesNo[]={
/* YES Button */	
  50,85,79,100,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_IC_HYES,
  0x0000,	
/* NO Button */  
  90,85,119,100,
  ACT_MAKE|ACT_MOVE_IN|ACT_MOVE_OUT|ACT_BREAK_IN,
  OBJ_IC_HNO,
  0x0000,	
/* End */  
  0,0,0,0,
  ACT_NONE,
  OBJ_END,
  0x0000
};  

/* Define YES-NO buttons */
T_ICON far IcnYes=
  { &TchYesNo[0],NULL,NULL,0x02 };
T_ICON far IcnNo=
  { &TchYesNo[1],NULL,NULL,0x02 };
  

char LangMsgOut[MSG_LEN];  /* Global variable for return messages in function GetMsg*/


/******************************************************************************
*	[Title]			MsgShotInfo
*	[Arguments]		int shotresult: Last shot information
*					int col: Column's shot
*					int row: Row's shot
*	[ReturnVAL]	
*   [Description] 	
*		Shows a message with the last shot result at the bottom area
******************************************************************************/
void MsgShotInfo(int shotresult,int col,int row)
{ int xp=28;
  int yp=144;
  char texto[30];
  
  LibPutFarData(xp,yp,14);
  sprintf(texto,"%c,%c: %s",col+'A',row+'1',GetMsg(shotresult+4));
  LibPutProStr(IB_CG57FONT,xp+5,yp+4,texto,100);
  LibPutDisp();
  LibWait(IB_1SWAIT);
}


/******************************************************************************
*	[Title]			MsgPlayerTurn
*	[Arguments]		byte pl: Player 
*	[ReturnVAL]	
*   [Description] 	
*		Shows the player name for next turn
******************************************************************************/
void MsgPlayerTurn(byte pl)
{ int xp=28;
  int yp=144;
  char texto[30];

  LibPutFarData(xp,yp,14);
  strcpy(texto,GetMsg(8));
  strcat(texto,GetMsg(pl));
  LibPutProStr(IB_CG57FONT,xp+5,yp+4,texto,100);
  LibPutDisp();
}	


/******************************************************************************
*	[Title]			MsgScore
*	[Arguments]		byte pl: Player  
*	[ReturnVAL]	
*   [Description] 	
*		Shows the player score.
******************************************************************************/
void MsgScore(byte pl)
{ int xp=Board[pl].xs;
  int yp=120;
  char texto[30];
  
  sprintf(texto,"%s:%2d",GetMsg(2),Board[pl].shots);
  LibPutProStr(IB_CG57FONT,xp,yp,texto,78);
  sprintf(texto,"%s:%2d",GetMsg(3),Board[pl].hits);
  LibPutProStr(IB_CG57FONT,xp,yp+9,texto,78);
  LibPutDisp();  
}	


/******************************************************************************
*	[Title]			MsgEndGame
*	[Arguments]		byte win: the winner. CASIO or Human player
*	[ReturnVAL] 	TRUE  if YES play again
*          			FALSE if NO play more
*   [Description]	
*		Shows the end-game screen asking for new game
*       If win was Human shows Congratulations!...
*		If win was CASIO shows Bad Luck!...
******************************************************************************/
byte MsgEndGame(byte win)
{ int xp=28;
  int yp=144;
  int i;
  TCHSTS tsts;  

  LibPutFarData(xp,yp,14); 
  LibPutProStr(IB_CG57FONT,xp+5,yp+4,GetMsg(10),100);

  ShowBoardAll(PL_CASIO,TRUE);  /* end game */
  LibTchStackClr();             /* Clear all stack area */
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon); /* Register hard icons    */
  LibTchStackPush(TchAllDsp);  /* Register all display   */
  LibTchWait(&tsts); /* Wait for a touch key */

  xp=17;
  yp=30;  
  LibClrDisp(); /* Clear screen */
  LibPutFarData(xp,yp,109);
  xp+=10;
  yp+=10;

/* Shows end-game screen messages */  
  if (win==PL_HUMAN)
    for (i=0;i<3;i++) {
      LibPutProStr(IB_PFONT1,xp,yp,GetMsg(11+i),120);
      yp+=10;
      }
  else 
    for (i=0;i<3;i++) {
      LibPutProStr(IB_PFONT1,xp,yp,GetMsg(14+i),120);
      yp+=10;
      }

  LibPutProStr(IB_PFONT1,xp,yp,GetMsg(17),120);   /* Play again? */
  LibPutProStr(IB_PFONT2,5,150,"Battle Ship (c)2000 MMSoft",155);
  LibPutFarData(TchYesNo[0].x1,TchYesNo[0].y1,113);
  LibPutFarData(TchYesNo[1].x1,TchYesNo[1].y1,113);
  LibPutMessageCenter(43,TchYesNo[0].x1,TchYesNo[0].x2,TchYesNo[0].y1+3,IB_PFONT1);
  LibPutMessageCenter(44,TchYesNo[1].x1,TchYesNo[1].x2,TchYesNo[1].y1+3,IB_PFONT1);
  LibPutDisp();  

  LibTchStackPush(TchYesNo);    /* Register buttons Yes-No      */  
  LibTchInit();
  
  while (TRUE) { 	
    LibTchWait(&tsts); 
    switch (tsts.obj) {
      case OBJ_IC_HYES:
        if (LibIconClick(&IcnYes,&tsts)==TRUE)
          return (TRUE);
        break;  
      case OBJ_IC_HNO:
        if (LibIconClick(&IcnNo,&tsts)==TRUE)
          return (FALSE);
        break;  
      default:
        break;          
      }  /* end switch */
    }  /* end while */
}	


/******************************************************************************
*	[Title]		GetMsg
*	[Arguments]	int id: Identifier for message
*	[ReturnVAL] a pointer to string LangMsgOut containing the message
*   [Description]
*     	Selects a message from an array.
*   	The main index is the language
*		The second index is the message number.
*		You can rewrite the messages to your own game
*       Please, be carefully in commas (,) CR's (\r) and SPACES in each message
*       Put attention to length messages too.
******************************************************************************/
char *GetMsg(int id)
{  int lang;

/* Language messages */
char *msg[5][MSG_LEN]={
/* Deutch   */ 
/* Msg00 */  {"CASIO PV",
/* Msg01 */   "SPIELER ",
/* Msg02 */   "  SHOTS",
/* Msg03 */   "   HITS",
/* Msg04 */   "WIEDERHOLUNG",
/* Msg05 */   "WASSER",
/* Msg06 */   "TREFFER",
/* Msg07 */   "GTESUNKEN",
/* Msg08 */   "Spielt:",
/* Msg09 */   " AUSWAHL",
/* Msg10 */   "Game Over",
/* Msg11 */   "GL\x0a4CKWUNSCH!",
/* Msg12 */   "Sie haben gewonnen",
/* Msg13 */   "",
/* Msg14 */   "    PECH GEHABT!",
/* Msg15 */   "  Vielleicht beim n\0b0chten mal",
/* Msg16 */   "",
/* Msg17 */   "Noch ein spiel?",
/* Msg18 */   "\r"
			  "Vor positioneren, \r"
			  "w\x0b0hlen sie ein schiff.\r\r",
/* Msg19 */   "\r"
			  "Positioneren sie alle schiffe\r"
			  "vorspielbeginn\r\r",
/* Msg20 */   "\r"
			  "Position nich\r"
			  "ausreichend.\r\r"
			  "W\x0b0hlen sie eine \r"
			  "andere position.\r\r",
/* Msg21 */   "\r"
			  "Ber\x0b4hren der schiffe\r"
			  "nicht m\x0b3glich.\r\r",
/* Msg22 */   "\r"
			  "Plazieren sie das schiff\r"
			  "bevor sie ein anderes.\r"
			  "ausw\x0b0hlen.\r\r"
             },   
/* English  */ 
/* Msg00 */  {"CASIO PV",
/* Msg01 */   " PLAYER ",
/* Msg02 */   "  SHOTS",
/* Msg03 */   "   HITS",
/* Msg04 */   "REPEATED",
/* Msg05 */   "WATER",
/* Msg06 */   "TOUCHED",
/* Msg07 */   "SUNK",
/* Msg08 */   "Turn of ",
/* Msg09 */   "SELECTION",
/* Msg10 */   "Game Over",
/* Msg11 */   " CONGRATULATIONS!",
/* Msg12 */   "    You win",
/* Msg13 */   "",
/* Msg14 */   "    BAD LUCK!",
/* Msg15 */   "  Check your aim",
/* Msg16 */   "Best luck next time.",
/* Msg17 */   "   Play Again?",
/* Msg18 */   "\r"
			  "Please, select\r"
			  "a ship before\r"
			  "touch the board\r\r",
/* Msg19 */   "\r"
			  "Please, place\r"
			  "all ships before\r"
			  "play the game\r\r",
/* Msg20 */   "\r"
			  "Ship doesn't fit\r"
			  "in the position.\r\r"			  
			  "Please, select\r"
			  "another cell.\r\r", 
/* Msg21 */   "\r"
			  "Ship can't touch\r"
	          "other ships\r\r"
			  "Please, select\r"
			  "another cell.\r\r", 
/* Msg22 */   "\r"
			  "Please, release\r"
			  "the ship before\r"
			  "select other.\r\r"
             },   
/* Español  */   
/* Msg00 */  {"CASIO PV",
/* Msg01 */   "JUGADOR",
/* Msg02 */   "DISPAROS",
/* Msg03 */   "IMPACTOS",
/* Msg04 */   "REPETIDO",
/* Msg05 */   "AGUA",
/* Msg06 */   "TOCADO",
/* Msg07 */   "HUNDIDO",
/* Msg08 */   "Turno:",
/* Msg09 */   "SELECCION",
/* Msg10 */   " Fin del juego",
/* Msg11 */   "   \x08fFELICIDADES!",
/* Msg12 */   "  Vd gana el juego",
/* Msg13 */   "",
/* Msg14 */   "    \x08fMALA SUERTE!",
/* Msg15 */   "  Mejore su punter\x092a",
/* Msg16 */   "    la pr\x093xima vez.",
/* Msg17 */   "\x09fQuiere jugar de nuevo?",
/* Msg18 */   "\r"
			  "Por favor, seleccione\r"
			  "un barco antes de\r"
			  "colocarlo en el tablero\r\r",
/* Msg19 */   "\r"
			  "Por favor, coloque\r"
			  "todos los barcos\r"
			  "antes de pasar al juego\r\r",
/* Msg20 */   "\r"
			  "El barco no cabe en la\r"
			  "posici\x093n apuntada.\r\r"			  
			  "Por favor, seleccione\r"
			  "otra posici\x093n.\r\r", 
/* Msg21 */   "\r"
			  "Un barco no puede tocar\r"
	          "las casillas de otro\r\r"
			  "Por favor, seleccione\r"
			  "otra posici\x093n.\r\r", 
/* Msg22 */   "\r"
			  "Por favor,\r"
			  "libere la selecci\x093n\r"
			  "actual antes de\r"
			  "seleccionar otro barco.\r\r"
             },   
/* Francais */
/* Msg00 */  {"CASIO PV",
/* Msg01 */   "JOUEUR ",
/* Msg02 */   "  COUPS",
/* Msg03 */   "IMPACTS",
/* Msg04 */   "REPETITION",
/* Msg05 */   "EAU",
/* Msg06 */   "FRAPPE",
/* Msg07 */   "ENFOCE",
/* Msg08 */   "Tour de ",
/* Msg09 */   "SELECTION",
/* Msg10 */   "Fin du jeu",
/* Msg11 */   " FELICITATIONS!",
/* Msg12 */   "  Vous gagnez le jeu",
/* Msg13 */   "",
/* Msg14 */   " PAS DE LA CHANCE!",
/* Msg15 */   " Meilleur pointage",
/* Msg16 */   " la proxieme fois.",
/* Msg17 */   "     Autre jeu?",
/* Msg18 */   "\r"
			  "Selecter an bateau\r"
			  "pour le mettre.\r"
			  "S'il vous pla\x09ct\r\r",
/* Msg19 */   "\r"
			  "Met touts les\r"
			  "bateaux pour\r"
			  "commenzer le jeu.\r"
			  "S'il vous pla\x09ct\r\r",
/* Msg20 */   "\r"
			  "Le bateau ne tiens pas\r" 
			  "a le position.\r\r"			  
			  "Selecter autre cellule.\r\r", 
/* Msg21 */   "\r"
			  "Le bateau ne peut pas\r"
	          "toucher autres\r"
	          "bateaux.\r\r"
			  "Selecter autre bateau\r"
			  "S'il vous pla\x09ct\r\r",
/* Msg22 */   "\r"
			  "Lacher le bateau\r"
			  "pres de selecter\r"
			  "autre.\r"
			  "S'il vous pla\x09ct\r\r"
             },
   /* Italiano */ 
/* Msg00 */  {"CASIO PV",
/* Msg01 */   "GIOCATORE",
/* Msg02 */   "   COLPI",
/* Msg03 */   "  CENTRI",
/* Msg04 */   "RIPETIZIONE",
/* Msg05 */   "ACQUA",
/* Msg06 */   "COLPITO",
/* Msg07 */   "AFFONDATO",
/* Msg08 */   "Turno: ",
/* Msg09 */   "SELEZIONE ",
/* Msg10 */   "Fine gioco",
/* Msg11 */   "CONGRATULAZIONI!",
/* Msg12 */   "  hai vinto",
/* Msg13 */   "",
/* Msg14 */   "  HAI PERSO!",
/* Msg15 */   " sar\x095 per la prossima",
/* Msg16 */   " volta.",
/* Msg17 */   "   nuova partita?",
/* Msg18 */   "\r"
			  "Selezionare un battello\r"
			  "prima di toccare\r"
			  "lo schermo.\r\r",
/* Msg19 */   "\r"
			  "Collocare tutti\r"
			  "i battelli prima\r"
			  "di cominciare la\r"
			  "partita.\r\r",
/* Msg20 */   "\r"
			  "Non c'\x096 abbastanza\r"
			  "spazio per posizionare\r"
			  "il battello.\r\r"
			  "Selezionare un'altra\r"
			  "casella.\r\r",
/* Msg21 */   "\r"
			  "Due battelli non\r"
			  "possono occupare \r"
			  "caselle adiacenti.\r\r"
			  "Selezionare un'altra\r"
			  "casella.\r\r", 
/* Msg22 */   "\r"
			  "Deselezionare il battello\r"
			  "prima di selezionarne\r"
			  "un'altro.\r\r"
       		 }
  };
                       
   lang=LibGetLang();
   strcpy(LangMsgOut,msg[lang][id]);
   return(LangMsgOut);
}
   

