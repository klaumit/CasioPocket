/*******************************************************************************
 Programname:	LodeRuner
      Author:	Vladius
        Date:	11.12.2002
********************************************************************************
Programhistory:

*******************************************************************************/

#include "lode.h"

#define DATAFILENAME "Runner"

struct {
  int a;
  int b;
  bool sou;
} SaveData;


void main()
{
char i;
    LibInitDisp();
    LibClrDisp ();
    Intro();
    Start();
    PollEvent(&tsts, 15);
    tsts.x= 1;
    tsts.y= 1;
            i=0;
            while (i!=1)
              {
              LibWait(IB_125MWAIT);
              PollEvent(&tsts, 15);
              kopat= getleverpush();
              Beg_Clear(); /*стирает*/
              LibWait(IB_125MWAIT);
              Beg_Clear();  /*рисует*/
              if ((((tsts.x > 1)&&(tsts.x < 160))&&((tsts.y > 1)/*&&(tsts.y < 160)*/))||(kopat!=0)) i=1;
              }
    while (loop!=20)
          {
          skorost= skorost_change+20;
          if (loop== WIN)
            {
      if (sound_on==1)
        {
        note("C4", 10000, 10);
        note("E4", 10000, 10);
        note("G4", 10000, 10);
        note("D4", 10000, 10);
        note("F4", 10000, 10);
        note("A4", 10000, 10);
        note("E4", 10000, 10);
        note("G4", 10000, 10);
        note("B4", 10000, 10);
        note("F4", 10000, 10);
        note("A4", 10000, 10);
        note("C5", 20000, 10);
        }
        else
        {
        LibWait(IB_500MWAIT);
        LibWait(IB_500MWAIT);
        LibWait(IB_500MWAIT);
        LibWait(IB_500MWAIT);
        }
            Start();
/*            LibPutMsgDlg("Level %d",level);*/
            PollEvent(&tsts, 15);
            tsts.x= 1;
            tsts.y= 1;
            i=0;
            while (i!=1)
              {
              LibWait(IB_125MWAIT);
              PollEvent(&tsts, 15);
              kopat= getleverpush();
              Beg_Clear(); /*стирает*/
              LibWait(IB_125MWAIT);
              Beg_Clear();  /*рисует*/
              if ((((tsts.x > 1)&&(tsts.x < 160))&&((tsts.y > 1)/*&&(tsts.y < 160)*/))||(kopat!=0)) i=1;
              }
            }

          if (loop== END)
            {
      if (sound_on==1)
        {
      note("C4", 3000, 10);
      note("B3", 3000, 10);
      note("A3", 3000, 10);
      note("G3", 3000, 10);
      note("F3", 3000, 10);
      note("E3", 3000, 10);
      note("D3", 6000, 10);
      note("C3", 12000, 10);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      }
      else
      {
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      }
         /*   saveSettings(0,0,sound_on);*/
            level=1;
            lives=3;
            LibClrDisp ();
            Intro();
            Start();

            PollEvent(&tsts, 15);
            tsts.x= 1;
            tsts.y= 1;
            i=0;
            while (i!=1)
              {
              LibWait(IB_125MWAIT);
              PollEvent(&tsts, 15);
              kopat= getleverpush();
              Beg_Clear(); /*стирает*/
              LibWait(IB_125MWAIT);
              Beg_Clear();  /*рисует*/
              if ((((tsts.x > 1)&&(tsts.x < 160))&&((tsts.y > 1)/*&&(tsts.y < 160)*/))||(kopat!=0)) i=1;
              }
            }

          if (loop== FAIL)
            {
      if (sound_on==1)
        {
      note("C4", 3000, 10);
      note("B3", 3000, 10);
      note("A3", 3000, 10);
      note("G3", 3000, 10);
      note("F3", 3000, 10);
      note("E3", 3000, 10);
      note("D3", 6000, 10);
      note("C3", 12000, 10);
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      }
      else
      {
      LibWait(IB_1SWAIT);
      LibWait(IB_1SWAIT);
      }
            Start();

            PollEvent(&tsts, 15);
            tsts.x= 1;
            tsts.y= 1;
            i=0;
            while (i!=1)
              {
              LibWait(IB_125MWAIT);
              PollEvent(&tsts, 15);
              kopat= getleverpush();
              Beg_Clear(); /*стирает*/
              LibWait(IB_125MWAIT);
              Beg_Clear();  /*рисует*/
              if ((((tsts.x > 1)&&(tsts.x < 160))&&((tsts.y > 1)))||(kopat!=0)) i=1;
              }
            }
          PollEvent(&tsts, 15);
          kopat= getleverpush();
          if (((tsts.x > 20)&&(tsts.x < 40))&&((tsts.y > 170)))
             {
             lev= level+1;
             if (lev==51) lev= 50;
             Menu();
             }
          if ((((tsts.x > 44)&&(tsts.x < 64))&&((tsts.y > 170)))||(kopat==6))
             {
             naprav= -2;
             tsts.x= 1/*x1-19  для x60*/;
             tsts.y= x2-8;
             }
          if ((((tsts.x > 68)&&(tsts.x < 88))&&((tsts.y > 170)))||(kopat==7))
             {
             naprav= 2;
             tsts.x= 159 /*x1+11  для x60*/;
             tsts.y= x2-8;
             }
          if ((((tsts.x > 92)&&(tsts.x < 112))&&((tsts.y > 170)))||(kopat==1))
             {
             vert= -2;
             tsts.x= x1-7;
             tsts.y= 1/*x2-13  для x60*/;
             }
/*x50*/   if ((((tsts.x > 116)&&(tsts.x < 136))&&((tsts.y > 170)))||(kopat==3))
             {
             vert= 2;
             tsts.x= x1-7;
             tsts.y= 159;
             }   /*end x50*/
/*  x60  if ((kopat==3))
             {
             vert= 2;
             tsts.x= x1-7;
             tsts.y= 159;
             }
          if ((((tsts.x > 116)&&(tsts.x < 136))&&((tsts.y > 170))))
             {
             kopat=2;
             }   для x60 */

          if (((tsts.x > 0)&&(tsts.x < 160))&&((tsts.y > 0)&&(tsts.y < 160)))
             {
             skorost= skorost_change;
             tsts.x= tsts.x+8;
             tsts.y= tsts.y+8;
             if (tsts.x/8<x1/8)
               {
               naprav= -2;
               }
             if (tsts.y/8<x2/8)
               {
               vert= -2;
               }
             if (tsts.x/8>x1/8)
               {
               naprav= 2;
               }
             if (tsts.y/8>x2/8)
               {
               vert= 2;
               }
             Dvijenie();
             }
          Padenie();
          if (zamed== 0)
            {
            Dvijenie_lox();
            zamed= 1;
            }
            else
            {
            Padenie_lox();
            zamed= 0;
            }
          Multik();
/*          LibWait(IB_125MWAIT);*/
         /* Задержка */
          for (i= 0; i<skorost; i++)
           {
           LibPutDisp ();
           }
          }

    LibJumpMenu();
}

