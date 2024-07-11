Auswertung(){
	if (a-2 == 0) tipp=e;
	if (a-2 == 2) tipp=z*1+e*10;
	if (a-2 == 4) tipp=h*1+z*10+e*100;
	LibPutMsgDlg3("Du denkst an die %i ?",tipp);
	LibPutMsgDlg3("Du denkst an die %i ?",tipp);

	if (tipp < zufallzahl) kleiner();
	if (tipp > zufallzahl) groesser();	
	if (tipp == zufallzahl) sieg();			
}