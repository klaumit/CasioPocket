/*
 * Module: GetBatt
 * Author: J. Steingraeber
 * Date: 2001-01-09
 * Changed: 2002-10-17 for adaption to HITACHI
 *
 * Released to the public domain.
 *
 * Get the battery status of a PV-S250, PV-S450, PV-250X, PV-450X or
 * PV750(+)
 * PV-S1600
 */

#ifdef __HITACHI__

#include <hitachi.h>
#include "libc.h"

#define FAR /* nothing */
#define NEAR /* nothing */
#define far_strcpy strcpy
#define far_strncpy strncpy
#define far_strcmp strcmp
#define far_strncmp strncmp

#else

#include <stdrom.h>
#include "libc.h"
#include "l_libc.h"

#define FAR far
#define NEAR near

#endif

#include "GetBatt.h"

/*
  GetBatt

  returns a 10 bit AD value (0..1023) for the battery voltage
  returns -1 if an error occured (system routine at C000:06D1 resp.
  0x8C02E35C not found)
*/
int GetBatt()
{
  union
  {
    unsigned int (FAR * GetBattValue)();
    unsigned long FAR * p;
  } u;
  
      /* test if GetBattValue will reach expected code */
  u.p = (unsigned long FAR *)GETBATTADDR;
  if (*u.p != GETBATTTARGET)
  {
        /* error: unexpected code */
    return -1;
  }
  else
  {
    return u.GetBattValue();
  }
}
