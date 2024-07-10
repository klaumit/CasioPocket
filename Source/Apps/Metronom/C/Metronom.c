/******************************************************************************
*	[Title]	Metronome Main File
*	[Model] 	PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include	"Metronom.h"

/***  ***/
void	TchSet( bool f );
void	DispTitle( );
void	ftoa(long double f, char *str);
void	TchNumKeyIconDef( void );
void	NumInput( void );
void	DispPricker( void );

TCHTBL	TchNumKeyIcon[13];
T_ICON	TiconNumKey[13];

TCHTBL far TchOperateIcon[] = {
	55,117,106,135,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN| ACT_500MSEC,
	OBJ_IC_START,
	0x0000,
	
	120,1,149,19,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN| ACT_500MSEC,
	OBJ_IC_NUMSET,
	0x0000,
	
	120,21,149,39,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN| ACT_500MSEC,
	OBJ_IC_TIMESET,
	0x0000,
	
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};
T_ICON far TiconStart = { &TchOperateIcon[0], NULL, NULL, 0x02 };
	
TCHSTS	tsts;
byte	Time[2],Freq[4];
bool	Start;
int freqcount,timecount;
byte FreqTime;

/******************************************************************************
*  [Title]			Metronome(main)
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void main(void) {
	
	TchNumKeyIconDef( );
	DispTitle( );
	strcpy(Freq,"100");
	strcpy(Time,"1");
	Start=FALSE;
	freqcount=0;
	timecount=0;
	FreqTime=0;
	TchSet( FALSE );
	for(;;){
		LibTchWait( &tsts );
		if( Start == TRUE ){
			if( tsts.ext==EXT_CODE_500MS ){
				DispPricker( );
			}
		}
		switch( tsts.obj ) {
			case  OBJ_IC_NUMSET:
				if(tsts.act!=ACT_MAKE)	break;
				if(Start==TRUE)	break;
				FreqTime=0;
				TchSet( TRUE );
				NumInput( );
				break;
				
			case  OBJ_IC_TIMESET:
				if(tsts.act!=ACT_MAKE)	break;
				if(Start==TRUE)	break;
				FreqTime=1;
				TchSet( TRUE );
				NumInput( );
				break;

			case  OBJ_IC_START:
				if(LibBlockIconClick( &TiconStart, &tsts , B_ICON_RIGHT ) == TRUE) {
					/*if(tsts.act!=ACT_MAKE)	break;*/
					if(Start==FALSE){
						/* disable TchHardIcon */
						LibTchStackClr();
						LibTchStackPush(NULL);
						LibTchStackPush(TchOperateIcon);
						LibTchInit();
	
						Start=TRUE;
						LibGdsClr(56,118,104,133);
						LibPutProStr(IB_PFONT3,68,121,"STOP",80);
						LibPutDisp( );
					}else{
						Start=FALSE;
						/* set TchHardIcon */
						TchSet( FALSE );
						
						LibGdsClr(56,118,104,133);
						LibPutProStr(IB_PFONT3,63,121,"START",80);
						LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[0] );
						LibPutDisp( );
						freqcount=0;	timecount=0;
						LibBuzzerOff();
					}
				}
				break;

			default:
				break;
		}
	}
}

