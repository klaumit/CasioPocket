void knack(void)
{
int l=0;
	if ( LibSoundGet() & IX_KEY_SOUND )
	{
    	LibBuzzerOn(IB_BEEP0_SET);
    	for (l=0;l<200;l++);
    	LibBuzzerOff();
	}
}
