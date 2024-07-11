static bool LoadData(void)
{
  FILE_BUF domfb;
  FILE_INFO domfinf;
  byte fs_entry;
  byte fm_entry;
  char filename[NAME_MAX];
  int i;

  /*
  LibWinIcnMsg(ICON_COFFEE,381,0);
  LibPutDisp();
  */
  /* DialogWin_Loading();
  */
  strcpy(filename,DATAFILENAME);
  if (LibSubEntrySave(filename,&fs_entry)==FALSE)
  /* filename save */
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

  if (LibFileFindNext(&domfb,&domfinf,0x00)==TRUE)
  {
    LibFileRead(&domfb,&domfinf);
    memcpy(&FileInfo,&domfb.fbuf.bin.bin_buf,sizeof(FileInfo));
    
    /* Strings in Variablen-Feld schreiben */
    for( i = 0; i<5; i++ )
    {
      sprintf(&werte_str[i][0],"%.2f\n",FileInfo.ini_posten[i]);
    }
    sprintf(&werte_ges[0],"%.2f\n",FileInfo.ini_gesamt);
    sprintf(&werte_letzte[0],"%.2f\n",FileInfo.ini_letzte);
    
    /* LibWinIcnMsg(ICON_OK,447,1);
    */
    /* DialogWin_LoadOk();
    LibWait (IB_1SWAIT);
    
    LibPutDisp();
    */
    return(TRUE);
  }
  else
  {
    LibErrorDisp(FlashStatus);
    return(FALSE);
  }
}


