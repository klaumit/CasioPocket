/******************************************************************************
*       
*       [Title] 	Franka
*       [Model]         PocketViewer
*       [Version]       Ver1.20
*       [Copyright]	2000 Frank Tusche All rights reserved.
*
******************************************************************************/

#include        <stdrom.h>
#include	<math.h>

#include        "define.h"
#include        "libc.h"
#include        "L_define.h"
#include        "L_libc.h"

#include        "Franka.h"

#ifndef DUMMY_H
#define DUMMY_H
char _osmajor='1';
void *sbrk(size_t s) {return(NULL); };
#endif

/* main loop */
void frankTR(void) {
	byte nr[2];
	byte o;
	bool touch;
	TCHSTS  tsts;

	init();

	LibTchInit();
	for(;;){
		showDisplay();
		if (touch = nextCommand())
			tsts.obj = command.key;
		else
			LibTchWait( &tsts );
		touch = !touch;
		switch( tsts.obj ) {
		case  OBJ_IC_MC:                       /* [MC] */
			if (touch && !LibIconClick(&iMC,&tsts))
				break;
			addCmd(tsts.obj, 0);
			memory = 0.0;
			lkey = OBJ_IC_Result;
			break;

		case  OBJ_IC_MR:                       /* [MR] */
			if (touch && !LibIconClick(&iMR,&tsts))
				break;
			addCmd(tsts.obj, 0);
			setDisp(memory);
			lkey = OBJ_IC_Result;
			break;

		case  OBJ_IC_MPlus:                    /* [M+] */
			if (touch && !LibIconClick(&iMPlus,&tsts))
				break;
			addCmd(tsts.obj, 0);
			memory += lastResult;
			lkey = OBJ_IC_Result;
			break;

		case  OBJ_IC_MMinus:                   /* [M-] */
			if (touch && !LibIconClick(&iMMinus,&tsts))
				break;
			addCmd(tsts.obj, 0);
			memory -= lastResult;
			lkey = OBJ_IC_Result;
			break;

		case  OBJ_IC_Back:                     /* [->] */
			if (touch && !LibIconClick(&iBack,&tsts))
				break;
			addCmd(tsts.obj, 0);
			if (strlen(dispString) > 0)
				dispString[strlen(dispString)-1] = 0x00;
			if (dispString[0] == 0x00)
				clear();
			lastResult = atof(dispString);
			lkey = OBJ_IC_0;
			break;

		case  OBJ_IC_C:                        /* [C/AC] */
			if (touch && !LibIconClick(&iC,&tsts))
				break;
			addCmd(tsts.obj, 0);
			if (lkey != OBJ_IC_C)
				clear();
			else {
				init();
				lkey = OBJ_IC_Result;
			}
			break;

		case  OBJ_IC_PlusMinus:                       /* [+/-] */
			if (touch && !LibIconClick(&iPlusMinus,&tsts))
				break;
			addCmd(tsts.obj, 0);
			setDisp(-lastResult);
			lkey = OBJ_IC_Result;
			break;

		case  OBJ_IC_Dot:			/* [.], [0]..[9] */
		case  OBJ_IC_0: 
		case  OBJ_IC_1: case  OBJ_IC_2: case  OBJ_IC_3:	
		case  OBJ_IC_4: case  OBJ_IC_5: case  OBJ_IC_6:
		case  OBJ_IC_7: case  OBJ_IC_8: case  OBJ_IC_9:
			if (!touch || LibIconClick(&iDot,&tsts) == TRUE ||
			    LibIconClick(&i0,&tsts) ||
			    LibIconClick(&i1,&tsts) ||
			    LibIconClick(&i2,&tsts) ||
			    LibIconClick(&i3,&tsts) ||
			    LibIconClick(&i4,&tsts) ||
			    LibIconClick(&i5,&tsts) ||
			    LibIconClick(&i6,&tsts) ||
			    LibIconClick(&i7,&tsts) ||
			    LibIconClick(&i8,&tsts) ||
			    LibIconClick(&i9,&tsts)) {
				addCmd(tsts.obj, 0);
				if (lkey != OBJ_IC_0)
					dispString[0] = 0x00;
				if (strlen(dispString) < maxInput) {
					if (tsts.obj == OBJ_IC_Dot) {
						if (!checkDot())
							strcat(dispString,".");
					} else {
						nr[0] = tsts.obj - OBJ_IC_0 + ((byte)'0');
						nr[1] = 0x00;
						strcat(dispString,nr);
					}
				}
				lastResult = atof(dispString);
				lkey = OBJ_IC_0;
			}
			break;

		case OBJ_IC_KAuf:			/* [(] */
			if (touch && !LibIconClick(&iKAuf,&tsts))
				break;
			addCmd(tsts.obj, 0);
			pushOp('(');
			lkey = OBJ_IC_Result;
			break;

		case OBJ_IC_KZu:			/* [)] */
			if (touch && !LibIconClick(&iKZu,&tsts))
				break;
			addCmd(tsts.obj, 0);
			while (nops > 0 && peekOp() != '(')
				execute(popOp());
			popOp();
			lkey = OBJ_IC_Result;
			break;

		case OBJ_IC_Mal:			/* [*], [/] */
		case OBJ_IC_Teilen:
			if (touch && !LibIconClick(&iMal,&tsts) && !LibIconClick(&iTeilen,&tsts))
				break;
			addCmd(tsts.obj, 0);
			while (nops > 0 && 
			      ((o = peekOp()) == '^' ||
			       o == '*' ||
			       o == '/')) 
				execute(popOp());
			if (tsts.obj == OBJ_IC_Mal)
				pushOp('*');
			else
				pushOp('/');
			push();
			lkey = OBJ_IC_Mal;
			break;

		case OBJ_IC_Plus:			/* [+], [-] */
		case OBJ_IC_Minus:
			if (touch && !LibIconClick(&iPlus,&tsts) && !LibIconClick(&iMinus,&tsts))
				break;
			addCmd(tsts.obj, 0);
			while (nops > 0 && 
			      ((o = peekOp()) == '^' ||
			       o == '*' ||
			       o == '/' ||
			       o == '+' ||
			       o == '-'))
				execute(popOp());
			if (tsts.obj == OBJ_IC_Plus)
				pushOp('+');
			else
				pushOp('-');
			push();
			lkey = OBJ_IC_Plus;
			break;

		case OBJ_IC_Result:			/* [=] */
			if (touch && !LibIconClick(&iResult,&tsts))
				break;
			addCmd(tsts.obj, 0);
			while (nops > 0)
				execute(popOp());
			lkey = OBJ_IC_Result;
			break;

		case OBJ_IC_Function:			/* [Exp.] */
			if (touch && !LibBlockIconClick(&iFunction,&tsts,B_ICON_CENTER))
				break;
			execFunc(selFunction(touch, DISPLAY_X+30, funcrtv, funccmnt));
			break;

		case OBJ_IC_Trig:			/* [Trig.] */
			if (touch && !LibBlockIconClick(&iTrig,&tsts,B_ICON_LEFT))
				break;
			execFunc(selFunction(touch, DISPLAY_X+2, trigrtv, trigcmnt));
			break;

		case OBJ_IC_Prog:			/* [Prog.] */
			if (!LibBlockIconClick(&iProg,&tsts,B_ICON_RIGHT))
				break;
			execProg(selFunction(TRUE, DISPLAY_X+60, progrtv, progcmnt));
			break;

		case OBJ_IC_Off:			/* [Off] */
			if (!LibBlockIconClick(&iOff,&tsts,B_ICON_RIGHT))
				break;
			LibJumpMenu();
			break;

		/** M-BAR **/
		case OBJ_HIC_MBAR:			/* [MenuBar] */
			GmPdwnSet();			/* PullDownMenu */
			if( GmPdwnFnc() == TRUE ){
				init();
				LibTchInit();
			};
		}
		if(tsts.act == ACT_ALM)
			LibAlarm();			/* AlarmProc */
	}
}