/******************************************************************************
*	[Title]		TchNumKeyIconDef
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void	TchNumKeyIconDef( void ){
	int i,j;
	
	/* 0 key */
	TchNumKeyIcon[0].x1=NUMKEYSTART_X+13+3*(NUMKEYSIZE_X+5);
	TchNumKeyIcon[0].y1=NUMKEYSTART_Y+7;
	TchNumKeyIcon[0].x2=TchNumKeyIcon[0].x1+NUMKEYSIZE_X-1;
	TchNumKeyIcon[0].y2=TchNumKeyIcon[0].y1+NUMKEYSIZE_Y-1;
	TchNumKeyIcon[0].act=ACT_ICON;
	TchNumKeyIcon[0].ext=0x0000;
	TchNumKeyIcon[0].obj=OBJ_IC_NUMKEY0;
	TiconNumKey[0].tch=&TchNumKeyIcon[0];
	TiconNumKey[0].ngp=NULL;
	TiconNumKey[0].rgp=NULL;
	TiconNumKey[0].kind=0x00;
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) {
			TchNumKeyIcon[i*3+j+1].x1=NUMKEYSTART_X+13+j*(NUMKEYSIZE_X+5);
	   	TchNumKeyIcon[i*3+j+1].y1=NUMKEYSTART_Y+7+i*(NUMKEYSIZE_Y+4);
	   	TchNumKeyIcon[i*3+j+1].x2=TchNumKeyIcon[i*3+j+1].x1+NUMKEYSIZE_X-1;
	   	TchNumKeyIcon[i*3+j+1].y2=TchNumKeyIcon[i*3+j+1].y1+NUMKEYSIZE_Y-1;
	   	TchNumKeyIcon[i*3+j+1].act=ACT_ICON;
			TchNumKeyIcon[i*3+j+1].ext=0x0000;
			TchNumKeyIcon[i*3+j+1].obj=0x9001+i*3+j;
			TiconNumKey[i*3+j+1].tch=&TchNumKeyIcon[i*3+j+1];
			TiconNumKey[i*3+j+1].ngp=NULL;
			TiconNumKey[i*3+j+1].rgp=NULL;
			TiconNumKey[i*3+j+1].kind=0x00;
		}
	}
	
	/* Backspace key */
	TchNumKeyIcon[10].x1=NUMKEYSTART_X+13+3*(NUMKEYSIZE_X+5);
	TchNumKeyIcon[10].y1=NUMKEYSTART_Y+7+1*(NUMKEYSIZE_Y+4);
	TchNumKeyIcon[10].x2=TchNumKeyIcon[10].x1+NUMKEYSIZE_X-1;
	TchNumKeyIcon[10].y2=TchNumKeyIcon[10].y1+NUMKEYSIZE_Y-1;
	TchNumKeyIcon[10].act=ACT_ICON;
	TchNumKeyIcon[10].ext=0x0000;
	TchNumKeyIcon[10].obj=OBJ_IC_NUMKEYBACK;
	TiconNumKey[10].tch=&TchNumKeyIcon[10];
	TiconNumKey[10].ngp=NULL;
	TiconNumKey[10].rgp=NULL;
	TiconNumKey[10].kind=0x00;
	
	/* Set key */
	TchNumKeyIcon[11].x1=NUMKEYSTART_X+13+3*(NUMKEYSIZE_X+5);
	TchNumKeyIcon[11].y1=NUMKEYSTART_Y+7+2*(NUMKEYSIZE_Y+4);
	TchNumKeyIcon[11].x2=TchNumKeyIcon[11].x1+2*NUMKEYSIZE_X+4;
	TchNumKeyIcon[11].y2=TchNumKeyIcon[11].y1+NUMKEYSIZE_Y-1;
	TchNumKeyIcon[11].act=ACT_ICON;
	TchNumKeyIcon[11].ext=0x0000;
	TchNumKeyIcon[11].obj=OBJ_IC_NUMKEYSET;
	TiconNumKey[11].tch=&TchNumKeyIcon[11];
	TiconNumKey[11].ngp=NULL;
	TiconNumKey[11].rgp=NULL;
	TiconNumKey[11].kind=0x00;
	
	/* AC key */
	TchNumKeyIcon[12].x1=NUMKEYSTART_X+13+4*(NUMKEYSIZE_X+5);
	TchNumKeyIcon[12].y1=NUMKEYSTART_Y+7;
	TchNumKeyIcon[12].x2=TchNumKeyIcon[12].x1+NUMKEYSIZE_X-1;
	TchNumKeyIcon[12].y2=TchNumKeyIcon[12].y1+2*NUMKEYSIZE_Y+4-1;
	TchNumKeyIcon[12].act=ACT_ICON;
	TchNumKeyIcon[12].ext=0x0000;
	TchNumKeyIcon[12].obj=OBJ_IC_NUMKEYAC;
	TiconNumKey[12].tch=&TchNumKeyIcon[12];
	TiconNumKey[12].ngp=NULL;
	TiconNumKey[12].rgp=NULL;
	TiconNumKey[12].kind=0x00;
}

