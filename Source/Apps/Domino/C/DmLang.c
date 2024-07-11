/******************************************************************************
*   Program Title	Domino
*   Module			Language functions to translate messages
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		01/2001
*	Rev     		1.2
*	MMSoft 2000 - Freeware
*	Notes:	
*		Thanks to:
*   Karin Biederlack for her translation to German language
*   Francesco Aldrovandi for his help to Italian messages and test the program
*   Valerie Verrier for her translation to French language
* 
******************************************************************************/
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_libc.h>
#include <domino.h>

#define MSG_LEN 15         /* Maximum messages number */
char LangMsgOut[MSG_LEN];  /* Global variable for return messages in function GetMsg*/

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
char *msg[5][MSG_LEN]={/* Language messages */
/* Deutch   */ 
/* Msg00 */  {"START!",
/* Msg01 */   "GESCHLOSSEN!",
/* Msg02 */   "GEWONNEN!",
/* Msg03 */   "PASSE!",
/* Msg04 */   "NICHT G\x0a4LTIG!",
/* Msg05 */   "Spei",
/* Msg06 */   "L\x0b3s.",
/* Msg07 */   "Esc",
/* Msg08 */   "ENDE",
/* Msg09 */   "KLASSIFIKATION",
/* Msg10 */   "NAME EINGEBEN (ESC exit)",
/* Msg11 */   "W\x0a0HLE BILD",
/* Msg12 */   "NOCH EIN SPIEL?",
/* Msg13 */   "Gespeicherte partie",
/* Msg14 */   "weiterspielen?"

             },   
/* English  */ 
/* Msg00 */  {"START!",
/* Msg01 */   "CLOSED!",
/* Msg02 */   "I WIN!",
/* Msg03 */   "PASS!",
/* Msg04 */   "INVALID!",
/* Msg05 */   "Aj.",
/* Msg06 */   "Clr",
/* Msg07 */   "Esc",
/* Msg08 */   "GAME OVER",
/* Msg09 */   "SCORES TABLE",
/* Msg10 */   "EDIT NAME (ESC to exit)",
/* Msg11 */   "   SELECT A PICTURE    ",
/* Msg12 */   "Play again?",
/* Msg13 */   "Continue last",
/* Msg14 */   "saved game?"
             },   
/* Español  */   
/* Msg00 */  {"\x08fEMPIEZO!",
/* Msg01 */   "\x08fCERRADO!",
/* Msg02 */   "\x08fHE GANADO!",
/* Msg03 */   "\x08fPASO!",
/* Msg04 */   "\x08fNO VALE!",
/* Msg05 */   "Aj.",
/* Msg06 */   "Clr",
/* Msg07 */   "Esc",
/* Msg08 */   "FIN DEL JUEGO",
/* Msg09 */   "CLASIFICACION",
/* Msg10 */   "EDITAR NOMBRE (ESC salir)",
/* Msg11 */   "SELECCIONAR FIGURA",
/* Msg12 */   "\x09fQuiere jugar de nuevo?",
/* Msg13 */   "\x09fQuiere continuar",
/* Msg14 */   "el \x094ltimo juego guardado?"
             },   
/* Francais */
/* Msg00 */  {"D\x081BUT!",
/* Msg01 */   "FERM\x081!",
/* Msg02 */   "J'AI GAGN\x081!",
/* Msg03 */   "JE PASSE!",
/* Msg04 */   "NON VALABLE!",
/* Msg05 */   "Sauv",
/* Msg06 */   "Clr",
/* Msg07 */   "Esc",
/* Msg08 */   "FIN",
/* Msg09 */   "CLASSIFICATION",
/* Msg10 */   "CHANGER LE NOM (ESC pour sortir)",
/* Msg11 */   "CHOISIR UN DESSIN",
/* Msg12 */   "Rejouer?",
/* Msg13 */   "Voulez-vous continuer",
/* Msg14 */   "le dernier jeu sauv\x091?"
             },
   /* Italiano */ 
/* Msg00 */  {"SI COMINCIA!",
/* Msg01 */   "PARTITA CHIUSA!",
/* Msg02 */   "HO VINTO!",
/* Msg03 */   "PASSO!",
/* Msg04 */   "NON POSSO!",
/* Msg05 */   "Mem",
/* Msg06 */   "Ann",
/* Msg07 */   "Esc",
/* Msg08 */   "FINE PARTITA",
/* Msg09 */   "TABELLA DEI PUNTEGGI",
/* Msg10 */   "MODIFICA NOME (ESC per uscire)",
/* Msg11 */   "SELEZIONA UN'IMMAGINE",
/* Msg12 */   "Vuoi giocare ancora?",
/* Msg13 */   "",
/* Msg14 */   "Continuare la partita salvata?"
       		 }
  };
                       
   lang=LibGetLang();
   strcpy(LangMsgOut,msg[lang][id]);
   return(LangMsgOut);
}
   