/* initialize data, display and touch panel */
static void init( void ) {
	running = FALSE;
	learning = FALSE;
	DataRead();
	initDisplay();
	tchSet();
	nops = 0;
	StackSize = 0;
	memory = 0.0;
	clear();
}

/* set Display to specified value */
void setDisp(double f) {
	int p, q;
	if (errno != 0)
		error();
	else {
		lastResult = f;
		sprintf(dispString,"%2.10f",f);
		if (checkDot())
			while (dispString[strlen(dispString)-1] == '0')
				dispString[strlen(dispString)-1] = 0x00;
		if (dispString[strlen(dispString)-1] == '.')
			dispString[strlen(dispString)-1] = 0x00;

		if ((f > -0.1 && f < 0.1)) {
			if ((f - atof(dispString)) != 0.0)
				sprintf(dispString,"%12.10e",f);
		} else if ((f <= -1e10) || (f >= 1e10))
			sprintf(dispString,"%12.10e",f);

		p = strlen(dispString) - 1;
		while ((p >= 0) && (dispString[p] != 'e'))
			p--;
		if (p >= 0) {
			q = p - 1;
			while ((q >= 0) && (dispString[q] == '0'))
				q--;
			memcpy(&dispString[q], &dispString[p], strlen(dispString) - p + 1);
		}
	}
}

