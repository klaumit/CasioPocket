#define   TCH_END     {0,0,0,0,ACT_NONE,OBJ_END,0x0000}

#define   OBJ_YES     0x9000
#define   OBJ_NO      0x9001

#define   OBJ_YES_X       72
#define   OBJ_YES_Y       88
#define   OBJ_YES_WIDTH   29
#define   OBJ_YES_HEIGHT  14

#define   OBJ_NO_X       106
#define   OBJ_NO_Y       88
#define   OBJ_NO_WIDTH   29
#define   OBJ_NO_HEIGHT  14

#define   OBJ_IC_SET     0xA020
#define   OBJ_IC_CLR     0xA021
#define   OBJ_IC_ESC     0xA022

#define   OBJ_MENU_OK	 	0x9002
#define   OBJ_MENU_CANCEL	0x9003
#define   OBJ_MENU_NEUEINGABE	0x9004
#define   OBJ_MENU_RECHNEN	0x9005
#define   OBJ_MENU_POSTEN1	0x9006
#define   OBJ_MENU_POSTEN2	0x9007
#define   OBJ_MENU_POSTEN3	0x9008
#define   OBJ_MENU_POSTEN4	0x9009
#define   OBJ_MENU_POSTEN5	0x9010

#define   OBJ_STAT_OK		0x9011


TCHTBL far Keys[]=
       {
       OBJ_YES_X,
       OBJ_YES_Y,
       OBJ_YES_X + OBJ_YES_WIDTH,
       OBJ_YES_Y + OBJ_YES_HEIGHT,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       /* ACT_ICON,
       */
       OBJ_YES,
       0x0000,

       OBJ_NO_X,
       OBJ_NO_Y,
       OBJ_NO_X + OBJ_NO_WIDTH,
       OBJ_NO_Y + OBJ_NO_HEIGHT,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       /* ACT_ICON,
       */
       OBJ_NO,
       0x0000,

       TCH_END
       };

T_ICON far TiconYes = {&Keys[0],NULL,NULL,0x00 };
T_ICON far TiconNo = {&Keys[1],NULL,NULL,0x00 };


TCHTBL far TchHeaderIcon[]=
       {
       40,0,68,11,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_IC_SET,
       0x0000,

       69,0,96,11,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_IC_CLR,
       0x0000,

       97,0,123,11,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_IC_ESC,
       0x0000,

       TCH_END
       };

T_ICON far TiconSet = {&TchHeaderIcon[0],NULL,NULL,0x00 };
T_ICON far TiconClr = {&TchHeaderIcon[1],NULL,NULL,0x00 };
T_ICON far TiconEsc = {&TchHeaderIcon[2],NULL,NULL,0x00 };


/*
  Eingabe:  1,1,20,10
  Rechnen:  141,1,158,10
  CANCEL :  80,140,159,159 
  OK     :  0,140,79,159  
*/
TCHTBL far MenuIcon[]=
       {
       0,0,20,12,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_NEUEINGABE,
       0x0000,

       140,0,158,11,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_RECHNEN,
       0x0000,
       
       0,140,79,158,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_OK,
       0x0000,

       80,140,158,158,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_CANCEL,
       0x0000,
       
       139,40,150,50,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_POSTEN1,
       0x0000,
       
       139,60,150,70,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_POSTEN2,
       0x0000,
       
       139,80,150,90,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_POSTEN3,
       0x0000,
       
       139,100,150,110,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_POSTEN4,
       0x0000,
       
       139,120,150,130,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_MENU_POSTEN5,
       0x0000,
       

       TCH_END
       };


T_ICON far TiconMenuNeueingabe	= {&MenuIcon[0],NULL,NULL,0x00 };
T_ICON far TiconMenuRechnen 	= {&MenuIcon[1],NULL,NULL,0x00 };
T_ICON far TiconMenuOk 		= {&MenuIcon[2],NULL,NULL,0x00 };
T_ICON far TiconMenuCancel 	= {&MenuIcon[3],NULL,NULL,0x00 };
T_ICON far TiconMenuPosten1 	= {&MenuIcon[4],NULL,NULL,0x00 };
T_ICON far TiconMenuPosten2 	= {&MenuIcon[5],NULL,NULL,0x00 };
T_ICON far TiconMenuPosten3 	= {&MenuIcon[6],NULL,NULL,0x00 };
T_ICON far TiconMenuPosten4 	= {&MenuIcon[7],NULL,NULL,0x00 };
T_ICON far TiconMenuPosten5 	= {&MenuIcon[8],NULL,NULL,0x00 };


TCHTBL far StatOkIcon[]=
       {
       0,140,159,159,
       ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
       OBJ_STAT_OK,
       0x0000,
       
       TCH_END
       };	

T_ICON far TiconStatOk		= {&StatOkIcon[0],NULL,NULL,0x00 };