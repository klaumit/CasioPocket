/******************************************************************************
*	
*	[Title]	        SoundLib
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver0.04
*
*	[Author]        2001 - Alessandro Dorigatti (Free Software)
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include        "soundlib.h"

#define  NOP_DUR  2

/* Note Table ------------------------------------------------------*/
unsigned int a[]  = {2000, 1000, 499, 249, 124,  62, 31};     /* A  */
unsigned int ad[] = {1876,  938, 469, 234, 117,  59,  0};     /* A# */
unsigned int b[]  = {1776,  888, 444, 222, 111,  55,  0};     /* B  */
unsigned int c[]  = {   0, 1672, 836, 418, 209, 104, 52};     /* C  */
unsigned int cd[] = {   0, 1476, 788, 394, 197,  98, 49};     /* C# */
unsigned int d[]  = {   0, 1488, 744, 372, 186,  93, 46};     /* D  */
unsigned int dd[] = {   0, 1412, 706, 353, 177,  88, 44};     /* D# */
unsigned int e[]  = {   0, 1328, 664, 332, 166,  83, 41};     /* E  */
unsigned int f[]  = {   0, 1256, 628, 314, 157,  78, 39};     /* F  */
unsigned int fd[] = {   0, 1186, 593, 296, 147,  74, 37};     /* F# */
unsigned int g[]  = {   0, 1120, 560, 280, 140,  70, 35};     /* G  */
unsigned int gd[] = {   0, 1056, 528, 264, 132,  66, 33};     /* G# */
/*------------------------------------------------------------------*/

/* Functions definition  -------------------------------------------*/

/* Play a SOUND: ptch  : wavelength (=> low pitch : high frequency) *
 * ------------  dur   :  duration                                  *
 *               wav   : 'waveform' (use wav<ptch/2)                */
void sound(unsigned int ptch, unsigned int dur, unsigned char wav)
{
  unsigned long cnt1;

  
  if(ptch>0)
    {
      for(cnt1=0; cnt1<dur; cnt1++)
	{
	  if(cnt1%ptch==0)
	    outpsound(0x80);
	  
	  if(cnt1%ptch==wav)
	    outpsound(0x00);
	}
      
      outpsound(0x00);
    }
  else
    for(cnt1=0; cnt1<dur*NOP_DUR; cnt1++);
}

/* Output a NOISE: ptch  : "wavelength" (use 29<ptch<250)               *
 * --------------  dur   :  duration    (use dur>=1000)                 *
 *                 wav   : 'waveform'   (use wav = 1,2,3)               */

void noise(unsigned int ptch, unsigned int dur, unsigned char wav)
{
  unsigned long cnt1;

  if(ptch>29)
    {
      for(cnt1=0; cnt1<dur/200; cnt1++)
	sound(rand()%(ptch-29)+30, 200, wav);
    }
  else
    for(cnt1=0; cnt1<dur*NOP_DUR; cnt1++);
}

/* Play a NOTE: *n   :  string like "A#5", "D3" or "P" => PAUSE!  *
 * -----------   dur :  duration                                  *
 *               wav : 'waveform' (use wav<ptch/2)                */
void note(char *n, unsigned int dur, unsigned char wav)
{
  if(n[0] == 'P')
  {
    sound(0, dur, wav);
    return;
  }

  if(n[1] == '#')
    switch(n[0])
      {
      case 'A':
	sound(ad[n[2]-48], dur, wav);
	break;
      case 'C':
	sound(cd[n[2]-48], dur, wav);
	break;
      case 'D':
	sound(dd[n[2]-48], dur, wav);
	break;
      case 'F':
	sound(fd[n[2]-48], dur, wav);
	break;
      case 'G':
	sound(gd[n[2]-48], dur, wav);
	break;
      default:
	break;
      }
  else
    switch(n[0])
      {
      case 'A':
	sound(a[n[1]-48], dur, wav);
	break;
      case 'B':
	sound(b[n[1]-48], dur, wav);
	break;
      case 'C':
	sound(c[n[1]-48], dur, wav);
	break;
      case 'D':
	sound(d[n[1]-48], dur, wav);
	break;
      case 'E':
	sound(e[n[1]-48], dur, wav);
	break;
      case 'F':
	sound(f[n[1]-48], dur, wav);
	break;
      case 'G':
	sound(g[n[1]-48], dur, wav);
	break;
      default:
	break;
      }
}





















