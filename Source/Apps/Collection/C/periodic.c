#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <L_libc.h>


#define OBJ_IC_PAD 0x9000

int npage(int elem);
int xcoord(int elem);
int ycoord(int elem);
void Disp(int page);
void PutMath(int x, int y, char* string);
char* GetEConf(int elem);

char font[48][6] = {
#include"smfont.c"
};

byte letter[10];

TCHTBL TchList[] = {
  0,0,159,159,
  ACT_MAKE | ACT_MOVE_IN | ACT_MOVE_OUT | ACT_BREAK_IN,
  OBJ_IC_PAD,
  0x0000,

  0,0,0,0,
  ACT_NONE,
  OBJ_END,
  0x0000
};

typedef struct {
  char sym[3];
  char name[14];
  float weight;
  char x;
  char y;
  char estrbase;
  char estr[22];
  char gst[10];
  float ie;
} ELEMENT;

#define NUM_ELEMENTS 118

ELEMENT Elem[118] = {
  "H" , "Hydrogen",    1.00794,    1,  1,  0, "1s",        "^2!S_1/2",   
13.5984,
  "He", "Helium",      4.002602,   8,  1,  1, "^2",        "^1!S_0",     
24.5874,

  "Li", "Lithium",     6.941,      1,  2,  0, "(He)2s",    "^2!S_1/2",    
5.3917,
  "Be", "Beryllium",   9.012182,   2,  2,  3, "^2",        "^1!S_0",      
9.3227,
  "B",  "Boron",      10.811,      3,  2,  4, "!2p",       "^2!P_1/2",    
8.2980,
  "C",  "Carbon",     12.0107,     4,  2,  5, "^2",        "^3!P_0",     
11.2603,
  "N",  "Nitrogen",   14.00674,    5,  2,  5, "^3",        "^4!S_3/2",   
14.5341,
  "O",  "Oxygen",     15.9994,     6,  2,  5, "^4",        "^3!P_2",     
13.6181,
  "F",  "Fluorine",   18.9984032,  7,  2,  5, "^5",        "^2!P_3/2",   
17.4228,
  "Ne", "Neon",       20.1797,     8,  2,  5, "^6",        "^1!S_0",     
21.5646,

  "Na", "Sodium",     22.989770,   1,  3,  0, "(Ne)3s",    "^2!S_1/2",    
5.1391,
  "Mg", "Magnesium",  24.3050,     2,  3, 11, "^2",        "^1!S_0",      
7.6462,
  "Al", "Aluminum",   26.981538,   3,  3, 12, "!3p",       "^2!P_1/2",    
5.9858,
  "Si", "Silicon",    28.0855,     4,  3, 13, "^2",        "^3!P_0",      
8.1517,
  "P",  "Phosphorus", 30.973761,   5,  3, 13, "^3",        "^4!S_3/2",   
10.4867,
  "S",  "Sulphur",    32.066,      6,  3, 13, "^4",        "^3!P_2",     
10.3600,
  "Cl", "Chlorine",   35.4527,     7,  3, 13, "^5",        "^2!P_3/2",   
12.9676,
  "Ar", "Argon",      39.948,      8,  3, 13, "^6",        "^1!S_0",     
15.7596,
  
  "K",  "Potassium",  39.0983,     1,  4,  0, "(Ar)4s",    "^2!D_3/2",   4.3407,
  "Ca", "Calcium",    40.078,      2,  4, 19, "^2",            "^1!S_0",    
6.1132,
  "Sc", "Scandium",   44.955910,   3,  4,  0, "(Ar)3d4s^2",    "^2!D_3/2",  
6.5615,
  "Ti", "Titanium",   47.867,      4,  4,  0, "(Ar)3d^2!4s^2", "^3!F_2",    
6.8281,
  "V",  "Vanadium",   50.9415,     5,  4,  0, "(Ar)3d^3!4s^2", "^4!F_3/2",  
6.7463,
  "Cr", "Cromium",    51.9961,     6,  4,  0, "(Ar)3d^5!4s",   "^7!S_3",    
6.7665,
  "Mn", "Manganese",  54.938049,   7,  4,  0, "(Ar)3d^5!4s^2", "^6!S_5/2",  
7.4340,
  "Fe", "Iron",       55.845,      8,  4,  0, "(Ar)3d^6!4s^2", "^5!D_4",    
7.9024,
  "Co", "Cobalt",     58.933200,   9,  4,  0, "(Ar)3d^7!4s^2", "^4!F_9/2",  
7.8810,
  "Ni", "Nickel",     58.93320,   10,  4,  0, "(Ar)3d^8!4s^2", "^3!F_4",    
7.6398,
  "Cu", "Copper",     63.546,      1,  5,  0, "(Ar)3d^10!4s",  "^2!S_1/2",  
7.7264,
  "Zn", "Zinc",       65.39,       2,  5, 29, "^2",            "^1!S_0",    
9.3942,
  "Ga", "Gallium",    69.723,      3,  5, 30, "!4p",           "^2!P_1/2",  
5.9993,
  "Ge", "Germanium",  72.61,       4,  5, 31, "^2",            "^3!P_0",    
7.8994,
  "As", "Arsenic",    74.92160,    5,  5, 31, "^3",            "^4!S_3/2",  
9.7886,
  "Se", "Selenium",   78.96,       6,  5, 31, "^4",            "^3!P_2",    
9.7524,
  "Br", "Bromine",    79.904,      7,  5, 31, "^5",            "^2!P_3/2", 
11.8138,
  "Kr", "Krypton",    83.80,       8,  5, 31, "^6",            "^1!S_0",   
13.9996,

  "Rb", "Rubidium",   85.4678,     1,  6,  0, "(Kr)5s",        "^2!S_1/2",  
4.1771,
  "Sr", "Strontium",  87.62,       2,  6, 37, "^2",            "^1!S_0",    
5.6949,
  "Y",  "Yttrium",    88.90585,    3,  6,  0, "(Kr)4d5s^2",    "^2!D_3/2",  
6.2171,
  "Zr", "Zirconium",  91.224,      4,  6,  0, "(Kr)4d^2!5s^2", "^3!F_2",    
6.6339,
  "Nb", "Niobium",    92.90638,    5,  6,  0, "(Kr)4d^4!5s",   "^6!D_1/2",  
6.6589,
  "Mo", "Molybdenum", 95.94,       6,  6,  0, "(Kr)4d^5!5s",   "^7!S_3",    
7.0924,
  "Tc", "Technetium", 97.907215,   7,  6, 42, "^2",            "^6!S_5/2",  
7.28,
  "Ru", "Ruthenium",  101.07,      8,  6,  0, "(Kr)4d^7!5s",   "^5!F_2",    
7.3605, 
  "Rh", "Rhodium",    102.90550,   9,  6,  0, "(Kr)4d^8!5s",   "^4!F_9/2",  
7.4589,
  "Pd", "Palladium",  106.42,     10,  6,  0, "(Kr)4d^10",     "^1!S_0",    
8.3369,
  "Ag", "Silver",     107.8682,    1,  7, 46, "!5s",           "^2!S_1/2",  
7.5762,
  "Cd", "Cadmium",    112.411,     2,  7, 47, "^2",            "^1!S_0",    
8.9938,
  "In", "Indium",     114.818,     3,  7, 48, "!5p",           "^2!P_1/2",  
5.7864,
  "Sn", "Tin",        118.710,     4,  7, 49, "^2",            "^3!P_0",    
7.3439,
  "Sb", "Antimony",   121.760,     5,  7, 49, "^3",            "^4!S_3/2",  
8.6084,
  "Te", "Tellurium",  127.60,      6,  7, 49, "^4",            "^3!P_2",    
9.0096,
  "I",  "Iodine",     126.90447,   7,  7, 49, "^5",            "^2!P_3/2", 
10.4513,
  "Xe", "Xenon",      131.29,      8,  7, 49, "^6",            "^1!S_0",   
12.1298,
  
  "Cs", "Cesium",     132.90545,   1,  8,  0, "(Xe)6s",        "^2!S_1/2", 
3.8939,
  "Ba", "Barium",     137.327,     2,  8, 55, "^2",            "^1!S_0",   
5.2117,
/* Lantanides */
  "La", "Lanthanum",   138.9055,    1,  13, 0, "(Xe)5d6s^2",      "^2!D_3/2",  
5.5770,
  "Ce", "Cerium",      140.116,     2,  13, 0, "(Xe)4f5d6s^2",    "^2!G_4",    
5.5387,
  "Pr", "Praseodymium",140.90765,   3,  13, 0, "(Xe)4f^3!6s^2",   "^4!I_9/2",  
5.464,
  "Nd", "Neodymium",   144.24,      4,  13, 0, "(Xe)4f^4!6s^2",   "^5!I_4",    
5.5250,
  "Pm", "Promethium",  144.912745,  5,  13, 0, "(Xe)4f^5!6s^2",   "^6!H_5/2",  
5.58,
  "Sm", "Samarium",    150.36,      6,  13, 0, "(Xe)4f^6!6s^2",   "^7!F_0",    
5.6436,
  "Eu", "Europium",    151.964,     7,  13, 0, "(Xe)4f^7!6s^2",   "^8!S_7/2",  
5.6704,
  "Gd", "Gadolinium",  157.25,      8,  13, 0, "(Xe)4f^7!5d6s^2", "^9!D_2",    
6.1498,
  "Tb", "Terbium",     158.92534,   9,  13, 0, "(Xe)4f^9!6s^2",   "^6!H_15/2", 
5.8638,
  "Dy", "Dysprosium",  162.50,     10,  13, 0, "(Xe)4f^10!6s^2",  "^5!I_8",    
5.9389,
  "Ho", "Holmium",     164.93032,   6,  14, 0, "(Xe)4f^11!6s^2",  "^4!I_15/2", 
6.0215,
  "Er", "Erbium",      167.26,      7,  14, 0, "(Xe)4f^12!6s^2",  "^3!H_6",    
6.1077,
  "Tm", "Thulium",     168.93421,   8,  14, 0, "(Xe)4f^13!6s^2",  "^2!F_7/2",  
6.1843,
  "Yb", "Ytterbium",   173.04,      9,  14, 0, "(Xe)4f^14!6s^2",  "^1!S_0",    
6.2542,
  "Lu", "Lutetium",    174.967,    10,  14, 0, "(Xe)4f^14!5d6s^2","^2!D_3/2",  
5.4259,
/* End of lantanides */
  "Hf", "Hafnium",     178.49,      4,  8,  0, "(Xe)4f^14!5d^2!6s^2", "^3!F_2",   
6.8251,
  "Ta", "Tantalum",    180.9479,    5,  8,  0, "(Xe)4f^14!5d^3!6s^2", 
"^4!F_3/2", 7.5496,
  "W",  "Tungsten",    183.84,      6,  8,  0, "(Xe)4f^14!5d^4!6s^2", "^5!D_0",   
7.8640,
  "Re", "Rhenium",     186.207,     7,  8,  0, "(Xe)4f^14!5d^5!6s^2", 
"^6!S_5/2", 7.8335,
  "Os", "Osmium",      190.23,      8,  8,  0, "(Xe)4f^14!5d^6!6s^2", "^5!D_4",   
8.4382,
  "Ir", "Iridium",     192.217,     9,  8,  0, "(Xe)4f^14!5d^7!6s^2", 
"^4!F_9/2", 8.9670,
  "Pt", "Platinum",    195.078,    10,  8,  0, "(Xe)4f^14!5d^9!6s",   "^3!D_3",   
8.9587,
  "Au", "Gold",        196.96655,   1,  9,  0, "(Xe)4f^14!5d^10!6s",  
"^2!S_1/2", 9.2255,
  "Hg", "Mercury",     200.59,      2,  9, 79, "^2",                  "^1!S_0",  
10.4375,
  "Tl", "Thallium",    204.3833,    3,  9, 80, "!6p",                 
"^2!P_1/2", 6.1082,
  "Pb", "Lead",        207.2,       4,  9, 81, "^2",                  "^3!P_0",   
7.4167,
  "Bi", "Bismuth",     208.98038,   5,  9, 81, "^3",                  
"^4!S_3/2", 7.2855,
  "Po", "Polonium",    208.982415,  6,  9, 81, "^4",                  "^3!P_2",   
8.4167,
  "At", "Astatine",    209.987131,  7,  9, 81, "^5",                  
"^2!P_3/2",-1.,
  "Rn", "Radon",       222.017570,  8,  9, 81, "^6",                  "^1!S_0",  
10.7485,
  
  "Fr", "Francium",    223.019731,  1, 10,  0, "(Rn)7s",              
"^2!S_1/2", 4.0727,
  "Ra", "Radium",      226.025402,  2, 10, 87, "^2",                  "^1!S_0",   
5.2784,
/* Actinides */
  "Ac", "Actinium",     227.027747,  1, 16, 0, "(Rn)6d7s^2",          
"^2!D_3/2", 5.17,
  "Th", "Thorium",      232.0381,    2, 16, 0, "(Rn)6d^2!7s^2",       "^3!F_2",   
6.3067,
  "Pa", "Protactinium", 231.03588,   3, 16, 0, "(Rn)5f^2!6d7s^2",     
"^4!K_11/2",5.89, 
  "U",  "Uranium",      238.0289,    4, 16, 0, "(Rn)5f^3!6d7s^2",     "^5!L_6",   
6.1941,
  "Np", "Neptunium",    237.048166,  5, 16, 0, "(Rn)5f^4!6d7s^2",     
"^6!L_11/2",6.2657,
  "Pu", "Plutonium",    244.064197,  6, 16, 0, "(Rn)5f^6!7s^2",       "^7!F_0",   
6.0262,
  "Am", "Americium",    243.061372,  7, 16, 0, "(Rn)5f^7!7s^2",       
"^8!S_7/2", 5.9738,
  "Cm", "Curium",       247.070346,  8, 16, 0, "(Rn)5f^7!6d7s^2",     "^9!D_2",   
5.9915,
  "Bk", "Berkelium",    247.070298,  9, 16, 0, "(Rn)5f^9!7s^2",       
"^6!H_15/2",6.1979,
  "Cf", "Californium",  251.079579, 10, 16, 0, "(Rn)5f^10!7s^2",      "^5!I_8",   
6.2817,
  "Es", "Einsteinium",  252.08297,   6, 17, 0, "(Rn)5f^11!7s^2",      
"^4!I_15/2",6.42,
  "Fm", "Fermium",      257.095096,  7, 17, 0, "(Rn)5f^12!7s^2",      "^3!H_6",   
6.50,
  "Md", "Mendelevium",  258.098427,  8, 17, 0, "(Rn)5f^13!7s^2",      
"^2!F_7/2", 6.58,
  "No", "Nobelium",     259.1011,    9, 17, 0, "(Rn)5f^14!7s^2",      "^1!S_0",   
6.65,
  "Lr", "Lawrencium",   262.1098,   10, 17, 0, "(Rn)5f^14!7s^2!7p?", 
"^2!P_1/2",-1., 
/* End of actinides */
  "Rf", "Rutherfordium",261.1089,    4, 10, 0, 
"(Rn)5f^14!6d^2!7s^2!?","^3!F_2!?", 6.0,
  "Db", "Dubnium",      262.1144,    5, 10, 0, "?", "?", -1.,
  "Sg", "Seaborgium",   263.1186,    6, 10, 0, "?", "?", -1.,
  "Bh", "Bohrium",      262.1231,    7, 10, 0, "?", "?", -1.,
  "Hs", "Hassium",      265.1306,    8, 10, 0, "?", "?", -1.,
  "Mt", "Meitnerium",   266.1378,    9, 10, 0, "?", "?", -1.,
  "",  "Not named",    269.273,    10, 10, 0, "?", "?", -1.,
  "",  "Not named",    272,         1, 11, 0, "?", "?", -1.,
  "",  "Not named",    277,         2, 11, 0, "?", "?", -1.,
  "",  " ",            0,           3, 11, 0, "?", "?", -1.,
  "",  "Not named",    289,         4, 11, 0, "?", "?", -1.,
  "",  " ",            0,           5, 11, 0, "?", "?", -1.,
  "",  "Not named",    289,         6, 11, 0, "?", "?", -1.,
  "",  " ",            0,           7, 11, 0, "?", "?", -1.,
  "",  "Not named",    293,         8, 11, 0, "?", "?", -1.
};

