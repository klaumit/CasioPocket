/* ataxx.c: the classic Ataxx game for PALM-PILOT
 *
 * (c) 1997 Valavan Manohararajah
 *
 * converted to PV 2004-06-22 J. Steingraeber
 * added block support 2004-06-27 J. Steingraeber
 */

#include "modeldef.h"

/*
 * UI definitions
 */

#include "ataxx.h"

void DrawMainForm();
void FrmGotoForm(int FrmID);
static int ActiveFrmID;
static void SelectBlockConfig();

const TCHTBL FAR TchButtons [] = {
  {3, 132, 3+34, 132+12, ACT_ICON, BlockButton, 0x0000},
  {3, 147, 3+24, 147+12, ACT_ICON, NewButton, 0x0000},
  {27+4, 147, 27+4+34, 147+12, ACT_ICON, SwitchButton, 0x0000},
  {65+4, 147, 65+4+26, 147+12, ACT_ICON, PassButton, 0x0000},
  {95+4, 147, 95+4+25, 147+12, ACT_ICON, HelpButton, 0x0000},
  {124+4, 147, 124+4+30, 147+12, ACT_ICON, AboutButton, 0x0000},
  {121, 71, 121+9, 71+12, ACT_ICON, Level1Button, 0x0000},
  {130+1, 71, 130+1+9, 71+12, ACT_ICON, Level2Button, 0x0000},
  {140+1, 71, 140+1+9, 71+12, ACT_ICON, Level3Button, 0x0000},
  {0,0,0,0,0,0,0}
};


/*----------------------------Ataxx Engine-------------------------------*/

#define max_ply					4
#define max_moves				256
#define infinity				100

#if !defined(max)
#define max(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#if !defined(abs)
#define abs(a)					(((a) >= 0) ? (a) : (-(a)))
#endif

#define distance(fr,fc,tr,tc)	(max(abs((fr)-(tr)),abs((fc)-(tc))))

enum {
  black,white,blank,block
};

enum {
  move_type_pass,move_type_grow,move_type_jump
};

/* typedef struct move_type { */
/*   unsigned int f:7; */
/*   unsigned int t:7; */
/*   unsigned int type:2; */
/* } move_t; */

typedef struct move_type {
  unsigned int f;
  unsigned int t;
  unsigned int type;
} move_t;

/*
 * block configuration predefined sets
 * taken from Windows 95 Attax by C.Rond~ao
 *
 *  . d e 4 e d .
 *  a g f 5 f g a
 *  b c h 6 h c b
 *  1 2 3 x 3 2 1
 *  b c h 6 h c b
 *  a g f 5 f g a
 *  . d e 4 e d .
 *
 */
const char FAR * blocksets[] = {
  "none",
  "random",
  "h",
  "cf",
  "36",
  "fgh",
  "3h",
  "6cf",
  "2h",
  "6g",
  "bf",
  "6bh",
  "x",
  "2",
  "3",
  "a",
  "b",
  "c",
  "g",
  "16",
  "23",
  "26",
  "1d",
  "2g",
  "3c",
  "236",
  "ac",
  "ch",
  "gh",
  "23c",
  "35b",
  "1bg",
  "1bh",
  "2ah",
  "2bc",
  "3bg",
  "3ch",
  "3fg",
  "123456",
  "12ef",
  "23bg",
  "acf",
  "bcg",
  "3dgx",
  "6bcg",
  "12cdx",
  "34acgh",
  "35abef",
  "14cfgx",
  "123afg",
  "1235afgx",
  "" /* end of list entry */
};

static int		blockconf;
static char		BlockConfigString[24];
static int		allow_block_config;

static int		board[49];
static int		turn;

static int		num_pieces[3];

static move_t	best_mv;

static move_t	mv_lst_heap[max_ply][max_moves];
static int		mv_score_heap[max_ply][max_moves];

static int		save_board[max_ply][8];
static int		save_num[max_ply];

static unsigned long	node_cnt;

static int		pregen_grow_tbl[49][8];
static int		pregen_grow_num[49];
static int		pregen_jump_tbl[49][16];
static int		pregen_jump_num[49];

/*
 * pregen() precomputes a set of tables used for fast move generation.
 */

#define pregen_gen(tbl,num,delta_r,delta_c)		\
	tr=r+(delta_r);								\
	tc=c+(delta_c);								\
	if(tr>=0 && tr<7 && tc>=0 && tc<7) {		\
		tbl[sq][num[sq]]=tr*7+tc;				\
		num[sq]++;								\
	}

static void pregen(void)
{
  int r,c,tr,tc,sq=0;

  for(r=0;r<7;r++) {
    for(c=0;c<7;c++,sq++) {
      pregen_grow_num[sq]=0;
      pregen_jump_num[sq]=0;

      pregen_gen(pregen_grow_tbl,pregen_grow_num,-1, 0);
      pregen_gen(pregen_grow_tbl,pregen_grow_num,-1, 1);
      pregen_gen(pregen_grow_tbl,pregen_grow_num, 0, 1);
      pregen_gen(pregen_grow_tbl,pregen_grow_num, 1, 1);
      pregen_gen(pregen_grow_tbl,pregen_grow_num, 1, 0);
      pregen_gen(pregen_grow_tbl,pregen_grow_num, 1,-1);
      pregen_gen(pregen_grow_tbl,pregen_grow_num, 0,-1);
      pregen_gen(pregen_grow_tbl,pregen_grow_num,-1,-1);

      pregen_gen(pregen_jump_tbl,pregen_jump_num,-2, 0);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-2, 1);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-2, 2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-1, 2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 0, 2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 1, 2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 2, 2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 2, 1);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 2, 0);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 2,-1);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 2,-2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 1,-2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num, 0,-2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-1,-2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-2,-2);
      pregen_gen(pregen_jump_tbl,pregen_jump_num,-2,-1);
    }
  }
}

/*
 * generate_mvs()
 */

