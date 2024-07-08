/******************************************************************************
	Copyright (C) 1999 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef DEFINEDEF
#define DEFINEDEF


/************************************************/
/*               tyepdef & enum                 */
/************************************************/
/* Variable */
typedef	unsigned char	byte;
typedef	unsigned int	word;
typedef	unsigned long	dword;
typedef	unsigned char	bool;

/* Judgment */
enum bool {FALSE=0, TRUE=1, HALF=2};

/* SuspectedKeyCode for TimeKeyBoard(It allocates for 0x80 since then.)*/
enum {CLKKEY_OTHER=0,
	  CLKKEY_CLKBOARD=0x80, CLKKEY_AM,CLKKEY_PM,CLKKEY_TIMEBAR,
	  CLKKEY_NUMBER, CLKKEY_DEL,CLKKEY_NEXT,CLKKEY_LEFT,CLKKEY_RIGHT,
	  CLKKEY_TIMEKEYB,CLKKEY_DRAGBAR,CLKKEY_TOUCHBAR,CLKKEY_LSHIFT,
	  CLKKEY_RSHIFT,CLKKEY_CSRMOVE,
	  CLKKEY_ILLEGAL };


/************************************************/
/*                 Literal                      */
/************************************************/
/* Judgment */
#define ON      1
#define OFF     0

/* Proportional font */
#define IB_PFONT1		0x00	/*	Proportional NORMAL		*/
#define IB_PFONT2		0x01	/*	Proportional BOLD		*/
#define IB_PFONT3		0x03	/*	Proportional for TITLE	*/
#define IB_CG57FONT		0x02	/*	Fixed Pitch 5*7			*/

/* Touch action code */
#define	ACT_MAKE		0x00000001L
#define	ACT_MOVE		0x00000002L
#define	ACT_MOVE_OUT	0x00000004L
#define	ACT_MOVE_IN		0x00000008L
#define	ACT_DOWN		0x00000010L
#define	ACT_DOWN_IN		0x00000020L
#define	ACT_BREAK		0x00000040L
#define	ACT_BREAK_IN	0x00000080L
#define	ACT_REPEAT		0x00000100L
#define	ACT_NONE		0x00000000L
#define ACT_500MSEC		0x00000200L
#define ACT_ALM			0x00000400L
#define	ACT_ICON		(ACT_MAKE | ACT_MOVE_IN |ACT_MOVE_OUT | ACT_BREAK_IN)
#define ACT_SCR_BAR		(ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_DOWN_IN | ACT_BREAK_IN)
#define	ACT_TXTAREA		(ACT_MAKE|ACT_MOVE_OUT|ACT_BREAK|ACT_DOWN_IN|ACT_DOWN)

/* Touch reserve object code(for hardicon) 2000/01/12 Hirakata */
#define	OBJ_HIC_POS1	0x0001	/* Menu       */
#define	OBJ_HIC_POS2	0x0004  /* Schedule   */
#define	OBJ_HIC_POS3	0x0005  /* Contacts   */
#define	OBJ_HIC_POS4	0x0006  /* Memo       */
#define	OBJ_HIC_POS5	0x0009  /* Quick Memo */
#define	OBJ_HIC_OFF 	0x0002  /* Off        */
#define	OBJ_HIC_EL  	0x0003  /* EL         */
#define	OBJ_HIC_MBAR	0x0007  /* MenuBar    */
#define	OBJ_HIC_ESC 	0x0008  /* ESC        */

/* Object code form OFF status 2000/01/12 Hirakata */
#define	OBJ_HIC_POS1_PON	0x0021		/* Menu       */
#define	OBJ_HIC_POS2_PON	0x0024  	/* Schedule   */
#define	OBJ_HIC_POS3_PON	0x0025  	/* Contacts   */
#define	OBJ_HIC_POS4_PON	0x0026  	/* Memo       */
#define	OBJ_HIC_POS5_PON	0x0029  	/* Quick Memo */
#define	OBJ_HIC_MBAR_PON	0x0027  	/* MenuBar    */
#define	OBJ_SYS_CRDLKEY_PON	0x0030		/* Cradle     */
#define	OBJ_LPSW_PON		0x0015		/* LeverPush  */

/* Touch reserve object code (for mode icon) */
#define	OBJ_MD_ICON		0x8900

/* Touch reserve object code (lever push switch) */
#define	OBJ_LPSW_UP			0x0011	/* Up	*/
#define	OBJ_LPSW_PUSH		0x0012	/* PUSH	*/
#define	OBJ_LPSW_DOWN		0x0013	/* Down	*/