void PutSmallFont(int x, int y, char ch) {
  int ch1, ch2, i;
  letter[0]=4; letter[1]=0; letter[2]=6; letter[3]=0;
  ch2 = (ch-32)/2;
  ch1 = (ch-32)%2;
  if (ch2>=0 && ch2<48) {
    if (ch1==0)
      for (i=0; i<6; i++) letter[i+4] = font[ch2][i] & 0xF0;
    else
      for (i=0; i<6; i++) letter[i+4] = (font[ch2][i]<<4) & 0xF0;
    LibPutGraph(x,y,letter);
  }
}

void PutSmallStr(int x, int y, char* string) {
  int cx=x;
  int i=0;
  do {
    PutSmallFont(cx,y,string[i]);
    if (string[i]=='i' || string[i]=='j' || string[i]=='l')
      cx+=3;
    else cx+=4;
    i++;
    if (string[i]=='i') cx-=1;
  } while(string[i] != 0x00);
}

int npage(int elem) {
  if (Elem[elem-1].y >= 10) return(2);
  else return(1);
}

int xcoord(int elem) {
  return (((int)(Elem[elem-1].x)-1)*14+10);
}

int ycoord(int elem) {
  if (npage(elem)==1) return (((int)(Elem[elem-1].y)-1)*16+9);
  else return (((int)(Elem[elem-1].y)-10)*16+9);
}

