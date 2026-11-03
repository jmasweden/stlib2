//**************************************************************************
//
//   Modul:      VIO.CPP, Function library for console applications
//                        with support for Warp.
//
//   Version:    0.076
//
//   Tid:        941206, 980309, 18h
//
//  ---------------------------------------------------------------------
//   941206:  Skapade Vio calls (MA,3h).
//  ---------------------------------------------------------------------
//   960203:  Removed Windows NT support (MA,1h).
//  ---------------------------------------------------------------------
//   960204:  Inserted into STLIB2 (MA,1h).
//  ---------------------------------------------------------------------
//   980128:  Started move to stvio library (MA,2h).
//  ---------------------------------------------------------------------
//   980309:  Fix for numeric keyboard in VioInkey (MA,1h).
//  ---------------------------------------------------------------------
//**************************************************************************

// Includes files for OS/2
//-------------------------
#define INCL_VIO
#define INCL_KBD
#define INCL_DOSMISC

#include <os2.h>

// Generic includes
//------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <conio.h>

#include "st_vio.h"

// Private 
//-------------------------------------------------------------------------------
void vio_debugLog(char *szFormat, ...);

/*-----------------------------------------------------------------------------
   VioGetInfo: Gets info on colors and screensize.
-----------------------------------------------------------------------------*/
void VioGetInfo(int *iMaxRows, int *iMaxCols, int *iColors)
     {
     VIOMODEINFO        ModeData;


     ModeData.cb = sizeof(VIOMODEINFO);
     VioGetMode(&ModeData, VIO_HANDLE);

     if (iMaxRows != NULL)
        *iMaxRows = ModeData.row;

     if (iMaxCols != NULL)
        *iMaxCols = ModeData.col;

     if (iColors != NULL)
        *iColors = ModeData.color;
     }


/*-----------------------------------------------------------------------------
   VioGetInfo: Gets info on colors and screensize.
-----------------------------------------------------------------------------*/
BOOL VioSetScreenSize(int iMaxRows, int iMaxCols)
     {
     VIOMODEINFO        ModeData;


     ModeData.cb = sizeof(VIOMODEINFO);
     VioGetMode(&ModeData, VIO_HANDLE);

     if (iMaxRows != NULL)
        ModeData.row = iMaxRows;

     if (iMaxCols != NULL)
        ModeData.col = iMaxCols;

     if (VioSetMode(&ModeData, VIO_HANDLE) == 0)
        return (TRUE);
     else
        return (FALSE);
     }



/*-----------------------------------------------------------------------------
   VioCls: Clears screen.
-----------------------------------------------------------------------------*/
void VioCls(int iForeColor, int iBackColor)
    {
    int                 iCounter;
    VIOMODEINFO         ModeData;
    USHORT              usFill;


    ModeData.cb = sizeof(VIOMODEINFO);
    VioGetMode(&ModeData, VIO_HANDLE);

    usFill = (VioColor(iForeColor, iBackColor) * 0x100) + 0x20;

    for (iCounter = 0; iCounter <= ModeData.row; iCounter++)
        VioWrtNCell((PBYTE) &usFill, ModeData.col, iCounter, 0, VIO_HANDLE);

    VioSetCurPos(0, 0, VIO_HANDLE);
    }


/*-----------------------------------------------------------------------------
   VioColorPrintString: Print string with color settings.
-----------------------------------------------------------------------------*/
void VioColorPrintString (int iRow, int iCol, int iForeColor, int iBackColor, char *szString)
     {
     BYTE              usFill;

     iRow--;
     iCol--;
     usFill = VioColor(iForeColor, iBackColor);
     VioWrtCharStrAtt(szString, strlen(szString), iRow, iCol, &usFill, VIO_HANDLE);
     }


/*-----------------------------------------------------------------------------
   VioPrintString: Print string (dont modify background).
-----------------------------------------------------------------------------*/
void VioPrintString (int iRow, int iCol, char *szString)
     {
     iRow--;
     iCol--;
     VioWrtCharStr(szString, strlen(szString), iRow, iCol, VIO_HANDLE);
     }


/*-----------------------------------------------------------------------------
   VioQueryCursorRow: Get current cursor row.
-----------------------------------------------------------------------------*/
int VioQueryCursorRow(void)
     {
     USHORT    Row;
     USHORT    Column;


     VioGetCurPos(&Row, &Column, VIO_HANDLE);
     return(++Row);
     }


/*-----------------------------------------------------------------------------
   VioQueryCursorColumn: Get current cursor column.
-----------------------------------------------------------------------------*/
int VioQueryCursorColumn(void)
     {
     USHORT    Row;
     USHORT    Column;


     VioGetCurPos(&Row, &Column, VIO_HANDLE);
     return(++Column);
     }


/*-----------------------------------------------------------------------------
   VioSetCursorPosition: Move cursor.
-----------------------------------------------------------------------------*/
void VioSetCursorPosition(int iRow, int iCol)
     {
     iRow--;
     iCol--;
     VioSetCurPos(iRow, iCol, VIO_HANDLE);
     }


/*-----------------------------------------------------------------------------
   VioShowCursor: Show or hide cursor.
-----------------------------------------------------------------------------*/
void VioShowCursor(BOOL bFlag)
     {
     VIOCURSORINFO    CursorData;


     VioGetCurType(&CursorData, VIO_HANDLE);
     if (bFlag == FALSE)
        CursorData.attr = -1;
     else
        CursorData.attr = 1;
     VioSetCurType(&CursorData, VIO_HANDLE);
     }

/*-----------------------------------------------------------------------------
   VioQueryCursorVisible: Is cursor shown or hidden.
-----------------------------------------------------------------------------*/
BOOL VioQueryCursorVisible(void)
     {
     VIOCURSORINFO    CursorData;


     VioGetCurType(&CursorData, VIO_HANDLE);
     if (CursorData.attr == -1)
        return(FALSE);
     else
        return(TRUE);
     }


/*-----------------------------------------------------------------------------
   VioColor: Converts fg/bg pair to attribute value.
-----------------------------------------------------------------------------*/
USHORT VioColor(USHORT usFore, USHORT usBack)
     {
     USHORT   usTemp;


     usTemp = (usFore & 16) * 8 + (usBack * 16) + (usFore & 15);
     if (usTemp & 128)
        usTemp = usTemp - 128;

     return (usTemp);
     }


/*-----------------------------------------------------------------------------
   VioGetWindow: Saves part of screen area in a string.
-----------------------------------------------------------------------------*/
char *VioGetWindow(int iRow1, int iCol1, int iRow2, int iCol2)
      {
      int               iCounter;
      char              *pbWindow, *pStrPos;
      unsigned short    ulTemp;

      pbWindow = (char*) calloc( ((iRow2 - iRow1 + 1) * 2) * (iCol2 - iCol1 + 1), sizeof(char) );

      pStrPos = pbWindow;

      for (iCounter = iRow1; iCounter <= iRow2; iCounter++)
          {
          ulTemp = (iCol2 - iCol1 + 1) * 2;
          VioReadCellStr(pStrPos, &ulTemp, iCounter - 1, iCol1 - 1, VIO_HANDLE);
          pStrPos += (iCol2 - iCol1 + 1) * 2;
          }
      return (pbWindow);
      }


/*-----------------------------------------------------------------------------
   VioPutWindow: Restores window saved by VioGetWindow.
-----------------------------------------------------------------------------*/
void VioPutWindow(char *pbWindow, int iRow1, int iCol1, int iRow2, int iCol2)
      {
      int               iCounter;
      char              *pStrPos;
      unsigned short    ulTemp;

      pStrPos = pbWindow;

      for (iCounter = iRow1; iCounter <= iRow2; iCounter++)
          {
          ulTemp = (iCol2 - iCol1 + 1) * 2;
          VioWrtCellStr(pStrPos, ulTemp, iCounter - 1, iCol1 - 1, VIO_HANDLE);
          pStrPos += (iCol2 - iCol1 + 1) * 2;
          }

      free (pbWindow);
      }


