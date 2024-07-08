/******************************************************************************
    Copyright (C) 1998 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef EXP_DEF
#define EXP_DEF
#include    "define.h"

/************************/
/************************/
#define	LST_DATA_MAX	400		/* All */
#define	LST_BUF_MAX		64		/* ListDataLength */
#define OBJ_SCR_BAR		0xc011	/* ScrollBarObjectCode */
#define OBJ_LDATA		0xc012	/* ListDataAreaObjectCode */
#define OBJ_ICON_020	0xc013	/* Icon[20] ObjectCode */
#define OBJ_ICON_100	0xc014	/* Icon[100] ObjectCode */
#define OBJ_ICON_300	0xc015	/* Icon[300] ObjectCode */
#define SCR__X			150		/* ScrollBarPosition X */
#define SCR__Y			2		/* ScrollBarPosition Y */
#define SCR__SIZE		156		/* ScrollBarSize */
#define SCR_DSP			16		/* DispCount */

/************************/
/*    Grobal            */
/************************/
extern byte			LstBuf[SCR_DSP][LST_BUF_MAX];
extern bool			LstFlg[SCR_DSP];
extern T_SCR_POS	Scr;
extern int			CurPtr;
extern TCHTBL   far	TchList[];
extern T_ICON   far Ticon01;
extern T_ICON	far Ticon02;
extern T_ICON	far Ticon03;
extern byte far		FileData[LST_DATA_MAX][LST_BUF_MAX];
extern byte far		ICON_DATA[3][18];


/************************/
/************************/
void SmpLstMain(void);
void SmpInpMain(void);
void SmpDspBk(void);
void SmpFlushLst(void);
void SmpDspData(void);
void SmpDspCur(void);
int SmpGetCurPtr(TCHSTS tsts);
int SmpAdjCurPtr(int now_ptr);

#endif
