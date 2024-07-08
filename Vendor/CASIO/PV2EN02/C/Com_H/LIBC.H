/******************************************************************************
	Copyright (C) 1999 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef CLIBDEF
#define CLIBDEF
#include	"define.h"

/************************************************/
/*           LibraryPrototype		            */
/************************************************/
/* Display */
byte LibLine(int x, int y, int xsize, int ysize, byte bold);
void LibMeshLine(int x, int y, int xsize, int ysize);
void LibPutGraph(int x, int y, const byte far *graph);
void LibPutGraphO(int px, int py, byte far *ptn, int mode);
void LibPutFarData(int x, int y, int no);
void LibGrpUp(int x, int y, int xsize, int ysize, int up_size);
void LibGrpDwn(int x, int y, int xsize, int ysize, int dw_size);
void LibPutDisp(void);
void LibPutDispBox(int x, int y, int xsize, int ysize);
void LibClrDisp(void);
void LibClrBox(int x, int y, int xsize, int ysize);
void LibSetDispMode(bool flag);
void LibInitDisp(void);
int LibPutProFont(byte type, byte code, int x_pos,int y_pos);
int LibGetProFontSize(byte type, byte code);
void LibPutProStr(byte type,int x,int y,byte *string,int x_size);
int LibGetProStrSize(byte type,byte *string);
void LibReverse(int px, int py, int xsize, int ysize);
void LibMesh(int px, int py, int xsize, int ysize);
byte LibBox(int x, int y, int xsize, int ysize, byte type);
void LibLineClr(int x, int y, int xsize, int ysize);
void LibLineCplmnt(int x, int y, int xsize, int ysize);
void LibDotOn(int x, int y);
void LibDotOff(int x, int y);
void LibGetGraph(int x, int y, int xsize, int ysize, byte far *ubfptr);
void LibPutGraphM(int x, int y, const byte far *graph);
void LibPut35Font(byte code,int x_pos,int y_pos);
void LibPut35Str(int x,int y,byte *string);
void LibGdsBox(int x,int y,int x2,int y2);
void LibGdsBoxMesh(int x,int y,int x2,int y2);
void LibGdsBoxClr(int x,int y,int x2,int y2);
void LibGdsBoxCmp(int x,int y,int x2,int y2);
void LibGdsClr(int x,int y,int x2,int y2);
void LibGdsReverse(int x,int y,int x2,int y2);
void LibGdsMesh(int x,int y,int x2,int y2);
void LibGdsDotOn(int x,int y);
void LibGdsDotOff(int x,int y);
void LibGdsDotCmp(int x,int y);
void LibGdsLine(int x,int y,int x2,int y2);
void LibGdsLineClr(int x,int y,int x2,int y2);
void LibGdsLineMesh(int x,int y,int x2,int y2);
void LibGdsLineCmp(int x,int y,int x2,int y2);
void LibGrphUpSideDown(byte *gw, const byte *gs);
void LibStringDsp(byte *strg, word xps, word yps, word lmtx, byte font);

/* Window */
bool LibOpenWindow(int x, int y, int xsize, int ysize);
void LibOpenWindowS(byte flame, word px, word py, word xsize, word ysize);
void LibCloseWindow(void);

/* Flash */
bool LibFileFindNext(const FILE_BUF *fb, FILE_INFO *finf, byte search);
bool LibFileFindPrev(const FILE_BUF *fb, FILE_INFO *finf, byte search);
bool LibFileFindNextExt(const FILE_BUF *fb, FILE_INFO *finf, byte search);
bool LibNextSearchCld(FILE_BUF *fb, FILE_INFO *finf, byte search);
bool LibFileRead(FILE_BUF *fb, const FILE_INFO *finf);
bool LibFileWrite(const FILE_BUF *fb, FILE_INFO *finf);
bool LibFileCorect(const FILE_BUF *fb, FILE_INFO *finf, byte type);
bool LibFileRemove(const FILE_BUF *fb, const FILE_INFO *finf);
bool LibFileRemoveAll(const FILE_BUF *fb);
void LibGetFileInfo(FILE_BUF *fd, FILE_INFO *fi);
word LibGetFileCnt(FILE_BUF *fd);
word LibGetFlash(void);
word LibGetFreeBlock(void);
bool LibGetDataCond(void);
bool LibFileRemake(void);
void LibTodoFileRemove(FILE_BUF *fd);
bool LibFileExch(const FILE_BUF *fb, FILE_INFO *finf, const word mvp);
word LibTelPtCnvrt(word fp);
void LibFileWriteCheckInit(void);
bool LibFileWriteCheck(const FILE_BUF *fb);
bool LibFileReadEx(FILE_BUF *fb, const FILE_INFO *finf,int maxblock);

