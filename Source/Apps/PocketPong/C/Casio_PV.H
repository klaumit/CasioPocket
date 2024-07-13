#include <stdrom.h>

#include "define.h"
#include "libc.h"
#include "l_define.h"
#include "l_libc.h"

#define PUT_XOR IB_GPREV
#define PUT_OR IB_GPOR

#ifndef BYTE
#define BYTE unsigned char
#endif

#define EVENT_TCH       1
#define EVENT_XXX       2
#define EVENT_CRADLE    4
#define EVENT_BLD1      8
#define NOP_DUR 2

#ifndef OBJ_LPSW_LEFT
#define	OBJ_LPSW_LEFT		0x0016	/* LEFT	*/
#define	OBJ_LPSW_RIGHT		0x0017	/* RIGHT*/
#endif

#define OBJ_IC_PAD 0x9000

TCHSTS 	_tch;
int	_tablet_x, _tablet_y, _table_event, _table_button, _tablet_event, _touched, _newtouch, _music;

unsigned int _a[]  = {2000, 1000, 499, 249, 124,  62, 31};     /* A  */
unsigned int _ad[] = {1876,  938, 469, 234, 117,  59,  0};     /* A# */
unsigned int _b[]  = {1776,  888, 444, 222, 111,  55,  0};     /* B  */
unsigned int _c[]  = {   0, 1672, 836, 418, 209, 104, 52};     /* C  */
unsigned int _cd[] = {   0, 1476, 788, 394, 197,  98, 49};     /* C# */
unsigned int _d[]  = {   0, 1488, 744, 372, 186,  93, 46};     /* D  */
unsigned int _dd[] = {   0, 1412, 706, 353, 177,  88, 44};     /* D# */
unsigned int _e[]  = {   0, 1328, 664, 332, 166,  83, 41};     /* E  */
unsigned int _f[]  = {   0, 1256, 628, 314, 157,  78, 39};     /* F  */
unsigned int _fd[] = {   0, 1186, 593, 296, 147,  74, 37};     /* F# */
unsigned int _g[]  = {   0, 1120, 560, 280, 140,  70, 35};     /* G  */
unsigned int _gd[] = {   0, 1056, 528, 264, 132,  66, 33};     /* G# */

TCHTBL far tchPad[] = {
	 0, 0,159,159 ,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN |ACT_500MSEC,
	OBJ_IC_PAD,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

/*
char _asm_ci ();
char _asm_cci ();
#define inp (p)		_asm_ci ("\n\tIN\tAL, DX", _asm_ci, _asm_ci, _asm_ci, (p))
#define outp (p, c)	_asm_cci ("\n\tOUT\tDX, AL", (c), _asm_ci, _asm_ci, (p))
*/
#define outpsound(c) _asm_sound("\n OUT 00h,AL\n",c)
void _asm_sound(unsigned char *, unsigned char);

void Sound(unsigned int ptch, unsigned int dur, unsigned char wav)
{
	unsigned long cnt1;

  	if ( ! _music ) return;

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

void Note(char *n, unsigned int dur, unsigned char wav)
{
  if(n[0] == 'P')
  {
    Sound(0, dur, wav);
    return;
  }

  if(n[1] == '#')
    switch(n[0])
      {
      case 'A':
	Sound(_ad[n[2]-48], dur, wav);
	break;
      case 'C':
	Sound(_cd[n[2]-48], dur, wav);
	break;
      case 'D':
	Sound(_dd[n[2]-48], dur, wav);
	break;
      case 'F':
	Sound(_fd[n[2]-48], dur, wav);
	break;
      case 'G':
	Sound(_gd[n[2]-48], dur, wav);
	break;
      default:
	break;
      }
  else
    switch(n[0])
      {
      case 'A':
	Sound(_a[n[1]-48], dur, wav);
	break;
      case 'B':
	Sound(_b[n[1]-48], dur, wav);
	break;
      case 'C':
	Sound(_c[n[1]-48], dur, wav);
	break;
      case 'D':
	Sound(_d[n[1]-48], dur, wav);
	break;
      case 'E':
	Sound(_e[n[1]-48], dur, wav);
	break;
      case 'F':
	Sound(_f[n[1]-48], dur, wav);
	break;
      case 'G':
	Sound(_g[n[1]-48], dur, wav);
	break;
      default:
	break;
      }
}

void PollEvent(TCHSTS far* tsts, byte event_mask)
{
	union REGS reg;
	reg.x.ax = 0x0200 | event_mask;
	reg.x.di = FP_OFF(tsts);
	reg.x.es = FP_SEG(tsts);
	int86(0x50,&reg,&reg);
}

unsigned long GetTime( void )
{
  	word y;
  	byte m,d,h,min,sec;
	LibGetDateTime2(&y,&m,&d,&h,&min,&sec);
	return ((h % 4)*60+min)*60+sec;
}


void InitOS( void )
{
	_table_button = 0;
	_tablet_x = -1;
	_tablet_y = -1;
	_music = LibSoundGet() & IX_KEY_SOUND;
}

void PutSprite(int x, int y, const BYTE far *data)
{
	LibPutGraph( x, y, data );
}

void PutSpriteOP(int x, int y, BYTE far *data, int op)
{
	LibPutGraphO( x, y, data, op );
}

void PutPixel( int x, int y )
{
	LibDotOn( x, y );
}

void DelPixel( int x, int y )
{
	LibDotOff( x, y );
}

void ShowText( int x, int y, char *text )
{
	LibStringDsp( text, x, y, 160, 2 );
}

void Message( char *text )
{
	LibPutMsgDlg( text );
}

void ShowScreen( void )
{
	LibPutDisp();
}

void ClearScreen( void )
{
	LibClrDisp();
}

void TabletInit( void )
{
}

void TabletRead( void )
{
	PollEvent( &_tch, 0x0f );

	_table_event = _tch.ext;

	if (_tch.x != -1 && _tch.y != -1)
	{
		_tablet_x = _tch.x;
		_tablet_y = _tch.y;
		_tablet_event = 1;
		if (!_touched) _newtouch = 1;
		_touched = 1;
	}
	else
	{
		_tablet_event = 0;
		_touched = 0;
	}
}

int TabletWaitTouch( void )
{
	int i;
	_newtouch = 0;
	while (_newtouch == 0)
	{
		TabletRead();
		for(i=0;i<32000;i++)
		{
		}
	}
}

int TabletGetEventType( void )
{
	return _tablet_event;
}

int TabletGetX( void )
{
	return _tablet_x;
}

int TabletGetY( void )
{
	return _tablet_y;
}

int TabletIsTouched( void )
{
	return _touched;
}

int TabletGetEvent( void )
{
	return _table_event;
}

int TabletGetButton( void )
{
	return _table_button;
}

void CloseProgram( void )
{
	LibJumpMenu();
}

int far getleverpush();