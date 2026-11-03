/**************************************************************************

   Modul:      SERIAL.CPP, Function library for console applications
                           with support for Warp            .

   Version:    0.062

   Tid:        941206, 960203, 17h

  ---------------------------------------------------------------------
   941206:  Skapade Vio calls (MA,3h).
  ---------------------------------------------------------------------
   950316:  Skapade "stal" serial calls (MA,2h).
  ---------------------------------------------------------------------
   951008:  Lade i hop till en fil med .H fil (MA,1h).
  ---------------------------------------------------------------------
   951014:  Delade upp i OS/2 och Windows NT delar. B�rjade att skriva
            p� Windows NT koden (MA,1h).
  ---------------------------------------------------------------------
   951026:  Fixade till Windows NT koden men har �nnu inte testat ut
            att den fungerar (MA,3h).
  ---------------------------------------------------------------------
   951031:  Changed some int's in recieving characters in Windows NT code
            to char's. (MA,1h).
  ---------------------------------------------------------------------
   951116:  Removed (temporary?) read thread in NT version (MA,1h).
  ---------------------------------------------------------------------
   951213:  Fixed a bug that stopped the read timeout in the
            serial port communication (MA,1h).
  ---------------------------------------------------------------------
   951223:  Started adding database support for ODBC (WinNT) and
            added a generic key read function GetKeyPress (MA,1h).
  ---------------------------------------------------------------------
   960117:  Played around with flowcontrol for WinNT (MA,1h).
  ---------------------------------------------------------------------
   960203:  Removed Windows NT support (MA,1h).
  ---------------------------------------------------------------------
   960204:  Moved into STLIB2 (MA,1h).
  ---------------------------------------------------------------------
**************************************************************************/
#define INCL_NOPMAPI
#define INCL_DOS
#define INCL_SUB
#define INCL_DOSDEVIOCTL

#include <os2.h>


/* Generic includes
  ------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <conio.h>

#include "st_serial.h"


/*-----------------------------------------------------------------------
   SerialInit: Initialise communication.
-----------------------------------------------------------------------*/
int SerialInit(PSERIALSTRUCT pss)
    {
    char                szPortName[5];
    unsigned long    ulAction, parmlen;
    int                 rc;


    sprintf(szPortName, "COM%d", pss->usPort);

    rc = DosOpen((PSZ) szPortName, &pss->hSerial, &ulAction, 0, FILE_NORMAL, FILE_OPEN,
                 OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE, NULL);
    if (rc == 0)
       {
       SerialSetSpeed(pss, pss->usBpsRate);
       SerialSetTimeout(pss, pss->usReadTimeOut, pss->usWriteTimeOut);

       SerialSetBits(pss, pss->usDataBit, pss->usParity, pss->usStopBit);

       DosCreateEventSem(NULL, &pss->hEvent, 0, 0);
       DosCreateMutexSem(NULL, &pss->hMutex, 0, 0);

       pss->bExitTask = pss->iHead = pss->iTail = 0;

       pss->tidRecieveThread = _beginthread(SerialRecieveThread, NULL, 8192, (PVOID) pss);
       }
    return rc;
    }


/*-----------------------------------------------------------------------
   SerialExit:
-----------------------------------------------------------------------*/
int SerialExit(PSERIALSTRUCT pss)
    {
    APIRET      rc;

    pss->bExitTask = TRUE;
    DosWaitThread(&pss->tidRecieveThread, DCWW_WAIT);
    DosCloseEventSem(pss->hEvent);
    DosCloseMutexSem(pss->hMutex);
    rc = DosClose(pss->hSerial);
    return (rc);
    }


