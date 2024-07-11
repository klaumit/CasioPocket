/******************************************************************************
*  Program Title 	Escoba
*  Module      	Main game
*  Author         J. Marcos Matilla (marcosjo@wanadoo.es)
*  Date           12/2001
*  Rev            1.0
*  MMSoft 2000-2002 Freeware
*  Notes:   
*  For english notes, see below
*
*  		ESCOBA (Versión española)
*	La ESCOBA es un juego basado en la baraja española.
*  Al inicio del juego, se reparten 3 cartas a cada jugador y 4 mas sobre la mesa
*	Cada jugador, jugando por turno, necesita sumar 15 puntos utilizando una de sus cartas
*  y al menos una de las cartas de la mesa.
*  El valor de cada carta es el su número, excepto:
* 	10 sota: 	8 puntos
*  11 caballo: 9 puntos
*  12 rey:    10 puntos
*
*  Cuando un jugador consigue una combinacion de 15 puntos, recoge todas las cartas de la
*  combinacion (la propia y las de la mesa que ha seleccionado) y las coloca en su mazo.
*	Si en la jugada consigue todas las cartas de la mesa, gana una "escoba".
*	Si no puede obtener ninguna combinacion, tendrá que descartar una de sus cartas sobre la
*  mesa y continua el siguiente jugador.
*
*	Cuando todos los jugadores terminan sus cartas, se reparten tres cartas mas a cada uno.
*	Si el mazo no contiene mas cartas, se termina el juego. Las cartas restantes sobre la mesa
*  son para el jugador que ganó la última mano.
*
*	En las puntuaciones finales,  cada jugador suma sus "escobas" y hasta cuatro mas 
* 	dependiendo de:
* 	Una "escoba" mas para el jugador que haya obtenido mas cartas.
*	Otra mas para el jugador con mas oros entre sus cartas.
* 	Otra para el jugador con mas sietes.
* 	La última para el jugador con el 7 de oros.
*	El ganador será el que obtenga mas "escobas" (puntos) en total.
*
*  ¿COMO JUGAR EN LA PV?
*	Puede jugar contra uno o dos oponentes: uno a la derecha y otro a la izquierda.
*  Cuando sea su turno, debe seleccionar (tocar) una de sus cartas y una o varias
*  de la mesa. Toque de nuevo para deseleccionar las cartas de la mesa.
*  Cuando la suma total alcance 15 puntos, todas las cartas se recogeran hacia su mazo.
*
*	Para descartar una carta, simplemente seleccionela y toque en el area de descarte o
*  sobre un espacio vacío de la mesa.
*
*	AJUSTES:
*	Abra la barra de menú para iniciar un nuevo juego o cambiar los ajustes.
*	Los ajustes cambian:
*  Nivel de juego (3 es el mas difícil)
*	Número de jugadores (2 ó 3)
*	Activar el modo DEMOSTRACION para aprender
*  Cuando el modo DEMO está activo, pulse ESC o MENU BAR para regresar a la pantalla
*  de ajustes. Debe de hacerse durante las pausas o cuando aparezca el mensaje.
*
*	La información mostrada durante el juego puede variar dependiendo del nivel seleccionado.
*	En el nivel fácil (1) se puede ver informacion de la suma de los puntos de cada carta 
*  y la información de los mazos de cada jugador: cartas y "escobas".
*	En el nivel medio (2) el juego solo muestrala información de los mazos.
*	En el nivel difícil (3) no hay información.
*
*		Diviertase con "ESCOBA"
*  
*
*  		ESCOBA (English version)
*  Escoba is a spanish cards game based on.
*	At start, on deals three cards for each player and four more over the table.
*	Playing sequentially, each player need sum 15 points using one of his cards and one 
* 	of cards over the table at least.
* 	The value of each card is the own number, exception of:
* 	10 sota?:	8 points value
*  11 knight:	9 points value
*  12 king:   10 points value
*
*  When a player gets a combination with 15 points, he collects all cards in the combination
*  (the own card and the table cards selected), and get them on his deck.
* 	If in his play gets all cards on the table, he wins an "escoba"
*  If can't get a 15 points combination, he will need discard one of his cards over the table,
*  and continues the next player.
*
*  When all players end they cards, on deals again three cards each one and so.
*  If the deck has no more cards, the game ends. The remaining cards over the table will be
*  to the last hand winner.
*  
*  At final scores, each player sums all his "escobas" and four more depending of:
*	One "escoba" more for the player who has more cards in total. (None if draw)
*	Other for the player with more gold coins (oros) cards in his deck.
*  Other for the player with more cards with 7.
*  The last for the player with the 7 oros.
*  The winner will be the player with more escobas (points) in total.
*
*	HOW TO PLAY IN THE PV?
*  You can play against one or two opponents: one at right side and other at left.
*  When your turn is, you must select (touch) one of your cards and one or several
*  over the table. Touch again to unselect the table's card.
*  When the total cards sum reaches 15 points, all cards will be collected to your deck.
*  To discard a card, simply select it and touch in the discard area or over an empty
*  space on the table. 
*
*	SETTINGS:
*	Open the menu bar to start a new play o change game settings.
*	The settings are:
*	The game level (3 is hardest)
*	Player's number (2 or 3)
*	Set the DEMO to learn.
*	When the demo mode es active, press ESC or MENU BAR again for back to settings screen.
*	This operation must be made during game pauses or when message display appears.
*
*	The information showed during the game can change depending of the level selected.
*  In the easy level (1) you can see information about the sum of points of each card
*  and information about the decks: cards and "escobas".
*	In the medium level (2) the game only shows the deck information.
*	In the hardest level (3) there are no information.
*
*		Enjoy with "ESCOBA"
*
*					J. Marcos Matilla
*
*	Agreements:
*	To Michael Denzel for his help to translate the german messages
*		Thanks Michael!
******************************************************************************/

