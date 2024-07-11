#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <l_define.h>
#include <l_libc.h>

#include "gdefs.h"

#include "stdprocs.h"  /* Standard procedures */
#include "graphics.h"  /* graphic routines */
#include "fm.h"        /* routines supporting the file system */
#include "mem.h"       /* memory copy procedures */
#include "xymodem.h"   /* implementation of the YMODEM-protocol */

#include "debug.h"


void      far *options;
byte      far restorebuffer[1024];
byte          number5[5+1];
byte          name[15+1];  /* buffer to store a file/folder name in */

T_SCR_POS     scrbar1;                                 /* obere Scrollbar */
T_SCR_POS     scrbar2;                                 /* untere Scrollbar*/
SRL_STAT      nearserial;
AREA12LINE    far area12line[2][7];
byte          far *data;
byte          *neardata;
/* void *sbrk(size_t s){ return(NULL); }; */

byte area12linesel=0;
bool area12 = AREA1;
bool firstsel=TRUE;

/* for Options */
byte activepage=0;
byte oldpage=0;

/* Funktion des Datei-Baumes:
   - für die area 1 existiert ein struct das für jede Zeile die Entry-nummer des angezeigten
     entry's, den ausgewählten Eintrag, die x Koordinate und die Länge in Feldern speichert
     (für das Anklicken)
   - die Funktion ShowTree zeichnet die Einträge(abhängig von der Scrollbar) auf den Bildschirm
     und setzt dabei das Zeilen struct für area 1
   - der case Zweig für die area 1 setzt einen klick aufgrund der Daten des Zeilenstructs und
     den dazugehörigen daten im entry um(z.B.: Zweig öffnen(danach ShowTree), Zweig schließen(
     danach ShowTree), Datei anwählen(Markieren), Datei auswählen(Info anzeigen oder entspr. 
     Show-prozedur ausführen))
  
  Dateistruktur im Datafile :
  Jeder Datenblock hat einen Filepointer (benutzt von fp) auf den nächsten block

*/

