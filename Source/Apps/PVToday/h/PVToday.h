/******************************************************************************
*	[Title]     PVToday
*	[Model] 	PocketViewer
*	[Version]	Ver 2.2
*
*	Copyright (C) 2004 Jean-Michel GIRARD . All rights reserved.
*       http://pocketcasio.pocketviewer.de
******************************************************************************/
#define SZ_TITLE          "PV Today"
#define VERSION           "2.2"

#define xLANG_GER
#define xLANG_ENG
#define xLANG_FRE
#define xLANG_CZE
#define xLANG_SPA
#define xLANG_RUS

#define I_YINFOSIZE 24
#define I_YDEBINFO  13
#define I_XTIMELU   20
#define I_YTIMELU   1

#define SZ_ABOUT1         VERSION
#define SZ_ABOUT3         "http://pocketcasio.pocketviewer.de"
#define SZ_ABOUT4         "Copyright \x0DC 2004, PocketCasio"
#define SZ_ABOUT5         ""
#define SZ_ABOUT6         ""

/* Set fixed lang */
#define LANG_ENG true

/******************************************************************************/
#ifdef LANG_GER
#define SZ_OWNER          "PV Today-Besitzer"
#define SZ_PREFTITLE      "Einstellungen"
#define SZ_PERSTITLE      "Pers\x0B3nliche Informationen"
#define SZ_ABOUTTITLE     "\x0A4ber ..."
#define SZ_ERRORTITLE     "Fehler!"
#define SZ_OKBUTTON       "OK"
#define SZ_CANCELBUTTON   "Abbruch"
#define SZ_ABOUT2         "Alle Rechte liegen bei"

#define SZ_SHOWOWNER      "Besitzer : %s"
#define SZ_SHOWTEL        "Telefon # : %s"
#define SZ_SHOWDATE       "%s %s %s, %s" /* NameTag NameMonat Tag, Jahr */
#define SZ_SHOWNOTIME     "Nicht das aktuelle Datum"
#define SZ_SHOWTASK       "%d Aktive Aufgabe(n)"
#define SZ_SHOWUNREADMAIL "%d ungelesene Nachricht(en)"
#define SZ_SHOWUNSENDMAIL "%d unversendete Nachricht(en)"
#define SZ_SHOWREMIND     "%d Aktive remind(s)"

#define SZ_PROWNER        "Besitzer :"
#define SZ_PRTEL          "Telefonnummer :"
#define SZ_PRCHKDATE      "Datum"
#define SZ_PRCHKOWNER     "Benutzer-Info"
#define SZ_PRCHKCALEND    "Termine"
#define SZ_PRCHKTASK      "Aufgaben"
#define SZ_PRCHKMAIL      "Mail"
#define SZ_PRCHKTIME      "Zeit 12Std-Format"
#define SZ_PRCHKTSKACT    "Termine werden �berpr�ft"
#define SZ_PRCHKSYSTEM    "System-Info"
#define SZ_PRMAILOPT1     "PV-750 Mail"
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "Keine Verabredungen"
#define SZ_NOUNREADMAIL   "Keine ungelesenen Nachrichten"
#define SZ_NOUNSENDMAIL   "Keine unversendeten Nachrichten"
#define SZ_NOTASK         "Keine Aufgaben"
#define SZ_NOTASKTIME     "Keine Zeit f�r diese Aufgabe definiert"
#define SZ_NODISPLAY      "Nichts anzuzeigen !"
#define SZ_NOREMIND       "Keine m\x091mento"

#define SZ_MAILOPENERR1   "Mail nur f�r PV-750 !"
#define SZ_MAILOPENERR2   "Keine Mail-Unterst�tzung !"
#define SZ_MAILOPENERR3   "PVMail nicht gefunden !"

#define SZ_SHAREDERROR1   "SaveRecord-Fehler in SHARED !"
#define SZ_SHAREDERROR2   "ReadRecord-Fehler in SHARED !"

#endif