#include <escoba.h>
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_libc.h>
#include <libc.h>

/* Touch area for main game */   
TCHTBL far TchPlayer[]=    
 { /* card 1 area */
   ME_CARD0_L,ME_CARD0_T,ME_CARD0_R,ME_CARD0_B,ACT_BREAK_IN,OBJ_PLAY0,0x0000,
   /* PLAYR 2 area */
   ME_CARD1_L,ME_CARD1_T,ME_CARD1_R,ME_CARD1_B,ACT_BREAK_IN,OBJ_PLAY1,0x0000,
   /* PLAYR 3 area */
   ME_CARD2_L,ME_CARD2_T,ME_CARD2_R,ME_CARD2_B,ACT_BREAK_IN,OBJ_PLAY2,0x0000,  
   TCH_END}; 

TCHTBL far TchTable[]=   
 { /* card 0 in table */
   TABLE0_L,TABLE0_T,TABLE0_R,TABLE0_B,ACT_BREAK_IN,OBJ_TBL0,0x0000,
   /* card 1 in table */
   TABLE1_L,TABLE1_T,TABLE1_R,TABLE1_B,ACT_BREAK_IN,OBJ_TBL1,0x0000,
   /* card 2 in table */
   TABLE2_L,TABLE2_T,TABLE2_R,TABLE2_B,ACT_BREAK_IN,OBJ_TBL2,0x0000,  
   /* card 3 in table */
   TABLE3_L,TABLE3_T,TABLE3_R,TABLE3_B,ACT_BREAK_IN,OBJ_TBL3,0x0000,  
   /* card 4 in table */
   TABLE4_L,TABLE4_T,TABLE4_R,TABLE4_B,ACT_BREAK_IN,OBJ_TBL4,0x0000,     
   /* card 5 in table */
   TABLE5_L,TABLE5_T,TABLE5_R,TABLE5_B,ACT_BREAK_IN,OBJ_TBL5,0x0000,     
   /* card 6 in table */
   TABLE6_L,TABLE6_T,TABLE6_R,TABLE6_B,ACT_BREAK_IN,OBJ_TBL6,0x0000,     
   /* card 7 in table */
   TABLE7_L,TABLE7_T,TABLE7_R,TABLE7_B,ACT_BREAK_IN,OBJ_TBL7,0x0000,     
   /* card 8 in table */
   TABLE8_L,TABLE8_T,TABLE8_R,TABLE8_B,ACT_BREAK_IN,OBJ_TBL8,0x0000,     
   /* card 9 in table */
   TABLE9_L,TABLE9_T,TABLE9_R,TABLE9_B,ACT_BREAK_IN,OBJ_TBL9,0x0000,     
   /* card 10 in table */
   TABLE10_L,TABLE10_T,TABLE10_R,TABLE10_B,ACT_BREAK_IN,OBJ_TBL10,0x0000,     
   /* card 11 in table */
   TABLE11_L,TABLE11_T,TABLE11_R,TABLE11_B,ACT_BREAK_IN,OBJ_TBL11,0x0000,     
   TCH_END};