void Menu()
   {
     LibOpenWindowS(ON,20,20,120,120);
/*     LibStringDsp ("MENU", 25,20,2000, IB_PFONT1);*/
     LibGdsBox(40,28,120,40);
     LibStringDsp ("Resume", 50,30,2000, IB_PFONT1);
     LibGdsBox(40,48,120,60);
     LibStringDsp ("Restart level", 50,50,2000, IB_PFONT1);
     LibGdsBox(40,68,105,80);
/*     LibStringDsp ("Next level", 50,70,2000, IB_PFONT1);*/

     LibGdsBox(110,68,118,80);
     LibStringDsp ("<", 111,70,2000, IB_PFONT1);
     LibGdsBox(129,68,137,80);
     LibStringDsp (">", 130,70,2000, IB_PFONT1);
     LibStringDsp ("Go to level", 50,70,2000, IB_PFONT1);
     itoa((lev), ib, 10);
     LibStringDsp (ib, 119,70,160, IB_PFONT1);

     LibGdsBox(25,88,75,100);
     if (sound_on==1)
        LibStringDsp ("Sound on", 27,90,2000, IB_PFONT1);
        else
        LibStringDsp ("Sound off", 27,90,2000, IB_PFONT1);
     LibGdsBox(110,88,118,100);
     LibStringDsp ("<", 111,90,2000, IB_PFONT1);
     LibGdsBox(129,88,137,100);
     LibStringDsp (">", 130,90,2000, IB_PFONT1);
     LibStringDsp ("Speed", 80,90,2000, IB_PFONT1);
     itoa((40-skorost_change), ib, 10);
     LibStringDsp (ib, 119,90,160, IB_PFONT1);
     LibGdsBox(90,108,120,120);
     LibGdsBox(40,108,70,120);
     LibStringDsp ("Save", 45,110,2000, IB_PFONT1);
     LibStringDsp ("Exit", 95,110,2000, IB_PFONT1);
     LibStringDsp ("v1.52", 25,130,2000, IB_PFONT1);
     LibStringDsp ("Vladius",100,130,2000, IB_PFONT1);
     LibPutDisp ();
     LibTchInit();
     LibTchStackClr();
     LibTchStackPush(TchAllDsp);
     LibTchWait(&tsts);
     if (((tsts.x > 40)&&(tsts.x < 120))&&((tsts.y > 30)&&(tsts.y < 40)))
         LibCloseWindow();
         else
     if (((tsts.x > 40)&&(tsts.x < 120))&&((tsts.y > 50)&&(tsts.y < 60)))
         {
         LibCloseWindow();
         lives=lives-1;
         if (lives<=0)
           loop= END;
           else
           loop= FAIL;
         }
         else
     if (((tsts.x > 40)&&(tsts.x < 105))&&((tsts.y > 70)&&(tsts.y < 80)))
         {
         LibCloseWindow();
         level= lev;
         lives= 1;
         loop= WIN;
         }
         else
     if (((tsts.x > 25)&&(tsts.x < 75))&&((tsts.y > 90)&&(tsts.y < 100)))
         {
         LibCloseWindow();
         if (sound_on==1)
           sound_on= 0;
           else
           sound_on= 1;
         Menu();
         }
         else
     if (((tsts.x > 110)&&(tsts.x < 118))&&((tsts.y > 70)&&(tsts.y < 80)))
         {
         LibCloseWindow();
         if (lev>1)
           lev= lev-1;
         Menu();
         }
         else
     if (((tsts.x > 129)&&(tsts.x < 137))&&((tsts.y > 70)&&(tsts.y < 80)))
         {
         LibCloseWindow();
         if (lev<50)
           lev= lev+1;
         Menu();
         }
         else
     if (((tsts.x > 110)&&(tsts.x < 118))&&((tsts.y > 90)&&(tsts.y < 100)))
         {
         LibCloseWindow();
         if (skorost_change<40)
           skorost_change= skorost_change+2;
         Menu();
         }
         else
     if (((tsts.x > 129)&&(tsts.x < 137))&&((tsts.y > 90)&&(tsts.y < 100)))
         {
         LibCloseWindow();
         if (skorost_change>0)
           skorost_change= skorost_change-2;
         Menu();
         }
         else
     if (((tsts.x > 40)&&(tsts.x < 70))&&((tsts.y > 110)&&(tsts.y < 120)))
         {
         saveSettings(lives,level,sound_on);
         LibCloseWindow();
         Menu();
         }
         else
     if (((tsts.x > 90)&&(tsts.x < 120))&&((tsts.y > 110)&&(tsts.y < 120)))
         {
         LibJumpMenu();
         }
         else
         {
         LibCloseWindow();
         Menu();
         }

   }

