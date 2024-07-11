/******************************************************************************
*  Program Title  Escoba
*  Module         Graphic functions
*  Author         J. Marcos Matilla (marcosjo@wanadoo.es)
*  Date           12/2001
*  Rev            1.0
*  MMSoft 2000 - 2001 Freeware
*  Notes:   
*
******************************************************************************/
#include <escoba.h>
#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

/******************************************************************************
*  [Title]        SlideCard
*  [Arguments]    xs,ys: source coords.
*						xd,yd: destinations coords.
*						CardId: Card to show
*  [ReturnVAL]    
*  [Description]  
*  Slide a card from a source position to other destination
******************************************************************************/
void SlideCard(int xs,int ys,int xd,int yd, byte CardId)
{	int spd=25; 		/* speed of movement in pixels/frame*/
	int xstp,ystp;
	
	if (abs(xd-xs)>abs(yd-ys)){  	/* Move X faster than Y */
		xstp=spd;
		ystp=1+abs(yd-ys)*xstp/abs(xd-xs);
		}	
	else {								/* Move Y faster than X */
		ystp=spd;
		xstp=1+abs(xd-xs)*ystp/abs(yd-ys);
		}
	if (xs>xd) xstp=-xstp;
	if (ys>yd) ystp=-ystp;

	do {
   	LibOpenWindow(xs,ys,18,28);
   	PrintCard(xs,ys,CardId);
   	LibPutDisp();
   	LibCloseWindow();
   	LibWait(IB_125MWAIT);
		if (abs(xd-xs)>abs(xstp)) 
			xs+=xstp;
		else
			xs=xd;	

		if (abs(yd-ys)>abs(ystp)) 
			ys+=ystp;
		else
			ys=yd;	
		} while ((xs!=xd)|(ys!=yd));	
}	
	
/******************************************************************************
*  [Title]        DispPlayerCards
*  [Arguments]    pl: player's number
*  [ReturnVAL]    
*  [Description]  
*  Show all existing cards of a player (pl).
******************************************************************************/
void DispPlayerCards(int pl)
{  int crd;

   switch (pl) {
      case PL_ME: /* My cards */
         /* First, clear the card's area */
         LibGdsClr(player[pl].card[0].coord[LEFT]-1,player[pl].card[0].coord[TOP]-1,player[pl].card[2].coord[RIGHT],player[pl].card[2].coord[BOTTOM]);
         /* Then draw the cards */
         for (crd=0;crd<CARDS_IN_PLAYER;crd++)         
            if (player[pl].card[crd].Id!=NOCARD) {
               PrintCard(player[pl].card[crd].coord[LEFT],player[pl].card[crd].coord[TOP],player[pl].card[crd].Id);
               /* Is the card selected? */
               if (player[pl].card[crd].select)
                  LibGdsReverse(player[pl].card[crd].coord[LEFT]-1,player[pl].card[crd].coord[TOP]-1,player[pl].card[crd].coord[RIGHT],player[pl].card[crd].coord[BOTTOM]);
               }
         break;
         
      case PL_RIGHT: /* Right Player      */
      case PL_LEFT:  /* Left Player       */
         /* First, clear the card's area  */
         LibGdsClr(player[pl].card[0].coord[LEFT],player[pl].card[0].coord[TOP],player[pl].card[2].coord[RIGHT],player[pl].card[2].coord[BOTTOM]);
         /* Then draw the cards */
         for (crd=0;crd<CARDS_IN_PLAYER;crd++)
            if (player[pl].card[crd].Id!=NOCARD)
            
/* The next lines are only for debug and see the opponent player cards */
#ifdef DEBUG 
             PrintCard(player[pl].card[crd].coord[LEFT],player[pl].card[crd].coord[TOP],player[pl].card[crd].Id); 
#else             
             PrintCard(player[pl].card[crd].coord[LEFT],player[pl].card[crd].coord[TOP],CARD_BACKL); 
#endif             
         break;
      }           
   LibPutDisp();  
}

