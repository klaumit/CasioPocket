/******************************************************************************
*	
*	[Title]	    List Box module 
*               (Copied and changed from the Kino program,
*               by Wittawatt Wamyong)
* 
*	[Model] 	PocketViewer
*	[Version]	Ver1.00

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

*
******************************************************************************/

#include <stdrom.h>
#include "define.h"
#include "libc.h"
#include "l_libc.h"


#include "Common.h"
#include "ListBox.h"

extern int CurFile;
extern int FileNum;

extern int   CurMail;
extern int   MailNum;

/**********************************************
 * List box
**********************************************/
static void donothing() {};

/*
 * in: ItemHeight,ItemsPerPage,LineType
 * out: l->h
 */
void initListBox(TListBox *l, byte ItemHeight, word ItemsPerPage, TLBLineType lt)
{

	l->ItemHeight = ItemHeight;
	l->sc.dsp = ItemsPerPage;
	l->LineType = lt;
	l->x = LB_X; l->y = 13; /* Perché non LB_Y? */
	l->w = LCD_WIDTH;
	l->h = ItemHeight * ItemsPerPage + 2;
	if (lt != lbltNone)
		l->h += ItemsPerPage - 1;
	l->CurLine = 0;
	l->sc.pos = 0;
	l->sc.vol = 0;
	l->drawItem = (TDrawLBItem) donothing;
	l->toggleItem = (TDrawLBItem) donothing;
}

static void updateLBScroll(TListBox *l)
{
	if (l->sc.pos == 0 && l->CurLine == 0)
		LibScrollArrowPrint(l->sc,SCR_UP_MASK);
	if ((l->sc.vol == 0) || getLBCurIdx(l) == (l->sc.vol - 1))
		LibScrollArrowPrint(l->sc,SCR_DWN_MASK);
	else if (getLBCurIdx(l) >= l->sc.vol) {
		/* Should not happen!! */
		if (l->sc.vol <= l->sc.dsp) {
			l->sc.pos = 0;
			l->CurLine = l->sc.vol - 1;
		} else {
			l->sc.pos = l->sc.vol - l->sc.dsp;
			l->CurLine = l->sc.dsp - 1;
		}
		LibScrollArrowPrint(l->sc,SCR_DWN_MASK);
	}
}

static void drawLBLine(TLBLineType lt,int x,int y,int w)
{
	switch (lt) {
	case lbltSolid:
		LibLine(x,y,w,1,1);
		break;

	case lbltDot:
		LibMeshLine(x,y,w,1);
		break;

	default:
		break;
	}
}

static void drawAllLBItems(const TListBox *l)
{
	int i,ystep;
	TLBItemInfo li;

	li.x = l->x + 1;
	li.y = l->y + 1;
	ystep = l->ItemHeight;
	if (l->LineType != lbltNone)
		ystep++;
	li.idx = l->sc.pos;
	for (i=0; i < l->sc.dsp-1; i++) {
		drawLBLine(l->LineType,li.x,li.y + l->ItemHeight,l->w2);
		li.reverse = (i == l->CurLine);
		(*l->drawItem)(l,&li);
		li.y += ystep;
		li.idx++;
	}
	li.reverse = (i == l->CurLine);
	(*l->drawItem)(l,&li);
}


void drawListBox(TListBox *l)
{
	l->w2 = l->w - SCR_XSIZE - 5;
	l->sc.x = l->x + l->w - SCR_XSIZE - 2;
	l->sc.y = l->y + 3;
	l->sc.size = l->h - 6;

	LibClrBox(l->x,l->y,l->w,l->h);
 /* LibLine(l->x + 1, l->y,            l->w - 2, 1, 1); TOP LINE REMOVED FOR TABS */
	LibLine(l->x + 1, l->y + l->h - 1, l->w - 2, 1, 1);
	LibLine(l->x,            l->y    , 1, l->h    , 1);
	LibLine(l->x + l->w - 1, l->y    , 1, l->h    , 1);
	LibLine(l->sc.x - 2, l->y, 1, l->h, 1);
	LibScrollPrint(l->sc);
	if (l->sc.vol <= l->sc.dsp)
		/* draw black arrow then gray it in updateLBScroll() */
		LibScrollArrowPrint(l->sc,SCR_NO_MASK);
	updateLBScroll(l);
	drawAllLBItems(l);
	
	LibPutGraph(0, l->y-9, l->tab);
}