/*-------------------------------------------------------------------------------
   VioFrameFill: Draws a frame and fills contents with attribute.
-------------------------------------------------------------------------------*/
void VioFrameFill(int iRow1, int iCol1, int iRow2, int iCol2, int iForeColor, int iBackColor, int iFrameType)
     {
     int                iCounter;
     char               *pStrPos;
     unsigned short     ulTemp;
     unsigned short     usFill;
     char               cUL, cUR, cLL, cLR, cH, cV;


     iRow1--;
     iCol1--;
     iRow2--;
     iCol2--;

	 // Set fill color and blank (32) space
     usFill = (VioColor(iForeColor, iBackColor) * 0x100) + 32;
     for (iCounter = iRow1; iCounter <= iRow2; iCounter++)
         VioWrtNCell((PBYTE) &usFill, (iCol2 - iCol1 + 1), iCounter, iCol1, VIO_HANDLE);

     // Get frame character    
     switch (iFrameType)
            {
             case 0:
                  cUL = 0xc9;
                  cUR = 0xbb;
                  cLL = 0xc8;
                  cLR = 0xbc;
                  cH  = 0xcd;
                  cV  = 0xba;
                  break;

             case 255:
                  cUL = 0xda;
                  cUR = 0xbf;
                  cLL = 0xc0;
                  cLR = 0xd9;
                  cH  = 0xc4;
                  cV  = 0xb3;
                  break;

            default:
                 cUL = cUR = cLL = cLR = cH = cV = iFrameType;
                 break;
             }

      VioWrtNChar((PBYTE) &cUL, 1, iRow1, iCol1, VIO_HANDLE);
      VioWrtNChar((PBYTE) &cH, iCol2 - iCol1 - 1, iRow1, iCol1 + 1, VIO_HANDLE);
      VioWrtNChar((PBYTE) &cUR, 1, iRow1, iCol2, VIO_HANDLE);

      for (iCounter = iRow1 + 1; iCounter <= iRow2 - 1; iCounter++)
          {
          VioWrtNChar((PBYTE) &cV, 1, iCounter, iCol1, VIO_HANDLE);
          VioWrtNChar((PBYTE) &cV, 1, iCounter, iCol2, VIO_HANDLE);
          }

      VioWrtNChar((PBYTE) &cLL, 1, iRow2, iCol1, VIO_HANDLE);
      VioWrtNChar((PBYTE) &cH, iCol2 - iCol1 - 1, iRow2, iCol1 + 1, VIO_HANDLE);
      VioWrtNChar((PBYTE) &cLR, 1, iRow2, iCol2, VIO_HANDLE);
      }

/*-----------------------------------------------------------------------------
   VioFill: Fills line with blank and color.
-----------------------------------------------------------------------------*/
void VioFill(int iRow, int iCol, int iLen, int iForeColor, int iBackColor)
     {
     USHORT     usFill;

     iRow--;
     iCol--;
     usFill = (VioColor(iForeColor, iBackColor) * 0x100) + 32;
     VioWrtNCell((PBYTE) &usFill, iLen, iRow, iCol, VIO_HANDLE);
     }


/*-----------------------------------------------------------------------------
   VioFillChar: Fills line with character.
-----------------------------------------------------------------------------*/
void VioFillChar(int iRow, int iCol, int iLen, char cChar)
     {
     USHORT     usFill;

     iRow--;
     iCol--;
     VioWrtNChar((PBYTE) &cChar, iLen, iRow, iCol, VIO_HANDLE);
     }


/*-----------------------------------------------------------------------------
   VioShade: Draws shade for specified triangle.
-----------------------------------------------------------------------------*/
void VioShade(int iRow1, int iCol1, int iRow2, int iCol2)
     {
     int                iCounter;
     USHORT             usFill;


     iRow1--;
     iCol1--;
     iRow2--;
     iCol2--;

     usFill = VioColor(COLOR_GRAY, COLOR_BLACK);
     VioWrtNAttr((PBYTE) &usFill, iCol2 - iCol1 + 1, iRow2 + 1, iCol1 + 1, VIO_HANDLE);

     for (iCounter = iRow1 + 1; iCounter <= iRow2 + 1; iCounter++)
         {
         VioWrtNAttr((PBYTE) &usFill, 1, iCounter, iCol2 + 1, VIO_HANDLE);
         VioWrtNAttr((PBYTE) &usFill, 1, iCounter, iCol2 + 2, VIO_HANDLE);
         }
     }


/*-----------------------------------------------------------------------------
   VioInkey: Get keypress.
-----------------------------------------------------------------------------*/
int VioInkey(BOOL bWaitFlag, int *iShiftstate)
     {
     KBDKEYINFO         CharData;


     if (bWaitFlag == 0)
        KbdCharIn(&CharData, 1, KBD_HANDLE);
     else
        KbdCharIn(&CharData, 0, KBD_HANDLE);

     if (iShiftstate != NULL)
        *iShiftstate = CharData.fsState;

     if (CharData.chScan == 224)
        {
        return(CharData.chChar);
        }
     else
        {
        if (CharData.fbStatus == 66)
           return(-CharData.chScan);
        else
           return(CharData.chChar);
        }
     }


/*-----------------------------------------------------------------------------
   VioTimer: Returns milliseconds since system start.
-----------------------------------------------------------------------------*/
int VioTimer(void)
    {
    int         iMilliSec;


    DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &iMilliSec, 4);
    return(iMilliSec);
    }


/*-----------------------------------------------------------------------------
   strcenter: Center string within positions.
-----------------------------------------------------------------------------*/
int strcenter(int x1, int x2, char *szString)
    {
    if (strlen(szString) >= x2 - x1 + 1)
       return (x1);
    else
       return ((x2 - x1 - strlen(szString) + x1) / 2 + 1);
    }


// ---------------------------------------------------------------------
//  VioDesktop:
// ---------------------------------------------------------------------
void VioDesktop(PSTVIO pstv, char *szRubrik, int iFillChar, BOOL bStatus)
     {
     int	iCounter;
     char	*szFillStr;

     VioSetCursorPosition(1, 1);
     VioShowCursor(FALSE);

     VioFrameFill(1, 1, pstv->iMaxRows, pstv->iMaxCols, pstv->iwfcolor, pstv->iwbcolor, 32);

     if (iFillChar == -1)
        iFillChar = 177;

     szFillStr = (char*) calloc(pstv->iMaxCols + 1, sizeof(char));
     memset(szFillStr, iFillChar, pstv->iMaxCols);
     szFillStr[pstv->iMaxCols + 1] = '\0';
     for (iCounter = 1; iCounter <= pstv->iMaxRows; iCounter++)
         VioPrintString(iCounter, 1, szFillStr);
     free(szFillStr);

     if (strlen(szRubrik) > 0)
        {
        VioFill(1, 1, pstv->iMaxCols, pstv->ibbcolor, pstv->ibfcolor);
        if (strlen(szRubrik) > pstv->iMaxCols - 4)
           szRubrik[pstv->iMaxCols - 3] = '\0';
        VioPrintString(1, 2, szRubrik);
        }

     if (bStatus == TRUE)
        VioFill(pstv->iMaxRows, 1, pstv->iMaxCols, pstv->ibbcolor, pstv->ibfcolor);
     }


// ---------------------------------------------------------------------
//  VioInit: Initiate Vio environment.
// ---------------------------------------------------------------------
PSTVIO VioInit (int iSetMaxRow, int iSetMaxCol)
     {
     int        iMaxRows, iMaxCols;
     PSTVIO     pstv;


     // Create this window struct
     pstv = (PSTVIO) calloc(1, sizeof(STVIO));

     pstv->bFirstTime = TRUE;
     VioGetInfo(&iMaxRows, &iMaxCols, (int*) NULL);
     pstv->iOrgMaxRows = iMaxRows;
     pstv->iOrgMaxCols = iMaxCols;

     pstv->iOrgCursorRow = VioQueryCursorRow();
     pstv->iOrgCursorCol = VioQueryCursorColumn();

     pstv->bOrgCursorVisible = VioQueryCursorVisible();

     pstv->pchOrgScreen = VioGetWindow(1, 1, pstv->iOrgMaxRows, pstv->iOrgMaxCols);

     if (iSetMaxRow == -1)
        pstv->iMaxRows = iMaxRows;
     else
        pstv->iMaxRows = iSetMaxRow;

     if (iSetMaxCol == -1)
        pstv->iMaxCols = iMaxCols;
     else
        pstv->iMaxCols = iSetMaxCol;

     if (pstv->iMaxRows != pstv->iOrgMaxRows || pstv->iMaxCols != pstv->iOrgMaxCols)
        VioSetScreenSize(pstv->iMaxRows, pstv->iMaxCols);

     pstv->ibfcolor = COLOR_BLACK;
     pstv->ibbcolor = COLOR_WHITE;
     pstv->iwfcolor = COLOR_BLACK;
     pstv->iwbcolor = COLOR_WHITE;
     pstv->iffcolor = COLOR_BLACK;
     pstv->ifbcolor = COLOR_WHITE;
     pstv->iefcolor = COLOR_WHITE;
     pstv->iebcolor = COLOR_BLACK;

     return(pstv);
     }