void Intro()
   {
    long int i=0;
    char l=0;
/*    loadData();*/
/*    if (SaveData.a>50)
      {
      saveSettings(0,0,1);
      loadData();
      }  */
    LibPutGraph (30, 20, title);
    LibGdsBox(45,118,115,130);
       LibStringDsp ("Control:", 20,50,2000, IB_PFONT1);
       LibStringDsp ("'Menu' - PVRunner menu", 5,60,2000, IB_PFONT1);
/*       LibStringDsp ("'Action lever' -left,right,up,down", 5,70,2000, IB_PFONT1);*/
       LibStringDsp ("'Action lever' - destroy the floor", 5,70,2000, IB_PFONT1);
       LibStringDsp ("'Scheduler'-left 'Contacts'-right", 5,80,2000, IB_PFONT1);
       LibStringDsp ("'Memo'-up 'QMemo'-down", 5,90,2000, IB_PFONT1);
/*       LibStringDsp ("'QMemo'- destroy", 5,80,2000, IB_PFONT1);*/
       LibStringDsp ("version for PV x50", 5,100,2000, IB_PFONT1);
/*       LibStringDsp ("version for PV x60", 5,100,2000, IB_PFONT1);*/

       LibStringDsp ("New game", 50,120,2000, IB_PFONT1);
/*       if ((SaveData.a>0)&&(SaveData.a<50))
         {*/
         LibGdsBox(45,133,115,145);
         LibStringDsp ("Load game", 50,135,2000, IB_PFONT1);
/*         }*/
    LibPutDisp ();
    PollEvent(&tsts, 15);
    tsts.x= 1;
    tsts.y= 1;
            while (l==0)
              {
              PollEvent(&tsts, 15);
              if ((((tsts.x > 50)&&(tsts.x < 120))&&((tsts.y > 120)&&(tsts.y < 130)))) l=1;
              if (/*(SaveData.a>0)&&*/(((tsts.x > 50)&&(tsts.x < 120))&&((tsts.y > 135)&&(tsts.y < 145)))) l=2;

              if (((tsts.x > 0)&&(tsts.x < 160))&&((tsts.y > 160)))
                 LibJumpMenu();
              }
    switch (l)
      {
      case 1:
         sound_on= SaveData.sou;
         break;
      case 2:
       loadData();
       if ((SaveData.a>0)&&(SaveData.a<50))
         {
         lives= SaveData.a;
         level= SaveData.b;
         sound_on= SaveData.sou;
         }
         break;
      }
   }


void Padenie()
     {
      /* Ящик взят */
      if ((k[(x1+1)/8][x2/8]== -2)&&(k[(x1+6)/8][x2/8]== -2))
         {
         k[x1/8][x2/8]= 0;
         Draw_box(x1/8,x2/8);
         yashikov= yashikov-1;
         LibPutDisp();
      if (sound_on==1)
         {
         note("C4", 1000, 10);
         }
         if (yashikov== 0)
            {
            Add_exit();
            LibPutDisp();
      if (sound_on==1)
        {
            note("C4", 10000, 10);
            note("E4", 10000, 10);
            note("G4", 10000, 10);
            note("C5", 20000, 10);
            note("G4", 10000, 10);
            note("C5", 20000, 10);
         }
            }
         }
     /* Падение на канат и с каната*/
     if ((k[(x1)/8][(x2)/8]==1)&&(k[(x1+7)/8][(x2)/8]==1)&&((int)((x2)/8)==(int)((x2+7)/8))&&(k[(x1)/8][(x2+7)/8]==1)&&(k[(x1+7)/8][(x2+7)/8]==1)&&(padla== 1)&&(kanat== 0))
          {
          padla= 0;
          kanat= 1;
          Beg_Clear();
          telo= 5;
          Beg_Runer();
          }
     /* Начал падать */
     if ((k[(x1)/8][(x2+8)/8]<=1)&&(k[(x1+7)/8][(x2+8)/8]<=1)&&(k[(x1)/8][(x2+1)/8]<=1)&&(k[(x1+7)/8][(x2+1)/8]<=1)&&(kanat==0))
          {
          padla= 1;
          vert= 2;
          Beg_Clear();
          telo= 4;
          x2=x2+vert;
          Beg_Runer();
          }
     /* долетел */
     if (  ((k[(x1)/8][x2/8+1]>1)  ||  (k[(x1+7)/8][x2/8+1]>1))  &&(padla== 1) )
          {
          padla= 0;
          Beg_Clear();
          telo= 0;
          Beg_Runer();
          }
     /* Копать */
     if ((kopat!= 0)/*   для x60 (kopat== 2)*/&&(k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][x2/8+1]==5)&&((k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][(x2)/8]== 0)||(k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][(x2)/8]== -1)||(k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][(x2)/8]== -3))&&(padla!=1)&&(lest!=1))
          {
          k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][(x2+8)/8]= -1;
          Get_Yama();
          Beg_Clear();
          x1=(((x1-2+(abs(abs(naprav)+naprav)*10/4))/8)*8/8- abs(naprav)/naprav)*8;
          Beg_Runer();
          if ((((k[(x1+1)/8][(x2+7+abs(abs(vert)+vert)/4)/8]==2)&&(k[(x1+6)/8][(x2+7+abs(abs(vert)+vert)/4)/8]==2))||((k[(x1+1)/8][(x2)/8]==2)&&(k[(x1+6)/8][(x2)/8]==2)))&&(k[(x1+1)/8][(x2+7+abs(abs(vert)+vert)/4)/8]<5))
              {
              lest= 1;
              }
          }
      }

