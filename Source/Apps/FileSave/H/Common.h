/******************************************************************************
*
*	[Title]		Common includes fro FileSave modules
*	[Model] 	PocketViewer
*	[Version]	Ver1.00
*
******************************************************************************/

#ifndef  COMMON_H
#define  COMMON_H

/*********** Defines **************/
#define		LCD_X_DOT	160
#define		LCD_Y_DOT	160
#define     LCD_WIDTH   LCD_X_DOT
#define     LCD_HEIGHT  LCD_Y_DOT

/* Touch objects */
#define     OBJ_USER            0x9000
#define		OBJ_IC_RECEIVE		(OBJ_USER+0x00)
#define		OBJ_IC_SEND 	   	(OBJ_USER+0x02)
#define		OBJ_IC_DELETE		(OBJ_USER+0x04)
#define		OBJ_STRING			(OBJ_USER+0x06)
#define		OBJ_IC_DSP_NONE		(OBJ_USER+0x0A)
#define		OBJ_IC_MSG_OK		(OBJ_USER+0x10)
#define     OBJ_LB              (OBJ_USER+0x11)

/* Touch area */
#define TCHTBL_BTN(x1,y1,x2,y2,obj)	\
	x1,y1,x2,y2,			        \
	ACT_ICON,			            \
	obj,0

#define TCHTBL_END			\
	0,0,0,0,			\
	ACT_NONE,			\
	OBJ_END,0

/* Common constants */
#define     MAX_FILES    20 /* Maximum number of files that can be loaded */

#endif /* COMMON_H */