// ---------------------------------------------------------------------
//  VioExit: Exit Vio environment.
// ---------------------------------------------------------------------
void VioExit(PSTVIO pstv)
     {
     int        iMaxRows, iMaxCols;


     if (pstv->iMaxRows != pstv->iOrgMaxRows || pstv->iMaxCols != pstv->iOrgMaxCols)
        VioSetScreenSize(pstv->iOrgMaxRows, pstv->iOrgMaxCols);

     VioPutWindow(pstv->pchOrgScreen, 1, 1, pstv->iOrgMaxRows, pstv->iOrgMaxCols);

     VioSetCursorPosition(pstv->iOrgCursorRow, pstv->iOrgCursorCol);
     VioShowCursor(pstv->bOrgCursorVisible);

     free(pstv);
     }


// ---------------------------------------------------------------------
//  VioSetGlobal: Set global values.
// ---------------------------------------------------------------------
void VioSetGlobal(PSTVIO pstv, BOOL bUseColor, int iFrame, BOOL bShade, BOOL bZoom, int iZoomTime, int bf, int bb, int wf, int wb, int ff, int fb, int ef, int eb)
     {
     if (iFrame == -1)
        pstv->iFrame = 32;
     else
        pstv->iFrame = iFrame;

     pstv->bShade = bShade;
     pstv->bZoom = bZoom;

     if (iZoomTime == -1)
        pstv->iZoomTime = 6;
     else
        pstv->iZoomTime = iZoomTime;

     pstv->bUseColor = bUseColor;
     if (pstv->bUseColor == FALSE)
        {
        pstv->ibfcolor = COLOR_BLACK;
        pstv->ibbcolor = COLOR_WHITE;
        pstv->iwfcolor = COLOR_BLACK;
        pstv->iwbcolor = COLOR_WHITE;
        pstv->iffcolor = COLOR_BLACK;
        pstv->ifbcolor = COLOR_WHITE;
        pstv->iefcolor = COLOR_WHITE;
        pstv->iebcolor = COLOR_BLACK;
        }
     else
        {
        if (bf != -1)
           pstv->ibfcolor = bf;

        if (bb != -1)
           pstv->ibbcolor = bb;

        if (wf != -1)
           pstv->iwfcolor = wf;

        if (wb != -1)
           pstv->iwbcolor = wb;

        if (ff != -1)
           pstv->iffcolor = ff;

        if (fb != -1)
           pstv->ifbcolor = fb;

        if (ef != -1)
           pstv->iefcolor = ef;

        if (eb != -1)
           pstv->iebcolor = eb;
        }

// All other settings...
// delimiters and so on

     pstv->bFirstTime = FALSE;
     }


// ---------------------------------------------------------------------
//  VioOpenStandardWindow
// ---------------------------------------------------------------------
PSTVIOWINDOW VioOpenStandardWindow(PSTVIO pstv, int iRow1, int iCol1, int iRowLen, int iColLen, char *szRubrik)
     {
     PSTVIOWINDOW       hwnd;
     int                iRow2, iCol2;
     char               *wndBuffer;
     int                iRowTo, iColFr, iColTo;


     // Create this window struct
     hwnd = (PSTVIOWINDOW) calloc(1, sizeof(STVIOWINDOW));

     // Save cursor position
     hwnd->iOldRow = VioQueryCursorRow();
     hwnd->iOldCol = VioQueryCursorColumn();
     hwnd->iOldCursorShow = VioQueryCursorVisible();

     // If Vio is not initialized, do so
     if (pstv->bFirstTime == TRUE)
        VioSetGlobal(pstv, TRUE, 32, TRUE, TRUE, 30, -1, -1, -1, -1, -1, -1, -1, -1);

     hwnd->pstv = pstv;
     hwnd->bShade = pstv->bShade;
     hwnd->iFrame = pstv->iFrame;
     hwnd->bZoom = pstv->bZoom;
     hwnd->ibfcolor = pstv->ibfcolor;
     hwnd->ibbcolor = pstv->ibbcolor;
     hwnd->iwfcolor = pstv->iwfcolor;
     hwnd->iwbcolor = pstv->iwbcolor;
     hwnd->iffcolor = pstv->iffcolor;
     hwnd->ifbcolor = pstv->ifbcolor;
     hwnd->iefcolor = pstv->iefcolor;
     hwnd->iebcolor = pstv->iebcolor;

     iRow2 = iRow1 + iRowLen - 1;
     iCol2 = iCol1 + iColLen - 1;

     // Set coordinates
     iRowTo = iRow2;
     iColFr = iCol1;
     iColTo = iCol2;

     // Check coordinates
     if (iRow1 < 1)
        iRow1 = 1;

     if (iRowTo > pstv->iMaxRows)
        iRowTo = pstv->iMaxRows;

     if (iColFr < 1)
        iColFr = 1;

     if (iColTo > pstv->iMaxCols)
        iColTo = pstv->iMaxCols;

     // Set windows positions in hwnd struct
     hwnd->iRow1 = iRow1;
     hwnd->iCol1 = iCol1;
     hwnd->iWndRows = iRow2 - iRow1 - 1;
     hwnd->iWndCols = iCol2 - iCol1 - 1;
     if (hwnd->bShade == TRUE)
        {
        hwnd->iRow2 = iRow2 + 1;
        hwnd->iCol2 = iCol2 + 2;
        }
     else
        {
        hwnd->iRow2 = iRow2;
        hwnd->iCol2 = iCol2;
        }

     // Save background
     hwnd->wndBuffer = VioGetWindow(hwnd->iRow1, hwnd->iCol1, hwnd->iRow2, hwnd->iCol2);

     // Zoom if requested
     if (hwnd->bZoom == TRUE)
        ZoomWindow(pstv, iRow1, iCol1, iRow2, iCol2, hwnd->ibfcolor, hwnd->iwbcolor, hwnd->iFrame);

     // Draw shade if requested
     if (hwnd->bShade == TRUE)
        VioShade(iRow1, iColFr, iRowTo, iColTo);

     // Draw Window
     VioFrameFill(iRow1, iCol1, iRow2, iCol2, hwnd->ibfcolor, hwnd->ibbcolor, hwnd->iFrame);
     VioFrameFill(iRow1 + 1, iCol1 + 1, iRow2 - 1, iCol2 - 1, hwnd->iwfcolor, hwnd->iwbcolor, 32);

     // Draw titlebar with titlebar text
     if (strlen(szRubrik) > 0)
        {
        VioFill(iRow1, iColFr, iColTo - iColFr + 1, hwnd->ibbcolor, hwnd->ibfcolor);

        // If string is to long, clip it
        if (strlen(szRubrik) > iCol2 - iCol1 - 3)
           szRubrik[iCol2 - iCol1 - 1] = '\0';

        VioPrintString(iRow1, iCol1 + 2, szRubrik);
        }
     return (hwnd);
     }


