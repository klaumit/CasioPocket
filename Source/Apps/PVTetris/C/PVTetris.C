/******************************************************************************
*	[Title]		PVTetris Main File
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include	<stdrom.h>
#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"
#include	"PVTetris.h"

int n,m,high,linenum,step,tmpos,scorenum;
int shape, i,j,pos,newshape,newpos;     /* Shape Data i--row,j--colum*/
int  right;           /* right end of shape */
char line[3];        
char score[6];
bool boxdsp[ROW_COUNT][COLUM_COUNT];  /* screen data */
bool start,pause,over,first,hardlevel,b_sound,speed_change;
byte lv,lvtemp,game;
byte Buf[15+1];
T_ICON far TiconStart = { tchstart, NULL, NULL, 0x02 };
T_ICON	TiconPause={&tchstart[1], NULL, NULL, 0x02};
/*HEAD*/
/******************************************************************************
*  [Title]			Tetris Main Function
*  [Arguments]		None
*  [ReturnVAL]		None
******************************************************************************/
void main()
{	
	start=FALSE;
	pause=FALSE;
	over=FALSE;	
	hardlevel=FALSE;
	speed_change=FALSE;
	b_sound=TRUE;
	pos=0;
	shape=0;
	newshape=0;
	newpos=0;
	high=ROW_COUNT-1;
	lv=0;
	lvtemp=0;
	linenum=0;	
	scorenum=0;
	strcpy(Buf, "Try again! ^o^"); 
	srand( GetSeed() );     
	DspBk();
	PlyGm();
}