void main()
{
  TCHSTS tsts;
  byte area1_x_field;
  byte area1_y_field;
  word act;
  word oldpos;
  byte selection;
/*  byte el_state; */
  
  /* TestFile(); */

/*  MessageBox(Message[0]); */
/*  RomCreationDate(); /* */
/*    PrintProStrSize();
    LibJumpMenu();  /* */
/*  byte number[6] = "00000";*/

  Init();
  while(TRUE)
    {
      LibTchWait( &tsts );
      switch( tsts.obj )
        {
          case OBJ_SCRBAR1 :
            oldpos = scrbar1.pos;
            act = LibScrollClick(&tsts, &scrbar1);
            switch (act) {
              case 0 : /* UP */
                if (scrbar1.pos > 0) {
                  scrbar1.pos--;
                  LibScrollPrint(scrbar1);
                }
              break;
              case 1 : /* DOWN */
                if (scrbar1.pos < scrbar1.vol-FLD_MAXYGRID) {
                  scrbar1.pos++;
                  LibScrollPrint(scrbar1);
                }

            }
            
            if (oldpos != scrbar1.pos) {
              /* selection movement according to oldpos and scrbar1.pos */
              AdjustSelection(oldpos, scrbar1.pos);
              /* Update tree */
              ClearArea1();
              ShowTree();
              globalselect(area12, area12linesel);
              LibPutDisp();
 
            }
          break;
          case OBJ_SCRBAR2 :
          break;  
          case OBJ_AREA1 :
            area1_x_field = tsts.x/FLD_SQRSIZE-1;
            area1_y_field = tsts.y/FLD_SQRSIZE-1;
            if ((area1_y_field <= scrbar1.dsp-1) && (tsts.act == ACT_BREAK_IN)) {
              if (area1_y_field != area12linesel) { /* Selected line is not the same */
                globalselect(AREA1, area1_y_field);
                LibPutDisp();
              } /* Open/Close subtree if possible */
              else {
                /* Wenn subtree vorhanden (öffnen/schließen) */
                if (fileinfo[area12line[area12?1:0][area12linesel].entryno].treesubentry != 0) {
                  fileinfo[area12line[area12?1:0][area12linesel].entryno].typeattr ^= BITS[5]; /* Change node open/close */
                  ClearArea1();
                  ShowTree();
                  globalselect(area12, area12linesel);
                  LibPutDisp();
                }
              }
            }
          break;  
          case OBJ_AREA2 :
          break;
          case OBJ_FILEICON :
            if (LibIconClick(&fileicon, &tsts)) 
/*                             Used with
              Open    0        Files
              Receive 1        Files
              Send    2        Files
              Delete  3        Files/Folders
              Rename  4        Files/Folders
              Move    5        Files/Folders
              New     6        Folders  */

              filemenuaction(filemenu());
          break;
          case OBJ_MISCICON :
            if (LibIconClick(&miscicon, &tsts)) 
              miscmenuaction(SelectionWindow(OBJ_MISCICONXORG, OBJ_MISCICONYORG+OBJ_MISCICONYSIZE+1,59, &MiscMenue[0][0], 4, 12, 0));
          break;
          case OBJ_RESETICON :
            if (LibIconClick(&reseticon, &tsts)) 
              Init();
          break;  
          case OBJ_LIGHT :
/*            el_state = LibGetEL();
            if (el_state = IB_ELP_OFF) { 
              LibELHandle(IB_ELP_ON); 
            }
            else {
              LibELHandle(IB_ELP_OFF);
            }*/
          break;
/***********************************************************************************************
      SUB Objects 
***********************************************************************************************/         
          /* *********** ABOUT BOX  *********** */
          case OBJ_ABOUTOKICON :
            if (LibIconClick(&aboutokicon, &tsts)) ExitSubTch();
          break;
          /* *********** OPTIONS allgemein *********** */
          case OBJ_OPTCB :
            if (LibIconClick(&optcbicon, &tsts)) {
              /*SelWindowOptionTypes(); */
              selection = SelectionWindow(OBJ_OPTCBXORG, OBJ_OPTCBYORG+OBJ_OPTCBYSIZE+2, OBJ_OPTCBXSIZE, &cb1string[0][0], 3, 22, activepage);
              if (selection != 0xff) {
                activepage = selection;
                DrawOpPage(); /* Draws new page and sets touch information accordingly */
                LibPutDisp();
              }
            }
          break;
          case OBJ_OPTOK :
            if (LibIconClick(&optokicon, &tsts)) {
              /* Save options (save tree) */
              SaveInfo("Save...", FALSE);
              ExitSubTch();
            }
          break;
          case OBJ_OPTCANCEL :
            if (LibIconClick(&optcancelicon, &tsts)) {
              /* set old Options */
              memfarcpy(restorebuffer, options, 1024);
              ExitSubTch();
            }
          break;
            /* *********** OPTIONS   PAGE 1 *********** */
            case OBJ_OPTP1CB1 :
              if (LibIconClick(&optp1cb1icon, &tsts)) {
                selection = SelectionWindow(10, 15+9+OBJ_OPTCBYSIZE+1, OBJ_OPTCBXSIZE, &BPSstring[0][0],8, 6, serial->speed-2);
                if (selection != 0xff) {
                  serial->speed = selection+2;
                  LibNumToStr(number5, BITS[serial->speed-1]*150, 5);
                  number5[5]=0;
                  LibPutProStr(IB_PFONT1, 12,26, number5, 89);
                  LibPutDisp();
                }
              }
            break;
            case OBJ_OPTP1Parity :
              serial->parit = parityredirect[(tsts.x-9)/40];
              PutSelection(10, 53, 40, 3, RedirectValues(serial->parit, parityredirect, 3));
              LibPutDisp();
            break;
            case OBJ_OPTP1DBL :
              serial->datab = databitredirect[(tsts.x-9)/40];
              PutSelection(10, 78, 40, 2, RedirectValues(serial->datab, databitredirect, 2));
              LibPutDisp();
            break;
            case OBJ_OPTP1SBL :
              serial->stopb = stopbitredirect[(tsts.x-9)/40];
              PutSelection(10, 103, 40, 2, RedirectValues(serial->stopb, stopbitredirect, 2));      
              LibPutDisp();
            break;
            case OBJ_OPTP1CB2 :
              if (LibIconClick(&optp1cb2icon, &tsts)) {
                selection = SelectionWindow(10, 118+5-(4*9+4), OBJ_OPTCBXSIZE, &flowcontrolstr[0][0],4, 16, serial->fctrl);
                if (selection != 0xff) {
                  serial->fctrl = selection;
                  LibPutFarData(10,118+9,79);
                  LibPutProStr(IB_PFONT1, 12,129, flowcontrolstr[serial->fctrl], 89);
                  LibPutDisp();
                }
              }
            break;
          default:
          break;        
        }
	  if(tsts.act == ACT_ALM)
        LibAlarm();  /* AlarmProc */
    }
}

/*void SelWindowOptionTypes()
{
  
  byte    valuesoptions[3+1] = {0,1,2,0xff};
  byte    stringlist[3*(23+1)+1];
  SLW2_ST listparams;
  byte    selection;
  byte    temp[3][23+1];
  word    i1,i2,len;

  strcpy(temp[0],cb1string[0]);
  strcpy(temp[1],cb1string[1]);
  strcpy(temp[2],cb1string[2]);
  for(i1=i2=0;i1<3;i1++,i2++) {
    len = strlen(temp[i1]);
    memcpy(&stringlist[i2], temp[i1],len);
    i2 += len;
    if(i1<2)
      stringlist[i2] = 0xfe;
    else {
     stringlist[i2] = 0xfe;
     stringlist[i2+1] = 0xff;    
    }
  }
  
  listparams.x    = OBJ_OPTCBXORG;
  listparams.y    = OBJ_OPTCBYORG+OBJ_OPTCBYSIZE+1;
  listparams.xs   = OBJ_OPTCBXSIZE;
  listparams.ity  = 9;
  listparams.rtv  = valuesoptions;  
  listparams.np   = 0;
  listparams.cmnt = stringlist;
  listparams.t_xs = 4;
  selection = LibSelWindowExt(&listparams);

  activepage = selection;
  DrawOpPage();   Draws new page and sets touch information accordingly 
  LibPutDisp();
}*/


