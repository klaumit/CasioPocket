/******************************************************************************
*
*	[Title]		Common includes for PVMail modules
*	[Model] 	PocketViewer
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

#ifndef  COMMON_H
#define  COMMON_H

/*********** Defines & Macros**************/
/* General */
#define		LCD_X_DOT	160
#define		LCD_Y_DOT	160
#define     LCD_WIDTH   LCD_X_DOT
#define     LCD_HEIGHT  LCD_Y_DOT

/* System Menu */
#define 	PDWN_SYS_DTTM	PULLDOWN_SYSTEM | 0x0001	/* Set_date/time		*/
#define 	PDWN_SYS_SOUND	PULLDOWN_SYSTEM | 0x0002	/* Sound				*/
#define 	PDWN_SYS_FORMT	PULLDOWN_SYSTEM | 0x0004	/* Format				*/
#define 	PDWN_SYS_CAPCT	PULLDOWN_SYSTEM | 0x0008	/* Capacity				*/
#define 	PDWN_SYS_MMANE	PULLDOWN_SYSTEM | 0x0010	/* Memory_management	*/
#define 	PDWN_SYS_CNTRS	PULLDOWN_SYSTEM | 0x0020	/* Contrast				*/
#define 	PDWN_SYS_TPA	PULLDOWN_SYSTEM | 0x0040	/* Touch_Panel_Alignment*/

/* Edit Menu */
#define 	PDWN_EDT_CUT     PULLDOWN_EDIT  | 0x0001	/* Cut            		*/
#define 	PDWN_EDT_COPY    PULLDOWN_EDIT  | 0x0002	/* Copy  				*/
#define 	PDWN_EDT_PASTE   PULLDOWN_EDIT  | 0x0004	/* Paste  				*/
#define 	PDWN_EDT_SEARCH  PULLDOWN_EDIT  | 0x0008	/* Search               */
#define 	PDWN_EDT_DELETE  PULLDOWN_EDIT  | 0x0010	/* Delete item          */

/* General text-box defines */
#define TXTSCROLLBARWIDTH    14
#define NXTCHR               0xFE
#define TXTROWHEIGHT         9
#define TXTICON_WIDTH        14
#define TXT_GUIDE "\x0FESubject?"    \
                  "\x0FEFrom?"       \
                  "\x0FETo?"         \
                  "\x0FEBody?"       \
                  "\x0FEDate?"       \
                  "\x0FEFolder?"     \
                  "\x0FECc?"         \
                  "\x0FEBcc?"
#define TXT_GUIDE_SRCH  "\x0FESearch?"

/* Mail preview text-box */
#define TXTPREVIEW_ROWNUM    6     
#define TXTPREVIEW_XLEFT     14
#define TXTPREVIEW_YTOP      15
#define TXTPREVIEW_YHEIGHT   TXTPREVIEW_ROWNUM*TXTROWHEIGHT

/* Mail Display text-box */
#define TXTDISPLAY_ROWHEIGHT 9
#define TXTDISPLAY_ROWNUM    15     
#define TXTDISPLAY_XLEFT     14
#define TXTDISPLAY_YTOP      15
#define TXTDISPLAY_YHEIGHT   TXTDISPLAY_ROWNUM*TXTROWHEIGHT

/* Mail Edit text-box */
#define TXTEDIT_ROWHEIGHT 9
#define TXTEDIT_ROWNUM    7     
#define TXTEDIT_XLEFT     14
#define TXTEDIT_YTOP      15
#define TXTEDIT_YHEIGHT   TXTEDIT_ROWNUM*TXTROWHEIGHT

/* Search text-box */
#define TXTSEARCH_ROWHEIGHT 9
#define TXTSEARCH_ROWNUM    6     
#define TXTSEARCH_XLEFT     14
#define TXTSEARCH_YTOP      28
#define TXTSEARCH_YHEIGHT   TXTSEARCH_ROWNUM*TXTROWHEIGHT
#define SEARCH_SIZE         20 /* Size of search string */


/* Touch objects */
/* ATTENTION: code 0x9002 is reserved for text-box object */
#define     OBJ_USER            0x9000
#define		OBJ_IC_SYNC         (OBJ_USER+0x00)
#define		OBJ_IC_NEW          (OBJ_USER+0x03)
#define		OBJ_IC_VIEW 		(OBJ_USER+0x04)
#define		OBJ_IC_LIST	        (OBJ_USER+0x05)
#define		OBJ_IC_REPLY        (OBJ_USER+0x06)
#define		OBJ_IC_EDIT         (OBJ_USER+0x07)
#define		OBJ_IC_FORWARD      (OBJ_USER+0x08)
#define		OBJ_IC_CANCEL       (OBJ_USER+0x0A)
#define		OBJ_IC_SAVE         (OBJ_USER+0x0D)
#define     OBJ_IC_OK           (OBJ_USER+0x0F)
#define		OBJ_STRING			(OBJ_USER+0x10)
#define		OBJ_IC_DSP_NONE		(OBJ_USER+0x12)
#define		OBJ_IC_MSG_OK		(OBJ_USER+0x14)
#define     OBJ_LB              (OBJ_USER+0x16)
#define     OBJ_TAB_INBOX       (OBJ_USER+0x18)
#define     OBJ_TAB_OUTBOX      (OBJ_USER+0x1A)
#define     OBJ_IC_SRCH_BYITEM  (OBJ_USER+0x1C)
#define     OBJ_IC_SRCH_ALL     (OBJ_USER+0x1E)
#define		OBJ_IC_BUTT_UP      (OBJ_USER+0x20)
#define		OBJ_IC_BUTT_DOWN    (OBJ_USER+0x21)

/* Types of messages to create */
#define     NEW_EMPTY   1
#define     NEW_FORWARD 2
#define     NEW_REPLY   3

/* Touch area */
#define TCHTBL_BTN(x1,y1,x2,y2,obj)	\
	x1,y1,x2,y2,			        \
	ACT_ICON,			            \
	obj,0

#define TCHTBL_END			\
	0,0,0,0,			    \
	ACT_NONE,			    \
	OBJ_END,0

/* Common constants */
#define     MAX_MAIL    2000  /* Maximum number of mail messages that can be loaded */
			   
#define     MAIL_INBOX  0x01
#define     MAIL_OUTBOX 0x02

#define     SRCH_ITEM   0x10
#define     SRCH_ALL    0x20

#endif /* COMMON_H */