/* Touch reserve obejct code (SYSTEM) */
#define	OBJ_END				0x0000	/* END                      */
#define	OBJ_NONE			0xFFFF	/* None                     */
#define OBJ_500MSEC			0x001F	/* Over500msec              */
#define	OBJ_NOEVENT			0x5000	/* No event(at SCAN mode)   */
#define	OBJ_SYS_BLD1		0x5001	/* BLD1(MessageLevelLowBattery) */
#define	OBJ_SYS_BLD2		0x5002	/* BLD2(OFFLevelLowBattery)     */
#define	OBJ_SYS_CRDLKEY		0x5003	/* Push CradleKey           */
#define	OBJ_SYS_BATSW		0x5004	/* BatterySwitch OPEN       */
#define	OBJ_SYS_RESETKEY	0x5005	/* PushSoftResetKey         */
#define	OBJ_SYS_CARDSW		0x5006	/* CardLockSwitch OPEN      */
#define	OBJ_SYS_ALARM		0x5007	/* ClockAlarm               */
#define	OBJ_SYS_APO			0x5008	/* APO(AutoPowerOFF)        */
#define	OBJ_SYS_ESCTCH		0x5009	/* ESC with break key sample */

/* Touch extension code */
#define EXT_CODE_DEL    	0x0022
#define EXT_CODE_ESC    	0x0026
#define EXT_CODE_SET    	0x0028
#define EXT_CODE_500MS  	0x0001

/* FlashDataBuffer */
#define	ITEM_TOP		0xFE
#define	TEXT_END		0x00

/* KeyBoardRequest */
#define SKEY_COM1_S		0x0000		/* NormalKeyBoard(SmallCharacterBorad)      */
#define SKEY_COM2_S		0x0001		/* NormalKeyBoard(SmallCharacterBoard with SHIFT)*/
#define SKEY_COM1_L		0x0002		/* NormalKeyBoard(LargeCharacterBoard)        */
#define SKEY_COM2_L		0x0003		/* NormalKeyBoard(LargeCharacterBoard with SHIFT)  */
#define SKEY_COM3_L		0x0012		/* NormalKeyBoard(LargeCharacterBoard at ItemTop)  */
#define SKEY_CODE		0x0004		/* NormalKeyBoard(AccentSymbolBoard)        */
#define SKEY_SMBL		0x0005		/* NormalKeyBoard(SymbolBoard)              */

/* Keycode */
#define KEY_RTN			0xf0		/* Return     */
#define KEY_DEL			0xf1		/* Del        */
#define KEY_SHIFT		0xf2		/* Å™         */
#define KEY_CAPS		0xf3		/* Caps       */
#define KEY_SPC			0xf4		/* Space      */
#define KEY_SMBL		0xf5		/* Symbol     */
#define KEY_CODE		0xf6		/* Accent     */
#define KEY_NEXT		0xf7		/* Next       */
#define KEY_BACK		0xf8		/* Back       */
#define KEY_ALLCODE		0xf9		/* All Code   */
#define KEY_CHANGE		0xfa		/* SynbolChange */
#define KEY_NOP			0xfe		/* CalenderNOP */
#define KEY_NONE		0xff		/* OtheTouchArea */

/* ScrollBarSizeInformation */
#define SCR_XSIZE		9			/* X dot size       */
#define	ARW_SIZE		5			/* Arrow size       */
#define	ARW_AREA		7			/* Arrow area size  */

/* ScrollBarArrowHatchingInformation */
#define	SCR_NO_MASK		0x00		/* Arrow No hatching */
#define	SCR_UP_MASK		0x01		/* UpArrow hatching     */
#define	SCR_DWN_MASK	0x02		/* DownArrow hatching   */
#define	SCR_ALL_MASK	(SCR_UP_MASK | SCR_DWN_MASK)

/* TouchObjectCode(GeneralCharacterInputting/for data display) */
#define	OBJ_TXTTCH			0x9002
#define OBJ_SCR_BAR			0xc011

/* FlashErrorCode */
#define	IW_NORMALCY			0x0000	/* No problem                                */
#define	IW_NUL_BUFF			0x0001	/* No data to register. */
#define	IW_WRITE_ERR		0x0002	/* xÅiused insideÅj                          */
#define	IW_FORMAT_ERR		0x0003	/* Fatal error                               */
#define	IW_MAX_CHAR_ERR		0x0004	/* The number of characters to be registered exceed the limit. */
#define	IW_TME_DATE_ERR		0x0005	/* The data other than numeric number exists. (Date/Time) */
#define	IW_OUT_PNTA_ERR		0x0006	/* The setting pointer has an illegal value (out of range). */
#define	IW_TWIN_DAT_ERR		0x0007	/* When deleting data in TEL<COMPANY> mode,
										there is no 4-byte data to be paired.	*/
#define	IW_MEMORY_OVER		0x0008	/* Unable to register because of the insufficient memory capacity. */
#define	IW_ARM_TOTL_ERR		0x0010	/* The number of registered alarms exceed the maximum. */
#define	IW_ARM_SAMV_ERR		0x0011	/* The same alarm value already exists. */
#define	IW_ARM_NULL_ERR		0x0012	/* Alarm is empty. */
#define	IW_CRADLE_ERR		0x0020	/* The cradle ID of the write destination is not FFFFh. */
#define	IW_PARAMET_ERR		0xFFFE	/* Illegal value is set: Debug purpose only.  */
#define	IW_NO_DATA_ERR		0xFFFF	/* No data exists for search. */
#define	IW_ARM_OLDV_ERR		0xB000	/* Alarm is the past date. */
#define	IW_SCRT_ACS_ERR		0xA000	/* Access a SecretData in OpenMode */