void Dvijenie()
     {
     /* Канат */
     if (((tsts.y/8-x2/8)>abs(x1/8-tsts.x/8))&&(k[(x1)/8][(x2+8)/8]<=1)&&(k[(x1+7)/8][(x2+8)/8]<=1)&&(kanat==1)&&(x1/8==(x1+7)/8))
        {
        kanat= 0;
          padla= 1;
          vert= 2;
          Beg_Clear();
          telo= 5;
          x2=x2+vert;
          Beg_Runer();
        }
     if (  (k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][x2/8]<5)&&(k[(x1-2+abs(abs(naprav)+naprav)*10/4)/8][(x2+7)/8]<5) && ((((int)((x2)/8)==(int)((x2+7)/8))&&(k[(x1)/8][(x2)/8]==1)&&(k[(x1)/8][(x2+7)/8]==1))||(((int)((x2)/8)==(int)((x2+7)/8))&&(k[(x1+7)/8][(x2)/8]==1)&&(k[(x1+7)/8][(x2+7)/8]==1))) )
          {
          if (kanat==0)
             {
             kanat= 1;
             Beg_Clear();
             telo= 5;
             x1=x1+naprav;
             Beg_Runer();
             }
             else
             {
             Beg_Clear();
             x1=x1+naprav;
             Beg_Runer();
             }
          }
     /* Стена слева и справа */
     if ((k[(x1-2+(abs(abs(naprav)+naprav)*10/4))/8][x2/8]<5)&&(k[(x1-2+abs(abs(naprav)+naprav)*10/4)/8][(x2+7)/8]<5)&&(padla!=1)&&(lest!=1)&&(kanat!=1))
          {
          Beg_Clear();
          x1=x1+naprav;
          Beg_Runer();
          }
      /*лестница*/
     if ( ((abs(x2/8-tsts.y/8)>=abs(x1/8-tsts.x/8))|| ((x2>tsts.y)&&(k[x1/8][x2/8]<=1)&&(k[(x1+7)/8][x2/8]<=1)))&&(x1/8==(x1+7)/8) )
        {
        if ((((k[(x1+1)/8][(x2+7+abs(abs(vert)+vert)/4)/8]==2)&&(k[(x1+6)/8][(x2+7+abs(abs(vert)+vert)/4)/8]==2))||((k[(x1+1)/8][(x2)/8]==2)&&(k[(x1+6)/8][(x2)/8]==2)))&&(k[(x1+1)/8][(x2-1+abs(abs(vert)+vert)*9/4)/8]<5))
            {
            lest= 1;
            Beg_Clear();
            x2=x2+vert;
            Beg_Runer();
            }
        }
     if (/*(tsts.y<160)&& */abs(x2-tsts.y)<abs(x1-tsts.x)) lest= 0;
     if ((k[(x1)/8][(x2+7)/8]!=1)&&(k[(x1+7)/8][(x2+7)/8]!=1)&&(kanat== 1))
          {
          kanat= 0;
          Beg_Clear();
          telo= 0;
          Beg_Runer();
          }
     if (x2/8==1)
       {
       level= level+1;
       lives= lives+1;
       loop= WIN;
       }
     }

void Padenie_lox()
  {
  char i;
  char j;
  
  for (i=0; i<loxov; i++)
     {
     if (xl3[i]!=0)
      {
      /* Взять ящик */
      if ((k[(xl1[i]+1)/8][xl2[i]/8]== -2)&&(k[(xl1[i]+6)/8][xl2[i]/8]== -2)&&(yashik_l[i]==0)&&(rand()%3)==1)
         {
         k[xl1[i]/8][xl2[i]/8]= 0;
         Draw_box(xl1[i]/8,xl2[i]/8);
         yashik_l[i]=(rand()%200)+1;
         LibPutDisp();
         }
      if  (yashik_l[i]>0)
         {
         yashik_l[i]= yashik_l[i]-1;
         if (yashik_l[i]== 0)
           if ((padla_l[i]==0)&&(bashka_l[i]==0)&&(k[(xl1[i])/8][xl2[i]/8]== 0)&&(k[(xl1[i]+7)/8][xl2[i]/8]== 0))
              {
              k[xl1[i]/8][xl2[i]/8]= -2;
              Draw_box(xl1[i]/8,xl2[i]/8);
              }
              else  yashik_l[i]= 30;
         }
     /* Падение на канат */
     if (((int)((xl2[i])/8)==(int)((xl2[i]+7)/8))&&(k[(xl1[i])/8][(xl2[i])/8]==1)&&(k[(xl1[i]+7)/8][(xl2[i])/8]==1)&&(k[(xl1[i])/8][(xl2[i]+7)/8]==1)&&(k[(xl1[i]+7)/8][(xl2[i]+7)/8]==1)&&(padla_l[i]== 1)&&(kanat_l[i]== 0))
          {
          padla_l[i]= 0;
          kanat_l[i]= 1;
          Beg_Clear_lox(i);
          telo_l[i]= 5;
          Beg_Runer_lox(i);
          check_nap[i]= 1;
          }
     /* Падение */
     if (((xl1[i]/8==(xl1[i]+7)/8))&&(k[(xl1[i])/8][(xl2[i]+8)/8]<=1)&&(k[(xl1[i]+7)/8][(xl2[i]+8)/8]<=1) &&(k[(xl1[i])/8][(xl2[i]+1)/8]<=1)&&(k[(xl1[i]+7)/8][(xl2[i]+1)/8]<=1) &&(kanat_l[i]==0)&&(yama_l[i]==0))
          {
            padla_l[i]= 1;

            for (j=0; j<loxov; j++)
               {
               if ((i!=j)&&(abs(xl1[i]-xl1[j])<8)&&(abs(xl2[i]-xl2[j])<=8)&&(xl2[i]<xl2[j]))
                  {
                  padla_l[i]= 0;
                  bashka_l[i]= 1;
                  j=8;
                  }
                 else bashka_l[i]= 0;
               }

            if (padla_l[i]== 1)
              {
              vert_l[i]= 2;
              Beg_Clear_lox(i);
              telo_l[i]= 4;
              xl2[i]=xl2[i]+vert_l[i];
              Beg_Runer_lox(i);
              }
          }
      /* Яма */
     if ( /*(xl1[i]/8!=(xl1[i]+7)/8)&&*/ (k[(xl1[i])/8][(xl2[i])/8]==-1)&&(k[(xl1[i]+7)/8][(xl2[i])/8]==-1)&&(k[(xl1[i])/8][(xl2[i]+7)/8]==-1)&&(k[(xl1[i]+7)/8][(xl2[i]+7)/8]==-1)&&(padla_l[i]== 1))
          {
          padla_l[i]= 0;
          if ((yama_l[i]== 0)&&(yashik_l[i]!=0))
            {
            if (k[xl1[i]/8][xl2[i]/8-1]== 0)
               {
               k[xl1[i]/8][xl2[i]/8-1]= -2;
               Draw_box(xl1[i]/8,xl2[i]/8-1);
               yashik_l[i]= 0;
               }
            }
          yama_l[i]= yama_l[i]+1;
          k[(xl1[i])/8][(xl2[i])/8]= 7;
          Beg_Clear_lox(i);
          telo_l[i]= 4;
          Beg_Runer_lox(i);
          }
     if ((k[(xl1[i])/8][(xl2[i])/8]==7)&&(yama_l[i]>0))
          {
          yama_l[i]= yama_l[i]+1;
          if (yama_l[i]== 30)
             {
             yama_l[i]= 0;
             k[(xl1[i])/8][(xl2[i])/8]= -1;
             Beg_Clear_lox(i);
             telo_l[i]= 0;
             xl1[i]= xl1[i]+naprav_l[i];
             xl2[i]= xl2[i]-8;
             Beg_Runer_lox(i);
             }
          }
     /* долетел */
     if (  ((k[(xl1[i])/8][xl2[i]/8+1]>1)  ||  (k[(xl1[i]+7)/8][xl2[i]/8+1]>1))  &&(padla_l[i]== 1)&&(yama_l[i]==0) )
          {
          padla_l[i]= 0;
          Beg_Clear_lox(i);
          telo_l[i]= 0;
          Beg_Runer_lox(i);
          check_nap[i]= 1;
          }
      }
     }
/*          itoa (bashka_l[0],ib,10);
          LibStringDsp (ib, 40,20,2000, IB_PFONT1);
          LibPutDisp ();
*/
  }

