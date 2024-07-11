/* Diese Programm gibt auf dem PV den Zeichensatz aus
email: Detlef.Ullrich@gmx.de */
#include <define.h>
#include <libc.h>
#include <l_libc.h>

void charprint ()
{
  byte i, h, z;
  TCHSTS tsts;
  LibInitDisp();                                                                                        
  LibLine(0, 10, 159, 1, 1);
  LibLine(10, 0, 1, 159, 1);
  for(i = 0; i < 10; ++i) LibPutProFont(IB_CG57FONT, 48+i,13+i*9, 1);
  for(i = 10; i < 16; ++i) LibPutProFont(IB_CG57FONT, 55+i,13+i*9, 1);
  for(i = 0; i < 10; ++i) LibPutProFont(IB_CG57FONT, 48+i, 3, 13+i*9);
  for(i = 10; i < 16; ++i) LibPutProFont(IB_CG57FONT, 55+i, 3,13+i*9);
  z=0;
  for(h = 13; h < 153; h=h+9) 
    for(i = 13; i < 153; i=i+9) {
      LibPutProFont(IB_CG57FONT, z, i, h);
    ++z;
    };
  LibPutDisp();         
  LibTchStackClr();
  LibTchStackPush(TchHardIcon);
  LibTchInit();                 
  for (;;) LibTchWait(&tsts);; 
                                                        
}