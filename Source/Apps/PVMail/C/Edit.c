/******************************************************************************
*       
*       [Title]  PVMail
*            Routines for mail Edit-mode
*
    Copyright (C) 2001 Fabio Fumi
        f_fumi@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/

#include    <stdrom.h>
#include        "define.h"
#include        "libc.h"
#include        "L_libc.h"

#include        "Common.h"
#include        "Flash.h"
#include        "PVMail.h"


/*** Structures ***/

/* Main window touch areas */
static TCHTBL far TchEditViewIcon[] = {
        103, 0, 130, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_CANCEL,
        0x0000,

        131, 0, 159, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_SAVE,
        0x0000,

        0, 0, 0, 0,
        ACT_NONE,
        OBJ_END,
        0x0000
};

static T_ICON far TiconCanc  = { &TchEditViewIcon[0], NULL, NULL, 0x00 };
static T_ICON far TiconSave = { &TchEditViewIcon[1], NULL, NULL, 0x00 };

static word xFFFE = 0xfffe;

extern TXTP        txtBox;
extern SLW_TBL    *ktb_s;
extern BK_SMPL_TCH *bk;
extern byte       *MailFileName;
extern FILE_BUF    mail_fb;
extern TPvFile     hMailFile;
extern int         CurMail;     /* Mail message currently selected from list */
extern int         MailNum; /* Number of mail messages loaded in the list */
extern int         SourceMail;

extern TCache      Cache;
extern TMailRecord MailRecord;

extern TCHTBL     *TchTxtBuf;  /* Text touch area table */
extern T_SCR_POS   ComScr;     /* Scroll bar position information */
extern byte *keywd_0;
extern byte *keywd_1;
extern byte *keywd_2;
    
/************ Function prototypes *************/
extern void GmPdwnSet_Edit(void);
extern bool GmPdwnFnc();
extern bool PdwnFncSys(word sel);
extern bool PdwnFncEdt(word sel);
extern void SplitItems(byte *m, TMailMessageItems *mi);
extern void PVMailTitle(void);
extern void TxtItemIconDsp(TXTP *txt);

/**********************************************************
*  [Title] InitTxtEdit
*  [Descr] Setup initial values for the preview txt box
**********************************************************/
static void InitTxtEdit(void)
{
        
        txtBox.st_x = TXTEDIT_XLEFT; /* Start (X) of text display */
        txtBox.st_y = TXTEDIT_YTOP;  /* Start  (Y) of text display */
        txtBox.ed_x = LCD_X_DOT - TXTSCROLLBARWIDTH ; /* End (X) of text display */
        txtBox.it_y = TXTEDIT_ROWHEIGHT; /* Text display line spacing (Y) */
        txtBox.MAXGYO = TXTEDIT_ROWNUM;  /* Number of text display lines */
        txtBox.font = IB_PFONT1;      /* Font type */
        txtBox.csen = 0x01;          /* Cursor enabled */
        txtBox.rtnen = TRUE;         /* CR code display enabled */
        txtBox.maxmj = MAIL_DATA_SIZE - 1; /* Maximum number of allowable characters */
        txtBox.txtobj = OBJ_TXTTCH;  /* Object when text area is touched */
        txtBox.sbrobj = OBJ_SCR_BAR; /* Object when the scroll bar is touched */
        txtBox.tchtb = TchTxtBuf;    /* Pointer for text scroll bar area */
        txtBox.s_pos = &ComScr;      /* Pointer for text and scroll bar information */
        txtBox.kwb_0 = keywd_0;      /* Keyword saving buffer 0 */
        txtBox.kwb_1 = keywd_1;      /* Keyword saving buffer 1 */
        txtBox.kwb_2 = keywd_2;      /* Keyword saving buffer 2 */
        txtBox.gdcmt = &xFFFE;       /* User-defined guidance comment table  */
    txtBox.gdcmt2 = TXT_GUIDE;   /* Guidance comment strings */

}

/**********************************************************
*  [Title] DrawtxtBox
*  [Descr] Show a display-only txt box based on 'data'
**********************************************************/
static void DrawtxtBox(byte * data)
{

    /* Set the text buffer pointer to actual data string */
        txtBox.txbf = data;
        
    /* Draw text box and scroll bar frames */
    LibClrBox(0, TXTEDIT_YTOP - 2,  
        LCD_X_DOT, TXTEDIT_YHEIGHT + 3);
    LibBox(0, TXTEDIT_YTOP - 2,
        LCD_X_DOT, TXTEDIT_YHEIGHT + 3, 1);
        LibPutDisp();
        LibLine(LCD_X_DOT - TXTSCROLLBARWIDTH + 1 , TXTEDIT_YTOP - 1,
            1, TXTEDIT_YHEIGHT + 2, 1);

    /* (Re)-initialization of the text-box data */
        LibTxtInit(&txtBox);
    if(LibTxtDsp(&txtBox) == TRUE)
        LibPutDisp();   
}

