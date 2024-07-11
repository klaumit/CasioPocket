/******************************************************************************
*	
*	[Title]	        Test4 for SoundLib - Noise example
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

/* Test4 - Noise example */
void main(void)
{
  int cnt1,cnt2;

  /* Fall... */
  for (cnt1=50; cnt1<300; cnt1+=10)
    for (cnt2 = 20; cnt2>-20; cnt2-=10)
      sound(cnt1+cnt2, 900, 7);

  /* ...Explosion */
  for (cnt1=150; cnt1<250; cnt1+=1)
    noise(cnt1, 2000, 2);

  LibJumpMenu();
}

/* IMPORTANT!:                         *
 * -------------------                 *
 *  Parameters usage:                  *
 * -------------------                 *
 *   29 < ptch < 250                   *
 *   dur >= 1000                       *
 *   wav = 1 || 2 || 3                 *
 * -------------------                 */






