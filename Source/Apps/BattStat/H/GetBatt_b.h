/*
 * Module: GetBatt
 * Author: J. Steingraeber
 * Date: 2001-01-09                                                    
 * Changed: 2002-10-17 for adaption to HITACHI
 *
 * Released to the public domain.
 */

#ifndef GETBATT_H_INCLUDED
#define GETBATT_H_INCLUDED

/*
  the routine I found for checking the batterie is located at
  C000:06D1 and starts with the bytes E4 6A 0C 10
  luckily this routine is located at the same position for all PV models
  for PV-S1600 a similar routine is located at 8C02E35C
*/
#ifdef __HITACHI__

#define GETBATTADDR 0x8C02E35C
#define GETBATTTARGET 0x2FE62FD6

#else

#define GETBATTADDR 0xC00006D1
#define GETBATTTARGET 0x100C6AE4

#endif

/*
  GetBatt

  returns a 10 bit AD value (0..1023) for the battery voltage
  returns -1 if an error occured (system routine at C000:06D1 resp.
  0x8C02E35C not found)
*/
int GetBatt();

#endif
