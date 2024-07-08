/******************************************************************************
	[TITLE]
		HEADER FILE for LIBRARY

	[OUTLINE]
		DEFINE to make out a LIBRARY

	Copyright (C) 1999 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef L_DEF
#define L_DEF


/************************************************/
/*               DEFINE                         */
/************************************************/
/* "LIBINT" FUNCTION CALL */
#define	Func_LibTxtInit			0x0000
#define	Func_LibTxtInp			0x0001
#define	Func_LibTxtDsp			0x0002
#define	Func_LibTxtDspC			0x0003
#define	Func_LibTxtDspS			0x0004
#define	Func_LibScrollPrint		0x0005
#define	Func_LibScrollClick		0x0006
#define	Func_LibGetKeyM			0x0007
#define	Func_LibDispKey			0x0008
#define	Func_LibGetCale			0x0009
#define	Func_LibDateDisp		0x000A
#define	Func_LibIconPrint		0x000B
#define	Func_LibIconPrintR		0x000C
#define	Func_LibIconClickSub	0x000D
#define	Func_LibCldKeyInit		0x000E
#define	Func_LibSelWindow		0x000F
#define	Func_LibPullDown		0x0010
#define	Func_LibPullDownInit	0x0011
#define	Func_LibPullDownAtrSet	0x0012
#define	Func_LibSelWindowExt	0x0013
#define	Func_LibSelWindow2		0x0014
#define	Func_LibSelWinExt2A		0x0015
#define	Func_LibSelWinExt2B		0x0016
#define	Func_LibTxtKeyWordSet	0x0017
#define	Func_LibAlarm			0x0018
#define	Func_LibNextAlmSet		0x0019
#define	Func_LibTchHardIcon		0x001A
#define	Func_LibTchWait			0x001B
#define	Func_LibSelWinLckA		0x001C
#define	Func_LibSelWinLckB		0x001D
#define	Func_LibWinIcnMsg		0x001E
#define	Func_LibJumpDate		0x001F
#define	Func_LibEditPullDown	0x0020
#define	Func_LibSelectFont		0x0021

/* "FUNC" FUNTION CALL */
#define	Func_FuncDateTime		0x0000
#define	Func_FuncSound			0x0001
#define	Func_FuncFormat			0x0002
#define	Func_FuncLang			0x0003
#define	Func_FuncCapa			0x0004
#define	Func_FuncContrast		0x0005
#define	Func_LibInputTime		0x0006
#define	Func_LibClkDispLine		0x0007
#define	Func_LibClkDispCursor	0x0008
#define	Func_LibInputTimeBar	0x0009
#define	Func_LibConvRaw2Lib		0x000A
#define	Func_LibConvLib2Raw		0x000B
#define	Func_LibGetCursorPos	0x000C
#define	Func_LibCalWin			0x000D
#define	Func_CalEnzan			0x000E
#define	Func_CalTisu			0x000F
#define	Func_CalAc				0x0010
#define	Func_CalC				0x0011
#define	Func_CalRoot			0x0012
#define	Func_CalErrorC			0x0013
#define	Func_LibInputTerm		0x0014
#define	Func_LibChkSysAlarm		0x0015
#define	Func_LibCalKeyDsp		0x0016
#define	Func_LibCalKeyTchWait	0x0017
#define	Func_LibCalBuf2Dat		0x0018
#define	Func_LibCalDat2Buf		0x0019
#define	Func_LibMemManagement	0x001A

/* BIOS CALL */
#define	LCDBIO		0x10		/* LCD                 */
#define	LCDWIN		LCDBIO		/* WINDOW              */
#define	SRLBIO		0x14		/* SERIAL              */
#define	KEYBIO		0x16		/* KEY                 */
#define	MEMBIO		0x46		/* MEMORY              */
#define	CALBIO		0x4D		/* CALUCLATE           */
#define	BATBIO		0x4E		/* BATTERY             */
#define	PENBIO		0x4F		/* PEN                 */
#define	EMSBIO		0x67		/* EMS                 */
#define	BANKBIO		0xC8		/* BANK	               */
#define	EVTLIB		0x50		/* EVENT               */
#define	KEYLIB		0x1F		/* KEY                 */
#define	COMLIB		0x28		/* COM                 */
#define	ELPBIO		0x22		/* EL CONTROL          */
#define	WAITBIO		0x23		/* WAIT 	           */
#define	VERBIO		0x20		/* VERSION GET         */
#define	FILE_BIOS	0x2e		/* FLASH               */
#define	GDSBIO		0xCC		/* Graphic             */
#define IB_GDSBIO   GDSBIO  	/* Graphic             */
#define	FGDSGRA		0xCE		/* FIXED Graphic       */
#define CLKBIO		0xC9		/* CLOCK               */
#define	MODEBIO		0xCA		/* MODE                */
#define	COMMONBIO	0xCF		/* COMMON              */
#define	COMMONLBIO	COMMONBIO	/* COMMON              */
#define	DRAW_BIO	0xc7		/* Drawing             */
#define	KEYWDBIO	0xd7		/* KEYWORD REGISTRATION*/
#define	CURBIO		0x24		/* CURSOR CONTROL      */
#define	LIB_BIOS	0xD9		/* C LIBINT            */
#define	LIB_FUNC	0xDB		/* C FUNC              */
#define	SUBCBIO		0x2F		/* SubEntry CONTROL    */
#define	ADINEBIO	0xD8		/* Addin Syncronaize   */


