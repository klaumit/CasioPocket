/******************************************************************************
*	[Titel]		Chimpa                                                            *
*	[Model] 	PocketViewer PV-250X & PV-450X                                    *
*	[Version]	Ver 1.00	 				                                                *
*	Copyright (C)   2001 Joan Jurado			                                      *
* joan@aceri.com                                                              *
******************************************************************************/

#include <stdrom.h>

#include "define.h"
#include "libc.h"
#include "L_define.h"
#include "L_libc.h"

#include "chimpa.h"

#define _PHOTOGRAM_WIDTH 100
#define _PHOTOGRAM_HEIGHT 80
#define _NUM_PHOTOGRAMS  62 
#define _LONG_HEADER_PHOTOGRAM 4
#define _PIXELS_PER_BYTE 8

void main()
{	
	int NumPhoto;			
  long LengthPhoto = _PHOTOGRAM_WIDTH * _PHOTOGRAM_HEIGHT / _PIXELS_PER_BYTE + _LONG_HEADER_PHOTOGRAM +40;

	LibClrDisp();		
  for(NumPhoto=0; NumPhoto< _NUM_PHOTOGRAMS; NumPhoto++) 
  {
	  LibPutGraph( 0,0, &Bild[LengthPhoto *NumPhoto] );
	  LibPutDisp();
	  LibWait (IB_125MWAIT);
  }

	LibWait (IB_1SWAIT);

  LibJumpMenu();
}
