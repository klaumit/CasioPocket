/******************************************************************************
    Author: Dirk Baumanns
    FREEWARE
    Attention: i'm not responsible for any damage when using this programm    
******************************************************************************/

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

#include "viewicon.h"

TCHTBL fullscreen[] =
{
  { 0,0,159,159,ACT_MAKE,0x8000,0 },
  { 0,0,0,0,0L,0,0 }
};

signed int x=0;

void show_icon(int x);

void main ()
{
 TCHSTS tsts; 
	LibInitDisp();		
 LibTchStackClr();
 LibTchStackPush(NULL);
 LibTchStackPush(TchHardIcon);
 LibTchStackPush(TchActionKey);
 LibTchStackPush(fullscreen);
 LibTchInit();
 LibStringDsp("Use Action-Key for + / -", 1, 50,160,IB_PFONT1);
 LibStringDsp("Use Action-Key push for +25", 1, 70,160,IB_PFONT1);
 LibStringDsp("Press ESC to quit", 1, 90,160,IB_PFONT1); 
 LibPutDisp();
 tsts.obj = 0;
 x=-1;
 while ((tsts.obj != OBJ_HIC_ESC) && (tsts.obj != OBJ_SYS_ALARM))
 {   
   LibTchWait( &tsts );
   switch(tsts.obj)
   {
      case OBJ_LPSW_UP :  x--;
                          if (x > 340) x=0;
                          if (x < 0) x=340;
			                        show_icon(x);
                          break;
      case OBJ_LPSW_DOWN : x++;
                           if (x > 340) x=0;
                           if (x < 0) x=340; 
                           show_icon(x);
                           break;                           
      case OBJ_LPSW_PUSH : x=x+25;
                           if (x > 340) x=0;
                           if (x < 0) x=340;
			                         show_icon(x);
                           break;    			 
      default : break;
   }   
 } 
	LibJumpMenu();
} /* main */

void show_icon(int x)
{
  byte buf15[15] = {0};
  itoa(x, buf15, 10); /* base 10 */
  LibClrDisp();
  LibStringDsp("No.", 1, 150,160,IB_PFONT1);
  LibStringDsp(buf15, 50, 150,160,IB_PFONT1);        
  LibPutFarData(0,0,x);
  LibPutDisp();
};