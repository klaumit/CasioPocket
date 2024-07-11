/******************************************************************************
*	
*	[Title]	
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.00
*
*	Copyright (C) 1997 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#include	<stdrom.h>
#include	"libc.h"
#include	"L_libc.h"


extern unsigned char counter;
                              /* Die ASSEMBLER-ROUTINEN für den INT-83h-Patch */
extern far setintvec();       /* von Johannes Steingräber - DANKEEEE!!!!      */
extern far resetintvec();     /* natürlich nur gelinkt, sind in intproc.a86   */
                    
extern char pic0[3200];       /* Von mir erweiter für die Grafikroutinen */
extern char pic1[3200];         
extern char pic2[3200];

void main() {

int i,x,y;
  setintvec();

/* ein paar rechtecke zeichnen */
  for (x=0;x<8;x++) {
   for(y=0;y<8;y++) {
     if (x&1) { pic0[y*20+x]=255; }
     if (x&2) { pic1[y*20+x]=255; }
     if (x&4) { pic2[y*20+x]=255; }
   }
  }
     
/* 10 sekunden warten */
  for (i=0;i<10;i++) {    
    LibWait (IB_1SWAIT);
  }

  resetintvec();
  LibJumpMenu();
}