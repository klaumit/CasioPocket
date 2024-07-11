/*
 * Program: BattStat
 * Author: J. Steingraeber
 * Date: 2001-01-04                                                    
 *
 * V0.2a 2001-01-08 Sebastian Schildt
 * Released to the public domain.
 *
 * This is an example program using the build in routine for getting
 * the battery status on a PV-S250, PV-S450, PV-250X, PV-450X or
 * PV750(+)
 */

#include <stdrom.h>
#include "define.h"
#include "libc.h"

#include "GetBatt.h"
#include "gui.h"

/*
   DrawPowerBar

   checks existance of system routine and prints out some warning if
   the system routine can not be found
   
   else draws a bar reflecting the actual batterie value (0..1023) and
   marks the "battery low" value, which is a fixed limit build into
   the PVOS

   whenever the actual battery value is less than the limit PVOS
   will activate the battery low warning message
*/

int debugpow=1023;
unsigned int n;



/*
  fullscreen

  TCHTBL which mainly is here to have an object which is triggered by
  the 500msec action
*/
static TCHTBL fullscreen[] =
{
  { 0,0,159,159,ACT_500MSEC,0x8000,0 },
  { 0,0,0,0,0L,0,0 }
};

/*
  main

  prints out some info and handles touchpad input
*/
void BattStat()
{
  TCHSTS tsts;
  
      /* Clear Screen */
  LibInitDisp();

      /* print some text */
  /*LibStringDsp("BattStat V0.2a by S. Schildt",0,1,160,IB_PFONT2);
  LibStringDsp("Original by J. Steingraeber",0,10,160,IB_PFONT1);*/
  /*LibStringDsp("",0,19,160,IB_PFONT1);*/

  LibStringDsp("Press ESC to exit",70,152,160,IB_PFONT1);

  drawOnInit(); 


  LibTchStackClr();
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(fullscreen);
  LibTchInit();
  
  tsts.obj = 0;
  
    
  while ((tsts.obj != OBJ_HIC_ESC) && (tsts.obj != OBJ_SYS_ALARM))
  {
    n = GetBatt();
    /* DEBUG ZEUCH */
    /*n=700;*/
    /*n=debugpow;
    debugpow-=5;*/
    if (n == -1)
    {
      /* error: unexpected code */
     LibStringDsp("Sorry, can't find system call",0,20,160,IB_PFONT1);
     LibStringDsp("for getting battery status.",0,29,160,IB_PFONT1);
    }
    else {
       displayPower(&n);
    }
    LibPutDisp();
    LibTchWait( &tsts );
  }

  LibJumpMenu();
}
