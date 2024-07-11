static int GetInput(int x,int y,int posten)
{
  TXTP      EventPrm;
  TCHTBL    TchTxtBuf[3];
  T_SCR_POS ComScr;
  word      GdTbl[1];
  byte      key_cd;
  TCHSTS    tsts;
  byte      input_end;
  int       modify;
  char      EditString[NAME_MAX+1];
  int       j;
  bool      esc;

  /* Clear Screen */
  LibInitDisp();
  LibClrBox(0,0,160,12);
  
  /* Box zeichnen */
  LibGdsBox(0,0,159,159);
  
  LibPutFarData(40,0,149);
  /* set */
  LibPutProStr(IB_CG57FONT,42,2,GetMsg(5),28);
  /* clr */
  LibPutProStr(IB_CG57FONT,72,2,GetMsg(6),28);
  /* esc */
  LibPutProStr(IB_CG57FONT,103,2,GetMsg(7),28);

  /* Posten anzeigen ... */
  LibStringDsp("Eingabe f\x0b4r ",15,25,159,IB_PFONT1);
  LibStringDsp(GetPosten(posten),70,25,159,IB_PFONT1);
  LibStringDsp(":",140,25,159,IB_PFONT1);
  LibLine(0,45,159,1,0x01);
  

  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchHeaderIcon);

  EditString[0]='\0';
  GdTbl[0]                        = 0xffff;
  EventPrm.st_x                   = x;
  EventPrm.st_y                   = y;
  EventPrm.ed_x                   = 56+x;
  EventPrm.it_y                   = 9;
  EventPrm.MAXGYO                 = 1;
  EventPrm.font                   = IB_CG57FONT;
  EventPrm.csen                   = FALSE;
  EventPrm.rtnen                  = 0x02;
  EventPrm.maxmj                  = NAME_MAX-2;
  EventPrm.txbf                   = EditString;
  EventPrm.gdcmt                  = GdTbl;
  EventPrm.txtobj                 = OBJ_STRING;
  EventPrm.sbrobj                 = NULL;
  EventPrm.tchtb                  = TchTxtBuf;
  EventPrm.s_pos                  = &ComScr;

  LibTxtInit(&EventPrm);
  LibTxtTchSet(&EventPrm);
  LibKeyInit();
  LibDispKey();

  input_end=FALSE;
  esc=FALSE;
  modify=0;

  while(input_end==FALSE)
  {
    LibTxtDsp(&EventPrm);
    LibPutDisp();
    key_cd=LibGetKeyM(&tsts);

    if (key_cd==247)
    {
      input_end=TRUE;
      modify=1;
    }

    if (key_cd==KEY_NONE)
      switch (tsts.obj)
      {
        /*
        case OBJ_HIC_MBAR:
          LibEditPullDown();
          break;
        case OBJ_HIC_ESC:
          modify=FALSE;
          input_end=TRUE;
          break;
        */
        case OBJ_IC_SET:
          if (LibBlockIconClick(&TiconSet,&tsts,B_ICON_LEFT)==TRUE)
          {
            if (EditString[0]!='\0')
            {
              input_end=TRUE;
              modify=1;
            } else
              LibWinIcnMsg(ICON_BADTZ,326,1);
          }
          break;
        case OBJ_IC_CLR:
          if (LibBlockIconClick(&TiconClr,&tsts,B_ICON_CENTER)==TRUE)
          {
            EditString[0]='\0';
            LibTxtInit(&EventPrm);
          }
          break;
        case OBJ_IC_ESC:
          if (LibBlockIconClick(&TiconEsc,&tsts,B_ICON_RIGHT)==TRUE)
          {
            modify=2;
            input_end=TRUE;
            esc=TRUE;
            check[posten]=FALSE;
          }
          break;
      }
      LibTxtInp(key_cd,&tsts,&EventPrm);
  }

   /* EditString checken ob auch nur zahlen ... */
  /* LibPutMsgDlg("stlen :\r\r%i",strlen(EditString));
  */
  if (strlen(EditString)<1) modify=0;
  else
  {
    for (j=0;j<=(strlen(EditString)-1);j++)
    {
      /* q=atoi(EditString[j]);
      LibPutMsgDlg("taste : \r\r%i",q);
      */
      if ( (EditString[j]>57 || EditString[j]<45)) modify=0;
      if (EditString[j]==47) modify=0;
           
    }
  }
  if (modify==1)
  {
    betrag=atof(EditString);
    /*  LibPutMsgDlg("Float :\r\r%.2f",betrag);
    */
  } else {
    if (esc==FALSE) LibWinIcnMsg(ICON_BADTZ,326,1);
    else modify=2;
  }
  LibClrBox(0,0,160,12);
  LibClrBox(0,88,160,72);
  LibTchRelease();
  LibTchInit();
  return(modify);
}