/******************************************************************************
*  [Title]        DispTableCards
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Show all existing cards over the table.
******************************************************************************/
void DispTableCards()
{  int crd;

   /* First, clear area */
   LibGdsClr(TABLE0_L,TABLE0_T,TABLE11_R,TABLE11_B-1);
   /* Table's cards */
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      if (table[crd].Id!=NOCARD) {
         PrintCard(table[crd].coord[LEFT],table[crd].coord[TOP],table[crd].Id);  
         if (table[crd].select) 
            LibGdsReverse(table[crd].coord[LEFT]-1,table[crd].coord[TOP]-1,table[crd].coord[RIGHT],table[crd].coord[BOTTOM]);
         }
   LibPutDispBox(TABLE0_L,TABLE0_T,TABLE11_R-TABLE0_L,TABLE11_B-TABLE0_T);
}  
   
/******************************************************************************
*  [Title]        DispPlayerDeck
*  [Arguments]    pl: player's number
*  [ReturnVAL]    
*  [Description]  
*  Show the player's deck info
******************************************************************************/
void DispPlayerDeck (int pl)
{  char texto[4];	

   switch (pl) {
      case PL_ME:
         if (player[pl].ncards>0) {
           	PrintCard(DECK0_L,DECK0_T,CARD_BACKL);
           	LibPutDisp();
           	LibWait(IB_250MWAIT);
            PrintCard(DECK0_L+2,DECK0_T+2,CARD_BACKL);
            if (GameLevel<3) {   
            	sprintf(texto,"%2d/%d",player[pl].ncards,player[pl].escobas);
            	LibPut35Str(DECK0_L+5,DECK0_T+21,texto);		/* Cards & points */
            	}
            else
            	LibClrBox(DECK0_L,DECK0_T+21,28,5);
            }
         else
            LibGdsBoxMesh(DECK0_L,DECK0_T,DECK0_L+27,DECK0_T+17);
         break;
      case PL_RIGHT: 
         if (player[pl].ncards>0) {
         	PrintCard(DECK1_L,DECK1_T,CARD_BACKL);
  	         LibPutDisp();
     	      LibWait(IB_250MWAIT);            
         	PrintCard(DECK1_L+2,DECK1_T+2,CARD_BACKL);
            if (GameLevel<3) {   
   	         sprintf(texto,"%2d/%d",player[pl].ncards,player[pl].escobas);
      	      LibPut35Str(DECK1_L+5,DECK1_T+21,texto);		/* Cards & points */
      	      }
            else
            	LibClrBox(DECK1_L,DECK1_T+21,28,5);
            }
         else
            LibGdsBoxMesh(DECK1_L,DECK1_T,DECK1_L+27,DECK1_T+17);
         break;
      case PL_LEFT:  
         if (player[pl].ncards>0) {
        		PrintCard(DECK2_L,DECK2_T,CARD_BACKL);
           	LibPutDisp();
           	LibWait(IB_250MWAIT);
            PrintCard(DECK2_L+2,DECK2_T+2,CARD_BACKL);
            if (GameLevel<3) {   
	            sprintf(texto,"%2d/%d",player[pl].ncards,player[pl].escobas);
   	         LibPut35Str(DECK2_L+5,DECK2_T+21,texto);		/* Cards & points */
   	         }
            else
            	LibClrBox(DECK2_L,DECK2_T+21,28,5);
            }
         else
            LibGdsBoxMesh(DECK2_L,DECK2_T,DECK2_L+27,DECK2_T+17);
         break;
		}
   LibPutDisp();  
}     
   
   
/******************************************************************************
*  [Title]        DispTableDeck
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Show the main deck info
******************************************************************************/
void DispTableDeck()
{  char texto[4];
   /* Deck */
   LibClrBox(2,120,20,39);       /* Remove the deck */   
   if (deck.remain>0) {
      PrintCard(DECKT_L,DECKT_T,CARD_BACK);
      if (deck.remain>1)
      	PrintCard(DECKT_L+2,DECKT_T+2,CARD_BACK);
      sprintf(texto,"%2d",deck.remain);
      LibPutProStr(IB_CG57FONT,DECKT_L+5,DECKT_T-9,texto,14);
		}
	else
      LibGdsBoxMesh(DECKT_L,DECKT_T,DECKT_L+17,DECKT_T+27);		
   LibPutDispBox(2,120,20,39);  
}  


