/******************************************************************************
	[Title]		SampleProgram

	Copyright (C) 1998 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#include	<stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"sample.h"


/*HEAD*/
/******************************************************************************
    [Title]		SampleProgramMain

******************************************************************************/
void main()
{
	Scr.vol	= 100;

	for(;;){
		SmpLstMain();
	}
}


/*HEAD*/
/******************************************************************************
    [Title]        ListDispMain

    [arguments]		none

    [return value]	none

******************************************************************************/
void SmpLstMain(void)
{

	TCHSTS		tsts;
	int			ans,t_ptr;
	T_SCR_POS	t_scr = {0};
	bool		loop  = TRUE;

	Scr.x	 = SCR__X;			/* initial scroll bar X */
	Scr.y	 = SCR__Y;			/* initial scroll bar Y */
	Scr.size = SCR__SIZE;		/* initial scroll bar size */
	Scr.dsp	 = SCR_DSP;			/* initial disp count*/
	Scr.pos	 = 0;				/* start positon */
	CurPtr	 = 0;				/* bar cursor position  */

	LibTchStackClr();
	LibTchStackPush(NULL);
	LibTchStackPush(TchHardIcon);
	LibTchStackPush(TchList);

	SmpDspBk();		/* DispBackgraund     */
/*	LibPutMsgDlg2("Data [%d]",Scr.vol);*/
	SmpFlushLst();	/* ListBufferFlush */
	SmpDspData();	/* ListDisp   */

	LibTchInit();
	for(;loop==TRUE;){
		LibTchWait(&tsts);
		switch(tsts.obj){
			case OBJ_SCR_BAR:	/* ScrollBar */
				ans = LibScrollClick(&tsts,&Scr);
				if		(ans == 0 && tsts.act == ACT_BREAK_IN){	/* Up */
					if(CurPtr>0){
						SmpDspCur();	/* Off cursor bar */
						CurPtr--;		/* Move cursor bar */
						SmpDspCur();	/* Disp cursor bar */
					}
					else{
						if(Scr.pos>0){
							SmpDspCur();	/* Off cursor bar */
							Scr.pos--;
							LibScrollPrint(Scr);
							SmpFlushLst();
							LibGrpDwn(5,8,(112+30),(111+34),9);
							LibPutProStr(IB_PFONT1,5,11,LstBuf[CurPtr],110+30);
							SmpDspCur();	/* Disp cursor bar */
							LibPutDisp();
						}
					}
				}
				else if	(ans == 1 && tsts.act == ACT_BREAK_IN){	/* down */
					if(CurPtr<SCR_DSP-1){
						if(LstFlg[CurPtr+1]==TRUE){
							SmpDspCur();	/* Off cursor bar */
							CurPtr++;		/* Move cursor bar */
							SmpDspCur();	/* Disp cursor bar */
						}
						else if(CurPtr){
		LIST_END:
							/* LastLineUp */
							Scr.pos++;
							CurPtr--;
							SmpFlushLst();
							LibGrpUp(5,9,(112+30),(111+34),9);
							LibPutDisp();
						}
					}
					else{
						if(Scr.pos<Scr.vol-Scr.dsp){
							SmpDspCur();	/* Off cursor bar */
							Scr.pos++;
							LibScrollPrint(Scr);
							SmpFlushLst();
							LibGrpUp(5,9,112+30,111+34,9);
							LibPutProStr(IB_PFONT1,5,11+(SCR_DSP-1)*9,LstBuf[CurPtr],110+30);
							SmpDspCur();	/* Disp cursor bar */
							LibPutDisp();
						}
						else	goto LIST_END;
					}
				}
				else if	(ans == 2){							/* middle area */
					if(t_scr.pos != Scr.pos){
						SmpFlushLst();
						SmpDspData();
						if(LstFlg[CurPtr]==FALSE){
							SmpDspCur();	/* Off cursor bar */
							CurPtr = SmpAdjCurPtr(CurPtr);
							SmpDspCur();	/* Disp cursor bar */
						}
					}
				}else if(ans == 4){

				}
				t_scr = Scr;

				break;

			case OBJ_LDATA:
				if(tsts.act == ACT_MAKE){
					t_ptr = SmpGetCurPtr(tsts);
					if(LstFlg[t_ptr]==TRUE){
						if(t_ptr != CurPtr){
							SmpDspCur();	/* Off cursor bar */
							CurPtr = t_ptr;
							SmpDspCur();	/* Disp cursor bar */
						}
					}
				}
				break;

			case OBJ_ICON_020:
				if(Scr.vol== 20)	break;
				if (LibIconClick(&Ticon01, &tsts) == TRUE){
					Scr.vol = 20;
					loop = FALSE;
				}
				break;

			case OBJ_ICON_100:
				if(Scr.vol==100)	break;
				if (LibIconClick(&Ticon02, &tsts) == TRUE){
					Scr.vol = 100;
					loop = FALSE;
				}
				break;

			case OBJ_ICON_300:
				if(Scr.vol==300)	break;
				if (LibIconClick(&Ticon03, &tsts) == TRUE){
					Scr.vol = 300;
					loop = FALSE;
				}
				break;

			case OBJ_HIC_MBAR:
				LibPutMsgDlg2("MENU BAR\r\r   OK!!");
				break;

			case OBJ_HIC_ESC:
				LibPutMsgDlg2("ESC\r\rOK!!");
				break;

			default:
				break;
		}
	}

}


/*HEAD*/
/******************************************************************************
******************************************************************************/
void SmpDspBk(void)
/*TAIL*/
{
	LibClrDisp();
	LibScrollPrint(Scr);
	LibBox(4,7,114+30,114+32+4,1);
	LibIconPrint(&Ticon01);
	LibIconPrint(&Ticon02);
	LibIconPrint(&Ticon03);
	LibPutDisp();
}

/*HEAD*/
/******************************************************************************
******************************************************************************/
void SmpFlushLst(void)
{
	int		i;

	for(i=0;i<SCR_DSP;i++){
		if(i+Scr.pos > Scr.vol-1){
			LstFlg[i] = FALSE;
		}
		else{
			LstFlg[i] = TRUE;
			far_strcpy(LstBuf[i],FileData[i+Scr.pos]);
		}
	}

}

/*HEAD*/
/******************************************************************************
******************************************************************************/
void SmpDspData(void)
{
	int		i;

	LibClrBox(5,8,112+30,112+32+4);

	for(i=0;i<SCR_DSP;i++){
		if(LstFlg[i]==TRUE)
			LibPutProStr(IB_PFONT1,5,11+i*9,LstBuf[i],110+30);
		else
			LibClrBox(5,11+i*9,112+30,8);
	}

	SmpDspCur();
	LibPutDisp();
}

/*HEAD*/
/******************************************************************************
******************************************************************************/
void SmpDspCur(void)
/*TAIL*/
{
	LibReverse(		5,10+CurPtr*9,112+30,9);
	LibPutDispBox(	5,10+CurPtr*9,112+30,9);
}


/*HEAD*/
/******************************************************************************
******************************************************************************/
int SmpGetCurPtr(TCHSTS tsts)
{
	int		i;

	for(i=0;i<SCR_DSP;i++){
		if(tsts.y>=11+i*9 && tsts.y<=11+(i+1)*9-1)
			return(i);
	}
	return(0);

}
/*HEAD*/
/******************************************************************************
******************************************************************************/
int SmpAdjCurPtr(int now_ptr)
{
	int		i;

	for(i=now_ptr-1;i>=0;i--){
		if(LstFlg[i]==TRUE)
			return(i);
	}
	return(0);

}
