
/******************************************************************************
*       
*       [Title]     PV Mail Program
*       [Model]         PocketViewer
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
#include        "Graphics.h"
#include        "Flash.h"
#include        "ListBox.h"
#include        "PVMail.h"
#include        "Serial.h"

/* Menu-bar items (see also Message.txt document) */
word    MPdEdt[] = {
            20,         /* Cut */
                21,         /* Copy */
                    22,         /* Paste */
                    16,         /* Search */
                    23,         /* Delete */
                    /* ... more items to add ... */
                        PDNTBLEND,
                };

word    MPdSys[] = {
                        38,                     /* Set_date/time                */
                        33,                     /* Sound                                */
                        39,                     /* Format                               */
                        34,                     /* Capacity                             */
                        35,                     /* Memory_management    */
/*                      36,                     /* Language                             */
                        37,                     /* Contrast                             */
                        40,                     /* Touch_Panel_Alignment*/
/*                      41,                     /* Data_communication   */
                        PDNTBLEND,
                };

word    MPdOpt[] = {
                        PDNTBLEND,
                };

SLW_TBL ktb_s[3] = {{312,0},{313,1},{314,2}};


/******** Main window (list-view) touch areas ************/
static TCHTBL far TchListViewIcon[] = {
        75, 0, 103, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_VIEW, 0x0000,

        104, 0, 131, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_NEW, 0x0000,

        132, 0, 160, 11,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_IC_SYNC, 0x0000,

        0, 0, 0, 0,
        ACT_NONE, OBJ_END, 0x0000
};
static T_ICON far TiconView = { &TchListViewIcon[0], NULL, NULL, 0x00 };
static T_ICON far TiconNew  = { &TchListViewIcon[1], NULL, NULL, 0x00 };
static T_ICON far TiconSync = { &TchListViewIcon[2], NULL, NULL, 0x00 };

/* Mail-list tabs */
static TCHTBL far TchListTabs[] = {
        6, 72, 27, 78,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_TAB_INBOX, 0x0000,

        35, 72, 63, 78,
        ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
        OBJ_TAB_OUTBOX, 0x0000,
        
        0, 0, 0, 0,
        ACT_NONE, OBJ_END, 0x0000
};
static T_ICON far TtabInbox  = { &TchListTabs[0], NULL, NULL, 0x00 };
static T_ICON far TtabOutbox = { &TchListTabs[1], NULL, NULL, 0x00 };

/* List-Box touch areas */
static TCHTBL far tchList[] = {
        TCHTBL_LB(LB_X,LB_Y,LB_W,LB_IH,LB_IPP,LB_LT,OBJ_LB),
        TCHTBL_END
};



/*************** Global Variables ************************/
long        SerialError = 0;   /* Global var to keep the error reason code */
BK_SMPL_TCH bk[2];             /* ESC touch icon */

byte        ProgTitle[] = "PVMail";   /* Program name */
byte        MailFileName[] = MAIL_FILENAME;
FILE_BUF    mail_fb;
TPvFile     hMailFile;

word    far RecListFP[MAX_MAIL];      /* Array of Flash pointers to mail record  */
int         CurMail = 0;              /* Mail message currently selected from list */
int         MailNum = 0;              /* Number of mail messages loaded in the list */
int         SourceMail;
byte        MailBox = MAIL_INBOX;     /* Current mail-box (inbox / outbox) */

bool        bSort = FALSE;            /* TRUE if sorting is wanted in collectMail */

byte        MailTxtItems[128];

TCache      Cache;                    /* pointers to fields of the currently selected mail */
TMailRecord MailRecord;

byte        SearchMsg[MAIL_SEARCH_SIZE]; /* Search buffer */
                                    
/* List box memory structure */
static TListBox lb;    
static word xFFFE = 0xfffe;

/* Text box variables */
TCHTBL      TchTxtBuf[3];     /* Text touch area table */
T_SCR_POS   ComScr;           /* Scroll bar position information */
TXTP txtBox;                  /* Structure for text display - preview */
byte keywd_0[33];
byte keywd_1[33];
byte keywd_2[33];

/* Drop-down list */
static SLW2_ST slw;
static byte slw_cmnt[10*(14+1)];

/* Used by the 'Search' function */
int SearchMode = SRCH_ALL;

 
/******** Local function prototypes ************/
static void DspFileOpenErr(void);