void Disp(int page) {
  int i;
  LibClrDisp();
  for (i=1; i<=8; i++) {
    char str[2];
    sprintf(str,"%d",i);
    LibPut35Str((i-1)*14+15, 2, str);
  }
  /* Draw up/down button */
  LibGdsBox(125,142,157,157);
  if (page==1) {
    int y = 7*16+9;
    LibGdsBox(38,y,38+14,y+16);
    LibPut35Str(38+2, y+2, "57-");
    LibPut35Str(38+4, y+8, "71");
    LibPut35Str(3,14,"1");
    LibPut35Str(3,30,"2");
    LibPut35Str(3,46,"3");
    LibPut35Str(3,70,"4");
    LibPut35Str(3,102,"5");
    LibPut35Str(3,134,"6");
    LibPutProStr(IB_PFONT2,30,12,"Periodic table",80);
    LibPutProStr(IB_PFONT2,128,146,"Down",30);
  }
  else {
    int y = 9;
    LibGdsBox(38,y,38+14,y+16);
    LibPut35Str(38+2, y+2, "89-");
    LibPut35Str(38+2, y+8, "103");
    LibPut35Str(3,22,"7");
    LibPutProStr(IB_PFONT1,10,2*16+14,"Lantanide series",100);
    LibPutProStr(IB_PFONT1,10,5*16+14,"Actinide series",100);
    PutSmallStr(5,142,"Written by Anton Poluektov");
    PutSmallStr(5,149,"A.O.Poluektov@inp.nsk.su");
    LibPutProStr(IB_PFONT2,135,146,"Up",30);
  }
  for (i=1; i<=NUM_ELEMENTS; i++) {
    if (npage(i) == page && Elem[i-1].weight != 0) {
      char str[4];
      int x=xcoord(i);
      int y=ycoord(i);
      sprintf(str,"%d",i);
      LibGdsBox(x,y,x+14,y+16);
      LibPut35Str(x+2,y+2,str);
      LibPutProStr(IB_CG57FONT, x+14-LibGetProStrSize(IB_CG57FONT,Elem[i-
1].sym),
	y+8,Elem[i-1].sym,15);
    }
  }
  LibPutDisp();
}