/******************************************************************************
*       [Title]         EditInit
*               Init Display-mode screen
******************************************************************************/
static void EditInit(void){

        LibClrDisp();
        PVMailTitle();

        /* Buttons */
        LibPutFarData(103,0,8); /* Two buttons */

        LibPutProStr(IB_PFONT1,107,2,"Canc",30);        
        LibPutProStr(IB_PFONT1,134,2,"Save",30);
        
        /* Init and show mail text-box and keyboard */
        InitTxtEdit();
        DrawtxtBox(Cache.Data);
        TxtItemIconDsp(&txtBox);
        LibKeyInit(); /* Initializes keyboard */
    LibDispKey(); /* Keyboard display */ 
        LibPutDisp();

}

/******************************************************************************
*       [Title]         EditTouchSet
*   [Descr]     Init touch info
******************************************************************************/
static void EditTouchSet(void){

        /* Touch init */
        LibTchStackClr();
        LibTchStackPush( NULL );
        LibTxtTchSet(&txtBox);
        LibTchStackPush( TchHardIcon );
    LibTchStackPush( TchActionKey );
        LibTchStackPush( TchEditViewIcon );

}


/******************************************************************************
*  [Title]       DisplayLoop
*  [Desc]        Main loop for 'mail display' mode
*******************************************************************************/
void EditLoop(void) {
    TCHSTS      tsts;
    byte    keycd;
    bool    done;
    
    /* Clear screen and init objects for the 'list' view */
    EditInit();
    EditTouchSet();

        for(;;){

        /* Text-box update */
                if(LibTxtDsp(&txtBox) == TRUE){
                    TxtItemIconDsp(&txtBox);
            LibPutDisp();
            }
                
        keycd = LibGetKeyM(&tsts); /* Waiting for software key */
        
/* bmf & fatty - russian keyboard switching */
#ifdef RUS_KEYBOARD
        if (keycd==KEY_NEXT) {
          SYS_KEY_KIND = SYS_KEY_KIND==SKEY_COM1_S?SKEY_COM1_L:SKEY_COM1_S;
          LibDispKey();
        } else 
#endif /* #defined  RUS_KEYBOARD */

        if(keycd==KEY_NONE){
                    switch( tsts.obj ) {
                
                /** BUTTONS **/
                        case  OBJ_IC_CANCEL:
                                if(LibBlockIconClick( &TiconCanc, &tsts , B_ICON_LEFT ) == TRUE)
                                        /* Go back to list-view mode and do nothing */
                                        return;
                                break;

                        case  OBJ_IC_SAVE:
                                if(LibBlockIconClick( &TiconSave, &tsts , B_ICON_RIGHT ) == TRUE){
                    /* CHECK NOT WORKING YET
                          if (NOT EMPTY 'To' OR 'Body'){ 
                        */
                        /* Save edited mail message (if not empty) */
                        /* Please wait! */
                        LibDspWinMessage(24,381,0,IB_MWIN_NONE | IX_MWIN_CENTER);
                        LibPutDisp();
                        CurMail = MailNum;
                        *(Cache.DataSize) = strlen(Cache.Data);
/*                    if (!saveMail(0xffff)) -- changed by BMF */
                        if (!saveMail(Cache.fp)) {
                           fatalError("Edit_saveMail");
                        } else {
                           /* Data stored! */
                           LibDspWinMessage(46,341,0,IB_MWIN_NONE | IX_MWIN_CENTER);
                           LibPutDisp();
                    }
                    return;
                     /* CHECK NOT WORKING YET
                          } else {
                                /^ Input all@required data! ^/
                                    LibWinIcnMsg(ICON_BIKKURI, 326, 0x01);        
                              }
                         */
                        }
                        break;

                /** ACTION KEY & ESCAPE**/
                /*
                    case OBJ_LPSW_UP:
                    case OBJ_LPSW_DOWN:
                */
                    case OBJ_HIC_ESC:
                        case OBJ_LPSW_PUSH:
                            return;
                            
                /** PULL-DOWN MENU-BAR (usa le funzioni in PVMail.c) **/
                        case OBJ_HIC_MBAR:
                                GmPdwnSet_Edit();
                                if( GmPdwnFnc() == TRUE ){
                                        EditInit();
                                        LibTchInit();
                                        return;
                                };
                                break;

                    default:
                            break;
                    }
                }
                
                /* Text-box touch processing */
        LibTxtInp(keycd,&tsts,&txtBox);
                
                /* AlarmProc */
                if(tsts.act == ACT_ALM)
                        LibAlarm();     }

}

