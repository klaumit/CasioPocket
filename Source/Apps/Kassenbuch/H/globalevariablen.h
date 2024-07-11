#define DATAFILENAME "KASSENBUCH"
#define NAME_MAX      12
#define OBJ_STRING    0xA029

#define POSTEN_1      "Essen"
#define POSTEN_2      "Kantine"
#define POSTEN_3      "Hobby"
#define POSTEN_4      "Tanken"
#define POSTEN_5      "Verschiedenes"

extern char *GetMsg(int);
extern char *GetPosten(int);
extern DialogWin_SaveOk();
extern DialogWin_Wait();
extern DialogWin_LoadOk();
extern DialogWin_Loading();
extern DialogWin_Saving();


/* Globale Variablen */
int test=0;
int count=0;
int zaehler=0;
int zufallzahl=0;
int exit=0;

char name[NAME_MAX];
float betrag=0.0;
float summe=0.0;
char str[10];

char werte_str[5][10];
char werte_ges[10] = "0.00\n";
char werte_letzte[10] = "0.00\n";
bool check[5] = { {FALSE}, {FALSE}, {FALSE}, {FALSE}, {FALSE} };


/* Structure for FileInfo */
struct
{
  float ini_posten[5];
  float ini_gesamt;
  float ini_letzte;
}
FileInfo;