void Dvijenie_lox()
  {
  char i;
  char j;
  
  for (i=0; i<loxov; i++)
     {
     if (xl3[i]!=0)
      {

             if ((x2/8<xl2[i]/8)&&(lest_l[i]== 0))
               {
                 vert_l[i]= -2;
               }
             if ((x2/8>xl2[i]/8)&&(lest_l[i]== 0))
               {
                 vert_l[i]= 2;
               }
         if (check_nap[i]==1)
             {
             if (x1/8<xl1[i]/8)
               {
                 naprav_l[i]= -2;
               }
             if (x1/8>xl1[i]/8)
               {
                 naprav_l[i]= 2;
               }
             check_nap[i]= 0;
             }
  /*Столкновение лохов*/
  for (j=0; j<loxov; j++)
     {
     if ((i!=j)&&(abs(xl1[i]-xl1[j])<8)&&(abs(xl2[i]-xl2[j])<8))
       {
          if (abs(xl2[i]-xl2[j])!=0)
            {
            vert_l[i]= abs(vert_l[i])*abs(xl2[i]-xl2[j])/(xl2[i]-xl2[j]);
            vert_l[j]= abs(vert_l[j])*abs(xl2[j]-xl2[i])/(xl2[j]-xl2[i]);
            }
          if (abs(xl1[i]-xl1[j])!=0)
            {
            naprav_l[i]= abs(naprav_l[i])*abs(xl1[i]-xl1[j])/(xl1[i]-xl1[j]);
            naprav_l[j]= abs(naprav_l[j])*abs(xl1[j]-xl1[i])/(xl1[j]-xl1[i]);
            }
       }
     }
     /* Падение с каната */
     if ((xl1[i]/8==(xl1[i]+7)/8)&&((x2/8-xl2[i]/8)>abs(xl1[i]/8-x1/8))&&(k[(xl1[i])/8][(xl2[i]+8)/8]<=1)&&(k[(xl1[i]+7)/8][(xl2[i]+8)/8]<=1)&&(kanat_l[i]==1))
        {
        kanat_l[i]= 0;
          padla_l[i]= 1;
          vert_l[i]= 2;
          Beg_Clear_lox(i);
          telo_l[i]= 5;
          xl2[i]=xl2[i]+vert_l[i];
          Beg_Runer_lox(i);
          check_nap[i]= 1;
        }
     /* Начало и продолжение каната */
     if (( ((int)((xl2[i])/8)==(int)((xl2[i]+7)/8)) &&(k[(xl1[i])/8][(xl2[i])/8]==1)&&(k[(xl1[i])/8][(xl2[i]+7)/8]==1))||(((int)((xl2[i])/8)==(int)((xl2[i]+7)/8))&&(k[(xl1[i]+7)/8][(xl2[i])/8]==1)&&(k[(xl1[i]+7)/8][(xl2[i]+7)/8]==1)))
          {
          if (kanat_l[i]==0)
             {
             kanat_l[i]= 1;
             Beg_Clear_lox(i);
             telo_l[i]= 5;
             xl1[i]=xl1[i]+naprav_l[i];
             Beg_Runer_lox(i);
             }
             else
             {
             Beg_Clear_lox(i);
             xl1[i]=xl1[i]+naprav_l[i];
             Beg_Runer_lox(i);
             }
          }
     /* Стена слева и справа */
     if ((k[(xl1[i]-2+(abs(abs(naprav_l[i])+naprav_l[i])*10/4))/8][xl2[i]/8]<5)&&(k[(xl1[i]-2+abs(abs(naprav_l[i])+naprav_l[i])*10/4)/8][(xl2[i]+7)/8]<5))
          {
          if ((bashka_l[i]==0)&&(yama_l[i]==0)&&(padla_l[i]!=1)&&(lest_l[i]!=1)&&(kanat_l[i]!=1))
             {
             Beg_Clear_lox(i);
             xl1[i]=xl1[i]+naprav_l[i];
             Beg_Runer_lox(i);
             }
          }
          else
          {
          if ((lest_l[i]!=1)&&(bashka_l[i]==0))
            if (naprav_l[i]==2) naprav_l[i]= -2; else naprav_l[i]= 2;
          }
      /*лестница*/
        if ((((k[(xl1[i]+1)/8][(xl2[i]+7+abs(abs(vert_l[i])+vert_l[i])/4)/8]==2)&&(k[(xl1[i]+6)/8][(xl2[i]+7+abs(abs(vert_l[i])+vert_l[i])/4)/8]==2))
               || ((k[(xl1[i]+1)/8][(xl2[i])/8]==2) && (k[(xl1[i]+6)/8][(xl2[i])/8]==2))
               )&&(k[(xl1[i]+1)/8][(xl2[i]-1+abs(abs(vert_l[i])+vert_l[i])*9/4)/8]<5)&&(bashka_l[i]!=1)&&(xl1[i]/8==(xl1[i]+7)/8))
            {
            if ((lest_l[i]== 0)&&((xl2[i])/8== x2/8))
             {;}else
             {
             if (lest_l[i]== 0)  check_nap[i]= 1;
             lest_l[i]= 1;
             Beg_Clear_lox(i);
             xl2[i]=xl2[i]+vert_l[i];
             Beg_Runer_lox(i);
             }
            }

     /* Проверка конца лестницы */
     if (
         (lest_l[i]== 1)&&
         (
            ( ((xl2[i])/8==x2/8)&& (k[(xl1[i]/8-1+(abs(abs(naprav_l[i])+naprav_l[i])*2/4))][(xl2[i]+8)/8]>=5) && (k[(xl1[i]/8-1+(abs(abs(naprav_l[i])+naprav_l[i])*2/4))][xl2[i]/8]==0)&&(k[(xl1[i]/8-1+(abs(abs(naprav_l[i])+naprav_l[i])*2/4))][(xl2[i]+7)/8]==0))
           ||( ((xl2[i])/8==x2/8) && (k[(xl1[i]/8-1+(abs(abs(naprav_l[i])+naprav_l[i])*2/4))][xl2[i]/8]==1)&&(k[(xl1[i]/8-1+(abs(abs(naprav_l[i])+naprav_l[i])*2/4))][(xl2[i]+7)/8]==1))
           ||(k[(xl1[i]+1)/8][(xl2[i]-1+abs(abs(vert_l[i])+vert_l[i])*9/4)/8]>=5)
           ||((k[(xl1[i]+1)/8][(xl2[i]+6+abs(abs(vert_l[i])+vert_l[i])/4)/8]!=2)&&(k[(xl1[i]+6)/8][(xl2[i]+6+abs(abs(vert_l[i])+vert_l[i])/4)/8]!=2))
         )
        )
               {
               lest_l[i]= 0;
               check_nap[i]= 1;
               }
     /* Кончился канат */
     if ((k[(xl1[i])/8][(xl2[i]+7)/8]!=1)&&(k[(xl1[i]+7)/8][(xl2[i]+7)/8]!=1)&&(kanat_l[i]== 1))
          {
          kanat_l[i]= 0;
          Beg_Clear_lox(i);
          telo_l[i]= 0;
          Beg_Runer_lox(i);
          }
      }
  /* Взяли за жопу! */
  if ((abs(xl1[i]-x1)<8)&&(abs(xl2[i]-x2)<8))
      {
      lives=lives-1;
      if (lives<=0)
        loop= END;
        else
        loop= FAIL;
      }
  }
}
void Start()
{
  char i1=0;
  char i2=0;
  char i;

  loop=0;
  LibClrDisp ();
naprav= 2;
vert= 0;
telo= 0;
kopat= 0;
padla= 0;
lest= 0;
kanat= 0;
for (i=0;i<15;i++)
 {
 yama[i]=0;
 yam1[i]=0;
 yam2[i]=0;
 }

for (i=0;i<8;i++)
 {
/* naprav_l[i]=0;*/
 vert_l[i]=0;
 telo_l[i]=0;
 yama_l[i]=0;
 yashik_l[i]=0;
 padla_l[i]=0;
 lest_l[i]=0;
 bashka_l[i]=0;
 kanat_l[i]=0;
 check_nap[i]=1;
 xl1[i]=0;
 xl2[i]=0;
 xl3[i]=0;
 naprav_l[i]=0;
 }
  while (i1<22)
      {
      i2=0;
      while (i2<22)
             {
             k[i1][i2]=0;
             i2=i2+1;
             }
      i1=i1+1;
      }
/*  полная */
  if (level>50)
    {
    level=1;
    }
/* демо  */
/*  if (level==4)
    {
    Bay_info();
    LibJumpMenu();
    }   */
  What_map();
  i1=0;
  while (i1<21)
        {
        k[i1][0]=6;
        k[0][i1]=6;
        k[i1][21]=6;
        k[21][i1]=6;
        i1=i1+1;
        }
    DrawMap();
/*    LibGdsReverse(20, 5+tudy, 30, 5+tudy);
    LibPutDisp ();*/
    Beg_Clear();
    for (i1=0; i1<8; i1++)
       Beg_Clear_lox(i1);

          itoa (lives,ib,10);
          LibStringDsp (ib, 40,0,2000, IB_PFONT1);
          LibStringDsp ("lives", 10,0,2000, IB_PFONT1);
          itoa (level,ib,10);
          LibStringDsp (ib, 90,0,2000, IB_PFONT1);
          LibStringDsp ("level", 60,0,2000, IB_PFONT1);
          LibPutDisp ();

}

