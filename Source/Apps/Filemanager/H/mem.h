#ifndef MEMH
#define MEMH

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

/* Memory related copy procedures */

/* copys any far-data to any near-data
  Parameters :

  const void *source    source-data
  void far *dest        destination-data
  word len              length of data
*/
void memneartofarcpy(const void *source,void far *dest,word len);

/* copys any near-data to any far-data
  Parameters :

  const void far *source  source-data
  void *dest            destination-data
  word len              length of data
*/
void memfartonearcpy(const void far *source,void *dest,word len);

/* copys far data to far data

  Parameters :
  
  const void far *source  source-data
  void far *dest        destination-data
  word len              length of data
  
*/
void memfarcpy(const void far *source,void far *dest,word len);

/* fills the far data with value

  Parameters :

  const void far *ptr   destination pointer
  word len              length of data
  byte value            value to be filled into the data
*/
void memfarfill(const void far *ptr,word len, byte value);

#endif