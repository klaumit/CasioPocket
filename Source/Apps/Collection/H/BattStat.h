/*
 * Program: BattStat
 * Author: J. Steingraeber
 * Date: 2001-01-04                                                    
 *
 * Released to the public domain.
 */

#ifndef EXP_DEF
#define EXP_DEF

/*
  the routine I found for checking the batterie is locatet at
  C000:06D1 and starts the bytes E4 6A 0C 10
  CB is the RETF instruction just one byte befor our routine
*/
#define GETBATTSEG 0xC000
#define GETBATTOFS 0x06D1
#define TARGETTEST {0xCB, 0xE4, 0x6A, 0x0C, 0x10}

#endif
