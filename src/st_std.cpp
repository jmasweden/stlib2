
#define INCL_NOPMAPI
#define INCL_DOS
#define INCL_DOSERRORS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <memory.h>
#include <direct.h>

#include "st_std.h"

int aiMonthLen[13];

/*------------------------------------------------------------------------------
   strxtract: Extract part of string.
-----------------------------------------------------------------------------*/
char *strxtract(char *szString, int iPos1, int iPos2)
     {
     int            iCounter, iAntal;
     static char    szReturn[_MAX_PATH];


     memset (szReturn, 0, _MAX_PATH);
     iAntal = 0;
     for (iCounter = (iPos1 - 1); iCounter < (iPos1 + iPos2 - 1); iCounter++)
         {
         szReturn[iAntal] = szString[iCounter];
         iAntal++;
         }

     return (szReturn);
     }


/*------------------------------------------------------------------------------
   strpostrim: Trims a string from within string.
-----------------------------------------------------------------------------*/
char *strpostrim(char *szString, int iPosition)
     {
     int        iCounter;


     for (iCounter = iPosition - 1; iCounter > 0; iCounter--)
         {
         if (szString[iCounter] != ' ')
            {
            szString[iCounter + 1] = '\0';
            break;
            }
         }
     return (szString);
     }


/*-----------------------------------------------------------------------------
   fgetsz: fgets that replaced newline with \0.
-----------------------------------------------------------------------------*/
char* fgetsz (char *szBuffer, int iMaxLen, FILE *stream)
     {
     fgets(szBuffer, iMaxLen, stream);

     if (szBuffer[strlen(szBuffer) - 1] == '\n')
        szBuffer[strlen(szBuffer) - 1] = '\0';
     return (szBuffer);
     }


/*------------------------------------------------------------------------------
   strtrim: Trims string (removes blanks in begin and end).
-----------------------------------------------------------------------------*/
void strtrim (char* szString)
     {
     char       szTempString[1024];
     int        iCounter, iStrLen;
     int        iBlanks;


     if ( (iStrLen = strlen(szString)) == 0 )
        return;

     iBlanks = 0;

     for (iCounter = 0; iCounter < iStrLen; iCounter++)
         {
         if (szString[iCounter] == ' ')
            iBlanks++;
         else
            break;
         }

     if (iBlanks > 0)
        strcpy(szTempString, &szString[iBlanks]);
     else
        strcpy(szTempString, szString);

     for (iCounter = strlen(szTempString) - 1; iCounter > -1; iCounter--)
         {
         if (szTempString[iCounter] == ' ')
            szTempString[iCounter] == '\0';
         else
            break;
         }

     strcpy(szString, szTempString);
     }


/*------------------------------------------------------------------------------
   strltrimn: Trims n character from left in string.
-----------------------------------------------------------------------------*/
char *strltrimn(char *szString, int iPosition)
     {
     int        iCounter;

     for (iCounter = 0; iCounter <= strlen(szString) - iPosition; iCounter++)
         {
         szString[iCounter] = szString[iPosition + iCounter];
         }
     szString[iCounter] = '\0';
     return (szString);
     }


/*------------------------------------------------------------------------------
   strrtrimn: Trims n character from right in string.
-----------------------------------------------------------------------------*/
char *strrtrimn(char *szString, int iPosition)
     {
     szString[strlen(szString) - iPosition] = '\0';
     return (szString);
     }

/*-----------------------------------------------------------------------------
   strreplace: Replaces all sLookFor with sReplaceWith.
-----------------------------------------------------------------------------*/
void strreplace(char *szText, char sLookFor, char sReplaceWith)
     {
     char       *pchFound;


     /* Change all sLookFor to sReplaceWith ones */
     pchFound = strrchr(szText, sLookFor);
     while (pchFound != 0)
           {
           (*pchFound) = sReplaceWith;
           pchFound = strrchr(szText, sLookFor);
           }
     }


/*-----------------------------------------------------------------------------
   strterminate: Terminates string with sTermChar.
-----------------------------------------------------------------------------*/
void strterminate(char *szText, char sTermChar)
     {
     if (szText[strlen(szText) - 1] != sTermChar)
        {
        szText[strlen(szText)] = sTermChar;
        szText[strlen(szText) + 1] = '\0';
        }
     }

/*************************************************************************************
  ArrayNew: Creates a new array.
*************************************************************************************/
void *ArrayNew(unsigned int iItemLen, unsigned int iAntal)
     {
     return calloc(iAntal, iItemLen);
     }


/*************************************************************************************
  ArrayFree: Frees (deletes) a array.
*************************************************************************************/
void ArrayFree(char *array)
     {
     free(array);
     }


/*************************************************************************************
  ArrayResize: Resizes an array.
*************************************************************************************/
void *ArrayResize(char *array, unsigned int iItemLen, unsigned int iNewSize)
     {
     array = (char*) realloc (array, (iNewSize + 1) * iItemLen);
     return (array);
     }


