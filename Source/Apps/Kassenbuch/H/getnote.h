
#define MSG_LEN 25         /* Maximum messages number */
char LangMsgOut[MSG_LEN];  /* Global variable for return messages in function GetMsg*/


char *GetMsg(int id)
{
   char *msg[MSG_LEN]=
     {

     /* Msg00 */   "Ausgaben",
     /* Msg01 */   "OK",
     /* Msg02 */   "Cancel",
     /* Msg03 */   "Neueingabe",
     /* Msg04 */   "Statistik",
     /* Msg05 */   "Spei",
     /* Msg06 */   "L\x0b3s.",
     /* Msg07 */   "Esc",
     /* Msg08 */   "",
     /* Msg09 */   "",
     /* Msg10 */   "Bitte warten",
     /* Msg11 */   "Save Ok",
     /* Msg12 */   "Load Ok",
     /* Msg13 */   "Loading...",
     /* Msg14 */   "Saving...",
     /* Msg15 */   "Anfangswert :",
     /* Msg16 */   "Verfügbar :",
     /* Msg17 */   "Gesamte Ausgaben :",
     /* Msg18 */   "Letzte Ausgabe :",
     /* Msg19 */   "Aktuelle Ausgabe :",
     /* Msg20 */   "KASSENBUCH",
     /* Msg21 */   "Version 1.00",
     /* Msg22 */   "Programmiert von",
     /* Msg23 */   "Klaus Bott",
     /* Msg24 */   "klausbott@t-online.de",
     };

   strcpy(LangMsgOut,msg[id]);
   return(LangMsgOut);
}

char *GetPosten(int id)
{
   char PostenOut[20];

   char *posten[5]= {
     /* 'Essen' */             POSTEN_1,
     /* 'Kantine' */           POSTEN_2,
     /* 'Hobby' */             POSTEN_3,
     /* 'Tanken' */            POSTEN_4,
     /* 'Verschiedenes' */     POSTEN_5
     };

   strcpy(PostenOut,posten[id]);
   return(PostenOut);
}

