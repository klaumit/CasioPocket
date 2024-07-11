/******************************************************************************
*	
*	[Zahlenraten]	
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.00
*
*	Copyright (C) 2000 Michael Denzler - http://www.pocketviewer.de
******************************************************************************/

#include "includes.h"

int main(){
	srand( GetSeed() );
	randomg();
	Anleitung();
	gamestart();
	LibJumpMenu();
}

gamestart(){
	DisplaySetup();
	Titel();
	Testbutton();
	Auswertung();
}