/*
 * Module: Simple Battery gui
 * Author: S. Schildt
 * Date: 2001-01-19                                                    
 *
 * Multi Language support with help from Bernd Huebler
 *
 * 2003-01-15 J.St.
 * updated for PV-S1600 (version unchanged)
 *
 * Released to the public domain.
 *
 * Battstat standard GUI module
 * 
 */

#include "modeldef.h"

#include "Graphics.h"
#include "gui.h"
#include "utils.h"
#include "gui_msg.h"





    /* draw (VERY) simple battery */
void drawOnInit() {
	
    /*Headline */
    LibPutGraph( 0, 0, heading );
    	
    /*Border l&r*/
    LibLine(20,32,1,123,1);
    LibLine(59,32,1,123,1);
    
    /*Bottom*/
    LibLine(21,155,38,1,1);
    
    /* Lines l&r from kathode */
    LibLine(21,32,15,1,1);
    LibLine(44,32,15,1,1);
    
    /* side l&r of kathode */
    LibLine(35,27,1,5,1);
    LibLine(44,27,1,5,1);
    
    /*top of kathode*/
    LibLine(35,27,10,1,1);
 }
 
 
 
 void displayPower(int *AD) {
 
  /*Necessaray  variables */
  int n = 12;
  int volts = 0;	    /* Variable for voltage */
  byte tempNrStr[10];
  byte voltsStr[10];	     /* String representing voltage */
  int percent = 20; /* Charge in percent */
  byte percentStr[64];	     /* String representing int percent */ 
  bool light;
   	 
   /*Clear screen areas */    
    LibClrBox(21,33,38,122);
    LibClrBox(36,28,8,10);
    
    
    /*651 is point 0. The PV will stop working at AD=651*/
    n=*AD-651;
    if (n < 0) { n=0; }
    
    /*  percent. Factor should be 372 if 1023 is  100%. But we assume 927 (3.0V) = 100% */
    percent = n/2.76;
    /* just to be sure ... if something ugly happens */
    if (percent > 100) { percent=100; }
    if (percent <   0) { percent = 0; }
    
    /* n btw. 0...372
     BUT 276=128=100% 
     Trying a conversion, without losing to much 
    */
    /*n=n*100;
    n=n/216;*/
    
    n=ScaleAD(AD,128);
    
    volts = ADToVoltage(AD);
    
    /* Using new utility function */
    light = isLightOn();
    
    LibClrBox(69,70,91,70);
    if (volts > 250) { /*more than 2.5V*/
    	battMsg_OK();
    }     
    else if (volts > 230) { /*more than 2.3V, less than 2.5V*/
       battMsg_Low();
    }    
    else if (volts > 215 && light) {
    	battMsg_VeryLowAndLight();
    }
    else if (volts > 215) { /*more than 2.15V, less than 2.3V*/
       battMsg_VeryLowNoLight();
    }    
    else  { /*must be dead*/
       battMsg_Dead();
    }    

        
    /*Safety first! */
    if (n>128) { n=128; }
    if (n<0  ) { n=  0; }
    
    
    /* Preparing percentStr */
    nr2str(&percent,percentStr);
    strcat((char*)percentStr,"%");
    
    /* Preparing voltsStr */ 
    nr2str(&volts,tempNrStr);
    voltsStr[0]=tempNrStr[0];
    voltsStr[1]=',';
    voltsStr[2]=tempNrStr[1];
    voltsStr[3]=tempNrStr[2];
    voltsStr[4]=0;
    strcat((char*)voltsStr," V");       
       
    /* Clear & draw */
    LibClrBox(65,35,95,20);
    LibStringDsp(percentStr,70,40,160,IB_PFONT3);
    LibStringDsp(voltsStr,70,55,160,IB_PFONT3);
        
    /* draw inside of battery. n>123 means drawing positive pole */    
    if (n > 123) {
          /* 2003-01-15 J.St.
           * DRW_REPLACE does not work for PV-S1600 as for other PV models
           * so substitute by two separate calls */
      LibClrBox(36,28+(128-n),8,5-(128-n));
      LibReverse(36,28+(128-n),8,5-(128-n));
/*        LibBox (36,28+(128-n),8,5-(128-n),DRW_REPLACE); */
      LibMesh(36,28+(128-n),8,5-(128-n));
      n=123;
    }
    
        /* 2003-01-15 J.St.
         * DRW_REPLACE does not work for PV-S1600 as for other PV models
         * so substitute by two separate calls */
/*     LibBox(21,33+(123-n),38,n-1,DRW_REPLACE); */
    LibClrBox(21,33+(123-n),38,n-1);
    LibReverse(21,33+(123-n),38,n-1);
    LibMesh(21,33+(123-n),38,n-1);
 }
