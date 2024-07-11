#ifndef GRAPHICSH
#define GRAPHICSH
#include <define.h>
#include <libc.h>

/* Graphic procedures of any type */

/* draws a box like this
      ************
      *          **
      *          **
      *************
       ************ 

  Parameters :

  byte xorg         x-coordinate of upper left corner
  byte yorg         y coordinate of upper left corner
  byte xsize        size of the box in x-direction
  byte ysize        size of the box in y-direction
 
*/
void Draw3DBox(byte xorg,byte yorg, byte xsize, byte ysize);

/* draws a series of radio buttons (8x8 pixel)
  
  Parameters :

  byte xpos        x-coordinate of first radio button
  byte ypos        y-coordinate of first radio button
  byte xwidth      indicates the distance between two radio buttons
  byte anz         number of radio buttons
  byte selected    indicates which radio button is selected
*/
void PutSelection(byte xpos, byte ypos, byte xwidth, byte anz, byte selected);

#endif