/*************************************************************************************
  ArrayInsert: Inserts item to an existing array.
*************************************************************************************/
void *ArrayInsert(char *array, unsigned int iItemLen, unsigned int iMaxValue, unsigned int iPosition)
     {
     unsigned int   iCounter;
     unsigned int   iArraySize;

     iArraySize = iMaxValue + 1;

     array = (char*) realloc (array, (iArraySize + 1) * iItemLen);

     for (iCounter = iPosition; iCounter < (iMaxValue + 1); iCounter++)
         memcpy(&array[(iCounter + 1) * iItemLen], &array[iCounter * iItemLen], iItemLen);

     return (array);
     }


/*************************************************************************************
  ArrayAppend: Appends item to the end of an array.
*************************************************************************************/
void *ArrayAppend(char *array, unsigned int iItemLen, unsigned int iMaxValue)
     {
     unsigned int   iArraySize;

     iArraySize = iMaxValue + 1;
     array = (char*) realloc (array, (iArraySize + 1) * iItemLen);
     return (array);
     }


/*************************************************************************************
  ArrayDelete: Deletes item from an existing array.
*************************************************************************************/
void *ArrayDelete(char *array, unsigned int iItemLen, unsigned int iMaxValue, unsigned int iPosition)
     {
     unsigned int   iCounter;
     unsigned int   iArraySize;

     iArraySize = iMaxValue + 1;

     for (iCounter = iPosition; iCounter < iMaxValue; iCounter++)
         memcpy(&array[iCounter * iItemLen], &array[(iCounter + 1) * iItemLen], iItemLen);

     array = (char*) realloc (array, (iArraySize - 1) * iItemLen);

     return (array);
     }


/*************************************************************************************
  ArrayCopy: Copies item in array to another item.
*************************************************************************************/
void ArrayCopy(char *array, unsigned int iItemLen, unsigned int iFrom, unsigned int iTo)
     {
     memcpy(&array[iTo * iItemLen], &array[iFrom * iItemLen], iItemLen);
     }


/*************************************************************************************
  ArraySwap: Copies item in array to other item.
*************************************************************************************/
void ArraySwap(char *array, unsigned int iItemLen, unsigned int iItem1, unsigned int iItem2)
     {
     char   *tmpBuffer;

     tmpBuffer = (char*) malloc(iItemLen);
     memcpy(&tmpBuffer, &array[iItem2 * iItemLen], iItemLen);
     memcpy(&array[iItem2 * iItemLen], &array[iItem1 * iItemLen], iItemLen);
     memcpy(&array[iItem1 * iItemLen], &tmpBuffer, iItemLen);
     }


/*-----------------------------------------------------------------------------
  MonthLen: Return correct lenght of specified year's months.
-----------------------------------------------------------------------------*/
int MonthLen(int iYear, int iMonth)
     {
     int                iCounter;

     for (iCounter = 1; iCounter <= 12; iCounter++)
         aiMonthLen[iCounter] = 31;

     aiMonthLen[2] = 28;
     aiMonthLen[4] = 30;
     aiMonthLen[6] = 30;
     aiMonthLen[9] = 30;
     aiMonthLen[11] = 30;

     if (isleap(iYear))
        aiMonthLen[2] = 29;

     return (aiMonthLen[iMonth]);
     }

