/* trm.c */
#include "stdio.h"
#include "define.h"
#include "libc.h"
#include "trm.h"

#define TRMCHARS (TRMLINES*TRMCOLS)

unsigned char trmbuffer[TRMCHARS];
unsigned char nflag[TRMLINES];
byte firstline;
byte trmsize;
byte cursorline, cursorcol;
byte spline, spcol; /* place to store current cursor position */



byte far GraphUP[] = /* Icon data1 UP */
{
  GSIZE(8, 16),
  0x18,0x18,0x3C,0x3C,0x7E,0x7E,0xFF,0xFF,
  0x18,0x18,0x18,0x18,0x18,0xFF,0xFF,0xFF
};

byte far GraphDOWN[] = /* Icon data2 DOWN */
{
  GSIZE(8, 16),
  0xFF,0xFF,0xFF,0xFF,0x18,0x18,0x18,0x18,
  0xFF,0xFF,0x7E,0x7E,0x3C,0x3C,0x18,0x18
};

static TCHTBL far TchRollBtn[] = {
  151, 20, 159, 36, ACT_ICON, OBJ_IC_UP, 0x0000, /* Icon UP */
  151, 54, 159, 70, ACT_ICON, OBJ_IC_DOWN, 0x0000, /* Icon DOWN */
};

static T_ICON far TiconUP= {&TchRollBtn[0],GraphUP,NULL,0x00};
static T_ICON far TiconDOWN= {&TchRollBtn[1],GraphDOWN,NULL,0x00};

void NFlags()
{
  /* mark all lines as NEW */
  int i;
  for (i=0;i<TRMLINES;i++) nflag[i]=TRUE;
}

void ModeTrm(int mode)
{
  /* switch mode and set globals */
  switch (mode)
    {
    case M_SHORT:
      trmsize=9;
      break;
    case M_LONG:
      trmsize=17;
      break;
    }
  LibClrBox(0,0,160,trmsize*LINEDIST+4);
  LibBox(0,0,LINECHARS*CHARWIDTH+4,trmsize*LINEDIST+4,1);
  LibIconPrint(&TiconUP); /* IconUP display*/
  LibIconPrint(&TiconDOWN); /* IconDOWN display*/
  if (cursorline-firstline+1>=trmsize)
    {
      NFlags();
      firstline=cursorline-trmsize+1; 
    }
  LibPutDisp();
  NFlags();
}

void DisplayTrm()
{
  int i, j;
  int line;
  for (i=0;i<trmsize;i++)
    {
      line=i+firstline;
      if (nflag[line]) /* is line changed ? */
	{
	  LibClrBox(2,i*LINEDIST+2,LINECHARS*CHARWIDTH,LINEDIST);
	  for (j=0; j<LINECHARS; j++)
	    LibPutProFont(IB_CG57FONT, /* Font */
              trmbuffer[line*TRMCOLS+j],
			  j*CHARWIDTH+2,i*LINEDIST+2);
	  nflag[line]=FALSE;
	}
    }
  LibPutDisp();
}

void InitTrm(void)
{
  int i;
  for (i=0;i<TRMCHARS;i++) trmbuffer[i]=' ';
  NFlags();
  firstline=0;
  cursorline=0;
  cursorcol=0;
  ModeTrm(M_SHORT); /* 9 lines; M_LONG = 17 lines */
  DisplayTrm();
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchRollBtn);
}

byte hex(byte c)
{
  byte hchar[16]={'0','1','2','3','4','5','6','7',
		  '8','9','A','B','C','D','E','F'};
  return hchar[c %16];
}

void cursornext()
{
  cursorcol++;
  if (cursorcol>=TRMCOLS) { cursorline++; cursorcol=0; }
}

void cursorprev()
{
  if (cursorcol>0) cursorcol--;
  else
    {
      if (cursorline>0) { cursorline--; cursorcol=TRMCOLS-1; }
    }
}

void rollup()
{
  int i;
  cursorline--;
  for (i=0;i<(TRMLINES-1)*TRMCOLS;i++) trmbuffer[i]=trmbuffer[i+TRMCOLS];
  NFlags();
  for (;i<TRMCHARS;i++) trmbuffer[i]=' ';
  if (spline>0) spline--; /* correct saved position */
}