void ExitSubTch()
{
  while(LibTchStackPop()!=NULL)
  LibTchInit();
  LibCloseWindow();
  LibPutDisp();
}

void miscmenuaction(byte action)
{
  byte i, y_add, x_add, i_mul;
  switch(action) {
    case 0 : /* Options */
      /* Alte Optionen Speichern */
      memfarcpy(options, restorebuffer, 1024);
      Options();
    break;
    case 1 : /* Item info */
      LibOpenWindow(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, OBJ_STDDLGBOXLXSIZE, OBJ_STDDLGBOXLYSIZE);
      LibPutFarData(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, 109);
      LibPutFarData(OBJ_ABOUTOKXORG, OBJ_ABOUTOKYORG, 27);

      LibPutProStr(IB_PFONT2,(160-50)/2,OBJ_STDDLGBOXLYORG+5,"Item info", 50);

      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5, OBJ_STDDLGBOXLYORG+16, "E-Nr.:", 28);
      LibNumToStr(number5, area12line[area12?1:0][area12linesel].entryno, 5);
      number5[5]=0;      
      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+28, OBJ_STDDLGBOXLYORG+16, number5, OBJ_STDDLGBOXLXSIZE-OBJ_STDDLGBOXLXORG-5);

      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+60, OBJ_STDDLGBOXLYORG+16, "SE-Nr.:", 28);
      LibNumToStr(number5, fileinfo[area12line[area12?1:0][area12linesel].entryno].treesubentry, 5);
      number5[5]=0;      
      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+92, OBJ_STDDLGBOXLYORG+16, number5, OBJ_STDDLGBOXLXSIZE-OBJ_STDDLGBOXLXORG-5);

      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5, OBJ_STDDLGBOXLYORG+46, "NE-Nr.:", 28);
      LibNumToStr(number5, fileinfo[area12line[area12?1:0][area12linesel].entryno].nextentry, 5);
      number5[5]=0;
      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+28, OBJ_STDDLGBOXLYORG+46, number5, OBJ_STDDLGBOXLXSIZE-OBJ_STDDLGBOXLXORG-5);

      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+60, OBJ_STDDLGBOXLYORG+46, "PE-Nr.:", 28);
    
      LibNumToStr(number5, fileinfo[area12line[area12?1:0][area12linesel].entryno].preventry, 5);
      number5[5]=0;      
      LibPutProStr(IB_PFONT1,OBJ_STDDLGBOXLXORG+5+92, OBJ_STDDLGBOXLYORG+46, number5, OBJ_STDDLGBOXLXSIZE-OBJ_STDDLGBOXLXORG-5);

      LibPutProStr(IB_PFONT1, OBJ_STDDLGBOXLXORG+5, OBJ_STDDLGBOXLYORG+26, "Attributes :", 55);
      y_add = 0;
      x_add = 54;
      i_mul = 0;
      for (i=0;i<8;i++) {
        LibPutProStr(IB_PFONT1, OBJ_STDDLGBOXLXORG+5+i_mul*22+x_add,OBJ_STDDLGBOXLYORG+26+y_add, attstrings[i],20);
        if ((fileinfo[area12line[area12?1:0][area12linesel].entryno].typeattr & BITS[i]) == BITS[i]) {
          LibPutProStr(IB_PFONT1, OBJ_STDDLGBOXLXORG+5+i_mul*22+15+x_add,OBJ_STDDLGBOXLYORG+26+y_add, "1", 5);
        }
        else  {
          LibPutProStr(IB_PFONT1, OBJ_STDDLGBOXLXORG+5+i_mul*22+15+x_add,OBJ_STDDLGBOXLYORG+26+y_add, "0", 5);
        }
        i_mul++;
        if (i==2) {
          x_add = 0;
          y_add = 10;
          i_mul = 0;
        }

      } 
      LibPutProStr(IB_PFONT1, (160-12)/2, OBJ_ABOUTOKYORG+2, "Ok", 12);      
      LibPutDispBox(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, OBJ_STDDLGBOXLXSIZE, OBJ_STDDLGBOXLYSIZE);
      LibTchStackPush(NULL);
      LibTchStackPush(TchAboutOk);
      /* Touch loop will continue in main procedure */
    break;
    case 2 : /* General info */
    break;
    case 3 : /* About */
      LibOpenWindow(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, OBJ_STDDLGBOXLXSIZE, OBJ_STDDLGBOXLYSIZE);

      LibPutFarData(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, 109);
      LibPutFarData(OBJ_ABOUTOKXORG, OBJ_ABOUTOKYORG, 27);
      LibPutProStr(IB_PFONT1, (160-12)/2, OBJ_ABOUTOKYORG+2, "Ok", 12);
      LibPutProStr(IB_PFONT1, (160-110)/2, OBJ_STDDLGBOXLYORG+7, "Filemanager by J. H\xB3ner", 110);      
      LibPutProStr(IB_PFONT1, (160-91)/2,  OBJ_STDDLGBOXLYORG+7+12, "\xDC 2000 by J. H\xB3ner", 91);      
      LibPutProStr(IB_PFONT1, (160-86)/2,  OBJ_STDDLGBOXLYORG+7+24, "All rights reserved", 86);
      LibPutProStr(IB_PFONT1, (160-73)/2, OBJ_STDDLGBOXLYORG+7+36, "e-mail: jh@ilo.de", 73);      
      LibPutDispBox(OBJ_STDDLGBOXLXORG, OBJ_STDDLGBOXLYORG, OBJ_STDDLGBOXLXSIZE, OBJ_STDDLGBOXLYSIZE);

      LibTchStackPush(NULL);
      LibTchStackPush(TchAboutOk);
      /* Touch loop will continue in main procedure */
    break;
  }
}