/******************************************************************************/
#ifdef LANG_ENG
#define SZ_OWNER          "PV Today Owner"
#define SZ_PREFTITLE      "Preferences"
#define SZ_PERSTITLE      "Personal Infos"
#define SZ_ABOUTTITLE     "About ..."
#define SZ_ERRORTITLE     "Error !"
#define SZ_OKBUTTON       "OK"
#define SZ_CANCELBUTTON   "Cancel"
#define SZ_ABOUT2         "All rights reserved"

#define SZ_SHOWOWNER      "Owner : %s"
#define SZ_SHOWTEL        "Phone # : %s"
#define SZ_SHOWDATE       "%s %s %s, %s"                  /* NameDay NameMonth Day, Year */
#define SZ_SHOWNOTIME     "Not the actual day"
#define SZ_SHOWTASK       "%d Active task(s)"
#define SZ_SHOWUNREADMAIL "%d unread message(s)"
#define SZ_SHOWUNSENDMAIL "%d unsent message(s)"
#define SZ_SHOWREMIND     "%d Active remind(s)"

#define SZ_PROWNER        "Owner :"
#define SZ_PRTEL          "Phone number :"
#define SZ_PRCHKDATE      "Date"
#define SZ_PRCHKOWNER     "Owner Info"
#define SZ_PRCHKCALEND    "Schedule"
#define SZ_PRCHKTASK      "Task"
#define SZ_PRCHKMAIL      "Mail"
#define SZ_PRCHKTIME      "Time 12Hrs Format"
#define SZ_PRCHKTSKACT    "Check schedule in progress"
#define SZ_PRCHKSYSTEM    "System Info"
#define SZ_PRMAILOPT1     "PV-750 Mail"
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "No upcomming appointment"
#define SZ_NOUNREADMAIL   "No unread messages"
#define SZ_NOUNSENDMAIL   "No unsent messages"
#define SZ_NOTASK         "No tasks"
#define SZ_NOTASKTIME     "No time define for this task"
#define SZ_NODISPLAY      "Nothing to display !"
#define SZ_NOREMIND       "No remind"

#define SZ_MAILOPENERR1   "Mail only for PV-750 !"
#define SZ_MAILOPENERR2   "No mail support !"
#define SZ_MAILOPENERR3   "PVMail not found !"

#define SZ_SHAREDERROR1   "SaveRecord failure in SHARED !"
#define SZ_SHAREDERROR2   "ReadRecord failure in SHARED !"

#endif

/******************************************************************************/
#ifdef LANG_FRE
#define SZ_OWNER          "Utilisateur PV Today"
#define SZ_PREFTITLE      "Pr\x091f\x091rences"
#define SZ_PERSTITLE      "Infos Personnelles"
#define SZ_ABOUTTITLE     "A propos ..."
#define SZ_ERRORTITLE     "Erreur !"
#define SZ_OKBUTTON       "OK"
#define SZ_CANCELBUTTON   "Annuler"
#define SZ_ABOUT2         "Tous droits r\x091serv\x91s"

#define SZ_SHOWOWNER      "Propr. : %s"
#define SZ_SHOWTEL        "T\x91l. : %s"
#define SZ_SHOWDATE       "%s %s %s, %s"                  /* NameDay NameMonth Day, Year */
#define SZ_SHOWNOTIME     "Pas le jour actuel"
#define SZ_SHOWTASK       "%d t\x09Ache(s) active(s)"
#define SZ_SHOWUNREADMAIL "%d message(s) non lu(s)"
#define SZ_SHOWUNSENDMAIL "%d message(s) non envoy\x91(s)"
#define SZ_SHOWREMIND     "%d m\x091mento(s) actif(s)"


#define SZ_PROWNER        "Propri\x91taire :"
#define SZ_PRTEL          "T\x91l\x91phone :"
#define SZ_PRCHKDATE      "Date"
#define SZ_PRCHKOWNER     "Info utilisateur"
#define SZ_PRCHKCALEND    "Emploi du Temps"
#define SZ_PRCHKTASK      "T\x09Aches"
#define SZ_PRCHKMAIL      "Mail"
#define SZ_PRCHKTIME      "Format Heure sur 12"
#define SZ_PRCHKTSKACT    "Regarder l'emp. du tps en cours"
#define SZ_PRCHKSYSTEM    "Info syst\x91me"
#define SZ_PRMAILOPT1     "PV-750 Mail"
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "Aucune r\x091union \x095 venir"
#define SZ_NOUNREADMAIL   "Aucun message non lu"
#define SZ_NOUNSENDMAIL   "Aucun message non envoy\x091"
#define SZ_NOTASK         "Aucune t\x09Ache"
#define SZ_NOTASKTIME     "Aucun horaire pour cette t\x09Ache"
#define SZ_NODISPLAY      "Rien \x095 afficher !"
#define SZ_NOREMIND       "Aucun m\x091mento"

