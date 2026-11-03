/**************************************************************
                                                        980718
   Include File: st_std.h
   Description:  Helper functions for generic programs.
**************************************************************/

#ifndef __stdio_h
#include <stdio.h>
#endif

/* DATE FUNCTIONS
***********************************/
#define isleap(year) ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0))

int DateSerial(char *szDate);
char *SerialDate(int iDateSerial);
int DayOfWeek(char *szDate);
char *DateToday(int iFlag);
int MonthLen(int iYear, int iMonth);


/* ARRAY FUNCTIONS
***********************************/
void *ArrayNew(unsigned int iItemLen, unsigned int iAntal);
void ArrayFree(char *array);
void *ArrayResize(char *array, unsigned int iItemLen, unsigned int iNewSize);
void *ArrayInsert(char *array, unsigned int iItemLen, unsigned int iMaxValue, unsigned int iPosition);
void *ArrayAppend(char *array, unsigned int iItemLen, unsigned int iMaxValue);
void *ArrayDelete(char *array, unsigned int iItemLen, unsigned int iMaxValue, unsigned int iPosition);
void ArrayCopy(char *array, unsigned int iItemLen, unsigned int iFrom, unsigned int iTo);
void ArraySwap(char *array, unsigned int iItemLen, unsigned int iItem1, unsigned int iItem2);


/* MEMO FUNCTIONS
***********************************/
typedef struct _MEMOHEADER    /* memoheader */
        {
        char            szMemoFile[4];
        unsigned long   ulMemoId;
        unsigned long   ulMemoActive;
        char            szDate[6];
        } MEMOHEADER;

typedef MEMOHEADER *PMEMOHEADER;


typedef struct _MEMOITEM    /* memoitem */
        {
        unsigned long   ulMemoId;
        unsigned long   ulMemoLen;
        char            sMemoActive;
        char            szDate[6];
        unsigned long   ulNextMemoPos;
        } MEMOITEM;

typedef MEMOITEM *PMEMOITEM;

int SearchId(FILE *stream, unsigned long ulMemoId);
void WriteHeader(FILE *stream, PMEMOHEADER pmh, int iFlag);
void ReadHeader(FILE *stream, PMEMOHEADER pmh);

BOOL MemoInfo(char* szMemoFile, PMEMOHEADER pmh);
char* MemoGet(char* szMemoFile, unsigned long ulMemoPos);
char* MemoGetId(char* szMemoFile, unsigned long ulMemoId);
int MemoQueryId(char* szMemoFile, unsigned long ulMemoPos);
int MemoQueryPos(char* szMemoFile, unsigned long ulMemoId);
int MemoLen(char* szMemoFile, unsigned long ulMemoPos);
int MemoLenId(char* szMemoFile, unsigned long ulMemoId);
int MemoPut(char* szMemoFile, unsigned long ulMemoSize, char* szMemo);int MemoPutId(char* szMemoFile, unsigned long ulMemoSize, char* szMemo);
BOOL MemoDelete(char* szMemoFile, unsigned long ulMemoPos);
BOOL MemoDeleteId(char* szMemoFile, unsigned long ulMemoId);


/* FILE FUNCTIONS
***********************************/
#define FILE_COUNT_MAX_FILES    250

#define FILE_COUNT_MAX_PATH     255

#define FILE_COUNT_NORMAL       0
#define FILE_COUNT_READONLY     1
#define FILE_COUNT_HIDDEN       2
#define FILE_COUNT_SYSTEM       4
#define FILE_COUNT_ARCHIVE      32

#define FILE_COUNT_SORTNONE     0
#define FILE_COUNT_SORTNAME     1
#define FILE_COUNT_SORTEXT      2
#define FILE_COUNT_SORTSIZE     4
#define FILE_COUNT_SORTDATE     8

typedef struct _FILELISTENTRY
        {
        char            szName[FILE_COUNT_MAX_PATH];
        int             iFileSize;
        int             iAttrib;
        short           sYear;
        char            cMonth;
        char            cDay;
        char            cHour;
        char            cMinute;
        char            cSecond;
        } FILELISTENTRY;
typedef FILELISTENTRY *PFILELISTENTRY;

typedef struct _FILELIST
        {
        int             iFiles;
        int             iSizeTotal;
        int             iSizeAlloc;
        PFILELISTENTRY  pfl;
        } FILELIST;
typedef FILELIST *PFILELIST;

int FileCount (BOOL bFlag, char* szPathWild, int iAttributes);
int FileGet (BOOL bFlag, char* szPathWild, PFILELIST pfl, int iAttributes, int iSortOrder);

int FileCountDirectories (char* szPathWild);
int FileCountFiles (char* szPathWild, int iAttributes);
int FileGetFiles (char* szPathWild, PFILELIST pfl, int iAttributes, int iSortOrder);
int FileGetDirectories (char* szPathWild, PFILELIST pfl, int iSortOrder);

char* GetExeDir (void);
void SetExeDirCurrent (void);
int GetDLLFullName(char *szDLLName, char *szFullPath);
int CheckForPreviousInstance (char *szAppName, int iFirstInstance);
int GetInstanceData(char *szAppName, char *szAppData);
int SetInstanceData(char *szAppName, int iAppDataLen, char *szAppData);
char *FileGetPath (char* szFullFile);
char *FileGetFile (char* szFullFile);
char *FileGetExt (char* szFullFile);
char *FileGetFileExt (char* szFullFile);
void FileSetDir (char* szFullFile);


/* STD FUNCTIONS
***********************************/
char *strxtract(char *szString, int iPos1, int iPos2);
char *strpostrim(char *szString, int iPosition);
char* fgetsz(char *, int, FILE *);
void strtrim (char* szString);
char *strltrimn(char *szString, int iPosition);
char *strrtrimn(char *szString, int iPosition);
void strreplace(char *szText, char sLookFor, char sReplaceWith);
void strterminate(char *szText, char sTermChar);

// Private 
//-------------------------------------------------------------------------------
void debugLog(char *szFormat, ...);
