/* event mask defines */

#define EVENT_TCH      1
#define EVENT_CRADLE    4
#define EVENT_BLD1      8


void PollEvent(TCHSTS far* tsts, byte event_mask)
/*----------------------------------------------------------------------------
description : poll on events
              thanx to Wittawat Yamwong
parameters  : tsts      - pointer to touch status struct
              event_mask - specifies events to poll
return      : %
----------------------------------------------------------------------------*/
{
  union REGS reg;
  
  reg.x.ax = 0x0200 | event_mask;
  reg.x.di = FP_OFF(tsts);
  reg.x.es = FP_SEG(tsts);
int86(0x50,&reg,&reg);
}


Testbutton(){
TCHSTS tsts;
int posx;
int posy;
int pause;

PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE); 

posx=tsts.x;
posy=tsts.y;

if (posx==-1 && posy==-1) tpressed=0;

if (tpressed==0)
{
if (posx>=80 && posy>=140 && posx<=140 && posy<=160) /* RIGHT */
{

	if (direction==1) direction=2;
else
	if (direction==0) direction=1;
else
	if (direction==2) direction=3;
else
	if (direction==3) direction=0;
knack();
tpressed=1;
}

if (posx>=20 && posy>=140 && posx<=80 && posy<=160) /* LEFT */
{
	if (direction==0) direction=3;
else
	if (direction==1) direction=0;
else
	if (direction==2) direction=1;
else
	if (direction==3) direction=2;

knack();
tpressed=1;
}

if (posx>=0 && posy>=0 && posx<=160 && posy<=134) /* Pause */
{
	knack();
	pause=1;
	while (pause==1)
	{
		PollEvent(&tsts, EVENT_TCH | EVENT_CRADLE);
		posx=tsts.x;
		posy=tsts.y;
		if (posx>=0 && posy>=134 && posx<=160 && posy<=160) /* Pause */
		{
		knack();
		break;
		}
		else
		{
		pause=1;
		}
	}
}

if (posy>170)
{
dead=1;
knack();
}
}
}

