/*
 * palmfont.h
 *
 * Author: J. Steingraeber
 * Date: 2002-06-23
 *
 * released to the public domain
 *
 */

#ifndef palmfont_h_included
#define palmfont_h_included

typedef struct tagPalmFontStruct {
  unsigned int fontType;     /* font type */
  unsigned int firstChar;    /* ASCII code of first character */
  unsigned int lastChar;     /* ASCII code of last character */
  unsigned int maxWidth;     /* maximum character width */
  unsigned int kernMax;      /* negative of maximum character kern */
  unsigned int nDescent;     /* negative of descent */
  unsigned int fRectWidth;   /* width of font rectangle */
  unsigned int fRectHeight;  /* height of font rectangle */
  unsigned int owTLoc;       /* offset to offset/width table */
  unsigned int ascent;       /* ascent */
  unsigned int descent;      /* descent */
  unsigned int leading;      /* leading */
  unsigned int rowWords;     /* row width of bit image / 2 */
} PalmFontStruct;

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
                     unsigned char far * pData, unsigned int nDataLen);

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
                          unsigned char far * pWidth);

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
void far * TestForPalmDatabase(unsigned char far * pData);

#endif /* ifndef palmfont_h_included */
