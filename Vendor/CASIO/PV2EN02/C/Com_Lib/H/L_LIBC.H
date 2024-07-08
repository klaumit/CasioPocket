/******************************************************************************
	[TITLE]
		HEADER FILE for LIBRARY

	[OUTLINE]
		DEFINE to make out a LIBRARY

	Copyright (C) 1999 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef L_LIB
#define L_LIB
#include    "l_define.h"


/************************************************/
/*               TYPE DEFINE                    */
/************************************************/
typedef void *va_list;


/************************************************/
/*       LIBRARY PROTOTYPE DECLARATION           */
/************************************************/
extern int _prf(const char *, va_list, int (*)(char));
void LibSKeyRev(TCHTBL far *sk_tbl,word obj_cd,byte sw);
bool LibSKeyIsCd(TCHTBL far *sk_tbl,word obj_cd);
void LibSKeyRevSub(int x, int y, int x2, int y2, byte rev,byte type);
void LibPutBoxSub(int x,int y,int x2,int y2,byte type);
void LibCngeBoxSub(int x,int y,int x2,int y2,byte type);
void LibPutDotSub(int x,int y,byte type);
void LibPutLineSub(int x,int y,int x2,int y2,byte type);
byte LibGetDate(byte *yearh, byte *yearl, byte *month, byte *day);
void LibGetTime(byte *hour, byte *minute, byte *second);
byte LibGetDate2(word *year2, byte *month2, byte *day2);
void LibGetTime2(byte *hour2, byte *minute2, byte *second2);
void LibAdjustTimeDeff2(int lag, word *year2, byte *month2, byte *day2, byte *dweek2,byte *hour2, byte *minute2, byte *second2);
void LibSetDateTime2(word year2, byte month2, byte day2, byte hour2, byte minute2);
void LibSetDate2(word year2, byte month2, byte day2);
void LibSetTime2(byte hour2, byte minute2);
int LibDebugPut(char ch);
void LibInitAlarmFlg(void);
void LibInitAlarm(void);
bool LibInitAlarmFlgCheck(void);
bool LibNextAlarmSet(ALMAPL *almap, char type);
bool LibSetDailyAlarm(char *tbp);
bool LibGetAlarmInfo(FILE_BUF *afd, FILE_INFO *afi);
bool LibGetAlarmFlg(void);
void LibGetDailyAlarm(char *tbp);
void LibGetNextAlm(ALMAPL *almap, byte ifdel);
int LibDrawPrmCall(byte func_no,void *draw_prm,byte al);
void LibKeyWordSrchSub(byte sw,byte *srch_str,byte *key_str);
void LibTchWaitSub(TCHSTS *tsts);
bool LibIconClickSub(const T_ICON far *icon, const TCHSTS *tsts,byte opt);
void LibELHandle(byte mode);
byte LibGetEL(void);
byte LibGetLang(void);
void LibSetLang(byte lang);
byte LibSoundGet(void);
void LibSoundSet(byte opt);
bool LibContrastInit(void);
bool LibContrastUp(void);
bool LibContrastDown(void);
byte LibDigitizer(word *obj);
void LibPassClr(void);
void LibPassSet(byte far *pas_buf);
int LibPassGet(byte far *pass_buf);
bool LibPassChk(byte far *pass_buf);
void LibCurErase(void);
word LibGetAPOTime(void);
void LibSetAPOTime(word msec);
void LibSetKeyKind(byte type);
byte LibGetKeyKind(void);
byte LibGetMessCnt(int mes_no);
void LibDspWinMessage(byte g_no,int mes_no,byte b_cnt,byte b_type);
void LibGetWinMessSize(int mes_no,byte w_pos,byte b_cnt,int *y,int *y_size);
void LibSecretCall(word m_seg,word m_ofs);
int DstCheck(byte smno);
void LibBkSampleInitSub(BK_SMPL_TCH far *t_tbl,byte b_smp);
void LibBuzzerOff(void);
void LibBuzzerOn(byte b_type);
byte LibGetLangInf(void);
dword LibGetModelType(void);	/* ZX481/482/488 */
byte LibAlarmBuzzSet(byte b_type);
void LibGetAlarmObj(TCHTBL *t_tbl);
void LibTchHardIcon(TCHSTS *tsts,byte opt);
word LibGetDLAllNum(byte part);
void LibGetUserMode(word *mode_code, word *status, byte condition);
bool LibGetProgramName(byte *name_str, byte main_code, byte sub_code);
bool LibGetModeVer(byte *ver_str, byte main_code, byte sub_code);
bool LibGetLibVer(byte *ver_str, byte main_code, byte sub_code);
bool LibGetMenuIcon(byte far **graph_addr, byte main_code, byte sub_code);
bool LibGetListIcon(byte far **graph_addr, byte main_code, byte sub_code);
bool LibCheckPMode(byte main_code, byte sub_code, word status);
word LibExeAddin(void);


/************************************************/
/*        MACRO DEFINE LIBRARY                  */
/************************************************/
#define LibFileDateCheck(d)	(far_strncmp(d,_inv_d[0],8)>0 && far_strncmp(d,_inv_d[1],8)<0)


/************************************************/
/*              AREA DEFINE                     */
/************************************************/
extern TCHSTS		scr_tch_bak;
extern int			scr_ret_bak;
extern int			scr_sts;
extern byte			Gobuf[BUFMAX];
extern word			Goidx;
extern ALMBIOS		almbdt;
extern ALMAPL		almif;
extern byte			KeyLang;
extern byte			KeyCdSts;
extern int far		Cityno[];
extern byte			PreKeyBoad;
extern byte			PassStatus;
extern byte 		_inv_d[][8];

#endif
