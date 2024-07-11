/******************************************************************************
*	
*	[PV Tron]	
*
*	[Model] 	PocketViewer PV-S660
*
*	[Version]	Ver1.00
*
*	Copyright (C) 2002 Michael Denzler - http://www.pocketviewer.de
******************************************************************************/

#include "includes.h"

int main(){
	ClearScreen();
	LibPutGraph( 0, 0, SplahScreen ); /* Center mark display */
	LibPutDisp();
	LibWait(IB_1SWAIT);
	LibWait(IB_1SWAIT);
while (exit==0)
	{
	DisplaySetup();
	gamestart();
	}
	LibJumpMenu();
}


gamestart(){
int xpos=60;
int ypos=80;
int aixpos=100;
int aiypos=80;
int cnt=0;
int dummy=0;
dead=0;
aidead=0;
direction=0;
aidirection=0;



LibDotOn(xpos, ypos); /* Set human player on screen */
LibDotOn(aixpos, aiypos); /* Set AI player on screen */
LibPutDisp();


while (dead==0)
	{

for (cnt=0; cnt<=level;cnt++)
	{
dummy=cnt;
	}

	playai(xpos, ypos, aixpos,aiypos);

	Testbutton();
	leverpush();

if (direction==0)	/* up */
	{
	ypos--;
	dead=TestDot(xpos,ypos);
	LibDotOn(xpos, ypos);
	LibPutDisp();
	score++;
	}

if (direction==2)	/* down */
	{
	ypos++;
	dead=TestDot(xpos,ypos);
	LibDotOn(xpos, ypos);
	LibPutDisp();
	score++;
	}

if (direction==3)	/* left */
	{
	xpos--;
	dead=TestDot(xpos,ypos);
	LibDotOn(xpos, ypos);
	LibPutDisp();
	score++;
	}

if (direction==1)	/* right */
	{
	xpos++;
	dead=TestDot(xpos,ypos);
	LibDotOn(xpos, ypos);
	LibPutDisp();
	score++;
	}

if (aidirection==0)	/* up */
	{
	aiypos--;
	aidead=TestDot(aixpos,aiypos);
	if (aidead==1) dead=aidead;
	LibDotOn(aixpos, aiypos);
	LibPutDisp();
	}

if (aidirection==1)	/* down */
	{
	aixpos++;
	aidead=TestDot(aixpos,aiypos);
	if (aidead==1) dead=aidead;
	LibDotOn(aixpos, aiypos);
	LibPutDisp();
	}

if (aidirection==2)	/* left */
	{
	aiypos++;
	aidead=TestDot(aixpos,aiypos);
	if (aidead==1) dead=aidead;
	LibDotOn(aixpos, aiypos);
	LibPutDisp();
	}

if (aidirection==3)	/* right */
	{
	aixpos--;
	aidead=TestDot(aixpos,aiypos);
	if (aidead==1) dead=aidead;
	LibDotOn(aixpos, aiypos);
	LibPutDisp();
	}


	}

if ( LibSoundGet() & IX_KEY_SOUND )
{
sound();
}
if (aidead==1)
{
LibPutMsgDlg3("Computer lost!");
LibPutMsgDlg3("Computer lost!");
LibPutMsgDlg3("Computer lost!");
level=level-1000;
lev++;
if (level<=0) exit=1;
}
else 
{
LibPutMsgDlg3("You Lost!");
LibPutMsgDlg3("You Lost!");
LibPutMsgDlg3("You Lost!");
exit=1;
}
LibPutMsgDlg3("Your SCORE: %i",score);
LibPutMsgDlg3("Your SCORE: %i",score);
LibPutMsgDlg3("Your SCORE: %i",score);
LibPutMsgDlg3("Your SCORE: %i",score);
if (aidead!=1) LibPutGraph( 0, 0, GameOver );
LibPutDisp();
LibWait(IB_1SWAIT);
	ClearScreen();

}