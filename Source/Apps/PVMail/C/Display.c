/******************************************************************************
*       
*       [Title]  PVMail
*            Routines for mail Display-mode
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
#include        "Graphics.h"

/*** Structures ***/

/* Display window touch areas */
static TCHTBL far TchDisplayViewIcon[] = {
        14, 0, 42, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_LIST, 0x0000,

        43, 0, 55, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_BUTT_UP, 0x0000,

        56, 0, 68, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_BUTT_DOWN, 0x0000,
                
        69, 0, 98, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_EDIT, 0x0000,

        99, 0, 130, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_REPLY, 0x0000,

        131, 0, 160, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_FORWARD, 0x0000,

        0, 0, 0, 0,
        ACT_NONE, OBJ_END, 0x0000
};

static T_ICON far TiconList     = { &TchDisplayViewIcon[0], NULL, NULL, 0x00 };
static T_ICON far TiconButtUp   = { &TchDisplayViewIcon[1], NULL, NULL, 0x00 };
static T_ICON far TiconButtDown = { &TchDisplayViewIcon[2], NULL, NULL, 0x00 };
static T_ICON far TiconEdit     = { &TchDisplayViewIcon[3], NULL, NULL, 0x00 };
static T_ICON far TiconReply    = { &TchDisplayViewIcon[4], NULL, NULL, 0x00 };
static T_ICON far TiconFwd      = { &TchDisplayViewIcon[5], NULL, NULL, 0x00 };

/* Display-window - Page-up / down (thanks to Marat Bakirov!) */
static struct TCHSTS TpageUp =
{
   OBJ_SCR_BAR, 1,
   LCD_X_DOT - (TXTSCROLLBARWIDTH / 2),
   23, 0
};
static struct TCHSTS TpageDown =
{
   OBJ_SCR_BAR, 1,
   LCD_X_DOT - (TXTSCROLLBARWIDTH / 2),
   140, 0
};

static byte   DisplayTxtItems[128];
static word xFFFE = 0xfffe;

extern TXTP   txtBox;
extern SLW_TBL    *ktb_s;
extern BK_SMPL_TCH *bk;
extern byte       *MailFileName;
extern FILE_BUF    mail_fb;
extern TPvFile     hMailFile;
extern int         CurMail; /* Mail message currently selected from list */
extern int         MailNum; /* Number of mail messages currently loaded */
extern int         SourceMail;

extern TCache      Cache;
extern TMailRecord MailRecord;

/* Display text-box variables */
extern TCHTBL     *TchTxtBuf;  /* Text touch area table */
extern T_SCR_POS   ComScr;     /* Scroll bar position information */
                /* Structure for text display - preview */
extern byte *keywd_0;
extern byte *keywd_1;
extern byte *keywd_2;


/************ Function prototypes *************/
/* Main menu */
extern void GmPdwnSet(void);
extern bool GmPdwnFnc();
extern bool PdwnFncSys(word sel);
extern bool PdwnFncEdt(word sel);

extern void PVMailTitle(void);

extern void TxtItemIconDsp(TXTP *txt);
extern bool NewMessage(int new_type);
extern void EditLoop(void);
extern void LoadMail(int idx);