static int generate_mvs(int ply)
{
  int n,fsq,tsq,score,save_num_mvs,num_mvs=0;
  int pregen_num,*pregen_ptr;
  move_t *mv_lst;
  int *mv_score;

  mv_lst=mv_lst_heap[ply];
  mv_score=mv_score_heap[ply];

  for(tsq=0;tsq<49;tsq++) {
    if(board[tsq]!=blank) continue;

    score=0;
    save_num_mvs=num_mvs;

        /* grow moves */

    pregen_num=pregen_grow_num[tsq];
    pregen_ptr=pregen_grow_tbl[tsq];

    for(n=0;n<pregen_num;n++) {
      fsq=pregen_ptr[n];

      if(board[fsq]==(1-turn)) {
        score++;
      } else if((num_mvs-save_num_mvs)==0 && board[fsq]==turn) {
        mv_lst[num_mvs].f=fsq;
        mv_lst[num_mvs].t=tsq;
        mv_lst[num_mvs].type=move_type_grow;
        mv_score[num_mvs]=2;
        num_mvs++;
      }
    }

        /* jump moves */

    pregen_num=pregen_jump_num[tsq];
    pregen_ptr=pregen_jump_tbl[tsq];

    for(n=0;n<pregen_num;n++) {
      fsq=pregen_ptr[n];

      if(board[fsq]==turn) {
        mv_lst[num_mvs].f=fsq;
        mv_lst[num_mvs].t=tsq;
        mv_lst[num_mvs].type=move_type_jump;
        mv_score[num_mvs]=1;
        num_mvs++;
      }
    }

        /* score generated moves */

    for(n=save_num_mvs;n<num_mvs;n++) {
      mv_score[n]+=score;
    }
  }

  if(num_mvs==0) {
    mv_lst[0].type=move_type_pass;
    mv_score[0]=1;
    num_mvs=1;
  }

  return num_mvs;
}

/*
 * make_gains()
 */

static void make_gains(int sq,int ply)
{
  int n,num=0,*ptr,pregen_num,*pregen_ptr;

  pregen_num=pregen_grow_num[sq];
  pregen_ptr=pregen_grow_tbl[sq];

  ptr=save_board[ply];

  for(n=0;n<pregen_num;n++) {
    sq=pregen_ptr[n];

    if(board[sq]==(1-turn)) {
      board[sq]=turn;
      ptr[num++]=sq;
    }
  }

  num_pieces[turn]+=num;
  num_pieces[1-turn]-=num;
  save_num[ply]=num;
}

/*
 * unmake_gains()
 */

static void unmake_gains(int ply)
{
  int n,num,*ptr;

  num=save_num[ply];
  ptr=save_board[ply];

  for(n=0;n<num;n++) {
    board[ptr[n]]=1-turn;
  }

  num_pieces[turn]-=num;
  num_pieces[1-turn]+=num;
}

/*
 * make_mv()
 */

static void make_mv(move_t move,int ply)
{
  if(move.type==move_type_grow) {
    board[move.t]=turn;

    num_pieces[turn]++;
    num_pieces[blank]--;

    make_gains(move.t,ply);
  } else if(move.type==move_type_jump) {
    board[move.f]=blank;
    board[move.t]=turn;

    make_gains(move.t,ply);
  } else {
    save_num[ply]=0;
  }

  turn=1-turn;
}

/*
 * unmake_mv()
 */

static void unmake_mv(move_t move,int ply)
{
  turn=1-turn;

  if(move.type==move_type_grow) {
    unmake_gains(ply);

    board[move.t]=blank;

    num_pieces[turn]--;
    num_pieces[blank]++;
  } else if(move.type==move_type_jump) {
    unmake_gains(ply);

    board[move.f]=turn;
    board[move.t]=blank;
  }
}

/*
 * evaluate()
 */

static int evaluate(int ply)
{
  if(num_pieces[blank]==0) {
    if(num_pieces[turn]>num_pieces[1-turn]) {
      return infinity-ply;
    } else {
      return -infinity+ply;
    }
  } else if(num_pieces[turn]==0) {
    return -infinity+ply;
  } else {
    return num_pieces[turn]-num_pieces[1-turn];
  }
}

/*
 * select_move()
 */

static int select_move(int ply,int num,int *mv_loc)
{
  int score=0,loc,n,*mv_score;

  mv_score=mv_score_heap[ply];

  for(n=0;n<num;n++) {
    if(mv_score[n]>score) {
      score=mv_score[n];
      loc=n;
    }
  }

  if(score!=0) {
    mv_score[loc]=0;
    *mv_loc=loc;
    return 1;
  } else {
    return 0;
  }
}

/*
 * alpha_beta_i()
 */

static int alpha_beta_i(int alpha,int beta,int depth,int ply,int nullmove)
{
  int score,n,num;
  move_t null_move;
  move_t *mv_lst;

      /* terminal condition */

  if(	depth<=0 || ply==max_ply ||
        num_pieces[turn]==0 || num_pieces[blank]==0) {
    return evaluate(ply);
  }

  if(nullmove) {
    null_move.type=move_type_pass;

    make_mv(null_move,ply);

    node_cnt++;

    score=-alpha_beta_i(-beta,-beta+1,depth-3,ply+1,0);

    unmake_mv(null_move,ply);

    if(score>=beta) return beta;
  }

  mv_lst=mv_lst_heap[ply];

  num=generate_mvs(ply);

  while(select_move(ply,num,&n)) {
    make_mv(mv_lst[n],ply);

    node_cnt++;

    score=-alpha_beta_i(-beta,-alpha,depth-1,ply+1,1);

    unmake_mv(mv_lst[n],ply);

    if(score>alpha) {
      if(score>=beta) return beta;
      alpha=score;
    }
  }

  return alpha;
}

/*
 * alpha_beta_r()
 */

static int alpha_beta_r(int depth)
{
  int score,n,num,alpha=-infinity;
  move_t *mv_lst;
  int *mv_score;

  mv_lst=mv_lst_heap[0];
  mv_score=mv_score_heap[0];

  num=generate_mvs(0);

  if(mv_lst[0].type==move_type_pass) {
    best_mv=mv_lst[0];
    return 0;
  }

  for(n=0;n<num;n++) {
    mv_score[n]<<=3;				/* multiply by 8 */
    mv_score[n]+=rand() % 5;	/* randomize the move list */
  }

  while(select_move(0,num,&n)) {
    make_mv(mv_lst[n],0);

    node_cnt++;

    score=-alpha_beta_i(-infinity,-alpha,depth-1,1,1);

    unmake_mv(mv_lst[n],0);

    if(score>alpha) {
      best_mv=mv_lst[n];
      alpha=score;
    }
  }

  return alpha;
}