/* Main menu */
void GmPdwnSet(void);
bool GmPdwnFnc();
bool PdwnFncSys(word sel);
bool PdwnFncEdt(word sel);

int  FileSend(void);
int  FileReceive(void);
void PVMailLoad(void);
void PVMailTitle(void);
void PVMailAppInit(void);
void PVMailTouchSet(void);
void TxtItemIconDsp(TXTP *txt);
bool NewMessage(int new_type);

static void drawPreview();

static bool initFile(void);
static int  LoadList(void);
static void DrawList(void);
static void InitTxtPreview(void);
static void DrawTxtPreview(byte * data);
/*commented by Marat Bakirov*/
/* static void FillFolders(void); */ 

extern void DisplayLoop(void); /* Open the full-screen message-display window */
extern void EditLoop(void);    /* Open the full-screen message-edit window */
extern int SearchLoop(void);   /* Enter a search string and perform search */
extern void InitSearchBuffer(void); /* Reset the text buffer used by search function */


/******************************************************************************
*  [Title]                      PVMail (MAIN)
*  [Arguments]          None
*  [ReturnVAL]          None
*******************************************************************************/
void far main(void) {
    TCHSTS      tsts;
    bool    temp;
    byte    buf[5];
    int     ret  = 0;

    /* Load messages, and init screen */
    PVMailLoad();
    PVMailAppInit();
    InitSearchBuffer();

        for(;;){
            
            /* Text-box update */
            if(LibTxtDspC(&txtBox) == TRUE) {
                TxtItemIconDsp(&txtBox);
            LibPutDisp();
            }

            /* Touch-screen input */
                LibTchWait( &tsts );
                switch( tsts.obj ) {
                
                /** BUTTONS **/
                        case  OBJ_IC_SYNC:
                                if(LibBlockIconClick( &TiconSync, &tsts , B_ICON_RIGHT ) == TRUE){
                                        /* Sync mail messages, and reload all */
                                        ret = SyncMail();
                                        PVMailLoad();
                    PVMailAppInit();
                                    
                                }
                            break;

                        case  OBJ_IC_NEW:
                                if(LibBlockIconClick( &TiconNew, &tsts , B_ICON_CENTER ) == TRUE){
                                    if (MailNum < MAX_MAIL - 1)
                                            if (NewMessage(NEW_EMPTY)){
                                                /* Enter edit mode (save procedure inside it) */
                                        EditLoop();
                                        /* Redraw the list mode screen */
                                        PVMailAppInit();
                                }
                                }
                            break;
                            
                        case  OBJ_IC_VIEW:
                                if(LibBlockIconClick( &TiconView, &tsts , B_ICON_LEFT ) == TRUE){
                                if (MailNum){
                                        /* Enter full-screen display mode */
                                        DisplayLoop();
                                    /* Redraw the list mode screen */
                                    PVMailAppInit();
                        LibTchInit();
                            }
                                }
                            break;
                
                /** LIST-TABS **/
                        case  OBJ_TAB_INBOX:
                                if(LibIconClick2( &TtabInbox, &tsts ) == TRUE){
                   if (MailBox != MAIL_INBOX) {
                               MailBox = MAIL_INBOX;
                               PVMailLoad();
                       PVMailAppInit();
                           }
                                }
                            break;
                        
                        case  OBJ_TAB_OUTBOX:
                                if(LibIconClick2( &TtabOutbox, &tsts ) == TRUE){
                   if (MailBox != MAIL_OUTBOX) {
                               MailBox = MAIL_OUTBOX;
                               PVMailLoad();
                       PVMailAppInit();
                           }
                                }
                            break;      
                
                /** LIST-BOX Events **/
            case OBJ_LB:
                        temp = LBClicked(&tsts,&lb);
                        if (temp == LB_UP || temp == LB_DOWN || temp == LB_MOVE) {
                    CurMail = getLBCurIdx(&lb);
                            LoadMail(CurMail);
                            DrawTxtPreview(Cache.Data);
                        } else if (temp == LB_SELECT) {
                            /* Enter full-screen display mode */
                                DisplayLoop();
                    /* Redraw the list mode screen */
                                    PVMailAppInit();
                    LibTchInit();
                        }
                        break;

            case OBJ_LPSW_UP:
                        if (scrollListBox(&lb,LB_UP) != -1) {
                    CurMail = getLBCurIdx(&lb);
                            LoadMail(CurMail);
                            DrawTxtPreview(Cache.Data);
                        }
                        break;

                case OBJ_LPSW_DOWN:
                        if (scrollListBox(&lb,LB_DOWN) != -1) {
                    CurMail = getLBCurIdx(&lb);
                            LoadMail(CurMail);
                            DrawTxtPreview(Cache.Data);
                            }
                        break;

                case OBJ_LPSW_PUSH:
                            if (MailNum){
                                    /* Enter full-screen display mode */
                                    DisplayLoop();
                                /* Redraw the list mode screen */
                                PVMailAppInit();
                    LibTchInit();
                        }
                sprintf(buf,"NOP");
                        break;

                /** MENU-BAR **/
                        case OBJ_HIC_MBAR:
                                GmPdwnSet();                            /* PullDownMenu */
                                if( GmPdwnFnc() == TRUE ){
                                        PVMailAppInit();
                                };
                                break;

                    default:
                            break;
                }
                
                /* Preview text-box update */
        LibTxtDspS(&txtBox,&tsts);

                if(tsts.act == ACT_ALM)
                        LibAlarm();     /* AlarmProc */
        }

}