#define SZ_MAILOPENERR1   "Mail que pour PV-750 !"
#define SZ_MAILOPENERR2   "Pas de support Mail !"
#define SZ_MAILOPENERR3   "PVMail non trouv\x091 !"

#define SZ_SHAREDERROR1   "Erreur SaveRecord dans SHARED !"
#define SZ_SHAREDERROR2   "Erreur ReadRecord dans SHARED !"

#endif

/******************************************************************************/
#ifdef LANG_CZE
#define SZ_OWNER          "Vlastn\x092k PV Today"
#define SZ_PREFTITLE      "Preference"
#define SZ_PERSTITLE      "Osobn\x092 Info"
#define SZ_ABOUTTITLE     "O programu ..."
#define SZ_ERRORTITLE     "Chyba!"
#define SZ_OKBUTTON       "OK"
#define SZ_CANCELBUTTON   "Storno"
#define SZ_ABOUT2         "All rights reserved"

#define SZ_SHOWOWNER      "Vlastn\x092k: %s"
#define SZ_SHOWTEL        "Tel. #: %s"
#define SZ_SHOWDATE       "%s %s %s, %s"
#define SZ_SHOWNOTIME     "Neaktu\x090ln\x092 den" 
#define SZ_SHOWTASK       "%d aktivn\x092ch \x094kol\x0EF" 
#define SZ_SHOWUNREADMAIL "%d nep\x0ECe\x0E8ten\x07F zpr\x090v" 
#define SZ_SHOWUNSENDMAIL "%d neodeslan\x07Fch zpr\x090v"
#define SZ_SHOWREMIND     "%d Aktivn\x092ch polo\x0D2ek"

#define SZ_PROWNER        "Vlastn\x092k:"
#define SZ_PRTEL          "Telefon:"
#define SZ_PRCHKDATE      "Datum"
#define SZ_PRCHKOWNER     "Osobn\x092 info"
#define SZ_PRCHKCALEND    "Kalend\x090\x0EC"
#define SZ_PRCHKTASK      "\x084koly"
#define SZ_PRCHKMAIL      "Po\x0EDta"
#define SZ_PRCHKTIME      "12-hodinov\x07F Form\x090t"
#define SZ_PRCHKTSKACT    "Prob\x092h\x090 kontrola kalend\x092\x0ECe" 
#define SZ_PRCHKSYSTEM    "Systemov\x091 Info" 
#define SZ_PRMAILOPT1     "PV-750 Mail" 
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "\x0A\x090dn\x090 budouc\x092 sch\x0EFzka" 
#define SZ_NOUNREADMAIL   "\x0A\x090dn\x091 nep\x0ECe\x0E8ten\x091 zpr\x090vy" 
#define SZ_NOUNSENDMAIL   "\x0A\x090dn\x091 neodesl. zpr\x090vy" 
#define SZ_NOTASK         "\x0A\x090dn\x091 \x094koly" 
#define SZ_NOTASKTIME     "Neur\x0E8en\x07F \x0E8as pro tento \x094kol" 
#define SZ_NODISPLAY      "Nen\x092 co zobrazit!"
#define SZ_NOREMIND       "\x0a\x090dn\x090 p\xecipom\x092nka"

#define SZ_MAILOPENERR1   "Po\x0EDta pouze pro PV-750!"
#define SZ_MAILOPENERR2   "Po\x0ED nen\x092 podporov\x090na!" 
#define SZ_MAILOPENERR3   "PVMail nenalezen!"

#define SZ_SHAREDERROR1   "Ulo\x0d2en\x092 z\x090znamu do SHARED selhalo!"
#define SZ_SHAREDERROR2   "\x04te\x092 z\x090znamu v SHARED selhalo!"

