/*
 * modeldef.h
 *
 * common defines model dependent
 */

#ifndef modeldef_h_included
#define modeldef_h_included

#ifdef __HITACHI__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hitachi.h>
#include "libc.h"

#define FAR /* nothing */
#define NEAR /* nothing */
#define far_strcpy strcpy
#define far_strncpy strncpy
#define far_strcmp strcmp
#define far_strncmp strncmp
#define far_memcpy memcpy
#define far_strchr strchr
#define far_strrchr strrchr
#define far_memset memset
#define far_strlen strlen
#define far_strcat strcat

#else

#include <stdrom.h>
#include "libc.h"
#include "l_libc.h"

#define FAR far
#define NEAR near

#define IMB_FILE_OPEN_MODE FILE_OPEN_MODE
#define IMB_FILE_SECRET_MODE FILE_SECRET_MODE

#define MAX_FILENAMELEN 16

#endif

#endif