/* LCD BIOS FUNCTION */
#define	IB_PSET			0x0C	/* Dot write                      */
#define	IB_LCDINIT		0x20	/* LCD initialize                 */
#define	IB_SETTRNSMOD	0x22	/* TRANSFER MODE set up           */
#define	IB_PGCLR		0x31	/* PAGE clear                     */
#define	IB_GSQRCLR		0x32	/* Square Area clear              */
#define	IB_DRWLINE		0x33	/* Draw LINE                      */
#define	IB_PUTPTN		0x36	/* Put SQUARE PATTERN             */
#define	IB_GETPTN		0x37	/* Get SQUARE PATTERN             */
#define	IB_PUTDISP		0x39	/* Transfer VRAM DATA to D/D      */
#define	IB_PUTVRAM		0x3B	/* Transfer VRAM DATA(Square area) to D/D */
#define	IB_ORNAPTN		0x4E	/* SQUARE AREA embellish          */
#define	IB_GSCROLLUP	0x4F	/* Scroll Up (Graphic unit)       */
#define	IB_GSCROLLDWN	0x50	/* Scroll Down (Graphic unit)     */

/* LCD BIOS Embellishment */
#define	IB_VPG0			0		/* normal            */
#define	IB_LSET			0		/* OR                */
#define	IB_LREPLACE		1		/* =                 */
#define	IB_LCOMPLEMENT	2		/* XOR               */
#define	IB_LAND			3		/* AND               */
#define	IB_LCLEAR		4		/* AND NOT           */
#define	IB_LMESH		5		/* MESH              */
#define	IB_LREVERSE		6		/* NOT =             */
#define	IB_LUNDLIN		7		/* UNDER LINE        */

/* WINDOW(LCD) BIOS Embellishment */
#define	IB_FRAME0		0		/* No Frame          */
#define	IB_FRAME1		1		/* Frame 1LINE       */
#define	IB_OPENWIN		0x60	/* Open              */
#define	IB_SAVEWIN		0x62	/* Open(save coordinates) */
#define	IB_CLOSEWIN		0x61	/* Close             */

/* LCD BIOS Transfer VRAM DATA to D/D */
#define	IB_TRNSON		0		/* INSTANTLY REFLECT    */
#define	IB_TRNSOFF		1		/* NOT REFLECT          */

/* GDS BIOS Display Type */
#define	IB_GDS_DDONLY	0x80	/* Write only DD        */
#define	IB_GDS_VRAMDD	0x40	/* Write VRAM and DD at the same time */

/* GDS BIOS Transfer Type */
#define IB_GDS_OVER		0x00	/* OVER                 */
#define IB_GDS_OR		0x01	/* OR                   */
#define IB_GDS_AND		0x02	/* AND                  */
#define IB_GDS_REV		0x03	/* REVERSE              */
#define IB_GDS_MESH		0x04	/* MESH                 */
#define IB_GDS_XOR		0x05	/* XOR                  */

/* GDS BIOS BOX */
#define IB_GBOXWR   	0x00    /* BOX Write            */
#define IB_GBCLR    	0x01    /* BOX Area Clear       */
#define IB_GBREV    	0x02    /* BOX Area Reverse     */
#define IB_GBMESH   	0x03    /* BOX Area Mesh        */

/* GDS BIOS BOX EXTENSION */
#define IB_GDSBOX		0x30	/* BOX Write            */
#define IB_GDSBOXSET	0x31	/* BOX Embellish        */
#define IB_GDS_BOXGET	0x10	/* Data get             */
#define IB_GDS_BOXSET	0x11	/* Data set             */

/* GDS BIOS DRAW */
#define IB_GPSETON  	0x04    /* Write POINT          */
#define IB_GPSETOFF 	0x05    /* Tuen out POINT       */
#define IB_GDRAWON  	0x06    /* Write LINE           */
#define IB_GDRAWOFF 	0x07    /* Turn out LINE        */

/* GDS BIOS DRAW EXTENSION */
#define IB_GDSPSET		0x32	/* DOT                  */
#define IB_GDS_OR		0x01	/*  OR Write            */
#define IB_GDS_AND		0x02	/*  AND Write           */
#define IB_GDS_XOR		0x05	/*  XOR Write           */
#define IB_GDSDRAW		0x33	/* LINE                 */
#define IB_GDS_OR		0x01	/*  OR Write            */
#define IB_GDS_AND		0x02	/*  AND Write           */
#define IB_GDS_MESH		0x04	/*  MESH Write          */
#define IB_GDS_XOR		0x05	/*  XOR Write           */