/******************************************************************************
*       [Title]         SplitItems
*               Split items in the message string
******************************************************************************/
void SplitItems(byte *m, TMailMessageItems *mi){

    byte sep[] = "\x0FE";
    
        /* Separe items in the original message */
        mi->Subject = (byte *)strtok( m, sep);
    mi->From = strtok( NULL, sep);
    mi->To = strtok( NULL, sep);
    mi->Body = strtok( NULL, sep);
    mi->Date = strtok( NULL, sep);
    mi->Folder = strtok( NULL, sep);
    mi->CC = strtok( NULL, sep);
    mi->BCC = strtok( NULL, sep);
    
}

/******************************************************************************
*       [Title]         NewMessage
*               Prepare memory for a new message 
******************************************************************************/
bool NewMessage(int new_type){
    TMailRecord m;
    TMailMessageItems i;
    byte Msg[MAIL_DATA_SIZE + 1];

    if (MailNum == MAX_MAIL)
        return FALSE;
        
        strcpy(Msg, Cache.Data);
        SplitItems(Msg, &i);

    /* Prepare a new 'body' (for a reply or forward) */
    /* ... TO DO ... */
    
    initRecord(&m);
        
        switch(new_type){
        case NEW_EMPTY: 
        /* Prepare an empty message */   
        sprintf(m.Data, 
            "\x0FE" /* Subject */
            "\x0FE" /* From */
            "\x0FE" /* To */
            "\x0FE" /* Body */
            "\x0FE" /* Date */
            "\x0FE" /* Folder */
            "\x0FE" /* CC */
            "\x000" /* BCC */
                );
            break;
    
    case NEW_FORWARD:   
        sprintf(m.Data, 
            "Fwd: %s\x0FE" /* Subject */
            "%s\x0FE" /* From */
            "\x0FE" /* To */
            "\x0D---Original Message---\x0D"
                "%s\x0FE" /* Body */
            "\x0FE" /* Date */
            "\x0FE" /* Folder */
            "\x0FE" /* CC */
            "\x000" /* BCC */,
                i.Subject,
                    i.To,
                    i.Body);    
        
           break;
           
        case NEW_REPLY:   
        sprintf(m.Data, 
            "Re: %s\x0FE" /* Subject */
            "%s\x0FE" /* From */
            "%s\x0FE" /* To */
            "\x0D---Original Message---\x0D"
                "%s\x0FE" /* Body */
            "\x0FE" /* Date */
            "\x0FE" /* Folder */
            "\x0FE" /* CC */
            "\x000" /* BCC */,
                i.Subject,
                    i.To,
                    i.From,
                    i.Body);    
        
           break;
           
        default:
           return FALSE;
        }
        
        m.DataSize = strlen(m.Data);
        m.New      = TRUE;
        m.MailBox  = MAIL_OUTBOX;
        
        /* Copy to GLOBAL memory record */
    memcpy(&MailRecord, &m, getRecordSize(&m));
        /* Set cache to point to new record */
    Cache.Data     = MailRecord.Data;
    Cache.DataSize = &(MailRecord.DataSize);
    Cache.EntryID  = MailRecord.EntryID;
    Cache.New      = &(MailRecord.New);
    Cache.MailBox  = &(MailRecord.MailBox);
    Cache.fp       = 0xffff;
        
    return TRUE;
}

