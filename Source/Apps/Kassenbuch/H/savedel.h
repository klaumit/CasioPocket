static bool SaveDataDel(bool flag)
{
  FILE_BUF domfb;
  FILE_INFO domfinf;
  byte fs_entry;
  byte fm_entry;
  char filename[NAME_MAX];
  int i;

  /*DialogWin_Saving();
  */
  /* Werte ins Save-Feld übergeben */
  for( i = 0; i<5; i++ ) { FileInfo.ini_posten[i]=atof(werte_str[i]); }
  
  /* Wenn Neueingabe, dann Summe addieren und übergeben */
  if (flag==TRUE)
  {
    FileInfo.ini_gesamt=0.0;
    for(i=0;i<5;i++) {
      FileInfo.ini_gesamt=FileInfo.ini_gesamt + (atof(werte_str[i]));	
    } } else {
      FileInfo.ini_gesamt=atof(werte_ges);
    
    FileInfo.ini_letzte=0.0;  
  } 
  		
  
  /* LibPutMsgDlg("Summe : \r\r %.2f \r\rini_letzte : \r\r %.2f",summe, FileInfo.ini_letzte);
  */
  if (summe!=0.0)
  {
    FileInfo.ini_letzte=summe;
  } else
  { FileInfo.ini_letzte=atoi(werte_letzte); }  
  
  strcpy(filename,DATAFILENAME);

  if (LibSubEntrySave(filename,&fs_entry)==FALSE)
    return(FALSE);

  LibGetAllEntry(filename,&fm_entry,&fs_entry);

  /* main entry */
  domfb.fsb_main_entry_=fm_entry;
  /* sub entry */
  domfb.fsb_sub_entry_=fs_entry;
  /* open mode */
  domfb.fsb_scrt_info_=0x80;
  /* first data */
  domfinf.fp=0xffff;
  /* binary mode */
  domfinf.kind=FILE_KIND_BIN;
  /* info */
  memcpy(&domfb.fbuf.bin.bin_buf,&FileInfo,sizeof(FileInfo));
  /* size */
  domfb.fbuf.bin.char_num=sizeof(FileInfo);


  if (LibFileRemoveAll(&domfb)==TRUE)
  {
    if (LibFileWrite(&domfb,&domfinf)==TRUE)
    {
      /*LibInitDisp();
      */
      /* LibWinIcnMsg(ICON_OK,341,1);
      LibPutDisp();
      DialogWin_SaveOk();
      LibWait (IB_1SWAIT);
      */
      return(TRUE);
    }
    else
    {
      LibInitDisp();
      LibErrorDisp(FlashStatus);
      return(FALSE);
    }
  }
  else
  {
    LibInitDisp();
    LibErrorDisp(FlashStatus);
    return(FALSE);
  }
}

