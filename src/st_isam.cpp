/****************************************************************************
   BTISAM.C
  ==========================================================================

  ==========================================================================
   930926 Converted to CSET/2
  ==========================================================================
   940205 Fixed bug with noninitialized string array.
  ==========================================================================
   940206 Fixed BtLof bug (use a different metod), why BtLof does not
          work I still don't know...
  ==========================================================================
   950804 Fixed more BtLof bug (returned only short).
  ==========================================================================
****************************************************************************/
#define INCL_NOBASEAPI
#define INCL_NOPMAPI

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include "st_isam.h"

/* Declare DLL entrypoint */
unsigned short _Far16 _Pascal btrcall(short operation, char * posblock, char * databuf,
                                      short * datalen, char * keybuf, short keylen, char ckeynum);

unsigned short _Far16 _Pascal BTRVINIT(char * szInitParam);

unsigned short _Far16 _Pascal BTRVSTOP(void);



/*************************************************************************************
  BtInit: Init or reinit the Btrieve DLL.
*************************************************************************************/
int BtInit(int iFlag, char * szInitString)
     {
     unsigned short     rc;

     if (iFlag == TRUE)
        rc = BTRINIT(szInitString);
     else
        rc = BTREXIT();

     return (rc);
     }


/*************************************************************************************
  BtReset: Reset connection with server (or reset all use of Btrieve DLL).
*************************************************************************************/
int BtReset(PBTFILE bt, long lConnectId)
     {
     unsigned short     rc;

     if (lConnectId != 0)
        rc = BTRV(BT_OP_RESET, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *)&lConnectId, -1);
     else
        rc = BTRV(BT_OP_RESET, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtCreate
*************************************************************************************/
int BtCreate(char * szFileName, PBTCFILESPEC pbtcf, PBTCKEYSPEC pbtck, char * szAltCol, PBTFILE bt)
     {
     unsigned short     rc;
     char               szGigant[4096];
     int                iStrLen, iCounter, iInner, iStart;
     char               *pcf, *pck, *pGigant;

     pGigant = (char*)&szGigant;
     pcf = (char*) pbtcf;
     pck = (char*) pbtck;

     /* Must use 16 since otherwise packed structures? */

     for (iCounter = 0; iCounter < 16; iCounter++)
         *pGigant++ = *pcf++;

     for (iCounter = 0; iCounter < pbtcf->sIndexCount; iCounter++)
         {
         iStart = iCounter * sizeof(BTCKEYSPEC);
         for (iInner = iStart ; iInner < (iStart + 16); iInner++)
             *pGigant++ = *pck++;
         pck += sizeof(BTCKEYSPEC) - 16;
         }

     iStrLen = 16 + (16 * pbtcf->sIndexCount);
     rc = BTRV(BT_OP_CREATE, bt->bPosBlock, szGigant, &iStrLen, szFileName, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }

/*************************************************************************************
  BtOpen: Open Btrieve file.
*************************************************************************************/
int BtOpen(char * szFileName, char * szOwner, int iMode, PBTFILE bt)
     {
     unsigned short     rc;
     int                iStrLen;

     if (szOwner == NULL)
        iStrLen = 0;
     else
        iStrLen = strlen(szOwner);

     rc = BTRV(BT_OP_OPEN, bt->bPosBlock, szOwner, &iStrLen, szFileName, iMode);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtClose: Close Btrieve File.
*************************************************************************************/
int BtClose(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_CLOSE, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetFirst: Get first record in file (current key).
*************************************************************************************/
int BtGetFirst(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETFIRST + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetPrevious: Get previous record in file (current key).
*************************************************************************************/
int BtGetPrevious(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETPREVIOUS + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetNext: Get next record in file (current key).
*************************************************************************************/
int BtGetNext(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETNEXT + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetLast: Get last record in file (current key).
*************************************************************************************/
int BtGetLast(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETLAST + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetEqual: Get record matching key.
*************************************************************************************/
int BtGetEqual(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     sprintf(szGlobalKey, "%s", szKey);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETEQUAL + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetGE: Get record greater than or equal key.
*************************************************************************************/
int BtGetGE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     sprintf(szGlobalKey, "%s", szKey);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETGREATEROREQUAL + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetGreater: Get record greater than key.
*************************************************************************************/
int BtGetGreater(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     sprintf(szGlobalKey, "%s", szKey);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETGREATER + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetLE: Get record less than or equal key.
*************************************************************************************/
int BtGetLE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     sprintf(szGlobalKey, "%s", szKey);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETLESSTHANOREQUAL + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtGetLess: Get record less than key.
*************************************************************************************/
int BtGetLess(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock)
     {
     unsigned short     rc;
     int                iLockFlag;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     sprintf(szGlobalKey, "%s", szKey);

     if (iLock == TRUE)
        iLockFlag = 200;
     else
        iLockFlag = 0;

     rc = BTRV(BT_OP_GETLESSTHAN + iLockFlag, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtInsert: Insert record into database.
*************************************************************************************/
int BtInsert(PBTFILE bt, char * pbRecord, int iRecordLen)
     {
     unsigned short     rc;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     rc = BTRV(BT_OP_INSERT, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtDelete: Delete record from database.
*************************************************************************************/
int BtDelete(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_DELETE, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtUpdate: Update current record.
*************************************************************************************/
int BtUpdate(PBTFILE bt, char * pbRecord, int iRecordLen)
     {
     unsigned short     rc;
     char               szGlobalKey[256];

     memset(szGlobalKey, 0, 256);

     rc = BTRV(BT_OP_UPDATE, bt->bPosBlock, pbRecord, &iRecordLen, szGlobalKey, bt->iCurKey);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtUnlock: Unlock locked record (or use BtDelete or BtUpdate).
*************************************************************************************/
int BtUnlock(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_UNLOCK, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtLof: Return length of file.
*************************************************************************************/
int BtLof(PBTFILE bt)
     {
     unsigned short     rc;
     char               szGlobalKey[65];
     char               szDataBuffer[512];
     int                iStrLen;
     unsigned long      *pulRecords;


     /* Skicka 512 bytes s† blir inte buffern f”r liten */
     memset(szGlobalKey, 0, 65);
     memset(szDataBuffer, 0, 512);

     iStrLen = 512;

     rc = BTRV(BT_OP_STAT, bt->bPosBlock, szDataBuffer, &iStrLen, szGlobalKey, 0);

     bt->iStatus = 0;

     if (rc == BT_ERR_NONE)
        {
        /* Antal poster finns p† position 7 ([6]) */
        pulRecords = (unsigned long *)&szDataBuffer[6];
        return (*pulRecords);
        }
     else
        {
        bt->iStatus = rc;
        return (-1);
        }
     }


/*************************************************************************************
  BtBof: Returns TRUE if passed beginning of file.                            921206
*************************************************************************************/
int BtBof(PBTFILE bt)
     {
     if (bt->iStatus == BT_ERR_EOF)
        return (TRUE);
     else
        return (FALSE);
     }


/*************************************************************************************
  BtEof: Returns TRUE if passed end of file.
*************************************************************************************/
int BtEof(PBTFILE bt)
     {
     return (BtBof(bt));
     }


/*************************************************************************************
  BtSetOwner: Set file owner (and possibly encrypt file).
*************************************************************************************/
int BtSetOwner(PBTFILE bt, char * szOwner, int iRestrict)
     {
     unsigned short     rc;
     int                iStrLen;

     if (szOwner == NULL)
        iStrLen = 0;
     else
        iStrLen = strlen(szOwner);

     rc = BTRV(BT_OP_SETOWNER, bt->bPosBlock, szOwner, &iStrLen, szOwner, iRestrict);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtClearOwner: Remove owner/encryption of file.
*************************************************************************************/
int BtClearOwner(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_CLEAROWNER, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtTransBegin: Begin transaction.
*************************************************************************************/
int BtTransBegin(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_TRANSBEGIN, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtTransAbort: Abort transaction.
*************************************************************************************/
int BtTransAbort(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_TRANSABORT, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }


/*************************************************************************************
  BtTransEnd: End transaction.
*************************************************************************************/
int BtTransEnd(PBTFILE bt)
     {
     unsigned short     rc;

     rc = BTRV(BT_OP_TRANSEND, bt->bPosBlock, (char *) NULL, (int *) NULL, (char *) NULL, 0);

     bt->iStatus = 0;

     if (rc != 0)
        {
        bt->iStatus = rc;
        return (FALSE);
        }
     else
        return (TRUE);
     }

/*************************************************************************************
  BTRV: Thunk procedure for Btrieve 16bit's btrcall.
*************************************************************************************/
unsigned short BTRV(int operation, char *posblock, char *databuf, int *datalen, char *keybuf, int keynum)
     {
     static short	dlen;
     short 		rc;

     if (datalen == NULL)
        dlen = 0;
     else
        dlen = *datalen;

     /* Make call to the dynamic link entry point */
     rc = btrcall(operation, posblock, databuf, &dlen, keybuf, 255, (char) keynum);

     if (datalen != NULL)
        *datalen = dlen;

     return (rc);
     }

/*************************************************************************************
  BTRINIT: Thunk procedure for Btrieve 16bit's BTRVINIT.
*************************************************************************************/
unsigned short BTRINIT(char *szInitParam)
     {
     short 		rc;

     /* Make call to the dynamic link entry point */
     rc = BTRVINIT(szInitParam);
     return (rc);
     }

/*************************************************************************************
  BTRINIT: Thunk procedure for Btrieve 16bit's BTRVINIT.
*************************************************************************************/
unsigned short BTREXIT(void)
     {
     short 		rc;

     /* Make call to the dynamic link entry point */
     rc = BTRVSTOP();
     return (rc);
     }

//------------------------------------------------------------------------------