/******************************************************************************
*       [Title]         PVMailLoad
*               Load all mail from Flash 
******************************************************************************/
void PVMailLoad(void){
    char errbuf[40];
        LibClrDisp();
        
        /* Wait! */
        LibDspWinMessage(24,381,0,IB_MWIN_NONE | IX_MWIN_CENTER);
        LibPutDisp();
        
        /* Load data from Flash */
        SourceMail = -1;
        if (!initFile()) {

                sprintf (errbuf, "Couldn't open data file!\r"
                    "Error #%d", SubEntryStat);
                /*
        The following status code is output to global "SubEntryStat".
        IB_SERR_RNEW    0x00    New registration (no error)
        IB_SERR_RALDY   0x01    It has registered already (no error)
        IB_SERR_FOPEN   0xFF    Pointer for the input doesn't have a specification file name
        IB_SERR_NOSUBC  0xFE    SubEntry FULL
        IB_SERR_NGSUBC  0xFD    SubEntryNo. outside of the range
        IB_SERR_ALDYFL  0xFC    already used file name
        IB_SERR_NOFILE  0xFB    no exist file name
        IB_SERR_INJUST  0xF0    input condition error(Illegal function No.)
        */
                fatalError(errbuf);
        }
        collectMail();
        
        /* Reset to first mail in the list at start-up */
    CurMail = 0;
    if (MailNum)
        LoadMail(CurMail);
    else
        NewMessage(NEW_EMPTY);
    
}

/******************************************************************************
*       [Title]  PVMailTitle
*                Draw Program title and icon
******************************************************************************/
void PVMailTitle(void){

        LibPutGraph(0,0,SIcon);
        LibPutProStr(IB_PFONT3,14,1,ProgTitle,114);
        
}

/******************************************************************************
*       [Title]         Init screen
******************************************************************************/
void PVMailAppInit(void){

        /* Program icon and name */
        LibClrDisp();
        PVMailTitle();

        /* Buttons */
        LibPutFarData(75,0,149); /* TRE PULSANTI */

        LibPutProStr(IB_PFONT1,78,2,"View",30); 
        LibPutProStr(IB_PFONT1,108,2,"New",30);
        LibPutProStr(IB_PFONT1,136,2,"Sync",30);
        
        /* Show the current mail preview text box */
        InitTxtPreview();
    if (MailNum)
        LoadMail(CurMail);
    else
        NewMessage(NEW_EMPTY);
        DrawTxtPreview(Cache.Data);
    
    /* Draw list of mail messages */
    DrawList();
        
        /* Fill The 'folder' drop-down box */
        /*****   FillFolders();   DEBUG TRIAL ****/
        
        /* Init touch-screen table */
        PVMailTouchSet();

}

#if 0
/******************************************************************************
*       [Title]         FillFolders
*               Fill the drop-down box for folder selection.
*               The folder array should be filled by collectMail() before.
******************************************************************************/
void FillFolders(void){

/*
slw->x : Start coordinate of the window. Horizontal
slw->y : Start coordinate of the window. Vertical
slw->xs : Horizontal size of the window.
slw->ity : Line spacing of message list (9 -)
slw->np : Default position of the highlighted cursor ( no first time display with "0xff").
slw->slw_cmnt : Display message buffer (separated by "0xfe", ended by "0xff").
slw->rtv : Return value relevant to the selected message.
slw->t_xs : Message display start position in the window.
*/

    byte rtv[10+1] = {0,1,2,3,4,5,6,7,8,9,0xff};
    byte t_tbl[10][14+1];
    int i,k,len;
        
    /* Source data - DEBUG */
    strcpy(t_tbl[0],"Mileage");
    strcpy(t_tbl[1],"Fuel");
    strcpy(t_tbl[2],"Parking&Tolls");
    strcpy(t_tbl[3],"Taxi");
    strcpy(t_tbl[4],"Meals");
    strcpy(t_tbl[5],"Phone");
    strcpy(t_tbl[6],"Entertainment");
    strcpy(t_tbl[7],"Hotel");
    strcpy(t_tbl[8],"Miscellaneous");
    strcpy(t_tbl[9],"ALL");
    
    /* Creates the display buffer. */
    for(i=k=0;i<10;i++,k++){
        len = strlen(t_tbl[i]);
        memcpy(&slw_cmnt[k],t_tbl[i],len);
        k += len;
        if(i<9) slw_cmnt[k] = 0xfe; /* Data separation */
        else slw_cmnt[k] = 0xff; /* Final data. */
    }
    
    /* Init drop-down structure */
    slw.x = 9;
    slw.y = 32;
    slw.xs = 119;
    slw.ity = 9;
    slw.np = 0xff; /* No default selection */
    slw.cmnt = slw_cmnt;
    slw.rtv = rtv; /* List of return values */
    slw.t_xs = 3;  /* List items display x-offset */

}
#endif

