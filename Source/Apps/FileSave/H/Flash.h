/******************************************************************************
*
*	[Title]		Flash file operations
*               (Copied and changed from the Kino program,
*               by Wittawatt Wamyong)
*
*	[Model] 	PocketViewer
*	[Version]	Ver1.00
*
******************************************************************************/

#ifndef  FLASH_H
#define  FLASH_H

/*********** Macros ************/

/* Flash Files */
#define FILE_NAME  "FileSaveData"    /* Global file name for the storage Flash file */
#define INDEX_NAME "FileSaveIndex"   /* Global File Name for the index Flash file */
#define BINBUF(fb)	((fb).fbuf.bin.bin_buf)
#define BINBUF_SIZE(fb)	((fb).fbuf.bin.char_num)

#define ASSIGN_BUF(fb,f) do {			\
	(fb).fsb_main_entry_ = (f).main;	\
	(fb).fsb_sub_entry_ = (f).sub;		\
	(fb).fsb_scrt_info_ = (f).area;		\
 } while (0)

#define FM_CREATE		1
#define FM_SECRET		2
#define FM_NEW			(FM_CREATE | 4)

/* Serial & blocks */
#define MAX_SER_BUF    1024 /* maximum size for each serial reading */
#define MAX_RECORD     2048 /* max records per file */
#define HEADER_LEN     256
#define BLOCK_SIZE     1024
#define COMMENT_LEN    128
#define FILENAME_POS   0
#define FILENAME_LEN   128
#define FILESIZE_LEN   12
#define CHECKSUM_LEN   12



/*********** Structs ************/

typedef struct {
	byte	main,sub;
	byte	area;
} TPvFile;

typedef struct {
	byte	FileName[FILENAME_LEN + 1];
	long	Size;
	int     nRecord;
} THeader;

typedef struct {
	byte	Data[BLOCK_SIZE];
	long	Size;
	long    Checksum;
} TRecord;

typedef struct {
	word    fp;  /* Pointer to the starting position in the storage file */
	byte	FileName[FILENAME_LEN + 1];
	long	Size;
	int     nRecord;
	byte    Comment[COMMENT_LEN];
} TFileIndex;

typedef struct LinkedList {
	TFileIndex        IndexRecord;  /* Place to store record data */
	struct LinkedList *next;        /* Pointer to next element in the list */
	                                /* (NULL if current one is the last one) */
} TIndexList;




/*********** Function Prototypes ************/

bool initStorageFile(void);
bool initIndexFile(void);
int  PvOpenFile(char *filename, TPvFile *f, int fmode);
bool saveHeader(int i);
bool saveRecord(void);
void loadHeader(int i);
void loadRecord(void);
bool deleteRecord(int idx);
bool loadIndexRecord(TFileIndex *IndexRecord);
void fatalError(char *msg);


#endif /* FLASH_H*/