bool LibLFileFindNext(const LFILE_BUF far *fb, FILE_INFO *finf, byte search);
bool LibLFileFindPrev(const LFILE_BUF far *fb, FILE_INFO *finf, byte search);
bool LibLFileFindNextExt(const LFILE_BUF far *fb, FILE_INFO *finf, byte search);
bool LibLNextSearchCld(LFILE_BUF far *fb, FILE_INFO *finf, byte search);
bool LibLFileRead(LFILE_BUF far *fb, const FILE_INFO *finf);
bool LibLFileWrite(const LFILE_BUF far *fb, FILE_INFO *finf);
bool LibLFileCorect(const LFILE_BUF far *fb, FILE_INFO *finf, byte type);
bool LibLFileRemove(const LFILE_BUF far *fb, const FILE_INFO *finf);
bool LibLFileRemoveAll(const LFILE_BUF far *fb);
void LibLGetFileInfo(LFILE_BUF far *fd, FILE_INFO *fi);
word LibLGetFileCnt(LFILE_BUF far *fd);
void LibLTodoFileRemove(LFILE_BUF far *fd);
bool LibLFileExch(const LFILE_BUF far *fb, FILE_INFO *finf, const word mvp);
bool LibLFileWriteCheck(const LFILE_BUF far *fb);
bool LibLFileReadEx(LFILE_BUF far *fb, const FILE_INFO *finf,int maxblock);

/* Touch */
void LibTchStackClr(void);
bool LibTchStackPush(TCHTBL far *tbl);
TCHTBL far *LibTchStackPop(void);
void LibTchWait(TCHSTS *tsts);
void LibTchInit(void);
void LibIconMoveDown(byte far *inbuf, byte *workbuf, byte kind);
void LibIconMoveUp(byte far *inbuf, byte *workbuf, byte kind);
void LibIconPrint(const T_ICON far *icon);
void LibIconPrintR(const T_ICON far *icon);
void LibIconPrintM(const T_ICON far *icon);
bool LibIconClick(const T_ICON far *icon, const TCHSTS *tsts);
bool LibIconClick2(const T_ICON far *icon, const TCHSTS *tsts);
void LibKeyInit(void);
byte LibGetKeyM(TCHSTS *tsts);
void LibDispKey(void);
void LibScrollPrint(T_SCR_POS scr);
void LibScrollArrowPrint(T_SCR_POS scr,byte mask);
int LibScrollClick(TCHSTS *tsts,T_SCR_POS *scr_pos);
int LibScrPosCheck(TCHSTS tsts,T_SCR_POS scr);
void LibCldKeyInit(TCHSTS *tsts, byte *db, int xsp, int ysp, byte type1, byte F_Type);
byte LibGetCale(byte *db, TCHSTS *tsts, int xsp, int ysp, int xep, word mes, byte type, byte type1, byte F_Type);
word LibInputTime(TCHSTS *tsts,LPTIMEKEYBCTRL lptbl,bool IsDispKeyb);
word LibInputTimeBar(TCHSTS *tsts,LPTIMEKEYBCTRL lpstbl,LPTIMEKEYBCTRL lpetbl);
word LibInputTerm(TCHSTS *tsts,LPTIMEKEYBCTRL lpstbl,LPTIMEKEYBCTRL lpetbl);
void LibBkSampleInit(BK_SMPL_TCH far *t_tbl);
byte LibBkSampleCheck(bool *done);
bool LibBlockIconClick(const T_ICON far *icon,TCHSTS *tsts,byte opt);
void LibRepOff(void);