TCHTBL far Tchdiscard[]=   
 { /* discard area */
   DISCARD_L,DISCARD_T,DISCARD_R,DISCARD_B,
   ACT_BREAK_IN,
   OBJ_DISC,
   0x0000,
   TCH_END};
      
void  InitGame(void);
bool  MyMove(void);
bool  CasioMove(int);
bool  CasioSel (int);
void  Collect(int);
void  Discard(int);
bool  CheckCardsSum();
bool  CheckEscoba();


/******************************************************************************
*  [Title]        main
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]                
*  main function:
*  Show logo
*	Prepare game with settings
*	Play each hand turn checking new deal or end game
*  In demo mode the human player don't play
*  In two players mode, the left player don't play too
******************************************************************************/
void main()
{  int   	plyturn;    /* Player's turn  */
   int   	lasthand;   /* Last hand won  */
   int   	pl;
   int   	crd;
   bool 	 	deal; 
   char 		texto[40];

	LibClrDisp();
	LibPutGraph(30,0,logo); /* Display Logo 			*/
	LibPutDisp();				
	LibWait(IB_1SWAIT);		/* Three seconds wait 	*/
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);		/* Default Settings 		*/
   GameLevel=2;				/* Normal Level			*/		
   nPlayers=3;					/* Three players			*/
   GameDemo=FALSE;			/* DEMO mode disabled	*/
	Settings();
	InitGame();
   plyturn=rand()%nPlayers;		/* Who starts the first game? */
   for(;;){
   	if (GameDemo==TRUE) 
   		WinMsgBox(GetMsg(28),ICON_BIKKURI,3);
      if (Initialize==TRUE) 
      	InitGame();   			
 		strcpy(texto,GetMsg(plyturn));
	   LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1);
      LibPutProStr(IB_CG57FONT,1+(DISCARD_L+DISCARD_R-LibGetProStrSize(IB_CG57FONT,texto))/2,DISCARD_T+5,texto,96);  /* print player turn*/
   	LibPutDisp();	      	
      if (plyturn==PL_ME & GameDemo==FALSE) {
         if (MyMove()==TRUE) lasthand=plyturn;
         }	
      else {    
         if (CasioMove(plyturn)==TRUE)	lasthand=plyturn;
         }
      plyturn++;
      plyturn%=nPlayers;   
      /* Check for new deal or end game*/   
      deal=TRUE;
      for (pl=PL_ME;pl<nPlayers;pl++)
         for(crd=0;crd<CARDS_IN_PLAYER;crd++)
            if (player[pl].card[crd].Id!=NOCARD) deal=FALSE;
      
      if (deal==TRUE)                           /* Deal the player's cards 				*/
         if (deck.remain>0)                    	/* Are more cards in the deck ? 			*/
            DealPlayers();                      /* New deal 									*/             
         else  {                                /* End game 									*/
            for (crd=0;crd<CARDS_IN_TABLE;crd++)	
               if (table[crd].Id!=NOCARD) 		/* Select all ramining cards in table	*/
               	table[crd].select=TRUE;
            CheckHrdIcn(2);							/* Wait 1 second								*/
            Collect(lasthand);     			  		/* Collect remaining cards					*/
            WinMsgBox(GetMsg(6),ICON_SIGN,0);	/* Show scores message 						*/
            ShowScores();								/* Final Scores								*/
            InitGame();
            plyturn=lasthand;							/* Start the last hand won player		*/
            }
      }     
}

