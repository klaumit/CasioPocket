/* ------------------------------------------------------------- */
/*            Dumper - a memory dump program for casio PV        */
/* ------------------------------------------------------------- */


#include <stdrom.h>
#include <define.h>
#include <libc.h>


/* ------------------------------------------------------------- */
/*                  basic type definitions                       */
/* ------------------------------------------------------------- */

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long



/* ------------------------------------------------------------- */
/*                  Global constants definitions                 */
/* ------------------------------------------------------------- */


#define TOPBARX      60        /* x position of Top Icon bar     */
#define TOPBARY      0         /* y position of Top Icon bar     */  

#define FIRSTLINEY   0         /* y position of Top line         */ 
#define SECONDLINEY  12        /* y position of second line      */ 
#define THIRDLINEY   147       /* y position of third line       */ 
#define LASTLINEY    159       /* y position of last  line       */ 

#define ADDRESSX     2         /* x position of Address          */ 
#define ADDRESSY     3         /* y position of Address          */   
#define ADDRESSW     (4*6)     /* size of each part of addr area */
#define ADDRESSH     7         /* height of address area         */

#define SCREENX      0         /* x position of memory dump      */
#define SCREENY      17        /* y position of memory dump      */
#define SCREENW      160       /* width of screen area           */
#define SCREENH      (16*8)    /* height of screen area          */
#define CHARZONEX    112       /* character zone position        */
#define SCREENMAXX   (8*14-2)  /* size of active area            */


#define LETTERSX     0         /* x position of letters area     */
#define LETTERSY     150       /* y position of letters area     */
#define LETTERSW     160       /* width of letters area          */
#define LETTERSH     8         /* height of letters area         */

#define FIRSTLETTERX 3         /* x posit of first bottom letter */
#define LETTERSSPACE 10        /* x spacing between letters      */

#define NOCURSOR     0x8000    /* cursor not visible             */ 



/* ------------------------------------------------------------- */
/*                  Global variable definitions                  */
/* ------------------------------------------------------------- */

uint    MemSegment    = 0x8000;
uint    MemOffset     = 0;

const   char  Chars[] = "0123456789ABCDEF" ;

int     Cursor        = NOCURSOR;
int     AdrCurs       = NOCURSOR;

int     Locked        = 1;       


/* ------------------------------------------------------------- */
/*                  Global Icons definitions                     */
/* ------------------------------------------------------------- */

