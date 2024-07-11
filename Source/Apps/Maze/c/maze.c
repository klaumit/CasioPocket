/* maze.c        by Alessandro Dorigatti *
 *        Simple 2D-Maze Game            */

#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"
#include        "soundlib.h"

/* Constants  */
#define  EAST_W    1
#define  SOUTH_W   2
#define  WEST_W    4
#define  NORTH_W   8
#define  VISITED  16

#define  TCH_GAMEAREA  0x8000
#define  OBJ_IC_NEW    0x8001
#define  OBJ_IC_LEVEL  0x8002

/* Maze size: 160/RES_X (RES_Y)  must be even value >=4 */
unsigned char RES_X = 20;
unsigned char RES_Y = 20;
unsigned char DX,DY;

byte         gamelevel;	/* Game Level*/

/* Game Area Information */
TCHTBL far TchGameArea[] = {	/* GameArea */
  0,0,159,159,
  ACT_MAKE | ACT_MOVE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK | ACT_500MSEC,
  TCH_GAMEAREA,
  0x0000,
  
  0, 0, 0, 0,
  ACT_NONE,
  OBJ_END,
  0x0000
};

TCHTBL far TchHeaderIcon[] = {
	0, 0, 27, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_NEW,
	0x0000,

	28, 0, 56, 11,
	ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
	OBJ_IC_LEVEL,
	0x0000,

	0, 0, 0, 0,
	ACT_NONE,
	OBJ_END,
	0x0000
};

T_ICON far TiconNew = { &TchHeaderIcon[0], NULL, NULL, 0x00 };
T_ICON far TiconLevel = { &TchHeaderIcon[1], NULL, NULL, 0x00 };

/* Maze array */
unsigned char maze[40][40];
unsigned char st[1600][2];
int top;

/* We use the following array to choose the next cell... */
unsigned char next[4];
unsigned char n;

void TraceMaze(void);

/* Stack Functions */
void StackClear(void)
{    
  int cnt;
  for(cnt=0; cnt<RES_X*RES_Y; cnt++)
  {
    st[cnt][0] = 0;
    st[cnt][1] = 0;
  }

  top = -1;
}

void StackPush(unsigned char x, unsigned char y)
{
  top++;
  st[top][0] = x;
  st[top][1] = y;
}

unsigned char StackPopX(void)
{
  return(st[top][0]);
}

unsigned char StackPopY(void)
{
  return(st[top--][1]);
}

/* Get int seed from Current Time *
 * (from BattleShip software)     */
int GetSeed(void)
{
  byte stime[7];
  long hh,mm,ss;
  int s;

  LibGetDateTimeM(stime);
  hh=(long)((10*stime[1]&0x0F+(stime[2]&0x0F)) *3600);
  mm=(long)((10*stime[3]&0x0F+(stime[4]&0x0F)) *60);
  ss=(long)((10*stime[5]&0x0F+(stime[6]&0x0F)));  
  s=(int) ((hh+mm+ss)%32768);
  return(s);
}

/* Build Maze Function */
void BuildMaze(unsigned int x, unsigned int y)
{
  StackClear();

  for(;;)
    {
      /* First, we VISIT the current cell           */
      maze[x][y] |= VISITED;

      /* Then, we check the possible next cells     */
      n = 0;
      
      if (x<RES_X-1 && !(maze[x+1][y] & VISITED))
	next[n++]=EAST_W;
      if (y<RES_Y-1 && !(maze[x][y+1] & VISITED))
	next[n++]=SOUTH_W;
      if (x>0       && !(maze[x-1][y] & VISITED))
	next[n++]=WEST_W;
      if (y>0       && !(maze[x][y-1] & VISITED))
	next[n++]=NORTH_W;
      
      /* If we have no more directions...           */
      if (n==0)
	{
	  if (top==-1)
	    return;

	  x = StackPopX();
	  y = StackPopY();
	}
      else
	{
	  /* else, we choose the next cell...           */
	  /* ...and we break the wall in that direction *
	   * ...then we go in that direction!           */
	  StackPush(x,y);
	  n = rand()%n;
      
	  switch (next[n])
	    {
	    case EAST_W:
	      maze[x][y] &= (SOUTH_W|WEST_W|NORTH_W|VISITED);
	      maze[x+1][y] &= (EAST_W|SOUTH_W|NORTH_W|VISITED);
	      x+=1;
	      break;

	    case SOUTH_W:
	      maze[x][y] &= (EAST_W|WEST_W|NORTH_W|VISITED);
	      maze[x][y+1] &= (EAST_W|WEST_W|SOUTH_W|VISITED);
	      y+=1;
	      break;

	    case WEST_W:
	      maze[x][y] &= (SOUTH_W|EAST_W|NORTH_W|VISITED);
	      maze[x-1][y] &= (WEST_W|SOUTH_W|NORTH_W|VISITED);
	      x-=1;
	      break;

	    case NORTH_W:
	      maze[x][y] &= (SOUTH_W|EAST_W|WEST_W|VISITED);
	      maze[x][y-1] &= (EAST_W|WEST_W|NORTH_W|VISITED);
	      y-=1;
	      break;

	    default:
	      break;
	    }
	}
    }
}

