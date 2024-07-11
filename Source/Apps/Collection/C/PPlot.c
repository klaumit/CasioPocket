#include	<stdrom.h>
#include	<string.h>
#include	"define.h"
#include	"libc.h"
#include 	"l_libc.h"


#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time		*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound				*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format				*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity				*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0020	/* Contrast				*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0040	/* Touch_Panel_Alignment*/

/* own control defines */
#define TCH_SETUP		0xc001
#define TCH_SETUP_OK		0xc002
#define	TCH_SETUP_FUNCTION_TXT	0xc003
#define	TCH_SETUP_XMIN_TXT	0xc004
#define	TCH_SETUP_XMAX_TXT	0xc005
#define	TCH_SETUP_YMIN_TXT	0xc006
#define	TCH_SETUP_YMAX_TXT	0xc007

/* mathematical constants */
#define MATH_PI 3.1415926536
#define MATH_E  2.71828182845

/* own type definitions */
typedef struct s_term{
	int typ;		/* 	1=zahl
					2='x'
					3='+'
					4='-'
					5='*'
					6='/'
					7='^'
					8='('
					9=')'
					10=sin
					11=cos	*/
	double value;		/* double value des termteils 	*/
};

typedef struct setupdata{
	double xmin,xmax,ymin,ymax;
	char function[1024];
};

/* prototypes */
int 	isnumeric(char c);
int 	parse(char *function);
double 	evaluate(double x,int pos);
void 	PlotProc();
void 	mainprogram();
double 	power(double x, int n);
void 	initMainTchStack();
void 	initSetupTchStack();
double 	StringToDouble(char string[10]);
long 	fak(int x);
double 	sin(double x);
double 	cos(double x);

/* button defintions */
TCHTBL far TchMainScreenButtons[]={
	2,2,29,12,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	TCH_SETUP,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};  

