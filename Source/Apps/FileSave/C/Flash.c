/******************************************************************************
*	
*	[Title]	  Flash File Operations 
*             (Many parts copied and changed from the Kino program,
*             by Wittawatt Wamyong)
* 
******************************************************************************/

#include	<stdrom.h>

#include	"define.h"
#include	"libc.h"
#include	"L_define.h"
#include	"L_libc.h"

#include    "Flash.h"
#include    "Common.h"

/********** Exernal variables **************/
extern TFileIndex IndexArray[MAX_FILES];   /* Array of indexes to files */
extern TRecord    Record;
extern FILE_BUF   fb;
extern FILE_INFO  fi;
extern FILE_BUF   idx_fb;
extern FILE_INFO  idx_fi;
/* extern word       DataFP[MAX_RECORD];  */
extern TPvFile    hStorage;
extern TPvFile    hIndex;
extern byte       FileName[];
extern byte       IndexName[];

/******************************************************************************
	[Title]	File operation (copied from Kino)
    [Arguments]	
	[ReturnVAL]	
******************************************************************************/
bool initStorageFile(void)
{

	if (PvOpenFile(FileName, &hStorage, 0) == 0) {
		/* File exist... Delete to replace */
		fi.fp = 0xffff;
		fi.kind = FILE_KIND_BIN;
		ASSIGN_BUF(fb, hStorage);
		if (!LibFileFindNext(&fb,&fi,0) ||
			!LibFileRemoveAll(&fb))
			return (FALSE);	
	}

	/* Create a new file */
	if (PvOpenFile(FileName, &hStorage, FM_CREATE) != 0)
		return (FALSE);

	return (TRUE);
}

/******************************************************************************
	[Title]	File operation (copied from Kino)
	[Return]
******************************************************************************/
bool initIndexFile(void)
{

	if (PvOpenFile(IndexName, &hIndex, 0) == 0) {
		/* File exist... Go to the beginning */
		idx_fi.fp = 0xffff;
		idx_fi.kind = FILE_KIND_BIN;
		ASSIGN_BUF(idx_fb, hIndex);
	}

	/* File not exist... create a new file */
	if (PvOpenFile(IndexName, &hIndex, FM_CREATE) != 0)
		return (FALSE);

	return (TRUE);

}

/******************************************************************************
	[Title]	File operation (copied from Kino)
    [Arguments]	
	[ReturnVAL]	
******************************************************************************/
int PvOpenFile(char *fn, TPvFile *f, int fmode)
{
	if (((fmode & FM_NEW) == FM_NEW) &&
	    LibSubEntrySearch(fn,&f->sub) == TRUE)
		return (-1); /* file exists */
	if (fmode & FM_CREATE) {
		if (LibSubEntrySave(fn, &f->sub) == FALSE)
			return (-1);
	} else {
		if (LibSubEntrySearch(fn,&f->sub) == FALSE)
			return (-1);
	}

	LibGetAllEntry(fn, &f->main, &f->sub);
	if (fmode & FM_SECRET) {
		f->area = FILE_SECRET_MODE; 
	} else {
		f->area = FILE_OPEN_MODE;
	}

	return (0);
}

/******************************************************************************
	[Title]	 saveHeader
    [Descr]  Save the header in the index flash file
             File operation (copied from Kino)
    [Args]   index to the element in the array of file indexes
	[Return] 0 if no error	
******************************************************************************/
bool saveHeader(int i)
{

	idx_fi.fp = 0xffff; /* Append */
	idx_fi.kind = FILE_KIND_BIN;
	ASSIGN_BUF(idx_fb, hIndex);
	if (LibFileFindNext(&idx_fb,&idx_fi,0) == FALSE) {
		idx_fi.fp = 0xffff;
		idx_fi.kind = FILE_KIND_BIN;
	}
	BINBUF_SIZE(idx_fb) = sizeof(TFileIndex);
	memcpy(BINBUF(idx_fb),
        &IndexArray[i],
        BINBUF_SIZE(idx_fb));

	return (LibFileWrite(&idx_fb,&idx_fi));

}

