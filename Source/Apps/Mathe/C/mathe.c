/*
*	
*	[Mathetrainer]	
*
*	[Model] 	PocketViewer
*
*	[Version]	Ver1.91
*
*	Copyright (C) 2001 Thomas Gaertner   thomas.gaertner@t-online.de 
*******************************************************************************************/

#include "mathe.h"

int main(){
	LibPutMsgDlg("Erics Mathetrainer\r\r    (C) 2001 by\r\rThomas Gaertner\r\r\r\r\rTouch Screen...");

	
	do {	level=level+1;
		LibPutMsgDlg("Berechne die folgenden\r\r%i Aufgaben im\r\r%i. Level\r\rso schnell Du kannst.\r\r\rTouch Screen...",gesaufgaben,level);

		startzeit=GetTime();
	  /*LibPutMsgDlg("zeit= %i",startzeit);*/
	
		for (l=1;l<=gesaufgaben;l++) {
			srand( GetSeed());
			verkehrt=FALSE;
			DisplaySetup();
			LibPutDisp();
			Aufgabe();
			} 
		LibPutMsgDlg("  Du brauchtest \r\r  %i Sekunden im\r\r  %i. Level\r\r  und hattest\r\r   %i Fehlversuche.",(GetTime()-startzeit),level,versuche-(gesaufgaben*level));
	} while (!gameover);
	
	LibPutMsgDlg(" \r GAME OVER !\r\r Du warst im\r\r %i. Level !",level);
	LibJumpMenu();
}



Auswertung(){
	if (a-2 == 0) tipp=e;
	if (a-2 == 2) tipp=z*1+e*10;
	if (a-2 == 4) tipp=h*1+z*10+e*100;
	
	if (tipp == ergebnis) richtig=TRUE;	
	else {verkehrt=TRUE;gameover=TRUE;}

	return;		
}



ClearScreen(){
	LibInitDisp();
	LibPutDisp();
}



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



Aufgabe(){
	int erste;
	int zweite;
	char first[4]="    ";
	char second[4]="    ";
	int digits=4;
	

	if (randomg(100)>50) operation=1;	/* Subtraktion */
	else operation=0;			/* Addition */
	/*LibPutMsgDlg("Op= %i",operation);*/
	
	do erste=randomg(level*50+50);
	while ((operation==1)&&(erste<20));
	
	/*LibPutMsgDlg("erste= %i",erste);*/
	
	do zweite=randomg(level*20+60);
	while ((operation==1)&&(zweite>erste));

	/*LibPutMsgDlg("zweite= %i",zweite);*/

	if (operation) ergebnis=erste-zweite;
	else ergebnis =erste+zweite;

	if (ergebnis>999) {
		LibPutMsgDlg(" \r GAME OVER !\r\r Du hast im\r\r %i. Level\r\r gewonnen !",level);
		LibJumpMenu();
		}  /*  max. 3stellige Eingabe erreicht*/

	/*LibPutMsgDlg("erg= %i",ergebnis);*/

	do {
	VarReset();
	if (verkehrt==TRUE) {
		LibBuzzerOn(IB_BEEP1_SET);
		for (e=0;e<1000;e++);
		LibBuzzerOff();
		}
	
	LibPutProStr(IB_PFONT1,20,60,"                     ",120);
	LibPutDisp();
	LibStringDsp("Loese folgende",40,20,150,IB_PFONT2);
	LibStringDsp("Aufgabe :",50,30,150,IB_PFONT2);
	if (erste<1000) digits=3;
	if (erste<100) digits=2;
	if (erste<10) digits=1;
	LibNumToStr(first,erste,digits);
	LibPutProStr(IB_PFONT1,23,60,first,20);
	if (operation) LibPutProStr(IB_PFONT1,44,60,"-",7);
	else LibPutProStr(IB_PFONT1,44,60,"+",7);
	
	if (zweite<1000) digits=3;
	if (zweite<100) digits=2;
	if (zweite<10) digits=1;
	LibNumToStr(second,zweite,digits);
	LibPutProStr(IB_PFONT1,53,60,second,20);
	LibPutProStr(IB_PFONT1,75,60,"=",7);
	LibPutDisp();
	versuche=versuche+1;
	ZahlenEingabe();
	Auswertung();
	} while (!richtig);

	
}