/*-----------------------------------------------------------------------
   SerialRecieveThread:
-----------------------------------------------------------------------*/
void SerialRecieveThread(PVOID pTmpSS)
     {
     char               buffer[20];
     unsigned long      act_read;
     int                i, c;
     PSERIALSTRUCT      pss;


     pss = (PSERIALSTRUCT)pTmpSS;
     do {
        DosRead(pss->hSerial, buffer, 20, &act_read);
        for (i = 0; i < act_read; ++i)
            {
            DosRequestMutexSem(pss->hMutex, SEM_INDEFINITE_WAIT);
            c = buffer[i];
            pss->acRecBuf[pss->iHead++] = buffer[i];
            pss->iHead %= 2000;
            if (pss->iHead == pss->iTail)
               {
               pss->iTail = (pss->iTail + 1) % 2000;
               }
            DosPostEventSem(pss->hEvent);
            DosReleaseMutexSem(pss->hMutex);
            }
        }
     while (!pss->bExitTask);
     }


/*-----------------------------------------------------------------------
   SerialSetSpeed: Sets serial port speed.
-----------------------------------------------------------------------*/
int SerialSetSpeed(PSERIALSTRUCT pss, unsigned short usBpsRate)
    {
    unsigned long       parmlen;
    APIRET              rc;

    parmlen = 2;
    rc = DosDevIOCtl(pss->hSerial, IOCTL_ASYNC, ASYNC_SETBAUDRATE, &usBpsRate,
                     sizeof(usBpsRate), &parmlen, NULL, 0, NULL);
    return (rc);
    }


/*-----------------------------------------------------------------------
   SerialSetTimeout: Set timeout value.
-----------------------------------------------------------------------*/
int SerialSetTimeout(PSERIALSTRUCT pss, unsigned short usReadTimeOut, unsigned short usWriteTimeOut)
    {
    unsigned long      length, len2;
    DCBINFO            dcb;
    APIRET             rc;

    length = sizeof(DCBINFO);
    if ((rc = DosDevIOCtl(pss->hSerial, IOCTL_ASYNC, ASYNC_GETDCBINFO, NULL, 0, NULL, &dcb, sizeof(dcb), &length)) == 0)
       {
       dcb.fbCtlHndShake = 9;
       dcb.fbFlowReplace = 64;
       dcb.fbTimeout = 4;
       dcb.usReadTimeout = usReadTimeOut;
       dcb.usWriteTimeout = usWriteTimeOut;
       length = sizeof(DCBINFO);
       rc=DosDevIOCtl(pss->hSerial, IOCTL_ASYNC, ASYNC_SETDCBINFO, &dcb, sizeof(dcb), &length, NULL, 0, NULL);
       }
    return (rc);
    }


/*-----------------------------------------------------------------------
   SerialSetBits: Sets databits/parity/stopbits
-----------------------------------------------------------------------*/
int SerialSetBits(PSERIALSTRUCT pss, USHORT usDataBit, USHORT usParity, USHORT usStopBit)
    {
    unsigned char       asParms[3];
    unsigned long       ulLength;
    APIRET              rc;

    asParms[0] = usDataBit;
    asParms[1] = usParity;
    asParms[2] = usStopBit;
    ulLength = 3;

    rc = DosDevIOCtl(pss->hSerial, IOCTL_ASYNC, ASYNC_SETLINECTRL, asParms, 3, &ulLength, NULL, 0, NULL);

    return (rc);
    }


/*-----------------------------------------------------------------------
   SerialGetC:
-----------------------------------------------------------------------*/
int SerialGetC(PSERIALSTRUCT pss, unsigned short usTimeout)
    {
    int              result;
    unsigned long    dummy;

    if (DosRequestMutexSem(pss->hMutex, usTimeout) == 0)
       {
       if (pss->iHead != pss->iTail)
          {
          result = pss->acRecBuf[pss->iTail++];
          pss->iTail %= 2000;
          DosReleaseMutexSem(pss->hMutex);
          }
       else
          {
          DosResetEventSem(pss->hEvent, &dummy);
          DosReleaseMutexSem(pss->hMutex);
          /* Hit the event sem when there's a character in the buffer
             nonzero means the wait timed out */
          if (DosWaitEventSem(pss->hEvent, usTimeout) == 0)
             {
             if (DosRequestMutexSem(pss->hMutex, usTimeout) == 0)
                {
                result = pss->acRecBuf[pss->iTail++];
                pss->iTail %= 2000;
                DosReleaseMutexSem(pss->hMutex);
                }
             else
                result = -1;
             }
          else
             result = -1;
          }
       }
    else
       result = -1;

    return result;
    }