/******************************************************************************
*  [Title]        InitGame
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]                
*  Initialize the active areas for the game
*  Initialize the player structures
*  Shuffle the deck 
******************************************************************************/
void InitGame()
{  int pl;
   int crd;
   char texto[50];

	strcpy(texto,GetMsg(16));
	texto[strlen(texto)-1]=GameLevel+'0';
	WinMsgBox(texto,ICON_COFFEE,3);
   LibClrDisp();                                /* Clear screen            */    
   

  /* Touch screen areas */
   LibTchStackClr();             	/* Clear all touch areas   */
   LibTchStackPush(NULL);        	/* Empty stack terminator  */
   LibTchStackPush(TchHardIcon); 	/* Register hard icons     */
   LibTchStackPush(TchModeIcon);
   if (GameDemo==FALSE) {
   	LibTchStackPush(TchPlayer);   /* Register Player's cards */
   	LibTchStackPush(TchTable);    /* Register Table cards    */
   	LibTchStackPush(Tchdiscard);  /* Register discard area   */
   	}

   /* Player0's cards touch areas   */
   for (crd=0;crd<CARDS_IN_PLAYER;crd++) {
      memcpy(player[0].card[crd].coord,Pl0Coords[crd],sizeof(Pl0Coords[crd]));
      memcpy(player[1].card[crd].coord,Pl1Coords[crd],sizeof(Pl1Coords[crd]));
      memcpy(player[2].card[crd].coord,Pl2Coords[crd],sizeof(Pl2Coords[crd]));
      }

   /* Table's cards touch areas  */
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      memcpy(table[crd].coord,TblCoords[crd],sizeof(TblCoords[crd]));
   
   for (pl=0;pl<nPlayers;pl++) {                /* Init player's cards data */
      for (crd=0;crd<CARDS_IN_PLAYER;crd++) {   /* Initialize cards        */
         player[pl].card[crd].Id=NOCARD;        /* Card number             */       
         player[pl].card[crd].select=FALSE;     /* No cards selected       */ 
         }                                         
      for (crd=0;crd<MAXCARDS;crd++)
        player[pl].history[crd]=NOCARD;         /* Clear history cards     */
      player[pl].ncards=0;                      /* Clear player cards      */
      player[pl].escobas=0;                     /* Clear points            */
      }     

   for (crd=0;crd<CARDS_IN_TABLE;crd++){        /* Init table's cards data */
      table[crd].Id=NOCARD;                     /* Clear cards in table    */
      table[crd].select=FALSE;                  /* No cards selected       */ 
      }
      
   Suffle();                                    /* Suffle the deck         */
   Initialize=FALSE;										/* Reset variable				*/

   for (pl=0;pl<nPlayers;pl++) 						/* Decks							*/
      DispPlayerDeck(pl);
/*   LibGdsBox (DISCARD_L-1,DISCARD_T-1,DISCARD_R+1,DISCARD_B+1);/* discard cards area      */             
   LibGdsBoxMesh (DISCARD_L-1,DISCARD_T-1,DISCARD_R+1,DISCARD_B+1); 	/* discard cards area      */             
/*   LibGdsBoxMesh (DISCARD_L,DISCARD_T,DISCARD_R,DISCARD_B); 	/* discard cards area      */             
   DealPlayers();    /* Deal the player's cards 		*/    
   DealTable();		/* Deal four cards on the table  */
} /* End InitGame */