/*
 * board_new_block()
 *
 * add blocks according to configuration
 */
static void board_new_block()
{
  int n;
  char const FAR * p;

  far_strcpy(BlockConfigString,blocksets[0]);
  
      /* "none" -> we are ready */
  if (blockconf == 0) return;

      /* count predefined configurations */
  n = 0;
  p = blocksets[0];
  while (p[0])
  {
    ++n;
    p = blocksets[n];
  }

  if ((blockconf < 1) || (blockconf >= n)) {
    blockconf = 1; /* force random */
  }
  
  if (blockconf == 1) {
    n = 2 + (rand() % (n-2));
  } else {
    n = blockconf;
  }
  
  p = blocksets[n];

  if (blockconf == 1)
  {
    strcpy(BlockConfigString,"random: ");
  } else {
    BlockConfigString[0] = 0;
  }
  far_strcat(BlockConfigString,p);
  
  while (*p) {
    switch (*p) {
        case '1':
          board[21] = board[27] = board[21] = board[27] = block;
          num_pieces[blank] -= 2;
          break;
          
        case '2':
          board[22] = board[26] = block;
          num_pieces[blank] -= 2;
          break;
          
        case '3':
          board[23] = board[25] = block;
          num_pieces[blank] -= 2;
          break;
          
        case '4':
          board[3] = board[45] = block;
          num_pieces[blank] -= 2;
          break;
          
        case '5':
          board[10] = board[38] = block;
          num_pieces[blank] -= 2;
          break;
          
        case '6':
          board[17] = board[31] = block;
          num_pieces[blank] -= 2;
          break;
          
        case 'a':
          board[7] = board[13] = board[35] = board[41] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'b':
          board[14] = board[20] = board[28] = board[34] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'c':
          board[15] = board[19] = board[29] = board[33] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'd':
          board[1] = board[5] = board[43] = board[47] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'e':
          board[2] = board[4] = board[44] = board[46] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'f':
          board[9] = board[11] = board[37] = board[39] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'g':
          board[8] = board[12] = board[36] = board[40] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'h':
          board[16] = board[18] = board[30] = board[32] = block;
          num_pieces[blank] -= 4;
          break;
          
        case 'x':
          board[24] = block;
          num_pieces[blank] -= 1;
          break;
    }
    ++p;
  }
}

/*
 * board_new()
 */

static void board_new(void)
{
  int s;

  for(s=0;s<49;s++) {
    board[s]=blank;
  }

  board[0]=board[48]=white;
  board[6]=board[42]=black;

  turn=white;

  num_pieces[blank]=45;
  num_pieces[black]=num_pieces[white]=2;

  board_new_block();
}

/*
 * add bits to bitmap with 2 rows/colums offset from top left
 */
static void AddBlockToBitmap(unsigned short * bitmap,
                             unsigned int r, unsigned int c)
{
  static const unsigned char mask[] = {
    0x20, 0x80,
    0x11, 0x00,
    0x0A, 0x00,
    0x04, 0x00,
    0x0A, 0x00,
    0x11, 0x00,
    0x20, 0x80
  };
  
  unsigned char * p = (unsigned char *)bitmap;

  p[8+2*r] |= mask[(c%7)*2];
  p[8+2*r+1] |= mask[(c%7)*2+1];
  p[8+12-2*r] |= mask[(c%7)*2];
  p[8+12-2*r+1] |= mask[(c%7)*2+1];
}

/*----------------------------Ataxx Engine-------------------------------*/





#define		BoardStartDrawX			0
#define		BoardStartDrawY			16

#define		BitmapStartDrawX		3
#define		BitmapStartDrawY		3

#define		SquareSizeX				16
#define		SquareSizeY				16

#define		NumberDrawWD1X			118
#define		NumberDrawWD1Y			17

#define		NumberDrawWD2X			137
#define		NumberDrawWD2Y			17

#define		NumberDrawBD1X			118
#define		NumberDrawBD1Y			114

#define		NumberDrawBD2X			137
#define		NumberDrawBD2Y			114

static char			StatusBarString[64];

static int			PenTrack;
static int			PenTrackR;
static int			PenTrackC;

static int			SelectedF;
static int			SelectedFR;
static int			SelectedFC;

enum {
  game_normal,game_ended
};

enum {
  highlight_draw,highlight_erase
};

static int			GameState;

static int			Level;

/* System Preferences Stuff */

#define AtaxxVersion		0x0101
#define AtaxxCreator		0x71746178	/* 'Atax' */

typedef struct ProgramState_Type {
  int		board[49];
  int		num_pieces[3];
  char	StatusBarString[64];
  int		turn;
  int		SelectedF;
  int		SelectedFR;
  int		SelectedFC;
  int		GameState;
  int		Level;
} ProgramState_t;

/*
 * NewGame()
 */

static void NewGame(void)
{
  allow_block_config = 1; /* enable block configuration */
  board_new();
  SelectedF=0;
  GameState=game_normal;
  StatusBarString[0]=0;
}

/*
 * LoadState()
 */

static int LoadState(void)
{
  ProgramState_t Prefs;

/*   if(	PrefGetAppPreferencesV10(AtaxxCreator,AtaxxVersion,&Prefs, */
/*                                  sizeof(ProgramState_t))==0) { */
    return 0; 
/*   } */

  memcpy(board,Prefs.board,98);
  memcpy(num_pieces,Prefs.num_pieces,6);
  memcpy(StatusBarString,Prefs.StatusBarString,64);

  turn=Prefs.turn;
  SelectedF=Prefs.SelectedF;
  SelectedFR=Prefs.SelectedFR;
  SelectedFC=Prefs.SelectedFC;
  GameState=Prefs.GameState;
  Level=Prefs.Level;

  return 1;
}

/*
 * SaveState()
 */