/* set display to "Error" */
void error(void) {
	strcpy(dispString,_Error);
	learning = FALSE;
	running = FALSE;
	errno = 0;
}

/* check if display is "Error" */
bool isError(void) {
	if (errno != 0)
		error();
	return(strcmp(dispString,_Error) == 0);
}

/* set display to "0" */
void clear(void) {
	strcpy(dispString,"0");
	lastResult = 0.0;
	lkey = OBJ_IC_C;
}

/* check if display contains '.' */
bool checkDot(void) {
	return(strchr(dispString,'.') != NULL);
}

/* put value of display on top of stack */
void push(void) {
	if (StackSize < maxStack)
		Stack[StackSize++] = lastResult;
	else
		error();
}

/* get most recent value from stack */
double pop(void) {
	if (StackSize > 0)
		return(Stack[--StackSize]);
	else {
		error();
		return(0.0);
	}
}

/* push an operator on top of the operator-stack */
void pushOp(byte aOp) {
	if (nops < maxOps)
		opString[nops++] = aOp;
	else
		error();
}

/* get most recent operator on operator-stack */
byte popOp(void) {
	if (nops > 0)
		return(opString[--nops]);
	else {
		error();
		return(0.0);
	}
}

/* get most recent operator on operator-stack but leave it there */
byte peekOp(void) {
	if (nops > 0)
		return(opString[nops-1]);
	else {
		error();
		return(0.0);
	}
}

void addCmd(word key, byte subkey) {
	if (learning)
		if (cmdPos < maxCmd) {
			commands[prognr][cmdLen[prognr]].key = key;
			commands[prognr][cmdLen[prognr]++].subkey = subkey;
		} else
			error();
}

bool nextCommand(void) {
	if (running) {
		command = commands[prognr][cmdPos++];
		if (cmdPos >= cmdLen[prognr])
			running = FALSE;
		return(TRUE);
	} else
		return(FALSE);
}

/* execute one of the elemental operations +, -, *, /, ^ */
void execute(byte cmd) {
	double r;
	switch (cmd) {
	case '+':
		r = pop()+lastResult;
		break;
	case '-':
		r = pop()-lastResult;
		break;
	case '*':
		r = pop()*lastResult;
		break;
	case '/':
		if (lastResult != 0.0)
			r = pop()/lastResult;
		else
			error();
		break;
	case '^':
		/* r = pow(pop(),lastResult); TODO */
		break;
	}
	if (!isError())
		setDisp(r);
}