void TraceMaze()
{
  unsigned int  x, y;
  
  LibClrDisp();

  for(y=0; y<RES_Y; y++)
    for(x=0; x<RES_X; x++)
      {
	if(maze[x][y] & WEST_W)
	    LibGdsLine(x*DX, y*DY, x*DX, (y+1)*DY-1);
	else
	  {
	    LibGdsDotOn(x*DX,y*DY);
	    LibGdsDotOn(x*DX,(y+1)*DY-1);
	  }
	if(maze[x][y] & NORTH_W)
	    LibGdsLine(x*DX, y*DY, (x+1)*DX-1, y*DY);
	else
	  {
	    LibGdsDotOn(x*DX,y*DY);
	    LibGdsDotOn((x+1)*DX-1,y*DY);
	  }
	if(maze[x][y] & EAST_W)
	    LibGdsLine((x+1)*DX-1, y*DY, (x+1)*DX-1, (y+1)*DY-1);
	else
	  {
	    LibGdsDotOn((x+1)*DX-1,y*DY);
	    LibGdsDotOn((x+1)*DX-1,(y+1)*DY-1);
	  }
	if(maze[x][y] & SOUTH_W)
	    LibGdsLine(x*DX, (y+1)*DY-1, (x+1)*DX-1, (y+1)*DY-1);
	else
	  {
	    LibGdsDotOn(x*DX,(y+1)*DY-1);
	    LibGdsDotOn((x+1)*DX-1,(y+1)*DY-1);
	  }
      }

  LibPutDisp();
}

/* Options Display */
void OptionsDisplay(void){

	LibClrDisp();
	
	LibPutFarData(0,0,8);

	LibPutProStr(IB_PFONT1,5,2,"New",30);				/* New */
	LibPutProStr(IB_PFONT1,30,2,"Level",30);			/* Level */

	/* Copyright */
	LibPutProStr(IB_PFONT1,60,60,"Maze Game",100);	
	LibPutProStr(IB_PFONT1,10,70,"(C) 2001 - Alessandro Dorigatti",150);
	LibPutProStr(IB_PFONT1,44,90,"Hit ESC to finish",150);
	LibGdsBox(8,58,158,104);

	/* Init the event handler  */
	LibTchStackClr();
	LibTchStackPush(NULL);
	LibTchStackPush(TchHeaderIcon);
	LibTchStackPush(TchHardIcon);
	LibTchInit();

	LibPutDisp();
}

static byte LevelSet(void)
{
	byte    rtv[3+1] = {0,1,2,0xff};
	byte    cmnt[3*(7+1)],np;
	SLW2_ST win_prm;
	byte    sel_ret;
	byte    t_tbl[3][7+1];
	int     i,k,len;

	/* data */
	strcpy(t_tbl[0],"Level1");
	strcpy(t_tbl[1],"Level2");
	strcpy(t_tbl[2],"Level3");

	/* initial */
	np = gamelevel;

	/* disp Buff Making */
	for(i=k=0;i<3;i++,k++){
	    len = strlen(t_tbl[i]);
	    memcpy(&cmnt[k],t_tbl[i],len);
	    k += len;
	    if(i<1) cmnt[k] = 0xfe;
	    else    cmnt[k] = 0x00;     /* End Data */
	}

	/* select window */
	win_prm.x    =  30;
	win_prm.y    =  12;
	win_prm.xs   =  37;
	win_prm.ity  =   9;
	win_prm.np   = np;
	win_prm.cmnt = cmnt;
	win_prm.rtv  = rtv;
	win_prm.t_xs =   4;

	sel_ret = LibSelWindowExt(&win_prm);
	return(sel_ret);
}

void output_sound(void)
{
  int cnt;
  
  for(cnt=500; cnt>50; cnt-=50)
    sound(cnt,400,5);
}