void InitOpPageTch()
{
  
  if (oldpage != activepage) {
    LibTchStackPop();
    switch(activepage) {
      case 0 :
        LibTchStackPush(TchOptionsPage1);
      break;
      case 1 :
        LibTchStackPush(TchOptionsPage2);
      break;
      case 2 :
        LibTchStackPush(TchOptionsPage3);
      break;
    }
  }
  oldpage = activepage;
}

void DrawOpPage()
{
  LibClrBox(OBJ_OPTCBXORG+1,OBJ_OPTCBYORG+1,OBJ_OPTCBXSIZE-14,OBJ_OPTCBYSIZE-2); /* Clear Combobox */
  LibPutProStr(IB_PFONT1, 2,2, cb1string[activepage], 89);
  LibClrBox(1,14,156,128);
  InitOpPageTch();
  switch(activepage) {
    case 0 : /* Serial options */
      LibPutProStr(IB_PFONT2, 3, 15, "BPS", 21);
      LibPutFarData(10,15+9,79);
      LibNumToStr(number5, BITS[serial->speed-1]*150, 5);
      number5[5]=0;
      LibPutProStr(IB_PFONT1, 12,26, number5, 89);

      LibPutProStr(IB_PFONT2, 3, 43, "Parity", 32);      
      LibPutProStr(IB_PFONT1, 20, 53, "Odd", 17);      
      LibPutProStr(IB_PFONT1, 60, 53, "Even", 21);
      LibPutProStr(IB_PFONT1, 100, 53, "None", 21);
      PutSelection(10, 53, 40, 3, RedirectValues(serial->parit, parityredirect, 3));
      
      LibPutProStr(IB_PFONT2, 3, 68, "Data bit length", 81);
      LibPutProStr(IB_PFONT1, 20, 78, "7 bit", 21);      
      LibPutProStr(IB_PFONT1, 60, 78, "8 bit", 21);
      PutSelection(10, 78, 40, 2, RedirectValues(serial->datab, databitredirect, 2));

      LibPutProStr(IB_PFONT2, 3, 93, "Stop bit length", 81);
      LibPutProStr(IB_PFONT1, 20, 103, "1 bit", 21);      
      LibPutProStr(IB_PFONT1, 60, 103, "2 bit", 21);
      PutSelection(10, 103, 40, 2, RedirectValues(serial->stopb, stopbitredirect, 2));
      
      LibPutProStr(IB_PFONT2, 3, 118, "Flow control", 100);
      LibPutFarData(10,118+9,79);
      LibPutProStr(IB_PFONT1, 12,129, flowcontrolstr[serial->fctrl], 89);
      
    break;
    case 1 : /* View options */
    break;
    case 2 : /* Registered file types */
    break;
  }
}

void Options()
{

  activepage = 0;
  oldpage=1;
  
  LibOpenWindow(0,0,160,160);
  LibTchStackPush(NULL);
  LibTchStackPush(TchOptions);  /* Combobox, Ok, Cancel */
  LibTchStackPush(TchOptionsPage1);

  LibPutFarData(OBJ_OPTCBXORG,OBJ_OPTCBYORG,79); /* Combobox */
  LibPutFarData(OBJ_OPTOKXORG, OBJ_OPTOKYORG, 27);
  LibPutFarData(OBJ_OPTCANCELXORG, OBJ_OPTCANCELYORG, 27);
  LibPutProStr(IB_PFONT1, OBJ_OPTOKXORG+((OBJ_OPTOKXSIZE-12)/2), OBJ_OPTOKYORG+2, "Ok", 12); 
  LibPutProStr(IB_PFONT1, OBJ_OPTCANCELXORG+((OBJ_OPTCANCELXSIZE-16)/2), OBJ_OPTCANCELYORG+2, "Esc", 16); 
  Draw3DBox(0,13,159,131);  
  DrawOpPage(); /* adds TchStack for page 1(0) */
  LibPutDisp();


}

void AdjustSelection(byte oldpos, byte newpos)
{
  if (oldpos < newpos) { /* DOWN */
    if (newpos-oldpos >= area12linesel)
      area12linesel = 0;
    else
      area12linesel -= (newpos-oldpos);
  }
  else { /* UP */
    switch (area12) {
      case AREA1 : 
        if ((oldpos-newpos)+area12linesel >= 6)
          area12linesel = 6;
        else
          area12linesel += (oldpos-newpos);
      break;
      case AREA2 :
        if ((oldpos-newpos)+area12linesel >= 5)
          area12linesel = 5;
        else
          area12linesel += (oldpos-newpos);
      break;
    }
  }
}