/* GDS BIOS Pattern */
#define IB_GPOVER   	0x08    /* OVER Write           */
#define IB_GPOR     	0x09    /* OR Write             */
#define IB_GPAND    	0x0A    /* AND Write            */
#define IB_GPREV    	0x0B    /* REVERSE Write        */
#define IB_GPMESH   	0x0C    /* MESH Write           */

/* GDS BIOS Proportional Font */
#define	IB_GPFOVER		0x0D	/* OVER Write           */
#define	IB_GPFOR		0x0E	/* OR Write             */
#define	IB_GPFAND		0x0F	/* AND Write            */
#define	IB_GPFREV		0x10	/* REVERSE Write        */
#define	IB_GPFMESH		0x11	/* MESH Write           */
#define	IB_GPFSIZE		0X12	/* FontSize             */

/* GDS BIOS Software Keybord Embellishment */
#define	IB_GDS_KEYREV	0x20	/* REVERSE              */
#define	IB_GDS_KEYREVR	0x21	/* REVERSE Return       */
#define	IB_GDS_KREVP1	0x00	/* PATTERN1 Normal      */
#define	IB_GDS_KREVP2	0x01	/* PATTERN2 Right shadow*/
#define	IB_GDS_KREVP3	0x02	/* PATTERN3 around right shadow */
#define	IB_GDS_KREVP4	0x03	/* PATTERN4 around      */

/* GDS BIOS Fixed Message Write */
#define IB_FMOVER		0x13	/* OVER Write           */
#define IB_FMOR			0x14	/* OR Write             */
#define IB_FMAND		0x15	/* AND Write            */
#define IB_FMREV		0x16	/* REVERSE Write        */
#define IB_FMMESH		0x17	/* MESH Write           */

/* GDS BIOS Fixed Message Read */
#define IB_FMREAD		0x18	/* MessageRead          */

/* GDS BIOS Fixed Message Centering & Right-leaning */
#define IB_FMCENTER		0x19	/* Centering            */
#define IB_FMRIGHT		0x1A	/* Right-leaning        */
#define IB_FMMIDLE		0x1B	/* Centering point Set  */

/* GDS BIOS Window Message */
#define IB_FMCOUNT		0x1C	/* Get Line count       */
#define IB_FMWINMES		0x1D	/* Display in Window    */
#define IB_FMWINCALC	0x1E	/* Get Window Size      */

/* GDS BIOS Window Message :Message Type */
#define	IB_MWIN_NO_ICON	0xFF	/* NO ICON              */

/* GDS BIOS Window Message :Button Type */
#define	IB_MWIN_NONE	0x00	/* NONE               */
#define	IB_MWIN_YES_NO	0x01	/* YES/NO  (2 Button) */
#define	IB_MWIN_SET_ESC	0x02	/* SET/ESC (2 Button) */
#define	IB_MWIN_OK		0x03	/* OK  (1 Button)     */
#define	IB_MWIN_SET		0x04	/* SET (1 Button)     */
#define	IB_MWIN_ESC		0x05	/* ESC (1 Button)     */
#define	IX_MWIN_CENTER	0x80	/* CENTER             */

/* GDS BIOS Fixed Graphic */
#define	IB_FGPOVER		0x00	/* OVER Write           */
#define	IB_FGPOR		0x01	/* OR Write             */
#define	IB_FGPAND		0x02	/* AND Write            */
#define	IB_FGPREV		0x03	/* REVERSE Write        */
#define	IB_FGPMESH		0x04	/* MESH Write           */

/* Touch */
#define	TCHSTACK_MAX	32
#define	TCH_WAIT		0x00
#define	TCH_INIT		0x01
#define ACT_COM_KEY		ACT_ICON

/* MODE BIOS FUNCTION NO */
#define	IB_MINFGET		0x00		/* MODE INFORMATION get               */
#define	IB_MINFSET		0x01		/* MODE INFORMATION set up(CAN'T USE) */
#define	IB_MDWINON		0x02		/* D-WINDOW Start                     */
#define	IB_MDWINOFF		0x03		/* D-WINDOW End                       */
#define	IB_MJUMP		0x04		/* Jump to ANOTHER MODE               */
#define	IB_MCALL		0x05		/* Call ANOTHER MODE                  */
#define	IB_MEINFGET		0x06		/* Get ANOTHER MODE INFORMATION       */
#define	IB_MSCRTJUMP	0x07		/* Jump to SECRET MIDDLE STATE        */
#define	IB_MEMSSET		0x08		/* EMS set up                         */
#define	IB_SCRTMODEJP	0x09		/* Jump from SECRET MIDDLE STATE to ANOTHER MODE */
#define	IB_OPENSYNCJP	0x0A		/* Jump to SYNCHRONIZE MODE(OPEN)     */
#define	IB_LASTINFGET	0x0b		/* LAST MODE INFORMATION get          */
#define	IB_MENUMJUMP	0x0c		/* Jump from MENU MODE                */