/* GeneralCharacterInput/DataDisplay */
#define		TXTNOP		0x00		/* NOP                               */
#define		TXTCUT		0x0B		/* CUT                               */
#define		TXTCOPY		0x0C		/* COPY                              */
#define		TXTPASTE	0x0D		/* PASTE                             */
#define		TXTDYTIM	0x0F		/* Paste of date&time                */
#define		TXTMJKEY	0x01
#define		TXTCSMV		0x02
#define		TXTCSUP		0x03
#define		TXTCSDWN	0x04
#define		TXTBS		0x05
#define		TXTDEL		0x06
#define		TXTDRG		0x07
#define		TXTDGDN		0x08
#define		TXTDGUP		0x09
#define		TXTSBAR		0x0A
#define		TXTREDSP	0x0E

/*== ipdpst(InputStatusFlag) ==*/
#define		NOCHG		0x00		/* It is not in the re-display.      */
#define		CSRMOV		0x10		/* The re-display only about the cursor. */
#define		CSRPOS		0x20		/* It rewrites a letter on the cursor.   */
#define		UPSCRL		0x30		/* Up scroll                         */
#define		DNSCRL		0x40		/* Down scroll                       */
#define		CSRAFT		0x50		/* It rewrites since the cursor.     */
#define		CSRAFT2		0x60		/* It rewrites since the cursor.(The cursor position fixation) */
#define		UPSCWRT		0x70		/* Up scrolled above,it rewrites since the cursor. */
#define		PRECSL		0x80		/* It rewrites since the line in front of the cursor. */
#define		ALLTXT		0x90		/* It rewrites all texts.            */
#define		ALLTXT2		0xA0		/* It rewrites all texts.(with the cursor revision) */
#define		fntx_min	2			/* The minimum font width            */

/* LibDateDisp() input condition */
#define		IN_MODE		0			/* for input    */
#define		DISP_MODE	1			/* for display  */
#define		DISP_YM		2			/* year / month */
#define		DISP_MY		3			/* month / year */
#define		DISP_MYS	5			/* month / year (2 digits below) */
#define		IN_YM		4			/* input year and month */

#define		WEEK_ON		1			/* day of the week ON  */
#define		WEEK_OFF	0			/* day of the week OFF */

/* message window icon classification */
#define ICON_NONE		0    		/* none          */
#define ICON_OK			1    		/* OK            */
#define ICON_BADTZ		2    		/* Å~            */
#define ICON_BIKKURI	3    		/* ÅI            */
#define ICON_COFFEE		4    		/* Coffee cup    */
#define ICON_TRASH		5    		/* Trash box     */
#define ICON_SIGN		6    		/* Hand          */
#define ICON_SYNC		7    		/* communicating */

/* Drawing BIOS DrawMode */
#define DRW_REPLACE			0
#define DRW_AND				1
#define DRW_OR				2
#define DRW_XOR				3

/* WAIT BIOS */
#define	IB_125MWAIT 	0x00		/* 125mSec      */
#define	IB_250MWAIT		0x01		/* 250mSec      */
#define	IB_500MWAIT		0x02		/* 500mSec      */
#define	IB_1SWAIT		0x03		/* 1Sec	        */

/* PullDown (index item specification) */
#define PULLDOWN_EDIT  			0x8000	/* [Edit]            */
#define PULLDOWN_SYSTEM			0x4000	/* [System]          */
#define PULLDOWN_OPTION			0x2000	/* [Option]          */
#define PULLDOWN_RESERV			0x1000	/* [Reserve]         */
#define PULLDOWN_IDX_MASK		0xF000	/* index item mask   */

/* PullDown (index attribute specification) */
#define PULLDOWN_NONDSP			0x01	/* makes a non-display */
#define PULLDOWN_HTNDSP			0x02	/* makes a hatching display  */
#define	PDNTBLEND				0xFFFF	/* PullDown Table END code */

/* SecretMode CallFunction */
#define PASS_CHECK		0x00	/* Password check                  */
#define PASS_GO_SECRET	0x01	/* to secret mode (with password check) */
#define	PASS_EDIT		0x02	/* Password correction             */
#define	PASS_GO_OPEN	0x03	/* to open mode                    */

/* TimeSettingKeyboard RetuenValue(Upper8bit) */
#define CLKKEYB_RSLT_NONE		0		/* NO EVENT                            */
#define CLKKEYB_RSLT_NEXTITEM	0x0f	/* After setting in the minute         */
#define CLKKEYB_RSLT_PREVITEM	0x0e	/* It pushed the left key when the cursor was in 0. */
#define CLKKEYB_RSLT_OUTOFKEYB	0x0d	/* Invalid area                        */
#define CLKKEYB_RSLT_KEYBSWAP	0x0c	/* It is a touch in the time bar switching button. */
#define CLKKEYB_RSLT_DISPGUIDE	0x0b	/* It became a guidance display condition. */