void ClearArea1()
{
  LibClrBox(1,14,159-12-1,92-14);
  firstsel = TRUE;
}

void Receive()
{
  /* Selection window for receive */
  byte    selection;
  byte    error;
  byte    number[10+1] = "0000000000";   /* 2.147.483.647 */
  FILEINFO fi;
  /* 3 Methoden :
      1. X/YMODEM
      2. own programm
      3. direct-cable-connection */
  /* Show selection window depending on wheter a folder or a file is selected */
  selection = SelectionWindow((160-70)/2, 50, 70, &receiveoptions[0][0],
                     3, 14, 0);
  switch(selection){
    case 0 : /* Receive X/YMODEM */
      if (InitReceive(&fi) != 0) {
        
      }
      
/*      while(LibTchStackPop()!=NULL)
      LibTchInit();
      LibCloseWindow();
      LibPutDispBox(MB_BXORG, MB_BYORG, MB_BXSIZE, MB_BYSIZE); */
    break;
    case 1 : /* PC-Direktverbindung (Direct Cable Connection) */
    break;
    case 2 : /* Program */
    break;
  }
}

void filemenuaction(byte action)
{
  byte  name[15+1];

/*                               Used with
              Open      0        Files
              Receive   1        Files
              Send      2        Files
              Delete    3        Files/Folders
              Rename    4        Files/Folders
              Move      5        Files/Folders
              New       6        Folders  
              Save tree 7        dont matter */
  switch (action) {
    case 0 : /* Open File */
    break;
    case 1 : /* Receive File(when folder is selected) */
      Receive();
    break;
    case 2 : /* Send File */
    break;
    case 3 : /* Delete File/Folder */
      fmdelete(area12line[area12?1:0][area12linesel].entryno);
      /* Show tree */
      ClearArea1();
      ShowTree();
      globalselect(area12, area12linesel);
      LibPutDisp();
    break;
    case 4 : /* Rename File/Folder */
    break;
    case 5 : /* Move File/Folder */
    break;
    case 6 : /* New folder */
      /* Get name (Eingabedialog) */
      Input(name, 15, "Foldername:", "New folder");
      fmNewFolder(name, area12line[area12?1:0][area12linesel].entryno);
      /* Show tree */
      ClearArea1();
      ShowTree();
      globalselect(area12, area12linesel);
      LibPutDisp();
    break;
    case 7 : /* Save current tree structure */
      SaveInfo("Save tree...", TRUE);
    break;
  }
}

void area1select(byte line)
{
  if (firstsel)  {
    LibReverse(FLD_XORG-1,
               FLD_YORG+line*FLD_SQRSIZE-1,
               FLD_SQRSIZE*FLD_MAXXGRID+3,
               FLD_SQRSIZE+1);
    firstsel = FALSE;
    area12linesel = line;
  }
  else {
    LibReverse(FLD_XORG-1,
               FLD_YORG+area12linesel*FLD_SQRSIZE-1,
               FLD_SQRSIZE*FLD_MAXXGRID+3,
               FLD_SQRSIZE+1);
    LibReverse(FLD_XORG-1,
               FLD_YORG+line*FLD_SQRSIZE-1,
               FLD_SQRSIZE*FLD_MAXXGRID+3,
               FLD_SQRSIZE+1);
    area12linesel = line;
  }
}

void area2select(byte line)
{
}

void globalselect(bool area, byte line)
{
  if (area != area12) { /* New area */
    /* Unselect previous area */
    firstsel = TRUE;
    switch (area12) {
      case AREA1 : area1select(area12linesel); break;
      case AREA2 : area2select(area12linesel); break;
    }
    /* Select new area for first time */
    firstsel = TRUE;
    area12 = area;
  }
  switch (area) {
    case AREA1 : area1select(line); break;
    case AREA2 : area2select(line); break;
  }
}