/******************************************************************************
*  [Title]        MyMove
*  [Arguments]    
*  [ReturnVAL]    TRUE if was sucessfull
*  [Description]              
*  Select the cards for a hand o a card to discard
******************************************************************************/
bool MyMove()
{  TCHSTS tsts;          /* Variable for touch event     */
   int crd;
   int plysel;          /* Last player's card touched    */
   int tblsel;          /* Table's card touched          */

      
  LibTchInit();         /* Clear touch informations      */  
  LibTchWait(&tsts);    /* Wait for a touch event        */
  plysel=NOCARD;        /* No player's card selected     */
   
  for (;;) {   /* Repeat forever */
   switch (tsts.obj) {
       case OBJ_HIC_ESC:   /* ESC key*/
         LibJumpMenu();
         break;

       case OBJ_HIC_MBAR:  /* MENU Key*/
        	GmPdwnFnc();
        	if (Initialize==TRUE)
        		return(FALSE);
         break;
                  
       case OBJ_PLAY0: 		/* Player's card #0 */
       case OBJ_PLAY1: 		/* Player's card #1 */
       case OBJ_PLAY2: 		/* Player's card #2 */
         if (player[PL_ME].card[tsts.obj-OBJ_PLAY0].Id==NOCARD)
            break;                                    /* Invalid card selected   */
            
         plysel=tsts.obj-OBJ_PLAY0;                   /* Actual card selected    */          
         for (crd=0;crd<CARDS_IN_PLAYER;crd++) 
            player[PL_ME].card[crd].select=(plysel==crd); /* Only mark the last card */

         if (CheckCardsSum(PL_ME)==TRUE) {
            Collect(PL_ME);
            return(TRUE);
            }
         else  
            DispPlayerCards(PL_ME);
         break;       

       case OBJ_TBL0: 		/* Table's card #0 */
       case OBJ_TBL1: 		/* Table's card #1 */
       case OBJ_TBL2: 		/* Table's card #2 */
       case OBJ_TBL3: 		/* Table's card #3 */
       case OBJ_TBL4: 		/* Table's card #4 */
       case OBJ_TBL5: 		/* Table's card #5 */
       case OBJ_TBL6: 		/* Table's card #6 */
       case OBJ_TBL7: 		/* Table's card #7 */
       case OBJ_TBL8: 		/* Table's card #8 */
       case OBJ_TBL9: 		/* Table's card #9 */
       case OBJ_TBL10: 		/* Table's card #10 */
       case OBJ_TBL11: 		/* Table's card #11 */       
         tblsel=tsts.obj-OBJ_TBL0;           
         if (table[tblsel].Id!=NOCARD){   /* Table card selected */  
            table[tblsel].select=!(table[tblsel].select);
            DispTableCards();          
            if (CheckCardsSum(PL_ME)==TRUE) {
               Collect(PL_ME);
               return(TRUE);
               }
            }     
         else 		/* no table card selected. Discard? */
            if (plysel!=NOCARD) {
				   table[tblsel].Id=player[PL_ME].card[plysel].Id;
   				player[PL_ME].card[plysel].Id=NOCARD;        /* Remove card from player */
   				player[PL_ME].card[plysel].select=FALSE;     /* Unselect the card       */
               for (crd=0;crd<CARDS_IN_TABLE;crd++)
                  table[crd].select=FALSE;   					/* deselect all table cards */
               DispPlayerCards(PL_ME);   
               DispTableCards(); 
               return(FALSE);
            	}
         	else
         		WinMsgBox(GetMsg(19),ICON_BIKKURI,0);   
         break;
         
         case OBJ_DISC:    	/* discard? */
            if (plysel!=NOCARD) {
               for (crd=0;crd<CARDS_IN_TABLE;crd++)
                  table[crd].select=FALSE;   /* deselect all table cards */
               DispTableCards(); 
               Discard(PL_ME);
               return(FALSE);
            	}
            else
            	WinMsgBox(GetMsg(17),ICON_BIKKURI,0);         
            break;

       default:
         break;  
      }  /* end switch */
    LibTchWait(&tsts);          /* Wait for next touch */
   }     
}

/******************************************************************************
*  [Title]        CasioMove
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]              
*  Casio's turn
******************************************************************************/
bool CasioMove(int pl)
{  int crd;

	CheckHrdIcn(4);

/* Has the player cards yet?  */
   for (crd=0;(crd<CARDS_IN_PLAYER)&(player[pl].card[crd].Id==NOCARD);crd++);
   if (crd==CARDS_IN_PLAYER) 
      return (FALSE);
   
   if (CasioSel(pl)==TRUE) {  /* Had the Player a valid combination?    */
      Collect(pl); 
      return(TRUE);
      }
   else {   /* Select a card to discard */
      Discard(pl);
      return(FALSE);
      }
}  