/******************************************************************************
*       [Title]         SelectFolder
*       [Arguments]     None
*       [ReturnVAL]     index in the folder array
*   [Descr]     Open a selecttion drop-down box with folders
******************************************************************************/
int SelectFolder(void){

    return (LibSelWindowExt(&slw)); /* DEBUG */
    
}

/******************************************************************************
*       [Title]         FileSaveTouchSet
*       [Arguments]     None
*       [ReturnVAL]     None
*   [Descr]     Init touch info
******************************************************************************/
void PVMailTouchSet(void){

        /* Touch init */
        LibTchStackClr();
        LibTchStackPush( NULL );
        LibTxtTchSet(&txtBox);
        LibTchStackPush( TchHardIcon );
    LibTchStackPush( TchActionKey );
        LibTchStackPush( TchListViewIcon );
        LibTchStackPush( TchListTabs);
        LibTchStackPush( tchList );
    LibTchInit();

}

/**********************************************************
* [Title]    drawLBItem
*            Load from mem array, and draw one list item
**********************************************************/
static void drawLBItem(const TListBox *l, const TLBItemInfo *li)
{

        LibClrBox(li->x,li->y,l->w2,l->ItemHeight);
        if (li->idx >= 0 && li->idx < MailNum) {
        /* Load data for one mail message */
            LoadMail(li->idx);
            
            /* Show flags and subjects (first item in Data string) */
            if (*(Cache.New))
            LibPutGraph(li->x, li->y, Flag);
            
        LibStringDsp( Cache.Data,
            li->x+5, li->y+1, l->w2 - 7,
            IB_PFONT1);

                if (li->reverse)
                        LibReverse(li->x,li->y,l->w2,l->ItemHeight);
        }
}

/**********************************************************
*  [Title] toggleLBItem
*  [Descr] Reverse selected list item
**********************************************************/
static void toggleLBItem(const TListBox *l, const TLBItemInfo *li)
{
        if (li->idx >= 0 && li->idx < MailNum) {
                LibReverse(li->x,li->y,l->w2,l->ItemHeight);
        } else {
                LibClrBox(li->x,li->y,l->w2,l->ItemHeight);
        }
}

/******************************************************************************
*       [Title]         ShowList
*   [Descr]             Retreive and show the list-box with mail messages
******************************************************************************/
static void DrawList(void){

        initListBox(&lb,LB_IH,LB_IPP,lbltDot);
        lb.x = LB_X;
        lb.y = LB_Y;
        lb.w = LB_W;
        if (MAIL_INBOX == MailBox )
        lb.tab = TabInbox;
    else
        lb.tab = TabOutbox;

        lb.CurLine = CurMail;         /* Set currently selected line */
        lb.drawItem = drawLBItem;     /* Define the single-line draw function */
        lb.toggleItem = toggleLBItem; /* Define the line selection function */
        
        
    setLBItemsCount(&lb,MailNum);
        if (CurMail >= 0 && CurMail < MailNum)
                setLBCurrentItem(&lb,CurMail);
        else
                CurMail = 0;
    LibPutDisp(); 

}