/*-----------------------------------------------------------------------
   SerialPutC:
-----------------------------------------------------------------------*/
int SerialPutC(PSERIALSTRUCT pss, unsigned short usTimeout, char cSendChar)
    {
    int                 iResult;
    unsigned long       ulActualWrite;
    APIRET              rc;

    if (DosRequestMutexSem(pss->hMutex, usTimeout) == 0)
       {
       rc = DosWrite(pss->hSerial, (PVOID) &cSendChar, 1, &ulActualWrite);
       if (rc != 0 || ulActualWrite != 1)
          iResult = -1;
       else
          iResult = rc;
       DosReleaseMutexSem(pss->hMutex);
       }
    else
       iResult = -1;

    return (iResult);
    }


/*-----------------------------------------------------------------------
   SerialRecieveString:
-----------------------------------------------------------------------*/
int SerialRecieveString(PSERIALSTRUCT pss, char *szRcvString, int iMaxLen, unsigned short usTimeout)
    {
    int                 iRcvChars, iChar;
    BOOL                bDone;


    iRcvChars = 0;
    bDone = FALSE;

    do {
       iChar = SerialGetC(pss, usTimeout);
       if (iChar != -1)
          {
          szRcvString[iRcvChars] = iChar;
          iRcvChars++;
          if (iRcvChars == iMaxLen)
             bDone = TRUE;
          }
       else
          return (-1);
       }
    while (!bDone);

    return (0);
    }


/*-----------------------------------------------------------------------
   SerialSendString:
-----------------------------------------------------------------------*/
int SerialSendString(PSERIALSTRUCT pss, unsigned short usTimeout, char *szSendString)
    {
    int                 iResult;
    unsigned long       ulActualWrite;
    APIRET              rc;

    if (DosRequestMutexSem(pss->hMutex, usTimeout) == 0)
       {
       rc = DosWrite(pss->hSerial, (PVOID) szSendString, strlen(szSendString), &ulActualWrite);
       if (rc != 0 || ulActualWrite != strlen(szSendString))
          iResult = -1;
       else
          iResult = rc;
       DosReleaseMutexSem(pss->hMutex);
       }
    else
       iResult = -1;

    return (iResult);
    }


/*-----------------------------------------------------------------------
   SerialWaitString:
-----------------------------------------------------------------------*/
int SerialWaitString(PSERIALSTRUCT pss, char *szRcvString, unsigned short usTimeout)
    {
    unsigned long       act_read;
    int                 state, done, buff_count, len, result, i, c;
    char                buffer[100];

    buff_count = 0;
    done = 0;
    len = strlen(szRcvString);
    result = 0;

    do {
       c = SerialGetC(pss, usTimeout);
       if (c != -1)
          {
          buffer[buff_count] = c;
          if (buffer[buff_count] == szRcvString[buff_count])
             {
             buff_count++;
             if (buff_count == len)
                {
                done = 1;
                }
             }
          else
             {
             if (buff_count > 0)
                {
                do {
                   for (i = 0; i < buff_count; ++i)
                       buffer[i] = buffer[i+1];

                   buff_count--;
                   for (i = 0; i < buff_count; ++i)
                       {
                       if (buffer[i] != szRcvString[i])
                          break;
                       }
                   }
                while(i != buff_count);
                }
             }
          }
       else
          {
          result = 1;
          done = 1;
          }
       }
    while(!done);

    return result;
    }