/******************************************************************************
*  [Title]        DispDiscardArea
*  [Arguments]    
*  [ReturnVAL]    
*  [Description]  
*  Show cards in the discard area
*  Meshes the selected cards and collect them 
******************************************************************************/
void DispDiscardArea(int pl)
{  int crd;					/* Aux. for loop											*/
   int crdcnt;				/* card counter in area									*/
   int xp;					/* Auxiliar for coordinates 							*/
   byte lastcrd=NOCARD;	/* card to draw then card when crdcntlect cards	*/
   bool plcards=FALSE;	/* some player card selected?							*/
   bool tbcards=FALSE;	/* Some table card selected?							*/
   char texto1[40];		/* Auxiliar for text number							*/
   char texto2[5];
   

   /* Clear area */
   LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1);
   crdcnt=0;

   /* Print player selected card and meshes the card  		*/
   /* Copy the selected card value to show in message below */
   for (crd=0;crd<CARDS_IN_PLAYER;crd++)
      if (player[pl].card[crd].select==TRUE) {
      	plcards=TRUE;
         LibGdsMesh(player[pl].card[crd].coord[LEFT]-1,player[pl].card[crd].coord[TOP]-1,player[pl].card[crd].coord[RIGHT],player[pl].card[crd].coord[BOTTOM]);            
         PrintCard(DISCARD_L+2,DISCARD_T+1,player[pl].card[crd].Id);
         LibPutDisp();
         if ((GameDemo==TRUE)|(GameLevel==1))
		   	sprintf(texto1,"%d",GetValue(player[pl].card[crd].Id));         
      	}        
   
   /* What's is the first position to show cards? */   
	if (plcards==TRUE) {	/* Position for selected table cards 	*/
		xp=DISCARD_L+25;	/* start position for the player card	*/
      if (GameDemo==TRUE | GameLevel==1) {
			LibOpenWindow(DISCARD_L,DISCARD_B+7,98,12);
			LibBox(DISCARD_L,DISCARD_B+7,97,11,1);
			LibLine(DISCARD_L+1,DISCARD_B+18,97,1,1);	/* Shadow */
			LibLine(DISCARD_L+97,DISCARD_B+8,1,11,1);		
   		LibPutProStr(IB_CG57FONT,DISCARD_L+2,DISCARD_B+9,texto1,95);
   		LibPutDisp();
   		}
   	}
	else
		xp=DISCARD_L+2;	/* start position if no player card selected (last cards at end game) */

   CheckHrdIcn(2);		/* Wait for 1 second */	
	/* Print table cards selected, if exist */	
   for (crd=0;crd<CARDS_IN_TABLE;crd++)
      if (table[crd].select==TRUE) {
      	tbcards=TRUE;
      	lastcrd=table[crd].Id; /* Card to show during slide cards */
         LibGdsMesh(table[crd].coord[LEFT]-1,table[crd].coord[TOP]-1,table[crd].coord[RIGHT],table[crd].coord[BOTTOM]);         
         PrintCard(xp,DISCARD_T+1,table[crd].Id); 
         if (plcards==TRUE & (GameDemo==TRUE |GameLevel==1)) {
		   	sprintf(texto2,"+%d",GetValue(table[crd].Id));
		   	strcat(texto1,texto2);
   			LibPutProStr(IB_CG57FONT,DISCARD_L+2,DISCARD_B+9,texto1,95);
   			}
         LibPutDisp();
         if (GameDemo==TRUE)
         	CheckHrdIcn(2);	/* Wait for 1 second */
         else	
         	CheckHrdIcn(1);	/* Wait for 0,5 seconds */
         	
         if (xp<DISCARD_R-21) xp+=10;	/* last position reached in the discard area?	*/
      	}

    if ((plcards==TRUE & tbcards==TRUE) & (GameDemo==TRUE | GameLevel==1)){
	  	strcat(texto1,"=15");
   	LibPutProStr(IB_CG57FONT,DISCARD_L+2,DISCARD_B+9,texto1,95);
   	LibPutDisp();
   	}   
   
   /* Collect cards with sliding */   
	if (tbcards==TRUE) {	/* Wait 2 second to see the combination and collect the cards */
		CheckHrdIcn(4);
   	for (xp=xp-10;xp>DISCARD_L+2;xp-=5) {	/* slide cards in area */
   		LibGdsClr(xp+5,DISCARD_T+1,DISCARD_R-2,DISCARD_B-1);
   		PrintCard(xp,DISCARD_T+1,lastcrd);   		
   		LibPutDispBox(DISCARD_L+1,DISCARD_T+1,DISCARD_R-DISCARD_L-2,DISCARD_B-DISCARD_T-1);
   		LibWait(IB_125MWAIT);
   		if ((plcards==FALSE)&(xp<DISCARD_L+15))
   			break;
   		}
	   LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1);
	   switch (pl){
	   	case PL_ME:	
	   		SlideCard(xp,DISCARD_T+1,DECK0_L,DECK0_T,CARD_BACK);
	   		break;
	   	case PL_RIGHT:	
	   		SlideCard(xp,DISCARD_T+1,DECK1_L,DECK1_T,CARD_BACK);
	   		break;
	   	case PL_LEFT:	
	   		SlideCard(xp,DISCARD_T+1,DECK2_L,DECK2_T,CARD_BACK);
	   		break;
   		}
   	}
   	if (plcards==TRUE & (GameDemo==TRUE |GameLevel==1))
   		LibCloseWindow();
	   LibGdsClr(DISCARD_L+1,DISCARD_T+1,DISCARD_R-1,DISCARD_B-1);
	   LibPutDisp();
}

