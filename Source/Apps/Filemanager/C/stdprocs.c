#include "stdprocs.h"

TCHTBL far TchCANCEL[] = 
  {
	/* CANCEL */
	IP_CANCELXORG, IP_CANCELYORG, IP_CANCELXORG+MB_BUTTONXSIZE-1, IP_CANCELYORG+MB_BUTTONYSIZE-1,
	ACT_ICON,
	OBJ_BUTTON_CANCEL,
	0x0000,

	/* END */
	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far IP_CANCEL = {&TchCANCEL[0], NULL, NULL, 0x02 };


byte buffer[2048]; 


void dwordtostring(dword value,byte *number)
{
  byte rest;
  byte stelle = 0;
  /* byte number[10+1] = "0000000000"; */
  /* number[0] = 0; */
  if (value == 0) {
   number[0] = 0x30;
   stelle = 1;
  }
  while(value!=0) {
    rest = value % 10;
    value /= 10;
    number[stelle] = rest+0x030;
    stelle++;
  }
  number[stelle] = 0;
  strrev(number);
}


void Input(byte *string, byte length, byte *subtitle, byte *title)
{
  TXTP   TextInput;
  TCHTBL Keyboard[3];
  TCHSTS tsts;
  bool   InputReady;
  byte   keycode;
  word   dumy[1];
  
  LibOpenWindow(0,0,160,160);

  LibPutProStr(IB_PFONT1, 10, 29, subtitle, 150);
  LibPutProStr(IB_PFONT3, (160-LibGetProStrSize(IB_PFONT3, title))/2, 10, title, 150);
  LibGdsBox(8,38,149,49);
  LibPutFarData(IP_CANCELXORG, IP_CANCELYORG,131);
  LibPutProStr(IB_PFONT1, IP_CANCELXORG+7, IP_CANCELYORG+2,"Esc" , 28);

  dumy[0] = 0xffff;
  TextInput.st_x   = 10;
  TextInput.st_y   = 40;
  TextInput.ed_x   = 140;
  TextInput.it_y   = 9;
  TextInput.MAXGYO = 1;
  TextInput.font   = IB_PFONT1;
  TextInput.csen   = TRUE;
  TextInput.rtnen  = 0x02; /* ????????? */
  TextInput.maxmj  = length;
  TextInput.txbf   = string;
  TextInput.gdcmt  = dumy;
  TextInput.txtobj = NULL;
  TextInput.sbrobj = 0x9000;
  TextInput.tchtb  = Keyboard;

  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchCANCEL);

  string[0] = 0;
  LibTxtInit(&TextInput);
  LibTxtTchSet(&TextInput);

  LibDispKey();
  
  InputReady = FALSE;
  while(!InputReady) {
    LibTxtDsp(&TextInput);
    LibPutDisp();
    
    keycode = LibGetKeyM(&tsts);
    
    if(keycode == KEY_NONE) {
      switch(tsts.obj){
        case OBJ_HIC_MBAR:
          LibEditPullDown();
        break;
        case OBJ_BUTTON_CANCEL :
          if (LibIconClick(&IP_CANCEL, &tsts)) {
            while(LibTchStackPop()!=NULL)
            LibTchInit();
            string[0] = 0;
            InputReady = TRUE;
          }
        break;
      }
    } else
    if (keycode == KEY_RTN) {
      while(LibTchStackPop()!=NULL)
      LibTchInit();
      InputReady = TRUE;
    }
    LibTxtInp(keycode,&tsts,&TextInput);
  }
  
  LibCloseWindow();
  LibPutDisp();
  /* InitTouch(); */
}

byte SelectionWindow(byte xpos, byte ypos, byte xsize, byte *strings, byte anz, byte maxstrlen, byte preselection)
{
  /* Strings Serial Options  About*/
  byte    values[21];
  SLW2_ST listparams;
  byte    selection=0;
  word    i1;
  byte    actanz;
  word    size;
  word    pos;
  bool    firstzero;
  
  if (anz <= 20) {
    for (i1=0;i1<anz;i1++) {
      values[i1] = i1;
    }
    values[anz] = 0xff;
  
    /* größe des blocks ? */
    actanz = 1;
    size = 0;
    pos = 0;
    firstzero = TRUE;
    while(actanz <=anz) {
      if ((strings[pos] == 0) && (firstzero)) {
        actanz++;
        size++;
        firstzero = FALSE;
      }
      if (strings[pos] != 0) firstzero = TRUE;
      if (firstzero) size++;
      pos++;
    }
    size++;
 
    pos = 0;
    for (i1=0;i1<anz;i1++) {
      memcpy((byte *)&buffer[0]+pos, strings+i1*maxstrlen, strlen(strings+i1*maxstrlen));
      pos += strlen(strings+i1*maxstrlen);
      buffer[pos] = 0xfe;
      pos++;
    }
    buffer[pos] = 0xff; 
  
    /* selection window */
    listparams.x    = xpos;
    listparams.y    = ypos;
    listparams.xs   = xsize;
    listparams.ity  = 9;
    listparams.rtv  = &values[0];  
    listparams.np   = preselection;
    listparams.cmnt = &buffer[0];
    listparams.t_xs = 4;
    selection = LibSelWindowExt(&listparams);
  }
  return(selection);
}

word RedirectValues(word value,word *redirectvalues, byte anz)
{
  word values;
  for(values=0;values<anz;values++) {
    if (redirectvalues[values] == value)
      break;
  }
  return(values);
}