/* MODE BIOS Function No.*/
#define	IB_HEADINF_TOP	0x0D		/* HEADER INFORMATION TOP ADDRESS get		*/
#define	IB_DLALL_NUM	0x0E		/* DownLoad Count get						*/
#define	IB_UMODE_GET	0x0F		/* UserModeCode get							*/
#define	IB_PNAME_GET	0x10		/* ProgramName get							*/
#define	IB_PVER_GET		0x11		/* ProgramVersion get						*/
#define	IB_LVER_GET		0x12		/* LinkLibralyVersion get					*/
#define	IB_MICON_GET	0x13		/* IconMenu Graphics get 					*/
#define	IB_LICON_GET	0x14		/* ListMenu Graphics get					*/
#define	IB_PMODE_CHK	0x15		/* DownloadProgramModeCheck					*/


/* MAIN MODE CODE */
#define	IB_MSYS			0x00		/* SYSTEM           */
#define	IB_MTEL			0x01		/* TELEPHONE        */
#define	IB_MMEMO		0x02		/* MEMO             */
#define	IB_MSCHD		0x03		/* SCHEDULE         */
#define	IB_MCLOCK		0x04		/* CLOCK            */
#define	IB_MCAL			0x05		/* CALCULATOR       */
#define	IB_MESHEET		0x06		/* SPREADSHEET      */
#define	IB_MCOM			0x07		/* COMMUNICATION    */
#define	IB_MADDIN		0x08		/* ADDIN            */
#define	IB_MQFORM		0x09		/* QUICK FORM       */
#define	IB_MGAME		0x0A		/* GAME             */
#define	IB_MDIC			0x0B		/* DICTIONARY       */
#define	IB_MEXPEN		0x0C		/* EXPENSE          */
/*#define	IB_MTOOL		0x0D		/* TOOL             */
#define	IB_MCONV		0x0D		/* Conversion       */	/*1999.11.29*/
#define	IB_MMENUMD		0x0E		/* MENU             */
#define	IB_MMAIL		0x10		/* MAIL				*/	/*1999.11.29*/
#define	IB_MSECRET		0x80		/* <SECRET ON>      */

#define	IW_MSYS			0x0000		/* SYSTEM           */
#define	IW_MTEL			0x0100		/* TELEPHONE        */
#define	IW_MMEMO		0x0200		/* MEMO             */
#define	IW_MSCHD		0x0300		/* SCHEDULE         */
#define	IW_MCLOCK		0x0400		/* CLOCK            */
#define	IW_MCAL			0x0500		/* CALCULATOR       */
#define	IW_MESHEET		0x0600		/* SPREADSHEET      */
#define	IW_MCOM			0x0700		/* COMMUNICATION    */
#define	IW_MADDIN		0x0800		/* ADDIN            */
#define	IW_MQFORM		0x0900		/* QUICK FORM       */
#define	IW_MGAME		0x0A00		/* GAME             */
#define	IW_MDIC			0x0B00		/* DICTIONARY       */
#define	IW_MEXPEN		0x0C00		/* EXPENSE          */
/*#define	IW_MTOOL		0x0D00		/* TOOL             */
#define	IW_MCONV		0x0D00		/* Conversion        */	/*1999.11.29*/
#define	IW_MMENUMD		0x0E00		/* MENU             */
#define	IW_MMAIL		0x1000		/* MAIL				*/	/*1999.11.29*/
#define	IW_MSECRET		0x8000		/* <SECRET ON>      */

/* SUB MODE CODE */
/* SYSTEM */
#define IB_SMNORMAL		0x00		/* NORMAL			*/
#define IB_SMTEST		0x01		/* TEST PROGRAM		*/
#define IB_SMSERVICE	0x02		/* SERVICE TRANSFER	*/

#define IB_SMLIBINT		0x80		/* C LibINT			*/
#define IB_SMFUNCINT	0x81		/* C FuncINT		*/
#define IB_SMSYSDAT1	0x82		/* SystemData1		*/
#define IB_SMSYSDAT2	0x83		/* SystemData2		*/

#define IB_SMALL		0x00		/* COMMON			*/
/* TELEPHONE */
#define IB_SMTELPAR		0x01		/* PERSONAL         */
#define IB_SMTELBIS		0x02		/* BUSINESS         */
#define IB_SMTELSTR		0x03		/* STORE            */
#define IB_SMTELETC		0x04		/* ETC	            */
#define	IB_SMTELPHB		0x05		/* PhoneBook		*/	/*ZX488 2000.02.07*/
/* SCHEDULE */
#define IB_SMSCHD		0x01		/* SCHEDULE         */
#define IB_SMSCHDK		0x02		/* TERM             */
#define IB_SMTODO		0x03		/* TODO             */
#define IB_SMREMI		0x04		/* REMINDER         */
#define IB_SMCALE		0x05		/* CALENDER         */
#define IB_SMSCHD_DAY	0x06		/* DAY              */
#define IB_SMSCHD_WEEK	0x07		/* WEEK             */
#define IB_SMSCHD_MONTH	0x08		/* MONTH            */
#define IB_SMSCHD_YEAR	0x09		/* YEAR             */
#define IB_SMSCHD_HALF	0x0A		/* HALF YEAR        */
/* CLOCK */
#define IB_SMHOME		0x01		/* HOME             */
#define IB_SMWORLD		0x02		/* WORLD            */
#define IB_SMALARM		0x03		/* ALARM            */
/* COMMUNICATION */
#define IB_SMMAIL		0x01		/* MAIL             */
#define IB_SMFAX		0x02		/* FAX              */
#define IB_SMPCLINK		0x03		/* PCLINK           */
#define IB_SMCRAIDL		IB_SMPCLINK	/* CRAIDLE          */
#define IB_SMONLINE		0x05		/* ONLINE           */