/******************************************************************************
*  [Title]        CasioSel
*  [Arguments]    pl: player number
*  [ReturnVAL]    TRUE if a combination was found (selects the best cards to collect)
*						FALSE if not (selects worst card to discard)
*												
*  [Description]                
*  Select a valid combination (sum values = 15)
*  Each combination has the cards_id and the total score.
*  The score for a ESCOBA is 100
*        "     "   7 only is 30
*        "     "   gold   is 10
*        "     "   the rest is 1 point
*  If the game level is 1 (easy), selects the first valid combination (not the best)
*  if the game level is 2 (normal) selects a random combination
*  if the level is 3 (hard) selects the best allways.
*  A special case is ESCOBA. Allways selects it.
******************************************************************************/
bool CasioSel(int pl)   
#define MAXCOMB   30
{  int plysel;       /* Player card in selection   */
   int tblsel;       /* Table card in selection    */
   int ncomb;        /* Combinations               */
   int ncard;        /* Cards in the combination   */
   int cardId;			/* Id for cards in for loop	*/
   int tblcards;     /* Cards in table             */
   int sum;          /* Sum values for combination */
   int crd;          /* temporal for loops         */
   int totalcomb;    /* Total of combinations      */ 
   TComb selection[MAXCOMB];
   int bestcomb[3];  /* The 3 best combinations    */
   
   ncomb=0;
   for (plysel=0;plysel<CARDS_IN_PLAYER;plysel++)  /* check for each player card 		*/
      if (player[pl].card[plysel].Id!=NOCARD){     /* Is there a card or empty space?  */
         sum=GetValue(player[pl].card[plysel].Id);    
         ncard=0;
         selection[ncomb].card[ncard]=plysel;      /* First card is the player card */
         ncard++;
         tblsel=0;            
         do {  /* while cards in the last combination */                 
            do {  /* while cards in table */
               if (table[tblsel].Id!=NOCARD) {      /* Is a valid card? */
	               sum+=GetValue(table[tblsel].Id);
                     
   	            if (sum<15) {     /* No points enought    */
      	            selection[ncomb].card[ncard]=tblsel; 	/* Adds card to selection  */
         	         ncard++;                
            	      }           

               	if (sum>15)       /* Too much points   */
                  	sum-=GetValue(table[tblsel].Id); /* Continue the sum without last card */

	               if (sum==15){                       /* Valid combination       */
   	               selection[ncomb].card[ncard]=tblsel;      /* Adds card to selection  */                               
      	            selection[ncomb].card[ncard+1]=NOCARD;    /* Ends the combination    */          
         	         /* Get last valid combination score */
            	      selection[ncomb].score=ncard+1;    /* first value is the cards amount */
               	   /* Check for ESCOBA */
                  	tblcards=0;
	                  for (crd=0;crd<CARDS_IN_TABLE;crd++)  /* Count cards in table */
   	                  if (table[crd].Id!=NOCARD) tblcards++;
      	            if (tblcards==ncard)  /* ESCOBA!!! */
         	            selection[ncomb].score+=100;                       
            	      /* Continue with the rest of scores*/
               	   for (crd=0;selection[ncomb].card[crd]!=NOCARD;crd++){
								if (crd==0) /* The first card is asigned to player card */
									cardId=player[pl].card[selection[ncomb].card[crd]].Id;
								else	
									cardId=table[selection[ncomb].card[crd]].Id;
      	               if (GetValue(cardId)==7) 
         	               selection[ncomb].score+=30;   /* The card is a 7      */
            	         if (GetSuit(cardId)==T_OROS)
               	         selection[ncomb].score+=10;    /* The card is a Gold   */ 
                  	   } /* end for */
	                  /* Copy last combination to continue with the next cards */
   	               memcpy(&selection[ncomb+1],&selection[ncomb],sizeof(TComb)); 
         	         ncomb++;                         /* Continue the new combination  */
      	            sum-=GetValue(table[tblsel].Id); /* Continue the sum without last card */
            	   	}  /* end if (sum==15) */           
              		}  /* end if is a valid card */
           		tblsel++;   /* Continue with next card */       	
           	} while ((tblsel<CARDS_IN_TABLE)&(ncomb<MAXCOMB-1));  
           	/* repeat while cards in table and ncomb in bounds of array */           
            
         	/* the loop has finalized */        
         	ncard--;                   					/* Continue with last card  		*/
         	tblsel=selection[ncomb].card[ncard];      /* What's the last card in list? */
         	sum-=GetValue(table[tblsel].Id); 			/* restore last sum              */    
         	tblsel++;                     				/* Try with next card            */
         	} while ((ncard>0)&(ncomb<MAXCOMB-1)&(tblsel<CARDS_IN_TABLE)); /* end while cards in list 		*/    
      	}  /* end if and for */
      
   if (ncomb==0)  {  /* No combinations found. Select the worst card */
     	sum=0;	
     	for(crd=0;crd<CARDS_IN_TABLE;crd++) 				  /* Sum of cards in table */
     		if (table[crd].Id!=NOCARD) sum+=GetValue(table[crd].Id);	
      for (plysel=0;plysel<CARDS_IN_PLAYER;plysel++)
         if (player[pl].card[plysel].Id!=NOCARD) {
         	bestcomb[plysel]=1;									/* The valid card have 1 point at least */	
         	if (GetValue(player[pl].card[plysel].Id)==7) 		/* 7			*/
         		bestcomb[plysel]+=30;
         	if (GetSuit(player[pl].card[plysel].Id)==T_OROS)  	/* Gold		*/
         		bestcomb[plysel]+=10;
				if (sum+GetValue(player[pl].card[plysel].Id)>5) 	/* Total cards < 5 is good 	*/
					bestcomb[plysel]+=GetValue(player[pl].card[plysel].Id);
				if (sum+GetValue(player[pl].card[plysel].Id)<16)	/* Total cards >15 is better 	*/
					bestcomb[plysel]+=5;					
				}	/* end if */ 
			else
				bestcomb[plysel]=200; /* Highest value to ignore it */
			/* end for */		
		/* Select the less calculated value card */	
		plysel=0;
		for (crd=0;crd<CARDS_IN_PLAYER;crd++) 
			if ((bestcomb[crd]<bestcomb[plysel])&(player[pl].card[crd].Id!=NOCARD))
				plysel=crd; /* New worst card */
		player[pl].card[plysel].select=TRUE;		/* returns with worst card selected */
      return(FALSE);
      }
   /* Some combinations was found. Select the best */
   totalcomb=ncomb;
   /* Search for the three best combinations */
   bestcomb[0]=0;
   bestcomb[1]=0;
   bestcomb[2]=0;
   for (ncomb=1;ncomb<totalcomb;ncomb++) {
      if (selection[ncomb].score>selection[bestcomb[0]].score) {  /* The best  					*/
      	bestcomb[2]=bestcomb[1];
      	bestcomb[1]=bestcomb[0];
      	bestcomb[0]=ncomb;
      	continue;
         } /* end if */
      if (selection[ncomb].score>selection[bestcomb[1]].score) {  /* Better than the middle  */
      	bestcomb[2]=bestcomb[1];
      	bestcomb[1]=ncomb;
      	continue;
      	} /* end if */
      if (selection[ncomb].score>selection[bestcomb[2]].score) {   /* Better than the worst   */
          bestcomb[2]=ncomb;
          continue;
          }      
      } /* end for */
   
   /* fill the empty combinations */
   if (selection[bestcomb[1]].score==0) bestcomb[1]=bestcomb[0];  
   if (selection[bestcomb[2]].score==0) bestcomb[2]=bestcomb[1];        
   if (selection[bestcomb[0]].score>=100)  /*Is the best combination a Escoba? */
      ncomb=bestcomb[0];
   else  
      switch (GameLevel) {
         case 1:     /* Easy     */
            ncomb=bestcomb[2];                  
         	break;
      	case 2:     /* Normal   */
            ncomb=bestcomb[1];                  
         	break;
      	default:    /* Hard     */
            ncomb=bestcomb[0];
         	break;
      }     
   /* Display the selected combination */ 
   player[pl].card[selection[ncomb].card[0]].select=TRUE;      /* Player card selected */
   DispPlayerCards(pl);
   for (crd=1;selection[ncomb].card[crd]!=NOCARD;crd++) {		/* Table cards selected */
      table[selection[ncomb].card[crd]].select=TRUE;
      DispTableCards();
      CheckHrdIcn(2);
      }
   return (TRUE);
}     