/* Date/Time */
byte LibGetDateTimeM(byte *d_data);
byte LibGetDateTime(byte *yearh, byte *yearl, byte *month, byte *day,byte *hour, byte *minute, byte *second);
byte LibGetDateTime2(word *year2, byte *month2, byte *day2,byte *hour2, byte *minute2, byte *second2);
void LibSetDateTime(byte yearh, byte yearl, byte month, byte day, byte hour, byte minute);
byte LibGetDow(byte *buff);
byte LibGetDays(word *buf);
byte LibChangeTotalDay(word *year2, byte *month2, byte *day2, dword tday);
dword LibGetTotalDay2(word year2, byte month2, byte day2);
byte LibChkFuture(byte *tgt, byte *now, byte ct);
bool LibDateDisp(byte *buff, byte type, int xp, int yp, int xep, byte dmode, byte F_Type);
void LibWait(byte time);
bool LibCheckDate(byte *dbuff);
bool LibChkTimeBuf(byte far *buf);
void LibClkDispLine(LPTIMEKEYBCTRL lptbl);
void LibClkDispCursor(LPTIMEKEYBCTRL lptbl,int csrpos);
void LibConvRaw2Lib(byte *dest,byte *src,bool IsLarge);
void LibConvLib2Raw(byte *dest,byte *src);
int LibGetCursorPos(int x,int y,LPTIMEKEYBCTRL lptbl);
bool LibJumpDate(byte *s_date);

/* Menu */
byte LibCpMenu(void);
bool LibWinIcnMsg(byte icn, word msg, byte wtyp);
byte LibSelWindow(int x, int y, int xs, byte ln, byte np, SLW_TBL *ktb);
byte LibSelWindow2(int y, byte ln, SLW_TBL *ktb, word msk);
byte LibSelWindowExt(SLW2_ST *slw);
byte LibSelWinExt2A(SLW2_ST *slw);
byte LibSelWinExt2B(SLW2_ST *slw, byte ln);
word LibPullDown( void);
int  LibPullDownInit(word *edt,word *sys,word *opt);
int  LibPullDownAtrSet(int mode,word type,word item);
void LibEditPullDown(void);
void LibSelWinLckA(int x, int y, int ity, int xs, byte ln, SLW_TBL *ktb);
byte LibSelWinLckB(int x, int y, int ity, int xs, byte ln, byte np, SLW_TBL *ktb);
bool LibSelectFont(byte far *font);

/* Mode */
void LibJumpMenu(void);
void LibGetMode(word *m_code, word *m_sts, word *m_seg, word *m_ofs);
void far *LibDualWin(word m_code,byte m_sts,void far *ptr);
bool LibDualWinExit(void far *ptr);
bool LibModeJump(word m_code,byte m_sts);
void LibScrtJmp(byte m_sts,word m_ofs);
void LibScrtModeJmp(void);
void LibCrdlOpnJmp(void);
void LibMenuJump(word m_code);
void LibGetLastMode(word *m_code, word *m_sts, word *m_seg, word *m_ofs);
void LibDataCom(void);
void LibCallListMenu(void);
bool LibPassWordCheck(void);
void LibPassWordEdit(void);
void LibMoveArea(void);
void LibModeRestart(void);

/* Message */
void LibPutMessage(word no,int p_x,int p_y,byte type);
void LibPutMessageCenter(word no,int p_x1,int p_x2,int p_y,byte type);
void LibPutMessageCenter2(word no,int p_x,int p_y,byte type);
void LibPutMessageRight(word no,int p_x,int p_y,byte type);
void LibReadMessage(word no,byte *buf);
void LibErrorDisp(word err_code);

/* System */
word LibSaveSysRam(void);
word LibSaveSysRamB(void);
bool LibGetBLD(void);
void LibGetVersion(byte *ver_str);
void LibGetVer2(byte *ver_str);

/* String */
void LibBCD2Ascii(byte bcd, byte *ascii);
byte LibAscii2BCD(char *ascii);
void LibNumToStr(byte *buff, word target, byte j);
void LibSToNum(byte *buff, word *res_num, byte j);
bool LibCutTextRtn(byte *txbf);
void LibKeyWordInit(void);
void LibKeyWordSet(byte	*key_str);
bool LibKeyWordFSrch(byte *srch_str,byte *key_str);
bool LibKeyWordNSrch(byte *srch_str,byte *key_str);
void LibLblAreaWrite(byte *fb, byte typ);
void LibLblAreaRead(byte *fb, byte typ);