void What_map()
     {
     switch(level)
       {
       case 1:
         Draw_array(map1);
         break;
       case 2:
         Draw_array(map2);
         break;
       case 3:
         Draw_array(map3);
         break;
       case 4:
         Draw_array(map4);
         break;
       case 5:
         Draw_array(map5);
         break;
       case 6:
         Draw_array(map6);
         break;
       case 7:
         Draw_array(map7);
         break;
       case 8:
         Draw_array(map8);
         break;
       case 9:
         Draw_array(map9);
         break;
       case 10:
         Draw_array(map10);
         break;
       case 11:
         Draw_array(map11);
         break;
       case 12:
         Draw_array(map12);
         break;
       case 13:
         Draw_array(map13);
         break;
       case 14:
         Draw_array(map14);
         break;
       case 15:
         Draw_array(map15);
         break;
       case 16:
         Draw_array(map16);
         break;
       case 17:
         Draw_array(map17);
         break;
       case 18:
         Draw_array(map18);
         break;
       case 19:
         Draw_array(map19);
         break;
       case 20:
         Draw_array(map20);
         break;
       case 21:
         Draw_array(map21);
         break;
       case 22:
         Draw_array(map22);
         break;
       case 23:
         Draw_array(map23);
         break;
       case 24:
         Draw_array(map24);
         break;
       case 25:
         Draw_array(map25);
         break;
       case 26:
         Draw_array(map26);
         break;
       case 27:
         Draw_array(map27);
         break;
       case 28:
         Draw_array(map28);
         break;
       case 29:
         Draw_array(map29);
         break;
       case 30:
         Draw_array(map30);
         break;
       case 31:
         Draw_array(map31);
         break;
       case 32:
         Draw_array(map32);
         break;
       case 33:
         Draw_array(map33);
         break;
       case 34:
         Draw_array(map34);
         break;
       case 35:
         Draw_array(map35);
         break;
       case 36:
         Draw_array(map36);
         break;
       case 37:
         Draw_array(map37);
         break;
       case 38:
         Draw_array(map38);
         break;
       case 39:
         Draw_array(map39);
         break;
       case 40:
         Draw_array(map40);
         break;
       case 41:
         Draw_array(map41);
         break;
       case 42:
         Draw_array(map42);
         break;
       case 43:
         Draw_array(map43);
         break;
       case 44:
         Draw_array(map44);
         break;
       case 45:
         Draw_array(map45);
         break;
       case 46:
         Draw_array(map46);
         break;
       case 47:
         Draw_array(map47);
         break;
       case 48:
         Draw_array(map48);
         break;
       case 49:
         Draw_array(map49);
         break;
       case 50:
         Draw_array(map50);
         break;
       }
    }