static byte far TOPBARICON [] = {
	GSIZE(73, 13),
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x80, 0x08, 0x00, 0x80, 0x08, 0x00, 0x80, 0x08, 0x00, 0xFF,
	0x8F, 0x88, 0xF8, 0x80, 0x08, 0x00, 0x82, 0x09, 0x8C, 0xFF,
	0x90, 0x49, 0x04, 0x80, 0x08, 0x00, 0x87, 0x08, 0xD8, 0xFF,
	0xA8, 0xAA, 0x02, 0x82, 0x09, 0x8C, 0x8D, 0x88, 0x70, 0xFF,
	0xAC, 0xAA, 0xFA, 0x87, 0x08, 0xD8, 0x98, 0xC8, 0x20, 0xFF,
	0xAA, 0xAA, 0xFA, 0x8D, 0x88, 0x70, 0x82, 0x09, 0x8C, 0xFF,
	0xA9, 0xAA, 0xFA, 0x98, 0xC8, 0x20, 0x87, 0x08, 0xD8, 0xFF,
	0xA8, 0xAA, 0x02, 0x80, 0x08, 0x00, 0x8D, 0x88, 0x70, 0xFF,
	0x90, 0x49, 0x04, 0x80, 0x08, 0x00, 0x98, 0xC8, 0x20, 0xFF,
	0x8F, 0x88, 0xF8, 0x80, 0x08, 0x00, 0x80, 0x08, 0x00, 0xFF,
	0x80, 0x08, 0x00, 0x80, 0x08, 0x00, 0x80, 0x08, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


static byte far ALLOWICON [] = {
	GSIZE(11, 11),
	0x00, 0x1F,
	0x00, 0x1F,
	0x02, 0x9F,
	0x05, 0x9F,
	0x0B, 0x9F,
	0x17, 0x1F,
	0x2E, 0x1F,
	0x24, 0x1F,
	0x38, 0x1F,
	0x00, 0x1F,
	0x00, 0x1F
};

static byte far LOCKICON [] = {
	GSIZE(11, 11),
	0x00, 0x1F,
	0x1F, 0x1F,
	0x20, 0x9F,
	0x40, 0x5F,
	0x5F, 0x5F,
	0x5F, 0x5F,
	0x5F, 0x5F,
	0x40, 0x5F,
	0x20, 0x9F,
	0x1F, 0x1F,
	0x00, 0x1F
};



/* ------------------------------------------------------------- */
/*                  Touch object definitions                     */
/* ------------------------------------------------------------- */

#define BTADDRESS     0x8001        /* address area  */ 
#define BTNORMAL      0x8002 
#define BTLOCK        0x8003 
#define BTUP          0x8004 
#define BTDOWN        0x8005 
#define BTPAGEUP      0x8006 
#define BTPAGEDOWN    0x8007
#define BTAREA        0x8008        /* display area  */ 
#define BTLETTERS     0x8009        /* letters area  */ 


#define BTNORMOFS     1             /* x offset in toolbar norm  button   */
#define BTLOCKOFS     13            /* x offset in toolbar lock  button   */
#define BTUPOFS       25            /* x offset in toolbar up    button   */
#define BTDOWNOFS     37            /* x offset in toolbar down  button   */
#define BTPAGEUPOFS   49            /* x offset in toolbar double up btn  */
#define BTPAGEDOWNOFS 61            /* x offset in toolbar double dn btn  */

#define BTTOP         1             /* y offset in toolbar of active zone */  
#define BTHEIGHT      11            /* y height in toolbar of active zone */
#define BTWIDTH       11            /*   widht  in toolbar of active zone */

#define POSX(x) (TOPBARX+x)         /*  to define x positions in tool bar */
#define POSY(y) (TOPBARY+y)         /*  to define y positions in tool bar */


TCHTBL far TouchTable [] =  {     /* TOUCH TABLE  */    

  ADDRESSX, ADDRESSY, ADDRESSX+ADDRESSW-1, ADDRESSY+ADDRESSH-1,                 /* first part */
  ACT_MAKE,
  BTADDRESS,
  0x0000, 

  ADDRESSX+ADDRESSW+6, ADDRESSY, ADDRESSX+2*ADDRESSW+6-1, ADDRESSY+ADDRESSH-1,  /* second part */
  ACT_MAKE,
  BTADDRESS,
  0x0000, 

  POSX(BTNORMOFS), POSY(BTTOP), POSX(BTNORMOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTNORMAL,
  0x0000,

  POSX(BTLOCKOFS), POSY(BTTOP), POSX(BTLOCKOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTLOCK,
  0x0000,

  POSX(BTUPOFS), POSY(BTTOP), POSX(BTUPOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTUP,
  0x0000,

  POSX(BTDOWNOFS), POSY(BTTOP), POSX(BTDOWNOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTDOWN,
  0x0000,

  POSX(BTPAGEUPOFS), POSY(BTTOP), POSX(BTPAGEUPOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTPAGEUP,
  0x0000,

  POSX(BTPAGEDOWNOFS), POSY(BTTOP), POSX(BTPAGEDOWNOFS+BTWIDTH-1), POSY(BTTOP+BTHEIGHT-1), 
  ACT_MAKE,
  BTPAGEDOWN,
  0x0000, 

  SCREENX, SCREENY, SCREENX+SCREENMAXX-1, SCREENY+SCREENH-1,
  ACT_MAKE,
  BTAREA,
  0x0000, 

  LETTERSX, LETTERSY, LETTERSX+LETTERSW-1, LETTERSY+LETTERSH-1,
  ACT_MAKE,
  BTLETTERS,
  0x0000, 

  0, 0, 0, 0,
  ACT_NONE,
  OBJ_END,
  0x0000

};


/* ------------------------------------------------------------- */
/*                Dump hex char and word in a buffer             */
/* ------------------------------------------------------------- */

int Nibble (uint value) {
  return (Chars[value & 0xf]);
}

void DumpByte(char * buf, uchar value) {
  *buf++ = Nibble (value >> 4);
  *buf++ = Nibble (value);
  *buf   = 0;
}

void DumpWord (char * buf, uint value) {
  DumpByte (buf,value >> 8);        /* high byte */
  DumpByte (buf+2,value & 0xff);    /* low  byte */
}



/* ------------------------------------------------------------- */
/*                Calcs the memory pointer                       */
/* ------------------------------------------------------------- */

char far * MemPnt () {
  return ((char far *) ((((ulong) MemSegment) << 16) + (ulong) MemOffset ));
}


/* ------------------------------------------------------------- */
/*           Display the icons on the top of the screen          */
/* ------------------------------------------------------------- */

void DisplayHeader () {
  LibLine (0,FIRSTLINEY,160,1,1);
  LibPutGraph (TOPBARX,TOPBARY,TOPBARICON);
  LibLine (0,SECONDLINEY,160,1,1); 
}


/* ------------------------------------------------------------- */
/*           Display the icons on the bottom of the screen       */
/* ------------------------------------------------------------- */

void DisplayFooter() {
  int i;

  LibLine (0,THIRDLINEY,160,1,1); 
  for (i=0; i<16; i++) {
    LibPutProFont (IB_CG57FONT,Chars[i],i*LETTERSSPACE+FIRSTLETTERX,LETTERSY);
    if (i!=0) LibLine (i*LETTERSSPACE,THIRDLINEY,1,160-THIRDLINEY,1); 
  } 
  LibLine (0,LASTLINEY,160,1,1); 
}





/* ------------------------------------------------------------- */
/*                       Cursor handling                         */
/* ------------------------------------------------------------- */


void CursorToScreen (int value, int *X, int *Y, int *W, int *H) {
  int row,col;
  row = Cursor / 16;
  col = Cursor % 16;
  *W = 7;
  *X = (col/2) * 14 + (col %2) * 6 + SCREENX -1;
  if (*X<0) { *X=0; *W=6;  }
  *Y = row * 8 + SCREENY -1 ;
  *H = 9;
}

void ScreenToCursor (int X, int Y, int *Cur) {
  int row,col,n,m ;
  row = (Y-SCREENY) / 8;
  n = (X-SCREENX) /14;
  m = (X-SCREENX) % 14;
  col = n*2 + m /6 ; 
  *Cur = row*16 + col;
}

void ToggleCursor () {
  int X,Y,W,H;
  if (Cursor!=NOCURSOR) {
    CursorToScreen (Cursor,&X,&Y,&W,&H);
    LibReverse (X,Y,W,H);
  }
}

void ClearCursorArea () {
  int X,Y,W,H;
  if (Cursor!=NOCURSOR) {
    CursorToScreen (Cursor,&X,&Y,&W,&H);
    LibClrBox (X,Y,W,H);
  }
}

void RefreshScreenCursor () {
  int X,Y,W,H;
  if (Cursor!=NOCURSOR) {
    CursorToScreen (Cursor,&X,&Y,&W,&H);
    LibPutDispBox (X,Y,W,H);
  }
}

/* ------------------------------------------------------------- */
/*                   ADR Cursor handling                         */
/* ------------------------------------------------------------- */

void AdrCursToScreen (int value, int *X, int *Y) {
  *X = AdrCurs * 6 + (AdrCurs/4)*6 + ADDRESSX - 1;
  *Y = ADDRESSY - 2;
}

void ScreenToAdrCurs (int X, int *Cur) {
  int n ;
  n = (X-ADDRESSX) /6;
  *Cur = n - n/5;
}

void ToggleAdrCurs () {
  int X,Y;
  if (AdrCurs!=NOCURSOR) {
    AdrCursToScreen (AdrCurs,&X,&Y);
    LibReverse (X,Y,7,11);
  }
}

void ClearAdrCursArea () {
  int X,Y;
  if (AdrCurs !=NOCURSOR) {
    AdrCursToScreen (AdrCurs,&X,&Y);
    LibClrBox (X,Y,7,11);
  }
}

void RefreshAdrCurs () {
  int X,Y;
  if (AdrCurs!=NOCURSOR) {
    AdrCursToScreen (AdrCurs,&X,&Y);
    LibPutDispBox (X,Y,7,11);
  }
}

/* ------------------------------------------------------------- */
/*                   Letter area calculations                    */
/* ------------------------------------------------------------- */

int LetterToNumber (int X) {
  return ( (X-LETTERSX) /10 ) ;
}


/* ------------------------------------------------------------- */
/*                  Display the memory pointer                   */
/* ------------------------------------------------------------- */

void DisplayPointer () {
  char  buf[12];

  ClearAdrCursArea();

  DumpWord (buf,MemSegment);  
  buf[4] = ':';
  DumpWord (buf+5,MemOffset);   /* low  word */
  LibPutProStr (IB_CG57FONT,ADDRESSX,ADDRESSY,buf,160);
}

/* ------------------------------------------------------------- */
/*           Display the memory area and memory pointer          */
/* ------------------------------------------------------------- */

void DisplayArea () {
  int   i,j,x;
  char  far * P;
  char  buf[3];

  ClearCursorArea();

  P = MemPnt();
  for (i=0; i<16; i++)  {
    for (j=0; j<8; j++)  {
      x = *P++;
      DumpByte (buf,x);  
      if (x<' ') x = ' '; 
      LibPutProStr  (IB_CG57FONT,j*14,i*8+SCREENY,buf,14);
      LibPutProFont (IB_CG57FONT,x,j*6+CHARZONEX,i*8+SCREENY);
    }
  }
}

void DisplayAll () {
  DisplayPointer ();
  ToggleAdrCurs();
  DisplayArea ();
  ToggleCursor ();
}

/* ------------------------------------------------------------- */
/*                Update of address on Letter pressed            */
/* ------------------------------------------------------------- */

void UpdateWord (uint *aword, int aNibble) {
  int  n;
  uint mask,value;
  
  n = AdrCurs %4;
  mask = 0xF000 >> (n*4);
  value = (uint) aNibble;
  value <<= (3-n)*4;
  *aword = (*aword & ~mask) | value;
}


void UpdateAddress (int aNibble) {
  if (AdrCurs<4)
    UpdateWord (&MemSegment,aNibble);
  else UpdateWord (&MemOffset,aNibble);
  ToggleAdrCurs();
  if (AdrCurs!=3 && AdrCurs!=7) AdrCurs++;
  DisplayAll();
  LibPutDisp();
  
}


/* ------------------------------------------------------------- */
/*                Update of data area on Letter pressed          */
/* ------------------------------------------------------------- */

void UpdateDataArea (int aNibble) {
  int       n;
  uint      mask;
  uchar     value;
  char  far * pnt;

  n = Cursor % 2;
  mask = 0x00F0 >> n*4;
  value = (uchar) aNibble;
  value <<= (1-n)*4;
  pnt = (char far *) ((((ulong) MemSegment) << 16) + (ulong) (MemOffset + Cursor/2));
  *pnt = (*pnt & ~mask) | value;
  ToggleCursor();
  Cursor = (++Cursor % 256);
  DisplayAll();
  LibPutDisp();
}




/* ------------------------------------------------------------- */
/*                    Handling of button events                  */
/* ------------------------------------------------------------- */


/* ------------------------- BtAddress ------------------------- */

void BtAddress (TCHSTS * event){
  ToggleAdrCurs();
  RefreshAdrCurs();
  ScreenToAdrCurs (event->x,&AdrCurs);
  ToggleAdrCurs();
  RefreshAdrCurs();
  ToggleCursor();
  RefreshScreenCursor();
  Cursor = NOCURSOR;
}



/* ------------------------ BtNormalize ------------------------ */

void BtNormal (TCHSTS * event){
  uint Para,Off;
  Para = MemOffset / 16;
  Off  = MemOffset % 16;
  MemSegment += Para;
  MemOffset = Off;
  DisplayAll();
  LibPutDisp();
}


/* ------------------------ BtLock ----------------------------- */

void BtLock (TCHSTS * event){
  ToggleCursor();
  RefreshScreenCursor();
  if (Locked)
    LibPutGraph (POSX(BTLOCKOFS),POSY(BTTOP),ALLOWICON);
  else {
    LibPutGraph (POSX(BTLOCKOFS),POSY(BTTOP),LOCKICON);
    Cursor = NOCURSOR;
  }
  Locked = ! Locked;
  LibPutDispBox (POSX(BTLOCKOFS),POSY(BTTOP),BTWIDTH,BTHEIGHT);
  ToggleCursor();
  RefreshScreenCursor();
}


/* ---------------------------- BtUp --------------------------- */

void BtUp (TCHSTS * event){
  MemOffset -= 8;
  if (Cursor!=NOCURSOR && Cursor <240) {
    ToggleCursor();
    Cursor += 16;
  }
  DisplayAll();
  LibPutDisp();
}


/* ---------------------------- BtDown ------------------------- */

void BtDown (TCHSTS * event){
  MemOffset += 8;
  if (Cursor!=NOCURSOR && Cursor > 15) {
    ToggleCursor();
    Cursor -= 16;
  }
  DisplayAll();
  LibPutDisp();
}


/* ---------------------------- BtPageUp ------------------------ */

void BtPageUp (TCHSTS * event){
  MemOffset -= 128;
  DisplayAll();
  LibPutDisp();
}

/* ---------------------------- BtPageDown ---------------------- */

void BtPageDown (TCHSTS * event){
  MemOffset += 128;
  DisplayAll();
  LibPutDisp();
}

/* ------------------------------ BtArea ------------------------ */

void BtArea (TCHSTS * event){ 
  if (!Locked) {
    ToggleCursor();
    RefreshScreenCursor();
    ScreenToCursor (event->x, event->y, &Cursor);
    ToggleCursor();
    RefreshScreenCursor();
    ToggleAdrCurs();
    RefreshAdrCurs();
    AdrCurs = NOCURSOR;
  }
}


/* ------------------------------ BtLetters --------------------- */

void BtLetters (TCHSTS * event){
  int nibble;
  nibble = LetterToNumber (event->x);
  if (Cursor!=NOCURSOR) {
    UpdateDataArea (nibble);
  }
  else { 
    if (AdrCurs!=NOCURSOR)
    UpdateAddress (nibble);
  } 
}


/* -------------------------------------------------------------- */
/*                          main program                          */
/* -------------------------------------------------------------- */


void main ()  {

  TCHSTS      events;

  LibClrDisp();	
  DisplayHeader();
  DisplayAll ();
  DisplayFooter();
  LibPutDisp();

  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);  
  LibTchStackPush(TouchTable);  

  LibTchInit();

  while (TRUE)  {
    LibTchWait(&events);
    switch (events.obj) {
      case BTADDRESS   : BtAddress   (&events);  break;
      case BTNORMAL    : BtNormal    (&events);  break;
      case BTLOCK      : BtLock      (&events);  break;
      case BTUP        : BtUp        (&events);  break;
      case BTDOWN      : BtDown      (&events);  break;
      case BTPAGEUP    : BtPageUp    (&events);  break;
      case BTPAGEDOWN  : BtPageDown  (&events);  break;
      case BTAREA      : BtArea      (&events);  break;
      case BTLETTERS   : BtLetters   (&events);  break;  
    }
  }
}