/* BlockTypeIconFunctoin Arguments */
#define B_ICON_LEFT		1
#define B_ICON_CENTER	0
#define B_ICON_RIGHT	2

/* Calculator size information */
#define CAL_EZSIZE		11		/* Maximum figure   */
#define CAL_BUFSIZE		14		/* buffer size for the calculation value */
#define CAL_MBUFSIZE	14		/* buffer size for the memory calculation value */
#define CAL_SISUUKIJUN	100		/* index standard value */

/* CalculatorKeyboardRelationObjectCode */
#define	OBJ_CAL_NEXT	0xc947	/* [NEXT] Key   */
#define	OBJ_CAL_EQUAL	0xc948	/* [=] Key      */

/* CalenderKeyboardRelationObjectCode */
#define	OBJ_CALE_NEXT	0x8665	/* [NEXT] Key   */

/**************************/
/* Used LibGetDLAllNum() */
#define	IB_DLALL_COUNT	0x00	/* Program AND Data		*/
#define	IB_DLPROG_COUNT	0x01	/* Program Only			*/
#define	IB_DLDATA_COUNT	0x02	/* Data Only			*/
/* Used LibGetUserMode() */
#define	IB_DLFIRST_SRCH	0x00	/* First search			*/
#define	IB_DLNEXT_SRCH	0x01	/* Next search			*/

/* Addin synchro BIOS */
#define	IX_ADIN_SUCCESS		0x0000	/* Success					*/
#define	IX_ADIN_COMMERR		0xffff	/* Time out or communication failure */
#define	IX_ADIN_DETECTBLD	0xfffe	/* BLD1 detection			*/
#define	IX_ADIN_DATAFULL	0xfffd	/* User data area FULL  	*/

/* flash BIOS Sub entry */
/* SubEntryStat output code */
#define	IB_SERR_RNEW	0x00	/* New registration					*/
#define	IB_SERR_RALDY	0x01	/* It has registered already.		*/
#define	IB_SERR_FOPEN	0xFF	/* Pointer for the input doesn't have a specification file name. */
#define	IB_SERR_NOSUBC	0xFE	/* SubEntry FULL					*/
#define	IB_SERR_NGSUBC	0xFD	/* SubEntryNo. outside of the range*/
#define	IB_SERR_ALDYFL	0xFC	/* already used file name*/
#define	IB_SERR_NOFILE	0xFB	/* no exist file name*/
#define	IB_SERR_INJUST	0xF0	/* input condition error(Illegal function No.)	*/

/* Flash Memory Access Library Definations */
#define  FILE_KIND_BIN	0x01		/* Binary File */
#define  FILE_KIND_TEXT 0x00		/* Text File */
#define  FILE_OPEN_MODE 0x80		/* open mode */
#define  FILE_SECRET_MODE 0x00		/* secret mode */
#define  FILE_BLOCK_SIZE 64			/* 1block size */

/* SerialCommunicationLibrary */
/* PortNo.	*/
#define	IB_SRL_COM1			0x00	/* COM1( 3PIN RS-232C)					*/
#define	IB_SRL_COM2			0x01	/* COM2( 9PIN RS-232C)					*/
#define	IB_SRL_COM3			0x02	/* COM3(68PIN) Not used					*/

/* CommunicationPortInitial */
#define	IB_SRL_300BPS		0x02	/* 300bps		*/
#define	IB_SRL_600BPS		0x03	/* 600bps		*/
#define	IB_SRL_1200BPS		0x04	/* 1200bps		*/
#define	IB_SRL_2400BPS		0x05	/* 2400bps		*/
#define	IB_SRL_4800BPS		0x06	/* 4800bps		*/
#define	IB_SRL_9600BPS		0x07	/* 9600bps		*/
#define	IB_SRL_19200BPS		0x08	/* 19200bps		*/
#define	IB_SRL_38400BPS		0x09	/* 38400bps		*/
#define	IB_SRL_57600BPS		0x0A	/* 57600bps		*/
#define	IB_SRL_115200BPS	0x0B	/* 115200bps	*/

#define	IX_SRL_NOFLOW		0x00	/* 00000000 ;Not flow control		*/
#define	IX_SRL_RSCS			0x02	/* 00000010 ;RS/CS flow control		*/
#define	IX_SRL_XONOFF		0x03	/* 00000011 ;XON/XOFF+RS/CS flow control */
#define	IX_SRL_XONOFFONLY	0x01	/* 00000001 ;XON/XOFF flow control	*/

#define	IX_SRL_7DATA		0x08	/* 00001000 ;data 7 bit	*/
#define	IX_SRL_8DATA		0x0C	/* 00001100 ;data 8 bit	*/

#define	IX_SRL_NONE			0x00	/* 00000000 ;None parity	*/
#define	IX_SRL_ODD			0x10	/* 00010000 ;Odd parity		*/
#define	IX_SRL_EVEN			0x30	/* 00110000 ;Even parity	*/