static void SaveState(void)
{
  ProgramState_t Prefs;

  memcpy(Prefs.board,board,98);
  memcpy(Prefs.num_pieces,num_pieces,6);
  memcpy(Prefs.StatusBarString,StatusBarString,64);

  Prefs.turn=turn;
  Prefs.SelectedF=SelectedF;
  Prefs.SelectedFR=SelectedFR;
  Prefs.SelectedFC=SelectedFC;
  Prefs.GameState=GameState;
  Prefs.Level=Level;

/*   PrefSetAppPreferencesV10(AtaxxCreator,AtaxxVersion,&Prefs, */
/*                            sizeof(ProgramState_t)); */
}

/*
 * StartApplication() initializes the application data
 */

static void StartApplication(void)
{
      /* prime the random number generator */

/*   SysRandom(TimGetSeconds()); */

      /* pregenerate move generation tables */

  pregen();

      /* setup for a new game at level 2 */

  if(LoadState()==0) {
    blockconf = 1; /* random blocks */
    NewGame();
    Level=2;
  }
}

/*
 * BitmapDraw() is used to draw bitmaps, pass ID in.
 */

static void BitmapDraw(const unsigned char FAR * BitmapSrc,int x,int y)
{
  LibPutGraph(x,y,BitmapSrc);
}

/*
 * HighLight()
 */

static void HighLight(int r,int c,int mode)
{
  int x1,x2,y1,y2;

  x1=c*SquareSizeX+BoardStartDrawX+1;
  x2=x1+SquareSizeX-2;
  y1=r*SquareSizeY+BoardStartDrawY+1;
  y2=y1+SquareSizeY-2;

  if(mode==highlight_draw) {
    LibGdsLine(x1,y1,x1,y2);
    LibGdsLine(x1,y2,x2,y2);
    LibGdsLine(x2,y2,x2,y1);
    LibGdsLine(x2,y1,x1,y1);
  } else {
    LibGdsLineClr(x1,y1,x1,y2);
    LibGdsLineClr(x1,y2,x2,y2);
    LibGdsLineClr(x2,y2,x2,y1);
    LibGdsLineClr(x2,y1,x1,y1);
  }
}

/*
 * PieceDraw() is used to the pieces, it converts the row/column
 * stuff into screen co-ordinates
 */

static void PieceDraw(const unsigned char FAR * BitmapSrc,int r,int c)
{
  LibPutGraph(  c*SquareSizeX+BoardStartDrawX+BitmapStartDrawX,
                r*SquareSizeY+BoardStartDrawY+BitmapStartDrawY,
                BitmapSrc);
}

/*
 * BoardDraw() draws the Ataxx board.
 */

static void BoardDraw(void)
{
  BitmapDraw(Bitmap_Board,BoardStartDrawX,BoardStartDrawY);
}

/*
 * NumPiecesDraw()
 */

static void NumPiecesDraw(void)
{
  static const unsigned char FAR * Bitmap_NumberW[] = {Bitmap_NumberW0, Bitmap_NumberW1, Bitmap_NumberW2, Bitmap_NumberW3, Bitmap_NumberW4, Bitmap_NumberW5, Bitmap_NumberW6, Bitmap_NumberW7, Bitmap_NumberW8, Bitmap_NumberW9};
  static const unsigned char FAR * Bitmap_NumberB[] = {Bitmap_NumberB0, Bitmap_NumberB1, Bitmap_NumberB2, Bitmap_NumberB3, Bitmap_NumberB4, Bitmap_NumberB5, Bitmap_NumberB6, Bitmap_NumberB7, Bitmap_NumberB8, Bitmap_NumberB9};
      
  int number;

      /* first render white number of pieces */

  number=num_pieces[white]/10;
  BitmapDraw(Bitmap_NumberW[number],NumberDrawWD1X,NumberDrawWD1Y);
  number=num_pieces[white]%10;
  BitmapDraw(Bitmap_NumberW[number],NumberDrawWD2X,NumberDrawWD2Y);

      /* first render white number of pieces */

  number=num_pieces[black]/10;
  BitmapDraw(Bitmap_NumberB[number],NumberDrawBD1X,NumberDrawBD1Y);
  number=num_pieces[black]%10;
  BitmapDraw(Bitmap_NumberB[number],NumberDrawBD2X,NumberDrawBD2Y);
}

/*
 * StatusBarDraw()
 */

static void StatusBarDraw(void)
{
#if 0  
  FormPtr CurrentFrm;
  FieldPtr StatusFld;

  CurrentFrm=FrmGetActiveForm();
  StatusFld=(FieldPtr)FrmGetObjectPtr(CurrentFrm,
                                      FrmGetObjectIndex(CurrentFrm,StatusField));

  FldSetTextPtr(StatusFld,StatusBarString);
  FldDrawField(StatusFld);
#endif
  LibClrBox(50,2,100,8);
  LibPutProStr(IB_PFONT1,50,2,(byte*)StatusBarString,100);
  LibPutDisp();

      /* block configuration */
/*   LibClrBox(5,134,150,8); */
/*   LibPutProStr(IB_PFONT1,5,134,(byte*)"Blocks: ",150); */
  LibPutProStr(IB_PFONT1,40,134,(byte*)BlockConfigString,110);
}

/*
 * InvertButton
 *
 * return 0 if ID not in pTable
 *        else 1
 */
int InvertButton(const TCHTBL FAR * pTable, word ButtonID)
{
  int i = 0;

  while (pTable[i].obj)
  {
    if (pTable[i].obj == ButtonID)
    {
      LibGdsReverse(pTable[i].x1+1,pTable[i].y1+1,
                    pTable[i].x2-2,pTable[i].y2-2);
      return 1;
    }
    ++i;
  }
  return 0;
}


/*
 * MainFormDraw() redraws the main board.
 */