/**********************************************************
*  [Title] InitTxtDisplay
*  [Descr] Setup initial values for the preview txt box
**********************************************************/
static void InitTxtDisplay(void)
{

        txtBox.st_x = TXTDISPLAY_XLEFT; /* Start (X) of text display */
        txtBox.st_y = TXTDISPLAY_YTOP;  /* Start  (Y) of text display */
        txtBox.ed_x = LCD_X_DOT - TXTSCROLLBARWIDTH ; /* End (X) of text display */
        txtBox.it_y = TXTDISPLAY_ROWHEIGHT; /* Text display line spacing (Y) */
        txtBox.MAXGYO = TXTDISPLAY_ROWNUM;  /* Number of text display lines */
        txtBox.font = IB_PFONT1;      /* Display font type */
        txtBox.csen = FALSE;          /* Cursor disabled */
        txtBox.rtnen = FALSE;         /* CR code display disabled */
        txtBox.maxmj = MAIL_DATA_SIZE - 1; /* Maximum number of allowable characters */
        txtBox.txtobj = OBJ_TXTTCH;  /* Object when text area is touched */
        txtBox.sbrobj = OBJ_SCR_BAR; /* Object when the scroll bar is touched */
        txtBox.tchtb = TchTxtBuf;    /* Pointer for text scroll bar area */
        txtBox.s_pos = &ComScr;      /* Pointer for text and scroll bar information */
        txtBox.gdcmt = &xFFFE;        /* User-defined guidance comment table */
    txtBox.gdcmt2 = TXT_GUIDE;    /* Guidance comment strings */
        
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
    LibClrBox(0, TXTDISPLAY_YTOP - 2,  
        LCD_X_DOT, TXTDISPLAY_YHEIGHT + 3);
    LibBox(0, TXTDISPLAY_YTOP - 2,
        LCD_X_DOT, TXTDISPLAY_YHEIGHT + 3, 1);
        LibPutDisp();
        LibLine(LCD_X_DOT - TXTSCROLLBARWIDTH + 1 , TXTDISPLAY_YTOP - 1,
            1, TXTDISPLAY_YHEIGHT + 2, 1);

    /* (Re)-initialization of the text-box data */
        LibTxtDspInit(&txtBox);
    if(LibTxtDspC(&txtBox) == TRUE)
        LibPutDisp();   
}

/******************************************************************************
*       [Title]         DisplayInit
*               Init Display-mode screen
******************************************************************************/
static void DisplayInit(void){
    
        LibClrDisp();
        
        /*PVMailTitle();*/
    LibPutGraph(0,0,SIcon); /* message icon */
    
        LibPutGraph(14,0, FourButtons);/* Four Buttons */
        /*LibPutFarData(71,0,149); /* Three Buttons */
        
        LibPutProStr(IB_PFONT1,20,2,"List",30);   /* x0 = 16 */
    /* Up-Down buttons x0 = 45, 58 */
        LibPutProStr(IB_PFONT1,75,2,"Edit",30);   /* x0 = 71 */ 
        LibPutProStr(IB_PFONT1,103,2,"Reply",30); /* x0 = 101 */
        LibPutProStr(IB_PFONT1,136,2,"Fwd",30);   /* x0 = 133 */
        
        /* Init mail text display-box */
        InitTxtDisplay();
        LoadMail(CurMail);
        DrawtxtBox(Cache.Data);
        TxtItemIconDsp(&txtBox);
        LibPutDisp();

}

/******************************************************************************
*       [Title]         DisplayTouchSet
*   [Descr]     Init touch info
******************************************************************************/
static void DisplayTouchSet(void){

        /* Touch init */
        LibTchStackClr();
        LibTchStackPush( NULL );
        LibTxtTchSet(&txtBox);
        LibTchStackPush( TchHardIcon );
    LibTchStackPush( TchActionKey );
        LibTchStackPush( TchDisplayViewIcon );

}

/******************************************************************************
*  [Title]       ScrollMessageUp
*  [Desc]        Scrolls one message up
*******************************************************************************/
void ScrollMessageUp()
{
    if (CurMail > 0)
    {
        CurMail--;              
        LoadMail(CurMail);
        /* Update message flag (READ) */
        if ((MAIL_INBOX == *(Cache.MailBox)) 
                    && (*(Cache.New) == TRUE))
        {
            *(Cache.New) = FALSE;
            if (!saveMail(Cache.fp))
                fatalError("Display_Save");
        }
        DisplayInit();                  
        DisplayTouchSet();
    }
}

/******************************************************************************
*  [Title]       ScrollMessageDown
*  [Desc]        Scrolls one message down
*******************************************************************************/
void ScrollMessageDown()
{
    if (CurMail < MailNum - 1)
    {
        CurMail++;              
        LoadMail(CurMail);
            /* Update message flag (READ) */
        if ((MAIL_INBOX == *(Cache.MailBox)) 
                && (*(Cache.New) == TRUE))
        {
            *(Cache.New) = FALSE;                   
            if (!saveMail(Cache.fp))
                fatalError("Display_Save");
        }
        DisplayInit();                              
        DisplayTouchSet();
    }
}

