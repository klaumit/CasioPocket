/*
 * Program: BattStat
 * Author: S. Schildt
 * Date: 2001-01-12                                                    
 *
 * Released to the public domain.
 *
 * defines utility functions
 *
 */

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/* Converts a given AD Value to voltage */
int ADToVoltage(int *AD);

/* Scales the AD Value (for example for progress bars*/
int ScaleAD(int* AD,int target);

/* Builds strings. More general than the lib function. Take care of the digits itself */
void nr2str(int* number,byte target[]);

/* Is the backlight on? */
bool isLightOn();

#endif