byte filemenu()
{
  /* Strings Open 0, Send 2, Delete 3, Rename 4, Move 5, Save tree 7*/
  byte    values_files[6+1] = {0,2,3,4,5,7,0xff};
  /* Strings New 6, Receive 1, Delete 3, Rename 4, Move 5, Save tree 7 */
  byte    values_dirs[6+1] = {6,1,3,4,5,7,0xff};
  byte    stringlist[7*(11+1)+1];
  SLW2_ST listparams;
  byte    selection;
  byte    temp[7][11+1];
  word    i1,i2,len;

  /* Show selection window depending on wheter a folder or a file is selected */
  if((fileinfo[area12line[area12?1:0][area12linesel].entryno].typeattr & BITS[0])==BITS[0]) { /* DIR */
	strcpy(temp[0]," New ");
	strcpy(temp[1]," Receive ");
	strcpy(temp[2]," Delete ");
	strcpy(temp[3]," Rename ");
	strcpy(temp[4]," Move ");
    strcpy(temp[5]," Save tree ");
	for(i1=i2=0;i1<6;i1++,i2++) {
	    len = strlen(temp[i1]);
	    memcpy(&stringlist[i2], temp[i1],len);
	    i2 += len;
	    if(i1<5)
          stringlist[i2] = 0xfe;
	    else {
          stringlist[i2] = 0xfe;    
          stringlist[i2+1] = 0xff;    
        }
	}
    listparams.rtv  = values_dirs;
  }
  else { /* FILE */
	strcpy(temp[0]," Open ");
	strcpy(temp[1]," Send ");
	strcpy(temp[2]," Delete ");
    strcpy(temp[3]," Rename ");
    strcpy(temp[4]," Move ");
    strcpy(temp[5]," Save tree ");
	for(i1=i2=0;i1<6;i1++,i2++) {
	    len = strlen(temp[i1]);
	    memcpy(&stringlist[i2], temp[i1],len);
	    i2 += len;
	    if(i1<5)
          stringlist[i2] = 0xfe;
	    else {
          stringlist[i2] = 0xfe;    
          stringlist[i2+1] = 0xff;    
        }
	}
    listparams.rtv  = values_files;
  }
  /* selection window */
  listparams.x    = OBJ_FILEICONXORG;
  listparams.y    = OBJ_FILEICONYORG+OBJ_FILEICONYSIZE+1;
  listparams.xs   = 63;  
  listparams.ity  = 9;
  listparams.np   = 0;
  listparams.cmnt = stringlist;
  listparams.t_xs = 4;
  selection = LibSelWindowExt(&listparams);
  return(selection);
}

void RunChain()
{
  word currententry;
  bool lastentry = FALSE;
/*  byte number[6] = "00000";   */
  byte name[15+1];

  currententry = startentry;

  while(!lastentry){
    /* if DIR show it */
    if ((fileinfo[currententry].typeattr & BITS[0])==BITS[0]) {
      openentrys++;
      if (countshow == SHOW) {
        /* Graphik zeichnen und area1line setzten */
        if ((openentrys-1 >= scrbar1.pos) && (openentrys-1 <= scrbar1.pos+FLD_MAXYGRID-1)) {
          if (currententry == 0)
            LibPutGraph(FLD_XORG+ebene*FLD_SQRSIZE,
                        FLD_YORG+(openentrys-1-scrbar1.pos)*FLD_SQRSIZE,MainFolder);
          else
            LibPutGraph(FLD_XORG+ebene*FLD_SQRSIZE,
                        FLD_YORG+(openentrys-1-scrbar1.pos)*FLD_SQRSIZE,Folder);
          memfartonearcpy(fileinfo[currententry].name, name, 16);
          LibPutProStr(IB_PFONT1,
                       FLD_XORG+(ebene+1)*FLD_SQRSIZE+FLD_NAMEXORG,
                       FLD_YORG+(openentrys-1-scrbar1.pos)*FLD_SQRSIZE+FLD_NAMEYORG,
                       name,
                       FLD_MAXXGRID*FLD_SQRSIZE-(FLD_XORG+(ebene+1)*FLD_SQRSIZE));

          area12line[area12?1:0][openentrys-1-scrbar1.pos].entryno = currententry;
          area12line[area12?1:0][openentrys-1-scrbar1.pos].x       = ebene;
          area12line[area12?1:0][openentrys-1-scrbar1.pos].length  = 1+LibGetProStrSize(IB_PFONT1,name)/FLD_SQRSIZE; 
          area12line[area12?1:0][openentrys-1-scrbar1.pos].used    = TRUE;
        }
      }
    }
 
    /* Node exists and node open */
    if ((fileinfo[currententry].treesubentry != 0) &&
        ((fileinfo[currententry].typeattr & BITS[5])==BITS[5])) {
      startentry = fileinfo[currententry].treesubentry;
      deleteeverythingfound = FALSE;
      ebene++;
      RunChain();
      ebene--;
    }

/*    LibNumToStr(number, fileinfo[currententry].typeattr, 5);
    number[6] = 0;
    LibPutProStr(IB_PFONT1, 20, 0, number, 100);
    memfartonearcpy(fileinfo[currententry].name, name, 5);
    LibPutProStr(IB_PFONT1, 20, 10, name , 150);
    LibPutDisp();*/

    if ((fileinfo[currententry].typeattr & BITS[4])==BITS[4]) lastentry=TRUE;


    if ((fileinfo[currententry].typeattr & BITS[3])!=BITS[3]) Error(FILE_SYSTEM_ERROR,NOT_USED_ENTRY_LINKED ,currententry,ebene,0);
    if (deleteeverythingfound) fileinfo[currententry].typeattr -= BITS[3];

    /* Check for overflow 
       The first check is not an very good one, but if the statement becomes true it is
       surely too late. The reason is because entryno is a pointer to the last entry used(
       for reading all data) so it represents not the number of entrys used, unused entrys
       that appear before the pointer are included !!!!!!!!!!!
       */

    if ((currententry > datafileinfo[0].entryno) || ((currententry==0xffff) && (lastentry==FALSE)))
      Error(FILE_SYSTEM_ERROR, TREE_FLOW_ERROR, datafileinfo[0].entryno, currententry, lastentry);
    /* set currententry to nextentry */
    currententry = fileinfo[currententry].nextentry;
  }
}