static int _scrollListBox(TListBox *l, int dir)
{
	int temp;
	int ret = 0;
	TLBItemInfo li;

	li.x = l->x + 1;
	switch(dir) {
	case 0:
		li.idx = getLBCurIdx(l);
		if (li.idx <= 0) {
			ret = -1;
			break;
		}
		li.y = l->y + l->CurLine * l->ItemHeight + 1;
		if (l->LineType != lbltNone) {
			li.y += l->CurLine;
			temp = l->ItemHeight + 1;
		} else
			temp = l->ItemHeight;
		li.reverse = 0;
		(*l->toggleItem)(l,&li);
		li.reverse = 1;

		if (l->CurLine > 0) {
			l->CurLine--;
			li.idx--;
			li.y -= temp;
			(*l->toggleItem)(l,&li);
			LibScrollArrowPrint(l->sc,SCR_NO_MASK);
		} else {
			l->sc.pos--;
			li.idx--;
			li.y = l->y+1;
			LibGrpDwn(li.x,l->y+1,l->w2,l->h-2,temp);
			drawLBLine(l->LineType,li.x,li.y + l->ItemHeight,l->w2);
			(*l->drawItem)(l,&li);
			LibScrollPrint(l->sc);
		}
		break;

	case 1:
		li.idx = getLBCurIdx(l);
		if (li.idx >= (l->sc.vol-1)) {
			ret = -1;
			break;
		}
		li.y = l->y + l->CurLine * l->ItemHeight + 1;
		if (l->LineType != lbltNone) {
			li.y += l->CurLine;
			temp = l->ItemHeight + 1;
		} else
			temp = l->ItemHeight;
		li.reverse = 0;
		(*l->toggleItem)(l,&li);
		li.reverse = 1;

		if (l->CurLine < l->sc.dsp-1) {
			l->CurLine++;
			li.idx++;
			li.y += temp;
			(*l->toggleItem)(l,&li);
			LibScrollArrowPrint(l->sc,SCR_NO_MASK);
		} else {
			l->sc.pos++;
			li.idx++;
			li.y = l->y + l->h - l->ItemHeight - 1;
			LibGrpUp(li.x,l->y+1,l->w2,l->h-2,temp);
			drawLBLine(l->LineType,li.x,li.y - 1,l->w2);
			(*l->drawItem)(l,&li);
			LibScrollPrint(l->sc);
		}
		break;

	case 2:
		drawAllLBItems(l);
		LibScrollArrowPrint(l->sc,SCR_NO_MASK);
		break;

	case 4:
		break;

	default:
		ret = -1;
	}

	if (ret != -1) {
		updateLBScroll(l);
		LibPutDisp();
	}
	return (ret);
}

int scrollListBox(TListBox *l, int dir)
{
	if (dir != LB_UP && dir != LB_DOWN)
        	return (-1);
	if ((dir == LB_UP && l->CurLine == 0 && l->sc.pos == 0) ||
	    (dir == LB_DOWN && getLBCurIdx(l) >= (l->sc.vol-1)))
		return (-1);
        return (_scrollListBox(l,dir));
}

static int calCurItemTop(const TListBox *l)
{
	int temp;

	temp = l->y + l->CurLine * l->ItemHeight + 1;
	if (l->LineType != lbltNone)
		temp += l->CurLine;
	return (temp);
}

static void setLBCurrentLine(TListBox *l, int curline)
{
	TLBItemInfo li;

	li.idx = getLBCurIdx(l);
	li.x = l->x+1;
	li.y = calCurItemTop(l);
	li.reverse = 0;
	(*l->toggleItem)(l,&li);

	l->CurLine = curline;
	li.idx = getLBCurIdx(l);
	li.y = calCurItemTop(l);
	li.reverse = 1;
	(*l->toggleItem)(l,&li);

	LibScrollArrowPrint(l->sc,SCR_NO_MASK);
	updateLBScroll(l);
}

static void flashLBCurLine(TListBox *l)
{
	TLBItemInfo li;

	li.idx = getLBCurIdx(l);
	li.x = l->x+1;
	li.y = calCurItemTop(l);
	li.reverse = 0;
	(*l->toggleItem)(l,&li);
	LibPutDisp();
	li.reverse = 1;
	(*l->toggleItem)(l,&li);
	LibPutDisp();
}

int LBClicked(TCHSTS *tsts, TListBox *l)
{
	int click = -1;
	int line,ih;

	if ((tsts->act & ACT_SCR_BAR) != 0 && tsts->x >= (l->sc.x-2)) {
		click = LibScrPosCheck(*tsts,l->sc);
		if ((click == 0 && l->CurLine == 0 && l->sc.pos == 0) ||
		    (click == 1 && getLBCurIdx(l) >= (l->sc.vol-1)))
		     return (click);
		click = LibScrollClick(tsts,&l->sc);
		_scrollListBox(l,click);
	} else if (tsts->act & ACT_MAKE) {
		ih = l->ItemHeight;
		if (l->LineType != lbltNone)  ih++;
		line = (tsts->y - l->y - 1) / ih;

		if ((line + l->sc.pos) < l->sc.vol) {
			if (line == l->CurLine) {
				click = LB_SELECT;
				flashLBCurLine(l);
			} else {
				click = LB_MOVE;
				setLBCurrentLine(l,line);
				LibPutDisp();
			}
		}
		LibTchInit(); /* FIX: clear event queue. */
	}
	return (click);
}

void setLBItemsCount(TListBox *l, int count)
{
	l->sc.vol = count;
	l->sc.pos = 0;
	l->CurLine = 0;
	drawListBox(l);
}

static void fixLBIndex(TListBox *l, int idx)
{
	if (l->sc.vol < l->sc.dsp) {
		    l->CurLine = idx;
            l->sc.pos = 0;
        } else if ((idx + l->sc.dsp) <= l->sc.vol) {
        	l->CurLine = 0;
            l->sc.pos = idx;
        } else {
        	l->sc.pos = l->sc.vol - l->sc.dsp;
            l->CurLine = idx - l->sc.pos;
        }
}

void setLBCurrentItem(TListBox *l, int curidx)
{
	int temp;

	temp = curidx - l->sc.pos;
	if (temp >= 0 && temp < l->sc.dsp) {
		setLBCurrentLine(l,temp);
	} else {
        fixLBIndex(l,curidx);
		drawAllLBItems(l);
		LibScrollPrint(l->sc);
		if (l->sc.vol <= l->sc.dsp)
			LibScrollArrowPrint(l->sc,SCR_NO_MASK);
		updateLBScroll(l);
	}
}