// ---------------------------------------------------------------------
//  VioOpenWindow
// ---------------------------------------------------------------------
PSTVIOWINDOW VioOpenWindow(PSTVIO pstv, int iRow1, int iCol1, int iRowLen, int iColLen, char *szRubrik, int iFrame, int iShade, int iZoom, int iZoomTime, int bf, int bb, int wf, int wb, int ff, int fb, int ef, int eb)
     {
     PSTVIOWINDOW       hwnd;
     int                iRow2, iCol2;
     char               *wndBuffer;
     int                iRowTo, iColFr, iColTo;


     // Create this window struct
     hwnd = (PSTVIOWINDOW) calloc(1, sizeof(STVIOWINDOW));

     // Save cursor position
     hwnd->iOldRow = VioQueryCursorRow();
     hwnd->iOldCol = VioQueryCursorColumn();
     hwnd->iOldCursorShow = VioQueryCursorVisible();

     // If Vio is not initialized, do so
     if (pstv->bFirstTime == TRUE)
        VioSetGlobal(pstv, TRUE, iFrame, iShade, iZoom, 30, -1, -1, -1, -1, -1, -1, -1, -1);

     hwnd->pstv = pstv;

     // Check for specific shade
     if (iShade == -1)
        hwnd->bShade = pstv->bShade;
     else
        hwnd->bShade = iShade;

     // Check for specific frame
     if (iFrame == -1)
        hwnd->iFrame = pstv->iFrame;
     else
        hwnd->iFrame = iFrame;

     // Check for specific zoom
     if (iZoom == -1)
        hwnd->bZoom = pstv->bZoom;
     else
        hwnd->bZoom = iZoom;

     // Check for specific colors
     if (bf == -1)
        hwnd->ibfcolor = pstv->ibfcolor;

     if (bb == -1)
        hwnd->ibbcolor = pstv->ibbcolor;

     if (wf == -1)
        hwnd->iwfcolor = pstv->iwfcolor;

     if (wb == -1)
        hwnd->iwbcolor = pstv->iwbcolor;

     if (ff == -1)
        hwnd->iffcolor = pstv->iffcolor;

     if (fb == -1)
        hwnd->ifbcolor = pstv->ifbcolor;

     if (ef == -1)
        hwnd->iefcolor = pstv->iefcolor;

     if (eb == -1)
        hwnd->iebcolor = pstv->iebcolor;

     iRow2 = iRow1 + iRowLen - 1;
     iCol2 = iCol1 + iColLen - 1;

     // Set coordinates
     iRowTo = iRow2;
     iColFr = iCol1;
     iColTo = iCol2;

     // Check coordinates
     if (iRow1 < 1)
        iRow1 = 1;

     if (iRowTo > pstv->iMaxRows)
        iRowTo = pstv->iMaxRows;

     if (iColFr < 1)
        iColFr = 1;

     if (iColTo > pstv->iMaxCols)
        iColTo = pstv->iMaxCols;

     // Set windows positions in hwnd struct
     hwnd->iRow1 = iRow1;
     hwnd->iCol1 = iCol1;
     hwnd->iWndRows = iRow2 - iRow1 - 1;
     hwnd->iWndCols = iCol2 - iCol1 - 1;

     if (hwnd->bShade == TRUE)
        {
        hwnd->iRow2 = iRow2 + 1;
        hwnd->iCol2 = iCol2 + 2;
        }
     else
        {
        hwnd->iRow2 = iRow2;
        hwnd->iCol2 = iCol2;
        }

     // Save background
     hwnd->wndBuffer = VioGetWindow(hwnd->iRow1, hwnd->iCol1, hwnd->iRow2, hwnd->iCol2);

     // Zoom if requested
     if (hwnd->bZoom == TRUE)
        ZoomWindow(pstv, iRow1, iCol1, iRow2, iCol2, hwnd->ibfcolor, hwnd->iwbcolor, iFrame);

     // Draw shade if requested
     if (hwnd->bShade == TRUE)
        VioShade(iRow1, iColFr, iRowTo, iColTo);

     // Draw Window
     VioFrameFill(iRow1, iCol1, iRow2, iCol2, hwnd->ibfcolor, hwnd->ibbcolor, iFrame);
     VioFrameFill(iRow1 + 1, iCol1 + 1, iRow2 - 1, iCol2 - 1, hwnd->iwfcolor, hwnd->iwbcolor, 32);

     // Draw titlebar with titlebar text
     if (strlen(szRubrik) > 0)
        {
        VioFill(iRow1, iColFr, iColTo - iColFr + 1, hwnd->ibbcolor, hwnd->ibfcolor);

        // If string is to long, clip it
        if (strlen(szRubrik) > iCol2 - iCol1 - 3)
           szRubrik[iCol2 - iCol1 - 1] = '\0';

        VioPrintString(iRow1, iCol1 + 2, szRubrik);

        }
     return (hwnd);
     }


// ---------------------------------------------------------------------
//  VioCloseWindow:
// ---------------------------------------------------------------------
void VioCloseWindow (PSTVIOWINDOW hwnd)
     {
     VioPutWindow(hwnd->wndBuffer, hwnd->iRow1, hwnd->iCol1, hwnd->iRow2, hwnd->iCol2);
     VioSetCursorPosition(hwnd->iOldRow, hwnd->iOldCol);
     VioShowCursor(hwnd->iOldCursorShow);
     free(hwnd);
     }


// ---------------------------------------------------------------------
//  VioGetKey:
// ---------------------------------------------------------------------
int VioGetKey(PSTVIOWINDOW hwnd, BOOL bWaitForKey, BOOL bEcho)
    {
    int         iKeyPress;
    char        szBuffer[8];


    if (bWaitForKey == TRUE)
       iKeyPress = VioInkey(TRUE, NULL);
    else
       iKeyPress = VioInkey(FALSE, NULL);

    if (bEcho == TRUE && iKeyPress > 0)
       {
       sprintf(szBuffer, "%c", iKeyPress);
       VioPrint(hwnd, hwnd->iCursorRow, hwnd->iCursorCol, szBuffer);
       }
    return(iKeyPress);
    }


// ---------------------------------------------------------------------
//  VioGetKeyBoardShift:
// ---------------------------------------------------------------------
int VioGetKeyBoardShift(PSTVIOWINDOW hwnd)
    {
    int         iShiftstate;


    VioInkey(FALSE, &iShiftstate);
    return(iShiftstate);
    }


// ---------------------------------------------------------------------
//  VioSetCursor:
// ---------------------------------------------------------------------
void VioSetCursor (PSTVIOWINDOW hwnd, int iRow, int iCol, int iShow)
    {
    int         iShiftstate;


    ValidPos (hwnd, &iRow, &iCol, NULL);
    hwnd->iCursorRow = iRow;
    hwnd->iCursorCol = iCol;
    if (iShow != -1)
       hwnd->bCursorShow = iShow;

    VioSetCursorPosition(hwnd->iRow1 + hwnd->iCursorRow, hwnd->iCol1 + hwnd->iCursorCol);
    VioShowCursor(hwnd->bCursorShow);
    }


// ---------------------------------------------------------------------
//  VioGetCursor:
// ---------------------------------------------------------------------
void VioGetCursor (PSTVIOWINDOW hwnd, int *iRow, int *iCol, int *iShow)
    {
    int         iShiftstate;

// This code is incorrect !!!!!!!!!!!!!
    if (iRow != NULL)
       *iRow = VioQueryCursorRow();

    if (iRow != NULL)
       *iCol = VioQueryCursorColumn();

    if (iRow != NULL)
       *iShow = VioQueryCursorVisible();
    }


// ---------------------------------------------------------------------
//  ValidPos: Validates positions (to be within window).
// ---------------------------------------------------------------------
void ValidPos (PSTVIOWINDOW hwnd, int *iRow, int *iCol, int *iLen)
     {
     if (*iRow > hwnd->iWndRows)
        *iRow = hwnd->iWndRows;

     if (*iCol >= hwnd->iWndCols)
        *iCol = hwnd->iWndCols;

     if (iLen !=NULL)
        {
        if (*iCol + *iLen > hwnd->iWndCols)
           *iLen = hwnd->iWndCols - *iCol;
        }
     }


// ---------------------------------------------------------------------
//  ZoomWindow: Zooms window.
// ---------------------------------------------------------------------
void ZoomWindow (PSTVIO pstv, int iRow1, int iCol1, int iRow2, int iCol2, int iForeColor, int iBackColor, int iFrame)
     {
     int        iRowMitt, iColMitt, iColStep, iRowStep, iRow, iCol;
     int        iCounter, iTimer;


     iRowMitt = iRow1 + ((iRow2 - iRow1) / 2);
     iColMitt = iCol1 + ((iCol2 - iCol1) / 2);

     if (iCol2 - iCol1 > iRow2 - iRow1)
        {
        iColStep = (iCol2 - iCol1) / (iRow2 - iRow1);
        for (iCounter = 1; iCounter <= (iRow2 - iRow1) / 2; iCounter++)
            {
            iCol = iColStep * iCounter;
            VioFrameFill(iRowMitt - iCounter, iColMitt - iCol, iRowMitt + iCounter, iColMitt + iCol, iForeColor, iBackColor, iFrame);
            iTimer = VioTimer();
            while (iTimer + pstv->iZoomTime > VioTimer())
                  ;
            }
        }
     else
        {
        iRowStep = (iRow2 - iRow1) / (iCol2 - iCol1);
        for (iCounter = 1; iCounter <= (iCol2 - iCol1) / 2; iCounter++)
            {
            iRow = iRowStep * iCounter;
            VioFrameFill(iRowMitt - iRow, iColMitt - iCounter, iRowMitt + iRow, iColMitt + iCounter, iForeColor, iBackColor, iFrame);
            while (iTimer + pstv->iZoomTime > VioTimer())
                  ;
            }
        }
     }