onscreen(char key){
	/*if (key == -72)	LibPutMsgDlg2("DEL");
	else
	LibPutMsgDlg2("%i",key);*/
}

void VarReset () {
	e=0;z=0;h=0;a=0;sec=0;eingabe=0;exit=0;richtig=FALSE;
	}

rechne(int eingabe){

	char out[3]="   ";

	a=a+1;
	if (a == 1) 	{ 
	e = eingabe;
	LibNumToStr(out,e,1);
	LibPutProStr(IB_PFONT2,110,60,out,8);
	LibPutDisp();
	if (e == 0) { sec = 1; }
			}
	
	if (a == 3) 	{
	
	z = eingabe;
	LibNumToStr(out,z,1);
	LibPutProStr(IB_PFONT2,115,60,out,8);
	LibPutDisp();
	if (z == 0) { sec = 1; }
			}
	
	if (a == 5) 	{
	
	h = eingabe;
	LibNumToStr(out,h,1);
	LibPutProStr(IB_PFONT2,120,60,out,8);
	LibPutDisp();
			}
}

ZahlenEingabe(){
	TCHSTS tsts;
	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( Tastiera );
	LibTchInit();
	LibRepOff();	

	for(;;){
		LibTchWait( &tsts );
		switch(tsts.obj){
	/*******TASTEN************************/
	case OBJ_1:
		rechne(1);	
		onscreen(1);
		break;
	case OBJ_2:
		rechne(2);
		onscreen(2);
		break;
	case OBJ_3:
		rechne(3);
		onscreen(3);
		break;
	case OBJ_4:
		rechne(4);
		onscreen(4);
		break;
	case OBJ_5:
		rechne(5);
		onscreen(5);
		break;
	case OBJ_6:
		rechne(6);
		onscreen(6);
		break;
	case OBJ_7:
		rechne(7);
		onscreen(7);
		break;
	case OBJ_8:
		rechne(8);
		onscreen(8);
		break;
	case OBJ_9:
		rechne(9);
		onscreen(9);
		break;
	case OBJ_0:
		rechne(0);
		onscreen(0);
		break;
	case OBJ_links:
		break;
	case OBJ_rechts:
		break;
	case OBJ_del:
                /*onscreen("DEL");  */
		a=a-1;/*LibPutMsgDlg("a=%i",a);*/
		if (a == 1) LibPutProStr(IB_PFONT2,110,60," ",8);
		if (a == 3) LibPutProStr(IB_PFONT2,115,60,"   ",8);
		if (a == 5) LibPutProStr(IB_PFONT2,120,60,"     ",8);
		LibPutDisp();
		break;
	case OBJ_next:
		exit = 1;
		break;
		}
		if (exit==1)
		break;
	}
}

/* Routine for initalize randomgenerator */
/*static */
int GetSeed( void ) {
	int   s;
	long  hh, mm, ss;
	byte  stime[7];
	LibGetDateTimeM( stime );
	hh = (long)( (10*(stime[1]&0x0F) + (stime[2]&0x0F)) * 3600 );
	mm = (long)( (10*(stime[3]&0x0F) + (stime[4]&0x0F)) * 60 );
	ss = (long)( (10*(stime[5]&0x0F) + (stime[6]&0x0F)) );
	s = (int)( (hh+mm+ss)%32768 );
	return( s );
}

int GetTime( void ) {
        
	byte hhh;
	byte mmm;
	byte sss;
	
	LibGetTime2(&hhh,&mmm,&sss);
	
	/*LibPutMsgDlg("h=%i",hhh);
	LibPutMsgDlg("m=%i",mmm);
	LibPutMsgDlg("s=%i",sss);*/
	
	return( hhh*3600+mmm*60+sss );
}

/* Routine for randomnumber generation */
int randomg(int max)
{
	do
	{
		zufallzahl = rand();
		zufallzahl = zufallzahl + 2;
	} 
        while((zufallzahl<(level*5))||(zufallzahl>max));
	return (zufallzahl);
	/*LibPutMsgDlg("Zufall= %i",zufallzahl);		Debugfunktion */
}

