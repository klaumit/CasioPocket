#ifndef DEBUGH
#define DEBUGH

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>
#include <gdefs.h>
#include <fm.h>
#include <stdprocs.h>

extern TCHTBL far TchPPSS[];

void PrintProStrSize(void);
void ROMCreationDate(void);
void TestFile1(void);
void TestFile(void);

#endif