/******************************************************************************
*	
*	[Title]	        Test3 for SoundLib - Sound Effect
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver0.01
*
*	[Author]        2001 - Alessandro Dorigatti (FreeSoftware)
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include        "soundlib.h"

/* Test3 - Sound Effect */
void main(void)
{
  int cnt1, cnt2;

  for (cnt1=150; cnt1>30; cnt1-=10)
    for (cnt2=cnt1+100; cnt2>cnt1; cnt2-=5)
      sound(cnt2, 500, 10);

  LibJumpMenu();
}




