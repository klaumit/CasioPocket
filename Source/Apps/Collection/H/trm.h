#ifndef TRM_H
#define TRM_H
#include "define.h"

#define TRMCOLS 22
#define TRMLINES 17
/* Return-Codes from Input */
#define TRM_ENTER 0
#define TRM_NEXT 1
#define TRM_ESC 2

void GotoTrm(byte col,byte row);
void InfTrm(byte *col,byte *row);
void InitTrm(void);
int InputTrm(unsigned char *ib,int cnr);
void OutTrm(unsigned char c);
void OutputTrm(unsigned char *ob);
void OutputITrm(int i);
void OutputFTrm(float i);
#endif