/******************************************************************************
*  [Title]       DisplayLoop
*  [Desc]        Main loop for 'mail display' mode
*******************************************************************************/
void DisplayLoop(void) {
    TCHSTS      tsts;

    /* Update memory buffer with current message */
    LoadMail(CurMail);
    
    /* Update message flag (READ) */
    if ((MAIL_INBOX == *(Cache.MailBox)) && (*(Cache.New) == TRUE)){
        *(Cache.New) = FALSE;
        if (!saveMail(Cache.fp))
            fatalError("Display_Save");
        }
        
    /* Clear screen and init objects for the 'list' view */
    DisplayInit();
    DisplayTouchSet();

        for(;;){

        /* Text-box update */
            if(LibTxtDspC(&txtBox) == TRUE){
                TxtItemIconDsp(&txtBox);
                LibPutDisp();
            }       
                  
            /* Touch-screen input */
                LibTchWait( &tsts );
                switch( tsts.obj ) {
                
                /** PUSH-BUTTONS & ACTION-KEY **/
                        case  OBJ_IC_LIST:
                                if(LibBlockIconClick( &TiconList, &tsts , B_ICON_LEFT ) == TRUE)
                                        /* Go back to list-view mode */
                                        return;
                                break;

                    case OBJ_LPSW_UP:
                     /*Marat Bakirov*/
                     if (ComScr.pos>0)
                     {               
                        tsts = TpageUp;
                     }
                     else
                     {
                        if (CurMail > 0)
                        {
                            ScrollMessageUp();
                            /* now moving to the end of message  - added by Marat Bakirov*/
                            /*i put difference 2*ComScr.dsp because one more click is needed*/
                            ComScr.pos = ComScr.vol - 2*ComScr.dsp;
                            if (ComScr.pos <0)
                            {
                                ComScr.pos = 0;
                            }
                            tsts = TpageDown;
                        }
                     }
                    break;
                     case  OBJ_IC_BUTT_UP:
                       if(LibBlockIconClick( &TiconButtUp, &tsts , B_ICON_CENTER ) == TRUE)
                        {  
                            ScrollMessageUp();
                        }
                        break;
                                
                    case OBJ_LPSW_DOWN:
                     /* Marat Bakirov */
                     if (ComScr.pos+ComScr.dsp<ComScr.vol)
                     {               
                         tsts = TpageDown;
                     }
                     else
                     {
                        ScrollMessageDown();;
                     }
                     break;
                     case  OBJ_IC_BUTT_DOWN:
                        if(LibBlockIconClick( &TiconButtDown, &tsts , B_ICON_CENTER ) == TRUE)
                        {
                            ScrollMessageDown();
                        }
                        break;

                        case  OBJ_IC_EDIT:
                                if(LibBlockIconClick( &TiconEdit, &tsts , B_ICON_CENTER ) == TRUE){
                                        /* Edit current mail */
                                    if ( (*(Cache.MailBox) == MAIL_OUTBOX) && *(Cache.New))
                                    {
                                            EditLoop();
                                            /* Go back to list-view mode */
                                            return;
                                    }
                        }
                        break;
                                                
                        case  OBJ_IC_REPLY:
                                if(LibBlockIconClick( &TiconReply, &tsts , B_ICON_CENTER) == TRUE){
                                    if (MailNum < MAX_MAIL - 1)
                                            /* Reply to current mail */
                        if (NewMessage(NEW_REPLY))
                                    EditLoop();
                                    /* Go back to list-view mode */
                                        return;
                                }
                            break;      
                            
                        case  OBJ_IC_FORWARD:
                                if(LibBlockIconClick( &TiconFwd, &tsts , B_ICON_RIGHT) == TRUE){
                                    if (MailNum < MAX_MAIL - 1)
                                            /* Forward current mail */
                        if (NewMessage(NEW_FORWARD))
                                    EditLoop();
                                        /* Go back to list-view mode */
                                        return;
                        }
                        break;
                
                        

                /** ACTION KEY; ESCAPE**/
                    case OBJ_HIC_ESC:
                        case OBJ_LPSW_PUSH:
                            return;
                            
                /** PULL-DOWN MENU-BAR (use function from PVMail.c) **/
                        case OBJ_HIC_MBAR:
                                GmPdwnSet();
                                if( GmPdwnFnc() == TRUE ){
                                /*
                                        DisplayInit();
                                        LibTchInit();
                                */
                                        return;
                                };
                                break;

                    default:
                            break;
                }
                
                /*Text-box touch processing */
        LibTxtDspS(&txtBox,&tsts);

                if(tsts.act == ACT_ALM)
                        LibAlarm();     /* AlarmProc */
        }

}

