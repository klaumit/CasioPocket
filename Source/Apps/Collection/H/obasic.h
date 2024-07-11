#ifndef _OBASIC_H
#define _OBASIC_H

#include <stdrom.h>
#include "define.h"
#include "libc.h"
#include "l_define.h"
#include "l_libc.h"
#include "trm.h"

#define filename "OWBSettings"
/* datatype */
/* Attention: DUNKNOWN must be set to 0 */
#define DUNKNOWN         0
#define DINTEGER         1
#define DFLOAT           2
#define DSTRING          3
#define DLABEL           4
#define DREFERENCE       5
#define TYPEMASK         15
/* special kind of data (bit mask) */
#define KVAR             16
#define KLAST            32
#define KARRAY           64
/* token_type */
#define TCHARACTER       1
#define TKEY      	 2
#define TSTRING		 3
#define TEDELIM          4
#define TOPERATOR        5
#define TNUMBER          6
#define TIDENT           7
#define TDELIM           8
#define TLABEL           9
/* Werte für tok ( lex() ) 
   for single characters their ASCII value is used
   */
#define LESS            256
#define GREATER         258
#define NOT_LESS        259
#define NOT_EQUAL       260
#define NOT_GREATER     261
#define DELIMITER	262
#define NUMBER		263
#define IDENT           264
#define LABEL           265
#define STRING		266
#define INT             267
#define FLOAT           268
/* -------------------------- */
#define PRINT           301
#define INPUT           302
#define IF              303
#define THEN            304
#define ELSE	        314
#define ENDIF           322
#define FOR             305
#define NEXT            306
#define STEP            340
#define TO              307
#define GOTO            308
#define GOSUB           311
#define RETURN          312
#define END             313
#define REM	        318
#define DIM             325
#define DEFAULT         326
#define WAIT            328
#define UNKNOWN         350
#define INCLUDE         337
#define APPEND          338
#define VERSION         341
#define PROC            342
#define ENDP            343
#define MULDIV          344
#define AND             345
#define OR              346
#define VAR             347
#define FILEBUFFER      348

/* constant for ReturnCode of compile_command */
#define RC_CONTINUE 1
#define RC_INCLUDE 2
#define RC_APPEND 3
#define RC_EXIT 4

/* constants for interpretercode */
#include "ifunc.h"

/* #define DEBUGCOMP */
/* #define DEBUGLEX  */

#define NUM_LAB         40
#define LAB_LEN         10
#define FOR_NEST        25

#define NUM_PROC        40

#ifndef DEBUGCOMP
#define PROG_SIZE       0x1000
#else
#define PROG_SIZE       0x800
#endif
#define NAMELEN         10

extern char intchar;
extern char floatchar;

#ifndef DEBUGCOMP
#define ADDCODE(x) { addcode(x); } 
#define ADDVAL(x) { addcode(x); }
#else
#define ADDCODE(x) { addcode(x); OutputTrm(#x " "); }
#define ADDVAL(x) { addcode(x); OutputITrm(x); }
#endif

void addcode(int code);

/*-------------- stacks ---------*/
#define I_STACK_SIZE    20

extern int i_stack[I_STACK_SIZE];
extern int i_stack_ptr;
#define TOS i_stack[i_stack_ptr]
#define ST1 i_stack[i_stack_ptr-1]
#define ST2 i_stack[i_stack_ptr-2]
#define ST3 i_stack[i_stack_ptr-3]
#define ST4 i_stack[i_stack_ptr-4]
int ipop(void);
void ipush(int i);
void ipopn(int n);

#define FLOATT float

#define F_STACK_SIZE    20

extern FLOATT f_stack[F_STACK_SIZE];
extern int f_stack_ptr;
#define FTOS f_stack[f_stack_ptr]
FLOATT fpop(void);
void fpush(FLOATT f);

extern int fortos;      

#define G_STACK_SIZE    20

extern int g_stack[G_STACK_SIZE];
extern int g_stack_ptr;

int gpop(void);
void gpush(int i);

/* -------------------------------------- */
extern FILE_BUF fb; /* filebuffer for all fileoperations */
extern word fbfp;   /* corresponding filepointer */
extern byte fbmin;   /* corresponding minor mode */
extern char fname[10];

extern char *prog;	/* current pos. to be analyzed */
extern char *prog_start; /* the source */
extern unsigned int bin[]; /* binary code */
extern unsigned int bin_start;

