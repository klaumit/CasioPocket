DisplaySetup(){

	ClearScreen();
LibPutMsgDlg3("LEVEL %i",lev);
LibPutMsgDlg3("LEVEL %i",lev);
LibPutMsgDlg3("LEVEL %i",lev);
	Rahmen();
}

Rahmen(){
LibGdsBox(0,0,160,134);	/* Rahmen */
LibPutGraph( 20,140, Links); /* Controller LEFT*/
LibPutGraph( 80,140, Rechts); /* Display RIGHT */
LibPutDisp();


}
