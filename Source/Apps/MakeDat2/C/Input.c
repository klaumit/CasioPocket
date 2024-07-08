#include  <stdrom.h>
#include  "define.h"
#include  "libc.h"
/*
#define  OBJ_IC_SET  0x9004
#define  OBJ_IC_CLR  0x9005
*/
#define  OBJ_STRING  0x9003

/*
TCHTBL far TchHeaderIcon2[] = {
        99, 0, 127, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_SET,
        0x0000,

        71, 0, 98, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_CLR,
        0x0000,

        0, 0, 0, 0,
        ACT_NONE,
        OBJ_END,
        0x0000
};
*/

/*
T_ICON far TiconSet = { &TchHeaderIcon2[0], NULL, NULL, 0x00 };
T_ICON far TiconClr = { &TchHeaderIcon2[1], NULL, NULL, 0x00 };
*/

void InputText(char *title, char *text, int yy)
{
char mtxbf[16];
bool input_end;
int  key_cd, xx;
TXTP m_in_tp;

TCHSTS     tsts;
word       telgd[1];
TCHTBL     TchTxtBuf[3];
T_SCR_POS  ComScr;

    telgd[0] = 0xffff;
    
/*    mtxbf[0] = 0; */
    strcpy(mtxbf, text);

    LibClrBox(2, yy, 157, 10);

    xx = LibGetProStrSize(IB_CG57FONT, title);
    LibPutProStr(IB_CG57FONT, 3, yy, title, xx);
    LibPutDisp();

m_in_tp.st_x   = xx + 4;       /* Start coordinate (X) of text display */
m_in_tp.st_y   = yy;           /* Start coordinate (Y) of text display */
m_in_tp.ed_x   = 158;          /* End coordinate (X) of text display */
m_in_tp.it_y   = 9;            /* Text display line spacing (Y) */
m_in_tp.MAXGYO = 1;            /* Number of text display lines */
m_in_tp.font   = IB_PFONT1;    /* Display font type */

m_in_tp.csen   = TRUE;
m_in_tp.rtnen  = 2;
m_in_tp.maxmj  = 15;
m_in_tp.txbf   = mtxbf;
m_in_tp.gdcmt  = telgd;
m_in_tp.txtobj = OBJ_STRING;
m_in_tp.sbrobj = NULL;
m_in_tp.tchtb  = TchTxtBuf;
m_in_tp.s_pos  = &ComScr;

    LibTxtInit(&m_in_tp);        /* Initialization of text input */

    LibTchStackClr();
    LibTchStackPush(NULL);
    LibTxtTchSet(&m_in_tp);
    LibTchStackPush(TchHardIcon);
    LibTchInit();

    LibDispKey();

    input_end = FALSE;
    while(input_end == FALSE)
    {
        LibTxtDsp(&m_in_tp);
        LibPutDisp();

        key_cd = LibGetKeyM(&tsts);
        if (key_cd == KEY_NEXT)
        {
            strcpy(text, mtxbf);
            input_end = TRUE;
        }
        LibTxtInp(key_cd, &tsts, &m_in_tp);
    }
    LibTchStackClr();
}

void CalcBuf2Str(char *str, byte *cbuf)
{    /* Calculator X-buffer -->  string buffer */
int  ii, nn, tt;

    tt = 13 - cbuf[0];  
    nn = 0;
    if (cbuf[1] == 0x0C) str[nn++] = '-';  
    if (cbuf[1] == 0x04) str[nn++] = '+';

    for (ii=13; ii >= 2; ii--)
    {
       if (cbuf[ii] > 0x20) str[nn++] = cbuf[ii];
       if (ii == tt) str[nn++] = '.';
    };
    /* if (str[nn-1] == '.') nn--; */
    str[nn] = 0;
}

void InputCalc(char *title, char *text, int yy)
{
char tbuf[16];
bool input_end;
int  xx;

TCHSTS   tsts;
CALWRAM  Calram;

    /* strcpy(tbuf, text); */

    LibClrBox(2, yy, 130, 10);

    xx = LibGetProStrSize(IB_CG57FONT, title);
    LibPutProStr(IB_CG57FONT, 3, yy, title, xx);
    LibPutDisp();

    LibTchStackClr();
    LibTchStackPush(NULL);
    LibTchStackPush(TchHardIcon);
    LibTchInit();

    LibCalKeyInit(&Calram);
    LibCalKeyDsp(&Calram);
    LibPutDisp();

    input_end = FALSE;
    while(input_end == FALSE)
    {
        LibCalKeyTchWait(&Calram, &tsts); /* Waits for touch of calculator */

        switch(tsts.obj) {
        case OBJ_CAL_NEXT: {    /* "Next" */
            CalcBuf2Str(tbuf, Calram.calxbuf);
            strcpy(text, tbuf);
            input_end = TRUE;
            break;
            }
        case OBJ_CAL_EQUAL: {   /* "=" */
            CalcBuf2Str(tbuf, Calram.calxbuf);
            break;
            }
        }
        LibClrBox(xx+4, yy, 80, 10);
        LibPutProStr(IB_PFONT1, xx+4, yy, tbuf, 80);
    }
    LibTchStackClr();
}
