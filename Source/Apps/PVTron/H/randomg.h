/* Routine for initalize randomgenerator */
static int GetSeed( void ) {
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

/* Routine for randomnumber generation */
void randomg(void)
{
	do
	{
		zufallzahl = rand();
	} 
	while((zufallzahl==0)||(zufallzahl<20));
	
/*	LibPutMsgDlg("Zufall= %i",zufallzahl);		Debugfunktion */
}