/* Main Program */
void main()
{
  unsigned int entr_x;		/* Entrance X              */
  unsigned int exit_x;		/* Exit X                  */
  unsigned int x,y;		/* Player X,Y              */
  unsigned int req_x,req_y;	/* Pen Position            */
  int cnt;

  byte   now_exit, ret;

  TCHSTS tsts;

initial:

  /* First, we choose the level  and then we play! */
  OptionsDisplay();
  gamelevel = 1;

  for(cnt=0; cnt<10; cnt++)
    {
      note("A4",  800, 4);
      note("C#5", 800, 4);
      note("E5",  800, 4);
      note("A5",  800, 4);
    }

  /* Options Handling */
  now_exit = 0;

  while(now_exit==0)
    {
      LibTchWait(&tsts);
      
      switch(tsts.obj)
	{
	case OBJ_HIC_ESC:
	  now_exit = 1;
	  break;
	  
	case  OBJ_IC_LEVEL:			/* [LEVEL] */
	  if(LibBlockIconClick( &TiconLevel, &tsts , B_ICON_RIGHT ) == TRUE)
	    {
	      ret = LevelSet();
	      if(ret < 3)
		gamelevel = ret;
	    }
	  break;

	case  OBJ_IC_NEW:			/* [NEW] */
	  if(LibBlockIconClick( &TiconNew, &tsts , B_ICON_LEFT) == TRUE)
	    now_exit=2;
	  break;
	  
	default:
	  break;
	}
    }

  /* If MenuBar Icon => exit! */
  if(now_exit==1)
    {
      LibJumpMenu();
      return;
    }

  /* ... else you've selected 'New' => Maze! */

  /* Set correct maze parameters */
  switch(gamelevel)
    {
    case 0:
      RES_X = 10;
      RES_Y = 10;
      break;
    case 1:
      RES_X = 20;
      RES_Y = 20;
      break;
    case 2:
      RES_X = 40;
      RES_Y = 40;
      break;
    default:
      break;
    }
  
  DX = 160 / RES_X;
  DY = 160 / RES_Y;

  /* First, we close all cells */
  for (y=0; y<RES_Y; y++)
    for (x=0; x<RES_X; x++)
      maze[x][y] = EAST_W|SOUTH_W|WEST_W|NORTH_W;

  /* Next, we choose entrance (south) and exit (north) */

  /* Init Random Sequence */
  srand(GetSeed());

  /* EXIT : no north wall */
  exit_x = rand()%RES_X;
  maze[exit_x][0] = EAST_W|SOUTH_W|WEST_W;

  /* ENTRANCE : no south wall */
  entr_x = rand()%RES_X;
  maze[entr_x][RES_Y-1] = EAST_W|WEST_W|NORTH_W;

  /* Then, create the maze: simple backtracking algorithm! */
  BuildMaze(entr_x, RES_Y-1);

  /* Finally, draw the maze! */
  TraceMaze();

  /* Un-visit all the cells  */
  for (y=0; y<RES_Y; y++)
    for (x=0; x<RES_X; x++)
      maze[x][y]&= EAST_W|SOUTH_W|WEST_W|NORTH_W;

  /* Set the player initial position */
  x = entr_x;
  y = RES_Y-1;

  for(cnt=1000;cnt>50; cnt-=50)
    sound(cnt, 400, 8);

  /* Init the event handler  */
  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchGameArea);
  LibTchStackPush(TchHardIcon);
  LibTchInit();
  
  /* Only wait 'Menu' button */
  now_exit = FALSE;

  while(!now_exit)
    {
      /* Draw Player */
      maze[x][y]|=VISITED;
      LibGdsBox(x*DX+1,y*DY+1,(x+1)*DX-2,(y+1)*DY-2);
      LibPutDisp();

      LibTchWait(&tsts);
      
      switch(tsts.obj)
	{
	case OBJ_HIC_ESC:
	  goto initial;
	  break;
	  
	case TCH_GAMEAREA:
	  req_x = tsts.x;
	  req_y = tsts.y;


	  
	  if (x*DX < req_x && req_x < (x+1)*DX)
	    {
	      if (req_y < y*DY && !(maze[x][y] & NORTH_W))
		{
		  LibGdsMesh(x*DX+1,y*DY+1,(x+1)*DX-2,(y+1)*DY-2);
		  LibPutDisp();
		  output_sound();
		  y-=1;
		}
	      else if (req_y > (y+1)*DY && !(maze[x][y] & SOUTH_W) && y!=RES_Y-1)
		{
		  LibGdsMesh(x*DX+1,y*DY+1,(x+1)*DX-2,(y+1)*DY-2);
		  LibPutDisp();
		  output_sound();		  
		  y+=1;
		}
	    }
	  else if (y*DY < req_y && req_y < (y+1)*DY)
	    {
	      if (req_x < x*DX && !(maze[x][y] & WEST_W))
		{
		  LibGdsMesh(x*DX+1,y*DY+1,(x+1)*DX-2,(y+1)*DY-2);
		  LibPutDisp();
		  output_sound();
		  x-=1;
		}
	      else if (req_x > (x+1)*DX && !(maze[x][y] & EAST_W))
		{
		  LibGdsMesh(x*DX+1,y*DY+1,(x+1)*DX-2,(y+1)*DY-2);
		  LibPutDisp();
		  output_sound();		  
		  x+=1;
		}
	    }
	  
	  /* Exit reached! */
	  if (x==exit_x && y==0)
	    now_exit = TRUE;
	  
	  break;

	default:
	  break;
	}
    }
  
  note("E5", 5000, 7);
  note("P", 1500, 7);  
  note("E5", 5000, 7);
  note("P", 1500, 7);  
  note("F#5", 5000, 7);
  note("P", 1500, 7);  
  note("E5", 5000, 7);
  note("P", 1500, 7);  
  note("F#5", 5000, 7);
  note("P", 1500, 7);  
  note("G#5", 5000, 7);
  note("P", 1500, 7);  
  
  goto initial;
}













