#define	IX_SRL_1STOP		0x40	/* 01000000 ;stop 1bit	*/
#define	IX_SRL_2STOP		0xC0	/* 11000000 ;stop 2bit	*/

/* ErrorCode */
#define	IW_SRL_NOERR		0x0000	/* No error				*/
#define	IW_SRL_PRMERR		0x0001	/* Input parameter error	*/
#define	IW_SRL_TRSERR		0x5002	/* Can not data sendind	*/
#define	IW_SRL_NODATA		0x0003	/* No receive data		*/
#define	IW_SRL_RCVERR		0x1004	/* Receive status error	*/
#define	IW_SRL_OVFERR		0x1006	/* Receive buffer over flow	*/
#define	IW_SRL_CLSERR		0x5007	/* Can not close		*/
#define	IW_SRL_TMOERR		0x100A	/* time out error		*/
#define	IW_SRL_CNCTERR		0x000C	/* Conect error			*/
#define	IW_SRL_RCHGERR		0x500D	/* Can not change baudrate	*/
#define	IW_SRL_FUNCERR		0xFFFF	/* */

/* SerialSignalLineStatus */
#define	IX_SRL_ER			0x01	/*	00000001	*/
#define	IX_SRL_RS			0x02	/*	00000010	*/
#define	IX_SRL_CS			0x20	/*	00100000	*/
#define	IX_SRL_CD			0x40	/*	01000000	*/
#define	IX_SRL_DR			0x80	/*	10000000	*/

#define	IX_SRL_TXEMP		0x0020	/* 00000000 00100000 ;sending register empty*/
#define	IX_SRL_FE			0x0008	/* 00000000 00001000 ;flaming error		*/
#define	IX_SRL_PE			0x0004	/* 00000000 00000100 ;parity error		*/
#define	IX_SRL_OE			0x0002	/* 00000000 00000010 ;over run error	*/
#define	IX_SRL_RXRDY		0x0001	/* 00000000 00000001 ;receive data ready	*/
#define	IX_SRL_CB			0x0200	/* 00000010 00000000 ;DCE busy			*/
#define	IX_SRL_TB			0x0400	/* 00000100 00000000 ;DTE busy			*/
#define	IX_SRL_OV			0x0100	/* 00000001 00000000 ;receive buffer over flow error*/

#define	IB_FOLLOW_BUSY		0x00	/* follows a flow control	*/
#define	IB_IGNORE_BUSY		0x01	/* ignores a flow control   */

/* Open Status */
#define	IB_NO_OPEN		0x00	/* NOT OPEN PORT	*/
#define	IB_COM1_OPEN	0x01	/* 3PIN PORT OPEN	*/
#define	IB_COM2_OPEN	0x41	/* 9PIN PORT OPEN	*/

/************************************************/
/*               define type                    */
/************************************************/
/* TouchTbaleInformation(TouchEventBIOS InputCondition) */
typedef struct TCHTBL{
	int		x1;				/* TouchArea X1 */
	int		y1;				/* TouchArea Y1 */
	int		x2;				/* TouchArea X2 */
	int		y2;				/* TouchArea Y2 */
	dword	act;			/* ActionCode   */
	word	obj;			/* ObjectCode(for Touch Judgment) */
	word	ext;			/* END Code(for the extension)    */
} TCHTBL;

/* TouchStatusInformation(TouchEventBIOS OutputCondition) */
typedef struct TCHSTS {
	word	obj;			/* ObjectCode */
	dword	act;			/* ActionCode */
	int		x;				/* TouchPosition X */
	int		y;				/* TouchPosition Y */
	word	ext;			/* END Code(for the extension) */
	byte	istr[4];		/* <not used> */
} TCHSTS;

/* TouchIconInformation */
typedef struct T_ICON{
	TCHTBL	far *tch;		/* TouchTableInformation */
	byte	far *ngp;		/* IconGraphicPattern(Normal) */
	byte	far *rgp;		/* IconGraphicPattern(Reverse)*/
	byte	kind;			/* ReverseType */
} T_ICON;

/* ScrollBarPositionInformation */
typedef struct T_SCR_POS{
	int		x;				/* ScrollBarDisplayStartPosition X */
	int		y;				/* ScrollBarDisplayStartPosition Y */
	int		size;			/* ScrollBarHeight */
	int		vol;			/* TotalCount      */
	int		dsp;			/* DisplayCount    */
	int		pos;			/* DisplayStartDataPosition */
} T_SCR_POS;

/* SelectWindowPositionInformation */
typedef struct SLW_TBL{
	int		msg;			/* DisplayMessageNo. */
	byte	rtv;			/* The return value in choice */
} SLW_TBL;

/* SelectWindowPositionInformation */
typedef struct SLW2_ST{
	int		x;				/* WindowDisplayStartPosition X */
	int		y;				/* WindowDisplayStartPosition Y */
	int		xs;				/* WindowDisplyWhith (X size)    */
	int		ity;			/* CommentsLinePitch             */
	byte 	np;				/* ReverseCursorDefaultPosition */
	byte 	*cmnt;			/* DisplayCommentsBufferPointer  */
	byte	*rtv;			/* Buffer pointer for return value in choice */
	byte	t_xs;			/* StringDisplayStartPosition */
}SLW2_ST;