bool MessageBox(byte *string)
{
  TCHSTS tsts;
  byte buffer[256];
  byte pos;
  byte lastpos;
  byte line;
  byte center;

  LibTchStackPush(NULL);
  LibTchStackPush(TchYESNO);
  LibTchInit();
 
  LibOpenWindow(MB_BXORG, MB_BYORG, MB_BXSIZE, MB_BYSIZE);
  LibClrBox(MB_BXORG, MB_BYORG, MB_BXSIZE, MB_BYSIZE);
  LibPutFarData(MB_BXORG, MB_BYORG, 109);
  LibPutFarData(MB_YESXORG, MB_YESYORG, 131);
  LibPutFarData(MB_NOXORG, MB_NOYORG, 131);
  LibPutProStr(IB_PFONT1,MB_YESXORG+((MB_BUTTONXSIZE-16)/2), MB_YESYORG+2, "Yes", 27);
  LibPutProStr(IB_PFONT1,MB_NOXORG+((MB_BUTTONXSIZE-11)/2), MB_NOYORG+2, "No", 27);
  
  pos = 0;
  lastpos = 0;
  line = 0;
  center = 0;
  while(string[pos]!=0) {
    if (string[pos]==13) {
      memcpy(buffer, string+lastpos, pos-lastpos);
      buffer[pos-lastpos] = 0;
      if (LibGetProStrSize(IB_PFONT1, buffer) >= MB_BXSIZE-MB_BEDGE*2)
        center = MB_BXORG+MB_BEDGE+1;
      else
        center = (160-LibGetProStrSize(IB_PFONT1, buffer)) /2;
      LibPutProStr(IB_PFONT1, center, 42+4+1+line*11, buffer, 128-8-2);
      lastpos = pos+1;
      line++;
    }
    pos++;
  }
  LibPutDisp();

  while(TRUE) {
    LibTchWait(&tsts);
    switch (tsts.obj) {
      case OBJ_BUTTON_YES : {
        if (LibIconClick(&MB_ICON_YES, &tsts)) {
          while(LibTchStackPop()!=NULL);
          LibTchInit();
          LibCloseWindow();
          return(TRUE);
        }
        break;
      }
      case OBJ_BUTTON_NO : {
        if (LibIconClick(&MB_ICON_NO, &tsts)) {
          while(LibTchStackPop()!=NULL);
          LibTchInit();
          LibCloseWindow();
          return(FALSE);
        }
        break;
      }
    }
  }
}  

void MessageOK(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3)
{
  TCHSTS tsts;
  byte   buffer[256];
  byte   pos;
  byte   lastpos;
  byte   line;
  byte   center;
  byte   number[6] = "00000";

  errcode--;  

  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchOK);
  LibTchInit();
 
  LibClrDisp();
  LibPutFarData(MB_BXORG, MB_BYORG, 109);
  LibPutFarData((160-MB_BUTTONXSIZE)/2, MB_NOYORG, 131);
  LibPutProStr(IB_PFONT1,(160-13)/2, MB_OKYORG+2, "OK", 27);

  if (LibGetProStrSize(IB_PFONT1, errcodemessages[errcode+1]) >= MB_BXSIZE-MB_BEDGE*2)
    LibPutProStr(IB_PFONT1,MB_BXORG+MB_BEDGE+1, MB_BYORG+MB_BEDGE+1, errcodemessages[errcode+1], 128-8-2);
  else
    LibPutProStr(IB_PFONT1,(160-LibGetProStrSize(IB_PFONT1, errcodemessages[errcode+1]))/2, MB_BYORG+MB_BEDGE+1, errcodemessages[errcode+1], 128-8-2);
  
  pos = 0;
  lastpos = 0;
  line = 0;
  center = 0;
  while(errnomessages[errcode][errno][pos]!=0) {
    if (errnomessages[errcode][errno][pos]==13) {
      memcpy(buffer, errnomessages[errcode][errno]+lastpos, pos-lastpos);
      buffer[pos-lastpos] = 0;
      if (LibGetProStrSize(IB_PFONT1, buffer) >= MB_BXSIZE-MB_BEDGE*2)
        center = MB_BXORG+MB_BEDGE+1;
      else
        center = (160-LibGetProStrSize(IB_PFONT1, buffer)) /2;
      LibPutProStr(IB_PFONT1, center, 53+4+1+line*11, buffer, 128-8-2);
      lastpos = pos+1;
      line++;
    }     
    pos++;
  }
  
  switch (errcode+1) {
    case FILE_SYSTEM_ERROR : {
      number[5] = 0;
      LibNumToStr(number, addinfo1, 5);
      LibPutProStr(IB_PFONT1, MB_BXORG+MB_BEDGE+1, MB_BYORG+MB_BYSIZE-26, number, 100);
      LibNumToStr(number, addinfo2, 5);      
      LibPutProStr(IB_PFONT1, (160-LibGetProStrSize(IB_PFONT1, number))/2, MB_BYORG+MB_BYSIZE-26, number, 100);
      LibNumToStr(number, addinfo3, 5);      
      LibPutProStr(IB_PFONT1, MB_BXORG+MB_BXSIZE-MB_BEDGE-LibGetProStrSize(IB_PFONT1, number), MB_BYORG+MB_BYSIZE-26, number, 100);
    }
  } 
  LibPutDisp();    

  while(TRUE) {
    LibTchWait(&tsts);
    switch (tsts.obj) {
      case OBJ_BUTTON_OK : {
        if (LibIconClick(&MB_ICON_OK, &tsts)) 
          LibJumpMenu();  
      break;
      }
    }
  }
  LibJumpMenu();
}