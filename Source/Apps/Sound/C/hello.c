void sound(void);

void main ()
{
	LibInitDisp();										/* Clear Screen */
	LibStringDsp("Hello World!!!",5,10,160,IB_PFONT2);		/* Text festlegen der Ausgegeben werden soll */
	LibStringDsp("von Michael Denzler",5,40,160,IB_PFONT2);
	LibStringDsp("http://www.pocketviewer.de",5,60,160,IB_PFONT2);
	LibStringDsp("mdn@pocketviewer.de",5,70,160,IB_PFONT2);
	LibPutDisp();						
	sound();
	/* Text auf Display legen */
	LibWait (IB_1SWAIT);									/* Warte 1 Sekunde */
	LibWait (IB_1SWAIT);									/* Warte nochmal 1 Sekunde */
	LibWait (IB_1SWAIT);									/* Warte nochmal 1 Sekunde */
	LibWait (IB_1SWAIT);									/* Warte nochmal 1 Sekunde */
	LibWait (IB_1SWAIT);									/* Warte nochmal 1 Sekunde */
	LibWait (IB_1SWAIT);									/* Warte nochmal 1 Sekunde */
	LibJumpMenu();											/* Springe ins Menu zurück, ohne diesen Rücksprung stürzt das OS ab */
}