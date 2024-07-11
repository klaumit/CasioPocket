#include <includes.h>


void main ()
{
	int input_back;
        int i;
        bool neucheck;
        
        
        ShowLogo(TRUE);
        LoadData();
         
        neucheck=FALSE;     
        input_back=4;
        do
        {
          switch(input_back)
          {
            case 4:		/* Cancel */
              for( i = 0; i<5; i++ )
    	      {
                sprintf(&werte_str[i][0],"%.2f\n",FileInfo.ini_posten[i]);
                check[i]=FALSE;             
              }              
              summe=0.0;
              neucheck=FALSE;
              DrawHead();
              input_back=Anzeige(TRUE);
              break;
            case 1:		/* Neueingabe */
              summe=0.0;
              neucheck=TRUE;
              DrawHeadNeu();
              input_back=Anzeige(FALSE); 	/* keine oberen Icons abfragen */
              break;
            case 2:		/* Statistik */
              neucheck=FALSE;
              DrawStat();
              input_back=5;
              break;
            case 5:		/* Normale Eingabe */
              neucheck=FALSE;
              DrawHead();
              input_back=Anzeige(TRUE);
              break;            
          }          
        } while( input_back!=3);
        
        /* OK und Speichern ... */
        ShowLogo(FALSE);
        SaveDataDel(neucheck);
        
        LibJumpMenu();
        /* Springe ins Menu zurück, ohne diesen Rücksprung stürzt das OS ab */        
}