static void MainFormDraw(void)
{
  int r,c,s=0;
/*   ControlPtr LevelPtr; */
/*   FormPtr CurrentFrm; */

      /* get a pointer to current form */

/*   CurrentFrm=FrmGetActiveForm(); */

      /* set up the level button so that it draws properly */

  switch(Level) {
      case 1:
        InvertButton(TchButtons,Level1Button);
        break;
      case 2:
        InvertButton(TchButtons,Level2Button);
        break;
      case 3:
        InvertButton(TchButtons,Level3Button);
        break;
  }

      /* draw the form items on the screen */

/*   FrmDrawForm(CurrentFrm); */

      /* draw the board */

  BoardDraw();

      /* draw the status bar */

  StatusBarDraw();

      /* draw the piece counts */

  NumPiecesDraw();

      /* draw the pieces */

  for(r=0;r<7;r++) {
    for(c=0;c<7;c++,s++) {
      switch(board[s]) {
          case black:
            PieceDraw(Bitmap_BPieceN,r,c);
            break;
          case white:
            PieceDraw(Bitmap_WPieceN,r,c);
            break;
          case block:
            PieceDraw(Bitmap_BlockPiece,r,c);
            break;
      }
    }
  }

      /* if a piece is currently selected, highlight it */

  if(SelectedF==1) {
    HighLight(SelectedFR,SelectedFC,highlight_draw);
  }
}

/*
 * WhiteToBlackAnimate()
 */

static void WhiteToBlackAnimate(int *change_board,int change_num)
{
  int n;
  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_WPieceA,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */

  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_PieceT,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */

  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_BPieceA,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */
	
  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_BPieceN,change_board[n]/7,change_board[n]%7);
  }
  LibPutDisp();
}

/*
 * BlackToWhiteAnimate()
 */

static void BlackToWhiteAnimate(int *change_board,int change_num)
{
  int n;

  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_BPieceA,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */

  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_PieceT,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */

  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_WPieceA,change_board[n]/7,change_board[n]%7);
  }

  LibPutDisp();
  LibWait(IB_125MWAIT);
/*   SysTaskDelay(12); */
	
  for(n=0;n<change_num;n++) {
    PieceDraw(Bitmap_WPieceN,change_board[n]/7,change_board[n]%7);
  }
  LibPutDisp();
}

/*
 * GameStateUpdate()
 */

static void GameStateUpdate(void)
{
  int eval;
  eval=evaluate(0);

  if(abs(eval)==infinity) {
    GameState=game_ended;
    allow_block_config = 1; /* re-enable block configuration */
    if(	(turn==white && eval==infinity) ||
        (turn==black && eval==-infinity)) {
      strcpy(StatusBarString,"White Wins");
    } else {
      strcpy(StatusBarString,"Black Wins");
    }
    if (num_pieces[white] == num_pieces[black])
    {
      strcpy(StatusBarString,"Draw");
    }
  } else {
    GameState=game_normal;
    StatusBarString[0]=0;
  }

      /* draw the status bar */

  StatusBarDraw();

      /* draw the piece counts */

  NumPiecesDraw();
  LibPutDisp();
}

/*
 * ComputerMoveMake()
 */

static void ComputerMoveMake()
{
  int fr,fc,tr,tc;

  if(GameState==game_normal) {
    allow_block_config = 0; /* disable block configuration */
    strcpy(StatusBarString,"thinking...");
    StatusBarDraw();
  
    alpha_beta_r(Level);

    make_mv(best_mv,0);

    StatusBarString[0] = 0;
    StatusBarDraw();
    
    if(best_mv.type==move_type_pass) {
      strcpy(StatusBarString,"Computer Passes");
      StatusBarDraw();
    } else {
      fr=best_mv.f/7;
      fc=best_mv.f%7;

      tr=best_mv.t/7;
      tc=best_mv.t%7;

      HighLight(fr,fc,highlight_draw);
      LibWait(IB_250MWAIT);
/*       SysTaskDelay(30); */

      HighLight(tr,tc,highlight_draw);
      LibWait(IB_250MWAIT);
/*       SysTaskDelay(30); */

      
      if(best_mv.type==move_type_jump) {
        PieceDraw(Bitmap_PieceE,fr,fc);
      }

      HighLight(fr,fc,highlight_erase);
      HighLight(tr,tc,highlight_erase);

      switch(board[best_mv.t]) {
          case black:
            PieceDraw(Bitmap_BPieceN,tr,tc);
            if(save_num[0]) {
              WhiteToBlackAnimate(save_board[0],save_num[0]);
            }
            break;
          case white:
            PieceDraw(Bitmap_WPieceN,tr,tc);
            if(save_num[0]) {
              BlackToWhiteAnimate(save_board[0],save_num[0]);
            }
            break;
      }

      GameStateUpdate();
    }
  }
}

/*
 * UserPassMake()
 */

static void UserPassMake(void)
{
  int num;
  if(GameState==game_normal) {
    num=generate_mvs(0);
    if(mv_lst_heap[0][0].type==move_type_pass) {
      if(SelectedF==1) {
        HighLight(SelectedFR,SelectedFC,highlight_erase);
      }

      make_mv(mv_lst_heap[0][0],0);
      GameStateUpdate();

      ComputerMoveMake();

      return;
    }
  }

      /* display illegal to pass dialog */

  LibPutMsgDlg((byte*)"'Pass' is not\ra legal move!");
/*   FrmAlert(PassAlert); */
}

/*
 * UserMoveMake()
 */

static void UserMoveMake(int fr,int fc,int tr,int tc,int dist)
{
  move_t move;
  move.f=fr*7+fc;
  move.t=tr*7+tc;

  allow_block_config = 0; /* disable block configuration */
  
  if(dist==2) {
    move.type=move_type_jump;
    PieceDraw(Bitmap_PieceE,fr,fc);
  } else {
    move.type=move_type_grow;
  }

  make_mv(move,0);

  switch(board[move.t]) {
      case black:
        PieceDraw(Bitmap_BPieceN,tr,tc);
        if(save_num[0]) {
          WhiteToBlackAnimate(save_board[0],save_num[0]);
        }
        break;
      case white:
        PieceDraw(Bitmap_WPieceN,tr,tc);
        if(save_num[0]) {
          BlackToWhiteAnimate(save_board[0],save_num[0]);
        }
        break;
  }

  HighLight(SelectedFR,SelectedFC,highlight_erase);

  GameStateUpdate();

  ComputerMoveMake();
}