#endif

/******************************************************************************/
#ifdef LANG_SPA

#define SZ_OWNER          "Pv Today Propietario" 
#define SZ_PREFTITLE      "Preferencias" 
#define SZ_PERSTITLE      "Info Personal" 
#define SZ_ABOUTTITLE     "Acerca de ..."
#define SZ_ERRORTITLE     "Error !"
#define SZ_OKBUTTON       "OK" 
#define SZ_CANCELBUTTON   "Cancelar" 
#define SZ_ABOUT2         "Todos los derechos reservados" 

#define SZ_SHOWOWNER      "Propietario : %s"   
#define SZ_SHOWTEL        "Tel\x091fono # : %s"  
#define SZ_SHOWDATE       "%s %s %s, %s"                  /* NameDay dia NameMonth Day    , a�o */  
#define SZ_SHOWNOTIME     "Dia actual"   
#define SZ_SHOWTASK       "%d tarea/s activa" 
#define SZ_SHOWUNREADMAIL "%d mensajes no leidos" 
#define SZ_SHOWUNSENDMAIL "%d mensajes no enviados" 
#define SZ_SHOWREMIND     "%d recuerdos activos" 

#define SZ_PROWNER        "Propietario :"    
#define SZ_PRTEL          "Tel\x091fono :" 
#define SZ_PRCHKDATE      "Fecha" 
#define SZ_PRCHKOWNER     "Informaci\x093n del propietario"  
#define SZ_PRCHKCALEND    "Calendario"  
#define SZ_PRCHKTASK      "Tarea" 
#define SZ_PRCHKMAIL      "E-mail" 
#define SZ_PRCHKTIME      "Formato 12h" 
#define SZ_PRCHKTSKACT    "Comprobar temas actuales"
#define SZ_PRCHKSYSTEM    "Informaci\x093n del sistema"
#define SZ_PRMAILOPT1     "PV-750 Mail"   
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "Ninguna cita pendiente"   
#define SZ_NOUNREADMAIL   "No hay mensajes sin leer"
#define SZ_NOUNSENDMAIL   "No hay mensajes sin enviar"
#define SZ_NOTASK         "No hay tareas pendientes"
#define SZ_NOTASKTIME     "tiempo no definido" 
#define SZ_NODISPLAY      "Nada que mostrar !" 
#define SZ_NOREMIND       "No hay recordatorios."

#define SZ_MAILOPENERR1   "Solo para PV-750 !"
#define SZ_MAILOPENERR2   "No mail soportado !"  
#define SZ_MAILOPENERR3   "PVMail no encontrado !"

#define SZ_SHAREDERROR1   "Error al Grabar en SHARED !"
#define SZ_SHAREDERROR2   "Error al Leer de SHARED !"

#endif 

/******************************************************************************/
#ifdef LANG_RUS

#define SZ_OWNER          "PV Today \x0AB\x0B1\x095\x0B0\x0BB\x0B1\x0BB\x0B3"
#define SZ_PREFTITLE      "\x082\x095\x0B7\x098\x0DE\x09C\x091\x09B\x096"
#define SZ_PERSTITLE      "\x0A1\x096\x0C3\x092\x095\x0D8 \x096\x092\x09D\x09C\x0DE\x097\x095\x0B3\x096\x0D8"
#define SZ_ABOUTTITLE     "About ..."
#define SZ_ERRORTITLE     "\x08C\x0BC\x096\x090\x09B\x095 !"
#define SZ_OKBUTTON       "OK"
#define SZ_CANCELBUTTON   "\x08C\x098\x097\x0BB\x092\x095"
#define SZ_ABOUT2         "\x0AB\x0B7\x0BB \x0B2\x0DE\x095\x09A\x095 \x0BA\x095\x0DE\x0BB\x0B5\x096\x0B7\x098\x0DE\x096\x0DE\x09C\x09A\x095\x092\x09E"