// ---------------------------------------------------------------------
//  VioColorPrint:
// ---------------------------------------------------------------------
void VioColorPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, int fc, int bc, char *szText)
     {
     char       szString[256];
     int        iLen = strlen(szText);


     ValidPos(hwnd, &iRow, &iCol, &iLen);
     memset(szString, 0, 256);
     strncpy(szString, szText, iLen);

     VioColorPrintString (hwnd->iRow1 + iRow, hwnd->iCol1 + iCol, fc, bc, szString);
     }


// ---------------------------------------------------------------------
//  VioEditPrint:
// ---------------------------------------------------------------------
void VioEditPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText)
     {
     char       szString[256];
     int        iLen = strlen(szText);


     ValidPos(hwnd, &iRow, &iCol, &iLen);
     memset(szString, 0, 256);
     strncpy(szString, szText, iLen);

     VioColorPrintString (hwnd->iRow1 + iRow, hwnd->iCol1 + iCol, hwnd->iefcolor, hwnd->iebcolor, szString);
     }

// ---------------------------------------------------------------------
//  VioFieldPrint:
// ---------------------------------------------------------------------
void VioFieldPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText)
     {
     char       szString[256];
     int        iLen = strlen(szText);


     ValidPos(hwnd, &iRow, &iCol, &iLen);
     memset(szString, 0, 256);
     strncpy(szString, szText, iLen);

     VioColorPrintString (hwnd->iRow1 + iRow, hwnd->iCol1 + iCol, hwnd->iffcolor, hwnd->ifbcolor, szString);
     }


// ---------------------------------------------------------------------
//  VioFrame:
// ---------------------------------------------------------------------
void VioFrame (PSTVIOWINDOW hwnd, char *szTitle, int iRow1, int iCol1, int iRowTo, int iColTo, int iFrame)
     {
     int                iCounter;
     char               cUL, cUR, cLL, cLR, cH, cV;
     char               szBuffer[256], szTemp[256];


     if (iFrame == -1)
        iFrame = hwnd->iFrame;

     switch (iFrame)
            {
             case 0:
                  cUL =0xc9;
                  cUR =0xbb;
                  cLL =0xc8;
                  cLR =0xbc;
                  cH =0xcd;
                  cV =0xba;
                  break;

             case 255:
                  cUL =0xda;
                  cUR =0xbf;
                  cLL =0xc0;
                  cLR =0xd9;
                  cH  =0xc4;
                  cV  =0xb3;
                  break;

            default:
                 cUL = cUR = cLL = cLR = cH = cV = hwnd->iFrame;
                 break;
            }

     memset(szTemp, 0, 256);
     memset(szTemp, cH, iColTo - 1);
     sprintf(szBuffer, "%c%s%c", cUL, szTemp, cUR);
     VioPrint(hwnd, iRow1, iCol1, szBuffer);

     for (iCounter = iRow1 + 1; iCounter <= iRow1 + iRowTo - 1; iCounter++)
         {
         sprintf(szBuffer, "%c", cV);
         VioPrint(hwnd, iCounter, iCol1, szBuffer);
         VioPrint(hwnd, iCounter, iCol1 + iColTo, szBuffer);
         }

     memset(szTemp, 0, 256);
     memset(szTemp, cH, iColTo - 1);
     sprintf(szBuffer, "%c%s%c", cLL, szTemp, cLR);
     VioPrint(hwnd, iRow1 + iRowTo, iCol1, szBuffer);

     if (strlen(szTitle) > 0)
        {
        sprintf(szBuffer, " %s ", szTitle);
        VioPrint(hwnd, iRow1, iCol1 + 1, szBuffer);
        }
     }


// ---------------------------------------------------------------------
//  VioPrint:
// ---------------------------------------------------------------------
void VioPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText)
     {
     char       szString[256];
     int        iLen = strlen(szText);


     ValidPos(hwnd, &iRow, &iCol, &iLen);
     memset(szString, 0, 256);
     strncpy(szString, szText, iLen);

     VioPrintString (hwnd->iRow1 + iRow, hwnd->iCol1 + iCol, szString);
     }


// ---------------------------------------------------------------------
//  VioStatusPrint:
// ---------------------------------------------------------------------
void VioStatusPrint(PSTVIO pstv, int iCol, int iField, BOOL bInverse, char *szText)
     {
     int        iRow;


     if (iField == 0)
        iRow = 1;
     else
        iRow = pstv->iMaxRows;

     if (bInverse == TRUE)
         VioColorPrintString (iRow, iCol, pstv->iffcolor, pstv->ifbcolor, szText);
     else
        VioPrintString (iRow, iCol, szText);
     }

// ---------------------------------------------------------------------
//  VioStatusPrint:
// ---------------------------------------------------------------------
char *VioFormatString(int iVarType, char *szFormatString, void *variable)
     {
     static char        szRetString[128];
     char               szBuffer[128];


     memset (szRetString, 0, 128);

     switch (iVarType)
            {
            case IDFS_STATIC:
            case IDFS_STRING:
                 memset(szRetString, ' ', 128);
                 memcpy(szRetString, (char*) variable, strlen((char*) variable));
                 szRetString[atoi(szFormatString)] = '\0';
                 break;

            case IDFS_LINE:
                 memset(szRetString, '�', atoi(szFormatString));
                 break;

            case IDFS_FRAME:
                 strcpy(szRetString, "ERROR");
                 break;

            case IDFS_INTEGER:
                 sprintf(szBuffer, "%%-%dd", atoi(szFormatString));
                 sprintf(szRetString, szBuffer, * ((int*) variable));
                 break;

            case IDFS_FLOAT:
                 strcpy(szRetString, "ERROR");
                 break;

            case IDFS_DATE:
                 strcpy(szRetString, "ERROR");
                 break;

            case IDFS_TIME:
                 strcpy(szRetString, "ERROR");
                 break;

            case IDFS_DROP:
                 strcpy(szRetString, "ERROR");
                 break;

            case IDFS_BUTTON:
                 sprintf(szRetString, " < %s > ", szFormatString);
                 break;

            case IDFS_BOOL:
                 if (* ((BOOL*)variable) == FALSE)
                    szRetString[0] = szFormatString[0];
                 else
                    szRetString[0] = szFormatString[1];
                 break;

            case IDFS_CHECK:
                 if (* ((BOOL*)variable) == TRUE)
                    sprintf(szRetString, "[X] %s", szFormatString);
                 else
                    sprintf(szRetString, "[ ] %s", szFormatString);
                 break;

            case IDFS_OPTION:
                 if (* ((BOOL*)variable) == TRUE)
                    sprintf(szRetString, "(%c) %s", 7, szFormatString);
                 else
                    sprintf(szRetString, "( ) %s", szFormatString);
                 break;

            case IDFS_LISTBOX:
                 strcpy(szRetString, "ERROR");
                 break;

            default:
                 strcpy(szRetString, "ERROR");
            }

     return(szRetString);
     }

// ---------------------------------------------------------------------
//  VioEditButton: Button in a vio window.
// ---------------------------------------------------------------------
int VioEditButton(PSTVIOWINDOW hwnd, char *szText, int iRow, int iCol, int iRet, int aiRet[], int iRetLen)
    {
    int         iCurRow, iCurCol, iCurShow;
    int         iCounter, iLength;
    int         iKey, iRc;
    char        szBuffer[255];
    BOOL        bQuit = FALSE;


    sprintf(szBuffer, " < %s > ", szText);
    iLength = strlen(szBuffer);

    ValidPos(hwnd, &iRow, &iCol, &iLength);

    if (strlen(szBuffer) > iLength)
       szBuffer[iLength] = '\0';

    VioGetCursor(hwnd, &iCurRow, &iCurCol, &iCurShow);
    VioSetCursor(hwnd, iRow, iCol + 3, TRUE);

    while (bQuit == FALSE)
          {
          VioFieldPrint(hwnd, iRow, iCol, szBuffer);

          iKey = VioGetKey (hwnd, TRUE, FALSE);

          switch (iKey)
                 {
                 case KEY_SPACE:
                      iRc = iRet;
                      bQuit = TRUE;
                      break;

                 default:
                      for (iCounter = 0; iCounter < iRetLen; iCounter++)
                          {
                          if (aiRet[iCounter] == iKey)
                             {
                             iRc = iKey;
                             bQuit = TRUE;
                             break;
                             }
                          }
                 }
          }
    VioSetCursor(hwnd, iCurRow, iCurCol, iCurShow);
    return(iRc);
    }

