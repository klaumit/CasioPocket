/*
 * Module: GetBatt
 * Author: J. Steingraeber
 * Date: 2001-01-09                                                    
 *
 * Released to the public domain.
 *
 * Get the battery status of a PV-S250, PV-S450, PV-250X, PV-450X or
 * PV750(+)
 */

#include <stdrom.h>

#include "GetBatt.h"

/*
  GetBatt

  returns a 10 bit AD value (0..1023) for the battery voltage
  returns -1 if an error occured (system routine at C000:06D1 not found)
*/
int GetBatt()
{
  unsigned char test[] = GETBATTTARGET;
  unsigned int (far * GetBattValue)();
  unsigned char far * p;
  int i;
  
      /* test if GetBattValue will reach expected code */
  p = MK_FP(GETBATTSEG,GETBATTOFS-1);
  for (i=0;i<sizeof(test);++i)
  {
    if (p[i] != test[i]) break;
  }
    
  if (i != sizeof(test))
  {
        /* error: unexpected code */
    return -1;
  }
  else
  {
    GetBattValue = MK_FP(GETBATTSEG,GETBATTOFS);
    return GetBattValue();
  }
}