/* execute one of the special functions p.ex. sin */
void execFunc(byte aFunc) {
	double r,t;
	if (aFunc == 0xff)
		return;
	if (aFunc <= 10)
		addCmd(OBJ_IC_Function, aFunc);
	else
		addCmd(OBJ_IC_Trig, aFunc);
	switch (aFunc) {
		case 1:					/* exp(x) */
			/* r = exp(lastResult); TODO */
			break;
		case 2:					/* ln(x) */
			/* if (lastResult > 0)
				r = log(lastResult);
			else
				error(); TODO */
			break;
		case 3:					/* x^y (preparation, because yet, y is missing) */
			while (nops > 0 && peekOp() == '^')
				execute(popOp());
			pushOp('^');
			push();
			break;
		case 4:					/* x^2 */
			r = lastResult*lastResult;
			break;
		case 5:					/* sqrt(x) */
			/* if (lastResult >= 0)
				r = sqrt(lastResult);
			else
				error(); TODO */
			break;
		case 6:					/* log(x) */
			/* if (lastResult > 0)
				r = log10(lastResult);
			else
				error(); TODO */
			break;
		case 7:					/* 10^x */
			/* r = pow(10,lastResult); TODO */
			break;
		case 11:				/* PI */
			r = PI;
			break;
		case 12:				/* sin(x) */
			r = sin(lastResult);
			break;
		case 13:				/* cos(x) */
			r = cos(lastResult);
			break;
		case 14:				/* tan(x) */
			t = cos(lastResult);
			if (lastResult >= 0)
				r = sin(lastResult)/t;
			else
				error();
			break;
		case 15:				/* cot(x) */
			t = sin(lastResult);
			if (lastResult >= 0)
				r = cos(lastResult)/t;
			else
				error();
			break;
		case 16:				/* arcsin(x) */
			/* if (fabs(lastResult) <= 1)
				r = asin(lastResult);
			else
				error();
			break; TODO */
		case 17:				/* arccos(x) */
			/*if (fabs(lastResult) <= 1)
				r = acos(lastResult);
			else
				error(); TODO */  
			break;
		case 18:				/* arctan(x) */
			/* r = atan(lastResult); TODO */
			break;
	}
	lkey = OBJ_IC_Result;
	if (!isError() && aFunc != 3)
		setDisp(r);
}

/* pull down menu function */
byte selFunction(bool manual, byte xp, byte rtv[], byte cmnt[]) {
	byte np;
	SLW2_ST win_prm;
	byte sel_ret;

	if (!manual)
		return(command.subkey);

	np = 0;

	win_prm.x = xp;
	win_prm.y = 59;
	win_prm.xs = 40;
	win_prm.ity = 9;
	win_prm.np = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv = rtv;
	win_prm.t_xs = 4;

	sel_ret = LibSelWindowExt(&win_prm);
	return(sel_ret);
}

/* execute one of the programming option */
void execProg(byte aFunc) {
	switch (aFunc) {
	case 1:
	case 2:
	case 3:
		if (learning) 
			learning = FALSE;
		else {
			prognr = aFunc-1;
			cmdLen[prognr] = 0;
			learning = TRUE;
		}
		break;
	case 4:
	case 5:
	case 6:
		if (learning) 
			learning = FALSE;
		cmdPos = 0;
		prognr = aFunc-4;
		running = TRUE;
		break;
	case 7:
		DataSave();
		break;
	case 8:
		DataRead();
	}
	lkey = OBJ_IC_Result;
}

/* load programs from file */
void DataRead(void){
	if (!LibSubEntrySearch(filename,&pzlfb.fsb_sub_entry_)) {
		cmdLen[0] = 0;
		cmdLen[1] = 0;
		cmdLen[2] = 0;
		return;
	}
	LibGetAllEntry(filename,&pzlfb.fsb_main_entry_,&pzlfb.fsb_sub_entry_);
	pzlfb.fsb_scrt_info_  = 0x80;
	pzlfinf.fp      = 0xffff;
	pzlfinf.kind    = FILE_KIND_BIN;
	if (LibFileFindNext(&pzlfb,&pzlfinf,0x00)){
		LibFileRead(&pzlfb,&pzlfinf);
		memcpy(&cmdLen,&pzlfb.fbuf.bin.bin_buf,sizeof(cmdLen));
		memcpy(&commands,&pzlfb.fbuf.bin.bin_buf[3],sizeof(commands));
	}
}

/* save programs to file */
void DataSave(void){
	if (!LibSubEntrySave(filename,&pzlfb.fsb_sub_entry_))
		return;
	LibGetAllEntry(filename,&pzlfb.fsb_main_entry_,&pzlfb.fsb_sub_entry_);
	pzlfb.fsb_scrt_info_  = 0x80;
	pzlfinf.fp      = 0xffff;
	pzlfinf.kind    = FILE_KIND_BIN;
	if (!LibFileFindNext(&pzlfb,&pzlfinf,0x00)) {
		pzlfinf.fp      = 0xffff;
		pzlfinf.kind    = FILE_KIND_BIN;
	}
	memcpy(&pzlfb.fbuf.bin.bin_buf,&cmdLen,sizeof(cmdLen));
	memcpy(&pzlfb.fbuf.bin.bin_buf[3],&commands,sizeof(commands));
	pzlfb.fbuf.bin.char_num = 3+sizeof(commands);
	LibFileWrite(&pzlfb,&pzlfinf);
}