/*-----------------------------------------------------------------------------
  DateSerial: Converts datestring to serial number.
-----------------------------------------------------------------------------*/
int DateSerial(char *szDate)
     {
     char               szTmpDate[10];

     time_t             ltime;
     struct tm          *curtime;

     int                iYear, iMonth, iDay;
     int                iCounter;
     int                iDays;

     if (strlen(szDate) == 0)
        {
        time(&ltime);
        curtime = localtime(&ltime);
        sprintf(szTmpDate, "%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);
        }
     else
        strcpy(szTmpDate, szDate);

     sscanf(szTmpDate, "%2d%2d%2d", &iYear, &iMonth, &iDay);

     iDays = 0;
     for (iCounter = 1;iCounter < (iYear - 79); iCounter++)
         {
         if (isleap(1979 + iCounter))
            iDays = iDays + 366;
         else
            iDays = iDays + 365;
         }

     MonthLen(1980 + iYear, 1);

     for (iCounter = 1;iCounter < iMonth; iCounter++)
         {
         iDays = iDays + aiMonthLen[iCounter];
         }

     return (iDays + iDay);

     }


/*-----------------------------------------------------------------------------
  SerialDate: Converts a serial number to a datestring.
-----------------------------------------------------------------------------*/
char *SerialDate(int iDateSerial)
     {
     static char        szTmpDate[10];

     char               szYear[3], szMonth[3], szDay[3];
     char               szTmpMonth[3], szTmpDay[3];

     int                iYear, iMonth, iDay;
     int                iCounter;
     int                iDays, iRest;
     int                iLast;


     iDays = 0;
     for (iCounter = 1; iCounter < 20; iCounter++)
         {
         if (isleap(1979 + iCounter))
            {
            iDays = iDays + 366;
            iLast = 366;
            }
         else
            {
            iDays = iDays + 365;
            iLast = 365;
            }

         if (iDays >= iDateSerial)
            {
            iYear = 79 + iCounter;
            if (iDays == iDateSerial)
               {
               iRest = 0;
               iMonth = 12;
               iDay = 31;
               }
            else
               {
               iRest = iDateSerial - (iDays - iLast);
               }

            break;
            }
         }


     if (iRest != 0)
        {
        MonthLen(1900 + iYear, 1);
        for (iCounter = 1; iCounter <= 12; iCounter++)
            {

            iRest = iRest - aiMonthLen[iCounter];
            iLast = aiMonthLen[iCounter];

            if (iRest <= 0)
               {
               iMonth = iCounter;

               if (iRest == 0)
                  iDay = aiMonthLen[iCounter];
               else
                  iDay = iRest + iLast;
               break;
               }
            }
        }

     _itoa(iYear, szYear, 10);

     _itoa(iMonth, szTmpMonth, 10);
     if (strlen(szTmpMonth) == 1)
        sprintf(szMonth, "0%s", szTmpMonth);
     else
        strcpy(szMonth, szTmpMonth);

     _itoa(iDay, szTmpDay, 10);
     if (strlen(szTmpDay) == 1)
        sprintf(szDay, "0%s", szTmpDay);
     else
        strcpy(szDay, szTmpDay);


     sprintf(szTmpDate, "%s%s%s", szYear, szMonth, szDay);

     return (szTmpDate);
     }


/*-----------------------------------------------------------------------------
  DateToday: Returns a string with todays date.
-----------------------------------------------------------------------------*/
char *DateToday(int iFlag)
     {
     static char        szBuffer[16];
     time_t             ltime;
     struct tm          *curtime;

     time(&ltime);
     curtime = localtime(&ltime);

     switch (iFlag)
            {
            case 0:
                 sprintf(szBuffer, "%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);
                 break;

            case 1:
                 sprintf(szBuffer, "(%.2d%.2d%.2d/%.2d:%.2d)", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday, curtime->tm_hour, curtime->tm_min);
                 break;

            case 2:
                 sprintf(szBuffer, "19%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);
                 break;

            case 3:
                 sprintf(szBuffer, "(19%.2d%.2d%.2d/%.2d:%.2d)", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday, curtime->tm_hour, curtime->tm_min);
                 break;
                 }

     return(szBuffer);
     }


/*-----------------------------------------------------------------------------
  DayofWeek: Gets day of week from date string.
-----------------------------------------------------------------------------*/
int DayOfWeek(char *szDate)
     {

     return (0);
     }


/*-----------------------------------------------------------------------------
   SetExeDirCurrent: Sets the .EXE directory as current directory.
-----------------------------------------------------------------------------*/
void SetExeDirCurrent (void)
     {
     APIRET        rc;
     PTIB          pTib;
     PPIB          pPib;
     static char        szBuffer[256];

     short              sCounter;

     DosGetInfoBlocks(&pTib, &pPib);
     rc = DosQueryModuleName(pPib->pib_hmte, sizeof(szBuffer), szBuffer);

     if (szBuffer[1] == ':')
        DosSetDefaultDisk((ULONG)(toupper(szBuffer[0]) - 'A' + 1));

     sCounter = strlen(szBuffer);

     while ((szBuffer[sCounter] != '\\') && (sCounter > 0))
           sCounter--;

     if (sCounter > 0)
        {
        if (szBuffer[1] == ':' && sCounter == 2)
           {
           szBuffer[sCounter + 1] = '\0';
           DosSetCurrentDir((PSZ) szBuffer);
           }
        else
           {
           szBuffer[sCounter] = '\0';
           DosSetCurrentDir((PSZ) szBuffer);
           }
        }
     }


/*-----------------------------------------------------------------------------
   GetExeDir: Returns the .EXE directory directory.
-----------------------------------------------------------------------------*/
char* GetExeDir (void)
     {
     static char        szBuffer[CCHMAXPATH];
     APIRET             rc;
     TIB                *pTib;
     PIB                *pPib;
     int                iCounter;


     DosGetInfoBlocks(&pTib, &pPib);
     DosQueryModuleName(pPib->pib_hmte, sizeof(szBuffer), szBuffer);

     iCounter = strlen(szBuffer);

     while ((szBuffer[iCounter] != '\\') && (iCounter > 0))
           iCounter--;

     if (iCounter > 0)
        {
        szBuffer[iCounter + 1] = '\0';
        return (szBuffer);
        }
     else
        return (NULL);
     }


/*------------------------------------------------------------------------------
   GetDLLFullName: Returns full pathname/size for specified .DLL file
-----------------------------------------------------------------------------*/
int GetDLLFullName(char *szDLLName, char *szFullPath)
     {
     HMODULE            hModule;
     HMODULE            hModFile;
     FILE               *handle;
     char               szFileFound[_MAX_PATH];
     int                iFileLength;
     APIRET             rc;
     HDIR               hdirFindHandle = HDIR_CREATE;
     FILEFINDBUF3       FindBuffer     = {0};
     ULONG              ulFindCount    = 1;


     DosLoadModule(NULL, 0, (PCSZ) szDLLName, &hModFile);

     rc = DosQueryModuleHandle((PCSZ)szDLLName, &hModule);
     if (rc != NO_ERROR)
       {
       DosFreeModule(hModFile);
       return(-1);
       }

    rc = DosQueryModuleName(hModule, _MAX_PATH, szFileFound);
     if (rc != NO_ERROR)
       {
       DosFreeModule(hModFile);
       return (-1);
       }
    else
       {
       strcpy(szFullPath, szFileFound);
       DosFreeModule(hModFile);
       if (DosFindFirst((PCSZ)szFullPath, &hdirFindHandle, FILE_NORMAL, &FindBuffer, sizeof(FILEFINDBUF3), &ulFindCount, FIL_STANDARD) ==NO_ERROR)
          {
          iFileLength = FindBuffer.cbFile;
          DosFindClose(hdirFindHandle);
          return(iFileLength);
          }
       else
          return(-1);
       }
  }


/*----------------------------------------------------------------------
  CheckForPreviousInstance: Looks for previous instance via shared mem.
-----------------------------------------------------------------------*/
int CheckForPreviousInstance (char *szAppName, int iFirstInstance)
     {
     int        *pvShrObject = NULL;
     char       szMemName[_MAX_PATH];


     sprintf(szMemName, "\\SHAREMEM\\%s", szAppName);

     /* If found, return hwnd of creator */
     if (DosGetNamedSharedMem((PPVOID) &pvShrObject, (PCSZ)szMemName, PAG_READ) == NO_ERROR)
        return (*pvShrObject);
     else
        {
        /* If not found, put our hwnd in sharemem */
        if (DosAllocSharedMem((PPVOID) &pvShrObject, (PCSZ)szMemName, 4096, PAG_COMMIT | PAG_WRITE ) == NO_ERROR)
           *pvShrObject = iFirstInstance;
        return(0);
        }
     }


/*----------------------------------------------------------------------
  GetInstanceData: Gets data from (previous) instance via shared mem.
-----------------------------------------------------------------------*/
int GetInstanceData(char *szAppName, char *szAppData)
     {
     int        *pvShrObject = NULL;
     char       szMemName[_MAX_PATH];


     sprintf(szMemName, "\\SHAREMEM\\%s", szAppName);

     if (DosGetNamedSharedMem((PPVOID) &pvShrObject, (PCSZ)szMemName, PAG_READ) == NO_ERROR)
        {
        memcpy(szAppData, pvShrObject + 8, *(pvShrObject + 4));
        return (*pvShrObject);
        }
     else
        return (0);
     }


/*----------------------------------------------------------------------
  SetInstanceData: Sets data for (previous) instance via shared mem.
-----------------------------------------------------------------------*/
int SetInstanceData(char *szAppName, int iAppDataLen, char *szAppData)
     {
     int        *pvShrObject = NULL;
     char       szMemName[_MAX_PATH];


     sprintf(szMemName, "\\SHAREMEM\\%s", szAppName);

     if (DosGetNamedSharedMem((PPVOID) &pvShrObject, (PCSZ)szMemName, PAG_READ | PAG_WRITE) == NO_ERROR)
        {
        *(pvShrObject + 4) = iAppDataLen;
        memcpy(pvShrObject + 8, szAppData, *(pvShrObject + 4));
        return (*pvShrObject);
        }
     else
        return (0);
     }


/*------------------------------------------------------------------------------
   FileGetPath: Get path from full filename.
-----------------------------------------------------------------------------*/
char *FileGetPath (char* szFullFile)
     {
     static char        szReturn[_MAX_DRIVE + _MAX_DIR];
     char               szDrive[_MAX_DRIVE];
     char               szDir[_MAX_DIR];
     char               szName[_MAX_FNAME];
     char               szExt[_MAX_EXT];


     _splitpath(szFullFile, szDrive, szDir, szName, szExt);

     sprintf(szReturn, "%s%s", szDrive, szDir);

     return (szReturn);
     }


/*------------------------------------------------------------------------------
   FileGetFile: Get file w/o ext from full filename.
-----------------------------------------------------------------------------*/
char *FileGetFile (char* szFullFile)
     {
     static char        szReturn[_MAX_FNAME];
     char               szDrive[_MAX_DRIVE];
     char               szDir[_MAX_DIR];
     char               szName[_MAX_FNAME];
     char               szExt[_MAX_EXT];


     _splitpath(szFullFile, szDrive, szDir, szName, szExt);

     sprintf(szReturn, "%s", szName);

     return (szReturn);
     }


/*------------------------------------------------------------------------------
   FileGetExt: Get ext from full filename.
-----------------------------------------------------------------------------*/
char *FileGetExt (char* szFullFile)
     {
     static char        szReturn[_MAX_EXT];
     char               szDrive[_MAX_DRIVE];
     char               szDir[_MAX_DIR];
     char               szName[_MAX_FNAME];
     char               szExt[_MAX_EXT];


     _splitpath(szFullFile, szDrive, szDir, szName, szExt);

     sprintf(szReturn, "%s", szExt);

     return (szReturn);
     }


/*------------------------------------------------------------------------------
   FileGetFileExt: Get filename + ext from full filename.
-----------------------------------------------------------------------------*/
char *FileGetFileExt (char* szFullFile)
     {
     static char        szReturn[_MAX_EXT];
     char               szDrive[_MAX_DRIVE];
     char               szDir[_MAX_DIR];
     char               szName[_MAX_FNAME];
     char               szExt[_MAX_EXT];


     _splitpath(szFullFile, szDrive, szDir, szName, szExt);

     sprintf(szReturn, "%s%s", szName, szExt);

     return (szReturn);
     }


/*------------------------------------------------------------------------------
   FileSetDir: Set full filename directory/drive as current.
-----------------------------------------------------------------------------*/
void FileSetDir (char* szFullFile)
     {
     char       szDrive[_MAX_DRIVE];
     char       szDir[_MAX_DIR];
     char       szName[_MAX_FNAME];
     char       szExt[_MAX_EXT];


     _splitpath(szFullFile, szDrive, szDir, szName, szExt);

     _chdrive(szDrive[0] - 64);

     szDir[strlen(szDir) - 1] = '\0';
     chdir(szDir);
     }


/*------------------------------------------------------------------------------
   FileCountFiles: Returns amount of files in path.
-----------------------------------------------------------------------------*/
int FileCountFiles (char* szPathWild, int iAttributes)
     {
     return (FileCount(FALSE, szPathWild, iAttributes));
     }


/*------------------------------------------------------------------------------
   FileCountDirectories: Returns amount of directories in path.
-----------------------------------------------------------------------------*/
int FileCountDirectories (char* szPathWild)
     {
     return (FileCount(TRUE, szPathWild, FILE_COUNT_NORMAL));
     }


/*------------------------------------------------------------------------------
   FileCount: Returns amount of dirs/files in path.
-----------------------------------------------------------------------------*/
int FileCount (BOOL bFlag, char* szPathWild, int iAttributes)
     {
     HDIR          hdirFindHandle = HDIR_CREATE;
     FILEFINDBUF3  *pulResultBuf;
     ULONG         ulResultBufLen = sizeof(FILEFINDBUF3) * FILE_COUNT_MAX_FILES;
     ULONG         ulFindCount;
     ULONG         ulFiles = 0;
     APIRET        rc;


     pulResultBuf = (PFILEFINDBUF3) calloc(sizeof(FILEFINDBUF3), FILE_COUNT_MAX_FILES);

     ulFindCount = FILE_COUNT_MAX_FILES;

     if (bFlag == TRUE)
        rc = DosFindFirst( szPathWild, &hdirFindHandle, MUST_HAVE_DIRECTORY, pulResultBuf, ulResultBufLen, &ulFindCount, FIL_STANDARD);
     else
        rc = DosFindFirst( szPathWild, &hdirFindHandle, iAttributes, pulResultBuf, ulResultBufLen, &ulFindCount, FIL_STANDARD);

     switch (rc)
            {
            case NO_ERROR:
                 ulFiles = ulFindCount;
                 break;

            case ERROR_NO_MORE_FILES:
                 DosFindClose(hdirFindHandle);
                 free(pulResultBuf);
                 return(0);
                 break;

            default:
                 DosFindClose(hdirFindHandle);
                 free(pulResultBuf);
                 return (-1);
            }


     while (rc != ERROR_NO_MORE_FILES)
           {
           ulFindCount = FILE_COUNT_MAX_FILES;
           rc = DosFindNext(hdirFindHandle, pulResultBuf, ulResultBufLen, &ulFindCount);

           if (rc != NO_ERROR && rc != ERROR_NO_MORE_FILES)
              {
              DosFindClose(hdirFindHandle);
              free(pulResultBuf);
              return (-1);
              }
           else
              ulFiles += ulFindCount;
           }

     DosFindClose(hdirFindHandle);
     free(pulResultBuf);
     return (ulFiles);
     }


/*------------------------------------------------------------------------------
   FileGetFiles: Returns files information.
-----------------------------------------------------------------------------*/
int FileGetFiles (char* szPathWild, PFILELIST pfl, int iAttr, int iSort)
     {
     return (FileGet(FALSE, szPathWild, pfl, iAttr, iSort));
     }


/*------------------------------------------------------------------------------
   FileGetDirectories: Returns directories information.
-----------------------------------------------------------------------------*/
int FileGetDirectories (char* szPathWild, PFILELIST pfl, int iSort)
     {
     return (FileGet(TRUE, szPathWild, pfl, FILE_COUNT_NORMAL, iSort));
     }


/*------------------------------------------------------------------------------
   FileGet: Returns dirs/files in path.
-----------------------------------------------------------------------------*/
int FileGet (BOOL bFlag, char* szPathWild, PFILELIST pfl, int iAttributes, int iSortOrder)
     {
     int           iFiles, iCounter;
     HDIR          hdirFindHandle = HDIR_CREATE;
     char          *pcRes;
     FILEFINDBUF3  *pulResultBuf, *pulRB;
     ULONG         ulResultBufLen;
     ULONG         ulFindCount;
     ULONG         ulFiles = 0;
     APIRET        rc;



     pfl->iFiles = 0;
     pfl->iSizeTotal = 0;
     pfl->iSizeAlloc = 0;
     pfl->pfl = NULL;

     iFiles = FileCount(bFlag, szPathWild, iAttributes);
     if (iFiles == -1)
        return (-1);

     pulRB = pulResultBuf = (PFILEFINDBUF3) calloc(sizeof(FILEFINDBUF3), iFiles);
     ulResultBufLen = sizeof(FILEFINDBUF3) * iFiles;
     ulFindCount = iFiles;

     if (bFlag == TRUE)
        rc = DosFindFirst((PCSZ)szPathWild, &hdirFindHandle, MUST_HAVE_DIRECTORY, pulResultBuf, ulResultBufLen, &ulFindCount, FIL_STANDARD);
     else
        rc = DosFindFirst((PCSZ)szPathWild, &hdirFindHandle, iAttributes, pulResultBuf, ulResultBufLen, &ulFindCount, FIL_STANDARD);

     if (rc != NO_ERROR)
        {
        DosFindClose(hdirFindHandle);
        free(pulResultBuf);
        return (-1);
        }

     if ( bFlag == TRUE && strcmp(pulResultBuf[0].achName, ".") == 0 )
        {
        pcRes = (char*) pulResultBuf;
        pcRes += pulResultBuf->oNextEntryOffset;
        pulResultBuf = (PFILEFINDBUF3) pcRes;
        iFiles--;
        }

     pfl->pfl = (PFILELISTENTRY) calloc(sizeof(FILELISTENTRY), iFiles);

     for (iCounter = 0; iCounter < iFiles; iCounter++)
         {
         pfl->iSizeTotal += pulResultBuf->cbFile;
         pfl->iSizeAlloc += pulResultBuf->cbFileAlloc;

         strcpy(pfl->pfl[iCounter].szName, pulResultBuf->achName);
         pfl->pfl[iCounter].iFileSize = pulResultBuf->cbFile;
         pfl->pfl[iCounter].iAttrib = pulResultBuf->attrFile;
         pfl->pfl[iCounter].sYear = 1980 + pulResultBuf->fdateLastWrite.year;
         pfl->pfl[iCounter].cMonth = pulResultBuf->fdateLastWrite.month;
         pfl->pfl[iCounter].cDay = pulResultBuf->fdateLastWrite.day;
         pfl->pfl[iCounter].cHour = pulResultBuf->ftimeLastWrite.hours;
         pfl->pfl[iCounter].cMinute = pulResultBuf->ftimeLastWrite.minutes;
         pfl->pfl[iCounter].cSecond = pulResultBuf->ftimeLastWrite.twosecs * 2;

         pcRes = (char*) pulResultBuf;
         pcRes += pulResultBuf->oNextEntryOffset;
         pulResultBuf = (PFILEFINDBUF3) pcRes;
         }

     pfl->iFiles = iFiles;

     DosFindClose(hdirFindHandle);
     free(pulRB);
     return(iFiles);
     }


/*------------------------------------------------------------------------------------
  SearchId: Searched memofile for specified memo id.
------------------------------------------------------------------------------------*/
int SearchId(FILE *stream, unsigned long ulMemoId)
     {
     MEMOHEADER         mh;
     MEMOITEM           mi;
     int                iCounter;
     unsigned long      ulCurrentPos;


     ReadHeader(stream, &mh);

     for (iCounter = 0; iCounter < mh.ulMemoId; iCounter++)
         {
         ulCurrentPos = ftell(stream);
         fread(&mi, sizeof(MEMOITEM), 1, stream);
         if (mi.ulMemoId == ulMemoId)
            {
            return (ulCurrentPos);
            }
         fseek(stream, mi.ulNextMemoPos, SEEK_SET);
         }
     return (FALSE);
     }


/*------------------------------------------------------------------------------------
  WriteHeader: Updates memofile header.
------------------------------------------------------------------------------------*/
void WriteHeader(FILE *stream, PMEMOHEADER pmh, int iFlag)
     {
     char               szBuffer[7];
     time_t             ltime;
     struct tm          *curtime;
     int                iCounter;


     time(&ltime);
     curtime = localtime(&ltime);
     sprintf(szBuffer, "%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);

     if (iFlag == FALSE)
        {
        fseek(stream, 0, SEEK_SET);
        for (iCounter = 0; iCounter < 6; iCounter++)
            pmh->szDate[iCounter] = szBuffer[iCounter];
        fwrite(pmh, sizeof(MEMOHEADER), 1, stream);
        }
     else
        {
        fseek(stream, 0, SEEK_SET);
        pmh->szMemoFile[0] = 'S';
        pmh->szMemoFile[1] = 'T';
        pmh->szMemoFile[2] = 'M';
        pmh->szMemoFile[3] = 2;
        pmh->ulMemoId = 0;
        pmh->ulMemoActive = 0;
        for (iCounter = 0; iCounter < 6; iCounter++)
            pmh->szDate[iCounter] = szBuffer[iCounter];
        fwrite(pmh, sizeof(MEMOHEADER), 1, stream);
        }
     }


/*------------------------------------------------------------------------------------
  ReadHeader: Gets memofile header.
------------------------------------------------------------------------------------*/
void ReadHeader(FILE *stream, PMEMOHEADER pmh)
     {
     fseek(stream, 0, SEEK_SET);
     fread(pmh, sizeof(MEMOHEADER), 1, stream);
     }


/*------------------------------------------------------------------------------------
  MemoInfo: Retrieve memo info from header.
------------------------------------------------------------------------------------*/
BOOL MemoInfo(char *szMemoFile, PMEMOHEADER pmh)
     {
     FILE       *stream;
     char       *szMemo;

     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        ReadHeader(stream, pmh);
        fclose(stream);
        return (TRUE);
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoGet: Gets memo from memofile.
------------------------------------------------------------------------------------*/
char *MemoGet(char *szMemoFile, unsigned long ulMemoPos)
     {
     FILE               *stream;
     char               *szMemo;
     MEMOITEM           mi;

     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        fseek(stream, ulMemoPos, SEEK_SET);
        fread(&mi, sizeof(MEMOITEM), 1, stream);
        szMemo = (char *)malloc(mi.ulMemoLen);
        memset(szMemo, 0, mi.ulMemoLen);
        fread(szMemo, sizeof(char), mi.ulMemoLen, stream);
        fclose(stream);
        return (szMemo);
        }
     return((char*)NULL);
     }


/*------------------------------------------------------------------------------------
  MemoGetId: Gets memo from memofile by Id.
------------------------------------------------------------------------------------*/
char *MemoGetId(char *szMemoFile, unsigned long ulMemoId)
     {
     FILE               *stream;
     char               *szMemo;
     MEMOITEM           mi;
     unsigned long      ulMemoPos;

     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        if ( (ulMemoPos = SearchId(stream, ulMemoId)) != 0)
           {
           fseek(stream, ulMemoPos, SEEK_SET);
           fread(&mi, sizeof(MEMOITEM), 1, stream);
           szMemo = (char *)malloc(mi.ulMemoLen);
           memset(szMemo, 0, mi.ulMemoLen);
           fread(szMemo, sizeof(char), mi.ulMemoLen, stream);
           fclose(stream);
           return (szMemo);
           }
        else
           {
           fclose(stream);
           return((char*)NULL);
           }
        }
     return((char*)NULL);
     }


/*------------------------------------------------------------------------------------
  MemoQueryId: Gets id using position.
------------------------------------------------------------------------------------*/
int MemoQueryId(char *szMemoFile, unsigned long ulMemoPos)
     {
     FILE               *stream;
     MEMOITEM           mi;


     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        fseek(stream, ulMemoPos, SEEK_SET);
        fread(&mi, sizeof(MEMOITEM), 1, stream);
        fclose(stream);
        return (mi.ulMemoId);
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoQueryPos: Gets position using Id.
------------------------------------------------------------------------------------*/
int MemoQueryPos(char *szMemoFile, unsigned long ulMemoId)
     {
     FILE               *stream;
     MEMOITEM           mi;
     unsigned long      ulMemoPos;


     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        if ( (ulMemoPos = SearchId(stream, ulMemoId)) != 0)
           {
           fseek(stream, ulMemoPos, SEEK_SET);
           fread(&mi, sizeof(MEMOITEM), 1, stream);
           fclose(stream);
           return (mi.ulMemoId);
           }
        else
           {
           fclose(stream);
           return(FALSE);
           }
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoLen: Gets memolength from memofile.
------------------------------------------------------------------------------------*/
int MemoLen(char* szMemoFile, unsigned long ulMemoPos)
     {
     FILE               *stream;
     MEMOITEM           mi;


     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        fseek(stream, ulMemoPos, SEEK_SET);
        fread(&mi, sizeof(MEMOITEM), 1, stream);
        fclose(stream);
        return (mi.ulMemoLen);
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoLenId: Gets memolength from memofile using Id.
------------------------------------------------------------------------------------*/
int MemoLenId(char* szMemoFile, unsigned long ulMemoId)
     {
     FILE               *stream;
     MEMOITEM           mi;
     unsigned long      ulMemoPos;


     if ((stream = fopen((char*)szMemoFile, "rb")) != 0)
        {
        if ( (ulMemoPos = SearchId(stream, ulMemoId)) != 0)
           {
           fseek(stream, ulMemoPos, SEEK_SET);
           fread(&mi, sizeof(MEMOITEM), 1, stream);
           fclose(stream);
           return (mi.ulMemoLen);
           }
        else
           {
           fclose(stream);
           return(FALSE);
           }
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoPut: Saves memo to memofile.
------------------------------------------------------------------------------------*/
int MemoPut(char *szMemoFile, unsigned long ulMemoSize, char *szMemo)
     {
     FILE               *stream;
     MEMOHEADER         mh;
     unsigned long      ulMemoPos;
     MEMOITEM           mi;
     char               szBuffer[7];
     time_t             ltime;
     struct tm          *curtime;
     int                iCounter;


     /* F�rs�k �ppna filen f�r l�sning */
     if ((stream = fopen((char*)szMemoFile, "r+b")) == 0)
        {
        /* Om inte s� skapar vi den */
        if ((stream = fopen((char*)szMemoFile,"w+b")) == 0)
           return (FALSE);
        else
           WriteHeader(stream, &mh, TRUE);
        }
     /* H�mta headern */
     ReadHeader(stream, &mh);

     /* G� till slutet av filen och spara position */
     fseek(stream, 0, SEEK_END);
     ulMemoPos = ftell(stream);

     /* Skriv memo't i filen */
     mi.ulMemoId = mh.ulMemoId + 1;
     mi.ulMemoLen = ulMemoSize;
     mi.sMemoActive = 1;
     time(&ltime);
     curtime = localtime(&ltime);
     sprintf(szBuffer, "%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);
     for (iCounter = 0; iCounter < 6; iCounter++)
         mi.szDate[iCounter] = szBuffer[iCounter];
     mi.ulNextMemoPos = ulMemoPos + sizeof(MEMOITEM) + mi.ulMemoLen - 1;

     fwrite(&mi, sizeof(MEMOITEM), 1, stream);
     fwrite((char*)szMemo, sizeof(char), mi.ulMemoLen, stream);

     mh.ulMemoId++;
     mh.ulMemoActive++;
     WriteHeader(stream, &mh, FALSE);

     /* St�ng filen, �terv�nd */
     fclose(stream);
     return (ulMemoPos);
     }


/*------------------------------------------------------------------------------------
  MemoPutId: Saves memo to memofile returning Id.
------------------------------------------------------------------------------------*/
int MemoPutId(char *szMemoFile, unsigned long ulMemoSize, char *szMemo)
     {
     FILE               *stream;
     MEMOHEADER         mh;
     unsigned long      ulMemoPos;
     MEMOITEM           mi;
     char               szBuffer[7];
     time_t             ltime;
     struct tm          *curtime;
     int                iCounter;


     /* F�rs�k �ppna filen f�r l�sning */
     if ((stream = fopen((char*)szMemoFile, "r+b")) == 0)
        {
        /* Om inte s� skapar vi den */
        if ((stream = fopen((char*)szMemoFile,"w+b")) == 0)
           return (FALSE);
        else
           WriteHeader(stream, &mh, TRUE);
        }
     /* H�mta headern */
     ReadHeader(stream, &mh);

     /* G� till slutet av filen och spara position */
     fseek(stream, 0, SEEK_END);
     ulMemoPos = ftell(stream);

     /* Skriv memo't i filen */
     mi.ulMemoId = mh.ulMemoId + 1;
     mi.ulMemoLen = ulMemoSize;
     mi.sMemoActive = 1;
     time(&ltime);
     curtime = localtime(&ltime);
     sprintf(szBuffer, "%.2d%.2d%.2d", curtime->tm_year, curtime->tm_mon+1, curtime->tm_mday);
     for (iCounter = 0; iCounter < 6; iCounter++)
         mi.szDate[iCounter] = szBuffer[iCounter];
     mi.ulNextMemoPos = ulMemoPos + sizeof(MEMOITEM) + mi.ulMemoLen - 1;

     fwrite(&mi, sizeof(MEMOITEM), 1, stream);
     fwrite((char*)szMemo, sizeof(char), mi.ulMemoLen, stream);

     mh.ulMemoId++;
     mh.ulMemoActive++;
     WriteHeader(stream, &mh, FALSE);

     /* St�ng filen, �terv�nd */
     fclose(stream);
     return (mi.ulMemoId);
     }


/*------------------------------------------------------------------------------------
  MemoDelete: Flags memo as deleted.
------------------------------------------------------------------------------------*/
BOOL MemoDelete(char *szMemoFile, unsigned long ulMemoPos)
     {
     FILE               *stream;
     MEMOITEM           mi;
     MEMOHEADER         mh;


     if ((stream = fopen((char*)szMemoFile, "r+b")) != 0)
        {
        /* Uppdatera sMemoActive */
        fseek(stream, ulMemoPos, SEEK_SET);
        fread(&mi, sizeof(MEMOITEM), 1, stream);
        mi.sMemoActive = 0;
        fseek(stream, ulMemoPos, SEEK_SET);
        fwrite(&mi, sizeof(MEMOITEM), 1, stream);

        /* Minska antalet i headern */
        ReadHeader(stream, &mh);
        mh.ulMemoActive--;
        WriteHeader(stream, &mh, FALSE);

        fclose(stream);
        return (TRUE);
        }
     return(FALSE);
     }


/*------------------------------------------------------------------------------------
  MemoDeleteId: Flags memo as deleted using Id.
------------------------------------------------------------------------------------*/
BOOL MemoDeleteId(char *szMemoFile, unsigned long ulMemoId)
     {
     FILE               *stream;
     MEMOITEM           mi;
     MEMOHEADER         mh;
     unsigned long      ulMemoPos;


     if ((stream = fopen((char*)szMemoFile, "r+b")) != 0)
        {
        if ( (ulMemoPos = SearchId(stream, ulMemoId)) != 0)
           {

           /* Uppdatera sMemoActive */
           fseek(stream, ulMemoPos, SEEK_SET);
           fread(&mi, sizeof(MEMOITEM), 1, stream);
           mi.sMemoActive = 0;
           fseek(stream, ulMemoPos, SEEK_SET);
           fwrite(&mi, sizeof(MEMOITEM), 1, stream);

           /* Minska antalet i headern */
           ReadHeader(stream, &mh);
           mh.ulMemoActive--;
           WriteHeader(stream, &mh, FALSE);

           fclose(stream);
           return (TRUE);
           }
        else
           {
           fclose(stream);
           return(FALSE);
           }
        }
     return(FALSE);
     }

//------------------------------------------------------------------------------
