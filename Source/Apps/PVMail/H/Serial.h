
/******************************************************************************
*	[Title]		Comm header
*	[Model] 	PocketViewer
*	[Version]	Ver1.00
*
    Copyright (C) 2001 Fabio Fumi
	f_fumi@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/

#ifndef  SERIAL_H
#define  SERIAL_H


/*********** Defines ****************/
#define ACK	1 /* Acknowledge */
#define BRK	2 /* Break signal */
#define ERR 3 /* Unrecoverable error */
#define CHK 4 /* Checksum error */
#define NXT 5 /* Next block request */
#define STP 6 /* Download complete with no errors */
#define RCV 7 /* Sync byte to set up Receive mode */
#define SND 8 /* Sync byte to set up Send mode */
#define BUF 9 /* Send next buffer (part of a block) */

#define CREATE 10 /* Signal to/from Outlook to save next message sent */
#define DELETE 11 /* Signal from Outlook to remove next message (by ID) */
#define ALL_SENT 12 /* Signal to/from Outlook 'all messages have been sent' */
#define SAVE_ID 13 /* Signal to Outlook to receive and save msg ID */

#define SER_BUF_SIZE 512 /* serial buffer size */

/*********** Function Prototypes ***************/
bool SyncMail(void);


#endif /* SERIAL_H */
