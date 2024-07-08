/******************************************************************************
    Copyright (C) 1998 CASIO COMPUTER CO.,LTD. All rights reserved.
******************************************************************************/
#ifndef LISTBOX_H
#define LISTBOX_H

/* Info area (on top of the list) */
#define L_X	0
#define L_Y 20
#define L_W LCD_WIDTH
#define L_ROWS 2
#define L_H (9*L_ROWS + L_ROWS + 1)

/* List box appearence */
#define LB_X	0
#define LB_W	160
#define LB_Y	(L_Y+L_H+1)
#define LB_IH	9		    /* item height */
#define LB_IPP	13 - L_ROWS	/* items per page */
#define LB_LT	lbltDot		/* line type */

/* Touch areas */
#define TCHTBL_LB(x,y,w,ih,ipp,typ,obj)			\
	x,y,						\
	(x)+(w)-1,					\
	(y) + (	((typ) == 0) ?				\
			((ih) * (ipp) + 1) :		\
			((ih) * (ipp) + (ipp)) ),	\
	ACT_SCR_BAR,obj,0

#define TCHTBL_LB2(x,yb,w,ih,ipp,typ,obj)		\
	x,(yb) -					\
	  ( ((typ) == 0) ?				\
		((ih) * (ipp) + 1) :			\
		((ih) * (ipp) + (ipp)) ),		\
	(x)+(w)-1,					\
	yb,ACT_SCR_BAR,obj,0

#define LB_UP		0
#define LB_DOWN		1
#define LB_MOVE		2
#define LB_SELECT	0x80

typedef enum {lbltNone=0,lbltSolid,lbltDot} TLBLineType;
typedef void (*TDrawLBItem)(const struct tListBox *,const struct tLBItemInfo *);

typedef struct tLBItemInfo {
	int	idx; /* ATTN: idx can be greater than count.
			Just clear the area if idx is not in the range [0,count-1]. */
	int	x,y;
	byte reverse;
} TLBItemInfo;

typedef struct tListBox {
	void far *user;
	int	x,y,w,h;
	byte	ItemHeight;
	TLBLineType LineType;
	int	CurLine;
	T_SCR_POS sc;
	int	w2;
	TDrawLBItem drawItem,toggleItem;
} TListBox, *PListBox;


void setLBItemsCount(TListBox *l, int count);
void setLBCurrentItem(TListBox *l, int curidx);
int  LBClicked(TCHSTS *tsts, TListBox *l);
int  scrollListBox(TListBox *l, int dir); /* 0 = up, 1 = down */
void initListBox(TListBox *l, byte ItemHeight, word ItemsPerPage, TLBLineType lt);
void drawListBox(TListBox *l);

#define getLBCurIdx(l) ((l)->CurLine + (l)->sc.pos)

#endif  /* LISTBOX_H */
