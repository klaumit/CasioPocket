#ifdef __HITACHI__

#include    <hitachi.h>
#include    "libc.h"

#define FAR /* nothing */
#define NEAR /* nothing */
#define far_strcpy strcpy
#define far_strncpy strncpy
#define far_strcmp strcmp
#define far_strncmp strncmp
#define far_memmove memmove

#else

#include    <stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#define FAR far
#define NEAR near

#endif

typedef unsigned char uchar;

extern int sfLoadRecord(const uchar *signature,int size,byte *buffer);
extern int sfSaveRecord(const uchar *signature,int size,byte *buffer);
extern int sfRemoveRecord(const uchar *signature);
