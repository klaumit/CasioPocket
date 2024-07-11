#ifdef __HITACHI__

#include    <hitachi.h>
#include    "Libc.h"

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

#include <string.h>
#include <stdio.h>

#define FILENAME "SHARED"

static byte mode=0;
static byte submode=0;

#ifdef __HITACHI__
static FILE_BUF fbuf;                       
#endif

typedef unsigned char uchar;

int sffindfile(FILE_BUF *fb)
{
  /* open or create shared data file */
  if (mode==0)
  {
    if (!LibGetAllEntry((byte *) FILENAME,&mode,&submode)) /* find shared file */
    {
      if (!LibSubEntrySave((byte *) FILENAME,&submode)) return FALSE;
      /* create new */
      LibGetAllEntry((byte *) FILENAME,&mode,&submode);/* get mode and submode */
    }
  }
  /* Set main and sub entry mode in fb*/
  fb->fsb_main_entry_ = mode;
  fb->fsb_sub_entry_  = submode;
  fb->fsb_scrt_info_  = 0x80;  /* Open Mode */
  return TRUE;
}
  
int sffindrecord(const uchar *signature,FILE_BUF *fb,FILE_INFO *fi)
{
  /* search named record and load it*/
  int found=FALSE;
  fi->fp = 0xffff;             /* first search */    
  fi->kind = FILE_KIND_BIN;    /* binary file */
  
  while ((!found) && (LibFileFindNext(fb,fi,0)))
  {
    if (!LibFileReadEx(fb,fi,3072/FILE_BLOCK_SIZE)) return FALSE; /* Read error */
    found = strcmp((char*)fb->fbuf.bin.bin_buf,(char*)signature)==0; /*compare signature*/
  }
  if (!found)
  {
    fi->fp = 0xffff;             /* prepare FILE_INFO for "write new" */
    fi->kind = FILE_KIND_BIN;
  }
  return found;
}

byte *sfgetdatapointer(FILE_BUF *fbuf,int *freebytes)
{
  /* pointer to real data */
  byte *ptr=fbuf->fbuf.bin.bin_buf;
  *freebytes=3*1024;
  while (*ptr!=0)
    {
      ptr++;   /* skip signature */
      (*freebytes)--;
    }
  ptr++;                   /* skip null byte */
  (*freebytes)--;
  return ptr;
}

int sfLoadRecord(char *signature,int size,byte *buffer)
{
  int i;
  int freebytes;
  FILE_INFO fi;
#ifndef __HITACHI__
  FILE_BUF fbuf;                    /* ! requires very much stack space ! */
#endif
  byte *sptr, *dptr;

  if (! sffindfile(&fbuf)) return FALSE;   /* find shared file */
  if (! sffindrecord(signature,&fbuf,&fi)) /* find and load record */   
    return FALSE;
  
  sptr = sfgetdatapointer(&fbuf,&freebytes);     /* get pointer to data */
  if (freebytes<size) return FALSE;              /* size too big error */
  dptr = buffer;
  for (i=0;i<size;i++)         /* move data to buffer */
    *(dptr++)=*(sptr++);
  return TRUE;
}

int sfSaveRecord(char *signature,int size,byte *buffer)
{
  int i;
  FILE_INFO fi;
#ifndef __HITACHI__
  FILE_BUF fbuf;                  /* requires very much stack space */
#endif
  byte *sptr,*dptr;
  int freebytes;

  if (!sffindfile(&fbuf)) return FALSE;      /* find or create shared file */
  sffindrecord(signature,&fbuf,&fi);         /* find record, but ignore error*/

  strcpy(fbuf.fbuf.bin.bin_buf,signature);   /* Write signature to filebuffer*/
  sptr=buffer;
  dptr=sfgetdatapointer(&fbuf,&freebytes);   /* get pointer to data */
  if (size>freebytes) return FALSE;          /* size too big error */

  for (i=0;i<size;i++)                       /* move data to filebuffer */
    *(dptr++)=*(sptr++);

  fbuf.fbuf.bin.char_num = dptr-fbuf.fbuf.bin.bin_buf; /* store size in filebuffer */

  return LibFileWrite(&fbuf,&fi);            /* write record */
}

int sfRemoveRecord(const uchar *signature)
{
  FILE_INFO fi;
#ifndef __HITACHI__
  FILE_BUF fbuf;                          /* requires very much stack space */
#endif  
  if (!sffindfile(&fbuf)) return FALSE;   /* find or create shared file */
  if (!sffindrecord(signature,&fbuf,&fi)) /* find record */
    return TRUE;                          /* ..no need to remove */
  
  return LibFileRemove(&fbuf,&fi);        /* remove it */
}