#define IB_SMCLIP		0x02		/* CLIP(DualWindow) */

/*MailSubModeCode*/	/*ZX488*/
#define	IB_SMMAIL_NEW		0x01		/*Mail New*/
#define	IB_SMMAIL_SEND		0x02		/*Mail Send*/
#define	IB_SMMAIL_RECV		0x03		/*Mail Recieve*/
#define	IB_SMMAIL_SR		0x04		/*Mail Send&Recieve*/
#define	IB_SMMAIL_INBOX		0x05		/*Mail Inbox*/
#define	IB_SMMAIL_OUTBOX	0x06		/*Mail Outbox*/
#define	IB_SMMAIL_SMS		0x07		/*ShortMessageSystem*/	/*2000.02.07*/

/* ERR DEFINE */
#define	IB_MOK			0x00		/* D-Window         */
#define	IB_MERR			0xff		/* D-Window         */

/* MODE INFORMATION */
/* SYSTEM INFO */
#define	IB_MDWINDOW			0x80		/* DUAL WINDOW      */
#define	IW_MDWINDOW			0x8000
#define	IB_MDNEXTALM_SET	0x40		/* NextAlarmSet     */
#define	IW_MDNEXTALM_SET	0x4000
#define	IW_SECRET_MODE		0x0100		/* SECRET           */
#define	IB_SECRET_MODE		0x01
/* APPLICATION INFO */
#define	IB_MINDEXDSP		0x01		/* INDEX/LIST       */
#define	IW_MINDEXDSP		0x0001
#define	IB_MDATADSP			0x02		/* DATA DISPLAY     */
#define	IW_MDATADSP			0x0002
#define	IB_MDATANEW			0x04		/* DATA NEW         */
#define	IW_MDATANEW			0x0004
#define	IB_MDATAREPR		0x08		/* DATA EDIT        */
#define	IW_MDATAREPR		0x0008
#define	IB_MDATASET			0x10		/* DATA REGISTRATION */
#define	IW_MDATASET			0x0010
#define	IB_MDATADEL			0x20		/* DATA DELETE      */
#define	IW_MDATADEL			0x0020
#define	IB_MSCRT_MOVE		0x80		/* DATA MOVE        */
#define	IW_MSCRT_MOVE		0x0080
/* MODE POINTER */
#define	IW_MDATAFLSH		0x0FFFF		/* FLASH            */

/* CONTRAST SET UP */
#define	IB_CONTINF		0x01	/* CONTRAST INFORMATION     */
#define	IB_CONT_INIT	0x00	/* CONTRAST INITIAL VALUE   */
#define	IB_CONT_UP		0x01	/* CONTRAST UP              */
#define	IB_CONT_DOWN	0x02	/* CONTRAST DOWN            */
#define	IB_CONTRAST_OK	0x00	/* OK                       */
#define	IB_CONTRAST_LMIT 0x01	/* CONTRAST LIMIT           */

/* CAPACITY & MEMORY */
#define	IB_GET_CAPA		0x09		/* VACANT BLOCK get          */
#define	IB_GET_FLASH	0x0a		/* SYSTEM FLASH CAPACITY get */
#define	TOTAL_BLOCK1M	0x29F0L		/* ALL BLOCK COUNT(1MBit)    */
#define	TOTAL_BLOCK2M	0x5b80L		/* ALL BLOCK COUNT(2MBit)    */
#define	FLASH2M			0x225b		/* 2M FLASH USE              */
#define	IB_REMAKE		0x30		/* MEMORY MANAGEMENT         */

/* EL */
#define	IB_ELP_OFF		0x00		/* EL PANEL OFF              */
#define	IB_ELP_ON		0x01		/* EL PANEL ON(15 SECOND)    */
#define	IB_ELP_SON		0x02		/* EL PANEL ON(CONTINUATION) */
#define	IB_ELP_STS		0x03		/* EL PANEL STATE get        */
 /* EL PANEL STATE */
#define	IB_ELPSTS_OFF 	0x00		/* EL OFF                    */
#define	IB_ELPSTS_ON  	0x01		/* EL ON(15 SEECOND)         */
#define	IB_ELPSTS_SON	0x02		/* EL ON(CONTINUATION)       */

/* ROM ID			ZX481/482/488 */
#define	IB_SYSCONFIG	0x0D		/* SYSTEM CONFIGURATION      */
#define	IB_MODEL_CHK	0x01        /* MODEL check               */