/******************************************************************************
*  [Title]        Collect
*  [Arguments]    pl: player number
*  [ReturnVAL]    
*  [Description]                
*  Get all selected table's cards and the player card after a valid hand
******************************************************************************/
void Collect(int pl)
{  int plysel; /* player selection */
   int crd;

   DispDiscardArea(pl);
   
   /* What's the selected player's card?   */
   for (plysel=0;(player[pl].card[plysel].select!=TRUE) & (plysel<CARDS_IN_PLAYER);plysel++);

   /* Add the player's card info to player deck    */
   if (plysel!=CARDS_IN_PLAYER) {  /* Have the player some selected card? 	*/
   	player[pl].history[player[pl].ncards]=player[pl].card[plysel].Id;
   	player[pl].card[plysel].Id=NOCARD;      /* Remove card from player 	*/
   	player[pl].card[plysel].select=FALSE;   /* Unselect card           	*/
   	player[pl].ncards++;
   	}
      
   /* Add the Table's cards info to player deck */
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      if (table[crd].select==TRUE){
         player[pl].history[player[pl].ncards]=table[crd].Id;            
         table[crd].Id=NOCARD;               /* Remove selected table cards */
         table[crd].select=FALSE;            /* Deselect all cards          */
         player[pl].ncards++;
      	}        
      
	/* Check for "escoba" only if some player card has been selected */
		if ((CheckEscoba()==TRUE)&(plysel!=CARDS_IN_PLAYER)) {  /* Have the player some selected card? 	*/
   		player[pl].escobas++;
	     	switch (pl) {
   	     	case PL_ME:
      	  		PrintCard(DECK0_L+7,DECK0_T-10,player[pl].history[player[pl].ncards-1]);
        			break;
	        	case PL_RIGHT:	
   	     		PrintCard(DECK1_L+7,DECK1_T-10,player[pl].history[player[pl].ncards-1]);
      	  		break;
        		case PL_LEFT:	
        			PrintCard(DECK2_L+7,DECK2_T-10,player[pl].history[player[pl].ncards-1]);
	        		break;
   	     	}	
   	   LibPutDisp();  	
  			WinMsgBox(GetMsg(4),ICON_OK,2);		/* ESCOBA!!! Message */ 	
			}
	DispPlayerDeck(pl);                       /* Draw the deck                 	*/          
   DispTableCards();                         /* Draw new table cards    	    	*/
   DispPlayerCards(pl);                      /* Draw new player cards	       	*/
}  