#define PenTrackUpdate() \
	PenTrackR=(event->y-BoardStartDrawY)/SquareSizeY; \
	PenTrackC=(event->x-BoardStartDrawX)/SquareSizeX;

#define PenTrackValid() \
	(PenTrackR>=0 && PenTrackR<7 && PenTrackC>=0 && PenTrackC<7)

/*
 * MainFormPenDownHandler()
 */

static int MainFormPenDownHandler(TCHSTS * event)
{
  PenTrackUpdate();

  if(GameState==game_normal && PenTrackValid()) {
        /* valid square has been received, start tracking */

    PenTrack=1;
    HighLight(PenTrackR,PenTrackC,highlight_draw);

    return 1;
  } else {
        /* invalid square has been received, disable tracking */

    PenTrack=0;

    return 0;
  }
}

/*
 * MainFormPenMoveHandler()
 */

static int MainFormPenMoveHandler(TCHSTS * event)
{
  if(PenTrack==1) {
    if(	PenTrackValid() &&
        (!SelectedF || SelectedFR!=PenTrackR ||
         SelectedFC!=PenTrackC)) {
      HighLight(PenTrackR,PenTrackC,highlight_erase);
    }

    PenTrackUpdate();

    if(PenTrackValid()) {
      HighLight(PenTrackR,PenTrackC,highlight_draw);
    }

    return 1;
  } else {
    return 0;
  }
}

/*
 * MainFormPenUpHandler()
 */

static int MainFormPenUpHandler(TCHSTS * event)
{
  int dist;
  if(PenTrack==1) {
    if(PenTrackValid()) {
      HighLight(PenTrackR,PenTrackC,highlight_erase);
    }

    PenTrackUpdate();

    if(PenTrackValid()) {
      if(SelectedF==1) {
            /* valid first square selection exists */

        if(board[PenTrackR*7+PenTrackC]==turn) {
              /* user wants to switch to another piece */

          HighLight(SelectedFR,SelectedFC,highlight_erase);

          SelectedFR=PenTrackR;
          SelectedFC=PenTrackC;

          HighLight(SelectedFR,SelectedFC,highlight_draw);
        } else {
          if(board[PenTrackR*7+PenTrackC]==blank) {
            dist=distance(	PenTrackR,PenTrackC,
                                SelectedFR,SelectedFC);

            if(dist<=2) {
                  /* valid */
              UserMoveMake(	SelectedFR,SelectedFC,
                                PenTrackR,PenTrackC,dist);

              SelectedF=0;
            }
          }
        }
      } else {
            /* user is making "from" square selection */

        if(board[PenTrackR*7+PenTrackC]==turn) {
              /* user makes valid from square selection */

          SelectedF=1;
          SelectedFR=PenTrackR;
          SelectedFC=PenTrackC;

          HighLight(SelectedFR,SelectedFC,highlight_draw);
        }
      }
    }
    return 1;
  } else {
    return 0;
  }
}

/*
 * MainFormHandler() handles all events associated with the main form.
 */

static int MainFormHandler(TCHSTS * event)
{
  switch(event->obj) {
      case NewButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,NewButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          NewGame();
          DrawMainForm();
          MainFormDraw();
        }
        break;
      case SwitchButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,SwitchButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          if(SelectedF==1) {
            HighLight(SelectedFR,SelectedFC,highlight_erase);
          }
          ComputerMoveMake();
        }
        break;
      case PassButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,PassButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          UserPassMake();
        }
        break;
      case HelpButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,HelpButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          LibPutMsgDlg((byte*)
                       "The game of Ataxx is\r" \
                       "based on an old arcade\r" \
                       "game by the same name.\r" \
                       "\r" \
                       "In this implementation\r" \
                       "of Ataxx, the user plays\r" \
                       "against the PV.");
          LibPutMsgDlg((byte*)
                       "To move, select a piece\r" \
                       "of your color and move\r" \
                       "it to a square 1 or 2\r" \
                       "steps away.\r" \
                       "The destination square\r" \
                       "must be empty.\r" \
                       "When moving 1 step,\r" \
                       "the moving piece doubles.\r" \
                       "When moving 2 steps,\r" \
                       "the moving piece 'jumps'\r" \
                       "to the new location.");
          LibPutMsgDlg((byte*)
                       "The end of the game is\r" \
                       "reached when no more\r" \
                       "moves remain for both\r" \
                       "sides.  The winning side\r" \
                       "is the one with the most\r" \
                       "pieces at the end.");
          LibPutMsgDlg((byte*)
                       "A special situation\r" \
                       "arises when only one\r" \
                       "side does not have any\r" \
                       "moves available.  In\r" \
                       "this case, the side\r" \
                       "without moves must\r" \
                       "'pass'.");
          LibPutMsgDlg((byte*)
                       "During block configu-\r" \
                       "ration tapping 'Blocks'\r" \
                       "enables random blocks.\r" \
                       "Tapping outside the\r" \
                       "board disables blocks.\r");
        }
        break;
      case AboutButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,AboutButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          FrmGotoForm(AboutForm);
        }
        break;
      case Level1Button:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,Level1Button);
        }
        if (event->act & ACT_BREAK_IN)
        {
          Level=1;
          DrawMainForm();
          MainFormDraw();
        }
        break;
      case Level2Button:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,Level2Button);
        }
        if (event->act & ACT_BREAK_IN)
        {
          Level=2;
          DrawMainForm();
          MainFormDraw();
        }
        break;
      case Level3Button:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,Level3Button);
        }
        if (event->act & ACT_BREAK_IN)
        {
          Level=3;
          DrawMainForm();
          MainFormDraw();
        }
        break;
      case BlockButton:
        if (event->act & ACT_ICON)
        {
          InvertButton(TchButtons,BlockButton);
        }
        if (event->act & ACT_BREAK_IN)
        {
          if (allow_block_config) {
            SelectBlockConfig();
            NewGame();
            DrawMainForm();
            MainFormDraw();
          } else {
            LibPutMsgDlg((byte*)"Cannot change while\rgame in progress");
          }
          
        }
        break;
  }

  if (event->act & ACT_MAKE) {
    return MainFormPenDownHandler(event);
  }
  
  if (event->act & (ACT_MOVE | ACT_MOVE_IN)) {
    return MainFormPenMoveHandler(event);
  }
  
  if (event->act & (ACT_BREAK | ACT_BREAK_IN)) {
    return MainFormPenUpHandler(event);
  }
  
  return 0;
}

