/*
 * Module: Battstat international Messages
 * Author: S. Schildt
 * Date: 2001-01-19                                                    
 *
 * Multi Language support with help from Bernd Huebler
 *
 * Released to the public domain.
 *
 * 
 */
 
#include <stdrom.h>
#include "define.h"
#include "libc.h"
#include "L_DEFINE.H"
#include "L_LIBC.H"
 
void battMsg_OK() {
	
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp("Batterie OK.",70,70,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp("Batterie OK.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp("Bater\222as OK.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp("Batteries OK.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp("Batteries OK.",70,70,160,IB_PFONT1);
      				break;
	}
}

void battMsg_Low() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp("Batterie wird ",70,70,160,IB_PFONT1);
      				 LibStringDsp("schw\260cher.",70,79,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp("Batterie basse.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp("Bater\222as bajas.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp("Batteries faibles.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp("Batteries low.",70,70,160,IB_PFONT1);
      				break;
	}
}

void battMsg_VeryLowAndLight() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp("Batterien sind",70,70,160,IB_PFONT1);
       				LibStringDsp("ziemlich schwach. ",70,79,160,IB_PFONT1);
       				LibStringDsp("Machen Sie besser",70,88,160,IB_PFONT1);
       				LibStringDsp("das Licht aus.",70,97,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp("Le batterie sono",70,70,160,IB_PFONT1);
      				  LibStringDsp("abbastanza",70,79,160,IB_PFONT1);
      				  LibStringDsp("scariche, meglio ",70,88,160,IB_PFONT1);
      				  LibStringDsp("spegnere la ",70,97,160,IB_PFONT1);
      				  LibStringDsp("retroilluminazione.",70,106,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp("Bater\222as bajas.",70,70,160,IB_PFONT1);
       				LibStringDsp("Deber\222a apagar ",70,79,160,IB_PFONT1);
       				LibStringDsp("la luz.",70,88,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp("Batteries tr\226s",70,70,160,IB_PFONT1);
      				  LibStringDsp("faibles. ",70,79,160,IB_PFONT1);
      				  LibStringDsp("D\221sactivez la",70,88,160,IB_PFONT1);
      				  LibStringDsp("lumi\226re. ",70,97,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp("Batteries are quite",70,70,160,IB_PFONT1);
       				LibStringDsp("low. ",70,79,160,IB_PFONT1);
       				LibStringDsp("Better turn off",70,88,160,IB_PFONT1);
       				LibStringDsp("the light.",70,97,160,IB_PFONT1);
      				break;
	}
}

void battMsg_VeryLowNoLight() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH:LibStringDsp("Batterien versagen",70,70,160,IB_PFONT1);
   				LibStringDsp("jeden Moment.",70,79,160,IB_PFONT1);
   				LibStringDsp("Vorsicht mit dem",70,88,160,IB_PFONT1);
       				LibStringDsp("Licht!",70,97,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp("Batterie quasi",70,70,160,IB_PFONT1);
      				  LibStringDsp("scariche.",70,79,160,IB_PFONT1);
   				  LibStringDsp("Evitare di usare",70,88,160,IB_PFONT1);
       				  LibStringDsp("l'illuminazione!",70,97,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp("Las bater\222as se ",70,70,160,IB_PFONT1);
   				 LibStringDsp("agotar\220n en breve.",70,79,160,IB_PFONT1);
   				 LibStringDsp("Procure no usar",70,88,160,IB_PFONT1);
       				 LibStringDsp("la luz.",70,97,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp("Batteries mortes",70,70,160,IB_PFONT1);
      				  LibStringDsp("sous peu!",70,79,160,IB_PFONT1);
   				  LibStringDsp("Prudence avec",70,88,160,IB_PFONT1);
       				  LibStringDsp("la lumi\226re.",70,97,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp("Batteries will fail",70,70,160,IB_PFONT1);
   				LibStringDsp("any minute!",70,79,160,IB_PFONT1);
   				LibStringDsp("Be careful with",70,88,160,IB_PFONT1);
       				LibStringDsp("light.",70,97,160,IB_PFONT1);
      				break;
	}
}

void battMsg_Dead() {
	switch (LibGetLang()) {
      		case IB_DEUTSCH: LibStringDsp("Batterien ersch\263pft.",70,70,160,IB_PFONT1);
      				 break;
      		case IB_ITALIANO: LibStringDsp("Batterie scariche.",70,70,160,IB_PFONT1);
      				 break;
		case IB_ESPANOL: LibStringDsp("Bater\222as agotadas.",70,70,160,IB_PFONT1);
      				 break;
  		case IB_FRANCAIS: LibStringDsp("Batteries mortes.",70,70,160,IB_PFONT1);
      				 break;
      		default:	LibStringDsp("Batteries down.",70,70,160,IB_PFONT1);
      				break;
	}
}