/*
 * Program: BattStat
 * Author: S. Schildt
 * Date: 2001-01-12                                                    
 *
 * Released to the public domain.
 *
 * defines gui-drawing functions
 *
 */

#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED


/* This is called ONCE when the program is started*/
void drawOnInit();

/* This is called every 0,5 s */
void displayPower(int *AD);

#endif