/**********************************************************
*  [Title] DrawTxtPreview
*  [Descr] Setup initial values for the preview txt box
**********************************************************/
static void InitTxtPreview(void)
{

        txtBox.st_x = TXTPREVIEW_XLEFT; /* Start (X) of text display */
        txtBox.st_y = TXTPREVIEW_YTOP;  /* Start  (Y) of text display */
        txtBox.ed_x = LCD_X_DOT - TXTSCROLLBARWIDTH ; /* End (X) of text display */
        txtBox.it_y = TXTROWHEIGHT;     /* Line spacing (Y) */
        txtBox.MAXGYO = TXTPREVIEW_ROWNUM;  /* Number of text display lines */
        txtBox.font = IB_PFONT1;      /* Display font type */
        txtBox.csen = FALSE;          /* Cursor disabled */
        txtBox.rtnen = FALSE;         /* CR code display disabled */
        txtBox.maxmj = MAIL_DATA_SIZE - 1; /* Maximum number of allowable characters */
        txtBox.txtobj = OBJ_TXTTCH;  /* Object when text area is touched */
        txtBox.sbrobj = OBJ_SCR_BAR; /* Object when the scroll bar is touched */
        txtBox.tchtb = TchTxtBuf;    /* Pointer for text scroll bar area */
        txtBox.s_pos = &ComScr;      /* Pointer for text and scroll bar information */
        txtBox.gdcmt = &xFFFE;     /* User-defined guidance comment table */
    txtBox.gdcmt2 = TXT_GUIDE; /* Guidance comment strings */
        
}

/**********************************************************
*  [Title] DrawTxtPreview
*  [Descr] Show a display-only txt box based on 'data'
**********************************************************/
static void DrawTxtPreview(byte * data)
{

    /* Set the text buffer pointer to actual data string */
        txtBox.txbf = data;
        
    /* Draw text box and scroll bar frames */
    LibClrBox(0, TXTPREVIEW_YTOP - 2,  
        LCD_X_DOT, TXTPREVIEW_YHEIGHT + 3);
    LibBox(0, TXTPREVIEW_YTOP - 2,
        LCD_X_DOT, TXTPREVIEW_YHEIGHT + 3, 1);
        LibPutDisp();
        LibLine(LCD_X_DOT - TXTSCROLLBARWIDTH + 1 , TXTPREVIEW_YTOP - 1,
            1, TXTPREVIEW_YHEIGHT + 2, 1);

    /* (Re)-initialization of the text-box data */
        LibTxtDspInit(&txtBox);
        if(LibTxtDspC(&txtBox) == TRUE) {
                TxtItemIconDsp(&txtBox);
        LibPutDisp();
        }
        
}

/**********************************************************
* [Title]    TxtItemIconDsp
*            Draw icons next to each text-box item
**********************************************************/
void TxtItemIconDsp(TXTP *txt){
    int n = 0, 
            y_pos,
            n_items = 1;
  
    /* Count all text items */
    while (txt->txbf[n] != '\0'){
            if (txt->txbf[n] == '\xFE') n_items++;
            n++;
    }
    
    /* Clear the icon area */
    LibClrBox( txt->st_x - TXTICON_WIDTH + 1, txt->st_y - 1,  
           TXTICON_WIDTH - 1, txt->it_y*txt->MAXGYO);
        
        n = 1;
        /* Draw Item Icons */
        while(n_items && n <= txt->MAXGYO){
        if (txt->ditp[n] != 0xFF){
                    n_items--; 
                y_pos = txt->st_y + (txt->it_y * (n-1)) - 1;
                    LibPutGraph(txt->st_x - TXTICON_WIDTH + 1, y_pos,
                        MsgIcons[txt->ditp[n]]);
            }
            n++;
        }

}

/******************************************************************************
*       [Title]  initFile
*            Open the application Flash file
*       [Return] FALSE if any error occurred - TRUE otherwise
******************************************************************************/
static bool initFile(void)
{
    /* Wait! */
        LibDspWinMessage(24,381,0,IB_MWIN_NONE | IX_MWIN_CENTER);
        LibPutDisp();
        
    /* Open an existing file */
        if (PvOpenFile(MailFileName,&hMailFile,0))
                return (TRUE);
        
        /* File does not exist - create a new one from scratch */
        if (!PvOpenFile(MailFileName,&hMailFile,FM_CREATE))
                return (FALSE); /* Error while creating file */

        /* loadHeader(); *** FROM KINO - Retrieve a 'snapshot' from last usage *** */
        
    /* *** DEBUG **** Create some dummy records (in a new, empty file only) ^/
        CreateDummyRecords(); */

        return (TRUE);
                
}

