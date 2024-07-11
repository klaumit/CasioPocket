#ifndef STDPROCSH
#define STDPROCSH

#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <gdefs.h>

#include <fm.h> /* constant FILE_SYSTEM_ERROR */


/* Standard procedures that can be used in every program */

/* **********************************************************************************************
   Definitions of data
********************************************************************************************** */

extern TCHTBL far TchCANCEL[];
 
extern T_ICON far IP_CANCEL;


extern byte buffer[2048];

/* **********************************************************************************************
   Definitions of procedures
********************************************************************************************** */

/* Converts a dword to a string with leading zeros 
  Parameters :

  dword value       value that will be converted into a string
  byte *number      pointer to an array that should have enough 
                    room to store the dword-string [10+1]
*/
void dwordtostring(dword value,byte *number);

/* uses the standart keyboard to supply the input of a string 
  Parameters :
  
  byte *string      pointer to an array that should have enough
                    room to store the string (length+1)
  byte length       maximum characters that the user can type in
  byte *subtitle    a zero-terminated character string holding a subtitle
                    the subtitle will be shown right about the input box
  byte *title       a zero-terminated character string holding a title
                    the title will be shown in a bigger font and centered
                    in the upper region
*/
void Input(byte *string, byte length, byte *subtitle, byte *title);

/* draws a standart selection window with given parameters 
  Parameters :
  
  byte xpos         x-coordinate of upper left corner
  byte ypos         y-coordinate of upper left corner
  byte xsize        size of the box in x-direction
  byte *strings     pointer to an array of zero-terminated strings
  byte anz          number of strings in the array (*strings)
  byte maxstrlen    max. number of characters in the strings-array
  byte preselection indicates which entry is selected when the box is shows up

*/
byte SelectionWindow(byte xpos, byte ypos, byte xsize, byte *strings,
                     byte anz, byte maxstrlen, byte preselection);

/* redirects value with a given list in *redirectvalues
  
  Parameters :
  
  word value       the value that will be redirected
  word *redirectvalues  pointer to a list of redirection-values
  byte anz         number of values in the list
*/
word RedirectValues(word value,word *redirectvalues, byte anz);

/* Shows a message box with ok and cancel button 

  Parameters :

  byte *string     points to the message that shows up
  
  Return values :
  
  bool             indicates wheter the user has pressed the 
                   ok(TRUE) or the cancel(FALSE) button
*/
bool MessageBox(byte *string);

/* aborts program with a message 

  Parameters :
  
  byte errcode     see constants
  byte errno       see constants
  word addinfo1    a value displayed to assist debuging
  word addinfo2    a value displayed to assist debuging
  word addinfo3    a value displayed to assist debuging
*/
void MessageOK(byte errcode, byte errno, word addinfo1, word addinfo2, word addinfo3);

#endif