#define DATASIZE 0x4000
extern int far xdata[DATASIZE];
extern int source_line;

#define MAXSTRLEN 300
#define S_STACK_SIZE 5
extern char hstring[S_STACK_SIZE][MAXSTRLEN+1];
#define STOS hstring[s_stack_ptr]
#define SST1 hstring[s_stack_ptr-1]
extern int s_stack_ptr;
void spop(int vadr);
void spop1(void);
void spop2(void);
void spush(int vadr);
void spush1(void);

extern int far sin_tab[];

typedef struct keyword {
  char command[20];
  int tok;
} Keyword;

int look_up(char *s,Keyword far *list);

#define MAXPARAM 10

typedef struct keywordpr {
  char command[20];
  char paratype[MAXPARAM];
  int bincode[MAXPARAM];
} KeywordPr;

typedef struct keywordfn {
  char command[20];
  int rtype;
  char paratype[MAXPARAM];
  int bincode;
} KeywordFn;

int strcmp_nf(char *sn,char far *sf);

extern Keyword far keytable[];
extern Keyword far commandtable[];
extern KeywordPr far proctable[];
extern KeywordFn far functable[];

struct proc {
  char name[LAB_LEN];
  char para[MAXPARAM];
  int addr;
};

extern struct proc proc_table[NUM_PROC];
extern int last_proc;

extern char default_datatype;

extern char token[];	/* Holds string representation of token */
extern char token_type;
extern char datatype;
extern int tok;
extern char *token_start;
extern char *quote;

extern char dtyp;

extern unsigned int pc;

extern int cproc;

void NewInp(byte *c,int maxchr);

#define exit(i) if (i!=0) LibPutMsgDlg("Error:%d",i); else LibPutMsgDlg("Finished"); LibJumpMenu();

/* Touch variables */
extern int tchx, tchy, tchobj; /* addresses of touch variables */
extern int errormessage;
extern int error;

bool load_memo(int sub,int nr);
bool load_memo_named(int sub,char *sig,char *name);
bool load_memo_fp(int sub,word fp);
bool save_memo(int sub,int nr);

bool load_file(int mode,int sub,int kind,int nr);
bool load_file_fp(int mode,int sub,int kind,word fp);
bool save_file(int mode,int sub,int kind,int nr);

int execute(void);

void loadsettings();
void savesettings();
void editsettings();
#define SETTINGSVER 0x0112

/* lexical analysis */
void putback(void);
int lex(void);
void expect(int k);

/* compiler (parser) functions */
void compile_proc(void);
void compile_endp(void);
void compile_userprocedure(void);
int compile_command(void);
void compile_include(void);
void compile_assignment(void);
void compile_print(void);
void compile_next(void);
void compile_input(void);
void compile_for(void);
void compile_gotoxy(void);
void compile_goto(void);
void compile_if(void);
void compile_else(void);
void compile_endif(void);
void compile_gosub(void);
void compile_return(void);
void compile_exit(void);
void compile_dim(void);
void compile_system(void);
void compile_wait(void);
int comp_var(char *dtyp);
int comp_array(char *dtyp);

int comp_svar(char *dtyp);

int store_label(void);

void serror(int error);
void rterror(char *func,int enr);

int comp_exp(char wtype);

int newentry(int dtype,char *n);
int findentry(char *dtypep,char *n);
int expandentry(int addr,int el);

void listreset(void);
void listhide(int pos);
int findvar(int dtype);
int find_label(char *s);
void check_labels(void);

int new_proc(char *s,int addr);
int search_proc(char *s);

int find_eol(void);

extern int listnext;
extern int datanext; /* first free "address" after variables */

char DT(char dtyp);

/* organization of programms */

#define PROGRAMSIG "! "
#define INCLUDESIG "!*"
#define APPENDSIG "!+"

extern int progsub;
extern int includesub;
extern int statistics;

bool load_program(void);

/* interpreter functions */
void x_pixel(void);
void x_input(void);
void x_input_f(void);
void x_print_quote(void);
void x_gotoxy(void);
void x_sin(void);
void x_cos(void);
void x_random(void);
void x_time(void);
FLOATT sinf(FLOATT);
FLOATT cosf(FLOATT);

/* string */
extern int firststring;
void resetstrings();
void printstrings();
int insertstring(int ref,char *st);
void deletestring(int addr);
void garbagecollection(void);
void getvar(int vadr);
void putvar(int vadr);
#endif