void ShowTree(void)
{
/*  byte number[6] = "00000"; */
  byte i;
  /* Clear area1line */
  for(i=0;i<=6;i++)
    area12line[area12?1:0][i].used = FALSE;
  
  countshow  = COUNT;
  startentry = 0;
  ebene      = 0;
  openentrys = 0; /* this value will be changed by RunChain */
  deleteeverythingfound = FALSE;
  RunChain();


/*  LibNumToStr(number, openentrys, 5);
  number[6] = 0;
  LibPutProStr(IB_PFONT1, 20, 0, number, 100);
  LibPutDisp();
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT);
  LibWait(IB_1SWAIT); */
  
  /* Setzen der scrollbar */
  scrbar1.vol = openentrys;                  /* TotalCount (number of records)   */
  if (openentrys <= FLD_MAXYGRID) {
    scrbar1.dsp = openentrys;                /* Number of display records on the screen */
    AdjustSelection(scrbar1.pos, 0);
    scrbar1.pos = 0;
  }
  else
    scrbar1.dsp = FLD_MAXYGRID;
  
  countshow  = SHOW;
  startentry = 0;
  ebene      = 0;
  openentrys = 0; 
  deleteeverythingfound = FALSE;
  RunChain();
  LibScrollPrint(scrbar1);
}

void Init(void)
{	
  /* Init Init */
  LibClrDisp();
  LibPutFarData(15,54,107);
  LibPutProStr(IB_PFONT3,(160-54)/2,74 , "Loading...", 54);
  LibPutDisp();

  InitData();
  InitFileSystem(); /* LoadData from Flash, create files*/
  Display(); /* with ShowTree */
  InitTouch();
}

void Error(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3)
{
/* Display errorno with OK */
 
  switch(errcode) {
    case FILE_SYSTEM_ERROR :
      switch(errno) {
        case IB_SERR_FOPEN      : { MessageOK(errcode, 0, addinfo1, addinfo2, addinfo3); break; }
        case IB_SERR_NOSUBC     : { MessageOK(errcode, 1, addinfo1, addinfo2, addinfo3); break; }
        case IB_SERR_NGSUBC     : { MessageOK(errcode, 2, addinfo1, addinfo2, addinfo3); break; }
        case IB_SERR_ALDYFL     : { MessageOK(errcode, 3, addinfo1, addinfo2, addinfo3); break; }
        case IB_SERR_NOFILE     : { MessageOK(errcode, 4, addinfo1, addinfo2, addinfo3); break; }
        case IB_SERR_INJUST     : { MessageOK(errcode, 5, addinfo1, addinfo2, addinfo3); break; }
        case DATA_DELETE_FAILED : { MessageOK(errcode, 6, addinfo1, addinfo2, addinfo3); break; }
        case DATA_FOUND_NONE_EXPECTED : { MessageOK(errcode, 8, addinfo1, addinfo2, addinfo3); break; } 
        case WRITE_ERROR        : { MessageOK(errcode, 9, addinfo1, addinfo2, addinfo3); break; }
        case NO_DATA_FOUND      : { MessageOK(errcode,10, addinfo1, addinfo2, addinfo3); break; }
        case READ_ERROR         : { MessageOK(errcode,11, addinfo1, addinfo2, addinfo3); break; }
        case TREE_FLOW_ERROR    : { MessageOK(errcode,12, addinfo1, addinfo2, addinfo3); break; }
        case NO_FREE_ENTRYS     : { MessageOK(errcode,13, addinfo1, addinfo2, addinfo3); break; }
        case ENTRY_NOT_USED     : { MessageOK(errcode,14, addinfo1, addinfo2, addinfo3); break; }
        case NOT_USED_ENTRY_LINKED : { MessageOK(errcode,15, addinfo1, addinfo2, addinfo3); break; }
     default                    : { MessageOK(errcode, 7, addinfo1, addinfo2, addinfo3); break; }
      }
    break;
    case SERIAL_ERROR :
      switch(errno) {
        case SR_CANT_OPEN_PORT  : { MessageOK(errcode, 0, addinfo1, addinfo2, addinfo3); break; }
      }
    break;
    case MEMORY_ERROR :
      switch(errno) {
        case MALLOC_FAILED      : { MessageOK(errcode, 0, addinfo1, addinfo2, addinfo3); break; }
      }
    break;
    default :
      MessageOK(0,0,0,0,0);
    break;
  }
/* Enter menu-mode */
  LibJumpMenu();
}

byte SetTypeAttr(bool filedir, bool viewable, bool protected, bool entryused, bool lastentry, bool nodeopen, bool read, bool firstentry)
{/*
   0      0 = File    1 = Dir
   1      File is viewable (a fitting procedure has been developed)
          this will be set by the receive procedure
   2      file is protected, system password is needed to access file
          note that the DATA and INFO-Files are stored in an unprotected area
   3      ENTRY USED(0=not used;1=used)
   4      last entry of a chain 
   5-7    reserved (0) */
  byte temp;
  temp = 0;
  if (filedir)    temp += 1;
  if (viewable)   temp += 2;
  if (protected)  temp += 4;
  if (entryused)  temp += 8;
  if (lastentry)  temp += 16;
  if (nodeopen)   temp += 32;
  if (read)       temp += 64;
  if (firstentry) temp += 128;
  return(temp);
}