/******************************************************************************
*  [Title]			DspBk
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void DspBk(){
	LibClrDisp();
	LibGdsBox(0,0,81,153);
	LibGdsBoxMesh(85,99,123,153);
	LibGdsBox( 85, 1,118,13 );
	LibGdsLine(119,2,119,14);
	LibGdsLine(86,14,119,14);
	LibPutProStr(IB_PFONT1,88,4,"SPEED", 100);
	LibGdsBox( 122	, 1,155,13 );
	LibGdsLine(156,2,156,14);
	LibGdsLine(123,14,156,14);
	LibPutProStr(IB_PFONT1,126,4,"GAME", 100);
	
	LibPutProStr(IB_PFONT3, 97, 17,"PVTetris", 100);
	LibPutProStr(IB_PFONT1, 90, 30,"Lines:", 100);
	LibPutProStr(IB_PFONT1, 90, 40,"Scores:", 100);
	
	itoa(scorenum,score,10);
	LibPutProStr(IB_CG57FONT,132,40,score,130);	
	itoa(linenum,line,10);
	LibPutProStr(IB_CG57FONT,132,30,line,130);
	
	LibGdsBox( 85, 53, 118,65 );
	LibGdsLine(119,54,119,66);
	LibGdsLine(86,66,119,66);
	LibPutProStr(IB_PFONT1, 88,56,"START", 100);
	LibGdsBox( 122, 53,155,65 );
	LibGdsLine(156,54,156,66);
	LibGdsLine(123,66,156,66);
	LibPutProStr(IB_PFONT1,125,56,"PAUSE", 100);
	LibPutGraph( 90, 70, bigleft );
	LibPutGraph( 125, 70, bigright );
	LibPutGraph( 125, 100, bigarrow );
	LibPutGraph( 125, 129, bigdown );
	LibPutDisp();
}

/******************************************************************************
*  [Title]			PlyGm
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/
void PlyGm(void)
{
	TCHSTS	tsts;  
	bool loop;
	int tm;
	i=1;
	j=5;
	game=0;
	tm=1;
	loop=TRUE;	
	/* set block status all be FALSE first */
	for(i=0;i<ROW_COUNT;i++)
		for(j=0;j<COLUM_COUNT;j++)
			boxdsp[i][j]=FALSE;
	
	LibTchStackClr();
	LibTchStackPush(NULL);
	LibTchStackPush(TchActionKey);
	LibTchStackPush(TchHardIcon);
	LibTchStackPush(control);
	LibTchStackPush(tchstart);	
	LibTchStackPush( TchHeaderIcon );
	LibTchInit();
		
	for(;loop==TRUE;){		
		
		LibTchWait(&tsts);
		
		/* Move Shape down Every one sencond */
		if( tsts.ext==EXT_CODE_500MS ){
			tm=(++tm)%2;	
			if(lv==1) tm=0;
			if(start&&!over&&!pause&&tm==0){
			 	for(step=0;step<=lv;step++){
			 		if(test(shape,pos,i+1,j)==FALSE){ 
			 			/* shape blocked here */
			 			setblock(shape,pos,i,j);
			 			if(i<=high) high=i;
			 			test2(shape,pos,i,j);			 				
						if(linenum>=50&&lv==0){
							lv=1;
							speed_change=TRUE;}
						if(linenum>=100)hardlevel=TRUE;
			 			shownew();
			 			break;
			 		}
			 		else{
			 		/* shape move down here */
			 			Draw(shape,pos,i,j,TRUE);
			 			Draw(shape,pos,++i,j,FALSE);	
						if(lv==1) step++;
						LibPutDisp();
					}		
				}							
			 }					
		}
		
/* Respond to user control */		
			switch(tsts.obj){
			
			case OBJ_HIC_ESC:
			b_sound=!b_sound;
			break;
			case OBJ_LPSW_DOWN:
			case OBJ_IC_Larrow:
			if(start&&!over&&!pause){			
				/* find the position to move */	
				if((j<=1)||test(shape,pos,i,j-1)==FALSE)
				break;					
				Draw(shape,pos,i,j,TRUE); 
				Draw(shape,pos,i,--j,FALSE); 
				LibPutDisp();
			}
			break;				

			case OBJ_LPSW_UP:
			case OBJ_IC_Rarrow:
			if(start&&!over&&!pause){
				/* find the position to move */	
				if((findright(shape,pos,i,j,&right)==FALSE)||test(shape,pos,i,j+1)==FALSE)
				break;	
				Draw(shape,pos,i,j,TRUE); 
				Draw(shape,pos,i,++j,FALSE); 
				LibPutDisp();
			}
			break;
				
			case OBJ_LPSW_PUSH:
			case OBJ_IC_arrow:
				/* ratate here */			
			if(start&&!over&&!pause){
				if(shape==0||shape==7)
				break;					
				tmpos=pos;					
				if(shape==2||shape==5||shape==6||shape==8)
				tmpos=(pos+1)%4;		
				else
				tmpos=(pos+1)%2;				
				if(test(shape,tmpos,i,j)==FALSE){    /* abnormal rotate */
					if(j==1) break;
					findright(shape,tmpos,i,j,&right);	
					if(right>10){	
						/* only ratate at right border */
						if(test(shape,pos,i,(10-(right-j)))){								
							Draw(shape,pos,i,j,TRUE);
							pos=tmpos;
							j=10-(right-j);
							Draw(shape,pos,i,j,FALSE);
							LibPutDisp();
							break;
						}
						else
						break;
					}
					break;				
				}
				else{                        /* normal rotate */
					Draw(shape,pos,i,j,TRUE);
					pos=tmpos;
					Draw(shape,pos,i,j,FALSE);
					LibPutDisp();
					break;
				}					
			}
			break;
			
			case OBJ_IC_dw:
			if(tsts.act!=ACT_MAKE)	break; 
			/* find the down position */
			if(start&&!over&&!pause){
			for(n=i;n<=ROW_COUNT-1;n++)
				if(test(shape,pos,n,j)==FALSE)
				break;
			if(n-1>i){
				Draw(shape,pos,i,j,TRUE);
			 	i=n-1;
			 	Draw(shape,pos,i,j,FALSE);				
				LibPutDisp();
			}	
			setblock(shape,pos,i,j);
			if(i<=high) high=i;
			test2(shape,pos,i,j);			 				
			if(linenum>=50&&lv==0){
				lv=1;
				speed_change=TRUE;
			}	
			if(linenum>=100)hardlevel=TRUE;
			shownew();		
			}
			break;
					
			case  OBJ_IC_LEVEL:	  	/* [LEVEL] */
			if(!start||over){			
			LibGdsReverse(85, 1,117,12 );
			LibGdsDotOff(118,1);
			LibGdsDotOff(85,13);
			LibPutDisp();			
				lvtemp = LevelSet();
			LibGdsReverse(85, 1,117,12 );
			LibGdsDotOn(118,1);
			LibGdsDotOn(85,13);
			LibPutDisp();	
				if(lvtemp<3) 
				lv = lvtemp;
			}			
				break;			
			
			case  OBJ_IC_GAME:	  	/* [GAME] */				
			LibGdsReverse(  122	, 1,154,12 );
			LibGdsDotOff(155,1);
			LibGdsDotOff(122,13);
			LibPutDisp();			
			lvtemp = LevelSetgame();
			LibGdsReverse(  122	, 1,154,12  );
			LibGdsDotOn(155,1);
			LibGdsDotOn(122,13);
			LibPutDisp();
				if(lvtemp<3) {
				game = lvtemp;
				start=FALSE;
				lv=0;
				DspBk();
				}		
				break;			
				
			case OBJ_IC_PAUSE:
			if(tsts.act!=ACT_MAKE) break;
			if(start&&!over){
				pause=!pause;
				
				if(pause){
					LibGdsClr(123, 54,154,64);
					LibPutProStr(IB_PFONT1,126,57,"PAUSE", 100);
					LibGdsReverse( 122, 53,154,64 );
					LibGdsDotOff(155,53);
					LibGdsDotOff(122,65);
				}
				else{
					LibGdsBox(122, 53,155,65);
					LibGdsClr(123, 54,154,64);
					LibPutProStr(IB_PFONT1,125,56,"PAUSE", 100);
				}				
				LibPutDisp();
			}				
				break;
							
			case OBJ_IC_START:
			if(LibBlockIconClick(&TiconStart,&tsts,B_ICON_LEFT)==TRUE){				
			/* start new game */	
			for(i=0;i<ROW_COUNT;i++)
				for(j=0;j<COLUM_COUNT;j++)
					boxdsp[i][j]=FALSE;	
				start=TRUE;
				over=FALSE;
				first=FALSE;
				pause=FALSE;
				if(speed_change) lv=0;
				speed_change=FALSE;
				linenum=0;
				scorenum=0;
				high=ROW_COUNT-1;
				DspBk();
				if(game==0||game==1)
				hardlevel=FALSE;
				if(game==1)
				setgame();
				if(game==2)
				hardlevel=TRUE;
				shownew();	
			}				
				break;			
			} 
		}
	}