/******************************************************************************
*  [Title]        Discard 
*  [Arguments]    pl: player number
*  [ReturnVAL]    
*  [Description]                
*  Release a card to an empty space
******************************************************************************/
void Discard (int pl)
{  int plysel; /* player selection 					*/
	int tblsel; /* table position to discard it	*/
   int crd;

   DispDiscardArea(pl);
   
   /* What's the selected player's card */
   for (plysel=0;player[pl].card[plysel].select!=TRUE;plysel++);
   
   /* Search the first empty space in the table */
   for (tblsel=0;(tblsel<CARDS_IN_TABLE) & (table[tblsel].Id!=NOCARD);tblsel++);
   if (tblsel==CARDS_IN_TABLE) {						/* Last position reached?					  */	
      WinMsgBox(GetMsg(18),ICON_BADTZ,0);  	/* No more free positions. Abort game!!! */
      LibJumpMenu();
      }

   table[tblsel].Id=player[pl].card[plysel].Id;	/* Copy the selected player card	*/
   player[pl].card[plysel].Id=NOCARD;        /* Remove card from player 		*/
   player[pl].card[plysel].select=FALSE;     /* Unselect the card       		*/
   
   /* Unselect all table's cards */ 
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      table[crd].select=FALSE;

   DispPlayerCards(pl);                      /* Draw player's cards           */   
	SlideCard(DISCARD_L+2,DISCARD_T+1,TblCoords[tblsel][LEFT],TblCoords[tblsel][TOP],table[tblsel].Id);
   DispTableCards();                         /* Draw table's cards            */
}  


/******************************************************************************
*  [Title]        CheckCardsSum
*  [Arguments]    pl: player number
*  [ReturnVAL]    
*  [Description]              
*  Check if the actual combination is valid i.e. total sum values are 15
******************************************************************************/

bool CheckCardsSum()
{  int sum;
   int crd;
   char texto[4];
   int plysel;
   

   sum=0;   
   /* Check for one player's card selected */
   for (plysel=0;(plysel<CARDS_IN_PLAYER)&(player[PL_ME].card[plysel].select==FALSE);plysel++);
   
   if(plysel==CARDS_IN_PLAYER) {                   /* No player's card selected     */
      plysel=NOCARD;
      sum=0;         
      }                 
   else  
      sum=GetValue(player[PL_ME].card[plysel].Id);    /* Selected Player's card value  */
         
   /* Check for table's cards selected */    
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      if ((table[crd].select)&(table[crd].Id!=NOCARD)) 
         sum+=GetValue(table[crd].Id);
   
   /* Is a valid combination ? */
   if ((sum!=15)|(plysel==NOCARD)) {
   	if (GameLevel==1) {	/* Extra help for novices (level 1) */
      	sprintf(texto,"%s: %2d",GetMsg(29),sum);
      	LibPutProStr(IB_PFONT2,DISCARD_L+15,DISCARD_T+17,texto,80);  /* print points sum */
      	LibPutDisp();
      	}
      return (FALSE);            /* Invalid combination  */
      }
   return(TRUE);  
}     
      
/******************************************************************************
*  [Title]        CheckEscoba
*  [Arguments]    True if the table is empty
*                 False if not
*  [ReturnVAL]    
*  [Description]                
*  Check for escoba
******************************************************************************/
bool CheckEscoba()
{ int crd;

   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      if (table[crd].Id!=NOCARD) return (FALSE);
   return (TRUE); 
}  
   