/* FLASH data buffer (binaryBuffer 3KB)*/
typedef struct FILE_BUF {
	byte	fsb_main_entry_;			/* MainEntryNo. */
	byte	fsb_sub_entry_;				/* SubEntryNo.  */

	byte	fsb_scrt_info_;				/* SecretInformation */
	byte	fsb_ararm_chk_;				/* AlarmCheckInformation */

	byte	fsb_todo_chek_;				/* TODO CheckInformation */
	byte	fsb_todo_rank_;				/* TODO RankInformation  */

	byte	fsb_date1_buf_[8];			/* Date/DueDate */
	byte	fsb_date2_buf_[8];			/* CheckDate     */
	byte	fsb_date_aram_[8];			/* AlarmDate     */
	byte	fsb_time1_buf_[4];			/* Time/DueTime(From)    */
	byte	fsb_time2_buf_[4];			/* Time/DueTime(To)      */
	byte	fsb_time3_buf_[4];			/* CheckTime             */
	byte	fsb_time_aram_[4];			/* AlarmTime             */

	byte	fsb_date_srch_[8];			/* SearchBuffer Date      */
	byte	fsb_time_srch_[4];			/* SearchBuffer Time      */
	byte	fsb_text_srch_[33];			/* SearchBuffer RealData(text) */

	union{
		struct{
			byte	fsb_text_buf_[2048+1];/* Buffer to store real data (TEXT)    */
		}text;

		struct{
			byte	dummy_16by[22];		/* MemoryManagementWork      */
			word	char_num;			/* BinaryDataCapacity        */
			byte	bin_buf[3072];		/* Buffer to store real data (BINARY) 3KB */
		}bin;

	}fbuf;
} FILE_BUF;

/* FLASH data buffer (BinaryBuffer 30KB) CAUTION:Allocate a buffer for FAR_BSS.*/
typedef struct LFILE_BUF {
	byte	fsb_main_entry_;			/* MainEntryNo. */
	byte	fsb_sub_entry_;				/* SubEntryNo.  */

	byte	fsb_scrt_info_;				/* SecretInformation */
	byte	fsb_ararm_chk_;				/* AlarmCheckInformation */

	byte	fsb_todo_chek_;				/* TODO CheckInformation */
	byte	fsb_todo_rank_;				/* TODO RankInformation  */

	byte	fsb_date1_buf_[8];			/* Date/DueDate */
	byte	fsb_date2_buf_[8];			/* CheckDate    */
	byte	fsb_date_aram_[8];			/* AlarmDate    */
	byte	fsb_time1_buf_[4];			/* Time/DueTime(From)     */
	byte	fsb_time2_buf_[4];			/* Tiem/DueTime(To)       */
	byte	fsb_time3_buf_[4];			/* CheckTime              */
	byte	fsb_time_aram_[4];			/* AlarmTime              */

	byte	fsb_date_srch_[8];			/* SearchBuffer Date      */
	byte	fsb_time_srch_[4];			/* SearchBuffer Time      */
	byte	fsb_text_srch_[33];			/* SearchBuffer RealData(TEXT)  */

	union{
		struct{
			byte	fsb_text_buf_[2048+1];/* Buffer to store real data (TEXT)    */
		}text;

		struct{
			byte	dummy_16by[22];		/* MemoryManagementWork   */
			word	char_num;			/* BinaryDataCapacity     */
			byte	bin_buf[30720];		/* Buffer to store real data (BINARY) 30KB  */
		}bin;

	}fbuf;
} LFILE_BUF;

/* FLASH data buffer(for Calender Keyboard Internal Process) */
typedef struct FILE_BUF_CALE {
	byte	fsb_main_entry_;			/* MainEntryNo. */
	byte	fsb_sub_entry_;				/* SubEntryNo.  */

	byte	fsb_scrt_info_;				/* SecretInformation */
	byte	fsb_ararm_chk_;				/* AlarmCheckInformation */

	byte	fsb_todo_chek_;				/* TODO CheckInformation  */
	byte	fsb_todo_rank_;				/* TODO RankInformation   */

	byte	fsb_date1_buf_[8];			/* Date/DueDate           */
	byte	fsb_date2_buf_[8];			/* CheckDate              */
	byte	fsb_date_aram_[8];			/* AlarmDate              */
	byte	fsb_time1_buf_[4];			/* Time/DueTime(From)     */
	byte	fsb_time2_buf_[4];			/* Time/DueTime(To)       */
	byte	fsb_time3_buf_[4];			/* CheckTime              */
	byte	fsb_time_aram_[4];			/* AlarmTime              */

	byte	fsb_date_srch_[8];			/* SearchBuffer	Date      */
	byte	fsb_time_srch_[4];			/* SearchBuffer Time      */
	byte	fsb_text_srch_[33];			/* SearchBuffer RealData(TEXT) */

	union{
		struct{
			byte	fsb_text_buf_[128];/* Buffer to store real data (TEXT) */
		}text;

		struct{
			byte	dummy_16by[22];		/* MemoryManagemantWork   */
			word	char_num;			/* BinaryDataCapacity     */
			byte	bin_buf[100];		/* Buffer to store real data (BINARY) */
		}bin;

	}fbuf;
} FILE_BUF_CALE;