/******************************************************************************
*  [Title]			Draw
*  [Arguments]		int shape,int pos,int i,int j,bool clear
*  [ReturnVAL]		None
*******************************************************************************/
void Draw(int shape,int pos,int i,int j,bool clear){
	int mode;	
	if(clear)
	mode=IB_GPAND;
	else
	mode=IB_GPOR; 
	switch (shape){		
		case 0:
			 LibPutGraphO((j-1)*8+1, (i-1)*8+1, block, mode);		
		break;
		
		case 1:
			 if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, line1, mode);
			 if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, line2, mode);
		break;
				
		case 2:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, t1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, t2, mode);
			if(pos==2) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, t3, mode);
		   if(pos==3) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, t4, mode);
		break;
		
		case 3:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sa1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sa2, mode);
		break;
				
		case 4:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sb1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sb2, mode);	
		break;
		
		case 5:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, la1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, la2,mode);
			if(pos==2) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, la3, mode);
		   if(pos==3) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, la4, mode);
		break;
		
		case 6:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, lb1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, lb2, mode);
			if(pos==2) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, lb3, mode);
		   if(pos==3) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, lb4, mode);
		break;
		
		case 7:
			LibPutGraphO((j-1)*8+1, (i-1)*8+1,o, mode);
		break;
			
		case 8:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, u1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, u2, mode);
			if(pos==2) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, u3, mode);
		   if(pos==3) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, u4, mode);
		break;
		
		case 9:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sc1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sc2, mode);
		break;
		
		case 10:
			if(pos==0) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sd1, mode);
		   if(pos==1) 	LibPutGraphO((j-1)*8+1, (i-1)*8+1, sd2, mode);
		break;
		
		default:
		break;
	}
}
/******************************************************************************
*  [Title]			makepos
*  [Arguments]		int shape
*  [ReturnVAL]		int
*******************************************************************************/
int makepos(int shape){
	int i;
	if(shape==0||shape==7)
	return 0;
	if(shape==2||shape==5||shape==6||shape==8)
	i=rand()%4;
	else
	i=rand()%2;
	return i;
}
/******************************************************************************
*  [Title]			makeshape
*  [Arguments]		None
*  [ReturnVAL]		int
*******************************************************************************/
int makeshape(){
	int i;
	if(hardlevel)
	i=rand()%11;
	else
	i=rand()%7;
	return i;
}
/******************************************************************************
*  [Title]			test
*  [Arguments]		int shape,int pos,int i,int j
*  [ReturnVAL]		bool
*******************************************************************************/
bool test(int shape,int pos,int i,int j){
	switch(shape){
		case 0:
		if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||(i+1)>19||(j+1)>10)
		return FALSE;
		else
		return TRUE;
		break;			
		case 1:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i+3][j]||(i+3)>19||j>10)
			return FALSE;
			else
			return TRUE;
		}
		if (pos==1){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i][j+3]||i>19||(j+3)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;		
		case 2:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i+1][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||boxdsp[i][j+1]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==2){
			if(boxdsp[i+1][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==3){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j+1]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;		
		case 3:
		if(pos==0){
			if(boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;		
		case 4:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;		
		case 5:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||boxdsp[i][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}		
		if(pos==2){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==3){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;		
		case 6:
		if(pos==0){
			if(boxdsp[i+2][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==2){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==3){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;
		case 7:
		if(boxdsp[i][j+1]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||boxdsp[i+2][j+1]||(i+2)>19||(j+2)>10)
		return FALSE;
		else
		return TRUE;
		break;
		case 8:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i][j+2]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+2][j]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==2){
			if(boxdsp[i][j]||	boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j]||boxdsp[i+1][j+2]||(i+1)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==3){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i+1][j]||boxdsp[i+2][j]||boxdsp[i+2][j+1]||(i+2)>19||(j+1)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;
		case 9:
		if(pos==0){
			if(boxdsp[i][j+1]||boxdsp[i][j+2]||boxdsp[i+1][j+1]||boxdsp[i+2][j]||boxdsp[i+2][j+1]||(i+2)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i][j]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||boxdsp[i+2][j+2]||(i+2)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;
		case 10:
		if(pos==0){
			if(boxdsp[i][j]||boxdsp[i][j+1]||boxdsp[i+1][j+1]||boxdsp[i+2][j+1]||boxdsp[i+2][j+2]||(i+2)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		if(pos==1){
			if(boxdsp[i][j+2]||boxdsp[i+1][j]||boxdsp[i+1][j+1]||boxdsp[i+1][j+2]||boxdsp[i+2][j]||(i+2)>19||(j+2)>10)
			return FALSE;
			else
			return TRUE;
		}
		break;
		default:
		break;
	}
}			
/******************************************************************************
*  [Title]			GetSeed
*  [Arguments]		None
*  [ReturnVAL]		int
*******************************************************************************/	
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
/******************************************************************************
*  [Title]			Shownew
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/	
void shownew(){
	i=1;
	j=5;
	if(!first){
		first=TRUE;
		shape=makeshape();
		pos=makepos(shape);				
		if(test(shape,pos,i,j)==FALSE){
		over=TRUE;			
		LibPutMsgDlg("%s\r",Buf); }
		else{
		Draw(shape,pos,i,j,FALSE);				
		newshape=makeshape();
		newpos=makepos(newshape);
		LibGdsClr(86,101,120,152);
		Draw(newshape,newpos,15,12,FALSE);			
		LibPutDisp();	
		} 
	}
	else{
		shape=newshape;
		pos=newpos;
		if(test(shape,pos,i,j)==FALSE){
		over=TRUE;
		LibPutMsgDlg("%s\r",Buf); }
		else{
		Draw(newshape,pos,i,j,FALSE);				
		newshape=makeshape();
		newpos=makepos(newshape);
		LibGdsClr(86,101,120,152);
		Draw(newshape,newpos,15,12,FALSE);		
		LibPutDisp();}	
	}
}
/******************************************************************************
*  [Title]			setblock
*  [Arguments]		int shape,int pos,int i,int j
*  [ReturnVAL]		None
*******************************************************************************/	
void setblock(int shape,int pos,int i,int j){
	switch(shape){
		case 0:
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			break;
		case 1:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+3][j]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i][j+3]=TRUE;
		}
		break;
		case 2:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		if(pos==2){
			boxdsp[i+1][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==3){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
		}
		break;
		case 3:
		if(pos==0){
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		break;
		case 4:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		if(pos==1){
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
		}
		break;
		case 5:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==1){
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
			boxdsp[i][j+2]=TRUE;
		}
		if(pos==2){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==3){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j]=TRUE;
		}
		break;
		case 6:
		if(pos==0){
			boxdsp[i+2][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		if(pos==2){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
		}
		if(pos==3){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		break;
		case 7:
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		break;
		case 8:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==2){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
		}
		if(pos==3){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		break;
		case 9:
		if(pos==0){
			boxdsp[i][j+1]=TRUE;
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
			boxdsp[i+2][j+2]=TRUE;
		}
		break;
		case 10:
		if(pos==0){
			boxdsp[i][j]=TRUE;
			boxdsp[i][j+1]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+2][j+1]=TRUE;
			boxdsp[i+2][j+2]=TRUE;
		}
		if(pos==1){
			boxdsp[i][j+2]=TRUE;
			boxdsp[i+1][j]=TRUE;
			boxdsp[i+1][j+1]=TRUE;
			boxdsp[i+1][j+2]=TRUE;
			boxdsp[i+2][j]=TRUE;
		}
		break;
		default:
		break;
	}
}
/******************************************************************************
*  [Title]			findright
*  [Arguments]		int shape,int pos,int i,int j,int *right
*  [ReturnVAL]		bool
*******************************************************************************/	
bool findright(int shape,int pos,int i,int j,int *right){
	switch(shape){
		case 0:
		if((j+1)>=10) return FALSE;	
		return TRUE;
		break;
		case 1:
		if(pos==0){
		*right=j;
		if(j>=10) return FALSE;	}
		if(pos==1){
		*right=j+3;
		if((j+3)>=10) return FALSE;}	
		return TRUE;
		break;
		case 2:
		if((pos==0)||(pos==2)){
		*right=j+1;
		if((j+1)>=10) return FALSE;	}
		if((pos==1)||(pos==3)){
		*right=j+2;
		if((j+2)>=10) return FALSE;}	
		return TRUE;
		break;
		case 3:
		if(pos==0){
		*right=j+2;
		if((j+2)>=10) return FALSE;}	
		if(pos==1){
		*right=j+1;
		if((j+1)>=10) return FALSE;}
		return TRUE;	
		break;
		case 4:
		if(pos==0){
		*right=j+2;
		if((j+2)>=10) return FALSE;}	
		if(pos==1){
		*right=j+1;
		if((j+1)>=10) return FALSE;}
		return TRUE;	
		break;
		case 5:
		if((pos==0)||(pos==2)){
		*right=j+1;
		if((j+1)>=10) return FALSE;}	
		if((pos==1)||(pos==3)){
		*right=j+2;
		if((j+2)>=10) return FALSE;}
		return TRUE;
		break;
		case 6:
		if((pos==0)||(pos==2)){
		*right=j+1;
		if((j+1)>=10) return FALSE;}	
		if((pos==1)||(pos==3)){
		*right=j+2;
		if((j+2)>=10) return FALSE;}	
		return TRUE;
		break;
		case 7:
		case 9:
		case 10:
		*right=j+2;
		if((j+2)>=10) return FALSE;
		return TRUE;
		break;
		case 8:
		if(pos==0||pos==2){
		*right=j+2;
		if((j+2)>=10) return FALSE;}
		if((pos==1)||(pos==3)){
		*right=j+1;
		if((j+1)>=10) return FALSE;}
		return TRUE;
		break;
		default:
		return TRUE;
	}
}
/******************************************************************************
*  [Title]			test2
*  [Arguments]		int shape,int pos,int i,int j
*  [ReturnVAL]		None
*******************************************************************************/	
void test2(int shape,int pos,int i,int j){
	int top,bottom,num,n,m,topscroll,endscroll,count;
	num=0;
	count=0;
	top=i;
	topscroll=high;
	switch(shape){
		case 0:
		bottom=top+1;
		break;
		case 1:
		if(pos==0) bottom=top+3;
		if(pos==1) bottom=top;
		break;		
		case 3:
		case 4:
		if(pos==0) bottom=top+1;
		if(pos==1) bottom=top+2;
		break;
		case 2:
		case 5:
		case 6:
		if((pos==0)||(pos==2)) bottom=top+2;
		if((pos==1)||(pos==3)) bottom=top+1;
		break;
		case 7:
		case 8:
		case 9:
		case 10:
		bottom=top+2;
		break;
		default:
		break;
	}
	for(n=bottom;n>=top;n--){
		for(m=1;m<=10;m++)
			if(boxdsp[n][m])
			num++;
		if(num==10){
			count++;
			linenum++;
			for(m=1;m<=10;m++)
				boxdsp[n][m]=FALSE;
				drawblank(n);			
		/* start to scroll down the screen */	
		endscroll=n-1;
		for(;endscroll>=topscroll;endscroll--){
			for(m=1;m<=10;m++){
				if(boxdsp[endscroll][m]){
					LibPutGraph((m-1)*8+1,(endscroll)*8+1,sblock);	
					boxdsp[endscroll+1][m]=TRUE;					
					LibPutGraph((m-1)*8+1,(endscroll-1)*8+1,blankblock);	
					boxdsp[endscroll][m]=FALSE;					
				}			
			}
		}
				
		/* end of scroll */	
		if(b_sound){
		LibBuzzerOn(IB_BEEP3_SET);
		LibWait(IB_125MWAIT);
		LibBuzzerOff();}
		else LibWait(IB_125MWAIT);
		n++;	
		}
		num=0;
	}
	if(count==1) scorenum+=10+lv*5;
	if(count<4&&count>1) scorenum+=(20+lv*5)*count;
	if(count>=4)	scorenum+=(30+lv*5)*count;
	if(scorenum>=10000) scorenum=0;
	itoa(scorenum,score,10);
	LibPutProStr(IB_CG57FONT,132,40,score,130);	
	if(linenum>=10000) linenum=0;
	itoa(linenum,line,10);
	LibPutProStr(IB_CG57FONT,132,30,line,130);
	high+=count;
}
/******************************************************************************
*  [Title]			drawblank
*  [Arguments]		int n
*  [ReturnVAL]		None
*******************************************************************************/	
void drawblank(int n){
	LibPutGraph(1, (n-1)*8+1,blank);			
}
/******************************************************************************
*  [Title]			LevelSet
*  [Arguments]		None
*  [ReturnVAL]	   byte
*******************************************************************************/		
static byte LevelSet()
{
	byte    rtv[3+1] = {0,1,2,0xff};
	byte    cmnt[3*(7+1)],np;
	SLW2_ST win_prm;
	byte    sel_ret;
	byte    t_tbl[3][7+1];
	int     i,k,len;
	/* data */
	strcpy(t_tbl[0],"Speed1");
	strcpy(t_tbl[1],"Speed2");
	strcpy(t_tbl[2],"Speed3");
	
	np = lv;
	/* disp Buff Making */
	for(i=k=0;i<3;i++,k++){
	    len = strlen(t_tbl[i]);
	    memcpy(&cmnt[k],t_tbl[i],len);
	    k += len;
	    if(i<1) cmnt[k] = 0xfe;
	    else    cmnt[k] = 0x00;     /* End Data */
	}
	/* select window */
	
	win_prm.x    =  85;
	win_prm.y    =  12;
	win_prm.xs   =  40;
	win_prm.ity  =   9;
	win_prm.np   = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv  = rtv;
	win_prm.t_xs =   4;
	sel_ret = LibSelWindowExt(&win_prm);
	return(sel_ret);
}				
/******************************************************************************
*  [Title]			LevelSetgame
*  [Arguments]		None
*  [ReturnVAL]		byte
*******************************************************************************/		
static byte LevelSetgame()
{
	byte    rtv[3+1] = {0,1,2,0xff};
	byte    cmnt[3*(7+1)],np;
	SLW2_ST win_prm;
	byte    sel_ret;
	byte    t_tbl[3][7+1];
	int     i,k,len;
	/* data */
	
	strcpy(t_tbl[0],"Game1");
	strcpy(t_tbl[1],"Game2");
	strcpy(t_tbl[2],"Game3");
	np = game;
	/* disp Buff Making */
	for(i=k=0;i<3;i++,k++){
	    len = strlen(t_tbl[i]);
	    memcpy(&cmnt[k],t_tbl[i],len);
	    k += len;
	    if(i<1) cmnt[k] = 0xfe;
	    else    cmnt[k] = 0x00;     /* End Data */
	}
	/* select window */
	
	win_prm.x    =  120;
	win_prm.y    =  12;
	win_prm.xs   =  40;
	win_prm.ity  =   9;
	win_prm.np   = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv  = rtv;
	win_prm.t_xs =   4;
	sel_ret = LibSelWindowExt(&win_prm);
	return(sel_ret);
}				
/******************************************************************************
*  [Title]			setgame
*  [Arguments]		None
*  [ReturnVAL]		None
*******************************************************************************/				
void setgame(){
	int x,y,k;
	for(k=0;k<50;k++){
	x=rand()%7+13;
	y=rand()%10+1;
	LibPutGraph((y-1)*8+1,(x-1)*8+1,sblock);	
	boxdsp[x][y]=TRUE;
	}
}
