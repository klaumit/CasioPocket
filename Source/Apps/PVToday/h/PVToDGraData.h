/******************************************************************************
*	[Title]		PVToday GraphicsData
*	[Model] 	PocketViewer
*	[Version]	Ver 2.10
*
*	Copyright (C) 2003 Jean-Michel GIRARD . All rights reserved.
*       http://pocketcasio.free.fr
******************************************************************************/
#ifdef __HITACHI__
#define FAR /* nothing */
#else
#define FAR far
#endif

/***Skin definition***********************************************************/
extern const byte FAR bmpCalendLate[];
extern const byte FAR bmpCalend[];
extern const byte FAR bmpMail[];
extern const byte FAR bmpOwner[];
extern const byte FAR bmpTask[];
extern const byte FAR bmpClock[];
extern const byte FAR *pbmpGraphs[];
extern const byte FAR bmpPower[];
extern const byte FAR bmpProgress[];
extern const byte FAR bmpRefresh[];

/*****************************************************************************/
extern const byte FAR bmpLeft[];
extern const byte FAR bmpRight[];
extern const byte FAR bmpButLeft[];
extern const byte FAR bmpButRight[];

extern void PVGraphDrawButton(int iX1,int iY1,int iX2,int iY2, char * szCaption);
extern void PVGraphPutMsgDlg(char * szCaption);
extern void PVGraphWaitForOK(void);