#define SZ_SHOWOWNER      "\x0AB\x0B1\x095\x0B0\x0BB\x0B1\x0BB\x0B3 : %s"
#define SZ_SHOWTEL        "\x088\x0BB\x0B1\x0BB\x09D\x09C\x092 # : %s"
#define SZ_SHOWDATE       "%s %s %s, %s" /* NameDay NameMonth Day, Year */
#define SZ_SHOWNOTIME     "\x082\x0BB \x0B7\x0BB\x0B5\x09C\x0B0\x092\x0D8\x0BC\x092\x096\x091 \x0B0\x0BB\x092\x0B4"
#define SZ_SHOWTASK       "\x085\x09B\x098\x096\x09A\x092\x09E\x0B6 \x0BA\x095\x0B0\x095\x0C3 - %d"
#define SZ_SHOWUNREADMAIL "\x082\x0BB\x0B2\x0DE\x09C\x0C3\x096\x098\x095\x092\x092\x09E\x0B6 \x0B7\x09C\x09C\x090\x099\x0BB\x092\x096\x091 - %d"
#define SZ_SHOWUNSENDMAIL "\x082\x0BB\x09C\x098\x0B2\x0DE\x095\x09A\x0B1\x0BB\x092\x09C \x0B7\x09C\x09C\x090\x099\x0BB\x092\x096\x091 - %d"
#define SZ_SHOWREMIND     "\x085\x09B\x098\x096\x09A\x092\x09E\x0B6 \x092\x095\x0B2\x09C\x097\x096\x092\x095\x092\x096\x091 - %d"

#define SZ_PROWNER        "\x0AB\x0B1\x095\x0B0\x0BB\x0B1\x0BB\x0B3 :"
#define SZ_PRTEL          "\x088\x0BB\x0B1\x0BB\x09D\x09C\x092\x092\x09E\x091 \x092\x09C\x097\x0BB\x0DE :"
#define SZ_PRCHKDATE      "\x0A0\x095\x098\x095"
#define SZ_PRCHKOWNER     "\x086\x092\x09D\x09C \x09C \x09A\x0B1\x095\x0B0\x0BB\x0B1\x0B4\x0B3\x0BB"
#define SZ_PRCHKCALEND    "\x0DF\x095\x0B7\x0B2\x096\x0B7\x095\x092\x096\x0BB"
#define SZ_PRCHKTASK      "\x0AA\x095\x0B0\x095\x0C3\x096"
#define SZ_PRCHKMAIL      "\x0A2\x09C\x0C3\x098\x095"
#define SZ_PRCHKTIME      "12-\x098\x096 \x0C3\x095\x0B7\x09C\x09A\x09C\x091 \x09D\x09C\x0DE\x097\x095\x098"
#define SZ_PRCHKTSKACT    "\x0A2\x0DE\x09C\x09A\x0BB\x0DE\x09B\x095 \x09A\x09E\x0B2\x09C\x0B1\x092\x0BB\x092\x096\x0D8 \x0DE\x095\x0B7\x0B2\x096\x0B7\x095\x092\x096\x0D8"
#define SZ_PRCHKSYSTEM    "\x0A7\x096\x0B7\x098\x0BB\x097\x092\x095\x0D8 \x096\x092\x09D\x09C\x0DE\x097\x095\x0B3\x096\x0D8"
#define SZ_PRMAILOPT1     "PV-750 Mail"
#define SZ_PRMAILOPT2     "PVMail 2.0.4"

#define SZ_NOSCHEDULE     "\x082\x0BB\x098 \x09A\x0B6\x09C\x0B0\x0D8\x099\x096\x0B6 \x09A\x0B7\x098\x0DE\x0BB\x0C3"
#define SZ_NOUNREADMAIL   "\x082\x0BB\x098 \x092\x0BB\x0B2\x0DE\x09C\x0C3\x096\x098\x095\x092\x092\x09E\x0B6 \x0B7\x09C\x09C\x090\x099\x0BB\x092\x096\x091"
#define SZ_NOUNSENDMAIL   "\x082\x0BB\x098 \x092\x0BB\x09C\x098\x0B2\x0DE\x095\x09A\x0B1\x0BB\x092\x092\x09E\x0B6 \x0B7\x09C\x09C\x090\x099\x0BB\x092\x096\x091"
#define SZ_NOTASK         "\x082\x0BB\x098 \x0BA\x095\x0B0\x095\x0C3"
#define SZ_NOTASKTIME     "\x0A0\x0B1\x0D8 \x0E3\x098\x09C\x091 \x0BA\x095\x0B0\x095\x0C3\x096 \x09A\x0DE\x0BB\x097\x0D8 \x092\x0BB \x09C\x0B2\x0DE\x0BB\x0B0\x0BB\x0B1\x0BB\x092\x09C"
#define SZ_NODISPLAY      "\x082\x0BB\x0C3\x0BB\x0B5\x09C \x09C\x098\x09C\x090\x0DE\x095\x0B9\x095\x098\x0B4 !"
#define SZ_NOREMIND       "\x082\x0BB\x098 \x092\x095\x0B2\x09C\x097\x096\x092\x095\x092\x096\x091"