/******************************************************************************
        [Title] GmPdwnSet_Edit
                Pull Down Menu Setup - for Edit mode
******************************************************************************/
void GmPdwnSet_Edit(void)
{
        /* MenuBAR Init */
        LibPullDownInit(MPdEdt,MPdSys,MPdOpt);  
        
    /* Disable some of the menu items */
    /* [System] */
        LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_SYSTEM,
            PULLDOWN_SYSTEM | 0x0100);
        
        /* [Edit] */
        LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_EDIT,
            PDWN_EDT_DELETE);

}

/******************************************************************************
        [Title] GmPdwnSet
                Pull Down Menu Setup
******************************************************************************/
void GmPdwnSet(void)
{
        word    pd;

        LibPullDownInit(MPdEdt,MPdSys,MPdOpt);          /* MenuBAR Init */

    /* Disable some of the menu items */
    /* [System] */
        pd =  PULLDOWN_SYSTEM | 0x0100;
        LibPullDownAtrSet( PULLDOWN_HTNDSP, PULLDOWN_SYSTEM, pd);


}

/******************************************************************************
        [Title]  GmPdwnFnc
                 Open Pull Down menu and execute Functions
        [Return] bool TRUE=Redisplay
******************************************************************************/
bool GmPdwnFnc()
{
        word    sel;
        bool    jdg = FALSE;

        sel = LibPullDown();            /* PullDownMenu */
        if(sel & PULLDOWN_SYSTEM)
                jdg = PdwnFncSys(sel);
        else if (sel & PULLDOWN_EDIT)
            jdg = PdwnFncEdt(sel);
            
        return(jdg);
}

/******************************************************************************
        [Title]         PdwnFncSys
                    System Pull Down Menu Functions
        [Arguments]     word Sel(select number)
        [Return]        bool TRUE=Rediplay
******************************************************************************/
bool PdwnFncSys(word sel)
/*TAIL*/
{
        bool    jdg = FALSE;
        switch(sel){

                case PDWN_SYS_DTTM:             /* Set_date/time                */
                        LibFuncDateTime();
                        break;

                case PDWN_SYS_SOUND:    /* Sound                                */
                        LibFuncSound(0);
                        break;

                case PDWN_SYS_FORMT:    /* Format                               */
                        LibFuncFormat();
                        break;

                case PDWN_SYS_CAPCT:    /* Capacity                             */
                        LibFuncCapa();
                        break;

                case PDWN_SYS_MMANE:    /* Memory_management    */
                        LibFuncMemoryManagement();
                        break;

/*              case PDWN_SYS_LANG:             /* Language                             */
/*                      if(LibFuncLang()==TRUE){                                        */
/*                              jdg = TRUE;                                                             */
/*                      }                                                                                       */
/*                      break;                                                                          */

                case PDWN_SYS_CNTRS:    /* Contrast                             */
                        LibFuncContrast();
                        break;

                case PDWN_SYS_TPA:              /* Touch_Panel_Alignment*/
                        LibFuncDigitizer();
                        break;

                default:
                        break;
        }
        return(jdg);
}

/******************************************************************************
        [Title]         PdwnFncEdt
                    Edit Pull Down Menu Functions
        [Arguments]     word Sel(select number)
        [Return]        bool TRUE=Rediplay
******************************************************************************/
bool PdwnFncEdt(word sel)
/*TAIL*/
{
    
        bool    jdg = FALSE;
        switch(sel){

                case PDWN_EDT_CUT:     
                    txtBox.txtst = TXTCUT;
                        break;
                        
                case PDWN_EDT_COPY:     
                    txtBox.txtst = TXTCOPY;
                        break;
                        
                case PDWN_EDT_PASTE:     
                    txtBox.txtst = TXTPASTE;
                        break;
                        
                case PDWN_EDT_DELETE: 
                    /* OK to Delete the selected data? Yes/No */
                    if (LibWinIcnMsg(ICON_TRASH, 274, 0x05)){  
                        jdg = DeleteMail(CurMail);
                }
                        break;
                        
        case PDWN_EDT_SEARCH:
                    if (SearchLoop()){
                        /* Searching! */
                LibDspWinMessage(24,337,0,IB_MWIN_NONE | IX_MWIN_CENTER);
                LibPutDisp();
                                if (!SearchMail(&CurMail, SearchMode)){
                            /* Data item not found! */
                    LibWinIcnMsg(ICON_BIKKURI,323,0x01);
                    LibPutDisp();
                            }
                    }
                        jdg = TRUE; /* refresh display */                   
                break;                  
                        
                default:
                        break;
        }
        return(jdg);
}