void PutMath(int x, int y, char* string) {
  int i=0;
  int cx=x;
  int level=0;
  do {
    if (string[i] == '!') level=0;
    else if (string[i] == '^') level=1;
    else if (string[i] == '_') level=-1;
    else {
      if (level==0) {
	LibPutProFont(IB_PFONT1,string[i],cx,y);
	cx += LibGetProFontSize(IB_PFONT1,string[i]);
      }
      else {
	PutSmallFont(cx,y-level*4+1,string[i]);
	cx+=4;
      }
    }
    i++;
  } while(string[i] != 0x00);
}

int GetMathWidth(char* string) {
  int i=0;
  int cx=0;
  int level=0;
  do {
    if (string[i] == '!') level=0;
    else if (string[i] == '^' || string[i] == '_') level=1;
    else {
      if (level==0)
	cx += LibGetProFontSize(IB_PFONT1,string[i]);
      else
	cx+=4;
    }
    i++;
  } while(string[i] != 0x00);
  return(cx);
}

char* GetEConf(int elem) {
  static char conf[100];
  char swap[100];
  int celem = elem;
  conf[0]=0x00;
  do {
    strcpy(swap,conf);
    strcpy(conf, Elem[celem-1].estr);
    strcat(conf,swap);
    celem=Elem[celem-1].estrbase;
  } while(celem != 0);
  return(conf);
}