/* CLOCK BIOS */
#define IB_GETDATE		0x00		/* DATE get               */
#define IB_SETDATE		0x01		/* DATE set up            */
#define IB_CHGTNDAY		0x02		/* Change to TOTAL NUMBER */
#define IB_GETTIME		0x03		/* TIME get               */
#define IB_SETTIME		0x04		/* TIME set up            */
#define IB_SETALRM		0x05		/* ALARM set up           */
#define IB_INITALRM		0x06		/* ALARM Clear            */
#define IB_GETALRMINF	0x07		/* ALARM STATE get        */
#define IB_GETALRMFLG	0x08		/* ALARM FLAG get         */
#define IB_CHGDTOTN		0x09		/* DATE TOTAL NUMBER     */
#define IB_CHGDTOW		0x0A		/* DATE WEEK             */

/* SERIAL FUNCTION NO */
#define	IB_PORTOPEN			0x00	/* PORT open			*/
#define	IB_PORTCLOSE		0x01	/* PORT close			*/
#define	IB_DTESTAT			0x02	/* DTE STATE get		*/
#define	IB_232CSTAT			0x03	/* RS232C STATE get		*/
#define	IB_SENDBYTE			0x04	/* 1BYTE send			*/
#define	IB_RECVBYTE			0x05	/* 1BYTE receive		*/
#define	IB_RATESET			0x07	/* Bouad rate set up				*/
#define	IB_232COPE			0x08	/* RS232C STATE operation		*/
#define	IB_BUFFCLR			0x09	/* RECEIVE BUFFER clear			*/
#define	IB_PREREAD			0x0A	/* RECEIVE BUFFER preread		*/
#define	IB_SENDBREAK		0x0B	/* BREAK SIGN send				*/
#define	IB_FORCEDCLOSE		0x0D	/* PORT forced close			*/
#define	IB_OPENSTAT			0x0E	/* PORT STATE get				*/
#define	IB_TXBUFFCLR		0x0F	/* TRANSMIT BUFFER clear		*/
#define	IB_GETRBUFCHARS		0x10	/* CHARACTER in RECEIVE BUFFER get	*/
#define	IB_GETXBUFCHARS		0x11	/* SPACE in TRANSMIT BUFFER get		*/
#define	IB_SENDBLOCK		0x12	/* BLOCK DATA transmit			*/
#define	IB_RECVBLOCK		0x13	/* BLOCK DATA receive			*/
#define	IB_GETINTPROC		0x14	/* INTRUDE ADDRESS get			*/
#define	IB_PAUSERESUME		0x15	/* TRANSMIT pause reopen		*/


/* PASSWORD */
#define	IB_PASSWDINF 	0x02	/* PASSWORD INFORMATION	*/
#define	IB_PASS_CLR		0x00	/* PASSWORD clear	    */
#define	IB_PASS_SET		0x01	/* PASSWORD set up		*/
#define	IB_PASS_GET		0x02	/* PASSWORD get			*/
#define	IB_PASS_CHK		0x03	/* PASSWORD check		*/
#define	IB_PASS_OK		0x00	/* PASSWORD OK			*/
#define	IB_PASS_NG		0x01	/* PASSWORD NG			*/

/* CURSOR CONTROL */
#define	IB_CURSOR		0x00	/* SET / GET    	*/
#define	IB_BLINK		0x01	/* TURN ON AND OFF	*/
#define	IB_BLINKON		0x02	/* TURN ON			*/
#define	IB_BLINKOFF		0x03	/* TUEN OFF			*/

/* INPUT OF IB_CURSOR */
#define	IB_CURSOROFF	0x00	/* NOT BLINK		  */
#define	IB_CURSORON		0x01	/* BLINK			  */
#define	IB_CURSORON2	0x03	/* BLINK (BLANK TYPE) */
#define	IB_CURSORGET	0x02	/* STATE GET          */

/* TOUCH PANEL ALIGNMENT */
#define	IB_CALIBRATION	0x05	/* CALIBRATION		            */
#define	IB_EXISTESC		0x00	/* INPUT :"ESC" ICON VALID		*/
#define	IB_NOESC		0x01	/* INPUT :"ESC" ICON INVALIDATE */
#define	IB_NOERR_END	0x00	/* OUTPUT:NO ERROR END          */
#define	IB_ESC_END		0x01	/* OUTPUT:ESC END OR OFF KEY	*/
#define	IB_ALM_END		0x02	/* OUTPUT:ALARM END             */
#define	IB_PON_END		0x03	/* OUTPUT:POWER ON              */

/* VERSION GET */
#define	IB_ROMVERSION	0x00	/* ROM VERSION GET */

/* ALARM */
#define	IB_SOUNDINF		0		/* BUZZER                        */
#define	IB_SNDINF_SET	0		/* BUZZER set up                 */
#define	IB_SNDINF_GET	1		/* BUZZER STATE get              */
#define	IB_ALMSTATE		9		/* ALARM STATE (KEY WAIT)        */