// ---------------------------------------------------------------------
//  VioEditInt: Edit an int in a vio window.
// ---------------------------------------------------------------------
int VioEditInt (PSTVIOWINDOW hwnd, int *iValue, int iRow, int iCol, int iFormLen, int aiRet[], int iRetLen)
    {
    char        szIntString[64];
    char        szBuffer[64];
    int         iStrLen;
    int         iRetKey;

    sprintf(szBuffer, "%%-%dd", iFormLen);
    sprintf(szIntString, szBuffer, *iValue);

    iRetKey = VioEditString (hwnd, szIntString, iRow, iCol, strlen(szIntString), aiRet, iRetLen, IDH_INT);

    *iValue = atoi(szIntString);
    return (iRetKey);
    }


// ---------------------------------------------------------------------
//  VioEditString: Edit a string in a vio window.
// ---------------------------------------------------------------------
int VioEditString (PSTVIOWINDOW hwnd, char *szInString, int iRow, int iColHome, int iLength, int aiRet[], int iRetLen, int iHook)
    {
    BOOL        bRedraw = TRUE;
    BOOL        bQuit = FALSE;
    char        szString[255];
    char        szBlank[255];
    int         iRc;
    int         iCol, iColEnd, iStrPos;
    int         kbd;
    int         iCounter;
    int         iCurRow, iCurCol, iCurShow;


    memset(szBlank, ' ', 255);
    memset(szString, ' ', 255);

    ValidPos(hwnd, &iRow, &iColHome, &iLength);

    VioGetCursor (hwnd, &iCurRow, &iCurCol, &iCurShow);
    VioSetCursor(hwnd, iRow, iColHome, TRUE);

    memcpy(szString, szInString, strlen(szInString));

    szString[iLength] = '\0';
    szBlank[iLength] = '\0';

    iCol = iColHome;
    iColEnd = iColHome + iLength + 1;
    iStrPos = 0;

    while (bQuit == FALSE)
          {
          if (bRedraw == TRUE)
             {
             if (iHook != IDH_PSW )
                VioEditPrint(hwnd, iRow, iColHome, szString);
             else
                VioEditPrint(hwnd, iRow, iColHome, szBlank);
             bRedraw = FALSE;
             }

          VioSetCursor(hwnd, iRow, iCol, TRUE);
          kbd = VioGetKey (hwnd, TRUE, FALSE);
          kbd = ESHook(iHook, kbd, iCol, szString);

          switch (kbd)
                 {
                 case KEY_RARROW:
                      iStrPos++;
                      break;

                 case KEY_LARROW:
                      iStrPos--;
                      break;

                 case KEY_HOME:
                      for (iCounter = 0; iCounter < iLength; iCounter++)
                          {
                          if (szString[iCounter] > 32)
                             break;
                          }
                      if (iCounter < iLength - 1)
                         iStrPos = iCounter;
                      else
                         iStrPos = 0;
                      break;

                 case KEY_END:
                      for (iCounter = iLength - 1; iCounter >= 0; iCounter--)
                          if (szString[iCounter] > 32)
                             break;
                      iStrPos = iCounter + 1;
                      break;

                 case KEY_DEL:
                      if (iStrPos == (iLength - 1))
                         szString[iLength - 1] = ' ';
                      else
                         {
                         for (iCounter = iStrPos; iCounter < (iLength - 1); iCounter++)
                             szString[iCounter] = szString[iCounter + 1];
                         szString[iLength - 1] = ' ';
                         }
                      bRedraw = TRUE;
                      break;

                 case KEY_BKSP:
                      if (iStrPos > 0)
                         {
                         // loop and take away character
                         iStrPos -= 1;
                         for (iCounter = iStrPos; iCounter < (iLength - 1); iCounter++)
                             szString[iCounter] = szString[iCounter + 1];
                         szString[iLength - 1] = ' ';
                         bRedraw = TRUE;
                         }
                      break;

                 default:
                      if (kbd > 31)
                         {
                         for (iCounter = iLength - 1; iCounter > iStrPos; iCounter--)
                             szString[iCounter] = szString[iCounter - 1];
                         szString[iStrPos] = kbd;
                         iStrPos++;
                         bRedraw = TRUE;
                         }
                      else
                         {
                         for (iCounter = 0; iCounter < iRetLen; iCounter++)
                             {
                             if (aiRet[iCounter] == kbd)
                                {
                                iRc = kbd;
                                bQuit = TRUE;
                                break;
                                }
                             }
                         }
                      break;
                 }

          if (iStrPos < 0)
             iStrPos = 0;

          if (iStrPos > (iLength - 1))
             iStrPos = iLength - 1;

          iCol = iColHome + iStrPos;
          }

    for (iCounter = iLength - 1; iCounter >= 0; iCounter--)
        {
        if (szString[iCounter] > 32)
           {
           szString[iCounter + 1] = '\0';
           break;
           }
        }

    strcpy(szInString, szString);

    VioSetCursor(hwnd, iCurRow, iCurCol, iCurShow);

    return(iRc);
    }


// ---------------------------------------------------------------------
//  ESHook: Here programmer can add hooks to EditString.       880420
// ---------------------------------------------------------------------
int ESHook(int iHook, int iKey, int iCol, char *szString)
    {
    int         iRetKey;


    if (iKey < KEY_SPACE )
       return (iKey);

    iRetKey = iKey;

    switch (iHook)
           {
           case IDH_UCASE:
                if (iKey >= 'a' && iKey <='z')
                   iRetKey = iKey - 32;

                switch (iKey)
                       {
                       case 0x86:
                            iRetKey = 0x8f;
                            break;

                       case 0x84:
                            iRetKey = 0x8e;
                            break;

                       case 0x94:
                            iRetKey = 0x99;
                            break;

                       case 0x81:
                            iRetKey = 0x9a;
                            break;
                       }
                break;

           case IDH_DATE:
           case IDH_TIME:
                if ( !(iKey >= '0' && iKey <= '9') )
                   iRetKey = 0;
                break;

           case IDH_NUM: /* Fix to support - at the beginning */
           case IDH_INT:
                if ( !(iKey >= '0' && iKey <= '9') )
                   iRetKey = 0;
                break;
           }
    return (iRetKey);
    }


// ---------------------------------------------------------------------
//  WinEditBool:
// ---------------------------------------------------------------------
int VioEditBool(PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szSelect, int aiRet[], int iRetLen)
    {
    char        sFirst, sLast;
    char        szFirst[2], szLast[2];
    BOOL        bQuit = FALSE;
    int         iCurRow, iCurCol, iCurShow;
    int         iKey, iRc, iCounter;


    VioGetCursor (hwnd, &iCurRow, &iCurCol, &iCurShow);
    VioSetCursor(hwnd, iRow, iCol, TRUE);

    sFirst = szSelect[0];
    szFirst[0] = sFirst;
    szFirst[1] = '\0';
    sLast = szSelect[strlen(szSelect) - 1];
    szLast[0] = sLast;
    szLast[1] = '\0';

    while (bQuit == FALSE)
          {
          if (*iCurVal == FALSE)
             VioEditPrint(hwnd, iRow, iCol, szFirst);
          else
             VioEditPrint(hwnd, iRow, iCol, szLast);

          iKey = VioGetKey (hwnd, TRUE, FALSE);


          if (toupper(iKey) == toupper(sFirst))
             *iCurVal = FALSE;

          if (toupper(iKey) == toupper(sLast))
             *iCurVal = TRUE;

          for (iCounter = 0; iCounter < iRetLen; iCounter++)
              {
              if (aiRet[iCounter] == iKey)
                 {
                 iRc = iKey;
                 bQuit = TRUE;
                 break;
                 }
              }
          }
    VioSetCursor (hwnd, iCurRow, iCurCol, iCurShow);
    return (iRc);
    }