void GotoTrm(byte col,byte line)
{
  cursorline=line;
  cursorcol=col;
  if (cursorline-firstline>=trmsize)
    {
      NFlags();
      firstline=cursorline-trmsize+1; 
    }
}
	
void OutTrm(unsigned char c)
{
  int i;
  switch(c)
    {
    case 0x0d:
    case '\n':
    case 0xF0: /* CRLF */
      cursorline++; cursorcol=0;
      break;
    case 0x0c:
	for (i=0;i<TRMCHARS;i++) trmbuffer[i]=' ';
	NFlags();
	firstline=0;
	cursorline=0;
	cursorcol=0;
     break;
    case 0xF1: /* DEL */
      cursorprev();
      trmbuffer[cursorline*TRMCOLS+cursorcol]=' ';
      nflag[cursorline]=TRUE;
      if (cursorline<firstline) { firstline=cursorline; NFlags(); }
      break;
    case 0xF2: /* deleol */
      for (i=cursorcol;i<TRMCOLS;i++) trmbuffer[cursorline*TRMCOLS+i]=' ';
      nflag[cursorline]=TRUE;
      break;
    case 0xF4: /* space */
      c=' ';    /* ! must run through to default */
    default:
      trmbuffer[cursorline*TRMCOLS+cursorcol]=c;
      nflag[cursorline]=TRUE;
      cursornext();
    }
  if (cursorline>=TRMLINES) rollup();
  if (cursorline-firstline+1>=trmsize)
    {
      NFlags();
      firstline=cursorline-trmsize+1; 
    }
}

void InputTrm(unsigned char *ib,int cnr)
{
  unsigned char *hp;
  byte aline, acol;
  int ready;
  int cnt;
  TCHSTS tsts;
  byte kycd;
  LibKeyInit();
  LibDispKey();
  ready=FALSE;
  ModeTrm(M_SHORT);
  spline=cursorline;
  spcol=cursorcol;
  while (!ready)
    {
      DisplayTrm();
      kycd=LibGetKeyM(&tsts); /* get key or other events */
      if (kycd!=KEY_NONE)     /* any Key */
	{
	  if (kycd==0xF0) ready=TRUE;
	  else OutTrm(kycd);
	}
      else 
	{                     /* no key */
	  switch (tsts.obj)
	    {
	    case OBJ_IC_DOWN:   /* roll up */
	      if (LibIconClick(&TiconUP, &tsts) == TRUE)
		{
		  if (firstline<TRMLINES) firstline++;
		  NFlags();
		}
	      break;
	    case OBJ_IC_UP: /* roll down */
	      if (LibIconClick(&TiconDOWN, &tsts) == TRUE)
		{
		  if (firstline>0) firstline--;
		  NFlags();
		}
	      break;
	    case OBJ_HIC_ESC:
	      ready=TRUE;     /* ESC pressed */
	      break;
	    } /* switch (tsts.obj) */
	} /* if (kycd!=KEY_NONE) */
    } /* while (!ready) */

  acol=cursorcol; cursorcol=spcol;
  aline=cursorline; cursorline=spline; /* start position */
  cnt=0;
  hp=ib;
  while ( (aline>cursorline)||
	     ((aline==cursorline)&&(acol>cursorcol))
	   )
	{
	if (cnt<cnr) *(hp++)=trmbuffer[cursorline*TRMCOLS+cursorcol];
	cnt++;
	cursornext();
     }

  *(hp++)=0;
  OutTrm(0xF0);
  ModeTrm(M_LONG);
  DisplayTrm(); /* update display */
}

void OutputTrm(unsigned char *ob)
{
  int i;
  i=0;
  while (ob[i]!=0) OutTrm(ob[i++]); /* output all chars */
  DisplayTrm();    /* update display */
}

void OutputITrm(int i)
{
  byte c[7];
  if (i<0) {c[0]='-'; i=-i;} else c[0]=' ';
  LibNumToStr(&(c[1]),i,5);
  c[6]=0;
  OutputTrm(c);
}