#define	IB_ALMOFF		0x00	/* ALARM OFF set          		*/
#define	IB_ALMON_DATA	0x01	/* ALARM ON  set(SCHEDULE)		*/
#define	IB_ALMON_DILY	0x02	/* ALARM ON  set(DAY) 			*/
#define	IB_ALMBZZ_NOW	0x03	/* ALARM STATE get              */
#define	IB_ALMPUSHWORK	0x04	/* Saves event management work    */
#define	IB_ALMPOPWORK 	0x05	/* Restores event management work */
#define	IB_ALMPUSHOBJ	0x06	
/* Gets the touch table information that has been touched during alarm matches*/
#define	IB_BUZZERSET	6		/* BUZZER ON-OFF                       */
#define	IB_BUZZER_ON	0		/* BUZZER ON                           */
#define	IB_BEEP0_SET	0		/*  1 SECOND                           */
#define	IB_BEEP1_SET	1		/*  1 TIME / SECOND                    */
#define	IB_BEEP2_SET	2		/*  2 TIME / SECOND                    */
#define	IB_BEEP3_SET	3		/*  3 TIME / SECOND                    */
#define	IB_BUZZER_OFF	1		/* BUZZER OFF                          */

#define	IX_DAYLY_ALM	0x01	/* DAY ALARM                          */
#define	IX_DATA_ALM		0x02	/* SCHEDULE                           */
#define	IX_KEY_SOUND	0x80	/* KEY TONE                           */

#define	BUZZ_ON_DAILY	2		/* DAY ALARM ON                       */
#define	BUZZ_ON_SCH		3		/* SCHEDULE ALARM ON                  */
#define	BUZZ_OFF		0		/* DAY ALARM OFF                      */

#define IB_ALMFCHK		0x0E	/* ALARM FLAG check                   */
#define IB_ALMFCLR		0x0F	/* ALARM FLAG clear                   */
#define IB_SETDAYLY		0x0C	/* DAY ALARM TIME set                 */
#define IB_GETDAYLY		0x0D	/* DAY ALRAM TIME get                 */

#define IB_CLK_INPERR	0x11	/* INPUT VALUE beyonds the limits     */
#define IB_CLK_ASERR	0x14	/* ALARM dose not set                 */
#define IB_CLK_ANSERR	0x15	/* That ALARM TIME is already used    */
#define IB_CLK_ALMON	0x01	/* ON                                 */
#define IB_CLK_ALMOFF	0x00	/* OFF                                */

#define IB_DAYLY_ALM	0x00	/* DAY ALARM                          */

/* BREAK KEY */
#define	IB_BRSAMP		0x06	/* FLASH BREAK KEY         */
/* FUNCTION SET */
#define IX_BRSAMP_CK	0x80	/* bit7	FACTOR CHECK       */
#define IX_BRSAMP_INIT	0x40	/* bit6	INITIAL            */
/* FACTOR */
#define IX_BLD1MSG		0x08	/* bit3	BLD MESSAGE LEVEL  */
#define IX_CRADLE		0x04	/* bit2	CRADLE KEY         */
#define IX_ESCBRK		0x01	/* bit0	ESC TOUCH          */
/* OUTPUT CODE */
#define IW_NOEVENT		0x5000	/* NOT BREAK               */
#define IW_SYS_CRDLKEY	0x5003	/* CRADLE KEY              */
#define IW_SYS_BLD1		0x5001	/* BLD MESSAGE LEBEL       */
#define IW_SYS_ESCTCH	0x5009	/* "ESC" TOUCH             */
/* DEVELOPMENT STATE */
#define IB_NULL			0x00	/* NULL                    */
#define IB_BR1SEC     	0x01	/* 1 SECOND PROGRESS       */
/* REPEAT OFF */
#define IB_REPOFF		0x0a	/* REPEAT OFF              */

/* LANGUAGE SET UP */
#define	IB_LANGINF		0x05
#define	IB_LANG_SET		0x00
#define	IB_LANG_GET		0x01
#define	IB_FUNCKEYB_SET	0x02
#define	IB_FUNCKEYB_GET	0x03
#define	IB_LANGINF_GET	0x04
#define	IB_DEUTSCH		0x00
#define	IB_ENGLISH		0x01
#define	IB_ESPANOL		0x02
#define	IB_FRANCAIS		0x03
#define	IB_ITALIANO		0x04
#define	IB_LANG_ENGLISH	0x01

/* KEYBOARD TYPE */
#define	IB_QWERTY		0x00	/* QWERTY */
#define	IB_AZERTY		0x01	/* AZERTY */
#define	IB_QWERTZ		0x02	/* QWERTZ */

/* BLD */
#define	IB_BLD_CHECK	0x07	/* BLD CHECK         */
#define	IB_BLD1_CHECK	0x00	/* MESSAGE BLD CHECK */
#define	IB_BLD_OK		0x00	/* BATTERY OK        */
#define	IB_BLD1_NG		0x01	/* BATTERY NG        */
#define	IB_BLD2_CHECK	0x01	/* OFFBLD CHECK      */

/* APO */
#define IB_APOTIMESET 	0x0b
#define IB_APOTMSET 	0x01
#define IB_APOTMGET 	0x02