/* CharacterInput */
void LibTxtInit(TXTP *tp);
void LibTxtTchSet(TXTP *tp);
void LibTxtInp(byte keycd, TCHSTS *tsts, TXTP *tp);
bool LibTxtDsp(TXTP *tp);
bool LibTxtDspC(TXTP *tp);
void LibTxtDspInit(TXTP *tp);
void LibTxtDspS(TXTP *tp, TCHSTS *tsts);
bool LibGetCursor(int *c_xp, int *c_yp, int *c_xsize ,int *c_ysize);
void LibCurBlnkOn(int x, int y, int xsize, int ysize);
void LibCurBlnkOn2(int x, int y, int xsize, int ysize);
void LibCurBlnkOff(void);
void LibTxtKeyWordSet(TXTP *tp);

/* Alarm */
void LibAlarm(void);
void LibNextAlmSet(void);
void LibChkSysAlarm(void);

/* Drawing */
void LibDrawInit(INIT_PACS draw_prm);
void LibDrawSetPtn(byte val);
void LibDrawSetClipArea(SETCLIPAREA_PACS draw_prm);
void LibDrawSetPoint(byte page,int x,int y,byte point,byte mode);
void LibDrawLine(DRAWLINE_PACS draw_prm,byte page);
void LibDrawBox(DRAWBOX_PACS draw_prm,byte page);
void LibDrawCircle(DRAWCIRCLE_PACS draw_prm,byte page);
void LibDrawFillArea(FillArea_PACS draw_prm,byte page);
void LibDrawTransDD(TransDD_PACS draw_prm,byte dd);
void LibDrawTransAll(byte val);
void LibDrawPutImage(PutImage_PACS draw_prm,byte page);
bool LibDrawGetImage(GetImage_PACS draw_prm,byte page);
bool LibDrawReductImage(ReductImage_PACS draw_prm);
bool LibScrShot(SHOT_INF *s_inf);

/* Function */
void LibFuncDateTime(void);
void LibFuncSound(bool IsAlarmOnly);
void LibFuncFormat(void);
bool LibFuncLang(void);
void LibFuncCapa(void);
void LibFuncContrast(void);
void LibFuncDigitizer(void);
void LibFuncMemoryManagement(void);
bool LibFuncPtool(void);
void LibCalWin(byte tch_btn);

/* Calculator */
void LibCalBase(CALWRAM *calram,byte kind);
void LibCalBaseData(byte *a_dat,const byte *n_dat1,const byte *n_dat2,byte kind);
void LibCalRoot(CALWRAM *calram);
void LibCalKeyInit(CALWRAM *calram);
void LibCalKeyDsp(CALWRAM *calram);
void LibCalKeyTchWait(CALWRAM *calram,TCHSTS *tsts);
void LibCalBuf2Dat(byte *c_dat,const byte *c_buf);
void LibCalDat2Buf(byte *c_buf,const byte *c_dat);

/* Debug */
void LibPutMsgDlg(byte *format, ...);
void LibPutMsgDlg2(byte *format, ...);
void LibPutMsgDlg3(byte *format, ...);
void LibPutMsgDlg4(byte *format, ...);

/* SubEntry */
bool LibSubEntrySave(byte *name_str, byte *sub_code);
bool LibSubEntryDel(byte *name_str, byte *sub_code);
bool LibSubEntryRename(byte *old_name, byte *new_name, byte *sub_code);
bool LibSubEntrySearch(byte *name_str, byte *sub_code);
bool LibGetSubEntrySt(byte *name_str, byte sub_entry);
word LibGetSubEntNum(void);
bool LibGetAllEntry(byte *name_str, byte *mode_entry, byte *sub_entry);

