DisplaySetup(){
	ClearScreen();
	Rahmen();
	DialogBox();
	DispKeyboard();
}

DispKeyboard(){
	LibPutFarData( 50, 84, 181);
	LibPutDisp();
}

Rahmen(){
	LibBox(0, 0, 160, 160, 3);
	
}

DialogBox(){
	LibPutFarData( 16, 5, 109);
	
}