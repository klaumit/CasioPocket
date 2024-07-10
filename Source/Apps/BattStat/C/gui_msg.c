/*
 * Module: Battstat international Messages
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
 * 
 */
 
#include "modeldef.h"
 
void battMsg_OK() {
	
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp((byte*)"Batterie OK.",70,70,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp((byte*)"Batterie OK.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp((byte*)"Bater\222as OK.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp((byte*)"Batteries OK.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp((byte*)"Batteries OK.",70,70,160,IB_PFONT1);
      				break;
	}
}

void battMsg_Low() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp((byte*)"Batterie wird ",70,70,160,IB_PFONT1);
      				 LibStringDsp((byte*)"schw\260cher.",70,79,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp((byte*)"Batterie basse.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp((byte*)"Bater\222as bajas.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp((byte*)"Batteries faibles.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp((byte*)"Batteries low.",70,70,160,IB_PFONT1);
      				break;
	}
}

void battMsg_VeryLowAndLight() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp((byte*)"Batterien sind",70,70,160,IB_PFONT1);
       				LibStringDsp((byte*)"ziemlich schwach. ",70,79,160,IB_PFONT1);
       				LibStringDsp((byte*)"Machen Sie besser",70,88,160,IB_PFONT1);
       				LibStringDsp((byte*)"das Licht aus.",70,97,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp((byte*)"Le batterie sono",70,70,160,IB_PFONT1);
      				  LibStringDsp((byte*)"abbastanza",70,79,160,IB_PFONT1);
      				  LibStringDsp((byte*)"scariche, meglio ",70,88,160,IB_PFONT1);
      				  LibStringDsp((byte*)"spegnere la ",70,97,160,IB_PFONT1);
      				  LibStringDsp((byte*)"retroilluminazione.",70,106,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp((byte*)"Bater\222as bajas.",70,70,160,IB_PFONT1);
       				LibStringDsp((byte*)"Deber\222a apagar ",70,79,160,IB_PFONT1);
       				LibStringDsp((byte*)"la luz.",70,88,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp((byte*)"Batteries tr\226s",70,70,160,IB_PFONT1);
      				  LibStringDsp((byte*)"faibles. ",70,79,160,IB_PFONT1);
      				  LibStringDsp((byte*)"D\221sactivez la",70,88,160,IB_PFONT1);
      				  LibStringDsp((byte*)"lumi\226re. ",70,97,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp((byte*)"Batteries are quite",70,70,160,IB_PFONT1);
       				LibStringDsp((byte*)"low. ",70,79,160,IB_PFONT1);
       				LibStringDsp((byte*)"Better turn off",70,88,160,IB_PFONT1);
       				LibStringDsp((byte*)"the light.",70,97,160,IB_PFONT1);
      				break;
	}
}

void battMsg_VeryLowNoLight() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH:LibStringDsp((byte*)"Batterien versagen",70,70,160,IB_PFONT1);
   				LibStringDsp((byte*)"jeden Moment.",70,79,160,IB_PFONT1);
   				LibStringDsp((byte*)"Vorsicht mit dem",70,88,160,IB_PFONT1);
       				LibStringDsp((byte*)"Licht!",70,97,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp((byte*)"Batterie quasi",70,70,160,IB_PFONT1);
      				  LibStringDsp((byte*)"scariche.",70,79,160,IB_PFONT1);
   				  LibStringDsp((byte*)"Evitare di usare",70,88,160,IB_PFONT1);
       				  LibStringDsp((byte*)"l'illuminazione!",70,97,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp((byte*)"Las bater\222as se ",70,70,160,IB_PFONT1);
   				 LibStringDsp((byte*)"agotar\220n en breve.",70,79,160,IB_PFONT1);
   				 LibStringDsp((byte*)"Procure no usar",70,88,160,IB_PFONT1);
       				 LibStringDsp((byte*)"la luz.",70,97,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp((byte*)"Batteries mortes",70,70,160,IB_PFONT1);
      				  LibStringDsp((byte*)"sous peu!",70,79,160,IB_PFONT1);
   				  LibStringDsp((byte*)"Prudence avec",70,88,160,IB_PFONT1);
       				  LibStringDsp((byte*)"la lumi\226re.",70,97,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp((byte*)"Batteries will fail",70,70,160,IB_PFONT1);
   				LibStringDsp((byte*)"any minute!",70,79,160,IB_PFONT1);
   				LibStringDsp((byte*)"Be careful with",70,88,160,IB_PFONT1);
       				LibStringDsp((byte*)"light.",70,97,160,IB_PFONT1);
      				break;
	}
}

void battMsg_Dead() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp((byte*)"Batterien ersch\263pft.",70,70,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp((byte*)"Batterie scariche.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp((byte*)"Bater\222as agotadas.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp((byte*)"Batteries mortes.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp((byte*)"Batteries down.",70,70,160,IB_PFONT1);
      				break;
	}
}