// ---------------------------------------------------------------------
//  VioEditCheck:
// ---------------------------------------------------------------------
int VioEditCheck (PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szText, int aiRet[], int iRetLen)
    {
    BOOL        bQuit = FALSE;
    int         iCurRow, iCurCol, iCurShow;
    int         iKey, iRc, iCounter;


    VioGetCursor (hwnd, &iCurRow, &iCurCol, &iCurShow);
    VioSetCursor(hwnd, iRow, iCol + 1, TRUE);

    while (bQuit == FALSE)
          {
          if (*iCurVal == TRUE)
             {
             VioFieldPrint (hwnd, iRow, iCol, "[X] ");
             VioFieldPrint (hwnd, iRow, iCol + 4, szText);
             }
          else
             {
             VioFieldPrint (hwnd, iRow, iCol, "[ ] ");
             VioFieldPrint (hwnd, iRow, iCol + 4, szText);
             }

          iKey = VioGetKey (hwnd, TRUE, FALSE);

          switch (iKey)
                 {
                 case KEY_SPACE:
                      if (*iCurVal == TRUE)
                         *iCurVal = FALSE;
                      else
                         *iCurVal = TRUE;
                      break;

                 default:
                      for (iCounter = 0; iCounter < iRetLen; iCounter++)
                          {
                          if (aiRet[iCounter] == iKey)
                             {
                             iRc = iKey;
                             bQuit = TRUE;
                             break;
                             }
                          }
                 }
          }
    VioSetCursor (hwnd, iCurRow, iCurCol, iCurShow);
    return (iRc);
    }

// ---------------------------------------------------------------------
//  WinEditOption:
// ---------------------------------------------------------------------
int VioEditOption (PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szText, int aiRet[], int iRetLen)
    {
    BOOL        bQuit = FALSE;
    int         iCurRow, iCurCol, iCurShow;
    int         iKey, iRc, iCounter;
    char        szBuffer[255];


    VioGetCursor (hwnd, &iCurRow, &iCurCol, &iCurShow);
    VioSetCursor(hwnd, iRow, iCol+ 1, TRUE);

    while (bQuit == FALSE)
          {
          if (*iCurVal == TRUE)
             {
             sprintf(szBuffer, "(%c) %s", 7, szText);
             VioFieldPrint (hwnd, iRow, iCol, szBuffer);
             }
          else
             {
             sprintf(szBuffer, "( ) %s", szText);
             VioFieldPrint (hwnd, iRow, iCol, szBuffer);
             }

          iKey = VioGetKey (hwnd, TRUE, FALSE);

          switch (iKey)
                 {
                 case KEY_SPACE:
                      if (*iCurVal == TRUE)
                         *iCurVal = FALSE;
                      else
                         *iCurVal = TRUE;
                      break;

                 default:
                      for (iCounter = 0; iCounter < iRetLen; iCounter++)
                          {
                          if (aiRet[iCounter] == iKey)
                             {
                             iRc = iKey;
                             bQuit = TRUE;
                             break;
                             }
                          }
                 }
          }
    VioSetCursor (hwnd, iCurRow, iCurCol, iCurShow);
    return (iRc);
    }


// -----------------------------------------------------------------------------
//  VioListBox: Puts list on screen to choose from.                  1990-12-28
// -----------------------------------------------------------------------------
int VioListBox (PSTVIOWINDOW hwnd, PSTVIOLISTBOX pli, int iListType, char *szStrPtr[], int aiRet[], int iHook)
    {
    int         iTemp, iCounter;
    int         iCurrent, iScrpos, iWidth;
    int         iAttrTemp;
    char        szBuffer[256];
    int         kbd;


    switch (iListType)
           {
           case 0:      // First time, setup list

                // Check to se if it fits

                 pli->lvisible = pli->rowTo;
                 if (pli->lvisible > pli->llen)
                    pli->lvisible = pli->llen;

                 iWidth = pli->colTo;

                 if (pli->startat == 0)
                    {
                    pli->offset = 0;
                    pli->curRow = 0;
                    }
                else
                   {
                   if (pli->startat > pli->llen)
                      pli->startat = 0;

                   if (pli->startat <= pli->lvisible)
                      {
                      pli->offset = 0;
                      pli->curRow = pli->startat;
                      }
                   else
                      {
                      if (pli->startat > (pli->llen - pli->lvisible + 1))
                         {
                         pli->offset = pli->startat - pli->lvisible;
                         pli->curRow = pli->startat - pli->offset;
                         }
                      else
                         {
                         pli->offset = pli->startat;
                         pli->curRow = 0;
                         }
                      }
                   }

                for (iCounter = pli->row1; iCounter <= pli->row1 + pli->rowTo - 1; iCounter++)
                    {
                    iTemp = pli->offset + (iCounter - pli->row1);

                    VioFill(hwnd->iRow1 + iCounter, hwnd->iCol1 + pli->col1, iWidth, hwnd->iebcolor, hwnd->iefcolor);

                    if (iTemp < pli->llen)
                       {
                       memset(szBuffer, 0, 256);
                       strncpy(szBuffer, szStrPtr[iTemp], min(strlen(szStrPtr[iTemp]), iWidth));
                       VioColorPrint(hwnd, iCounter, pli->col1, hwnd->iebcolor, hwnd->iefcolor, szBuffer);
                       }
                    }
                pli->curpos = 1;
                return(1);


           // ------------------------------------------------------------------
           case 1: // Poll list
                iCurrent = pli->offset + pli->curRow;
                if (iCurrent > pli->llen)
                   iCurrent = pli->llen;
                iScrpos = hwnd->iRow1 + pli->row1 + pli->curRow;

                iWidth = pli->colTo;

                if (pli->redraw == TRUE)
                   {
                   for (iCounter = pli->row1; iCounter <= pli->row1 + pli->rowTo - 1; iCounter++)
                       {
                       iTemp = pli->offset + (iCounter - pli->row1);
                       VioFill(hwnd->iRow1 + iCounter, hwnd->iCol1 + pli->col1, iWidth, hwnd->iebcolor, hwnd->iefcolor);

                       if (iTemp < pli->llen)
                          {
                          memset(szBuffer, 0, 256);
                          strncpy(szBuffer, szStrPtr[iTemp], min(strlen(szStrPtr[iTemp]), iWidth));
                          VioColorPrint(hwnd, iCounter, pli->col1, hwnd->iebcolor, hwnd->iefcolor, szBuffer);
                          }
                       }
                   }

                memset(szBuffer, 0, 256);
                strncpy(szBuffer, szStrPtr[iCurrent], min(strlen(szStrPtr[iCurrent]), iWidth));

                VioFill(iScrpos, hwnd->iCol1 + pli->col1, iWidth, hwnd->iefcolor, hwnd->iebcolor);
                VioColorPrint(hwnd, pli->row1 + pli->curRow, pli->col1, hwnd->iefcolor, hwnd->iebcolor, szBuffer);

                kbd = VioGetKey(hwnd, TRUE, FALSE);

                VioFill(iScrpos, hwnd->iCol1 + pli->col1, iWidth, hwnd->iebcolor, hwnd->iefcolor);
                VioColorPrint(hwnd, pli->row1 + pli->curRow, pli->col1, hwnd->iebcolor, hwnd->iefcolor, szBuffer);

                switch (kbd)
                       {
                       case KEY_UARROW:
                            pli->curRow--;
                            break;

                       case KEY_DARROW:
                            pli->curRow++;
                            break;

                       case KEY_PAGEDN:
                            pli->offset = pli->offset + pli->lvisible;
                            break;

                       case KEY_PAGEUP:
                            pli->offset = pli->offset - pli->lvisible;
                            break;

                       case KEY_HOME:
                            pli->offset = 0;
                            pli->curRow = 0;
                            break;

                       case KEY_END:
                            pli->offset = pli->llen - pli->lvisible ;
                            pli->curRow = pli->lvisible - 1;
                            break;

                       case KEY_SPACE:
                            if (pli->mark == TRUE)
                               {
                               if (strlen(szStrPtr[iCurrent]) > 1)
                                  {
                                  if (szStrPtr[iCurrent][0] == 251)
                                     szStrPtr[iCurrent][0] = 32;
                                  else
                                     szStrPtr[iCurrent][0] = 251;

                                  memset(szBuffer, 0, 256);
                                  strncpy(szBuffer, szStrPtr[iCurrent], min(strlen(szStrPtr[iCurrent]), iWidth));
                                  VioFill(iScrpos, hwnd->iCol1 + pli->col1, iWidth, hwnd->iebcolor, hwnd->iefcolor);
                                  VioColorPrint(hwnd, pli->row1 + pli->curRow, pli->col1, hwnd->iebcolor, hwnd->iefcolor, szBuffer);
                                  }
                               pli->curRow = pli->curRow + 1;
                               }
                            break;

                       default:
                            pli->curpos = iCurrent;
                            return (kbd);
                            break;
                       }

                if (pli->lvisible > pli->rowTo)
                   pli->lvisible = pli->rowTo;

                if (pli->offset + pli->curRow >= pli->llen)
                   pli->curRow--;

                if (pli->curRow < 0)
                   {
                   pli->curRow = 0;
                   pli->offset = pli->offset - 1;
                   }
                else if (pli->curRow >= pli->lvisible)
                   {
                   pli->curRow = pli->lvisible - 1;
                   pli->offset = pli->offset + 1;
                   }
                else
                   pli->redraw = FALSE;

                if (pli->offset > pli->llen - pli->lvisible)
                   {
                   pli->offset = pli->llen - pli->lvisible;
                   pli->redraw = FALSE;
                   }

                if (pli->offset < 0)
                   {
                   pli->offset = 0;
                   pli->redraw = FALSE;
                   }

                if (pli->oldoffset != pli->offset)
                   pli->redraw = TRUE;

                pli->oldoffset = pli->offset;

                pli->curpos = pli->offset + pli->curRow - 1;

                return (kbd);

           // ------------------------------------------------------------------
           case 2:      // Exit Listbox
                return (TRUE);

           default:
                return(TRUE);
           }
    }