static void DrawBlockConfig()
{
  int i, n;
  char const FAR * p;
  unsigned short bitmap[13];

  bitmap[0] = bitmap[1] = 11;
  
  n = 0;
  p = blocksets[2];

  while ((n < 49) && *p) {
    memset(&bitmap[2],0,(13-2)*sizeof(unsigned short));
    ((unsigned char *)bitmap)[4] = 0xFF;
    ((unsigned char *)bitmap)[5] = 0xE0;
    ((unsigned char *)bitmap)[24] = 0xFF;
    ((unsigned char *)bitmap)[25] = 0xE0;
    for (i=3;i<12;++i) {
      ((unsigned char *)bitmap)[2*i] = 0x80;
      ((unsigned char *)bitmap)[2*i+1] = 0x20;
    }
    
    while (*p) {
      switch (*p) {
          case '1':
            AddBlockToBitmap(bitmap,3,0);
            break;
          
          case '2':
            AddBlockToBitmap(bitmap,3,1);
            break;
          
          case '3':
            AddBlockToBitmap(bitmap,3,2);
            break;
          
          case '4':
            AddBlockToBitmap(bitmap,0,3);
            break;
          
          case '5':
            AddBlockToBitmap(bitmap,1,3);
            break;
          
          case '6':
            AddBlockToBitmap(bitmap,2,3);
            break;
          
          case 'a':
            AddBlockToBitmap(bitmap,1,0);
            break;
          
          case 'b':
            AddBlockToBitmap(bitmap,2,0);
            break;
          
          case 'c':
            AddBlockToBitmap(bitmap,2,1);
            break;
          
          case 'd':
            AddBlockToBitmap(bitmap,0,1);
            break;
          
          case 'e':
            AddBlockToBitmap(bitmap,0,2);
            break;
          
          case 'f':
            AddBlockToBitmap(bitmap,1,2);
            break;
          
          case 'g':
            AddBlockToBitmap(bitmap,1,1);
            break;
          
          case 'h':
            AddBlockToBitmap(bitmap,2,2);
            break;
          
          case 'x':
            AddBlockToBitmap(bitmap,3,3);
            break;
      }
      ++p;
    }

    PieceDraw((const unsigned char FAR *)bitmap,n/7,n%7);
    
    ++n;
    p = blocksets[n+2];
  }
}

static void SelectBlockConfig()
{
  TCHSTS tsts;
  TCHSTS * event = &tsts; /* needed for PenTrack... */
  
  BoardDraw();
  DrawBlockConfig();

  blockconf = 0; /* default: no blocks */
  
  do {
    LibPutDisp();
    LibTchWait( event ); /* Waiting for touch */

    if (event->act & ACT_ICON)
    {
          /* redraw icons, but ignore all but BlockButton */
      if (InvertButton(TchButtons,event->obj))
      {
        if (event->act & ACT_BREAK_IN)
        {
          if (event->obj == BlockButton)
          {
            blockconf = 1; /* random */
          }
          break;
        }
      } else if (event->act & ACT_MAKE) {
        PenTrackUpdate();
        if (PenTrackValid())
        {
          blockconf = 2 + (PenTrackR * 7) + PenTrackC;
        }
        break;
      }
    }
  } while(1);
}


/*
 * AboutFormHandler() handles all events associated with the about form.
 */

static int AboutFormHandler(TCHSTS * event)
{
  if (event->act & ACT_MAKE) {
    FrmGotoForm(MainForm);
    MainFormDraw();
    return 1;
  }
  
  return 0;
}

/*
 * ApplicationHandleEvent() handles any application related events.
 */

static int ApplicationHandleEvent(TCHSTS * event)
{
#if 0  
  FormPtr CurrentFrm;

  if(event->eType==frmLoadEvent) {
    CurrentFrm=FrmInitForm(event->data.frmLoad.formID);
    FrmSetActiveForm(CurrentFrm);

    switch(event->data.frmLoad.formID) {
        case MainForm:
          FrmSetEventHandler(CurrentFrm,MainFormHandler);
          break;
        case AboutForm:
          FrmSetEventHandler(CurrentFrm,AboutFormHandler);
          break;
    }

    return 1;
  } else {
    return false;
  }
#endif  
}

/*
 * EventLoop() fetchs new events from the event queue and dispatches them.
 */

static void EventLoop(void)
{
  TCHSTS event;

  LibTchStackClr();
  LibTchStackPush(TchAllDsp);
  LibTchStackPush(TchHardIcon); /* Hard icon touch table */
  LibTchStackPush((TCHTBL FAR *)TchButtons);
  
  MainFormDraw();

  do {
    LibPutDisp();
    LibTchWait( &event ); /* Waiting for touch */
    switch (ActiveFrmID)
    {
        case MainForm:
          MainFormHandler(&event);
          break;

        case AboutForm:
          AboutFormHandler(&event);
          break;
    }
/*     EvtGetEvent(&event,evtWaitForever); */
/*     if(!SysHandleEvent(&event)) { */
/*       if(!MenuHandleEvent((void *)0,&event,&error)) { */
/*         if(!ApplicationHandleEvent(&event)) { */
/*           FrmDispatchEvent(&event); */
/*         } */
/*       } */
/*     } */
  } while(event.obj!=OBJ_HIC_ESC);
}

/*
 * StopApplication() houses all clean up code
 */

static void StopApplication(void)
{
  SaveState();
}

