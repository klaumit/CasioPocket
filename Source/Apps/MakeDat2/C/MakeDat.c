/******************************************************************************
        [Title]         Program for Create Data File   
******************************************************************************/
#include  <stdrom.h>
#include  "define.h"
#include  "libc.h"
#include  "input.h"
#include  "stdio.h"
#include  "l_define.h"
#include  "l_libc.h"

/****************************************************************************/

FILE_INFO fi;
FILE_BUF fb;

char ProgTitle[] = "Make data file";   /* Program name */

void main()
{
    char fname[16] = "XX-DATA";
    char flen[16]  = "1000";
    char code[16]  = "MBKC";
    long len;
    int  i, j;

    LibClrDisp();
    LibPutProStr(IB_PFONT3, 20, 1, ProgTitle, 140);
    LibBox(0, 15, 160, 73, 1);

        InputText("File name:", fname, 20);

        for (len = 0; len < 1; ) {
            InputCalc("File length:", flen, 36);
            len = atol(flen);             /*  переведем в целое число  */
//            sscanf(flen, "%ld", &len);    
        }
        ltoa(len, flen, 10);              /*  напечатаем число */
        LibPutProStr(IB_PFONT2, 120, 36, flen, 39);

        InputText("Fill code:", code, 52);
        LibPutProStr(IB_PFONT2, 120, 52, code, 39);

        LibPutProStr(IB_PFONT1, 4, 68, "writing file ", 60);
        LibPutProStr(IB_PFONT2, 60, 68, fname, 60);
        LibPutFarData(132, 62, 28);     /* write data icon */
        LibPutDisp();

        /*  Здесь начинается создание файла данных  */
        if (!LibGetAllEntry(fname,
                          &(fb.fsb_main_entry_), &(fb.fsb_sub_entry_)))
        {
            LibSubEntrySave(fname, &(fb.fsb_sub_entry_));
            fb.fsb_main_entry_ = 0xA0;
        }

        fi.fp = 0xFFFF;
        fi.kind = FILE_KIND_BIN;
        fb.fbuf.bin.char_num = 0xC00;
        fb.fsb_scrt_info_  = 0x80;

        for (i=0; i < (len+0xBFF)/0xC00; i++)
        {
            LibFileFindNext(&fb, &fi, 0);
            fi.kind = FILE_KIND_BIN;    /* binary file */

            for (j=0; j < 0xC00/4; j++)
            {
              *(long *)&fb.fbuf.bin.bin_buf[j*4] = *(long *)&code;
            }

            if(LibFileWrite(&fb, &fi) == FALSE)
            LibErrorDisp(FlashStatus);
        }

        fi.fp = 0xFFFF;
        fi.kind = FILE_KIND_BIN;
        fb.fsb_scrt_info_  = 0x80;

        LibFileFindNext(&fb, &fi, 0);
        fi.kind = FILE_KIND_BIN;    /* binary file */
        LibFileReadEx(&fb, &fi, 0xC00/FILE_BLOCK_SIZE+1);

//    InputCalc("Exit:", flen, 77);
    LibJumpMenu();
}