/* Serial */
word LibSrlPortOpen(SRL_STAT *po);
word LibSrlPortClose(void);
word LibSrlPortFClose(void);
word LibSrlRxBufClr(void);
word LibSrlTxBufClr(void);
word LibSrlGetDteStat(word *num, word *flag);
byte LibSrl232CStat(void);
word LibSrl232COpe(byte stat);
word LibSrlRateSet(byte speed);
word LibSrlGetRBufChar(void);
word LibSrlGetTBufSpace(void);
word LibSrlSendByte(byte tmode, byte data);
word LibSrlRecvByte(byte *data);
word LibSrlPreRead(word oft, byte *data);
word LibSrlSendBreak(byte time);
word LibSrlSendBlock(byte *data, word size);
word LibSrlRecvBlock(byte *data, word size, word *num);
word LibSrlGetOpenStat(void);

/************************************************/
/*        Macro					                */
/************************************************/
#define	GSIZE(x, y)			((x) & 0x00ff), ((unsigned int)(x) >> 8), ((y) & 0x00ff), ((unsigned int)(y) >> 8)
#define CENTER(max, len)	(((int)(max)-(int)(len))>>1<0?0:((int)(max)-(int)(len))>>1)
#define LibChangeBcdVal(x)	(byte)((int)(x >> 4) * 10 + (x & 0x0f))
#define LibChangeValBcd(x)	(x==0?0:(byte)(((x / 10) << 4) | (x % 10)))
#define LibTchRelease()		while(LibTchStackPop()!=NULL)
#define LibDateClr(d)		memset(d,0x00,8)
#define LibModeIconDsp()							\
{                                                   \
	LibPutFarData(130,  0, 44);						\
	LibPutMessageCenter( 89,131,157,2,IB_PFONT1);	\
}


/************************************************/
/*		GrobalArea								*/
/************************************************/
extern word			FlashStatus;
extern byte			SubEntryStat;
extern TCHTBL far	TchHardIcon[];
extern TCHTBL far	TchActionKey[];
extern TCHTBL far	TchModeIcon[];
extern TCHTBL far	TchAllDsp[];


