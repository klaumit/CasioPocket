/******************************************************************************
	[Title]		ReplMenu Program

	Author: J. Steingraeber
	Date: 2006-04-01

	Released to the Public Domain
******************************************************************************/
#include	<stdrom.h>
#include	"define.h"
#include	"l_define.h"
#include	"libc.h"
#include	"l_libc.h"

extern unsigned long far newintCA;
extern unsigned char far hookend;
extern unsigned long far orgintCAvec;
extern unsigned short far mode;

/*
 * TestInstalled
 *
 * test if hook is installed already
 *
 * returns 0 if not installed
 *         mode started by installed hook
 *         -1 if installation impossible (memory not empty)
 */
unsigned short TestInstalled()
{
  unsigned long far * p;
  unsigned long far * q;
  unsigned long ul;
  unsigned int n;
  
      /* destination address of hook routine */
  p = MK_FP(0x0011,0x0000); /* aka 0000:0150 */

      /* test if memory is "empty" */
  ul = p[0];
  n = FP_OFF(&hookend) / 4;
  while (--n)
  {
    if (p[n] != ul)
    {
      break;
    }
  }

  if (n)
  {
        /* memory not "empty", test for our own code */
    q = &newintCA;
    if (far_memcmp(p,q,FP_OFF(&mode)) == 0)
    {
          /* return installed mode */
      return ((unsigned short far *)MK_FP(0x0011,FP_OFF(&mode)+1))[0];
    }
    
    return -1;
  }
  
  return 0;
}

/*
 * RemoveHook
 *
 * remove hook routine from memory and restore original vector
 *
 * return 1 if routine was removed
 *        0 if an error occured (bad vector)
 */
int RemoveHook()
{
  unsigned long far * p;
  unsigned long far * q;

  q = MK_FP(0x0000,0x0328);
  
  if (q[0] != 0x00110000)
  {
    return 0;
  }

      /* restore vector */
  p = MK_FP(0x0011,FP_OFF(&orgintCAvec)+1);
  q[0] = p[0];
  
      /* destination address of hook routine */
  p = MK_FP(0x0011,0x0000); /* aka 0000:0150 */

  far_memset(p,0,FP_OFF(&hookend));

  return 1;
}

void InstallHook(unsigned short nMode)
{
  unsigned long far * p;
  unsigned long far * q;
  unsigned short far * s;
  
  p = MK_FP(0x0011,0x0000); /* aka 0000:0150 */
  q = &newintCA;
  
  far_memcpy(p,q,FP_OFF(&hookend));

  p = MK_FP(0x0000,0x0328);
  q = MK_FP(0x0011,FP_OFF(&orgintCAvec)+1);

      /* store original vector inside of hook code */
  q[0] = p[0];
      /* set new vector */
  p[0] = 0x00110000;

      /* set mode */
  s = MK_FP(0x0011,FP_OFF(&mode)+1);
  s[0] = nMode;
}

unsigned short SelectAddIn(unsigned short nMode)
{
  unsigned short nResult = -1;

  byte rtv[16+1] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0xff};
  byte cmnt[300];
  byte np;
  SLW2_ST win_prm;
  byte sel_ret;
  byte name[16];
  byte sub_mode;
  int k;
  
  np = 0;
  k = 0;
  cmnt[0] = 0;

      /* populate list */
  if (nMode)
  {
    strcpy(cmnt,"[remove]");
    ++k;
  }

  for (sub_mode=1;sub_mode<16;++sub_mode)
  {
    name[0] = 0;
    if (LibGetProgramName(name,IB_MADDIN,sub_mode)
        && (name[0]))
    {
      if (cmnt[0])
      {
        strcat(cmnt,"\376");
      }
      rtv[k] = sub_mode;
      strcat(cmnt,name);

      if (sub_mode == (byte)nMode)
      {
        np = k;
      }
      
      ++k;
    }
  }
    
  rtv[k] = 0xFF;

  if (!k)
  {
    return -1;
  }
  
      /* The selection window processing */
  win_prm.x = 9;
  win_prm.y = 0;
  win_prm.xs = 142;
  win_prm.ity = 9;
  win_prm.np = np;
  win_prm.cmnt = cmnt;
  win_prm.rtv = rtv;
  win_prm.t_xs = 3;
  
  sel_ret = LibSelWindowExt(&win_prm);

  if (sel_ret == 255)
  {
    return -1;
  }

  nResult = sel_ret;
  if (nResult)
  {
    nResult += IW_MADDIN;
  }
  
  return nResult;
}

void main()
{
  unsigned short nMode;

  if (FP_OFF(&newintCA))
  {
    LibPutMsgDlg("Compile Error\r\rASM code at\rwrong address.");
    LibJumpMenu();
  }

  nMode = TestInstalled();

  if (nMode == -1)
  {
    LibPutMsgDlg("Error\r\rmemory area not available.");
    LibJumpMenu();
  }

  nMode = SelectAddIn(nMode);

  if (nMode == 0)
  {
    if (!RemoveHook())
    {
      LibPutMsgDlg("Error\r\rRemoval failed.");
    }
  }
  else if (nMode != -1)
  {
    RemoveHook(); /* InstallHook expects clean int vectors */
    InstallHook(nMode);
    LibPutMsgDlg("ReplMenu installed.");
  }

  LibJumpMenu();
}