void DrawMap()
     {
     char i1=1;
     char i2=1;
     yashikov= 0;
     loxov= 0;
     while (i1<21)
           {
           i2=1;
           while (i2<21)
                 {
                 if (k[i1][i2]==6)
                   {
                   Draw_gdsbox(i1*8-8,i2*8-8,i1*8+7-8,i2*8+6-8);
                   }
                 if (k[i1][i2]==-4)
                   {
                   Draw_gdsbox(i1*8-8,i2*8-8,i1*8+4-8,i2*8+2-8);
                   Draw_gdsbox(i1*8+6-8,i2*8-8,i1*8+7-8,i2*8+2-8);
                   Draw_gdsbox(i1*8-8,i2*8+4-8,i1*8+1-8,i2*8+6-8);
                   Draw_gdsbox(i1*8+3-8,i2*8+4-8,i1*8+7-8,i2*8+6-8);
                   }
                 if (k[i1][i2]==5)
                   {
                   Draw_gdsbox(i1*8-8,i2*8-8,i1*8+4-8,i2*8+2-8);
                   Draw_gdsbox(i1*8+6-8,i2*8-8,i1*8+7-8,i2*8+2-8);
                   Draw_gdsbox(i1*8-8,i2*8+4-8,i1*8+1-8,i2*8+6-8);
                   Draw_gdsbox(i1*8+3-8,i2*8+4-8,i1*8+7-8,i2*8+6-8);
                   }
                 if (k[i1][i2]==2)
                   {
                   Draw_gdsbox(i1*8+1-8,i2*8-8,i1*8+1-8,i2*8+7-8);
                   Draw_gdsbox(i1*8+6-8,i2*8-8,i1*8+6-8,i2*8+7-8);
                   Draw_gdsbox(i1*8+2-8,i2*8+1-8,i1*8+5-8,i2*8+1-8);
                   Draw_gdsbox(i1*8+2-8,i2*8+5-8,i1*8+5-8,i2*8+5-8);
                   }
                 if (k[i1][i2]==1)
                   {
                   Draw_gdsbox(i1*8-8,i2*8-8,i1*8+7-8,i2*8-8);
                   }
                 if (k[i1][i2]==-2)
                   {
                   Draw_box(i1,i2);
                   yashikov= yashikov+1;
                   }
                 if (k[i1][i2]==-5)
                   {
                   xl1[loxov]=i1*8;
                   xl2[loxov]=i2*8;
                   xl3[loxov]=1;
                   naprav_l[loxov]=2;
                   loxov= loxov+1;
                   k[i1][i2]= 0;
                   }
                 if (k[i1][i2]==-6)
                   {
                   x1=i1*8;
                   x2=i2*8;
                   x3=-1;
                   k[i1][i2]= 0;
                   }

                 i2=i2+1;
                 }
           i1=i1+1;
           }

     }
     
