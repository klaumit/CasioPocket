/******************************************************************************
*	
*	[Title]	        Test1 for SoundLib - Chords
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

/* Test1 - Chords */
void main(void)
{
  int cnt;

  /* Fast "ARPEGGIO" => "CHORD"!! */


  /* => CMaj                      */
  for (cnt=0; cnt<30; cnt++)
    {
      note("C4", 800, 7);
      note("E4", 800, 7);
      note("G4", 800, 7);
    }

  /* => FMaj */
  for (cnt=0; cnt<30; cnt++)
    {
      note("C4", 800, 7);
      note("F4", 800, 7);
      note("A4", 800, 7);
    }

  /* => GMaj */
  for (cnt=0; cnt<30; cnt++)
    {
      note("D4", 800, 7);
      note("G4", 800, 7);
      note("B4", 800, 7);
    }

  /* => AMin */
  for (cnt=0; cnt<30; cnt++)
    {
      note("E4", 800, 7);
      note("A4", 800, 7);
      note("C5", 800, 7);
    }

  LibJumpMenu();
}