/******************************************************************************
*	[Title]		Touch set
*	[Arguments]	FALSE(OperateIcon),TRUE(NumkeyIcon)
*	[ReturnVAL]	None
******************************************************************************/
void TchSet( bool f ){
	LibTchStackClr();
	LibTchStackPush(NULL);
	LibTchStackPush(TchHardIcon);
	
	if(f==FALSE)	LibTchStackPush(TchOperateIcon);
	else	LibTchStackPush(TchNumKeyIcon);
	
	LibTchInit();
}

/******************************************************************************
*	[Title]		NumInput
*	[Arguments]	None
*	[ReturnVAL]	None
******************************************************************************/
void	NumInput( ){
	int	number;
	byte	*num=" ";
	bool	loop_end;
	
	LibOpenWindow(0,40,159,159);
	if(FreqTime==0){
		LibGdsClr(122,3,146,16);
		Freq[0]=0;
	}else{
		LibGdsClr(122,23,146,36);	
		Time[0]=0;
	}
	/* display number key */
	LibPutGraph( NUMKEYSTART_X, NUMKEYSTART_Y, GraphNumKey );
	LibPutDisp();
	
	loop_end = FALSE;
	while(loop_end==FALSE){
		LibTchWait(&tsts);
		number=tsts.obj-0x9000;
		switch(tsts.obj){
			case	OBJ_IC_NUMKEY0:
			case	OBJ_IC_NUMKEY1:
			case	OBJ_IC_NUMKEY2:
			case	OBJ_IC_NUMKEY3:
			case	OBJ_IC_NUMKEY4:
			case	OBJ_IC_NUMKEY5:
			case	OBJ_IC_NUMKEY6:
			case	OBJ_IC_NUMKEY7:
			case	OBJ_IC_NUMKEY8:
			case	OBJ_IC_NUMKEY9:
				if(LibBlockIconClick(&TiconNumKey[number],&tsts,B_ICON_RIGHT)==TRUE){
					number=objtonum[number];
					num[0]=0;
					itoa(number,num,10);
					if(FreqTime==0){
						if((number==0)&&(Freq[0]==0))	break;
						if(strlen(Freq)>=STRING_LEN_FREQ)		break;
						strcat(Freq,num);
					}else{
						if((number==0)&&(Time[0]==0))	break;
						if(strlen(Time)>=STRING_LEN_TIME)		break;
						strcat(Time,num);
					}
				}
				break;
			
			case	OBJ_IC_NUMKEYBACK:
				if(LibBlockIconClick(&TiconNumKey[number],&tsts,B_ICON_RIGHT)==TRUE){
					if(FreqTime==0){
						if(Freq[0]==0)	break;
						Freq[strlen(Freq)-1]=0;
					}else{
						if(Time[0]==0)	break;
						Time[strlen(Time)-1]=0;
					}
				}
				break;
			
			case	OBJ_IC_NUMKEYAC:
				if(LibBlockIconClick(&TiconNumKey[number],&tsts,B_ICON_RIGHT)==TRUE){
					if(FreqTime==0)	Freq[0]=0;
					else	Time[0]=0;
				}
				break;
			
			case	OBJ_IC_NUMKEYSET:
				if(LibBlockIconClick(&TiconNumKey[number],&tsts,B_ICON_RIGHT)==TRUE){
					if(FreqTime==0){
						if(Freq[0]==0){
							LibPutMsgDlg("Please input Frequency!");
							LibPutDisp( );
							break;
						}
						if(atoi(Freq)>240){
							LibPutMsgDlg("The MAX Frequency is 240.") ;
							Freq[0]=0;
							break;
						}
					}else{
						if(Time[0]==0){
							LibPutMsgDlg("The Time can't be empty.");
							LibPutDisp( );
							break;
						}
						if(atoi(Time)>7){
							LibPutMsgDlg("The MAX Time is 7.") ;
							Time[0]=0;
							break;
						}
					}
					loop_end=TRUE;
				}
				break;
		}	
		if(FreqTime==0){
			LibGdsClr(122,3,146,16);
			LibPutProStr(IB_PFONT3,124,5,Freq,80);
		}else{
			LibGdsClr(122,23,146,36);
			LibPutProStr(IB_PFONT3,131,25,Time,80);
		}
		LibPutDisp();
	}
	TchSet( FALSE );
	LibCloseWindow();
	LibPutDisp();
}

