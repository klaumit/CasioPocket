/******************************************************************************
    Copyright (C) 1998 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef TRM_H
#define TRM_H
#include    "define.h"


#define TRMCOLS 21
#define TRMLINES 50

#define M_LONG 1
#define M_SHORT 2

#define LINEDIST 9
#define DLINES (160/9)
#define CHARWIDTH 7
#define LINECHARS TRMCOLS

#define OBJ_IC_UP   0x8107
#define OBJ_IC_DOWN 0x8108


void GotoTrm(byte col,byte row);
void InitTrm(void);
void InputTrm(unsigned char *ib,int cnr);
void OutTrm(unsigned char c);
void OutputTrm(unsigned char *ob);
void OutputITrm(int i);
void fatalError(char *msg);
#endif