/* FLASH data information */
typedef struct FILE_INFO {
	word	fp;				/*	file pointer	*/
	byte	kind;			/*	TEXT/BINARY identification */
} FILE_INFO;

/* GeneralCharacterInput/DataDisplay ParameterInformation */
typedef struct  TXTP {

	/* TextInputLibrary InputCondition */
	int			st_x;		/* TextArea Left X position */
	int			ed_x;		/* TextArea Right X position */
	int			st_y;		/* TextArea Up Y position    */
	int			it_y;		/* TextAreaLinePitch         */
	int			MAXGYO;		/* TextDisplayMaximumLineNumber */
	word		maxmj;		/* MaximumInputCharacterNumber  */
	byte		font;		/* FontSizeSpecification */
	bool		csen;		/* Cursor DisplayPermission */
	byte		rtnen;		/* LineFeedMark DisplayPermission */
	byte		*txbf;		/* TextBufferPointer     */
	word		*gdcmt;		/* GuidanceCommentsTablePointer */
	byte		*gdcmt2;	/* GuidanceCommentsBufferPointer */
	word		txtobj;		/* TextAreaTouchObject   */
	word		sbrobj;		/* ScrollBarTouchObject  */
	byte		*kwb_0;		/* KeywordBuffer0 Pointer */
	byte		*kwb_1;		/* KeywordBuffer1 Pointer */
	byte		*kwb_2;		/* KeywordBuffer2 Pointer */
	byte		*kw_dbk;	/* KeywordScreenShelterPointer */
	TCHTBL		*tchtb;		/* TouchTablePointer */
	T_SCR_POS	*s_pos;		/* ScrollBarPositionInformationPointer */

	/* TextInputLibraryWorkRAM */
	word		cp;			/* CursorPointer         */
	int			xp;			/* Display X position    */
	int			yp;			/* Display Y position    */
	int			c_xp;		/* Display Cursor X position */
	int			c_yp;		/* Display Cursor Y position */
	word		dlmp[22];	/* Line top character pointer */
	byte		ditp[22];	/* Line top character pointer */
	word		csln;		/* Cursor display line position */
	word		mjln;		/* Character draw start position */
	word		mjcnt;		/* Character Capacity in Text buffer */
	byte		ipdpst;		/* Input Display Status Flag */
	byte		txtst;		/* TextInput Status Flag */
	word		sr_sp;		/* Reverse start position pointer */
	word		sr_ep;		/* Reverse end position pointer */
	int			sr_xp;		/* Reverse end position pointer X */
	int			sr_yp;		/* Reverse end position pointer Y */
	word		srln;		/* Reverse line number            */
	bool		selrv;		/* Select reverse flag   */
	word		ktyp;		/* For keyboard type shelter */
	byte		citm;		/* Now cursor item       */
	byte		wdcnt;		/* Input string length   */
	byte		kwnmt;		/* Candidate display flag */
	byte		kwsln;		/* Candidate display select line */

} TXTP;

/* Drawing InitialInformation */
typedef struct INIT_PACS{
	int		RaOfs;			/* RealData VRAM(offset)  */
	int		RaSeg;			/* RealData VRAM(segment) */
	int		BkOfs;			/* Background VRAM(offset)   */
	int		BkSeg;			/* Background VRAM(segment)  */
} INIT_PACS;

/* DrawingAreaInformation */
typedef struct SETCLIPAREA_PACS{
	int		Sx;				/* Start X position */
	int		Sy;				/* Start Y position */
	int		Ex;				/* End X position */
	int		Ey;				/* End Y position */
} SETCLIPAREA_PACS;

/* LineDrawingInformation */
typedef struct DRAWLINE_PACS{
	int		x1;				/* Start X position */
	int		y1;				/* Start Y position */
	int		x2;				/* End X position */
	int		y2;				/* End Y position */
	int		Mode;			/* Drawing mode   */
	int		Size;			/* Pen size       */
	int		Style;			/* Line style     */
} DRAWLINE_PACS;

/* Drawing BOX DrawingInformation */
#define DRAWBOX_PACS		DRAWLINE_PACS

/* Drawing CircleDrawingInformation */
typedef struct DRAWCIRCLE_PACS{
	int		x;				/* Center X position */
	int		y;				/* Center Y position */
	int		r;				/* Radius */
	int		Dummy;			/* <Dummy>               */
	int		Mode;			/* Drawing mode          */
	int		Size;			/* Pen size              */
} DRAWCIRCLE_PACS;

/* Drawing FillInformation */
#define FillArea_PACS	SETCLIPAREA_PACS