/* set up touchpanel */
static void tchSet(void){
	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( keys );
	LibTchStackPush( tbtns );
	LibTchStackPush( TchModeIcon );         /* Pop Up Tools */
}

/* update display */
void showDisplay(void){
	byte astr[3];
	int w;
	w = LibGetProStrSize(IB_PFONT1,dispString);
	LibClrBox(31,32,99,11);
	LibPutProStr(IB_PFONT1,33+96-w,33,dispString,w);
	if (memory != 0.0)
		LibPut35Str(31,37,"M");
	if (running) {
		sprintf(astr,"A%i",prognr+1);
		LibPut35Str(31,32,astr);
	}
	if (learning) {
		sprintf(astr,"P%i",prognr+1);
		LibPut35Str(31,32,astr);
	}
	LibPutDispBox(31,32,99,11);
}

/* initialize display */
void initDisplay(void){
	int x,y;

	LibClrDisp();

/*	LibPutMessage(472,1,1,IB_PFONT3);	/* "Calculator" */
	LibPutProStr(IB_PFONT3,1,1,"Franka",50);
	LibPutFarData(FRAME_X,FRAME_Y,106);	/* Frame */
        LibPutFarData(DISPLAY_X,DISPLAY_Y,134);	/* Display */

	LibPutFarData(DISPLAY_X+2,47,110);	/* menu buttons */
	LibPutFarData(DISPLAY_X+DISPLAY_W-2-30,47,27);	/* "Off" button */
	LibPutProStr(IB_PFONT1,DISPLAY_X+5,49,"Trig.",29);
	LibPutProStr(IB_PFONT1,DISPLAY_X+34,49,"Exp.",29);
	LibPutProStr(IB_PFONT1,DISPLAY_X+63,49,"Prog.",29);
	LibPutProStr(IB_PFONT1,DISPLAY_X+DISPLAY_W-29,49,"Off",29);

	LibModeIconDsp();               /* Pop Up Tools */
	for (x=0; x<5; x++) {		/* Calculator keys */
		for (y=0; y<5; y++) {
			LibGdsBox(KEY_LOX+x*KEY_DIST_X,KEY_LOY+y*KEY_DIST_Y,KEY_LOX+x*KEY_DIST_X+KEY_SIZE_X,KEY_LOY+y*KEY_DIST_Y+KEY_SIZE_Y);
			LibPutProStr(IB_PFONT1,KEY_LOX+3+x*KEY_DIST_X,KEY_LOY+3+y*KEY_DIST_Y,keytexts[y][x],KEY_SIZE_X-2);
		}
	}

	LibPutDisp();
}

/* set system menu */
void GmPdwnSet(void)
{
	word p_sys;

	/*LibPullDownInit(MPdEdt,MPdSys,MPdOpt);   */       /* MenuBAR Init */

	/* [System] */
	p_sys =  PULLDOWN_SYSTEM | 0x0100;
	LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_SYSTEM, p_sys);

}

/*  execute systemmenu */
bool GmPdwnFnc()
{
	word sel;
	bool jdg = FALSE;

	sel = LibPullDown();            /* PullDownMenu */
	if(sel & PULLDOWN_SYSTEM)
		jdg = PdwnFncSys(sel);
	return(jdg);
}

/* execute functions of system menu */
bool PdwnFncSys(word sel)
{
	bool jdg = FALSE;
	switch(sel){
	case PDWN_SYS_DTTM:	/* Set_date/time */
		LibFuncDateTime();
		break;

	case PDWN_SYS_SOUND:	/* Sound */
		LibFuncSound(0);
		break;

	case PDWN_SYS_FORMT:	/* Format */
		LibFuncFormat();
		break;

	case PDWN_SYS_CAPCT:	/* Capacity */
		LibFuncCapa();
		break;

	case PDWN_SYS_MMANE:	/* Memory_management */
		LibFuncMemoryManagement();
		break;

	case PDWN_SYS_CNTRS:	/* Contrast */
		LibFuncContrast();
		break;

	case PDWN_SYS_TPA:	/* Touch_Panel_Alignment */
		LibFuncDigitizer();
	}
	return(jdg);
}