void Beg_Runer()
     {
     if (telo<3) telo= telo+1;
     if (telo==3) telo=0;
     if (telo>4) telo= telo+1;
     if (telo==8) telo=5;
     if (naprav>0) x3=1;
     if (naprav<0) x3=-1;
     switch(telo)
       {
       case 0:
         Draw_Runer0(x1,x2,x3, 0);
         break;
       case 1:
         Draw_Runer1(x1,x2,x3, 0);
         break;
       case 2:
         Draw_Runer2(x1,x2,x3, 0);
         break;
       case 4:
         Draw_Runer4(x1,x2,x3, 0);
         break;
       case 5:
         Draw_Runer5(x1,x2,x3, 0);
         break;
       case 6:
         Draw_Runer6(x1,x2,x3, 0);
         break;
       case 7:
         Draw_Runer7(x1,x2,x3, 0);
         break;
       }
     LibPutDisp ();
     }

void Beg_Clear()
     {
     switch(telo)
       {
       case 0:
         Draw_Runer0(x1,x2,x3, 0);
         break;
       case 1:
         Draw_Runer1(x1,x2,x3, 0);
         break;
       case 2:
         Draw_Runer2(x1,x2,x3, 0);
         break;
       case 4:
         Draw_Runer4(x1,x2,x3, 0);
         break;
       case 5:
         Draw_Runer5(x1,x2,x3, 0);
         break;
       case 6:
         Draw_Runer6(x1,x2,x3, 0);
         break;
       case 7:
         Draw_Runer7(x1,x2,x3, 0);
         break;
       }
     LibPutDisp ();
}

void Beg_Runer_lox(int b1)
     {
     if (telo_l[b1]<3) telo_l[b1]= telo_l[b1]+1;
     if (telo_l[b1]==3) telo_l[b1]=0;
     if (telo_l[b1]>4) telo_l[b1]= telo_l[b1]+1;
     if (telo_l[b1]==8) telo_l[b1]=5;
     if (naprav_l[b1]>0) xl3[b1]=1;
     if (naprav_l[b1]<0) xl3[b1]=-1;
     switch(telo_l[b1])
       {
       case 0:
         Draw_Runer0(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 1:
         Draw_Runer1(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 2:
         Draw_Runer2(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 4:
         Draw_Runer4(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 5:
         Draw_Runer5(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 6:
         Draw_Runer6(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 7:
         Draw_Runer7(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       }
     LibPutDisp ();
     }

void Beg_Clear_lox(int b1)
     {
     switch(telo_l[b1])
       {
       case 0:
         Draw_Runer0(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 1:
         Draw_Runer1(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 2:
         Draw_Runer2(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 4:
         Draw_Runer4(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 5:
         Draw_Runer5(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 6:
         Draw_Runer6(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       case 7:
         Draw_Runer7(xl1[b1],xl2[b1],xl3[b1],1);
         break;
       }
     LibPutDisp ();
}

void PollEvent(TCHSTS far* tsts, byte event_mask)
  {
  union REGS reg;

  reg.x.ax = 0x0200 | event_mask;
  reg.x.di = FP_OFF(tsts);
  reg.x.es = FP_SEG(tsts);
  int86(0x50,&reg,&reg);
  }

void loadData(){
  FILE_BUF domfb;
  FILE_INFO domfinf;
  byte fs_entry;
  byte fm_entry;
  char filename[10];

  strcpy(filename,DATAFILENAME);
  if (LibSubEntrySave(filename,&fs_entry)==FALSE)
    LibErrorDisp(FlashStatus);

  LibGetAllEntry(filename,&fm_entry,&fs_entry);

  domfb.fsb_main_entry_=fm_entry;
  domfb.fsb_sub_entry_=fs_entry;
  domfb.fsb_scrt_info_=0x80;

  domfinf.fp=0xffff;
  domfinf.kind=FILE_KIND_BIN;

  if ((LibFileFindNext(&domfb,&domfinf,0x00)==TRUE) && (LibFileRead(&domfb,&domfinf)==TRUE)) {
      memcpy(&SaveData,&domfb.fbuf.bin.bin_buf,sizeof(SaveData));
  } else {
    LibErrorDisp(FlashStatus);
  }
}

void saveSettings(int x, int y, bool z) {
  FILE_BUF domfb;
  FILE_INFO domfinf;
  byte fs_entry;
  byte fm_entry;
  char filename[10];

  strcpy(filename,DATAFILENAME);
  if (LibSubEntrySave(filename,&fs_entry)==FALSE)
    LibErrorDisp(FlashStatus);
  LibGetAllEntry(filename,&fm_entry,&fs_entry);

  domfb.fsb_main_entry_=fm_entry;
  domfb.fsb_sub_entry_=fs_entry;
  domfb.fsb_scrt_info_=0x80;

  domfinf.fp=0xffff;
  domfinf.kind=FILE_KIND_BIN;

  if (LibFileFindNext(&domfb,&domfinf,0x00)==FALSE)
    domfinf.fp=0xffff;
  domfinf.kind=FILE_KIND_BIN;

  SaveData.a = x;
  SaveData.b = y;
  SaveData.sou = z;

  memcpy(&domfb.fbuf.bin.bin_buf,&SaveData,sizeof(SaveData));
  domfb.fbuf.bin.char_num=sizeof(SaveData);

  if (LibFileWrite(&domfb,&domfinf)!=TRUE) {
    LibErrorDisp(FlashStatus);
  }
}


