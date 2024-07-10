/******************************************************************************
	[Title]		SampleProgram

	Copyright (C) 1998 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#include	<stdrom.h>
#include	"define.h"
#include	"l_define.h"
#include	"libc.h"

#include "modehook.h"

/* global variable nCnt, just to show that
   is is avaiable when LateSaveHook is called */
unsigned int nCnt;

void drawscreen(char * title)
{
  char msg[80];

  LibClrDisp();
  
  LibPutProStr(IB_PFONT1,0,0,title,160);

  sprintf(msg,"Counter is %d.",nCnt);
  LibPutProStr(IB_PFONT1,0,10,msg,160);
}

void far LateSaveHook()
{
  drawscreen("LateSaveHook");
  LibPutDisp();
  LibWait(IB_1SWAIT);
}

void main()
{
  TCHSTS tsts;

  drawscreen("int mode hook example");

  setmodehook(&LateSaveHook);
  
  LibTchStackClr();
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  LibTchInit();

  nCnt = 0;
  
  while (1)
  {
    tsts.act = 0;
    
    LibPutDisp();
    LibTchWait(&tsts);

    if (tsts.act == ACT_MAKE)
    {
          /* count ACT_MAKE and display current value */
      ++nCnt;
      drawscreen("ACT_MAKE");
    }

        /* leave program when ESC is touched */
    if (tsts.obj == OBJ_HIC_ESC) break;
  }

      /* don't call LateSaveHook if we leave via ESC */
  releasemodehook();
  
  LibJumpMenu();
}
