/*
 * palmfontex.c
 *
 * example Add-In to show usage of palmfont.c
 *
 * Author: J. Steingraeber
 * Date: 2002-06-23
 *
 * released to the public domain
 */

#include	<stdrom.h>
#include	"define.h"
#include	"l_define.h"
#include	"libc.h"
#include	"l_libc.h"

#include 	"palmfont.h"

extern unsigned char far font1[];
extern unsigned char far  font2[];

/*
 * printchartable
 *
 * print characters from pFont onto screen until screen is full
 *
 * pFont     pointer to palm font
 * y0        start y position
 *
 * returns   y position of next line
 */
unsigned int printchartable(PalmFontStruct far * pFont, unsigned int y0)
{
  unsigned int nFontHeight;
  unsigned char nFontWidth[256];
  unsigned int x, y;
  unsigned int n;
  unsigned char glyph[100]; /* graphics data buffer */

      /* get width of all characters into font width array
         for faster rendering */
  nFontHeight = GetFontWidth(pFont,nFontWidth);

      /* initialise x and y position */
  x = 0;
  y = y0;
  
  n = 0x20;

  while (n < 256) /* exit loop via break when last line is reached */
  {
    if (x + nFontWidth[n] >= 160)
    {
          /* enter next line */
      x = 0;
      y += nFontHeight;
      if (y > (160 - nFontHeight)) break; /* loop exit */
    }

        /* get bitmap for this character */
    if (GetPalmFontGlyph(pFont,n,glyph,sizeof(glyph)))
    {
          /* put it onto screen, allow overlapping characters */
      LibPutGraphO(x,y,glyph,IB_GPOR);
    }

        /* calculate next x position */
    x += nFontWidth[n];

        /* switch to next character */
    ++n;
  }

  return y + nFontHeight;
}

void main()
{
  TCHSTS tsts;
  PalmFontStruct far * pFont;
  unsigned int y0;

      /* clear screen */
  LibClrDisp();

      /* first font */
  pFont = (PalmFontStruct far *)font1;
      /* try to autodetect PDB file and correct pointer */
  pFont = TestForPalmDatabase((unsigned char far *)pFont);

      /* output font table */
  y0 = printchartable(pFont,0);

      /* second font */
  pFont = (PalmFontStruct far *)font2;
      /* try to autodetect PDB file and correct pointer */
  pFont = TestForPalmDatabase((unsigned char far *)pFont);

      /* output font table */
  printchartable(pFont,y0+8);

      /* update screen */
  LibPutDisp();

      /* prepare touch screen events */
  LibTchStackClr();
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchAllDsp);
  LibTchInit();

      /* wait for screen touch */
  LibTchWait(&tsts);

      /* exit program */
  LibJumpMenu();
}