/******************************************************************************
	[Title]	Append a record to the saved ones
    [Arguments]	
	[ReturnVAL]	
******************************************************************************/
bool saveRecord(void)
{

	fi.fp = 0xffff; /* Append the record after the last one
	                   already saved on the same file*/

	/* This is to access and save a single record,
	   if the 'fp' pointers created when the record was
	   first saved have been put in an array
	   (we're saving the record in the list)

       if (idx > Header.nRecord) return (FALSE);
	   fi.fp = DataFP[idx]; 
	*/

	fi.kind = FILE_KIND_BIN;

	ASSIGN_BUF(fb, hStorage);
	BINBUF_SIZE(fb) = sizeof(Record);
	memcpy(BINBUF(fb),&Record,BINBUF_SIZE(fb));

	/* Write */
	if (LibFileWrite(&fb,&fi) == FALSE) {
		LibWinIcnMsg(ICON_BADTZ,324,2);
		return(FALSE);
	}

	/* Now we could save the 'fp' pointer for later reference 
	DataFP[idx] = fi.fp; 
	*/

	return(TRUE);
}

/******************************************************************************
	[Title]	loadHeader
    [Descr] Retreive from Flash mem the index data
            for a single file.
            File operation (mostly copied from Kino)
    [Args]  Index to the element in the array of file indexes

******************************************************************************/
void loadHeader(int i)
{
	/* Load header from Flash index file */

	ASSIGN_BUF(fb, hIndex);
	idx_fi.kind = FILE_KIND_BIN;
	idx_fi.fp = 0xffff; /* <--- this should be initialized if headers are indexed */
	if (LibFileFindNext(&idx_fb,&idx_fi,0) == FALSE ||
	    LibFileRead(&idx_fb,&idx_fi) == FALSE ||
	    BINBUF_SIZE(idx_fb) != sizeof(TFileIndex)) {
		fatalError("Corrupted file! (h)");
	}

    /* Tranfer data into the current element
       of the array of file indexes */
	memcpy(&IndexArray[i],
        BINBUF(idx_fb),
        sizeof(TFileIndex));

}

/******************************************************************************
	[Title]	File operation (mostly copied from Kino)
    [Arguments]	
	[ReturnVAL]	
******************************************************************************/
void loadRecord(void)
{
	/* Load next record from Flash */
	/* Assume previous record (or header) alredy read */
	/* (fi and fb already initialized) */
	if (LibFileFindNext(&fb,&fi,0) == FALSE ||
		LibFileRead(&fb,&fi) == FALSE ||
	    BINBUF_SIZE(fb) != sizeof(Record)) {
		fatalError("Corrupted file! (r)");
	}

	memcpy(&Record,BINBUF(fb),sizeof(Record));
}

/******************************************************************************
	[Title]	File operation (copied from Kino)
    [Arguments]	
	[ReturnVAL]	
******************************************************************************/
bool deleteRecord(int n)
{

 /*
 Use the file index instead ()
	fi.fp = IndexArray[n].fp;
    and all the following record, for 
    IndexArray[n].nRecords times
*/ 
	fi.kind = FILE_KIND_BIN;
	if (LibFileRemove(&fb,&fi) != TRUE)
		return (FALSE);

	return (TRUE);

}


/******************************************************************************
	[Title]	File operation (mostly copied from Kino)
    [Arguments]	Returns a pointer to loaded record
	[ReturnVAL] TRUE: record loaded; FALSE: End Of File	
******************************************************************************/
bool loadIndexRecord(TFileIndex *IndexRecord)
{

	/* Load next record from Flash index file */
	/* Assume fi and fb already initialized */

	if (LibFileFindNext(&idx_fb,&idx_fi,0) == FALSE) {
		return (FALSE); /* Not found (EOF) */
	}

	if (LibFileRead(&idx_fb,&idx_fi) == FALSE ||
	    BINBUF_SIZE(idx_fb) != sizeof(TFileIndex)) {
		fatalError("Corrupted file! (r)"); /* Never return */
	}

	/* Record found */
	IndexRecord = (TFileIndex *)&(BINBUF(idx_fb));
	return (TRUE);
}

bool _loadIndexRecord(TFileIndex *IndexRecord)
{  /***** ONLY FOR DEBUG ****/
    
static i = 0;
TFileIndex  debug_record;

    if (i++ < 5) { /* Create some dummy records */
        sprintf(debug_record.Comment, 
            "Commento al file num. %d", i);
	    sprintf(debug_record.FileName, "Nome File %d", i);
        debug_record.fp = i;
        debug_record.nRecord = i;
        debug_record.Size = i;
	    memcpy (IndexRecord, &debug_record, sizeof(TFileIndex));
	    return (TRUE);
    }
    else
        return (FALSE);
}

/******************************************************************************
	[Title]		Fatal error message and exit function
	[Arguments]	
	[ReturnVAL]	
******************************************************************************/
void fatalError(char *msg)
{
	LibPutMsgDlg(msg);
	LibJumpMenu();
	/* never returns */

	/* Use OFF to get out of this */

}