void periodicmain (void)
{
  TCHSTS tsts;
  int curpage=1;
  int exit=0;

  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchActionKey);
  LibTchStackPush(TchList);

  LibInitDisp();
  Disp(curpage);
/*  for (i=32; i<64; i++) {
    PutSmallFont((i-32)*4,0,i);
    PutSmallFont((i-32)*4,6,i+32);
    PutSmallFont((i-32)*4,12,i+64);
  }
  LibPutDisp(); */
  LibTchInit();

  do {
    int i,elem;
    LibTchWait(&tsts);
    switch (tsts.obj) {
      case OBJ_IC_PAD:
	elem = 0;
	for (i=1; i<=NUM_ELEMENTS; i++)
	  if (npage(i) == curpage && Elem[i-1].weight != 0.) {
	    int x=xcoord(i);
	    int y=ycoord(i);
	    if (tsts.x > x && tsts.x < x+14 &&
		tsts.y > y && tsts.y < y+15) {
	      elem = i;
	      break;
	    }
	  }
	if (elem != 0) {
	  char str[30];
	  char* econf;
	  int x=xcoord(elem);
	  int y=ycoord(elem);
	  LibGdsReverse(x+1,y+1,x+13,y+15);
	  LibPutDisp();
	  LibTchWait(&tsts);
	  LibGdsReverse(x+1,y+1,x+13,y+15);

	  LibOpenWindowS(ON,30,30,100,90);
	  LibPutProStr(IB_PFONT2,32,32,Elem[elem-1].sym,20);
	  LibPutProStr(IB_PFONT2,
		128-LibGetProStrSize(IB_PFONT2, Elem[elem-1].name),32,
		Elem[elem-1].name,100);
	  LibGdsReverse(31,31,128,40);
	  LibPutProStr(IB_PFONT1,32,42,"Number",50);
	  sprintf(str,"%d",elem);
	  LibPutProStr(IB_PFONT1,80,42,str,50);
	  LibPutProStr(IB_PFONT1,32,54,"Atom. wt:",50);
	  sprintf(str,"%g",Elem[elem-1].weight);
	  LibPutProStr(IB_PFONT1,80,54,str,50);

	  LibPutProStr(IB_PFONT1,32,66,"Ion. engy:",50);
	  if (Elem[elem-1].ie > 0)
	    sprintf(str,"%g eV",Elem[elem-1].ie);
	  else
	    sprintf(str,"?");
	  LibPutProStr(IB_PFONT1,80,66,str,50);
	  PutMath(32,81,"^2S+1!L_J! :");
	  PutMath(80,81,Elem[elem-1].gst);

	  econf=GetEConf(elem);
	  LibPutProStr(IB_PFONT1,32,96,"Electron config.:",100);
	  PutMath(128-GetMathWidth(econf),110,econf);

	  LibGdsLine(31,40,129,40);
	  LibGdsLine(31,52,129,52);
	  LibGdsLine(31,64,129,64);
	  LibGdsLine(31,75,129,75);
	  LibGdsLine(31,94,129,94);
	  LibGdsLineMesh(78,41,78,93);

	  LibPutDisp();
	  LibTchWait(&tsts);
	  LibCloseWindow();
	  LibTchWait(&tsts);
	  LibPutDisp();
	}
	else if (tsts.x>125 && tsts.x<157 &&
		 tsts.y>142 && tsts.y<157) {
	  curpage = 3-curpage;
	  LibGdsReverse(126,143,156,156);
	  LibPutDisp();
	  LibTchWait(&tsts);
	  Disp(curpage);
	}
	break;
      case OBJ_LPSW_UP:
	curpage=1;
	Disp(curpage);
	break;
      case OBJ_LPSW_DOWN:
	curpage=2;
	Disp(curpage);
	break;
      case OBJ_HIC_POS1:
      case OBJ_HIC_ESC:
      case OBJ_HIC_OFF:
	exit=1;
      default:
        break;
    }
    if (tsts.act == ACT_ALM) LibAlarm();
  } while(exit==0);

  LibJumpMenu();
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