/* Drawing BIOS */
#define DRAW_INIT			0x00	/* INITIALIZE          */
#define DRAW_SETPATTERN		0x01	/* PEN PATTERN SET     */
#define DRAW_SETCLIPAREA	0x02	/* CLIP AREA SET       */
#define DRAW_SETPOINT		0x03	/* POINT SIZE SET      */
#define DRAW_LINE			0x04	/* LINE                */
#define DRAW_BOX			0x05	/* BOX                 */
#define DRAW_CIRCLE			0x06	/* CIRCLE              */
#define FILLAREA			0x07	/* PAINT               */
#define TRANSFER_DD			0x08	/* Transfer VRAM to DD */
#define TRANSALL			0x09	/* Transfer ALL AREA   */
#define PUTIMAGE			0x0A	/* IMAGE DRAW          */
#define GETIMAGE			0x0B	/* IMAGE GET           */
#define REDUCTIMAGE			0x0C	/* IMAGE COMPRESSION   */

/* KEYWORD REGISTRATION */
#define IB_KEYWD_INIT	0x00	/* KEYWORD INITIALIZE      */
#define IB_KEYWD_SET	0x01	/* KEYWORD REGISTRATION    */
#define IB_KEYWD_SRCH	0x02	/* KEYWORD SEARCH          */

/* KEYWORD REGISTRATION OPTION */
#define IB_KEYWD_FSRCH	0x00	/* FIRST SEARCH    */
#define IB_KEYWD_NSRCH	0x01	/* NEXT SEARCH     */

/* CONTACTS CATEGORY */
#define IB_CATEGORY		0x0c
#define IB_CATECLR		0x00	/* CLEAR        */
#define IB_CATEWR		0x01	/* WRITE        */
#define IB_CATERD		0x02	/* READ         */

/* ALL TOUCH AREA OBJECT CODE */
#define OBJ_ALLTCH		0x9fff

/* DEBUG */
#define WIN_SIZEX		160		/* X SIZE               */
#define WIN_SIZEY		160		/* Y SIZE               */
#define XFS				(5+1)	/* FONT X SIZE          */
#define YFS				7		/* FONT Y SIZE          */
#define BUFMAX			500		/* OUTPUT BUFFER LENGTH */
#define STRS			24		/* DATA MAX LENGTH      */
#define GXSIZE			128		/* X SIZE(OLD) */
#define GYSIZE			128		/* Y SIZE(OLD) */

/* SUB ENTRY Function No.*/
#define	IB_SUBCSAVE		0x00	/* FILE NAME(SUB ENTRY) REGIST	*/
#define	IB_SUBCDEL		0x01	/* FILE NAME(SUB ENTRY) DELETE	*/
#define	IB_SUBCRENAME	0x02	/* FILE NAME EDIT               */
#define	IB_FILESRC		0x03	/* FILE NAME SEARCH				*/
#define	IB_GETSUBCCOND	0x04	/* SUB ENTRY STATE get			*/
#define	IB_GETMODEENTY	0x05	/* MODE ENTRY get				*/

#define	IB_SUBENT_MAX	16		/* SUB ENTRY MAX REGIST COUNT	*/


/************************************************/
/*               TYPE DEFINE                    */
/************************************************/
/* LCD BIOS PARAMETER */
typedef	struct LCDPRM {
	byte	colr;
	byte	vatt;
	byte	wmod;
	byte	opt1;
	word	xpos;
	word	ypos;
	word	xsiz;
	word	ysiz;
	word	boft;
	word	bseg;
}	LCDPRM;

/* LIBINT OR FUNC READ PARAMETER */
typedef struct{
	byte		p_byte1;
	byte		p_byte2;
	int			p_int1;
	int			p_int2;
	int			p_int3;
	int			p_int4;
	void		*vn1;
	void		*vn2;
	void		*vn3;
	void far	*vf;
} PRM_LIST;

/* ALARM (BIOS) */
typedef struct ALARM_BIOS {
		int		ptr;			/* POINTER                      */
		byte	year_high;		/* YEAR(High):BCD               */
		byte	year_low;		/* YEAR(Low):BCD                */
		byte	month;			/* MONTH:BCD                    */
		byte	day;			/* DAY:BCD                      */
		byte	hour;			/* HOUR:BCD                     */
		byte	min;			/* MINUTE:BCD                   */
		byte	mode;			/* MODE INFORMATION             */
		byte	submode;		/* SUB MODE INFORMATION         */
		byte	regisdata;		/* REGIST DATA FORM             */
		byte	reserve;		/* RESERVE                      */
} ALMBIOS;

/* ALARM (APPLICATION) */
typedef struct ALARM_APL {
		int		ptr;			/* POINTER                 */
		byte	date[9];		/* YEAR MONTH DAY:ASCII    */
		byte	time[4];		/* HOUR MINUTE:ASCII       */
		byte	mode;			/* MODE                    */
		byte	submode;		/* SUB MODE                */
		byte	regisdata;		/* REGIST DATA FORM        */
		byte	reserve;		/* RESERVE                 */
} ALMAPL;

#endif