// ---------------------------------------------------------------------
//  VioMessageBox:
// ---------------------------------------------------------------------
int VioMessageBox (PSTVIO pstv, int iBoxType, char *szRubrik, char *szRad1, char *szRad2, char *szRad3)
    {
    PSTVIOWINDOW		hwnd;
    int				iWinMessageBox;
    BOOL			bQuit = FALSE;
    int				iFieldId = 0;
    int				iScan;


// Base coordinates on screen size !!
    hwnd = VioOpenStandardWindow(pstv, 9, 20, 8, 40, szRubrik);

    VioPrint(hwnd, 2, strcenter(1, 38, szRad1), szRad1);
    VioPrint(hwnd, 3, strcenter(1, 38, szRad2), szRad2);
    VioPrint(hwnd, 4, strcenter(1, 38, szRad3), szRad3);

    switch (iBoxType)
           {
           case 0:
                VioColorPrint(hwnd, 7, 16, hwnd->iefcolor, hwnd->iebcolor, " < OK > ");
                VioSetCursor(hwnd, 6, 19, TRUE);
                while (bQuit == FALSE)
                      {
                      iScan = VioGetKey(hwnd, TRUE, FALSE);
                      switch (iScan)
                             {
                             case KEY_ENTER:
                             case KEY_SPACE:
                             case KEY_ESC:
                                  bQuit = TRUE;
                                  break;
                             }
                       }
                iWinMessageBox = TRUE;
                break;

           case 1:
                VioColorPrint(hwnd, 7, 10, hwnd->iefcolor, hwnd->iebcolor, " < Yes > ");
                VioColorPrint(hwnd, 7, 22, hwnd->iefcolor, hwnd->iebcolor, " < No > ");

                while (bQuit == FALSE)
                      {
                      if (iFieldId == 0)
                         VioSetCursor(hwnd, 6, 13, TRUE);
                      else
                         VioSetCursor(hwnd, 6, 25, TRUE);

                      iScan = VioGetKey(hwnd, TRUE, FALSE);
                      switch (iScan)
                             {
                             case KEY_ENTER:
                             case KEY_SPACE:
                                  bQuit = TRUE;
                                  break;

                             case KEY_RARROW:
                             case 'N':
                             case 'n':
                                  iFieldId = 1;
                                  break;

                             case KEY_LARROW:
                             case 'Y':
                             case 'y':
                                  iFieldId = 0;
                                  break;

                             case KEY_TAB:
                                  iFieldId = !iFieldId;
                                  break;
                             }
                      }
                if (iFieldId == 1)
                   iWinMessageBox = FALSE;
                else
                   iWinMessageBox = TRUE;
                break;

           case 2:
                VioColorPrint(hwnd, 7, 10, hwnd->iefcolor, hwnd->iebcolor, " < No > ");
                VioColorPrint(hwnd, 7, 21, hwnd->iefcolor, hwnd->iebcolor, " < Yes > ");

                while (bQuit == FALSE)
                      {
                      if (iFieldId == 0)
                         VioSetCursor(hwnd, 6, 13, TRUE);
                      else
                         VioSetCursor(hwnd, 6, 24, TRUE);

                      iScan = VioGetKey(hwnd, TRUE, FALSE);
                      switch (iScan)
                             {
                             case KEY_ENTER:
                             case KEY_SPACE:
                                  bQuit = TRUE;
                                  break;

                             case KEY_LARROW:
                             case 'N':
                             case 'n':
                                  iFieldId = 0;
                                  break;

                             case KEY_RARROW:
                             case 'Y':
                             case 'y':
                                  iFieldId = 1;
                                  break;

                             case KEY_TAB:
                                  iFieldId = !iFieldId;
                                  break;
                             }
                      }
                if (iFieldId == 1)
                   iWinMessageBox = TRUE;
                else
                   iWinMessageBox = FALSE;
                break;
           }
    VioSetCursor(hwnd, -1, -1, FALSE);
    VioCloseWindow(hwnd);
    return(iWinMessageBox);
    }

// --------------------------------------------------------------------
//  VioProgressBox: To use in Vio for longer jobs.           890801
// --------------------------------------------------------------------
PSTVIOWINDOW VioProgressBox (PSTVIO pstv, PSTVIOWINDOW hwnd, int iFlag, char *szRubrik, int iRow, int iCol, int iStart, int iStop)
     {
     int                iLen, iStrLen, iPercent;
     char               szBuffer[80];


     switch (iFlag)
            {
            case 0:
                 hwnd = VioOpenStandardWindow(pstv, iRow, iCol, 7, 31, szRubrik);
                 hwnd->iProgressStart = iStart;
                 hwnd->iProgressStep = iStop - iStart;
                 hwnd->iProgressStop = iStop;

                 VioFill(hwnd->iRow1 + 3, hwnd->iCol1 + 3, 25, hwnd->iefcolor, hwnd->iebcolor);
                 VioColorPrint(hwnd, 5, 15, hwnd->iwfcolor, hwnd->iwbcolor, "0%");
                 return (hwnd);
                 break;

            case 1:
                 iStrLen = ((iStop * 100) / hwnd->iProgressStep) / 4;
                 iPercent = (iStop * 100) / hwnd->iProgressStep;
                 VioFillChar(hwnd->iRow1 + 3, hwnd->iCol1 + 3, iStrLen, 219);
                 sprintf(szBuffer, "%d%%", iPercent);
                 VioColorPrint(hwnd, 5, 15, hwnd->iwfcolor, hwnd->iwbcolor, szBuffer);
                 return (NULL);
                 break;

            case 2:
                 VioCloseWindow(hwnd);
                 return (NULL);
                 break;
            }
     return (NULL);
     }

/*-----------------------------------------------------------------------------
  debugLog: Skickar information till debuglog-filen.
-----------------------------------------------------------------------------*/
void vio_debugLog(char *szFormat, ...)
     {
     FILE               *hLogFile;
     time_t             ltime;
     struct tm        *curtime;

     char              szData[2048];
     va_list            argptr;


     va_start(argptr, szFormat);
     vsprintf(szData, szFormat, argptr);
     va_end(argptr);

     time(&ltime);
     curtime = localtime(&ltime);

     hLogFile = fopen("./st_vio.log", "a");
     fprintf(hLogFile, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d\t%s\n",
             1900 + curtime->tm_year, curtime->tm_mon + 1, curtime->tm_mday,
             curtime->tm_hour, curtime->tm_min + 1, curtime->tm_sec, szData);
     fclose(hLogFile);
     }