TCHTBL far TchSetupDialogItems[]={
	81,76,129,85,
	ACT_ICON,
	TCH_SETUP_OK,
	0x0000,

	16,19,104,29,
	ACT_TXTAREA,
	TCH_SETUP_FUNCTION_TXT,
	0x0000,

	16,46,39,57,
	ACT_TXTAREA,
	TCH_SETUP_XMIN_TXT,
	0x0000,

	51,46,74,57,
	ACT_TXTAREA,
	TCH_SETUP_XMAX_TXT,
	0x0000,

	86,46,114,57,
	ACT_TXTAREA,
	TCH_SETUP_YMIN_TXT,
	0x0000,

	126,46,151,59,
	ACT_TXTAREA,
	TCH_SETUP_YMAX_TXT,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TchSetupButton       = { &TchMainScreenButtons[0], NULL, NULL, 0x00 };
T_ICON far TchSetupOKButton     = { &TchSetupDialogItems[0], NULL, NULL, 0x00 };

/**************************************************************************/
/* globale variablen */
struct setupdata setup;
struct s_term term[100];
int termnr;
/***************************************************************************/
int isnumeric(char c){
	if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9' || c=='.')
		return 1;
	return 0;
}
/***************************************************************************/
long fak(int x){
	long result=1;
	int i=0;

	for(i=1;i<=x;i++) result*=i;

	return result;
}
/***************************************************************************/
double 	sin(double x){
	double result=0;
	double pi=3.141592654;
	int negpos=0;
	
	while(x>pi)  { negpos++; x-=pi; }
	while(x<-pi) { negpos++; x+=pi; }
	result=x-(power(x,3)/fak(3))+(power(x,5)/fak(5))-(power(x,7)/fak(7))+(power(x,9)/fak(9))-(power(x,11)/fak(11))+(power(x,13)/fak(13));
	if(negpos%2) result*=-1;
	return result;
}

/***************************************************************************/
double 	cos(double x){
	double result=0;
	double pi=3.141592654;
	int negpos=0;

	while(x>pi)  { negpos++; x-=pi; }
	while(x<-pi) { negpos++; x+=pi; }
	result=1-(power(x,2)/fak(2))+(power(x,4)/fak(4))-(power(x,6)/fak(6))+(power(x,8)/fak(8))-(power(x,10)/fak(10))+(power(x,12)/fak(12));
	if(negpos%2) result*=-1;
	return result;
}

/***************************************************************************/
double power(double x, int n){
    double tmp;
	int i;

	if(n==0) return 1;
	tmp=x;
	for(i=1;i<n;i++) tmp*=x;
	return tmp;
}

/***************************************************************************/
int parse(char function[1024]){
	int i=0,z=0;	/* zählvariablen				*/
	int nachkomma;	/* nachkommastelle nr.				*/
	int spos;	/* position des jew. term strings		*/
	double dtmp;	/* temp für double auswertung			*/
	int termdone;	/* ist der aktuelle termteil abgearbeitet	*/	

	termnr=0;
	memset(term,0x00,sizeof(term));

	while(function[i]){
		spos=0;
		termdone=0;
		nachkomma=0;

		/* sinus */
		if( (function[i]=='s' || function[i]=='S') &&
		    (function[i+1]=='i' || function[i+1]=='I') &&
		    (function[i+2]=='n' || function[i+1]=='N')    ){
			term[termnr].typ=10;
			termnr++;
			i+=3;
		}

		/* cosinus */
		if( (function[i]=='c' || function[i]=='C') &&
		    (function[i+1]=='o' || function[i+1]=='O') &&
		    (function[i+2]=='s' || function[i+1]=='S')    ){
			term[termnr].typ=11;
			termnr++;
			i+=3;
		}

		/* mathematical constants */
		else if( (function[i]=='p' || function[i]=='P') &&
		    (function[i+1]=='i' || function[i+1]=='I') ) {
			term[termnr].typ=1;
			term[termnr].value=MATH_PI;
			termnr++;
			i+=2;
                }
		else if(function[i]=='e' || function[i]=='E') {			
			term[termnr].typ=1;
			term[termnr].value=MATH_E;
			termnr++;
			i++;
		}

		/* Klammern */
		else if(function[i]=='(') {
			term[termnr].typ=8;		
			termnr++;
			i++;
		}
		else if(function[i]==')') {
			term[termnr].typ=9;		
			termnr++;
			i++;
		}

		/* wenn der termteil eine zahl ist */
		else if(isnumeric(function[i])) {
			while(isnumeric(function[i])){
				term[termnr].typ=1;
				if(function[i]=='.') {
					nachkomma=1;
				}else{
					if(!nachkomma) {
					term[termnr].value*=10;
					term[termnr].value+=function[i]-48;
					} else {
						dtmp=function[i]-48;
						for(z=0;z<nachkomma;z++) dtmp/=10;
						term[termnr].value+=dtmp;
						nachkomma++;
					}
				}
				i++;
				spos++;
			}
			termnr++;
		}
	
		/* wenn der termteil 'x' darstellt */
		else if(function[i]=='x' || function[i]=='X') {
			term[termnr].typ=2;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn der termteil '+' darstellt */
		else if(function[i]=='+') {
			term[termnr].typ=3;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn der termteil '-' darstellt */
		else if(function[i]=='-') {
			term[termnr].typ=4;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn der termteil '*' darstellt */
		else if(function[i]=='*') {
			term[termnr].typ=5;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn der termteil '/' darstellt */
		else if(function[i]=='/') {
			term[termnr].typ=6;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn der termteil '^' darstellt */
		else if(function[i]=='^') {
			term[termnr].typ=7;
			term[termnr].value=0;
			i++;
			termnr++;
		}

		/* wenn unerkanntes zeichen */
		else i++;		
	}
	
	return 0;
}

/***************************************************************************/
double 	evaluate(double x,int pos){
	int i;				/* zählvariable */
	double result;			/* ergebnis	*/
	struct tterm{
		int typ;		/* 1=zahl 2='x' 3='+' 4='-' 5='*' 6='/' 7='^'	*/
		double value;		/* double value des termteils			*/
	} tmpterm[100], tmpterm2[100];
	int tmptermnr=0;
	int tmptermnr2;

	/* evaluate terms in brackets */
	memset(tmpterm2,0x00,sizeof(tmpterm2));
	tmptermnr2=0;
	for(i=pos;i<=termnr;i++)
		if(pos!=0 && term[i].typ==9) break;
		else if(term[i].typ!=8){
			tmpterm2[tmptermnr2].typ=term[i].typ;
			tmpterm2[tmptermnr2].value=term[i].value;
			tmptermnr2++;
		} else {
			tmpterm2[tmptermnr2].typ=1;
			i++;
			tmpterm2[tmptermnr2].value=evaluate(x,i);
			tmptermnr2++;
			while(term[i].typ!=9) { i++; if(i>100) break; }
		}

	/* evaluate 'sin' expressions */
	memset(tmpterm,0x00,sizeof(tmpterm));
	tmptermnr=0;
	for(i=0;i<tmptermnr2;i++)
		if(tmpterm2[i].typ==9) break;
		else if(tmpterm2[i].typ!=10){
			tmpterm[tmptermnr].typ=tmpterm2[i].typ;
			tmpterm[tmptermnr].value=tmpterm2[i].value;
			tmptermnr++;
		}else{
			if(tmpterm2[i+1].typ==1) tmpterm[tmptermnr].value=sin(tmpterm2[i+1].value);
			else if(tmpterm2[i+1].typ==2) tmpterm[tmptermnr].value=sin(x);
			tmpterm[tmptermnr].typ=1;
			tmptermnr++;
			i++;
		}


	/* evaluate 'cos' expressions */
	memset(tmpterm2,0x00,sizeof(tmpterm2));
	tmptermnr2=0;
	for(i=0;i<tmptermnr;i++)
		if(tmpterm[i].typ==9) break;
		else if(tmpterm[i].typ!=11){
			tmpterm2[tmptermnr2].typ=tmpterm[i].typ;
			tmpterm2[tmptermnr2].value=tmpterm[i].value;
			tmptermnr2++;
		}else{
			if(tmpterm[i+1].typ==1) tmpterm2[tmptermnr2].value=cos(tmpterm[i+1].value);
			else if(tmpterm[i+1].typ==2) tmpterm2[tmptermnr2].value=cos(x);
			tmpterm2[tmptermnr2].typ=1;
			tmptermnr2++;
			i++;
		}


	/* evaluate '^' expressions	*/
	memset(tmpterm,0x00,sizeof(tmpterm));
	tmptermnr=0;
	for(i=0;i<tmptermnr2;i++)
		if(tmpterm2[i].typ==9) break;
		else if(tmpterm2[i+1].typ!=7){
			tmpterm[tmptermnr].typ=tmpterm2[i].typ;
			tmpterm[tmptermnr].value=tmpterm2[i].value;
			tmptermnr++;
		}else{
			if     (tmpterm2[i].typ==1 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=power(tmpterm2[i].value,(int)tmpterm2[i+2].value);
			else if(tmpterm2[i].typ==1 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=power(tmpterm2[i].value,(int)x);
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=power(x,(int)tmpterm2[i+2].value);
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=power(x,(int)x);
			tmpterm[tmptermnr].typ=1;
			i+=2;
			while(tmpterm2[i+1].typ==5){
				double dt=tmpterm[tmptermnr].value;
				if(tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=power(dt,(int)tmpterm2[i+2].value);
				else if(tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=power(dt,(int)x);
				i+=2;
			}
			tmptermnr++;
		}

	/* evaluate '/' expressions 	*/
	memset(tmpterm2,0x00,sizeof(tmpterm2));
	tmptermnr2=0;
	for(i=0;i<tmptermnr;i++)
		if(tmpterm[i].typ==9) break;
		else if(tmpterm[i+1].typ!=6){
			tmpterm2[tmptermnr2].typ=tmpterm[i].typ;
			tmpterm2[tmptermnr2].value=tmpterm[i].value;
			tmptermnr2++;
		}else{
			if     (tmpterm[i].typ==1 && tmpterm[i+2].typ==1) tmpterm2[tmptermnr2].value=tmpterm[i].value/tmpterm[i+2].value;
			else if(tmpterm[i].typ==1 && tmpterm[i+2].typ==2) tmpterm2[tmptermnr2].value=tmpterm[i].value/x;
			else if(tmpterm[i].typ==2 && tmpterm[i+2].typ==1) tmpterm2[tmptermnr2].value=x/tmpterm[i+2].value;
			else if(tmpterm[i].typ==2 && tmpterm[i+2].typ==2) tmpterm2[tmptermnr2].value=x/x;
			tmpterm2[tmptermnr2].typ=1;
			i+=2;
			while(tmpterm[i+1].typ==6){
				if(tmpterm[i+2].typ==1) tmpterm2[tmptermnr].value/=tmpterm[i+2].value;
				else if(tmpterm[i+2].typ==2) tmpterm2[tmptermnr].value/=x;
				i+=2;
			}
			tmptermnr2++;
		}

	/* evaluate '*' expressions	*/
	memset(tmpterm,0x00,sizeof(tmpterm2));
	tmptermnr=0;
	for(i=0;i<tmptermnr2;i++)
		if(tmpterm2[i].typ==9) break;
		else if(tmpterm2[i+1].typ!=5){
			tmpterm[tmptermnr].typ=tmpterm2[i].typ;
			tmpterm[tmptermnr].value=tmpterm2[i].value;
			tmptermnr++;
		}else{
			if     (tmpterm2[i].typ==1 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=tmpterm2[i].value*tmpterm2[i+2].value;
			else if(tmpterm2[i].typ==1 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=tmpterm2[i].value*x;
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=x*tmpterm2[i+2].value;
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=x*x;
			tmpterm[tmptermnr].typ=1;
			i+=2;
			while(tmpterm2[i+1].typ==5){
				if(tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value*=tmpterm2[i+2].value;
				else if(tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value*=x;
				i+=2;
			}
			tmptermnr++;
		}

	/* evaluate '-' expressions	*/
	memset(tmpterm2,0x00,sizeof(tmpterm2));
	tmptermnr2=0;
	for(i=0;i<tmptermnr;i++)
		if(tmpterm[i].typ==9) break;
		else if(tmpterm[i+1].typ!=4){
			tmpterm2[tmptermnr2].typ=tmpterm[i].typ;
			tmpterm2[tmptermnr2].value=tmpterm[i].value;
			tmptermnr2++;
		}else{
			if     (tmpterm[i].typ==1 && tmpterm[i+2].typ==1) tmpterm2[tmptermnr2].value=tmpterm[i].value-tmpterm[i+2].value;
			else if(tmpterm[i].typ==1 && tmpterm[i+2].typ==2) tmpterm2[tmptermnr2].value=tmpterm[i].value-x;
			else if(tmpterm[i].typ==2 && tmpterm[i+2].typ==1) tmpterm2[tmptermnr2].value=x-tmpterm[i+2].value;
			else if(tmpterm[i].typ==2 && tmpterm[i+2].typ==2) tmpterm2[tmptermnr2].value=x-x;
			tmpterm2[tmptermnr2].typ=1;
			i+=2;
			while(tmpterm[i+1].typ==4){
				if(tmpterm[i+2].typ==1) tmpterm2[tmptermnr].value-=tmpterm[i+2].value;
				else if(tmpterm[i+2].typ==2) tmpterm2[tmptermnr].value-=x;
				i+=2;
			}
			tmptermnr2++;
		}

	/* evaluate '+' expressions	*/
	memset(tmpterm,0x00,sizeof(tmpterm2));
	tmptermnr=0;
	for(i=0;i<tmptermnr2;i++)
		if(tmpterm2[i].typ==9) break;
		else if(tmpterm2[i+1].typ!=3){
			tmpterm[tmptermnr].typ=tmpterm2[i].typ;
			tmpterm[tmptermnr].value=tmpterm2[i].value;
			tmptermnr++;
		}else{
			if     (tmpterm2[i].typ==1 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=tmpterm2[i].value+tmpterm2[i+2].value;
			else if(tmpterm2[i].typ==1 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=tmpterm2[i].value+x;
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value=x+tmpterm2[i+2].value;
			else if(tmpterm2[i].typ==2 && tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value=x+x;
			tmpterm[tmptermnr].typ=1;			
			i+=2;
			while(tmpterm2[i+1].typ==3){
				if(tmpterm2[i+2].typ==1) tmpterm[tmptermnr].value+=tmpterm2[i+2].value;
				else if(tmpterm2[i+2].typ==2) tmpterm[tmptermnr].value+=x;
				i+=2;
			}
			tmptermnr++;
		}

	if(tmpterm[0].typ=1) result=tmpterm[0].value;
	else if(tmpterm[0].typ=2) result=x;
	else result=0;

	return result;
}

/* ------------------------------------------------------------------------ */
/* System-Menu-Functions                                                    */
/* ------------------------------------------------------------------------ */
void SysMenuFunc(word sel){
	switch(sel){

		case PDWN_SYS_DTTM:		/* Set_date/time	*/
			LibFuncDateTime();
			break;

		case PDWN_SYS_SOUND:		/* Sound		*/
			LibFuncSound(0);
			break;

		case PDWN_SYS_FORMT:		/* Format		*/
			LibFuncFormat();
			break;

		case PDWN_SYS_CAPCT:		/* Capacity		*/
			LibFuncCapa();
			break;

		case PDWN_SYS_MMANE:		/* Memory_management	*/
			LibFuncMemoryManagement();
			break;

		case PDWN_SYS_CNTRS:		/* Contrast		*/
			LibFuncContrast();
			break;

		case PDWN_SYS_TPA:		/* Touch_Panel_Alignment*/
			LibFuncDigitizer();
			break;

		default:
			break;
	}
}



/* ------------------------------------------------------------------------ */
/* Initialize and use Menu                                                  */
/* ------------------------------------------------------------------------ */
void DoMenu(){
	word	sel;
	word	sys[] = {
			38,			/* Set_date/time	*/
			33,			/* Sound		*/
			39,			/* Format		*/
			34,			/* Capacity		*/
			35,			/* Memory_management	*/
/*			36,			/* Language		*/
			37,			/* Contrast		*/
			40,			/* Touch_Panel_Alignment*/
/*			41,			/* Data_communication	*/
			PDNTBLEND,
		};

	word	edt[]={PDNTBLEND};
	word	opt[]={PDNTBLEND};

	LibPullDownInit(edt,sys,opt);	
	LibPullDownAtrSet(PULLDOWN_HTNDSP,PULLDOWN_SYSTEM,PULLDOWN_SYSTEM|0x0100);
	sel=LibPullDown();

	if(sel & PULLDOWN_SYSTEM) SysMenuFunc(sel);
}

/* ------------------------------------------------------------------------ */
/* drawMainScreen		                                            */
/* ------------------------------------------------------------------------ */
void drawMainScreen(){
	LibClrDisp();				/* clear display */
	LibGdsBox(2,2,30,13);			/* draw setup button */
	LibPutProStr(IB_PFONT1,4,4,"Setup",30);	
	LibPutDisp();				/* show updated display */
}

void initSetupTchStack(){
	LibTchStackClr();
	LibTchStackPush(NULL);
	LibTchStackPush(TchHardIcon);		/* Hardware Icons   */
	LibTchStackPush(TchSetupDialogItems); /* Dialog Button(s) */
}

/* ------------------------------------------------------------------------ */
/* GetDoubleFromString                    		                    */
/* ------------------------------------------------------------------------ */
double StringToDouble(char string[10]){
	int i=0,z=0,nachkomma=0;
	double result=0;
	double dtmp=0;
	int negativ=0;

	while(string[i]){
		if(string[i]=='.') {
			nachkomma=1;
		}
		else if(string[i]=='-') {
			negativ=1;
		}else{
			if(!nachkomma) {
				result*=10;
				result+=string[i]-48;
			} else {
				dtmp=string[i]-48;
				for(z=0;z<nachkomma;z++) dtmp/=10;
				result+=dtmp;
				nachkomma++;
			}
		}
		i++;
	}
	if(negativ) result*=-1;
	return result;
}

/* ------------------------------------------------------------------------ */
/* SetupProc     		                                            */
/* ------------------------------------------------------------------------ */
void SetupProc(){
	TXTP		FTxtInput;	/* Declaration of structure for text input */
	TXTP		FTxtInput2;	/* Declaration of structure for text input */
	TXTP		FTxtInput3;	/* Declaration of structure for text input */
	TXTP		FTxtInput4;	/* Declaration of structure for text input */
	TXTP		FTxtInput5;	/* Declaration of structure for text input */
	TCHTBL 		TchTxtBuf[3];	/* Text touch area table */
	word		GdTbl[1];	/* Guidance comment table */
	T_SCR_POS	ComScr;		/* Scroll bar position information */
	byte		key_cd;
	TCHSTS		tsts;
	byte		input_end;
	char 		tmpfunction[1024]; /* function txt buffer */
	TXTP		*CurrentInput;
	char		sxmin[10];
	char		sxmax[10];
	char		symin[10];
	char		symax[10];


	/* initial data */
	memset(tmpfunction,0x00,sizeof(tmpfunction));
	strcpy(tmpfunction,setup.function);
	memset(sxmin,0x00,sizeof(sxmin));
	memset(sxmax,0x00,sizeof(sxmax));
	memset(symin,0x00,sizeof(symin));
	memset(symax,0x00,sizeof(symax));
	sprintf(sxmin,"%f",setup.xmin);
	sprintf(sxmax,"%f",setup.xmax);
	sprintf(symin,"%f",setup.ymin);
	sprintf(symax,"%f",setup.ymax);

	/* open setup window */
	LibOpenWindow(0,0,160,160);

	/* sraw Plot Graph Button */
	LibPutProStr(IB_PFONT1,82,77,"Plot Graph",50);
	LibGdsBox(80,75,130,86);

	/* draw Function-Term TextBox */
	LibPutProStr(IB_PFONT1,15,8,"Function Term",100);
	LibGdsBox(15,18,105,30);

	/* draw xmin TextBox */
	LibPutProStr(IB_PFONT1,15,35,"xmin",25);
	LibGdsBox(15,45,40,56);

	/* draw xmax TextBox */
	LibPutProStr(IB_PFONT1,50,35,"xmax",25);
	LibGdsBox(50,45,75,56);

	/* draw ymin TextBox */
	LibPutProStr(IB_PFONT1,85,35,"ymin",25);
	LibGdsBox(85,45,115,56);

	/* draw ymax TextBox */
	LibPutProStr(IB_PFONT1,125,35,"ymax",25);
	LibGdsBox(125,45,150,56);
	LibPutDisp();

	/* Function Input TxtBox */
	GdTbl[0]	= 0xffff;	/* No Guidance comment */
	FTxtInput.st_x	= 20;		/* Start coordinate (X) of text display */
	FTxtInput.st_y	= 20;		/* Start coordinate (Y) of text display */
	FTxtInput.ed_x	= 100;		/* End coordinate (X) of text display  */
	FTxtInput.it_y	= 9;		/* Text display line spacing (Y) */
	FTxtInput.MAXGYO= 1;		/* Number of text display lines  */
	FTxtInput.font	= IB_PFONT1;	/* Display font type  */
	FTxtInput.csen	= TRUE;		/* Cursor display enabled(Keyword registration enabled) */
	FTxtInput.rtnen	= 0x02;		/* CR code display enabled  */
	FTxtInput.maxmj	= 1024;		/* Maximum number of allowable input characters. */
	FTxtInput.txbf	= tmpfunction;	/* Designation of text buffer address  */
	FTxtInput.gdcmt	= GdTbl;	/* Guidance comment table  */
	FTxtInput.txtobj= NULL;		/* Object when text area is touched.  */
	FTxtInput.sbrobj= NULL;		/* Object when the scroll bar is touched. */
	FTxtInput.tchtb	= TchTxtBuf;	/* Pointer for text scroll bar area  */
	FTxtInput.s_pos	= &ComScr;	/* Pointer for text and scroll bar information */

	/* xmin Input TxtBox */
	GdTbl[0]	 = 0xffff;	/* No Guidance comment */
	FTxtInput2.st_x	 = 17;		/* Start coordinate (X) of text display */
	FTxtInput2.st_y	 = 47;		/* Start coordinate (Y) of text display */
	FTxtInput2.ed_x	 = 39;		/* End coordinate (X) of text display  */
	FTxtInput2.it_y	 = 9;		/* Text display line spacing (Y) */
	FTxtInput2.MAXGYO= 1;		/* Number of text display lines  */
	FTxtInput2.font	 = IB_PFONT1;	/* Display font type  */
	FTxtInput2.csen	 = TRUE;	/* Cursor display enabled(Keyword registration enabled) */
	FTxtInput2.rtnen = 0x02;	/* CR code display enabled  */
	FTxtInput2.maxmj = 10;		/* Maximum number of allowable input characters. */
	FTxtInput2.txbf	 = sxmin;	/* Designation of text buffer address  */
	FTxtInput2.gdcmt = GdTbl;	/* Guidance comment table  */
	FTxtInput2.txtobj= NULL;	/* Object when text area is touched.  */
	FTxtInput2.sbrobj= NULL;	/* Object when the scroll bar is touched. */
	FTxtInput2.tchtb = TchTxtBuf;	/* Pointer for text scroll bar area  */
	FTxtInput2.s_pos = &ComScr;	/* Pointer for text and scroll bar information */

	/* xmax Input TxtBox */
	GdTbl[0]	 = 0xffff;	/* No Guidance comment */
	FTxtInput3.st_x	 = 52;		/* Start coordinate (X) of text display */
	FTxtInput3.st_y	 = 47;		/* Start coordinate (Y) of text display */
	FTxtInput3.ed_x	 = 74;		/* End coordinate (X) of text display  */
	FTxtInput3.it_y	 = 9;		/* Text display line spacing (Y) */
	FTxtInput3.MAXGYO= 1;		/* Number of text display lines  */
	FTxtInput3.font	 = IB_PFONT1;	/* Display font type  */
	FTxtInput3.csen	 = TRUE;	/* Cursor display enabled(Keyword registration enabled) */
	FTxtInput3.rtnen = 0x02;	/* CR code display enabled  */
	FTxtInput3.maxmj = 10;		/* Maximum number of allowable input characters. */
	FTxtInput3.txbf	 = sxmax;	/* Designation of text buffer address  */
	FTxtInput3.gdcmt = GdTbl;	/* Guidance comment table  */
	FTxtInput3.txtobj= NULL;	/* Object when text area is touched.  */
	FTxtInput3.sbrobj= NULL;	/* Object when the scroll bar is touched. */
	FTxtInput3.tchtb = TchTxtBuf;	/* Pointer for text scroll bar area  */
	FTxtInput3.s_pos = &ComScr;	/* Pointer for text and scroll bar information */

	/* ymin Input TxtBox */
	GdTbl[0]	 = 0xffff;	/* No Guidance comment */
	FTxtInput4.st_x	 = 87;		/* Start coordinate (X) of text display */
	FTxtInput4.st_y	 = 47;		/* Start coordinate (Y) of text display */
	FTxtInput4.ed_x	 = 114;		/* End coordinate (X) of text display  */
	FTxtInput4.it_y	 = 9;		/* Text display line spacing (Y) */
	FTxtInput4.MAXGYO= 1;		/* Number of text display lines  */
	FTxtInput4.font	 = IB_PFONT1;	/* Display font type  */
	FTxtInput4.csen	 = TRUE;	/* Cursor display enabled(Keyword registration enabled) */
	FTxtInput4.rtnen = 0x02;	/* CR code display enabled  */
	FTxtInput4.maxmj = 10;		/* Maximum number of allowable input characters. */
	FTxtInput4.txbf	 = symin;	/* Designation of text buffer address  */
	FTxtInput4.gdcmt = GdTbl;	/* Guidance comment table  */
	FTxtInput4.txtobj= NULL;	/* Object when text area is touched.  */
	FTxtInput4.sbrobj= NULL;	/* Object when the scroll bar is touched. */
	FTxtInput4.tchtb = TchTxtBuf;	/* Pointer for text scroll bar area  */
	FTxtInput4.s_pos = &ComScr;	/* Pointer for text and scroll bar information */

	/* ymax Input TxtBox */
	GdTbl[0]	 = 0xffff;	/* No Guidance comment */
	FTxtInput5.st_x	 = 127;		/* Start coordinate (X) of text display */
	FTxtInput5.st_y	 = 47;		/* Start coordinate (Y) of text display */
	FTxtInput5.ed_x	 = 149;		/* End coordinate (X) of text display  */
	FTxtInput5.it_y	 = 9;		/* Text display line spacing (Y) */
	FTxtInput5.MAXGYO= 1;		/* Number of text display lines  */
	FTxtInput5.font	 = IB_PFONT1;	/* Display font type  */
	FTxtInput5.csen	 = TRUE;	/* Cursor display enabled(Keyword registration enabled) */
	FTxtInput5.rtnen = 0x02;	/* CR code display enabled  */
	FTxtInput5.maxmj = 10;		/* Maximum number of allowable input characters. */
	FTxtInput5.txbf	 = symax;	/* Designation of text buffer address  */
	FTxtInput5.gdcmt = GdTbl;	/* Guidance comment table  */
	FTxtInput5.txtobj= NULL;	/* Object when text area is touched.  */
	FTxtInput5.sbrobj= NULL;	/* Object when the scroll bar is touched. */
	FTxtInput5.tchtb = TchTxtBuf;	/* Pointer for text scroll bar area  */
	FTxtInput5.s_pos = &ComScr;	/* Pointer for text and scroll bar information */

	initSetupTchStack();

	CurrentInput=&FTxtInput;

	LibTxtInit(&FTxtInput);
	LibTxtTchSet(&FTxtInput);

	LibTxtInit(&FTxtInput2);
	LibTxtTchSet(&FTxtInput2);

	LibTxtInit(&FTxtInput3);
	LibTxtTchSet(&FTxtInput3);

	LibTxtInit(&FTxtInput4);
	LibTxtTchSet(&FTxtInput4);

	LibTxtInit(&FTxtInput5);
	LibTxtTchSet(&FTxtInput5);

	/* show all updated txt input fields */
	LibTxtDsp(&FTxtInput);
	LibTxtDsp(&FTxtInput2);
	LibTxtDsp(&FTxtInput3);
	LibTxtDsp(&FTxtInput4);
	LibTxtDsp(&FTxtInput5);

	LibDispKey();
	input_end=FALSE;

	while(input_end==FALSE){
		LibTxtDsp(CurrentInput);
		LibPutDisp();
		key_cd=LibGetKeyM(&tsts);	/* get event	*/
		if(key_cd==KEY_NONE){		/* Not keyboard */
			switch(tsts.obj){

				case OBJ_HIC_MBAR:
					DoMenu();
					break;

				case TCH_SETUP_OK:
					if(LibBlockIconClick(&TchSetupOKButton,&tsts,B_ICON_RIGHT)==TRUE){
						memset(setup.function,0x00,sizeof(setup.function));
						strcpy(setup.function,tmpfunction);
						setup.xmin=StringToDouble(sxmin);
						setup.xmax=StringToDouble(sxmax);
						setup.ymin=StringToDouble(symin);
						setup.ymax=StringToDouble(symax);
						input_end=TRUE;
					}
					break;

				case TCH_SETUP_FUNCTION_TXT:
					CurrentInput=&FTxtInput;
					break;

				case TCH_SETUP_XMIN_TXT:
					CurrentInput=&FTxtInput2;
					break;

				case TCH_SETUP_XMAX_TXT:
					CurrentInput=&FTxtInput3;
					break;

				case TCH_SETUP_YMIN_TXT:
					CurrentInput=&FTxtInput4;
					break;

				case TCH_SETUP_YMAX_TXT:
					CurrentInput=&FTxtInput5;
					break;

				case OBJ_HIC_ESC:
					input_end=TRUE;
					break;
			}
		}
		LibTxtInp(key_cd,&tsts,CurrentInput);
	}
	LibCloseWindow();
}

/* ------------------------------------------------------------------------ */
/* DrawAxis     		                                            */
/* ------------------------------------------------------------------------ */
void drawAxis(double xmin,double xmax,double ymin,double ymax){
	double	xt,yt;

	xt=(160-(xmax*(160/(xmax-xmin))));
	yt=(160+(ymin*(160/(ymax-ymin))));
	if(xmin<0 && xmax>0) LibGdsLine(xt,0,xt,160);
	if(ymin<0 && ymax>0) LibGdsLine(0,yt,160,yt);
}

/* ------------------------------------------------------------------------ */
/* PlotProc     		                                            */
/* ------------------------------------------------------------------------ */
void PlotProc(){
	double	xact,yact;
	int	x=0,y=0;
	double	xstep;
	int termnr;
	
	memset(term,0x00,sizeof(term));
	termnr=parse(setup.function);

	xstep=((setup.xmax-setup.xmin)/160);
	drawAxis(setup.xmin,setup.xmax,setup.ymin,setup.ymax);

	/* draw first point */
	xact=setup.xmin;
	yact=evaluate(xact,0);
	y=160-((yact-setup.ymin)*(160/(setup.ymax-setup.ymin)));
	LibDotOn(x,y);

	for(x=1;x<160;x++){
		/* calculate actual coordinates */
		xact+=xstep;
		yact=evaluate(xact,0);

		/* draw pixel */
		y=160-((yact-setup.ymin)*(160/(setup.ymax-setup.ymin)));
		LibDotOn(x,y);
	}
	LibPutDisp();
}
/* ------------------------------------------------------------------------ */
/* Main-Program		                                                    */
/* ------------------------------------------------------------------------ */
void pplot(){
	/* initial function setup */
	memset(setup.function,0x00,sizeof(setup.function));

	strcpy(setup.function,"x^2-2\0");
	setup.xmin=-5;
	setup.xmax=5;
	setup.ymin=-5;
	setup.ymax=5;
	
	mainprogram();
}

/*void initMainTchStack(){
	LibTchStackClr();			
	LibTchStackPush(NULL);		
	LibTchStackPush(TchHardIcon);				LibTchStackPush(TchActionKey);
	LibTchStackPush(TchMainScreenButtons);
	LibTchInit();	
}		*/	


void mainprogram(){
	TCHSTS	tsts;
	bool	loop=TRUE;
	int	scrolldir=0; /* 0=y-axis 1=xaxis */

	drawMainScreen();
	PlotProc();
	
	initMainTchStack();

	while(loop){
		LibTchWait(&tsts);		/* wait for action */
		switch(tsts.obj){

			case TCH_SETUP:				
				if(LibBlockIconClick(&TchSetupButton,&tsts,B_ICON_RIGHT)==TRUE){
					while(LibTchStackPop()!=NULL) LibTchInit();
					SetupProc();
					drawMainScreen();
					PlotProc();
					initMainTchStack();
				}
				break;

			case OBJ_HIC_MBAR:
				DoMenu();
				break;

			case OBJ_LPSW_PUSH:
				if(scrolldir) scrolldir=0;
				else scrolldir=1;
				break;

			case OBJ_LPSW_UP:
				if(scrolldir){
					double tmp;
					tmp=(setup.ymax-setup.ymin)/5;
					setup.ymin+=tmp;
					setup.ymax+=tmp;
				} else {
					double tmp;
					tmp=(setup.xmax-setup.xmin)/5;
					setup.xmin+=tmp;
					setup.xmax+=tmp;
				}
				drawMainScreen();
				PlotProc();
				initMainTchStack();
				break;

			case OBJ_LPSW_DOWN:
				if(scrolldir){
					double tmp;
					tmp=(setup.ymax-setup.ymin)/5;
					setup.ymin-=tmp;
					setup.ymax-=tmp;
				} else {
					double tmp;
					tmp=(setup.xmax-setup.xmin)/5;
					setup.xmin-=tmp;
					setup.xmax-=tmp;
				}
				drawMainScreen();
				PlotProc();
				initMainTchStack();
				break;

			default:
				break;
		}
	}
	LibTchRelease();
}
