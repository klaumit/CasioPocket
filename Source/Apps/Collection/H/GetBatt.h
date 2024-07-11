/*
 * Module: GetBatt
 * Author: J. Steingraeber
 * Date: 2001-01-09                                                    
 *
 * Released to the public domain.
 */

#ifndef GETBATT_H_INCLUDED
#define GETBATT_H_INCLUDED

/*
  the routine I found for checking the batterie is located at
  C000:06D1 and starts with the bytes E4 6A 0C 10
  CB is the RETF instruction just one byte befor our routine
  luckily this routine is located at the same position for all PV models
*/
#define GETBATTSEG 0xC000
#define GETBATTOFS 0x06D1
#define GETBATTTARGET {0xCB, 0xE4, 0x6A, 0x0C, 0x10}

/*
  GetBatt

  returns a 10 bit AD value (0..1023) for the battery voltage
  returns -1 if an error occured (system routine at C000:06D1 not found)
*/
int GetBatt();

#endif