/******************************************************************************
*  [Title]        ShowScores
*  [Arguments]    
*  [ReturnVAL]    the winner
*  [Description]  
*  Show the cards obtained for a player and returns the total points
******************************************************************************/
void ShowScores()
#define NOWIN  -1
#define xPos(pl)(pl==PL_ME?98:(pl==PL_RIGHT?116:80))  /* X column for player information */
{  TScore score[MAXPLAYERS];
   int pl;
   int crd;
   char texto[20];
   int win;
   int counter;
   int xWin=16;
   int yWin=84;
   int cxWin=16;
   int cyWin=6;

   /* Prepare scores window */
   LibPutFarData (xWin,yWin,109);
   LibPutGraph(xWin+58,yWin+4,Asigna);
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+20,GetMsg(10),120);  	/* cards 	*/
   LibPutDisp();
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+28,GetMsg(11),120);	/* Golds		*/
   LibPutDisp();
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+36,GetMsg(12),120);	/* 7's		*/    
   LibPutDisp();
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+44,GetMsg(13),120);	/* Gold 7	*/
   for (pl=0;pl<nPlayers;pl++)  											/* none for each player */
	   LibPutFarData(xPos(pl)+4,yWin+44,90);
   LibPutDisp();
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+52,GetMsg(14),120);	/* Escobas	*/
   LibPutDisp();
   LibPutProStr(IB_CG57FONT,xWin+10,yWin+63,GetMsg(15),120);	/* Total score */
   for (pl=0;pl<nPlayers;pl++) { /* for each player */
      sprintf(texto,"%2d",player[pl].escobas);
      LibPutProStr(IB_CG57FONT,xPos(pl),yWin+52,texto,20);     /* Escobas	*/ 	
      }
   LibPutDisp();
   CheckHrdIcn(2);
   
   /* Show cards & info for each player */
   for (pl=0;pl<nPlayers;pl++) { /* for each player */   
      /* History Cards window */
      LibOpenWindow (cxWin,cyWin,129,77);
      LibPutFarData (cxWin,cyWin,109); 
      switch (pl) {
         case PL_ME:
            strcpy(texto,GetMsg(7));
            break;
         case PL_RIGHT:
            strcpy(texto,GetMsg(8));
            break;   
         case PL_LEFT:
            strcpy(texto,GetMsg(9));
            break;   
         default:
            break;   
         }     
            
      LibPutProStr(IB_PFONT1,80-LibGetProStrSize(IB_PFONT1,texto)/2,cyWin+2,texto,120);
      LibPutDisp();
      
      /* Init player counters */
      score[pl].npoints=player[pl].escobas;
      score[pl].ncards=0;
      score[pl].ngolds=0;
      score[pl].nseven=0;
      score[pl].gold7=FALSE;
      
      for (crd=0;crd<player[pl].ncards;crd++) { /* for each card in history*/
         score[pl].ncards++;
         PrintCard(9*(crd%12)+cxWin+6,15*(crd/12)+cyWin+10,player[pl].history[crd]);
         sprintf(texto,"%2d",score[pl].ncards);
         LibPutProStr(IB_CG57FONT,xPos(pl),yWin+20,texto,20);
         if (GetSuit(player[pl].history[crd])==0) {         /* Check for gold */
            score[pl].ngolds++;
            sprintf(texto,"%2d",score[pl].ngolds);
            LibPutProStr(IB_CG57FONT,xPos(pl),yWin+28,texto,20);
            }           
         if (GetValue(player[pl].history[crd])==7) {        /* Check for seven */
            score[pl].nseven++;
            sprintf(texto,"%2d",score[pl].nseven);
            LibPutProStr(IB_CG57FONT,xPos(pl),yWin+36,texto,20);
            }
         if (player[pl].history[crd]==6) {                  /* Check for gold 7 */
            score[pl].gold7=TRUE;
            LibPutFarData(xPos(pl)+3,yWin+44,89);
            }
         LibPutDisp();
         LibWait(IB_250MWAIT);
      	}  /* end for each card in history */
      CheckHrdIcn(4);
      LibCloseWindow();    
      } /* end for each player */

   /* 7 gold winner */
   for (pl=0;pl<nPlayers;pl++)
      if (score[pl].gold7==TRUE) score[pl].npoints++;
      
   /* Cards winner */
   win=PL_ME;
   counter=score[PL_ME].ncards;
   for (pl=PL_RIGHT;pl<nPlayers;pl++) {
      if (score[pl].ncards==counter)   /* Draw. Nobody wins */
         win=NOWIN;
      if (score[pl].ncards>counter) {  /* pl is the winner*/
         win=pl;
         counter=score[pl].ncards;
         }     
      }
   if (win!=NOWIN) score[win].npoints++;  

   /* Golds winner */
   win=PL_ME;
   counter=score[PL_ME].ngolds;
   for (pl=PL_RIGHT;pl<nPlayers;pl++) {
      if (score[pl].ngolds==counter)   /* Draw. Nobody wins */
         win=NOWIN;
      if (score[pl].ngolds>counter) {  /* pl is the winner */
         win=pl;
         counter=score[pl].ngolds;
         }     
      }
   if (win!=NOWIN) score[win].npoints++;  

   /* 7's winner */
   win=PL_ME;
   counter=score[PL_ME].nseven;
   for (pl=PL_RIGHT;pl<nPlayers;pl++) {
      if (score[pl].nseven==counter)   /* Draw. Nobody wins */
         win=NOWIN;
      if (score[pl].nseven>counter) { 	/* pl is the winner */
         win=pl;
         counter=score[pl].nseven;
         }     
      }
   if (win!=NOWIN) score[win].npoints++;  

   /* Final scores */
   for (pl=0;pl<nPlayers;pl++) {
      sprintf(texto,"%2d",score[pl].npoints);
      LibPutProStr(IB_CG57FONT,xPos(pl),yWin+63,texto,20);
   	}
   LibPutDisp();
   WinMsgBox(GetMsg(5),ICON_OK,0);
}   

