DrawStat()
{
  int i;
  char count[10];
  float ergebnis;
  bool input_end=FALSE;
  TCHSTS tsts;
  
  	 
  /* Clear Screen */
  LibInitDisp();
  /* Box zeichnen */
  LibGdsBox(0,0,159,159);
  /* Überschrift */
  LibStringDsp(GetMsg(4),53,10,158,IB_PFONT3);
  LibLine(0,30,159,1,0x01);
  LibLine(0,140,159,1,0x01);
   /* OK Button */
  LibStringDsp(GetMsg(1),74,145,159,IB_PFONT3);
  
  /* Bezeichnungen ausgeben */
  for(i=0;i<5;i++) { LibStringDsp(GetMsg(15+i),10,40+(i*20),159,IB_PFONT1); }
  
  /* Werte anzeigen */  	
  
  /* 'Anfangswert' */
  LibStringDsp(werte_ges,120,40,159,IB_PFONT1);
  
  /* 'Verfügbar' */
  ergebnis=0.0;
  for(i=0;i<5;i++) { ergebnis=ergebnis + (atof(werte_str[i])); }    	
  sprintf(&count[0],"%.2f\n",ergebnis);
  LibStringDsp(count,120,60,159,IB_PFONT1);
  
  /* 'Gesamte Ausgaben' */
  ergebnis= (atof(werte_ges)) - ergebnis;
  sprintf(&count[0],"%.2f\n",ergebnis);
  LibStringDsp(count,120,80,159,IB_PFONT1);
  
  /* 'Letzte Ausgabe' */
  LibStringDsp(werte_letzte,120,100,159,IB_PFONT1);
  
  /* 'Aktuelle Ausgabe' */
  sprintf(&count[0],"%.2f\n",summe);
  LibStringDsp(count,120,120,159,IB_PFONT1);
  
  
  LibPutDisp();
  
  
   /* Buttons abfragen */
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(StatOkIcon);
  LibTchInit();
  LibRepOff();
  
  while(input_end==FALSE)
  {
    LibTchWait(&tsts);
    switch(tsts.obj)
    {
      case OBJ_STAT_OK:
        if (LibBlockIconClick(&TiconStatOk,&tsts,B_ICON_CENTER)==TRUE)
        {
          input_end=TRUE;
        }
        break;
    } 
  }
  /* WaitTouch();
  */
} 
 
 /*


drawstring ini[7],120,100
drawstring sum,120,120
show

$stat1
if touched(1,141,158,158)>0 then
  goto start
endif
goto stat1
*/