/* Drawing VRAM SendingInformation */
#define TransDD_PACS	SETCLIPAREA_PACS

/* Drawing VRAM ImageWritingInformation */
typedef struct PutImage_PACS{
	int		x;				/* ImageOutputPosition X */
	int		y;				/* ImageOutputPosition Y */
	int		Width;			/* ImageWidth            */
	int		Height;			/* ImageHeight           */
	int		Mode;			/* WritingMode           */
	int		ofs;			/* ImageBufferOffset     */
	int		seg;			/* ImageBufferSegments   */
} PutImage_PACS;

/* Drawing VRAM GetImageInformation */
typedef struct GetImage_PACS{
	int		sx;				/* GetImageAreaLeftTopPosition X */
	int		sy;				/* GetImageAreaLeftTopPosition Y */
	int		width;			/* GetImageWidth          */
	int		height;			/* GetImageHeight         */
	int		dummy;			/* <Dummy>                */
	int		ofs;			/* ImageWritingOffset   */
	int		seg;			/* ImageWritingSegments */
} GetImage_PACS;

/* Drawing ImageCompressionInformation */
typedef struct ReductImage_PACS{
	int		sofs;			/* Compression SourceImageOffset */
	int		sseg;			/* Compression SourceImageSegments */
	int		swidth;			/* Compression SourceImageWidth    */
	int		sheight;		/* Compression SourceImageHeight   */
	int		dofs;			/* Compression DestinationImageOffset */
	int		dseg;			/* Compression DestinationImageSegmants */
	int		dwidth;			/* Compression DestinationImageWidth */
	int		dheight;		/* Compression DestinationImageHeight */
} ReductImage_PACS;

/* ScreenShotImageInformation */
typedef struct SHOT_INF{
	int		x;				/* ImageAreaLeftTopPosition X */
	int		y;				/* ImageAreaLeftTopPosition Y */
	int		x_size;			/* ImageWidth                 */
	int		y_size;			/* ImageHeight                */
	int		p_x;			/* WritingPosition X          */
	int		p_y;			/* WritingPosition Y          */
	byte	g_buf[3072];	/* ImageDataBuffer            */

} SHOT_INF;

/* TimeKeyboardTable */
typedef struct tTIMEKEYBCTRL {
	int		xsp,ysp;		/* SquareLeftTopPosition                    */
	int		xep,yep;		/* SquareRightTopPosition                   */
	int		font;			/* FontType                                 */
	int		csrpos;			/* NowCursorPosition                        */
	char	barpos;			/* for internal process                     */
	char	barshift;		/* for internal process                     */
	bool	enabletimebar; 	/* It sets 'TRUE' when permitting TimBarbutton   */
	bool	existnextitem; 	/* It sets 'TRUE' when there is NextItem */
	bool	existprevitem; 	/* It sets 'TRUE' when there is PreItem */
	byte far *timbuf;		/* Store buffer ('HHMMA'+00h)                  */
	byte far *guide;		/* Guidance strings(equal or to less than 31 bytes) */
} TIMEKEYBCTRL,*PTIMEKEYBCTRL,far *LPTIMEKEYBCTRL;

/* BreakKeySample ESC IconTouchTable */
typedef struct BK_SMPL_TCH{
	int		x1;			/* LeftTopPosition X */
	int		y1;			/* LeftTopPosition Y */
	int		x2;			/* RightDownPosition X */
	int		y2;			/* RightDownPosition Y */
} BK_SMPL_TCH;

/* Calculator data buffer */
typedef struct caldata{
	byte	calkey;						/* Input key        */
	byte	calichi;					/* Status           */
	byte	calerror;					/* Error flag       */
	byte	calkonkai;					/* Input status (now value) */
	byte	calzenkai;					/* Input status (last value) */
	byte	calenzanf;					/* Calculation flag  */
	byte	calxbuf[CAL_BUFSIZE];		/* Calculation X Buffer */
	byte	calybuf[CAL_BUFSIZE];		/* Calculation Y Buffer */
	byte	calzbuf[CAL_BUFSIZE];		/* Fixed number buffer */
	byte	calxdata[CAL_EZSIZE];		/* Calculation X value buffer */
	byte	calydata[CAL_EZSIZE];		/* Calculation Y value buffer */
	byte	calmbufcp[CAL_BUFSIZE];		/* Memory Value shelter buffer */
	byte	calycpbuf[CAL_BUFSIZE];		/* Calculation shelter buffer  */
	byte	caltisuk;					/* Placed number flag */
	byte	calperprs;					/* +% control flag */
	byte	calnoover;					/* +% overflow flag */
} CALWRAM;

/* SerialCommunicationStatus */
typedef struct SRL_STAT {
	byte	port;			/* Port No.			*/
	byte	speed;			/* Bouadrate		*/
	byte	parit;			/* Parity bit		*/
	byte	datab;			/* DataBitLength	*/
	byte	stopb;			/* StopBitLength	*/
	byte	fctrl;			/* FlowControl		*/
} SRL_STAT;

#endif