/************************************************/
/*               SYSTEM AREA                    */
/************************************************/
extern word	far SYS_KEY_FLG;			/* Keyboard flag                   */
extern byte	far SYS_KEY_KIND;			/* Keyboard type                   */
extern word	far	SYS_CLD_DATE;			/* Calender date                   */
extern byte	far	SYS_SCH_MODE;			/* Mode No.                        */
extern byte	far	SYS_SCH_SBMODE;			/* SubMode No.                     */
extern byte	far	SYS_SCH_SUBENTRY;		/* sub_entry                       */
extern word	far	SYS_SCH_DTPOINT;		/* DataPointer                     */
extern word	far	SYS_SCH_CTHD0;			/* Category0 Header                */
extern byte	far	SYS_SCH_CTNM0[17];		/*           Nmae                  */
extern word	far	SYS_SCH_CTHD1;			/* Category1 Header                */
extern byte	far	SYS_SCH_CTNM1[17];		/*           Name                  */
extern word	far	SYS_SCH_CTHD2;			/* Category2 Header                */
extern byte	far	SYS_SCH_CTNM2[17];		/*           Name                  */
extern byte	far	SYS_CALE_TYPE;			/* Calender Type                   */
extern byte	far	SYS_SYNC_ID1[24];		/* Certification ID open           */
extern byte	far	SYS_SYNC_ID2[24];		/*                  secret         */
extern word	far	SYS_SYNC_MODE_INF;		/* CallingModeCode                 */
extern byte	far	SYS_CAL_MEM[14];		/* Calculator Memory               */
extern byte	far	SYS_DATE_FORMAT;		/* Date display format             */
extern byte	far	SYS_TIME_FORMAT;		/* Time display format             */
extern byte	far	SYS_DST[30];			/* DST according to the country    */
extern byte	far	SYS_HOME_CITY;			/* HomeCity No.                    */
extern byte	far	SYS_WORLD_CITY;			/* WorldCity No.                   */
extern byte	far	SYS_MENU_PTR;			/* ActionMenuPointer               */
extern byte	far	SYS_TEL_CATEGORY;		/* CONTACTS CategoryType           */
extern byte	far	SYS_MEMO_CATEGORY;		/* MEMO CategoryType               */
extern byte	far	SYS_DELAY500;			/* */
extern word	far	SYS_START_DATE[3];		/* ShedulerDueDate(start)          */
extern word	far	SYS_END_DATE[3];		/* ShedulerDueDate(end)            */
extern byte	far	SYS_INP_BUF[2049];		/* Copy/Paste Buffer               */
extern byte	far	SYS_TIME_FLG;			/* Home/World TimeFlag             */
extern byte	far	SYS_MENU_MODE;			/* MenuDisplayType                 */
extern byte far SYS_PAY_TYPE[10][14+1];	/* Payment Type Table              */
extern byte far SYS_EXP_TYPE[10][14+1];	/* Expense Type Table              */
extern byte far SYS_PAY_NO;				/* Payment Type Last Setting       */
extern byte far SYS_EXP_NO;				/* Expense Type Last Setting       */
extern byte far SYS_AMT_ITEM;			/* AmountListDisplayItem LastSetting */
extern byte far SYS_PTTL_ITEM;			/* Period TotalDisplayItem LastSetting */
extern byte	far	SYS_SRCH_WORD[13];		/* SchedulerSearchKeyword          */
extern byte far SYS_SOL_LEVEL;			/* Game1 LevelInformation          */
extern dword far SYS_SOL_HI_SCORE[3];	/* Game1 HighScore                 */
extern dword far SYS_SOL_HI_TIME[3];	/* Game1 BestTime                  */
extern byte far SYS_PYR_LEVEL;			/* Game2 LevelInformation          */
extern dword far SYS_PYR_HI_SCORE[3];	/* Game2 HighScore                 */
extern dword far SYS_PYR_HI_TIME[3];	/* Game2 BestTime                  */
extern byte	far SYS_KEYBOAD_TYPE;		/* KeyboardLastValue               */
extern word	far	SYS_TEL_INP_ITEM[7];	/* CONTACTS InputItem              */
extern word	far	SYS_TEL_HIST_PTR[17];	/* CONTACTS HistoryPointer          */
extern byte	far SYS_TEL_FONT;			/* CONTACTS FontType               */
extern word	far SYS_TEL_SHIST_PTR[17];	/* CONTACTS HistoryPointer(for SECRET) */
extern byte far SYS_TEL_CENTER_POS[9];	/* CONTACTS CenterLinePosition     */
extern byte far SYS_TEL_RGT_KMK[7];		/* CONTACTS ListDisplayItem        */
extern byte far SYS_PASS_MATCH;			/* Password Match Flag             */
extern byte far SYS_PEN_INF;			/* Q-MEMO PenSize                  */
extern byte far SYS_ERA_INF;			/* Q-MEMO EraserSize               */
extern byte	far	SYS_DAILY_ALM[4];		/* DailyAlarmTime                  */
extern byte	far	SYS_CONV_MODE;			/* CONVERSION ModeFlag             */
extern byte	far	SYS_DECI_CMODE;			/* CrossConversionDecimalPointModeFlag */
extern byte	far	SYS_DECI_EMODE;			/* EuroConversionDecimalPointModeFlag   */
extern byte	far	SYS_ECONV_RATE[11][7];	/* EuroConversionRateTable         */
extern byte	far	SYS_ECONV_NAME[11][5];	/* EuroConversionUintNameTable     */
extern byte	far	SYS_ECONV_DISP[3];		/* EuroConversionSelectUnitNameTable */
extern byte	far	SYS_ECONV_SEIDO[11];	/* EuroConversionDecimalPointSettingTable */
extern byte	far	SYS_CONV_RATE[3][13];	/* CrossConversionRateTable        */
extern byte	far	SYS_CONV_NAME[3][5];	/* CrossConversionUnitNameTable    */
extern byte	far	SYS_CONV_SEIDO[3];		/* CrossConversionDecimalPointSettingTable */
extern byte	far SYS_SCH_FONT;			/* SCHEDULER FontType              */
extern byte	far SYS_MEMO_FONT;			/* MEMO    FontType                */
extern byte	far SYS_EXP_FONT;			/* EXPENSE FontType                */

extern byte	far SYS_IMENU_PAGE;			/* Page                            */
extern byte	far SYS_IMENU_MAXPAGE;		/* MaximumPage                     */
extern byte	far SYS_IMENU_INFO;			/* SYS_IMENU_LINK Initial OK?      */
extern byte	far SYS_IMENU_LINK[128];	/* MenuLineInformation             */

extern byte far SYS_TEL_CENTERPB_POS;
#endif
