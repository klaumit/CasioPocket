static int Anzeige(bool flag)
{
  int i;
  int z;
  int input_back;
  bool input_end=FALSE;
  TCHSTS tsts;


  LibLine(0,30,159,1,0x01);
  LibLine(80,30,1,130,0x01);
  LibLine(130,30,1,110,0x01);
  LibLine(0,140,159,1,0x01);
   /* OK Button */
  LibStringDsp(GetMsg(1),32,145,80,IB_PFONT3);
  /* Cancel Button */
  LibStringDsp(GetMsg(2),100,145,159,IB_PFONT3);
  LibPutDisp();

  /* Posten anzeigen */
  for(i=0;i<=4;i++)
  {
    LibStringDsp(GetPosten(i),10,40+(i*20),159,IB_PFONT1);
  }
  LibPutDisp();

  /* Checkboxen anzeigen */
  for (i=0;i<=4;i++)
  {
    if (check[i]==FALSE)
      /* Checkbox false */
      LibPutFarData(140,40+(i*20),16);
    else
      LibPutFarData(140,40+(i*20),17);
  }
  LibPutDisp();
  
  /* Werte anzeigen */
  for (i=0;i<=4;i++)
  {
    LibStringDsp(werte_str[i],90,40+(i*20),159,IB_PFONT1);
  }
  LibPutDisp();
     
  /* Buttons abfragen */
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(MenuIcon);
  LibTchInit();
  LibRepOff();
  
  i=0;
  while(input_end==FALSE)
  {
    LibTchWait(&tsts);
    switch(tsts.obj)
    {
      case OBJ_MENU_NEUEINGABE:
        if (flag==TRUE)
        {
          if (LibBlockIconClick(&TiconMenuNeueingabe,&tsts,B_ICON_RIGHT)==TRUE)
          {
            i=1;
            input_end=TRUE;
          }
          for( z = 0; z<5; z++ )
    	  {
            sprintf(&werte_str[z][0],"%.2f\n",FileInfo.ini_posten[z]);
            check[z]=FALSE;             
          }
        }  
        break;
      case OBJ_MENU_RECHNEN:
        if (flag==TRUE)
        {
          if (LibBlockIconClick(&TiconMenuRechnen,&tsts,B_ICON_LEFT)==TRUE)
          {
            i=2;
            input_end=TRUE;          
          }
        }
        break;
      case OBJ_MENU_OK:
        if (LibBlockIconClick(&TiconMenuOk,&tsts,B_ICON_RIGHT)==TRUE)
        {
          i=3;
          input_end=TRUE;         
        }
        break;
      case OBJ_MENU_CANCEL:
        if (LibBlockIconClick(&TiconMenuCancel,&tsts,B_ICON_RIGHT)==TRUE)
        {
          i=4;	
          input_end=TRUE;          
        }
        break;
      case OBJ_MENU_POSTEN1:
        check[0]=TRUE;
        LibPutFarData(140,40,17);
        LibPutDisp();
        LibWait (IB_500MWAIT);
        
        for (;;)
        {
          input_back=GetInput(40,60,0);
          if (input_back!=0) break;
        }
        
        if (input_back==1) 
        {
          if (flag==TRUE)
          {
            /* wert abziehen */	
            summe=summe+betrag;
            betrag=(atof(werte_str[0]))-betrag;	
          }
          sprintf(&werte_str[0][0],"%.2f\n",betrag);
        }
        if (flag==TRUE) i=5;
        else i=1;
        input_end=TRUE; 
        break;
      case OBJ_MENU_POSTEN2:
	check[1]=TRUE;
	LibPutFarData(140,60,17);
        LibPutDisp();
        LibWait (IB_500MWAIT);
        
        for (;;)
        {
          input_back=GetInput(40,60,1);
          if (input_back!=0) break;
        }
        if (input_back==1) 
        {
          if (flag==TRUE)
          {
            /* wert abziehen */	
            summe=summe+betrag;
            betrag=(atof(werte_str[1]))-betrag;	
          }
          sprintf(&werte_str[1][0],"%.2f\n",betrag);
        }        
        if (flag==TRUE) i=5;
        else i=1;
        input_end=TRUE; 
        break;
      case OBJ_MENU_POSTEN3:
        check[2]=TRUE;
        LibPutFarData(140,80,17);
        LibPutDisp();
        LibWait (IB_500MWAIT);
        
        for (;;)
        {
          input_back=GetInput(40,60,2);
          if (input_back!=0) break;
        }
        if (input_back==1) 
        {
          if (flag==TRUE)
          {
            /* wert abziehen */
            summe=summe+betrag;	
            betrag=(atof(werte_str[2]))-betrag;	
          }
          sprintf(&werte_str[2][0],"%.2f\n",betrag);
        }
        if (flag==TRUE) i=5;
        else i=1;
        input_end=TRUE; 
        break;
      case OBJ_MENU_POSTEN4:
        check[3]=TRUE;
        LibPutFarData(140,100,17);
        LibPutDisp();
        LibWait (IB_500MWAIT);
        
        for (;;)
        {
          input_back=GetInput(40,60,3);
          if (input_back!=0) break;
        }
        if (input_back==1) 
        {
          if (flag==TRUE)
          {
            /* wert abziehen */
            summe=summe+betrag;	
            betrag=(atof(werte_str[3]))-betrag;	
          }
          sprintf(&werte_str[3][0],"%.2f\n",betrag);
        }
        if (flag==TRUE) i=5;
        else i=1;
        input_end=TRUE; 
        break;
      case OBJ_MENU_POSTEN5:
        check[4]=TRUE;
        LibPutFarData(140,120,17);
        LibPutDisp();
        LibWait (IB_500MWAIT);
      
        for (;;)
        {
          input_back=GetInput(40,60,4);
          if (input_back!=0) break;
        }
        if (input_back==1) 
        {
          if (flag==TRUE)
          {
            /* wert abziehen */
            summe=summe+betrag;	
            betrag=(atof(werte_str[4]))-betrag;	
          }
          sprintf(&werte_str[4][0],"%.2f\n",betrag);
        }
        if (flag==TRUE) i=5;
        else i=1;
        input_end=TRUE; 
        break;       
        
    }
  }
  return(i); 
}


