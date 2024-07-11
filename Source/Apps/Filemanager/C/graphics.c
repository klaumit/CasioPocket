#include "graphics.h"

void Draw3DBox(byte xorg,byte yorg, byte xsize, byte ysize)
{
  LibGdsBox(xorg, yorg, xorg+xsize-1, yorg+ysize-1);
  
  LibLine(xorg+1,yorg+ysize,xsize, 1, 1);
  LibLine(xorg+xsize, yorg+1, 1, ysize,1);
}

void PutSelection(byte xpos, byte ypos, byte xwidth, byte anz, byte selected)
{
  byte element;
  for (element=0;element <anz;element++) {
    if (element==selected)
      LibPutFarData(xpos+element*xwidth,ypos,12);
    else
      LibPutFarData(xpos+element*xwidth,ypos,13);      
  }
 
}