/******************************************************************************
*  [Title]			display Titel
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void DispTitle( )
{
	LibGdsClr(0,0,159,159);
	
	LibPutProStr(IB_PFONT3,12,9,"METRONOME",120);
	LibPutGraph( 88, 2, GraphTime );
	LibGdsBox(120,1,148,18);
	LibGdsBox(121,2,147,17);
	LibPutProStr(IB_PFONT3,124,5,"100",80);
	LibPutProStr(IB_PFONT3,88,25,"TIME",120);
	LibPutProStr(IB_PFONT3,131,25,"1",80);
	
	/* display pricker */
	LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[0] );
	
	/* display START icon */
	LibGdsBox(55,117,105,134);
	LibGdsLine(56,135,106,135);
	LibGdsLine(106,118,106,135);
	LibPutProStr(IB_PFONT3,63,121,"START",80);
	
	LibPutDisp( );
}

/******************************************************************************
*  [Title]			display Pricker
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void DispPricker( )
{
	long int i,l;
	int j,k;
	
	LibBuzzerOff();
	if(atoi(Freq)<=119){
		j=120/atoi(Freq);
		k=240/atoi(Freq);
		if((atoi(Freq)>60)&&(atoi(Freq)<=80)){
			j=2;	k=4;	
		}
		freqcount++;
		if(freqcount==j){
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[1] );
			if(atoi(Time)==1)	LibBuzzerOn(IB_BEEP2_SET);
			else{
				if(timecount==0)	LibBuzzerOn(IB_BEEP2_SET);
				else	LibBuzzerOn(IB_BEEP1_SET);
				timecount=(timecount+1)%atoi(Time);
			}
		}
		if(freqcount==k){
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[2] );
			freqcount=0;
			if(atoi(Time)==1)	LibBuzzerOn(IB_BEEP2_SET);
			else{
				if(timecount==0)	LibBuzzerOn(IB_BEEP2_SET);
				else	LibBuzzerOn(IB_BEEP1_SET);
				timecount=(timecount+1)%atoi(Time);
			}
		}
		LibPutDisp( );
	}else{
		if(atoi(Freq)/60<=3){	/* 120<=Time<240 */
			l=38000*120/atoi(Freq);
			i=0;
			for(;i<l;i++);
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[1] );
			LibPutDisp( );
			l=48000*120/atoi(Freq);
			i=0;
			for(;i<l;i++);
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[2] );
			LibPutDisp( );
			
			if(atoi(Time)==1)	LibBuzzerOn(IB_BEEP2_SET);
			else{
				if(timecount==0)	LibBuzzerOn(IB_BEEP2_SET);
				else	LibBuzzerOn(IB_BEEP1_SET);
				timecount=(timecount+1)%atoi(Time);
			}
		}else{	/* 240<=Freq<300 */
			l=40000*120/atoi(Freq);
			i=0;
			for(;i<l;i++);
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[1] );
			LibPutDisp( );
			
			if(atoi(Time)==1)	LibBuzzerOn(IB_BEEP2_SET);
			else{
				if(timecount==0)	LibBuzzerOn(IB_BEEP2_SET);
				else	LibBuzzerOn(IB_BEEP1_SET);
				timecount=(timecount+1)%atoi(Time);
			}
			
			l=75000*120/atoi(Freq);
			i=0;
			for(;i<l;i++);
			LibBuzzerOff();
			l=40000*120/atoi(Freq);
			i=0;
			for(;i<l;i++);
			LibPutGraph( PRICKER_X, PRICKER_Y, GraphPricker[2] );
			LibPutDisp( );
			
			if(atoi(Time)==1)	LibBuzzerOn(IB_BEEP2_SET);
			else{
				if(timecount==0)	LibBuzzerOn(IB_BEEP2_SET);
				else	LibBuzzerOn(IB_BEEP1_SET);
				timecount=(timecount+1)%atoi(Time);
			}
			
			i=0;
			for(;i<l;i++);
		}
	}
}