void InitData()
{
  scrbar1.x    = SCRBAR1X;           /* ScrollBarDisplayStartPosition X */
  scrbar1.y    = SCRBAR1Y;           /* ScrollBarDisplayStartPosition Y */
  scrbar1.size = SCRBAR1SIZE;        /* ScrollBarHeight (vertical width) */
  scrbar1.vol  = 0;                  /* TotalCount (number of records)   */
  scrbar1.dsp  = 0;                  /* Number of display records on the screen */
  scrbar1.pos  = 0;                  /* Display start data position on the screen */

  scrbar2.x    = SCRBAR2X;     
  scrbar2.y    = SCRBAR2Y;     
  scrbar2.size = SCRBAR2SIZE;  
  scrbar2.vol  = 0;   
  scrbar2.dsp  = 0;   
  scrbar2.pos  = 0;
  
  /* Variablen initialisieren die während der Programmausführung geändert werden könnten */
  area12linesel=0;
  area12 = AREA1;
  firstsel=TRUE;
  activepage=0;
  oldpage=0;


  Initfmdata();
  serial = ((SRL_STAT far*)&finfofilebuf.fbuf.bin.bin_buf[0]);
  options = ((byte far*)&finfofilebuf.fbuf.bin.bin_buf[0]);
  
  data = ((byte far*)&fdatafilebuf.fbuf.bin.bin_buf[0]);
  neardata = ((byte *)&neardata[0]);
}

void InitTouch()
{
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchList);
  LibTchStackPush(TchModeIcon);		/* Pop Up Tools */
  LibTchInit();
}

void Display(void)
{
  int i;
  LibClrDisp();
  LibPutGraph(0,0,CornerIcon);
  LibPutFarData(OBJ_FILEICONXORG, OBJ_FILEICONYORG, 27);
  LibPutProStr(IB_PFONT1, 
               OBJ_FILEICONXORG+((OBJ_FILEICONXSIZE-15)/2), 
               OBJ_FILEICONYORG+2, "File", 28);
  
  LibPutFarData(OBJ_MISCICONXORG, OBJ_MISCICONYORG, 27);
  LibPutProStr(IB_PFONT1, 
               OBJ_MISCICONXORG+((OBJ_MISCICONXSIZE-20)/2), 
               OBJ_MISCICONYORG+2, "Misc", 28);

/*  LibPutGraph(2,15,Folder);  
  LibPutGraph(2,26,Folder);  
  LibPutGraph(2,37,Folder);  
  LibPutGraph(2,48,Folder);  
  LibPutGraph(2,59,Folder);  
  LibPutGraph(2,70,Folder);   
  LibPutGraph(2,81,Folder);   

  LibPutGraph(13,15,Folder);  
  LibPutGraph(24,15,Folder);  
  LibPutGraph(35,15,Folder);  
  LibPutGraph(46,15,Folder);  
  LibPutGraph(57,15,Folder);  
  LibPutGraph(68,15,Folder);  
  LibPutGraph(79,15,Folder);  
  LibPutGraph(90,15,Folder);  
  LibPutGraph(101,15,Folder);  
  LibPutGraph(112,15,Folder);  
  LibPutGraph(123,15,Folder);  
  LibPutGraph(134,15,Folder);   */
  

/*  LibPutFarData(25,0,149); 
  
  LibPutProStr(IB_PFONT1,25+2,2, sendstr, 26);
  LibPutProStr(IB_PFONT1,25+31,2, receivestr, 26);
  LibPutProStr(IB_PFONT1,25+59,2, deletestr, 26);*/

  for(i=1;i<=5;i++) LibMeshLine(1,93+i*11, 145,1);
    
  
 /* LibPutProStr(IB_PFONT1,2,150,"Length 145 1 TestTESTTestTESTTestTESTTest" , 145);
  LibPutProStr(IB_PFONT1,2,139,"Length 145 2 TestTESTTestTESTTestTESTTest" , 145);
  LibPutProStr(IB_PFONT1,2,128,"Length 145 3 TestTESTTestTESTTestTESTTest" , 145);
  LibPutProStr(IB_PFONT1,2,117,"Length 145 4 TestTESTTestTESTTestTESTTest" , 145);
  LibPutProStr(IB_PFONT1,2,106,"Length 145 5 TestTESTTestTESTTestTESTTest" , 145);
  LibPutProStr(IB_PFONT1,2,95,"Length 145 6 TestTESTTestTESTTestTESTTest" , 145); */
    
  LibLine(1,13,157,1,1);  
  LibLine(0,14,1,159-15,1);
  LibLine(159,14,1,159-15,1);
  LibLine(1,159,157,1,1);

  LibLine(1,93,158,1,1);
  LibLine(159-12,14,1,159-14,1);
  
  LibModeIconDsp();  /* popup tools */

  ShowTree(); 
  /* First select */
  globalselect(area12, area12linesel);

  LibScrollPrint(scrbar1);
  LibScrollPrint(scrbar2);
  LibPutDisp();
}