void DrawMainForm()
{
/* FORM MainForm 0 0 160 160 */
  LibClrDisp();
  
/* 	TITLE	"Ataxx" */
  LibPutProStr(IB_PFONT2,2,2,(byte*)"Ataxx",160);
  LibReverse(0,0,4+LibGetProStrSize(IB_PFONT2,(byte*)"Ataxx"),12);
  LibLine(0,10,160,1,2);

  LibBox(3,132,34,12,1);
  LibPutProStr(IB_PFONT1,3+3,132+2,(byte*)"Blocks",34);
  
/* 	FIELD			ID StatusField  AT ( 50   2  100 10 ) USABLE LEFTALIGN FONT 1 NONEDITABLE SINGLELINE MAXCHARS 32 */
/*   LibBox(50,2,100,10,1); */
/*   LibClrBox(50,2,100,10); */

/* 	BUTTON	"New"		ID NewButton	AT (  3 145  24 AUTO ) */
  LibBox(3,147,24,12,1);
  LibPutProStr(IB_PFONT1,3+3,147+2,(byte*)"New",24);
  
/* 	BUTTON	"Switch"	ID SwitchButton	AT ( PREVRIGHT+4 145  32 AUTO ) */
  LibBox(27+4,147,34,12,1);
  LibPutProStr(IB_PFONT1,27+4+2,147+2,(byte*)"Switch",34);
  
/* 	BUTTON	"Pass"		ID PassButton	AT ( PREVRIGHT+4 145  26 AUTO ) */
  LibBox(65+4,147,26,12,1);
  LibPutProStr(IB_PFONT1,65+4+3,147+2,(byte*)"Pass",26);
  
/* 	BUTTON	"Help"		ID HelpButton	AT ( PREVRIGHT+4 145  25 AUTO ) */
  LibBox(95+4,147,25,12,1);
  LibPutProStr(IB_PFONT1,95+4+3,147+2,(byte*)"Help",25);
  
/* 	BUTTON	"About"		ID AboutButton	AT ( PREVRIGHT+4 145  30 AUTO ) */
  LibBox(124+4,147,30,12,1);
  LibPutProStr(IB_PFONT1,124+4+3,147+2,(byte*)"About",30);
  
/* 	LABEL	"Level"		ID 2000 AT (122 60) FONT 1 */
  LibPutProStr(IB_PFONT2,122,60,(byte*)"Level",160);
  
/* 	PUSHBUTTON "1"		ID Level1Button AT (121 71 9 9) GROUP LevelGroup */
  LibBox(121,71,9,12,1);
  LibPutProStr(IB_PFONT1,121+2,71+2,(byte*)"1",9);
  
/* 	PUSHBUTTON "2"		ID Level2Button AT (PREVRIGHT+1 71 9 9) GROUP LevelGroup */
  LibBox(130+1,71,9,12,1);
  LibPutProStr(IB_PFONT1,130+1+3,71+2,(byte*)"2",9);
  
/* 	PUSHBUTTON "3"		ID Level3Button AT (PREVRIGHT+1 71 9 9) GROUP LevelGroup */
  LibBox(140+1,71,9,12,1);
  LibPutProStr(IB_PFONT1,140+1+3,71+2,(byte*)"3",9);
}

void LibPutProStrCenter(byte type,int x,int y,byte *string,int x_size)
{
  int size = LibGetProStrSize(type,string);
  LibPutProStr(type,x+(x_size/2)-(size/2),y,string,size);
}

void DrawAboutForm()
{
/* FORM AboutForm 2 2 156 156 */
  LibClrDisp();
  LibBox(2,2,154,154,1);
  
/* 	TITLE	"About Ataxx" */
  LibPutProStr(IB_PFONT2,4,4,(byte*)"About Ataxx",150);
  LibReverse(3,3,4+LibGetProStrSize(IB_PFONT2,(byte*)"Abbout Ataxx"),12);
  LibLine(2,13,154,1,2);
          
/* 	FORMBITMAP AT (35 19) BITMAP Bitmap_About */
  BitmapDraw(Bitmap_About,35,19);
  
/* 	LABEL	"V 1.01 FreeWare" ID 2001 AT (CENTER 88) FONT 1 */
  LibPutProStrCenter(IB_PFONT2,4,88,(byte*)"V 1.01 FreeWare",152);
  
/* 	LABEL	"By Valavan Manohararajah '97" ID 2002 AT (CENTER 98) FONT 0 */
  LibPutProStrCenter(IB_PFONT1,4,98,(byte*)"By Valavan Manohararajah '97",152);
  
/* 	LABEL	"Email any comments to" ID 2003 AT (CENTER 112) FONT 0 */
  LibPutProStrCenter(IB_PFONT1,4,112,(byte*)"Email any comments to",152);
  
/* 	LABEL	"manohar@ecf.toronto.edu" ID 2004 AT (CENTER 122) FONT 0 */
  LibPutProStrCenter(IB_PFONT1,4,122,(byte*)"manohar@ecf.toronto.edu",152);
  
/* 	BUTTON	"Ok" ID AboutOkButton AT (CENTER 140 AUTO AUTO) */
}

void FrmGotoForm(int FrmID)
{
  ActiveFrmID = FrmID;
  
  switch (FrmID)
  {
      case MainForm:
        DrawMainForm();
        break;

      case AboutForm:
        DrawAboutForm();
        break;
  }
}

/*
 * PILOT entry point
 */ 

unsigned long PilotMain(unsigned short cmd, void * cmdPBP, unsigned short launchFlags)
{
      /* if not a normal launch then return to OS */

  if(cmd) return 0;

      /* initialize the app */

  StartApplication();

      /* show the main form */

  FrmGotoForm(MainForm);

      /* event processor */

  EventLoop();
	
      /* cleanup code */

  StopApplication ();

  return 0;
}

/******************************************************************************
*	[Title]		RandomSeed(0<=VAL<=32768)
*	[Arguments]	None
*	[ReturnVAL]	RandomSeed
******************************************************************************/
static int GetSeed( void ) {
	int   s;
	long  hh, mm, ss;
	byte  stime[7];

	LibGetDateTimeM( stime );
	hh = (long)( (10*(stime[1]&0x0F) + (stime[2]&0x0F)) * 3600 );
	mm = (long)( (10*(stime[3]&0x0F) + (stime[4]&0x0F)) * 60 );
	ss = (long)( (10*(stime[5]&0x0F) + (stime[6]&0x0F)) );
	s = (int)( (hh+mm+ss)%32768 );
	return( s );
}

void main()
{
  srand( GetSeed() );			/* Randomize */
  PilotMain(0,0,0);
  LibJumpMenu();
}
