/*
 * Module: utility functions
 * Author: S. Schildt
 * Date: 2001-01-09                                                    
 *
 * 15.01.2001 isLightOn() added
 *
 * 2003-01-15 J.St.
 * updated for PV-S1600 (version unchanged)
 *
 * Released to the public domain.
 *
 */
 
#include "modeldef.h"

#include "utils.h"


/* Returns voltage. 256 => 2.56 V */
int ADToVoltage(int *AD) {
	int volts;
	volts = *AD/3.089;
	return volts;	
}

int ScaleAD(int* AD,int target) {
	int n = *AD;
	/* Should be 372.0 / target, but since AD 927=100% all values ranging up from
	 * 276 must be = target (3.31-3.0V = 100% = target*/ 
	float f = 276.0/target;
	f = (n-651)/f;
	n=f;
	if (n>target) { n=target; }
	return n;
}


/*Convert number to string. Only numbers < 9999*/
void nr2str(int* number, byte * target) {
	int digits=4;
	/*If number to big */
	if (*number > 10000) {
		strcpy((char*)target,"?");
		return;	
	}	
	if (*number < 1000) { digits=3; }; 	
	if (*number < 100 ) { digits=2; }; 
	if (*number < 10  ) { digits=1; };
	LibNumToStr(target,*number,digits);
	target[digits]=0;
}

/* Returns TRUE if the backlight is avtive */
bool isLightOn() {
	byte is = LibGetEL();
	if (is == IB_ELP_OFF) {
		/*LibPutMsgDlg("Light off");*/
		return FALSE;	
	}
	else {
		/*LibPutMsgDlg("Light on");*/
		return TRUE;	
	}
}
