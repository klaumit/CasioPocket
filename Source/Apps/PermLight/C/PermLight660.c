/******************************************************************************
	[Title]		PermLight660

        * Author: J. Steingraeber
        * Date: 2002-12-29
        *
        * released to the Public Domain

******************************************************************************/
#include	<stdrom.h>
#include	"define.h"
#include	"l_define.h"
#include	"libc.h"

/* extern unsigned long far hookaddr; */
extern unsigned char far permlighthook;
extern unsigned char far permlightjmpf;
extern unsigned char far permlighthook_end;

void _asm_cli(char *);
void _asm_sti(char *);
#define CLI() _asm_cli("\n CLI\n");
#define STI() _asm_sti("\n STI\n");

void main()
{
  if (((unsigned long far *)MK_FP(0xC400,0x15EA))[0] != 0x05FD06C6)
  {
    LibPutMsgDlg((byte*)"This is for\rPV-S460/S660 only");
  }
  else if (((unsigned long far *)MK_FP(0x0000,0x0204))[0] == (unsigned long)MK_FP(0x0000,FP_OFF(&permlighthook)))
  {
        /* hook is installed, uninstall it */
    CLI();
    ((unsigned long far *)MK_FP(0x0000,0x0204))[0] = ((unsigned long far *)MK_FP(0x0000,FP_OFF(&permlightjmpf)+1))[0];
    STI();
    LibPutMsgDlg((byte*)"PermLight uninstalled");
  }
  else
  {
        /* hook is not installed, install it now */
    CLI();
    far_memcpy(MK_FP(0x0000,FP_OFF(&permlighthook)),&permlighthook,&permlighthook_end - &permlighthook);
    ((unsigned long far *)MK_FP(0x0000,FP_OFF(&permlightjmpf)+1))[0] = ((unsigned long far *)MK_FP(0x0000,0x0204))[0];
    ((unsigned long far *)MK_FP(0x0000,0x0204))[0] = (unsigned long)MK_FP(0x0000,FP_OFF(&permlighthook));
    STI();
    LibPutMsgDlg((byte*)"PermLight installed");
  }
  LibJumpMenu();
}