#define SZ_MAILOPENERR1   "\x0A2\x09C\x0C3\x098\x095 \x098\x09C\x0B1\x0B4\x09B\x09C \x0B0\x0B1\x0D8 PV-750 !"
#define SZ_MAILOPENERR2   "\x082\x0BB\x098 \x0B2\x09C\x0B0\x0B0\x0BB\x0DE\x0B9\x09B\x096 \x0B2\x09C\x0C3\x098\x09E !"
#define SZ_MAILOPENERR3   "PVMail \x092\x0BB \x092\x095\x091\x0B0\x0BB\x092 !"

#define SZ_SHAREDERROR1   "\x08C\x0BC\x096\x090\x09B\x095 \x0BA\x095\x0B2\x096\x0B7\x096 \x09A SHARED !"
#define SZ_SHAREDERROR2   "\x08C\x0BC\x096\x090\x09B\x095 \x0C3\x098\x0BB\x092\x096\x0D8 \x096\x0BA SHARED !"
#endif 

/******************************************************************************/
#define I_MAXINFO 6
#define I_MAXNAME 25

#define	OBJ_AREA                        0x9002
#define	OBJ_STRING                      0x9003

#define OBJ_IC_POK                      0x9000
#define OBJ_IC_PCANCEL                  0x9001
#define OBJ_IC_DSP_NONE                 0x9002
#define OBJ_IC_AOK                      0x9003
#define OBJ_IC_PCHECK                   0x9004
#define OBJ_IC_CHOICE                   0x9005
#define OBJ_IC_PEOK                     0x9006
#define OBJ_IC_PECANCEL                 0x9007
#define OBJ_IC_PETXT1                   0x9008
#define OBJ_IC_PETXT2                   0x9009
#define OBJ_IC_LOK                      0x900A
#define OBJ_IC_REFRESH                  0x900B         /* V 1.2 */
#define OBJ_IC_PREVDAY                  0x900C         /* V 2.0 */
#define OBJ_IC_NEXTDAY                  0x900D         /* V 2.0 */
#define OBJ_IC_CHANDAY                  0x900E         /* V 2.0 */
#define OBJ_IC_PMAIL                    0x900F         /* V 2.0 */

#define  PDWN_OPT_OPTIONS  PULLDOWN_OPTION | 0x0001
#define  PDWN_OPT_PERSONA  PULLDOWN_OPTION | 0x0002
#define  PDWN_OPT_VERSION  PULLDOWN_OPTION | 0x0004

typedef struct
{
  byte blnInfoShow[I_MAXINFO+1];		/* Differents showed infos */
  byte bInfoOrder[I_MAXINFO+1];			/* infos order */
  char szInfoOwner[I_MAXNAME+1];                /* Owner information */
  char szInfoPhone[I_MAXNAME+1];                /* Phone information */
  byte bChkTime;                                /* Check if 12hrs format for time */
  byte bChkSched;                               /* Check if schedule in progress */
} INFOSHOW;

typedef struct
{
  TXTP EventPrm;
  TCHTBL TchTxtBuf[3];
  T_SCR_POS ComScr;
} TEXTAREA;

extern TEXTAREA    TextArea[2];                 /* Ptr to Textbuffer struct */

extern byte blnInfoHere[I_MAXINFO+1];           /* To select correct info */

