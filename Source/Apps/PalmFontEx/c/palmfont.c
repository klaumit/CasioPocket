/*
 * palmfont.c
 *
 * Author: J. Steingraeber
 * Date: 2002-06-23
 *
 * released to the public domain
 *
 */

#include	<stdrom.h>

#include "palmfont.h"

unsigned int _asm_swaplohi(char *,unsigned int);
#define SWAPLOHI(n) _asm_swaplohi("\n XCHG AL,AH\n",n)

/*
 * GetPalmFontGlyph
 *
 * extract bitmap information of one glyph out of palm font data
 *
 * pFont     pointer to palm font
 * nChar     ascii code of character to extract
 * pData     pointer to data buffer which should receive the PV graphic
 * nDataLen  size of data buffer in bytes
 *
 * returns   0 in case of any error
 *             (contents of pData invalid and maybe altered)
 *           1 otherwise
 */
int GetPalmFontGlyph(PalmFontStruct far * pFont, unsigned int nChar,
                     unsigned char far * pData, unsigned int nDataLen)
{
  unsigned char far * pImage;
  unsigned int far * pLocation;
  unsigned int wFirstChar;
  unsigned int wLastChar;
  unsigned int wHeight;
  unsigned int wBytesPerRow;
  unsigned int nLocOffset;
  unsigned int wWidth;
  unsigned int x, y;
  
  if ((pFont == NULL) || (pData == NULL) || (nDataLen == 0))
  {
    return 0;
  }
  
  wFirstChar = SWAPLOHI(pFont->firstChar);
  
  wLastChar = SWAPLOHI(pFont->lastChar);

  if ((wFirstChar > 255) || (wLastChar > 255) || (nChar > 255))
  {
    return 0;
  }

  wHeight = SWAPLOHI(pFont->fRectHeight);
  wBytesPerRow = SWAPLOHI(pFont->rowWords) * 2;
  
  pImage = (unsigned char far *)pFont;
  pImage += 26;

  pLocation = (unsigned int far *)pImage;
  pLocation += wHeight * (wBytesPerRow / 2);

  if ((nChar < wFirstChar) || (nChar > wLastChar))
  {
        /* use default char */
    nLocOffset = wLastChar - wFirstChar + 1;
  }
  else
  {
    nLocOffset = nChar - wFirstChar;
    if (pLocation[nLocOffset+1 ] == pLocation[nLocOffset])
    {
          /* empty char: use default char */
      nLocOffset = wLastChar - wFirstChar + 1;
    }
  }

  wWidth = SWAPLOHI(pLocation[nLocOffset+1]) - SWAPLOHI(pLocation[nLocOffset]);
  
  nLocOffset = SWAPLOHI(pLocation[nLocOffset]);

      /* test if we have enough space in pData */
  if ((((wWidth + 7) / 8 * wHeight) + 4) > nDataLen)
  {
    return 0;
  }

      /* set x and y dim */
  ((unsigned int far *)pData)[0] = wWidth;
  ((unsigned int far *)pData)[1] = wHeight;

  pData += 4;
  pImage += nLocOffset / 8;

  for (y=0;y<wHeight;++y)
  {
    if (nLocOffset % 8)
    {
      for (x=0;x<(wWidth+7)/8;++x)
      {
        pData[x] = pImage[x] << (nLocOffset % 8);
        pData[x] |= pImage[x+1] >> (8 - (nLocOffset % 8));
      }
      pData += (wWidth + 7) / 8;
      pImage += wBytesPerRow;
    }
    else
    {
          /* simple case: image data at byte boundary */
      for (x=0;x<(wWidth+7)/8;++x)
      {
        pData[x] = pImage[x];
      }
      pData += (wWidth + 7) / 8;
      pImage += wBytesPerRow;
    }
  }
  
  return 1;
}

/* GetFontWidth
 *
 * calculate width of all characters in font and store them in an array
 *
 * pFont    pointer to PalmFontStruct
 * pWidth   pointer to array to receive width of characters
 *          must be large enough to hold 256 bytes
 *
 * returns  height of font
 */
unsigned int GetFontWidth(PalmFontStruct far * pFont,
                          unsigned char far * pWidth)
{
  unsigned char far * pImage;
  unsigned int far * pLocation;
  unsigned int wFirstChar;
  unsigned int wLastChar;
  unsigned int wHeight;
  unsigned int wBytesPerRow;
  unsigned int nLocOffset;
  unsigned int wWidth;
  unsigned int nChar;
  
  wFirstChar = SWAPLOHI(pFont->firstChar);
  
  wLastChar = SWAPLOHI(pFont->lastChar);

  wHeight = SWAPLOHI(pFont->fRectHeight);
  wBytesPerRow = SWAPLOHI(pFont->rowWords) * 2;
  
  pImage = (unsigned char far *)pFont;
  pImage += 26;

  pLocation = (unsigned int far *)pImage;
  pLocation += wHeight * (wBytesPerRow / 2);

  for (nChar=0;nChar<256;++nChar)
  {
    if ((nChar < wFirstChar) || (nChar > wLastChar))
    {
          /* use default char */
      nLocOffset = wLastChar - wFirstChar + 1;
    }
    else
    {
      nLocOffset = nChar - wFirstChar;
      if (pLocation[nLocOffset+1 ] == pLocation[nLocOffset])
      {
            /* empty char: use default char */
        nLocOffset = wLastChar - wFirstChar + 1;
      }
    }

    wWidth = SWAPLOHI(pLocation[nLocOffset+1]) - SWAPLOHI(pLocation[nLocOffset]);

    pWidth[nChar] = wWidth;
  }

  return SWAPLOHI(pFont->ascent); /* this may result in overlapping glyphs */
/* return wHeight; /* use this to ensure non overlapping glyphs */
}

/*
 * TestForPalmDatabase
 *
 * test if pData points to a Palm database (.pdb) with a font as
 * it's first entry
 *
 * pData     pointer to PDB
 *
 * returns   pData if not a database or no font at first position
 *           corrected pData (pointing to font data) otherwise
 */
void far * TestForPalmDatabase(unsigned char far * pData)
{
  char type[8];
  unsigned long nRecOffs;
  unsigned int far * p;
  
  far_memcpy(type,&pData[0x3C],4);
  type[4] = 0;
  if (stricmp(type,"FONT") == 0)
  {
        /* OK, it's a Palm Database file with a Font in it */
    p = (int far *)&pData[0x4E];
    nRecOffs = SWAPLOHI(p[0]);
    nRecOffs = nRecOffs << 16;
    nRecOffs += SWAPLOHI(p[1]);
    return &pData[nRecOffs];
  }

  return pData;
}
