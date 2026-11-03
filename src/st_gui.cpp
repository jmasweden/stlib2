#define INCL_DOS
#define INCL_WIN
#define INCL_GPI
#define INCL_DEV
#define INCL_SPL
#define INCL_SPLDOSPRINT

#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "st_gui.h"

PFNWP DefaultButtonBarProc;

/*******************************************************************************/
STHOVERBOX      hb;

BOOL            bIsReg = 0;


/*-------------------------------------------------------------------------------------------------
   SetWindowTitle: Sets title of windowtitlebar.
-------------------------------------------------------------------------------------------------*/
void SetWindowTitle (HWND hwnd, PSZ szNewTitle)
     {
     HWND       hwndParent, hwndTitle;

     hwndParent = WinQueryWindow(hwnd, QW_PARENT);
     hwndTitle = WinWindowFromID(hwndParent, FID_TITLEBAR);
     WinSetWindowText(hwndTitle, (PSZ)szNewTitle);
     }

/*-------------------------------------------------------------------------------------------------
   EnableWindowUpdate: Sets window 'state'.
-------------------------------------------------------------------------------------------------*/
void EnableWindowUpdate (HWND hwnd, int iFlag)
     {
     if (iFlag == FALSE)
        {
        WinSetPointer(hwnd, WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, (ULONG) NULL));
        WinEnableWindowUpdate(hwnd, FALSE);
        }
     else
        {
        WinSetPointer(hwnd, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, (ULONG) NULL));
        WinEnableWindowUpdate(hwnd, TRUE);
        }
     }


/*-----------------------------------------------------------------------------
   StartProgram: Starts another program with parameter.
-----------------------------------------------------------------------------*/
BOOL StartProgram(HWND hwndNotify, char *szTitle, char *szProg, char *szParam, char *szStartDir)
     {
     PROGDETAILS        Details;
     HAPP               happ;


     Details.Length          = sizeof(PROGDETAILS);
     Details.progt.progc     = PROG_DEFAULT;
     Details.progt.fbVisible = SHE_VISIBLE;
     Details.pszTitle        = szTitle;
     Details.pszExecutable   = szProg;
     Details.pszParameters   = szParam;
     Details.pszStartupDir   = szStartDir;
     Details.pszIcon         = NULL;
     Details.pszEnvironment  = NULL;
     Details.swpInitial.fl   = SWP_ACTIVATE;
     Details.swpInitial.cy   = 0;
     Details.swpInitial.cx   = 0;
     Details.swpInitial.y    = 0;
     Details.swpInitial.x    = 0;
     Details.swpInitial.hwndInsertBehind = HWND_TOP;
     Details.swpInitial.hwnd             = HWND_DESKTOP;
     Details.swpInitial.ulReserved1      = 0;
     Details.swpInitial.ulReserved2      = 0;

     happ = WinStartApp(hwndNotify, &Details, szParam, NULL, 0);

     if (happ == NULLHANDLE)
        return(FALSE);

     return(TRUE);
     }

/*************************************************************************************
  WinPrint: Prints string at logical char coordinates.
*************************************************************************************/
void WinPrint(HWND hwnd, HPS hps, USHORT usRow, USHORT usCol, PSZ szMessage)
     {
     FONTMETRICS        fm;
     ULONG              yLen,xLen;
     RECTL              rcl;
     POINTL             ppointl;

     GpiQueryFontMetrics(hps, sizeof(fm), &fm);
     yLen = fm.lMaxBaselineExt;
     xLen = fm.lEmInc;

     WinQueryWindowRect(hwnd, &rcl);

     ppointl.y = rcl.yTop - ((usRow) * yLen);
     ppointl.x = (usCol) * xLen;

     GpiMove(hps, &ppointl);
     GpiCharString(hps, strlen(szMessage), szMessage);
     }


/*************************************************************************************
   WinCenterWindow: Centers a window on the screen.
*************************************************************************************/
void WinCenterWindow (HWND hwnd)
     {
     RECTL rclScreen,rclWnd;
     LONG  sWidth,sHeight,sBLCx,sBLCy;

     WinQueryWindowRect(HWND_DESKTOP, &rclScreen);
     WinQueryWindowRect(hwnd, &rclWnd);

     sWidth = (LONG) (rclWnd.xRight - rclWnd.xLeft);
     sHeight = (LONG) (rclWnd.yTop - rclWnd.yBottom);
     sBLCx = ((LONG) rclScreen.xRight - sWidth) / 2;
     sBLCy = ((LONG) rclScreen.yTop - sHeight) / 2;

     WinSetWindowPos(hwnd, HWND_TOP, sBLCx, sBLCy, 0, 0, SWP_MOVE);
     }


/*************************************************************************************
   WinSetDlgLen: Sets length of Dialogitem input.
*************************************************************************************/
void WinSetDlgLen(HWND hwnd, int iItem, int iLen)
     {
     WinSendDlgItemMsg(hwnd, iItem, EM_SETTEXTLIMIT, MPFROM2SHORT(iLen, 0), NULL);
     }


/*************************************************************************************
   WinSelDlgEntry: Selects text of Dialogitem input.
*************************************************************************************/
void WinSelDlgEntry(HWND hwnd, int iItem)
     {
     WinSendDlgItemMsg(hwnd, iItem, EM_SETSEL, MPFROM2SHORT(0, 1024), NULL);
     }


/*************************************************************************************
   WinAddToTaskList: Adds window to the tasklist.
*************************************************************************************/
LHANDLE WinAddToTaskList(HWND hwndFrame, char *szTitle)
     {
     SWCNTRL    swctl;
     PID        pid;


     WinQueryWindowProcess(hwndFrame, &pid, NULL);

     swctl.hwnd = hwndFrame;
     swctl.hwndIcon = NULLHANDLE;
     swctl.hprog = NULLHANDLE;
     swctl.idProcess = pid;
     swctl.idSession = 0;
     swctl.uchVisibility = SWL_VISIBLE;
     swctl.fbJump = SWL_JUMPABLE;
     strcpy(swctl.szSwtitle, szTitle);
     swctl.bProgType = PROG_DEFAULT;

     return (WinAddSwitchEntry(&swctl));
     }


/*************************************************************************************
   WinRemoveFromTaskList: Removes window from the tasklist.
*************************************************************************************/
void WinRemoveFromTaskList(HWND hwndFrame)
     {
     HSWITCH    hswitch;


     hswitch = WinQuerySwitchHandle(hwndFrame, 0);
     WinRemoveSwitchEntry(hswitch);
     }

/******************************************************************************
   WinEmphaseDrag: Emphases drop window in a drag/drop operation.
******************************************************************************/
void WinEmphaseDrag (HWND hwnd, BOOL bFlag)
     {
     HPS                hps;
     POINTL             ptl;
     RECTL              rcl;
     long               lColor;
     static BOOL        bDraw = TRUE;



     if (bFlag == TRUE && bDraw == FALSE)
        return;

     WinQueryWindowRect(hwnd, &rcl);

     hps = DrgGetPS(hwnd);


     if (bFlag == TRUE)
        bDraw = FALSE;
     else
        bDraw = TRUE;

     ptl.x = ptl.y = 5;
     GpiMove(hps, &ptl);

     ptl.x = rcl.xRight - rcl.xLeft - 5;
     ptl.y = rcl.yTop - rcl.yBottom - 5;

     GpiSetColor(hps, CLR_RED);
     GpiSetMix(hps, FM_XOR);
     GpiSetBackMix(hps, BM_LEAVEALONE);

     GpiBox(hps, DRO_OUTLINE, &ptl, 0L, 0L);
     DrgReleasePS(hps);
     }


/*************************************************************************************
   WinMaximizeWindow: Maximizes a window.
*************************************************************************************/
void WinMaximizeWindow (HWND hwnd)
     {
     WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MAXIMIZE | SWP_SHOW);
     }


/*************************************************************************************
   WinMinimizeWindow: Minimizes a window.
*************************************************************************************/
void WinMinimizeWindow (HWND hwnd)
     {
     WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MINIMIZE);
     }


/*************************************************************************************
   WinRestoreWindow: Restores a window.
*************************************************************************************/
void WinRestoreWindow (HWND hwnd)
     {
     WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_RESTORE | SWP_SHOW);
     }

/*------------------------------------------------------------------------------------
  QueryFontSize: Returns font size of current font.
------------------------------------------------------------------------------------*/
void QueryFontSize(HWND hwnd, PULONG pulFontWidth, PULONG pulFontHeight)
     {
     FONTMETRICS                fm;
     HPS                        hps;

     hps = WinGetPS(hwnd);
     GpiQueryFontMetrics(hps, sizeof(fm), &fm);
     *pulFontWidth = fm.lEmInc;
     *pulFontHeight = fm.lMaxBaselineExt;
     WinReleasePS(hps);
     }


/*-----------------------------------------------------------------------------
   SelectFont: Selects or deselects a font in a ps.
-----------------------------------------------------------------------------*/
ULONG SelectFont(BOOL bFlag, HPS hps, char *szFontName, int iFontSize)
     {
     ULONG              lcidFont;
     FATTRS             fat;

     if (bFlag == TRUE)
        {
        lcidFont = 1L;
        memset(&fat, 0, sizeof(fat));
        fat.usRecordLength = sizeof(fat);

        strcpy(fat.szFacename, szFontName);
        ScaleFont (hps, iFontSize);
        GpiCreateLogFont(hps, 0L, lcidFont, &fat);
        GpiSetCharSet(hps, lcidFont);
        return (lcidFont);
        }
     else
        {
        GpiSetCharSet(hps, 0L);
        GpiDeleteSetId(hps, iFontSize);
        return (TRUE);
        }
     }


/*---------------------------------------------------------------------------
  ScaleFont: Sets font size to selected size in points.
---------------------------------------------------------------------------*/
BOOL ScaleFont (HPS hpsPrinter, USHORT usFontSize)
     {
     HDC                hdc;
     LONG               xRes, yRes;
     POINTL             aptl[2];
     SIZEF              sizef;


     hdc = GpiQueryDevice(hpsPrinter);

     /****************************************************************************
       F”r att skala en font s† exakt som m”jligt anv„nder jag f”ljande formel:
           ”nskad_points * pixels_meter * meters_tum * antaltum_punkt
       samt hanterar avrundningsfel med hj„lp av * och /.
       D† GpiSetCharBox kr„ver fixed-v„rden g”rs slutligen en s†dan konvertering.
                                                                    (MA, 941127)
     ****************************************************************************/

     DevQueryCaps(hdc, CAPS_HORIZONTAL_RESOLUTION, 1, &xRes);
     DevQueryCaps(hdc, CAPS_VERTICAL_RESOLUTION,   1, &yRes);

     sizef.cx = MAKEFIXED( (((usFontSize * xRes * 254) / 10000) * 1384 ) / 100000, 0 );
     sizef.cy = MAKEFIXED( (((usFontSize * yRes * 254) / 10000) * 1384 ) / 100000, 0 );

     return GpiSetCharBox(hpsPrinter, &sizef);
     }

/****************************************************************************
  MenuHwnd: Gets menu hwnd.
****************************************************************************/
HWND MenuHwnd (HWND hwnd)
     {
     return (WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT), FID_MENU));
     }


/****************************************************************************
  MenuEnableItem: Enable or disable menuitem.
****************************************************************************/
void MenuEnableItem (HWND hwndMenu, USHORT usMenuId, BOOL bEnabled)
     {
     USHORT             usFlag;

     if (!bEnabled)
        usFlag = MIA_DISABLED;
     else
        usFlag = TRUE;

     WinSendMsg(hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(usMenuId, TRUE), MPFROM2SHORT(MIA_DISABLED, usFlag));
     }


/****************************************************************************
  MenuCheckItem: Check or uncheck menuitem.
****************************************************************************/
void MenuCheckItem (HWND hwndMenu, USHORT usMenuId, BOOL bCheck)
     {
     USHORT             usFlag;

     if (bCheck)
        usFlag = MIA_CHECKED;
     else
        usFlag = TRUE;
     WinSendMsg(hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(usMenuId, TRUE), MPFROM2SHORT(MIA_CHECKED, usFlag));
     }


/****************************************************************************
  MenuSetItemText: Set text of menuitem.
****************************************************************************/
void MenuSetItemText (HWND hwndMenu, USHORT usMenuId, PSZ szItemText)
     {
     WinSendMsg(hwndMenu, MM_SETITEMTEXT, MPFROMSHORT(usMenuId), MPFROMLONG(szItemText));
     }


/****************************************************************************
  MenuGetItemText: Get text of menuitem.
****************************************************************************/
PSZ MenuGetItemText (HWND hwndMenu, USHORT usMenuId)
    {
    ULONG               ulMenuTextLen;
    PSZ                 szMenuText;


    ulMenuTextLen = (ULONG) WinSendMsg(hwndMenu, MM_QUERYITEMTEXTLENGTH, MPFROMSHORT(usMenuId), NULL);
    ulMenuTextLen++;
    szMenuText = (PSZ) malloc(ulMenuTextLen);
    WinSendMsg(hwndMenu, MM_QUERYITEMTEXT, MPFROM2SHORT(usMenuId, 0), MPFROMLONG(szMenuText));
    return (szMenuText);
    }



/****************************************************************************
  MenuInsertItem: Insert/appends a menuitem.
****************************************************************************/
void MenuInsertItem(HWND hwndMenu, USHORT usTopMenuId, USHORT usPosition, USHORT usNewMenuId, PSZ szItemText)
     {
     MENUITEM   mi;
     HWND       hwndSubMenu, hwndPullDown;

     WinSendMsg(hwndMenu, MM_QUERYITEM, MPFROM2SHORT(usTopMenuId, TRUE), (MPARAM) &mi);
     hwndPullDown = mi.hwndSubMenu;

     mi.iPosition = usPosition;
     mi.afStyle = MIS_TEXT;
     mi.afAttribute = 0;
     mi.id = usNewMenuId;
     mi.hwndSubMenu = (HWND) NULL;
     mi.hItem = 0;

     WinSendMsg(hwndPullDown, MM_INSERTITEM, (MPARAM) &mi, (MPARAM) szItemText);
     }


/****************************************************************************
  MenuRemoveItem: Removes a menuitem.
****************************************************************************/
void MenuRemoveItem(HWND hwndMenu, USHORT usMenuId)
     {
     WinSendMsg(hwndMenu, MM_DELETEITEM, MPFROM2SHORT(usMenuId, TRUE), NULL);
     }


/****************************************************************************
  ShowPopupMenu: Shows a popup menu.
****************************************************************************/
BOOL ShowPopupMenu (HWND hwndClient, ULONG ulMenuId)
     {
     HWND               hwndMenu;
     POINTL             ptl;
     ULONG              flOptions;

     WinQueryPointerPos(HWND_DESKTOP, &ptl);
     WinMapWindowPoints(HWND_DESKTOP, hwndClient, &ptl, 1);
     hwndMenu = WinLoadMenu(hwndClient, NULLHANDLE, ulMenuId);
     flOptions = PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_HCONSTRAIN | PU_VCONSTRAIN;
     return (WinPopupMenu(hwndClient, hwndClient, hwndMenu, ptl.x, ptl.y, 0, (USHORT) flOptions));
     }

/****************************************************************************
  IconSize: Get x and y size of icons.                               930107
****************************************************************************/
void IconSize (PULONG pxIcon, PULONG pyIcon)
     {
     *pxIcon  = WinQuerySysValue(HWND_DESKTOP, SV_CXICON);
     *pyIcon  = WinQuerySysValue(HWND_DESKTOP, SV_CYICON);
     }

/****************************************************************************
  PointerNormal: Set pointer to standard arrow.                      930107
****************************************************************************/
void PointerNormal()
     {
     WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, (ULONG) NULL));
     }

/****************************************************************************
  PointerWait: Set pointer to stopwatch.                             930107
****************************************************************************/
void PointerWait()
     {
     WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, (ULONG) NULL));
     }

/*-----------------------------------------------------------------------------------
  LoadBitmapFromFile: Copys bitmap from file to hbm.
-----------------------------------------------------------------------------------*/
HBITMAP LoadBitmapFromFile(HWND hwnd, char *szFileName)
     {
     APIRET              rc;                     /* API return code */
     BOOL                fRet = FALSE;           /* Function return code. */
     HFILE               hfile;
     ULONG               ulAction;
     FILESTATUS          fsts;
     PBITMAPFILEHEADER2  pbfh2;                  /* can address any file types */
     PBITMAPINFOHEADER2  pbmp2;                  /* address any info headers */
     PBYTE               pFileBegin = NULL;      /* beginning of bitmap file data */
     ULONG               cbRead;                 /* Number of bytes read by DosRead. */
     ULONG               cScans, cScansRet;      /* number of scan lines in bitmap (cy) */

     HAB                 hab;
     HDC                 hdc;
     HPS                 hps;
     SIZEL               sizel;
     CHAR                szToken[]="*";

     HBITMAP             hbm;
     BOOL                bIgnore = FALSE;


     hab = WinQueryAnchorBlock(hwnd);

     /**********************************************************************
       Use Loop to avoid duplicate cleanup code.  If any errors, a break
       statement will jump directly to error cleanup code.
      **********************************************************************/
     do
        {
        /* Read the bitmap from file
        ****************************/
        rc = DosOpen(szFileName, &hfile, &ulAction, 0, FILE_NORMAL, FILE_OPEN, OPEN_ACCESS_READONLY | OPEN_SHARE_DENYREADWRITE, NULL);
        if (rc)
           break;

        rc = DosQueryFileInfo(hfile, 1, &fsts, sizeof(fsts));
        if (rc)
           break;

        rc = DosAllocMem((PPVOID)&pFileBegin, (ULONG)fsts.cbFile, (ULONG) PAG_READ | PAG_WRITE | PAG_COMMIT);
        if (rc)
           break;

        if (DosRead( hfile, (PVOID)pFileBegin, fsts.cbFile, &cbRead))
           break;

        /**********************************************************************
          If it's a bitmap-array, point to common file header.  Otherwise,
          point to beginning of file.
        ***********************************************************************/
        pbfh2 = (PBITMAPFILEHEADER2) pFileBegin;
        pbmp2 = NULL;

        switch (pbfh2->usType)
               {
               case BFT_BITMAPARRAY:
                    /************************************************************
                      If this is a Bitmap-Array, adjust pointer to the normal
                      file header.  We'll just use the first bitmap in the
                      array and ignore other device forms.
                    *************************************************************/
                    pbfh2 = &(((PBITMAPARRAYFILEHEADER2) pFileBegin)->bfh2);
                    pbmp2 = &pbfh2->bmp2;    /* pointer to info header (readability) */
                    break;

               case BFT_BMAP:
                    pbmp2 = &pbfh2->bmp2;    /* pointer to info header (readability) */
                    break;

               default:      /* these formats aren't supported; don't set any ptrs */
                    case BFT_ICON:
                    case BFT_POINTER:
                    case BFT_COLORICON:
                    case BFT_COLORPOINTER:
                    break;
               }

        if (pbmp2 == NULL)
           break;        /* File format NOT SUPPORTED: break out to error code */

        /************************************************************************
          Check to see if BMP file has an old structure, a new structure, or
          Windows structure.  Capture the common data and treat all bitmaps
          generically with pointer to new format.  API's will determine format
          using cbFixed field.

          Windows bitmaps have the new format, but with less data fields
          than PM.  The old strucuture has some different size fields,
          though the field names are the same.


          NOTE: bitmap data is located by offsetting the beginning of the file
                by the offset contained in pbfh2->offBits.  This value is in
                the same relatie location for different format bitmap files.
         ***********************************************************************/

        if (pbmp2->cbFix == sizeof(BITMAPINFOHEADER))           /* old format? */
           cScans = (ULONG) ((PBITMAPINFOHEADER)pbmp2)->cy;
        else                               /* new PM format, Windows, or other */
           cScans = pbmp2->cy;

        /* Create memory device context and make a hps.
         ***********************************************/
        sizel.cx = 0;
        sizel.cy = 0;
        hdc = DevOpenDC(hab, OD_MEMORY, szToken, 0L, 0L, 0L);
        hps = GpiCreatePS(hab, hdc, &sizel, PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);

        hbm = GpiCreateBitmap(hps, pbmp2, 0L, NULL, NULL);

        if (!hbm)
           break;

        if (GpiSetBitmap(hps, hbm) == (HBITMAP)BMB_ERROR)
           break;

        /*********************************************************************
          Tell GPI to put the bits into the thread's PS. The function returns
          the number of scan lines of the bitmap that were copied.  We want
          all of them at once.
         *********************************************************************/
        cScansRet = GpiSetBitmapBits(hps, 0L, cScans, pFileBegin + pbfh2->offBits, (PBITMAPINFO2) pbmp2);

        if (cScansRet != cScans)
           break;

        GpiDestroyPS(hps);
        DevCloseDC(hdc);
        DosFreeMem(pFileBegin);
        DosClose(hfile);
        return (hbm);
        }
     while (bIgnore == FALSE);

     /* Error return)
     *********************/
     if (pFileBegin != NULL)
        DosFreeMem(pFileBegin);
     if (hps != (HPS) NULL)
        GpiDestroyPS(hps);
     if (hdc != (HDC) NULL)
        DevCloseDC(hdc);
     DosClose( hfile);
     return ((HBITMAP) NULL);
     }


/*************************************************************************************
  CreateButtonBar: Creates buttonbar & loads icons.
*************************************************************************************/
HWND CreateButtonBar(HWND hwndOwner, PSTBTNBAR pBtnBar, int iBarType, USHORT usItems, int yIco, int xIco)
     {
     static BOOL        bIsClassRegistered = 0;
     PSZ                pszClass = "STButtonBarClass";
     int                iCounter;
     ULONG              flFlags;


     if (!bIsClassRegistered)
        {
        WinRegisterClass(WinQueryAnchorBlock(hwndOwner), pszClass,
                        (PFNWP)STButtonBarWndProc, CS_SIZEREDRAW | CS_SYNCPAINT, 4);
        bIsClassRegistered = TRUE;
        }

     flFlags = FCF_BORDER | FCF_NOBYTEALIGN | WS_SYNCPAINT;

     pBtnBar->hwndButtonBar = WinCreateWindow(hwndOwner, pszClass, (PSZ)NULL, flFlags, 0, 0, 0, 0,
                                              hwndOwner, HWND_TOP, 0, NULL, NULL);

     WinSetWindowPtr(pBtnBar->hwndButtonBar, 0, pBtnBar);

     pBtnBar->hwndOwner = hwndOwner;
     pBtnBar->usItemCount = usItems;
     pBtnBar->ulBarType = iBarType;
     pBtnBar->bShow = TRUE;
     if (yIco == 0)
        pBtnBar->yIco = YICO;
     else
        pBtnBar->yIco = yIco;

     if (xIco == 0)
        pBtnBar->xIco = XICO;
     else
        pBtnBar->xIco = xIco;

     pBtnBar->hwndBtnCurrent = NULLHANDLE;

     pBtnBar->pBtnItems = (PSTBTNBARITEM) calloc(usItems, sizeof(STBTNBARITEM));

     for (iCounter = 0; iCounter < pBtnBar->usItemCount; iCounter++)
         {
         pBtnBar->pBtnItems[iCounter].pstOwner = pBtnBar;
         pBtnBar->pBtnItems[iCounter].iItemId = 0;
         pBtnBar->pBtnItems[iCounter].iItemType = BBT_SPACE;
         pBtnBar->pBtnItems[iCounter].hwndButton = NULLHANDLE;
         pBtnBar->pBtnItems[iCounter].hMod = NULLHANDLE;
         pBtnBar->pBtnItems[iCounter].ulResId1 = 0;
         pBtnBar->pBtnItems[iCounter].ulResId2 = 0;
         pBtnBar->pBtnItems[iCounter].hResHandle1 = 0;
         pBtnBar->pBtnItems[iCounter].hResHandle2 = 0;
         if (pBtnBar->ulBarType == BBP_LEFT)
            pBtnBar->pBtnItems[iCounter].sizeIco = YICO;
         else
            pBtnBar->pBtnItems[iCounter].sizeIco = XICO;
         pBtnBar->pBtnItems[iCounter].usState = BBS_ENABLED;
         sprintf(pBtnBar->pBtnItems[iCounter].szFlyoverText1, "");
         sprintf(pBtnBar->pBtnItems[iCounter].szFlyoverText2, "");
         }

     CreateHoverBox(HWND_DESKTOP, &hb);
     SetHoverBoxFont(&hb, ST_FONT_HOVER);
     SetHoverBoxColor(&hb, ST_COLOR_HOVERFOREGROUND, ST_COLOR_HOVERBACKGROUND);
     SetHoverBoxAlignment(&hb, HOVER_CENTER);
     SetHoverBoxMotion(&hb, FALSE);

     WinStartTimer(WinQueryAnchorBlock(pBtnBar->hwndOwner), pBtnBar->hwndButtonBar, IDT_BBTIMER, BB_TIMEOUT);

     return(pBtnBar->hwndButtonBar);
     }


/*************************************************************************************
  DestroyButtonBar: Kills buttonbar & unloads icons.
*************************************************************************************/
BOOL DestroyButtonBar(PSTBTNBAR pBtnBar)
     {
     int        iCounter;


     for (iCounter = 0; iCounter < pBtnBar->usItemCount; iCounter++)
         {
         switch (pBtnBar->pBtnItems[iCounter].iItemType)
                {
                case BBT_BITMAP:
                     GpiDeleteBitmap(pBtnBar->pBtnItems[iCounter].hResHandle1);
                     GpiDeleteBitmap(pBtnBar->pBtnItems[iCounter].hResHandle2);
                     break;
                }
         }

     free(pBtnBar->pBtnItems);

     DestroyHoverBox(&hb);

     WinStopTimer(WinQueryAnchorBlock(pBtnBar->hwndOwner), pBtnBar->hwndButtonBar, IDT_BBTIMER);

     return(WinDestroyWindow(pBtnBar->hwndButtonBar));
     }


/*************************************************************************************
  SetButtonBarItem: Sets contence of item.
*************************************************************************************/
BOOL SetButtonBarItem(PSTBTNBAR pBtnBar, int iItem, int iItemId, int iItemType, HMODULE hMod, ULONG ulResId1, ULONG ulResId2, ULONG ulSizeIco, USHORT usState, char *szFlyoverText1, char *szFlyoverText2)
     {
     HPS        hps;


     if (iItem + 1 > pBtnBar->usItemCount)
        return (FALSE);

     pBtnBar->pBtnItems[iItem].iItemId = iItemId;
     pBtnBar->pBtnItems[iItem].iItemType = iItemType;
     pBtnBar->pBtnItems[iItem].hMod = hMod;
     pBtnBar->pBtnItems[iItem].ulResId1 = ulResId1;
     pBtnBar->pBtnItems[iItem].ulResId2 = ulResId2;
     if (ulSizeIco != 0)
        pBtnBar->pBtnItems[iItem].sizeIco = ulSizeIco;
     pBtnBar->pBtnItems[iItem].usState = usState;

     hps = WinGetPS(pBtnBar->hwndOwner);
     switch (pBtnBar->pBtnItems[iItem].iItemType)
            {
            case BBT_BITMAP:
                 pBtnBar->pBtnItems[iItem].hResHandle1 = GpiLoadBitmap(hps, (HMODULE) pBtnBar->pBtnItems[iItem].hMod,
                                                                      pBtnBar->pBtnItems[iItem].ulResId1, 0, 0);
                 pBtnBar->pBtnItems[iItem].hResHandle2 = GpiLoadBitmap(hps, (HMODULE) pBtnBar->pBtnItems[iItem].hMod,
                                                                      pBtnBar->pBtnItems[iItem].ulResId2, 0, 0);
                 break;

            case BBT_SPACE:
                 pBtnBar->pBtnItems[iItem].hResHandle1 = NULLHANDLE;
                 pBtnBar->pBtnItems[iItem].hResHandle2 = NULLHANDLE;
                 break;
            }
     WinReleasePS(hps);

     if (strlen(szFlyoverText1) < 1)
        pBtnBar->pBtnItems[iItem].szFlyoverText1[0] = '\0';
     else
        sprintf(pBtnBar->pBtnItems[iItem].szFlyoverText1, szFlyoverText1);

     if (strlen(szFlyoverText2) < 1)
        pBtnBar->pBtnItems[iItem].szFlyoverText2[0] = '\0';
     else
        sprintf(pBtnBar->pBtnItems[iItem].szFlyoverText2, szFlyoverText2);

     return(TRUE);
     }


/*************************************************************************************
  DrawButtonBar: Positions/sizes all defined items.
*************************************************************************************/
void DrawButtonBar(PSTBTNBAR pBtnBar, int ySize)
     {
     HWND       hwndButton;
     int        iCounter;
     int        iPos;
     RECTL      rcl;

     if (ySize == 0 && pBtnBar->ulBarType == BBP_LEFT)
        {
        WinQueryWindowRect(pBtnBar->hwndOwner, &rcl);
        ySize = rcl.yTop;
        }

     switch (pBtnBar->ulBarType)
            {
            case BBP_LEFT:
                 iPos = ySize - pBtnBar->yIco - STBORDERSPACE;
                 for (iCounter = 0; iCounter < pBtnBar->usItemCount; iCounter++)
                     {
                     switch (pBtnBar->pBtnItems[iCounter].iItemType)
                            {
                            case BBT_BITMAP:
                                 pBtnBar->pBtnItems[iCounter].hwndButton = WinCreateWindow(pBtnBar->hwndButtonBar, WC_BUTTON, (PSZ) NULL,
                                                              WS_VISIBLE | BS_PUSHBUTTON | BS_USERBUTTON | BS_NOPOINTERFOCUS,
                                                              STBORDERSPACE, iPos, pBtnBar->xIco, pBtnBar->yIco,
                                                              pBtnBar->hwndButtonBar, HWND_BOTTOM,
                                                              pBtnBar->pBtnItems[iCounter].iItemId, 0, 0);
                                 WinSetWindowPtr(pBtnBar->pBtnItems[iCounter].hwndButton, 0, &pBtnBar->pBtnItems[iCounter]);
                                 DefaultButtonBarProc = WinSubclassWindow(pBtnBar->pBtnItems[iCounter].hwndButton, (PFNWP) STButtonBarSubClassButtonWndProc);
                                 iPos = iPos - pBtnBar->yIco;
                                 break;

                            case BBT_SPACE:
                                 iPos = iPos - (pBtnBar->yIco / 4);
                                 break;
                            }
                     }

            case BBP_TOP:
                 iPos = STBORDERSPACE;
                 for (iCounter = 0; iCounter < pBtnBar->usItemCount; iCounter++)
                     {
                     switch (pBtnBar->pBtnItems[iCounter].iItemType)
                            {
                            case BBT_BITMAP:
                                 pBtnBar->pBtnItems[iCounter].hwndButton = WinCreateWindow(pBtnBar->hwndButtonBar, WC_BUTTON, (PSZ) NULL,
                                                              WS_VISIBLE | BS_PUSHBUTTON | BS_USERBUTTON | BS_NOPOINTERFOCUS,
                                                              iPos, STBORDERSPACE, pBtnBar->xIco, pBtnBar->yIco,
                                                              pBtnBar->hwndButtonBar, HWND_BOTTOM,
                                                              pBtnBar->pBtnItems[iCounter].iItemId, 0, 0);
                                 WinSetWindowPtr(pBtnBar->pBtnItems[iCounter].hwndButton, 0, &pBtnBar->pBtnItems[iCounter]);
                                 DefaultButtonBarProc = WinSubclassWindow(pBtnBar->pBtnItems[iCounter].hwndButton, (PFNWP) STButtonBarSubClassButtonWndProc);
                                 iPos = iPos + pBtnBar->xIco;
                                 break;

                            case BBT_SPACE:
                                 iPos = iPos + (pBtnBar->xIco / 4);
                                 break;
                            }
                     }
            }
     }


/*************************************************************************************
  HiliteButton: Sets state of button.
************************************************************************************/
BOOL HiliteButton(PSTBTNBAR pBtnBar, int iItem, BOOL bHilite)
     {
     HWND       hwndButton;
     int        iOldState, iPos;


     iPos = ButtonBarPosFromItem(pBtnBar, iItem);
     iOldState = pBtnBar->pBtnItems[iPos].usState;

     if (bHilite == TRUE)
         pBtnBar->pBtnItems[iPos].usState = BBS_HIGHLITED;
     else
         pBtnBar->pBtnItems[iPos].usState = BBS_ENABLED;

     WinInvalidateRect(WinWindowFromID(pBtnBar->hwndButtonBar, pBtnBar->pBtnItems[iPos].iItemId),
                       NULL, FALSE);

     return (iOldState);
     }


/*************************************************************************************
  EnableButton: Sets state of button.
************************************************************************************/
BOOL EnableButton(PSTBTNBAR pBtnBar, int iItem, BOOL bEnable)
     {
     HWND       hwndButton;
     int        iOldState, iPos;


     if ( (iPos = ButtonBarPosFromItem(pBtnBar, iItem)) == -1)
        return (FALSE);

     iOldState = pBtnBar->pBtnItems[iPos].usState;

     if (bEnable == TRUE)
         pBtnBar->pBtnItems[iPos].usState = BBS_ENABLED;
     else
         pBtnBar->pBtnItems[iPos].usState = BBS_DISABLED;

     WinInvalidateRect(WinWindowFromID(pBtnBar->hwndButtonBar, pBtnBar->pBtnItems[iPos].iItemId),
                       NULL, FALSE);

     return (iOldState);
     }


/*************************************************************************************
  IsButtonHilited: Querys state of button.
************************************************************************************/
BOOL IsButtonHilited(PSTBTNBAR pBtnBar, int iItem)
     {
     HWND       hwndButton, iPos;


     iPos = ButtonBarPosFromItem(pBtnBar, iItem);

     if (pBtnBar->pBtnItems[iPos].usState == BBS_HIGHLITED)
        return(TRUE);
     else
        return(FALSE);
     }


/*************************************************************************************
  IsButtonEnabled: Querys state of button.
************************************************************************************/
BOOL IsButtonEnabled(PSTBTNBAR pBtnBar, int iItem)
     {
     HWND       hwndButton, iPos;


     iPos = ButtonBarPosFromItem(pBtnBar, iItem);

     if (pBtnBar->pBtnItems[iPos].usState == BBS_DISABLED)
        return(FALSE);
     else
        return(TRUE);
     }


/*************************************************************************************
  STButtonBarWndProc: Handle Button Paint/presses.
*************************************************************************************/
MRESULT EXPENTRY STButtonBarWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
        {
        RECTL              rcl;
        PUSERBUTTON        ubInfo;
        HPS                hps;
        ULONG              y, ulFlag;
        int                iItemId, iPos;
        int                iCounter;
        POINTL             pp;
        PSTBTNBAR          pBtnBar;
        HBITMAP            hbmUse;


        switch (msg)
               {

               case WM_PAINT:
                    hps = WinBeginPaint(hwnd, 0, 0);
                    WinQueryWindowRect(hwnd, &rcl);
                    WinFillRect(hps, &rcl, ST_COLOR_BACKGROUND);

                    GpiSetColor(hps, ST_COLOR_BORDERLIGHT);

                    pp.x = STBORDERWIDTH;
                    pp.y = 0;
                    GpiMove(hps, &pp);
                    pp.x = STBORDERWIDTH;
                    pp.y = rcl.yTop;
                    GpiLine(hps, &pp);

                    pp.x = 0;
                    pp.y = rcl.yTop - STBORDERWIDTH;
                    GpiMove(hps, &pp);
                    pp.x = rcl.xRight;
                    pp.y = rcl.yTop - STBORDERWIDTH;
                    GpiLine(hps, &pp);

                    GpiSetColor(hps, ST_COLOR_BORDERDARK);

                    pp.x = 0;
                    pp.y = 0;
                    GpiMove(hps, &pp);
                    pp.x = rcl.xRight;
                    pp.y = 0;
                    GpiLine(hps, &pp);

                    pp.x = rcl.xRight - STBORDERWIDTH;
                    pp.y = 0;
                    GpiMove(hps, &pp);
                    pp.x = rcl.xRight - STBORDERWIDTH;
                    pp.y = rcl.yTop;
                    GpiLine(hps, &pp);

                    WinEndPaint(hps);
                    break;

               case WM_CONTROL:
                    pBtnBar = (PSTBTNBAR) WinQueryWindowPtr(hwnd, 0);
                    switch (SHORT2FROMMP(mp1))
                           {
                           case BN_CLICKED:
                                iPos = ButtonBarPosFromItem(pBtnBar, SHORT1FROMMP(mp1));
                                if (ButtonBarIsMouseOnHwnd(pBtnBar->pBtnItems[iPos].hwndButton))
                                   {
                                   WinPostMsg(pBtnBar->hwndOwner, WM_BUTTONBARSELECT,
                                              (PVOID) SHORT1FROMMP(mp1), NULL);
                                   }
                                break;

                           case BN_PAINT:
                                iPos = ButtonBarPosFromItem(pBtnBar, SHORT1FROMMP(mp1));
                                y = LONGFROMMP(mp2);
                                ubInfo = (PUSERBUTTON) y;

                                switch (ubInfo->fsState)
                                       {
                                       /* Draw normal, select bitmap (normal/selected)
                                        ***********************************************/
                                       case BDS_DEFAULT:
                                            switch (pBtnBar->pBtnItems[iPos].usState)
                                                   {
                                                   case BBS_ENABLED:
                                                        hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                        ulFlag = DBM_NORMAL;
                                                        break;

                                                   case BBS_HIGHLITED:
                                                        if (pBtnBar->pBtnItems[iPos].hResHandle2 != NULLHANDLE)
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle2;
                                                           ulFlag = DBM_NORMAL;
                                                           }
                                                        else
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                           ulFlag = DBM_INVERT;
                                                           }
                                                        break;

                                                   case BBS_DISABLED:
                                                        hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                        ulFlag = DBM_HALFTONE;
                                                        break;
                                                   }
                                            pp.x = 0;
                                            pp.y = 0;
                                            WinDrawBitmap(ubInfo->hps, hbmUse, 0, &pp, CLR_NEUTRAL, CLR_BACKGROUND, ulFlag);
                                            ButtonBarFrameButton(&pBtnBar->pBtnItems[iPos], ubInfo->hps, ulFlag);
                                            return ((MRESULT) TRUE);
                                            break;


                                       /* Draw highlighted (user clicked) , select bitmap (normal/selected)
                                        *********************************************************************/
                                       case BDS_HILITED:
                                            switch (pBtnBar->pBtnItems[iPos].usState)
                                                   {
                                                   case BBS_ENABLED:
                                                        if (pBtnBar->pBtnItems[iPos].hResHandle2 != NULLHANDLE)
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle2;
                                                           ulFlag = DBM_NORMAL;
                                                           }
                                                        else
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                           ulFlag = DBM_INVERT;
                                                           }
                                                        break;

                                                   case BBS_HIGHLITED:
                                                        if (pBtnBar->pBtnItems[iPos].hResHandle2 != NULLHANDLE)
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                           ulFlag = DBM_NORMAL;
                                                           }
                                                        else
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                           ulFlag = DBM_NORMAL;
                                                           }
                                                        break;

                                                   case BBS_DISABLED:
                                                        hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                        ulFlag = DBM_HALFTONE;
                                                        break;
                                                   }
                                            pp.x = 0;
                                            pp.y = 0;
                                            WinDrawBitmap(ubInfo->hps, hbmUse, 0, &pp, CLR_NEUTRAL, CLR_BACKGROUND, ulFlag);
                                            ButtonBarFrameButton(&pBtnBar->pBtnItems[iPos], ubInfo->hps, ulFlag);
                                            return ((MRESULT) TRUE);
                                            break;


                                       /* Draw disabled, allways use the first bitmap */
                                       case BDS_DISABLED:
                                            hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                            pp.x = 0;
                                            pp.y = 0;
                                            WinDrawBitmap(ubInfo->hps, hbmUse, 0, &pp, CLR_NEUTRAL, CLR_BACKGROUND, DBM_HALFTONE);
                                            ButtonBarFrameButton(&pBtnBar->pBtnItems[iPos], ubInfo->hps, DBM_HALFTONE);
                                            return ((MRESULT) TRUE);
                                            break;


                                       default:
                                            switch (pBtnBar->pBtnItems[iPos].usState)
                                                   {
                                                   case BBS_ENABLED:
                                                        hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                        ulFlag = DBM_NORMAL;
                                                        break;

                                                   case BBS_HIGHLITED:
                                                        if (pBtnBar->pBtnItems[iPos].hResHandle2 != NULLHANDLE)
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle2;
                                                           ulFlag = DBM_NORMAL;
                                                           }
                                                        else
                                                           {
                                                           hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                           ulFlag = DBM_INVERT;
                                                           }
                                                        break;

                                                   case BBS_DISABLED:
                                                        hbmUse = pBtnBar->pBtnItems[iPos].hResHandle1;
                                                        ulFlag = DBM_HALFTONE;
                                                        break;
                                                   }
                                            pp.x = 0;
                                            pp.y = 0;
                                            WinDrawBitmap(ubInfo->hps, hbmUse, 0, &pp, CLR_NEUTRAL, CLR_BACKGROUND, ulFlag);
                                            ButtonBarFrameButton(&pBtnBar->pBtnItems[iPos], ubInfo->hps, ulFlag);
                                            return ((MRESULT) TRUE);
                                            break;
                                       }

                                break;
                           }

               case WM_TIMER:
                    pBtnBar = (PSTBTNBAR) WinQueryWindowPtr(hwnd, 0);

                    if (pBtnBar->hwndBtnCurrent != NULLHANDLE)
                       {
                       if (SHORT1FROMMP(mp1) == IDT_BBTIMER && ButtonBarIsMouseOnHwnd(pBtnBar->hwndBtnCurrent) == FALSE)
                          {
                          WinInvalidateRect(pBtnBar->hwndBtnCurrent, NULL, FALSE);
                          pBtnBar->hwndBtnCurrent = NULLHANDLE;
                          }
                       }
                    break;

               default:
                    return (WinDefWindowProc(hwnd, msg, mp1, mp2));
               }
        return (FALSE);
        }


/*************************************************************************************
  SizeButtonBar: Sizes the button bar.
************************************************************************************/
ULONG SizeButtonBar(PSTBTNBAR pBtnBar, ULONG ulBtnLen)
      {
      RECTL     rclOwner;
      int       xpos, xsize, ypos, ysize;


      if (pBtnBar->hwndButtonBar == NULLHANDLE)
         return (0);

      if (pBtnBar->bShow == FALSE)
         return (0);

      WinQueryWindowRect(pBtnBar->hwndOwner, &rclOwner);
      switch (pBtnBar->ulBarType)
             {
             case BBP_LEFT:
                  xpos = rclOwner.xLeft;
                  xsize = pBtnBar->xIco + (STBORDERSPACE * 2);
                  ysize = ulBtnLen;
                  if (ysize == 0)
                     ysize = rclOwner.yTop;
                  ypos = rclOwner.yBottom;
                  WinSetWindowPos(pBtnBar->hwndButtonBar, HWND_TOP,
                                  xpos, ypos, xsize, ysize,
                                  SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
                  return (xsize);
                  break;

             case BBP_TOP:
                  ypos = rclOwner.yTop - pBtnBar->yIco - (STBORDERSPACE * 2);
                  ysize = pBtnBar->yIco + (STBORDERSPACE * 2);
                  xsize = ulBtnLen;
                  if (xsize == 0)
                     xsize = rclOwner.xRight;
                  xpos = 0;
                  WinSetWindowPos(pBtnBar->hwndButtonBar, HWND_TOP,
                                  xpos, ypos, xsize, ysize,
                                  SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
                  return (ysize);
                  break;
             }
      return (0);
      }

/*************************************************************************************
  ShowButtonBar: Shows (or hides) the button bar.
************************************************************************************/
BOOL ShowButtonBar(PSTBTNBAR pBtnBar, BOOL bShow)
      {

      if (pBtnBar->hwndButtonBar == NULLHANDLE)
         return (FALSE);

      if (pBtnBar->bShow == bShow)
         return (pBtnBar->bShow);

      if (bShow == TRUE)
         WinSetWindowPos(pBtnBar->hwndButtonBar, 0, 0, 0, 0, 0, SWP_SHOW);
      else
         WinSetWindowPos(pBtnBar->hwndButtonBar, 0, 0, 0, 0, 0, SWP_HIDE);

      pBtnBar->bShow = bShow;

      return (!bShow);
      }


/*************************************************************************************
  ButtonBarFrameButton: Draw frame around button if mouse is on top.
************************************************************************************/
void ButtonBarFrameButton(PSTBTNBARITEM pBtnItem, HPS hps, ULONG ulHowDraw)
    {
    PSTBTNBAR   pBtnBar;
    POINTL      ptl;



    pBtnBar = (PSTBTNBAR) pBtnItem->pstOwner;
    if (pBtnItem->hwndButton == pBtnBar->hwndBtnCurrent && ButtonBarIsMouseOnHwnd(pBtnBar->hwndBtnCurrent) == TRUE)
       {
       ptl.x = 0;
       ptl.y = 0;
       GpiSetCurrentPosition(hps, &ptl);
       switch(ulHowDraw)
             {
             case DBM_NORMAL:
                  GpiSetColor(hps, ST_COLOR_BORDERBLACK);
                  ptl.x = pBtnBar->xIco - 1;
                  GpiLine(hps, &ptl);
                  ptl.y = pBtnBar->yIco - 1;
                  GpiLine(hps, &ptl);
                  ptl.x = 0;
                  GpiLine(hps, &ptl);
                  ptl.y = 0;
                  GpiLine(hps, &ptl);
                  break;

             case DBM_INVERT:
                  GpiSetColor(hps, ST_COLOR_BORDERBLACK);
                  ptl.x = pBtnBar->xIco - 1;
                  GpiLine(hps, &ptl);
                  ptl.y = pBtnBar->yIco - 1;
                  GpiLine(hps, &ptl);
                  ptl.x = 0;
                  GpiLine(hps, &ptl);
                  ptl.y = 0;
                  GpiLine(hps, &ptl);
                  break;

             case DBM_HALFTONE:
                  break;
             }
       }
    }


/*************************************************************************************
  ButtonBarIsMouseOnHwnd: Is mouse on top of window.
************************************************************************************/
BOOL ButtonBarIsMouseOnHwnd(HWND hwndHit)
     {
     RECTL              rcl;
     int                iHit;
     POINTL             ptl;


     WinQueryPointerPos(HWND_DESKTOP, &ptl);
     WinMapWindowPoints(HWND_DESKTOP, hwndHit, &ptl, 1);
     WinQueryWindowRect(hwndHit, &rcl);
     if (WinPtInRect(WinQueryAnchorBlock(hwndHit), &rcl, &ptl))
        return (TRUE);
     else
        return(FALSE);
     }



/*************************************************************************************
  ButtonBarPosFromItem: Get button bar pos (in array).
************************************************************************************/
int ButtonBarPosFromItem(PSTBTNBAR pBtnBar, int iItemId)
      {
      int       iCounter;


      for (iCounter = 0; iCounter < pBtnBar->usItemCount; iCounter++)
          {
          if (pBtnBar->pBtnItems[iCounter].iItemId == iItemId)
             {
             return(iCounter);
             break;
             }
          }
      return(-1);
      }


/*-----------------------------------------------------------------------------
   STButtonBarSubClassButtonWndProc: Subclassed button WndProc.
-----------------------------------------------------------------------------*/
MRESULT EXPENTRY STButtonBarSubClassButtonWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     PSTBTNBARITEM      pBtnBarItem;
     PSTBTNBAR          pBtnBar;
     char               szUseText[64];


     switch (msg)
            {
            case WM_MOUSEMOVE:
                 pBtnBarItem = (PSTBTNBARITEM) WinQueryWindowPtr(hwnd, 0);
                 pBtnBar = (PSTBTNBAR) pBtnBarItem->pstOwner;

                 if (pBtnBarItem->hwndButton != pBtnBar->hwndBtnCurrent)
                    {
                    WinInvalidateRect(pBtnBar->hwndBtnCurrent, NULL, FALSE);
                    pBtnBar->hwndBtnCurrent = pBtnBarItem->hwndButton;
                    WinInvalidateRect(pBtnBarItem->hwndButton, NULL, FALSE);
                    }

                 switch (pBtnBarItem->usState)
                        {
                        case BBS_HIGHLITED:
                             strcpy(szUseText, pBtnBarItem->szFlyoverText2);
                             break;

                        default:
                             strcpy(szUseText, pBtnBarItem->szFlyoverText1);
                             break;
                        }

                 if (strlen(szUseText) > 0)
                    {
                    SetHoverBoxText(&hb, szUseText);
                    ShowHoverBox(&hb, hwnd, pBtnBarItem->iItemId, TRUE);
                    }
                 return WinDefWindowProc (hwnd, msg, mp1, mp2);

            default:
                 return ((MRESULT) DefaultButtonBarProc(hwnd, msg, mp1, mp2));
            }
     return (FALSE);
     }

/*-----------------------------------------------------------------------------------
  ClipQueryText: Is there CF_TEXT in the clipboard.
-----------------------------------------------------------------------------------*/
BOOL ClipQueryText(HWND hwnd)
     {
     BOOL       bIsText;


     if (WinOpenClipbrd (WinQueryAnchorBlock(hwnd)))
        {
        bIsText = WinQueryClipbrdFmtInfo(WinQueryAnchorBlock(hwnd), CF_TEXT, 0L);
        WinCloseClipbrd (WinQueryAnchorBlock(hwnd));
        return (bIsText);
        }
      else
        return(FALSE);
     }


/*-----------------------------------------------------------------------------------
  ClipToText: Copy Text from clipboard.
-----------------------------------------------------------------------------------*/
char* ClipToText(HWND hwnd)
     {
     static char        szClipBoardText[1024];
     PSZ                pszClipText;


     if (WinOpenClipbrd (WinQueryAnchorBlock(hwnd)))
        {
        if ( (pszClipText = (PSZ) WinQueryClipbrdData(WinQueryAnchorBlock(hwnd), CF_TEXT))  != NULL )
           {
           strncpy(szClipBoardText, pszClipText, 1023);
           WinCloseClipbrd (WinQueryAnchorBlock(hwnd));
           return(szClipBoardText);
           }
        WinCloseClipbrd (WinQueryAnchorBlock(hwnd));
        }
     return((char*) NULL);
     }


/*-----------------------------------------------------------------------------------
  TextToClip: Copy text to clipboard.
-----------------------------------------------------------------------------------*/
void TextToClip(HWND hwnd, PSZ szText)
     {
     PBYTE      pb;


     DosAllocSharedMem((PVOID *) &pb, (PSZ) NULL, strlen(szText) + 1, fALLOC | OBJ_GIVEABLE);
     strcpy((char*) pb, szText);

     if (WinOpenClipbrd (WinQueryAnchorBlock(hwnd)) != FALSE)
        {
        WinEmptyClipbrd(WinQueryAnchorBlock(hwnd));
        WinSetClipbrdData (WinQueryAnchorBlock(hwnd), (int) pb, CF_TEXT, CFI_POINTER);
        WinCloseClipbrd (WinQueryAnchorBlock(hwnd));
        }
      }

/*
*************************************************************************************
  ClipQueryBitmap: Is there CF_BITMAP in the clipboard.
*************************************************************************************
BOOL ClipQueryBitmap(HAB hab)
     {
     BOOL       bRetVal;

     if(WinOpenClipBrd(hab))
       {
       bRetVal = WinQueryClipbrdFmtInfo(hab, CF_BITMAP, (ULONG) NULL);
       WinCloseClipbrd (hab);
       return(bRetVal);
       }
     else
       WinCloseClipbrd(hab);

     return(TRUE);
     }


*************************************************************************************
  BitmapToClip: Copy bitmap to clipboard.
*************************************************************************************
BOOL BitmapToClip(HAB hab, HBITMAP hbm)
     {
     BOOL           bRetVal;
     HBITMAP        hbmClip;

     if (WinOpenClipBrd(hab))
        {
        WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, (ULONG) NULL));
        hbmClip = CopyOfBitmap(hab, hbm);
        WinEmptyClipBrd(hab);
        bRetVal = WinSetClipbrdData(hab, hbmClip, CF_BITMAP, CFI_HANDLE);
        WinCloseClipbrd(hab);
        WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, (ULONG) NULL));
        return(bRetVal);
        }
     else
        return(FALSE);
     }

*************************************************************************************
  ClipToBitmap: Copy bitmap from clipboard.
*************************************************************************************
HBITMAP ClipToBitmap(HAB hab)
     {
     BOOL               bRetVal;
     HBITMAP            hbmClip, hbmTemp;

     if (WinOpenClipBrd(hab))
        {
        hbmClip = WinQueryClipBrdData(hab, CF_BITMAP);
        if (hbmclip)
           {
           WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, (ULONG) NULL));
           hbmTemp = CopyOfBitmap(hab, hbmClip);
           WinSetPointer(HWND_DESKTOP,WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, (ULONG) NULL));
           WinCloseClipbrd(hab);
           return(hbmTemp);
           }
         else
           {
           WinCloseClipbrd(hab);
           return((HBITMAP) NULL);
           }
         }
      else
         return((HBITMAP) NULL);
     }


*************************************************************************************
  CopyOfBitmap: Make a copy of a bitmap.
*************************************************************************************
HBITMAP CopyOfBitmap(HAB hab, HBITMAP hbmSource)
     {
     BITMAPINFOHEADER           bih;
     POINTL                     aptl[2];
     HPS                        hpsSource, hpsTarget;
     HDC                        hdcSource, hdcTarget;
     HBITMAP                    hbmTarget;

     BitmapPSandDC(hab, hpsSource, hdcSource);
     BitmapPSandDC(hab, hpsTarget, hdcTarget);

     GpiQueryBitmapParameters(hbmSource, &bih);
     hbmTarget = GpiCreateBitmap(hpsTarget, &bih, 0L, 0L, 0L);
     GpiSetBitmap(hpsSource, hbmSource);
     GpiSetBitmap(hpsTarget, hbmTarget);

     aptl[0].x = 0L;
     aptl[0].y = 0L;
     aptl[1].x = bih.cx;
     aptl[1].y = bih.cy;
     aptl[2].x = 0L;
     aptl[2].y = 0L;

     GpiBitBlt(hpsTarget, hpsSource, 3, &aptl[0], ROP_SRCCOPY, BBO_AND);
     GpiDestroyPS(hpsSource);
     GpiDestroyPS(hpsTarget);
     DevCloseDC(hdcSource);
     DevCloseDC(hdcTarget);
     return(hbmTarget);
     }


*************************************************************************************
  BitmapPSandDC: Create PS and DC for Bitmap copying.
*************************************************************************************
void BitmapPSandDC(HAB hab, HPS hps, HDC hdc)
     {
     SIZEL              sizel;
     CHAR               szToken[]="*";

     sizl.cx = 0;
     sizl.cy = 0;
     hdc = DevOpenDC(hab, OD_MEMORY,szToken, 0L, 0L, 0L);
     hps = GpiCreatePS(hab, hdc, &sizl, PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);
     }

*/


/*************************************************************************************
   CreateContainer:
*************************************************************************************/
HWND CreateContainer(HWND hwndOwner, PSTCONTAINER pCnr, int iIdCnr, int iExtra, int iItems, int iCreateFlags)
     {
     static BOOL        bIsReg = 0;
     char               szClass[] = "STContainerClass";
     HWND               hwndContainer;
     CNRINFO            cnrInfo;


     /* Set default values */
     pCnr->hwndOwner = hwndOwner;
     pCnr->iIdCnr = iIdCnr;
     pCnr->iItems = iItems;
     pCnr->iExtra = iExtra;
     pCnr->iCount = 0;

     if (!bIsReg)
        {
        WinRegisterClass(WinQueryAnchorBlock(pCnr->hwndOwner), (PCSZ)szClass, (PFNWP)ContainerWndProc,
                         CS_SYNCPAINT | CS_SIZEREDRAW, 4);
        bIsReg = TRUE;
        }

     pCnr->hwndClient = WinCreateWindow(pCnr->hwndOwner, (PCSZ)szClass, (PSZ)NULL, WS_SYNCPAINT, FCF_NOBYTEALIGN,
                                           0, 0, 0, pCnr->hwndOwner, HWND_TOP, 0, NULL, NULL);

     WinSetWindowPtr(pCnr->hwndClient, 0, pCnr);

     pCnr->hwndCnr = WinCreateWindow(pCnr->hwndClient, WC_CONTAINER, NULL, iCreateFlags | WS_SYNCPAINT,
                                     0, 0, 0, 0, pCnr->hwndClient, HWND_BOTTOM, iIdCnr, NULL, NULL);

     pCnr->pList = (PVOID) WinSendMsg(pCnr->hwndCnr, CM_ALLOCRECORD, MPFROMLONG(iExtra), MPFROMLONG(pCnr->iItems));

     return (hwndContainer);
     }


/*************************************************************************************
  ContainerWndProc: Handle Container messages.
*************************************************************************************/
MRESULT EXPENTRY ContainerWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     HPS                        hps;
     RECTL                      rcl;
     PSTCONTAINER               pCnr;
     PNOTIFYRECORDENTER         pNre;
     PNOTIFYRECORDEMPHASIS      pNreEmph;
     PCNREDITDATA               pCnrEd;
     PRECORDCORE                prc;
     PCNRDRAGINIT               pcdi;
     int                        iItem;


     switch (msg)
            {
            case WM_PAINT:
                 hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
                 WinQueryWindowRect(hwnd, &rcl);
                 WinDrawBorder(hps, &rcl, STBORDERWIDTH, STBORDERWIDTH, CLR_DARKGRAY, CLR_DARKGRAY, DB_PATCOPY);
                 rcl.xLeft = rcl.xLeft + STBORDERWIDTH;
                 rcl.yTop = rcl.yTop - STBORDERWIDTH;
                 WinDrawBorder(hps, &rcl, STBORDERWIDTH, STBORDERWIDTH, CLR_WHITE, CLR_WHITE, DB_PATCOPY);
                 WinEndPaint(hps);
                 break;

            case WM_SIZE:
                 pCnr = (PSTCONTAINER) WinQueryWindowPtr(hwnd, 0);
                 if (pCnr != NULL)
                    {
                    WinQueryWindowRect(hwnd, &rcl);
                    WinSetWindowPos(pCnr->hwndCnr, HWND_BOTTOM, rcl.xLeft + STBORDERWIDTH, rcl.yBottom + STBORDERWIDTH,
                                    (rcl.xRight - rcl.xLeft) - (STBORDERWIDTH * 2), (rcl.yTop - rcl.yBottom) - (STBORDERWIDTH * 2),
                                    SWP_SIZE | SWP_MOVE | SWP_SHOW);
                    }
                 break;

            case WM_CONTROL:
                 pCnr = (PSTCONTAINER) WinQueryWindowPtr(hwnd, 0);
                 if (SHORT1FROMMP(mp1) == pCnr->iIdCnr)
                    {
                    switch (SHORT2FROMMP(mp1))
                           {
                           case CN_EMPHASIS:
                                pNreEmph = (PNOTIFYRECORDEMPHASIS) mp2;
                                WinPostMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)pNreEmph->pRecord);
                                break;

                           case CN_ENTER:
                                pNre = (PNOTIFYRECORDENTER) mp2;
                                WinPostMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)pNre->pRecord);
                                break;

                           case CN_REALLOCPSZ:
                                return ((MRESULT)TRUE);
                                break;

                           case CN_ENDEDIT:
                                pCnrEd = (PCNREDITDATA) mp2;
                                WinPostMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)pCnrEd->pRecord);
                                break;

                           case CN_CONTEXTMENU:
                                prc = (PRECORDCORE) mp2;
                                WinPostMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)prc);
                                break;

                           case CN_INITDRAG:
                                pcdi = (PCNRDRAGINIT) mp2;
                                WinPostMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)pcdi->pRecord);
                                break;

                           case CN_DRAGOVER:
                                return(WinSendMsg(pCnr->hwndOwner, WM_CONTAINERMSG, (PVOID)mp1, (MPARAM)mp2));
                                break;
                           }
                    }
                 break;

            default:
                 return (WinDefWindowProc (hwnd, msg, mp1, mp2));
            }
     return (FALSE);
     }


/*************************************************************************************
   DestroyContainer:
*************************************************************************************/
void DestroyContainer(PSTCONTAINER pCnr)
     {
     WinDestroyWindow (pCnr->hwndCnr);
     WinDestroyWindow (pCnr->hwndClient);

     if (pCnr->hbmFolder != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmFolder);

     if (pCnr->hbmFile != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmFile);

     if (pCnr->hbmXtra1 != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmXtra1);

     if (pCnr->hbmXtra2 != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmXtra2);

     if (pCnr->hbmXtra3 != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmXtra3);

     if (pCnr->hbmXtra4 != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmXtra4);

     if (pCnr->hbmPlus != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmPlus);

     if (pCnr->hbmMinus != NULLHANDLE)
        GpiDeleteBitmap(pCnr->hbmMinus);

     WinSendMsg(pCnr->hwndCnr, CM_FREERECORD, MPFROMP(pCnr->pList), MPFROMLONG(pCnr->iItems));
     }


/*************************************************************************************
   ContainerExpandTree:
*************************************************************************************/
BOOL ContainerExpandTree(PSTCONTAINER pCnr, PVOID pItem)
     {
     return ( (BOOL) WinSendMsg(pCnr->hwndCnr, CM_EXPANDTREE, MPFROMP(pItem), NULL));
     }


/*************************************************************************************
   QueryContainerSelected:
*************************************************************************************/
PVOID QueryContainerSelected(PSTCONTAINER pCnr)
     {
     PRECORDCORE        prc;


     prc = (PRECORDCORE) WinSendMsg(pCnr->hwndCnr, CM_QUERYRECORDEMPHASIS, (PRECORDCORE)CMA_FIRST, MPFROMSHORT(CRA_SELECTED));

     return ((PVOID) prc);
     }


/*************************************************************************************
   ContainerSelect:
*************************************************************************************/
BOOL ContainerSelect(PSTCONTAINER pCnr, PVOID pItem)
     {
     return ( (BOOL) WinSendMsg(pCnr->hwndCnr, CM_SETRECORDEMPHASIS, MPFROMP(pItem), MPFROM2SHORT(TRUE, CRA_SELECTED)));
     }


/*************************************************************************************
   QueryContainerFolder:
*************************************************************************************/
BOOL QueryContainerFolder(PVOID prc)
     {

     /* Folders always have hbmMiniBitmap equal to 0.
      ************************************************/
     if ( ((PRECORDCORE)prc)->hbmMiniBitmap == 0 )
        return (TRUE);
     else
        return (FALSE);
     }


/*************************************************************************************
   ContainerQueryRecord:
*************************************************************************************/
PVOID ContainerQueryRecord(PSTCONTAINER pCnr, int iCommand, PVOID prcItem, PVOID prcParent)
     {
     PVOID              prc;

     prc = (PVOID) WinSendMsg(pCnr->hwndCnr, CM_QUERYRECORD, prcItem, MPFROM2SHORT(iCommand, CMA_ITEMORDER));

     if (prc == NULL || prc == (PVOID) -1)
        return ((PVOID) NULL);
     else
        {
        prcParent = (PVOID) WinSendMsg(pCnr->hwndCnr, CM_QUERYRECORD, prc, MPFROM2SHORT(CMA_PARENT, CMA_ITEMORDER));
        if (prc == NULL || prc == (PVOID) -1)
           return ((PVOID) NULL);
        }

     return ((PVOID) prc);
     }


/*************************************************************************************
   InsertContainerFolder:
*************************************************************************************/
PVOID InsertContainerFolder(PSTCONTAINER pCnr, PVOID prcAfter, PVOID prcAskedParent)
     {
     RECORDINSERT       recsIn;
     PRECORDCORE        pIn;
     PRECORDCORE        prcParent;
     int                iPosition;
     BOOL               bSkip;


     if (pCnr->iCount > pCnr->iItems)
        return (NULL);

     prcParent = (PRECORDCORE) prcAskedParent;

     recsIn.cb = sizeof(RECORDINSERT);

     /* Does the caller ask for CMA_FIRST or CMA_END ?? */
     if (prcAfter == (PVOID) CMA_FIRST || prcAfter == (PVOID) CMA_END)
        {
        /* Place where asked for (depends on whats in prcParent */
        recsIn.pRecordOrder = (PRECORDCORE)prcAfter;
        recsIn.pRecordParent = (PRECORDCORE)prcParent;
        }
     else
        {
        bSkip = FALSE;
        if ((PRECORDCORE)prcParent == NULL)
           {
           /* Parent = Selected */
           prcParent = (PRECORDCORE) WinSendMsg(pCnr->hwndCnr, CM_QUERYRECORDEMPHASIS, (PRECORDCORE)CMA_FIRST, MPFROMSHORT(CRA_SELECTED));
           if (prcParent == NULL)
              {
              if (prcAfter == NULL)
                 prcAfter = (PVOID) CMA_FIRST;
              recsIn.pRecordOrder = (PRECORDCORE)prcAfter;
              recsIn.pRecordParent = (PRECORDCORE)NULL;
              bSkip = TRUE;
              }
           }

        if (bSkip == FALSE)
           {
           /* Check for parent type */
           if ( ((PRECORDCORE)prcParent)->hbmMiniBitmap == 0 )
              {
              /* Parent is a folder */
              recsIn.pRecordOrder = (PRECORDCORE)CMA_END;
              recsIn.pRecordParent = (PRECORDCORE)prcParent;
              }
           else
              {
              /* Parent is a file, put after "parent" */
              recsIn.pRecordOrder = (PRECORDCORE)prcAfter;
              recsIn.pRecordParent = (PRECORDCORE)NULL;
              }
           }
        }

     recsIn.zOrder = (USHORT)CMA_TOP;
     recsIn.fInvalidateRecord = TRUE;
     recsIn.cRecordsInsert = 1;

     if (pCnr->iCount == 0)
        pIn = (PRECORDCORE) pCnr->pList;
     else
        pIn = (PRECORDCORE) pCnr->prcNext;

     pIn->cb = sizeof(RECORDCORE);
     pIn->hbmMiniBitmap = 0;

     WinSendMsg(pCnr->hwndCnr, CM_INSERTRECORD, pIn, &recsIn);

     pCnr->prcNext = pIn->preccNextRecord;
     pCnr->iCount++;
     return (pIn);
     }


/*************************************************************************************
   InsertContainerFile:
*************************************************************************************/
PVOID InsertContainerFile(PSTCONTAINER pCnr, PVOID prcAfter, PVOID prcAskedParent)
     {
     RECORDINSERT       recsIn;
     PRECORDCORE        pIn;
     PRECORDCORE        prcParent;
     int                iPosition;
     BOOL               bSkip;


     prcParent = (PRECORDCORE) prcAskedParent;

     if (pCnr->iCount > pCnr->iItems)
        return (NULL);

     recsIn.cb = sizeof(RECORDINSERT);


     /* Does the caller ask for CMA_FIRST or CMA_END ?? */
     if (prcAfter == (PVOID) CMA_FIRST || prcAfter == (PVOID) CMA_END)
        {
        /* Place where asked for (depends on whats in prcParent */
        recsIn.pRecordOrder = (PRECORDCORE)prcAfter;
        recsIn.pRecordParent = (PRECORDCORE)prcParent;
        }
     else
        {
        bSkip = FALSE;
        if ((PRECORDCORE)prcParent == NULL)
           {
           /* Parent = Selected */
           prcParent = (PRECORDCORE) WinSendMsg(pCnr->hwndCnr, CM_QUERYRECORDEMPHASIS, (PRECORDCORE)CMA_FIRST, MPFROMSHORT(CRA_SELECTED));
           if (prcParent == NULL)
              return(NULL);
           }

        /* Check for parent type */
        if ( ((PRECORDCORE)prcParent)->hbmMiniBitmap == 0 )
           {
           /* Parent is a folder */
           recsIn.pRecordOrder = (PRECORDCORE)CMA_END;
           recsIn.pRecordParent = (PRECORDCORE)prcParent;
           }
        else
           {
           /* Parent is a file, put after "previous" */
           recsIn.pRecordOrder = (PRECORDCORE)prcAfter;
           recsIn.pRecordParent = (PRECORDCORE)NULL;
           }
        }

     recsIn.zOrder = (USHORT)CMA_TOP;
     recsIn.fInvalidateRecord = TRUE;
     recsIn.cRecordsInsert = 1;

     if (pCnr->iCount == 0)
        pIn = (PRECORDCORE) pCnr->pList;
     else
        pIn = (PRECORDCORE) pCnr->prcNext;

     pIn->cb = sizeof(RECORDCORE);
     pIn->hbmMiniBitmap = 1;

     WinSendMsg(pCnr->hwndCnr, CM_INSERTRECORD, pIn, &recsIn);

     pCnr->prcNext = pIn->preccNextRecord;
     pCnr->iCount++;
     return (pIn);
     }


/*************************************************************************************
   DeleteContainerItem:
*************************************************************************************/
BOOL DeleteContainerItem(PSTCONTAINER pCnr, PVOID pItem)
     {

     if (pCnr->iCount <= 0)
        return (FALSE);

     WinSendMsg(pCnr->hwndCnr, CM_REMOVERECORD, &pItem, MPFROM2SHORT(1, CMA_INVALIDATE));

     WinSendMsg(pCnr->hwndCnr, CM_INVALIDATERECORD, MPFROMP(pItem),MPFROM2SHORT(0, CMA_REPOSITION));

     pCnr->iCount--;
     return (TRUE);
     }


/*************************************************************************************
   SetContainerItem:
*************************************************************************************/
BOOL SetContainerItem(PSTCONTAINER pCnr, PVOID pItem, ULONG flRecordAttr, HMODULE hMod, HBITMAP hbmShow, char *szItemText)
     {

     if (pItem == NULL)
        return (FALSE);

     /* Check to see if hbm is valid, if not
        set folder or file.
      ***************************************/
     switch (hbmShow)
            {
            case IDSTCNR_HBMFOLDER:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmFolder;
                 break;

            case IDSTCNR_HBMFILE:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmFile;
                 break;

            case IDSTCNR_HBMOWNER1:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmXtra1;
                 break;

            case IDSTCNR_HBMOWNER2:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmXtra2;
                 break;

            case IDSTCNR_HBMOWNER3:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmXtra3;
                 break;

            case IDSTCNR_HBMOWNER4:
                 ((PRECORDCORE)pItem)->hbmBitmap = pCnr->hbmXtra4;
                 break;

            default:
                 ((PRECORDCORE)pItem)->hbmBitmap = hbmShow;
                 break;
            }

     ((PRECORDCORE)pItem)->pszIcon = (PSZ) szItemText;
     ((PRECORDCORE)pItem)->pszText = (PSZ) szItemText;
     ((PRECORDCORE)pItem)->pszName = (PSZ) szItemText;
     ((PRECORDCORE)pItem)->pszTree = (PSZ) szItemText;

     ((PRECORDCORE)pItem)->flRecordAttr = ((PRECORDCORE)pItem)->flRecordAttr | flRecordAttr;

     WinSendMsg(pCnr->hwndCnr, CM_INVALIDATERECORD, MPFROMP(&pItem), MPFROM2SHORT(1, CMA_REPOSITION));

     return (TRUE);
     }


/*************************************************************************************
   EmptyContainer:
*************************************************************************************/
BOOL EmptyContainer(PSTCONTAINER pCnr)
     {

     if (pCnr->iCount <= 0)
        return (FALSE);

     WinSendMsg(pCnr->hwndCnr, CM_REMOVERECORD, (MPARAM)NULL, MPFROM2SHORT(0, CMA_INVALIDATE));

     pCnr->iCount = 0;
     return (TRUE);
     }


/*************************************************************************************
   ContainerOpenEdit: Opens the text edit of the selected container item.
*************************************************************************************/
void ContainerOpenEdit(PSTCONTAINER pCnr, PVOID pItem)
     {
     CNREDITDATA        cnrEditData;
     APIRET             rc;


     WinSendMsg(pCnr->hwndCnr, CM_SETRECORDEMPHASIS, MPFROMP(pItem), MPFROM2SHORT(TRUE, CRA_SELECTED));

     cnrEditData.cb = sizeof(CNREDITDATA);
     cnrEditData.hwndCnr = pCnr->hwndCnr;
     cnrEditData.pRecord = (PRECORDCORE) pItem;
     cnrEditData.pFieldInfo = NULL;
     cnrEditData.ppszText = NULL;
     cnrEditData.cbText = 0;
     cnrEditData.id = CID_CNRTITLEWND;

     rc = (APIRET) WinSendMsg(pCnr->hwndCnr, CM_OPENEDIT, (MPARAM)&cnrEditData, NULL);
     }


/*************************************************************************************
  QueryContainerColor:
*************************************************************************************/
void QueryContainerColor(PSTCONTAINER pCnr, int *iColorFore, int *iColorBack)
     {
     char *szMsg = "QueryContainerColor / Not implemented";
     DEBUG((PSZ)szMsg);
     }


/*************************************************************************************
  SetContainerColor:
*************************************************************************************/
void SetContainerColor(PSTCONTAINER pCnr, int iColorFore, int iColorBack)
     {
     pCnr->iColorFore = iColorFore;
     pCnr->iColorBack = iColorBack;

     WinSetPresParam(pCnr->hwndCnr, PP_FOREGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColorFore);
     WinSetPresParam(pCnr->hwndCnr, PP_BACKGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColorBack);
     WinSetPresParam(pCnr->hwndClient, PP_BACKGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColorBack);
     }


/*************************************************************************************
  QueryContainerFont:
*************************************************************************************/
void QueryContainerFont(PSTCONTAINER pCnr, char* szFont)
     {
     DEBUG((PSZ)"FIX_THIS");
     }


/*************************************************************************************
  SetContainerFont:
*************************************************************************************/
void SetContainerFont(PSTCONTAINER pCnr, char* szFont)
     {
     strcpy(pCnr->szFont, szFont);
     WinSetPresParam(pCnr->hwndCnr, PP_FONTNAMESIZE, (ULONG)strlen(pCnr->szFont) + 1, (PVOID)&pCnr->szFont);
     }


/*************************************************************************************
  SetContainerBitmap:
*************************************************************************************/
void SetContainerBitmap(PSTCONTAINER pCnr, HMODULE hMod, ULONG ulFolderRes, ULONG ulFileRes, ULONG ulXtra1, ULONG ulXtra2, ULONG ulXtra3, ULONG ulXtra4)
     {
     HPS        hps;

     hps = WinGetPS(pCnr->hwndOwner);
     pCnr->hbmFolder = GpiLoadBitmap(hps, hMod, ulFolderRes, 0, 0);
     pCnr->hbmFile = GpiLoadBitmap(hps, hMod, ulFileRes, 0, 0);

     if (ulXtra1 != 0)
        pCnr->hbmXtra1 = GpiLoadBitmap(hps, hMod, ulXtra1, 0, 0);
     else
        pCnr->hbmXtra1 = (HBITMAP) NULL;

     if (ulXtra2 != 0)
        pCnr->hbmXtra2 = GpiLoadBitmap(hps, hMod, ulXtra2, 0, 0);
     else
        pCnr->hbmXtra2 = (HBITMAP) NULL;

     if (ulXtra3 != 0)
        pCnr->hbmXtra3 = GpiLoadBitmap(hps, hMod, ulXtra3, 0, 0);
     else
        pCnr->hbmXtra3 = (HBITMAP) NULL;

     if (ulXtra4 != 0)
        pCnr->hbmXtra4 = GpiLoadBitmap(hps, hMod, ulXtra4, 0, 0);
     else
        pCnr->hbmXtra4 = (HBITMAP) NULL;

     WinReleasePS(hps);
     }


/*************************************************************************************
  SetContainerBitmapSize:
*************************************************************************************/
void SetContainerBitmapSize(PSTCONTAINER pCnr, int iSizeX, int iSizeY)
     {
     CNRINFO            cnrInfo;

     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);
     cnrInfo.slBitmapOrIcon.cy = iSizeY;
     cnrInfo.slBitmapOrIcon.cx = iSizeX;
     WinSendMsg(pCnr->hwndCnr, CM_SETCNRINFO, (PCNRINFO)&cnrInfo, MPFROMLONG(CMA_SLBITMAPORICON));
     }


/*************************************************************************************
  SetContainerTreeBitmap:
*************************************************************************************/
void SetContainerTreeBitmap(PSTCONTAINER pCnr, HMODULE hMod, ULONG ulPlusRes, ULONG ulMinusRes)
     {
     CNRINFO            cnrInfo;
     HPS                hps;


     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);

     hps = WinGetPS(pCnr->hwndOwner);
     pCnr->hbmPlus = GpiLoadBitmap(hps, hMod, ulPlusRes, 0, 0);
     pCnr->hbmMinus = GpiLoadBitmap(hps, hMod, ulMinusRes, 0, 0);
     WinReleasePS(hps);

     cnrInfo.hbmExpanded = pCnr->hbmPlus;
     cnrInfo.hbmCollapsed = pCnr->hbmMinus;

     WinSendMsg(pCnr->hwndCnr, CM_SETCNRINFO, (PCNRINFO)&cnrInfo, MPFROMLONG(CMA_TREEBITMAP));
     }


/*************************************************************************************
  SetContainerTreeBitmapSize:
*************************************************************************************/
void SetContainerTreeBitmapSize(PSTCONTAINER pCnr, int iSizeX, int iSizeY)
     {
     CNRINFO            cnrInfo;

     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);
     cnrInfo.slTreeBitmapOrIcon.cy = iSizeY;
     cnrInfo.slTreeBitmapOrIcon.cx = iSizeX;
     WinSendMsg(pCnr->hwndCnr, CM_SETCNRINFO, (PCNRINFO)&cnrInfo, MPFROMLONG(CMA_SLTREEBITMAPORICON));
     }


/*************************************************************************************
  QueryContainerTreeLineLen:
*************************************************************************************/
int QueryContainerTreeLineLen(PSTCONTAINER pCnr)
     {
     CNRINFO            cnrInfo;

     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);
     return(cnrInfo.cxTreeIndent);
     }


/*************************************************************************************
  SetContainerTreeLineLen:
*************************************************************************************/
void SetContainerTreeLineLen(PSTCONTAINER pCnr, int iTreeLineLen)
     {
     CNRINFO            cnrInfo;

     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);
     cnrInfo.cxTreeIndent = iTreeLineLen;
     WinSendMsg(pCnr->hwndCnr, CM_SETCNRINFO, (PCNRINFO)&cnrInfo, MPFROMLONG(CMA_CXTREEINDENT));
     }


/*************************************************************************************
  SetContainerWindowAttr:
*************************************************************************************/
void SetContainerWindowAttr(PSTCONTAINER pCnr, ULONG flWindowAttr)
     {
     CNRINFO            cnrInfo;


     WinSendMsg(pCnr->hwndCnr, CM_QUERYCNRINFO, (PCNRINFO)&cnrInfo, NULL);
     cnrInfo.flWindowAttr = cnrInfo.flWindowAttr | flWindowAttr;
     WinSendMsg(pCnr->hwndCnr, CM_SETCNRINFO, (PCNRINFO)&cnrInfo, MPFROMLONG(CMA_FLWINDOWATTR));
     }


/*************************************************************************************
  CreateControl: Creates a control window.
*************************************************************************************/
HWND CreateControl(HWND hwndOwner, PSTCONTROL pStCtrl, int iType, int iItemId, int iCtrlId, int iCreateFlags, BOOL bUseHover, int iItemCount)
     {
     static BOOL        bIsClassRegistered = 0;
     char               szClass[] = "STControlWindowClass";
     char               szControlClass[16];

     ULONG              flFlags, ulFlags;
     BOOL               bNoExtraWindow;


     pStCtrl->hwndOwner = hwndOwner;
     pStCtrl->iItemId = iItemId;
     pStCtrl->iControlId = iCtrlId;
     pStCtrl->iItemCount = iItemCount;
     pStCtrl->iCurrentItem = 0;
     pStCtrl->bUseHover = bUseHover;
     pStCtrl->bShow = TRUE;

     pStCtrl->iType = iType;
     pStCtrl->iCreateFlags = iCreateFlags;

     /* Hover settings */
     if (pStCtrl->bUseHover == TRUE)
        {
        CreateHoverBox(HWND_DESKTOP, &pStCtrl->sthb);
        SetHoverBoxFont(&pStCtrl->sthb, "8.Helv");
        SetHoverBoxColor(&pStCtrl->sthb, CLR_BLACK, CLR_YELLOW);
        SetHoverBoxAlignment(&pStCtrl->sthb, HOVER_CENTER);
        SetHoverBoxMotion(&pStCtrl->sthb, FALSE);
        sprintf(pStCtrl->szFlyOverText1, "");
        sprintf(pStCtrl->szFlyOverText2, "");
        }

     /* Register main window class */
     if (!bIsClassRegistered)
        {
        WinRegisterClass(WinQueryAnchorBlock(hwndOwner), szClass,
                        (PFNWP)STControlWindowWndProc, CS_SIZEREDRAW | CS_SYNCPAINT, 4);
        bIsClassRegistered = TRUE;
        }

     bNoExtraWindow = TRUE;
     ulFlags = 0;
     switch (pStCtrl->iType)
            {
            case IDSTCTRL_TEXT:
                 strcpy(szControlClass, WC_STATIC);
                 ulFlags = SS_TEXT;
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_BUTTON:
                 strcpy(szControlClass, WC_BUTTON);
                 ulFlags = BS_PUSHBUTTON;
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_ENTRYFIELD:
                 strcpy(szControlClass, WC_ENTRYFIELD);
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_CHECK:
                 strcpy(szControlClass, WC_ENTRYFIELD);
                 ulFlags = BS_AUTOCHECKBOX;
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_RADIO:
                 strcpy(szControlClass, WC_ENTRYFIELD);
                 ulFlags = BS_AUTORADIOBUTTON;
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_COMBO:
                 strcpy(szControlClass, WC_COMBOBOX);
                 bNoExtraWindow = FALSE;
                 break;

            case IDSTCTRL_BITMAP:
                 break;

            case IDSTCTRL_STATE:
                 break;

            case IDSTCTRL_MANYSTATE:
                 ControlInitItems(pStCtrl);
                 break;

            case IDSTCTRL_ANIMATE:
                 ControlInitItems(pStCtrl);
                 break;

            case IDSTCTRL_PROGRESS:
                 break;

            case IDSTCTRL_DATETIME:
                 break;

            case IDSTCTRL_PRINT:
                 break;

            case IDSTCTRL_OWNERDRAW:
                 break;
            }

     /* Create main (background) window */
     flFlags = FCF_NOBYTEALIGN | WS_SYNCPAINT;
     pStCtrl->hwndControl = WinCreateWindow(pStCtrl->hwndOwner, szClass, (PSZ)NULL, flFlags, 0, 0, 0, 0, pStCtrl->hwndOwner, HWND_TOP, 0, NULL, NULL);

     if (bNoExtraWindow == FALSE)
        {
        pStCtrl->hwndIntControl = WinCreateWindow(pStCtrl->hwndControl, szControlClass, (PSZ)NULL,
                                                  ulFlags | pStCtrl->iCreateFlags, 0, 0, 0, 0,
                                                  pStCtrl->hwndControl, HWND_BOTTOM, 0, NULL, NULL);
        }

     WinSetWindowPtr(pStCtrl->hwndControl, 0, pStCtrl);
     return(pStCtrl->hwndControl);
     }


/*************************************************************************************
  ControlInitItems: allocates and initiates items array.
*************************************************************************************/
void ControlInitItems(PSTCONTROL pStCtrl)
     {
     int                iCounter;


     pStCtrl->pStCi = (PSTCONTROLITEM) calloc(pStCtrl->iItemCount, sizeof(STCONTROLITEM));

     for (iCounter = 0; iCounter < pStCtrl->iItemCount; iCounter++)
         {
         pStCtrl->pStCi[iCounter].iItemId = 0;
         pStCtrl->pStCi[iCounter].iItemType = IDSTCTRL_SUB_NONE;
         pStCtrl->pStCi[iCounter].ulResourceId = 0;
         pStCtrl->pStCi[iCounter].hResHandle = NULLHANDLE;
         sprintf(pStCtrl->pStCi[iCounter].szText, "");
         pStCtrl->pStCi[iCounter].iForeColor = CLR_BLACK;
         pStCtrl->pStCi[iCounter].iBackColor = CLR_PALEGRAY;
         sprintf(pStCtrl->pStCi[iCounter].szFlyOverText, "");
         }
     }


/*************************************************************************************
  DestroyControl: Kills of the contol.
*************************************************************************************/
BOOL DestroyControl(PSTCONTROL pStCtrl)
     {
     int        iCounter;


     /* Destroy any subitems */
     if (pStCtrl->iItemCount != 0)
        {
        for (iCounter = 0; iCounter < pStCtrl->iItemCount; iCounter++)
            {
            switch (pStCtrl->pStCi[iCounter].iItemType)
                   {
                   case IDSTCTRL_SUB_BITMAP:
                   case IDSTCTRL_SUB_HBITMAP:
                        GpiDeleteBitmap(pStCtrl->pStCi[iCounter].hResHandle);
                        break;
                   }
            }
        free(pStCtrl->pStCi);
        }

     /* Delete control specific data */
     switch (pStCtrl->iType)
            {
            case IDSTCTRL_BITMAP:
                 GpiDeleteBitmap(pStCtrl->hResHandle1);
                 break;

            case IDSTCTRL_STATE:
                 GpiDeleteBitmap(pStCtrl->hResHandle1);
                 GpiDeleteBitmap(pStCtrl->hResHandle2);
                 break;
            }

     /* Destroy windows */
     WinDestroyWindow(pStCtrl->hwndIntControl);
     return(WinDestroyWindow(pStCtrl->hwndControl));
     }


/*************************************************************************************
  ControlSetItem: Sets contence of one item.
*************************************************************************************/
BOOL ControlSetItem(PSTCONTROL pStCtrl, int iItem, int iItemId, int iItemType, HMODULE hMod, ULONG ulResId, char *szItemText, int iForeColor, int iBackColor, char *szFlyOverText)
     {
     HPS        hps;


     if (iItem + 1 > pStCtrl->iItemCount)
        return (FALSE);

     pStCtrl->pStCi[iItem].iItemId = iItemId;
     pStCtrl->pStCi[iItem].iItemType = iItemType;
     pStCtrl->pStCi[iItem].hMod = hMod;
     pStCtrl->pStCi[iItem].ulResourceId = ulResId;

     pStCtrl->pStCi[iItem].iForeColor = iForeColor;
     pStCtrl->pStCi[iItem].iBackColor = iBackColor;

     strcpy(pStCtrl->pStCi[iItem].szFlyOverText, szFlyOverText);

     hps = WinGetPS(pStCtrl->hwndOwner);
     switch (pStCtrl->pStCi[iItem].iItemType)
            {
           case IDSTCTRL_SUB_TEXT:
                strcpy(pStCtrl->pStCi[iItem].szText, szItemText);
                break;

            case IDSTCTRL_SUB_BITMAP:
                 pStCtrl->pStCi[iItem].hResHandle = GpiLoadBitmap(hps, pStCtrl->pStCi[iItem].hMod, pStCtrl->pStCi[iItem].ulResourceId, 0, 0);
                 break;

            case IDSTCTRL_SUB_HBITMAP:
                 pStCtrl->pStCi[iItem].hResHandle = pStCtrl->pStCi[iItem].ulResourceId;
                 break;

           case IDSTCTRL_SUB_COLOR:
                pStCtrl->pStCi[iItem].iBackColor = iBackColor;
                break;

           default:
                 break;
            }
     WinReleasePS(hps);

     return(TRUE);
     }


/*************************************************************************************
  ControlItemRealloc: Reallocates control items.
************************************************************************************/
void ControlItemRealloc(PSTCONTROL pStCtrl, int iItemCount)
     {
     int        iCounter;

     /* Destroy any existing subitems */
     if (pStCtrl->iItemCount != 0)
        {
        for (iCounter = 0; iCounter < pStCtrl->iItemCount; iCounter++)
            {
            switch (pStCtrl->pStCi[iCounter].iItemType)
                   {
                   case IDSTCTRL_SUB_BITMAP:
                   case IDSTCTRL_SUB_HBITMAP:
                        GpiDeleteBitmap(pStCtrl->pStCi[iCounter].hResHandle);
                        break;
                   }
            }
        free(pStCtrl->pStCi);
        }

     pStCtrl->iItemCount = iItemCount;
     ControlInitItems(pStCtrl);
     }


/*************************************************************************************
  ControlResize: Sizes the control window.
************************************************************************************/
int ControlResize(PSTCONTROL pStCtrl, int yPos, int xPos, int ySize, int xSize)
    {
    RECTL              prclClient;
    FONTMETRICS        fm;
    ULONG              yFont, xFont;
    HPS                hps;


    if (pStCtrl->bShow == FALSE || pStCtrl->hwndControl == NULLHANDLE)
       return ((int) MPFROM2SHORT(0, 0));

    switch (pStCtrl->iType)
           {
           case IDSTCTRL_TEXT:
           case IDSTCTRL_ENTRYFIELD:
           case IDSTCTRL_COMBO:
                hps = WinGetPS(pStCtrl->hwndControl);
                GpiQueryFontMetrics(hps, sizeof(fm), &fm);
                yFont = fm.lMaxBaselineExt;
                xFont = fm.lEmInc;
                WinReleasePS(hps);
                ySize = yPos + (STBORDERSPACE * 2) + yFont;
                break;

           case IDSTCTRL_DATETIME:
           case IDSTCTRL_CHECK:
           case IDSTCTRL_RADIO:
                hps = WinGetPS(pStCtrl->hwndControl);
                GpiQueryFontMetrics(hps, sizeof(fm), &fm);
                yFont = fm.lMaxBaselineExt;
                xFont = fm.lEmInc;
                WinReleasePS(hps);
                ySize = yPos + (STBORDERSPACE * 2) + yFont;
                xSize = xPos + (STBORDERSPACE * 2) + ( strlen(pStCtrl->szText1) * xFont);
                break;

           case IDSTCTRL_ANIMATE:
           case IDSTCTRL_PROGRESS:
           case IDSTCTRL_PRINT:
                break;

           }

    WinSetWindowPos(pStCtrl->hwndControl, HWND_TOP, xPos, yPos, xSize, ySize, SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ZORDER);

    return ((int) MPFROM2SHORT(ySize, xSize));
    }


/*************************************************************************************
  ControlShow: Shows (or hides) the control window.
************************************************************************************/
BOOL ControlShow(PSTCONTROL pStCtrl, BOOL bShow)
     {

     if (pStCtrl->hwndControl == NULLHANDLE)
        return (FALSE);

     if (pStCtrl->bShow == bShow)
        return (pStCtrl->bShow);

     if (bShow == TRUE)
        {
        WinSetWindowPos(pStCtrl->hwndControl, 0, 0, 0, 0, 0, SWP_SHOW);
        if (pStCtrl->hwndIntControl != NULLHANDLE)
           WinSetWindowPos(pStCtrl->hwndIntControl, 0, 0, 0, 0, 0, SWP_SHOW);
        }
     else
        {
        WinSetWindowPos(pStCtrl->hwndControl, 0, 0, 0, 0, 0, SWP_HIDE);
        if (pStCtrl->hwndIntControl != NULLHANDLE)
           WinSetWindowPos(pStCtrl->hwndIntControl, 0, 0, 0, 0, 0, SWP_HIDE);
        }

     pStCtrl->bShow = bShow;

     return (!bShow);
     }



/*************************************************************************************
  ControlSetFont: Sets font (used with text display).
************************************************************************************/
void ControlSetFont(PSTCONTROL pStCtrl, char *szFont)
     {
     strcpy(pStCtrl->szFont, szFont);
     WinSetPresParam(pStCtrl->hwndControl, PP_FONTNAMESIZE, (ULONG)strlen(pStCtrl->szFont) + 1, (PVOID)&pStCtrl->szFont);
     WinSetPresParam(pStCtrl->hwndIntControl, PP_FONTNAMESIZE, (ULONG)strlen(pStCtrl->szFont) + 1, (PVOID)&pStCtrl->szFont);
     WinInvalidateRect(pStCtrl->hwndControl, NULL, FALSE);
     }


/*************************************************************************************
  ControlSetFlyOverText: Sets to use with hover (flyover).
************************************************************************************/
void ControlSetFlyOverText(PSTCONTROL pStCtrl, char *szFlyOverText1, char *szFlyOverText2)
     {
     strcpy(pStCtrl->szFlyOverText1, szFlyOverText1);
     strcpy(pStCtrl->szFlyOverText1, szFlyOverText2);
     }


/*************************************************************************************
  ControlSetEffect: Sets effect (2d or 3d).
************************************************************************************/
void ControlSetEffect(PSTCONTROL pStCtrl, int iEffect)
     {
     pStCtrl->iEffect = iEffect;
     WinInvalidateRect(pStCtrl->hwndControl, NULL, FALSE);
     }


/*************************************************************************************
  ControlSetText: Sets text of control window.
************************************************************************************/
void ControlSetText(PSTCONTROL pStCtrl, char *szText1, char *szText2)
     {

     strcpy(pStCtrl->szText1, szText1);
     strcpy(pStCtrl->szText2, szText2);

     switch (pStCtrl->iType)
            {
            case IDSTCTRL_TEXT:
            case IDSTCTRL_BUTTON:
            case IDSTCTRL_ENTRYFIELD:
            case IDSTCTRL_CHECK:
            case IDSTCTRL_RADIO:
            case IDSTCTRL_COMBO:
                 WinSetWindowText(pStCtrl->hwndIntControl, pStCtrl->szText1);
                 break;

            case IDSTCTRL_STATE:
                 WinInvalidateRect(pStCtrl->hwndControl, NULL, FALSE);
                 break;
            }
      }


/*************************************************************************************
  ControlGetText: Gets text of control window.
************************************************************************************/
char *ControlGetText(PSTCONTROL pStCtrl)
     {
     static char        szText[128];


     switch (pStCtrl->iType)
            {
            case IDSTCTRL_TEXT:
            case IDSTCTRL_BUTTON:
            case IDSTCTRL_ENTRYFIELD:
            case IDSTCTRL_CHECK:
            case IDSTCTRL_RADIO:
            case IDSTCTRL_COMBO:
                 WinQueryWindowText(pStCtrl->hwndIntControl, 127, szText);
                 return (szText);
                 break;
            }
     return ((char *) NULL);
     }


/*************************************************************************************
  ControlClear: Clears control.
************************************************************************************/
void ControlClear(PSTCONTROL pStCtrl)
     {

     switch (pStCtrl->iType)
            {
            case IDSTCTRL_TEXT:
            case IDSTCTRL_BUTTON:
            case IDSTCTRL_ENTRYFIELD:
                 strcpy(pStCtrl->szText1, "");
                 WinSetWindowText(pStCtrl->hwndIntControl, pStCtrl->szText1);
                 break;

            case IDSTCTRL_CHECK:
                 break;

            case IDSTCTRL_RADIO:
                 break;

            case IDSTCTRL_COMBO:
                 break;
            }
     }


/*************************************************************************************
  AnimateSetTimerId: Sets timer Id.
************************************************************************************/
void AnimateSetTimerId(PSTCONTROL pStCtrl, int iTimerId)
     {
     pStCtrl->iTimerId = iTimerId;
     }


/*************************************************************************************
  AnimateSetTimerTicks: Sets animation timer.
************************************************************************************/
void AnimateSetTimerTicks(PSTCONTROL pStCtrl, int iTimer)
     {
     pStCtrl->iTimerTicks = iTimer;
     }


/*************************************************************************************
  AnimateStart: Starts animation.
************************************************************************************/
BOOL AnimateStart(PSTCONTROL pStCtrl)
     {
     pStCtrl->iCurrentItem = 0;
     WinStartTimer(WinQueryAnchorBlock(pStCtrl->hwndControl), pStCtrl->hwndControl, pStCtrl->iTimerId, pStCtrl->iTimerTicks);
     return (TRUE);
     }


/*************************************************************************************
  AnimateStop: Stops animation.
************************************************************************************/
void AnimateStop(PSTCONTROL pStCtrl)
     {
     WinStopTimer(WinQueryAnchorBlock(pStCtrl->hwndControl), pStCtrl->hwndControl, pStCtrl->iTimerId);
     pStCtrl->iCurrentItem = 0;
     ControlDrawItem(pStCtrl, NULLHANDLE, pStCtrl->iCurrentItem);
     }


/*************************************************************************************
  ControlDrawItem: Draws specified item.
************************************************************************************/
void ControlDrawItem(PSTCONTROL pStCtrl, HPS hps, int iItem)
     {
     RECTL              rcl;
     BOOL               bOwnHps;


     if (hps == NULLHANDLE)
        bOwnHps = TRUE;
     else
        bOwnHps = FALSE;

     if (bOwnHps == TRUE)
        hps = WinGetPS(pStCtrl->hwndControl);

     WinQueryWindowRect(pStCtrl->hwndControl, &rcl);

     switch (pStCtrl->iType)
            {
            case IDSTCTRL_MANYSTATE:
            case IDSTCTRL_ANIMATE:
                 switch (pStCtrl->pStCi[iItem].iItemType)
                        {
                        case IDSTCTRL_SUB_BITMAP:
                        case IDSTCTRL_SUB_HBITMAP:
                             WinDrawBitmap(hps, (HBITMAP) pStCtrl->pStCi[iItem].hResHandle, 0, (PPOINTL)&rcl, CLR_NEUTRAL, CLR_BACKGROUND, DBM_NORMAL | DBM_STRETCH);
                             break;

                        case IDSTCTRL_SUB_TEXT:
                             WinDrawText(hps, -1, pStCtrl->pStCi[iItem].szText, &rcl,
                                         pStCtrl->pStCi[iItem].iForeColor, pStCtrl->pStCi[iItem].iBackColor,
                                         DT_CENTER | DT_ERASERECT | DT_VCENTER);
                             break;

                        case IDSTCTRL_SUB_COLOR:
                             WinFillRect(hps, &rcl, pStCtrl->pStCi[iItem].iBackColor);
                             break;

                        case IDSTCTRL_SUB_OWNERDRAW:
                             WinPostMsg(pStCtrl->hwndOwner, WM_CTRLMESSAGE, MPFROMLONG(pStCtrl), MPFROMLONG(WM_PAINT));
                             break;
                        }
            }

     if (bOwnHps == TRUE)
        WinReleasePS(hps);
     }


/*************************************************************************************
  STControlWindowWndProc: Handle Control window messages.
*************************************************************************************/
MRESULT EXPENTRY STControlWindowWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
        {
        PSTCONTROL         pStCtrl;
        HPS                hps;

        switch (msg)
               {
               case WM_PAINT:
                    pStCtrl = (PSTCONTROL) WinQueryWindowPtr(hwnd, 0);

                    hps = WinBeginPaint(hwnd, 0, 0);

                    switch (pStCtrl->iType)
                           {
                           case IDSTCTRL_TEXT:
                                break;

                           case IDSTCTRL_BITMAP:
                                break;

                           case IDSTCTRL_STATE:
                                break;

                           case IDSTCTRL_MANYSTATE:
                           case IDSTCTRL_ANIMATE:
                                ControlDrawItem(pStCtrl, hps, pStCtrl->iCurrentItem);
                                break;

                           case IDSTCTRL_PROGRESS:
                                break;

                           case IDSTCTRL_DATETIME:
                                break;

                           case IDSTCTRL_PRINT:
                                break;

                           case IDSTCTRL_OWNERDRAW:
                                pStCtrl->mp1 = mp1;
                                pStCtrl->mp2 = mp2;
                                pStCtrl->hpsPaint = hps;
                                WinSendMsg(pStCtrl->hwndOwner, WM_CTRLMESSAGE, MPFROMLONG(pStCtrl->iControlId), MPFROMLONG(msg));
                                break;
                     }
                    WinEndPaint(hps);
                    break;


               case WM_BUTTON1CLICK:
               case WM_BUTTON2CLICK:
               case WM_BUTTON3CLICK:
               case WM_BUTTON1DBLCLK:
               case WM_BUTTON2DBLCLK:
               case WM_BUTTON3DBLCLK:
                    pStCtrl = (PSTCONTROL) WinQueryWindowPtr(hwnd, 0);
                    pStCtrl->mp1 = mp1;
                    pStCtrl->mp2 = mp2;
                    WinSendMsg(pStCtrl->hwndOwner, WM_CTRLMESSAGE, MPFROMLONG(pStCtrl->iControlId), MPFROMLONG(msg));
                    return (WinDefWindowProc(hwnd, msg, mp1, mp2));
                    break;


               case WM_SIZE:
                    pStCtrl = (PSTCONTROL) WinQueryWindowPtr(hwnd, 0);
                    break;


               case WM_TIMER:
                    pStCtrl = (PSTCONTROL) WinQueryWindowPtr(hwnd, 0);

                    if (LONGFROMMP(mp1) == pStCtrl->iTimerId)
                       {
                       if (pStCtrl->iCurrentItem >= pStCtrl->iItemCount)
                          pStCtrl->iCurrentItem = 0;

                       WinInvalidateRect(pStCtrl->hwndControl, NULL, FALSE);
                       pStCtrl->iCurrentItem++;
                       }
                    break;


               case WM_MOUSEMOVE:
                    pStCtrl = (PSTCONTROL) WinQueryWindowPtr(hwnd, 0);

                    if (pStCtrl->bUseHover != FALSE)
                       {
                       SetHoverBoxText(&pStCtrl->sthb, pStCtrl->szFlyOverText1);
                       ShowHoverBox(&pStCtrl->sthb, hwnd, 0, TRUE);
                       }
                    return WinDefWindowProc (hwnd, msg, mp1, mp2);
                    break;


               default:
                    return (WinDefWindowProc(hwnd, msg, mp1, mp2));
               }
        return (FALSE);
        }


/*************************************************************************************
  CreateControlBar: Creates controlbar & creates control.
*************************************************************************************/
HWND CreateControlBar (HWND hwnd, PSTCONTROLBAR pcb, char *szType, int iStyles, int iId)
     {
     static BOOL            bIsReg = 0;
     char                   szClass[] = "STControlBarClass";


     if (!bIsReg)
        {
        WinRegisterClass(WinQueryAnchorBlock(hwnd), szClass, (PFNWP)ControlBarWndProc,
                         CS_SYNCPAINT | CS_SIZEREDRAW, 4);
        bIsReg = TRUE;
        }

     memset(pcb, 0, sizeof(STCONTROLBAR));

     pcb->hwndParent = hwnd;
     pcb->bShow = TRUE;

     pcb->hwndControlBar = WinCreateWindow(hwnd, szClass, (PSZ)NULL, 0, FCF_NOBYTEALIGN,
                                           0, 0, 0, hwnd, HWND_TOP, 0, NULL, NULL);

     WinSetWindowPtr(pcb->hwndControlBar, 0, pcb);

     pcb->iControlId = iId;
     pcb->hwndControl = WinCreateWindow(pcb->hwndControlBar, szType, (PSZ) NULL, iStyles,
                                        0, 0, 0, 0, pcb->hwndControlBar, HWND_TOP,
                                        pcb->iControlId, 0, 0);
     return(pcb->hwndControlBar);
     }


/*************************************************************************************
  DestroyControlBar: Kills controlbar.
*************************************************************************************/
BOOL DestroyControlBar (PSTCONTROLBAR pcb)
     {
     return(WinDestroyWindow(pcb->hwndControlBar));
     }


/*************************************************************************************
  ControlBarWndProc: Handle Control bar messages.
*************************************************************************************/
MRESULT EXPENTRY ControlBarWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     RECTL              rcl, rcl1, rcl2, rcl11, rcl21;
     HPS                hps;
     POINTL             pp;

     PSTCONTROLBAR      pcb;



     switch (msg)
            {
            case WM_PAINT:
                 pcb = (PSTCONTROLBAR) WinQueryWindowPtr(hwnd, 0);

                 hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
                 WinQueryWindowRect(hwnd, &rcl);
                 WinFillRect(hps, &rcl, CLR_PALEGRAY);
                 ControlBar3D(hps, rcl);
                 WinEndPaint(hps);
                 break;

            case WM_CONTROL:
                 pcb = (PSTCONTROLBAR) WinQueryWindowPtr(hwnd, 0);

                 if (SHORT1FROMMP(mp1) == pcb->iControlId)
                    WinPostMsg(pcb->hwndParent, WM_CONTROLBARMSG, (PVOID)SHORT2FROMMP(mp1), (PVOID)mp2);
                 break;

            default:
                 return (WinDefWindowProc(hwnd, msg, mp1, mp2));
            }
     return (FALSE);
     }


/*************************************************************************************
  SizeControlBar: Sizes the control bar.
************************************************************************************/
int SizeControlBar(PSTCONTROLBAR pcb, int iVertTop, int iMaxLen)
     {
     RECTL              prclClient;
     FONTMETRICS        fm;
     ULONG              xpos, ypos, ysize, xsize;
     ULONG              xposC, yposC, ysizeC, xsizeC, yReturn;
     ULONG              yFont, xFont;
     HPS                hps;


     if (pcb->hwndControlBar == NULLHANDLE)
        return (0);

     if (pcb->bShow == FALSE)
        return (0);

     WinQueryWindowRect(pcb->hwndParent, &prclClient);

     ysize = (STBORDERSPACE * 2) + pcb->iControlHeight;

     xsize = iMaxLen;
     if (xsize == 0)
        xsize = prclClient.xRight;

     ypos = iVertTop - ysize;
     xpos = prclClient.xLeft;

     /* Size the control */
     xposC = xpos + STBORDERSPACE;

     if (pcb->iControlWidth == 0)
        xsizeC = xsize - (STBORDERSPACE * 2);
     else
        xsizeC = pcb->iControlWidth;

     if (pcb->iControlDecend > 0)
        {
        yposC = STBORDERSPACE - pcb->iControlDecend;
        ysizeC = pcb->iControlHeight + pcb->iControlDecend;
        }
     else
        {
        yposC = STBORDERSPACE;
        ysizeC = pcb->iControlHeight;
        }

     /* Draw the windows */
     WinSetWindowPos(pcb->hwndControlBar, HWND_TOP, xpos, ypos, xsize, ysize,
                     SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
     yReturn = ysize;

     WinSetWindowPos(pcb->hwndControl, HWND_BOTTOM, xposC, yposC, xsizeC, ysizeC,
                     SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);

     return (yReturn);
     }


/*************************************************************************************
  ShowControlBar: Shows (or hides) the control bar.
************************************************************************************/
BOOL ShowControlBar(PSTCONTROLBAR pcb, BOOL bShow)
     {
     if (pcb->hwndControlBar == NULLHANDLE)
        return (FALSE);

     if (pcb->bShow == bShow)
        return (pcb->bShow);

     if (bShow == TRUE)
        WinSetWindowPos(pcb->hwndControlBar, 0, 0, 0, 0, 0, SWP_SHOW);
     else
        WinSetWindowPos(pcb->hwndControlBar, 0, 0, 0, 0, 0, SWP_HIDE);

     pcb->bShow = bShow;

     return (!bShow);
     }


/*************************************************************************************
  EnableControlBar: Enable or disable controlbar.
*************************************************************************************/
void EnableControlBar(PSTCONTROLBAR pcb, BOOL bEnable)
     {
     WinEnableWindow(pcb->hwndControl, bEnable);
     }


/*************************************************************************************
  SizeCtrlControlBar: Set the size of the control in the bar.
*************************************************************************************/
void SizeCtrlControlBar(PSTCONTROLBAR pcb, int iControlHeight, int iControlWidth, int iDecend)
     {
     FONTMETRICS        fm;
     ULONG              yFont, xFont;
     HPS                hps;


     pcb->iControlHeight = iControlHeight;
     pcb->iControlWidth = iControlWidth;
     pcb->iControlDecend = iDecend;

     hps = WinGetPS(pcb->hwndControlBar);
     GpiQueryFontMetrics(hps, sizeof(fm), &fm);
     WinReleasePS(hps);

     if (pcb->iControlHeight == 0)
        pcb->iControlHeight = (fm.lMaxBaselineExt * 15) / 10;

     if (iDecend > 0)
        pcb->iControlDecend = fm.lMaxBaselineExt * iDecend;
     }


/*************************************************************************************
  ControlBar3D: Draws 3D look in specified RECTL.
*************************************************************************************/
void ControlBar3D (HPS hps, RECTL rcl)
     {
     POINTL             pp;

     GpiSetColor(hps, CLR_WHITE);

     pp.x = rcl.xLeft;
     pp.y = rcl.yBottom;
     GpiMove(hps, &pp);
     pp.x = rcl.xLeft;
     pp.y = rcl.yTop;
     GpiLine(hps, &pp);

     pp.x = rcl.xLeft;
     pp.y = rcl.yTop;
     GpiMove(hps, &pp);
     pp.x = rcl.xRight;
     pp.y = rcl.yTop;
     GpiLine(hps, &pp);

     GpiSetColor(hps, CLR_DARKGRAY);

     pp.x = rcl.xLeft + 1;
     pp.y = rcl.yBottom;
     GpiMove(hps, &pp);
     pp.x = rcl.xRight;
     pp.y = rcl.yBottom;
     GpiLine(hps, &pp);

     pp.x = rcl.xRight - 1;
     pp.y = rcl.yBottom;
     GpiMove(hps, &pp);
     pp.x = rcl.xRight - 1;
     pp.y = rcl.yTop - 1;
     GpiLine(hps, &pp);
     }


/*************************************************************************************
  CreateHoverBox: Creates HoverBox.
*************************************************************************************/
HWND CreateHoverBox (HWND hwndParent, PSTHOVERBOX phb)
     {
     static BOOL      bIsClassRegistered = 0;
     char              *pszClass = "STHoverBoxClass";
     ULONG              flFlags;


     if (!bIsClassRegistered)
        {
        WinRegisterClass(WinQueryAnchorBlock(hwndParent), (PCSZ)pszClass, (PFNWP)HoverBoxWndProc,
                         CS_SYNCPAINT | CS_SIZEREDRAW | CS_SAVEBITS, 4);
        bIsClassRegistered = TRUE;
        }

     memset(phb, 0, sizeof(STHOVERBOX));

     phb->hwndParent = hwndParent;

     flFlags = FCF_NOBYTEALIGN | FCF_BORDER;
     phb->hwndHoverBox = WinCreateStdWindow(hwndParent, WS_SYNCPAINT | WS_SAVEBITS, &flFlags, (PCSZ)pszClass, (PSZ)NULL,
                                            0, 0, (HMODULE)NULL, &phb->hwndHoverBoxChild);

     WinSetWindowPtr(phb->hwndHoverBoxChild, 0, phb);
     phb->bShow = FALSE;
     phb->bFirstShow = FALSE;
     phb->bMotion = TRUE;
     phb->fnUserTest = NULL;
     phb->idTimer = WinStartTimer(WinQueryAnchorBlock(phb->hwndParent), phb->hwndHoverBoxChild, IDT_HOVERTIMER, HOVER_TIMEOUT);

     return(phb->hwndHoverBox);
     }


/*************************************************************************************
  DestroyHoverBox: Kills HoverBox.
*************************************************************************************/
BOOL DestroyHoverBox (PSTHOVERBOX phb)
     {
     phb->bShow = FALSE;
     WinStopTimer(WinQueryAnchorBlock(phb->hwndHoverBox), phb->hwndHoverBoxChild, phb->idTimer);
     return(WinDestroyWindow(phb->hwndHoverBox));
     }


/*************************************************************************************
  IsMouseOnHwnd: Checks if Mouse is ontop of hwnd .
************************************************************************************/
BOOL IsMouseOnHwnd(HWND hwndHit, int iHitId, int (*fnUserTest)(void),  PPOINTL pptl)
     {
     RECTL              rcl;
     int                iHit;


     // if ((*fnUserTest) != NULL)
     //   {
     //   if ((*fnUserTest) == FALSE)
     //      return (FALSE);
     //   }

     if (iHitId != 0)
        {
        if (WinQueryWindowUShort(hwndHit, QWS_ID) != iHitId)
           return (FALSE);
        }

     WinQueryPointerPos(HWND_DESKTOP, pptl);
     WinMapWindowPoints(HWND_DESKTOP, hwndHit, pptl, 1);
     WinQueryWindowRect(hwndHit, &rcl);
     if (WinPtInRect(WinQueryAnchorBlock(hwndHit), &rcl, pptl))
        return (TRUE);
     else
        return(FALSE);
     }


/*************************************************************************************
  ShowHoverBox: Shows/positions the Hover Box.
************************************************************************************/
void ShowHoverBox(PSTHOVERBOX phb, HWND hwndHit, int iHitId, BOOL bShow)
     {
     RECTL              rcl;
     POINTL             ptl;
     int                yPos;
     unsigned long      ulIgnore;
     APIRET             rc;


     if (bShow == FALSE)
        {
        phb->bShow = FALSE;
        WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, 0, 0, 0, 0, SWP_HIDE);
        return;
        }

     phb->hwndHit = hwndHit;

     if (IsMouseOnHwnd(phb->hwndHit, phb->iHitId, phb->fnUserTest, &ptl) == TRUE)
        {
        if (phb->bMotion == FALSE)
           {
           if (phb->bShow == TRUE)
              return;
           }

        if (phb->bMotion == FALSE)
           {
           WinQueryWindowRect(hwndHit, &rcl);
           ptl.x = 0;
           ptl.y = 0;
           WinMapWindowPoints(hwndHit, phb->hwndParent, &ptl, 1);
           WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, ptl.x,
                           ptl.y - rcl.yTop,
                           phb->xSize, phb->ySize,
                           SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
           }
        else
           {
           if (phb->bShow == TRUE)
              {
              WinQueryWindowRect(hwndHit, &rcl);
              WinMapWindowPoints(hwndHit, phb->hwndParent, &ptl, 1);

              WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, ptl.x - (phb->xSize / 2) + WinQuerySysValue(HWND_DESKTOP, SV_CXPOINTER),
                             (ptl.y - phb->ySize) - (WinQuerySysValue(HWND_DESKTOP, SV_CYPOINTER) / 2),
                             phb->xSize, phb->ySize,
                             SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);

              if (WinIsWindowShowing(phb->hwndHoverBox) == FALSE)
                 WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, ptl.x - (phb->xSize / 2) + WinQuerySysValue(HWND_DESKTOP, SV_CXPOINTER),
                                 (ptl.y - phb->ySize) - (WinQuerySysValue(HWND_DESKTOP, SV_CYPOINTER) / 2),
                                 phb->xSize, phb->ySize,
                                 SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
              }
           else
              {
              WinQueryWindowRect(hwndHit, &rcl);
              WinMapWindowPoints(hwndHit, phb->hwndParent, &ptl, 1);

              WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, ptl.x - (phb->xSize / 2) + WinQuerySysValue(HWND_DESKTOP, SV_CXPOINTER),
                              (ptl.y - phb->ySize) - (WinQuerySysValue(HWND_DESKTOP, SV_CYPOINTER) / 2),
                              phb->xSize, phb->ySize,
                              SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
              }
           }

        WinStartTimer(WinQueryAnchorBlock(phb->hwndParent), phb->hwndHoverBoxChild, IDT_HOVERTIMER, HOVER_TIMEOUT);
        phb->bShow = TRUE;
        }
     else
        {
        phb->bShow = FALSE;
        WinSetWindowPos(phb->hwndHoverBox, HWND_TOP, 0, 0, 0, 0, SWP_HIDE);
        }
     phb->iHitId = iHitId;
     }


/*************************************************************************************
  HoverBoxWndProc: Handle messages to HoverBox.
*************************************************************************************/
MRESULT EXPENTRY HoverBoxWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     RECTL              rcl;
     POINTL             ptl;
     HPS                hps;

     PSTHOVERBOX        phb;
     int                iStrLen;
     char               szText[256];


     switch (msg)
            {
            case WM_CREATE:
                 break;

            case WM_TIMER:
                 phb = (PSTHOVERBOX) WinQueryWindowPtr(hwnd, 0);

                 if (phb->bShow == TRUE)
                    {
                    if (SHORT1FROMMP(mp1) == IDT_HOVERTIMER && IsMouseOnHwnd(phb->hwndHit, phb->iHitId, phb->fnUserTest, &ptl) == FALSE)
                       {
                       WinShowWindow(phb->hwndHoverBox, FALSE);
                       phb->bShow = FALSE;
                       }
                    }
                 break;

            case WM_PAINT:
                 phb = (PSTHOVERBOX) WinQueryWindowPtr(hwnd, 0);

                 hps = WinBeginPaint(hwnd, 0L, 0L);
                 WinQueryWindowRect(hwnd, &rcl);

                 szText[0] = ' ';
                 strcpy(szText + 1, phb->szText);
                 iStrLen = strlen(szText);
                 szText[iStrLen] = ' ';
                 szText[iStrLen + 1] = '\0';

                 WinDrawText(hps, -1, (PCSZ)szText, &rcl, phb->iColorText, phb->iColorBack,
                             phb->iAlignInt | DT_ERASERECT | DT_VCENTER);

                 WinEndPaint(hps);
                 break ;

            default:
                 return ((MRESULT) WinDefWindowProc(hwnd, msg, mp1, mp2));

            }
     return (FALSE);
     }


/*************************************************************************************
  SetHoverBoxText: Sets text of HoverBox.
*************************************************************************************/
void SetHoverBoxText(PSTHOVERBOX phb, char* szText)
     {
     HPS                hps;
     RECTL              rcl;
     char               szTmpText[256];
     int                iStrLen;


     if (strcmp(szText, phb->szText) == 0)
        return;

     strcpy(phb->szText, szText);

     memset(&rcl, 0, sizeof(RECTL));
     rcl.yTop = 100;
     rcl.xRight = 100;

     szTmpText[0] = ' ';
     strcpy(szTmpText + 1, phb->szText);
     iStrLen = strlen(szTmpText);
     szTmpText[iStrLen] = ' ';
     szTmpText[iStrLen + 1] = '\0';

     hps = WinGetPS(phb->hwndHoverBoxChild);
     WinDrawText(hps, -1, (PCSZ)szTmpText, &rcl, 0, 0, DT_QUERYEXTENT);
     WinReleasePS(hps);

     phb->xSize = (STBORDERSPACE * 2) + (rcl.xRight- rcl.xLeft);
     phb->ySize = (STBORDERSPACE * 2) + (rcl.yTop - rcl.yBottom);

     WinInvalidateRect(phb->hwndHoverBox, NULL, FALSE);
     }


/*************************************************************************************
  SetHoverBoxMotion: Move or not on window.
*************************************************************************************/
void SetHoverBoxMotion (PSTHOVERBOX phb, BOOL bMotion)
     {
     phb->bMotion = bMotion;
     }


/*************************************************************************************
  SetHoverBoxFont: Sets font of HoverBox.
*************************************************************************************/
void SetHoverBoxFont(PSTHOVERBOX phb, char* szFont)
     {
     strcpy(phb->szFont, szFont);
     WinSetPresParam(phb->hwndHoverBoxChild, PP_FONTNAMESIZE, (ULONG)strlen(phb->szFont) + 1, &phb->szFont);

     WinInvalidateRect(phb->hwndHoverBox, NULL, FALSE);
     }


/*************************************************************************************
  SetHoverBoxColor: Sets color of text/background in HoverBox.
*************************************************************************************/
void SetHoverBoxColor(PSTHOVERBOX phb, int iColorText, int iColorBack)
     {
     phb->iColorText = iColorText;
     phb->iColorBack = iColorBack;

     WinInvalidateRect(phb->hwndHoverBox, NULL, FALSE);
     }


/*************************************************************************************
  SetHoverBoxAlignment: Sets aligmnent of HoverBox text.
*************************************************************************************/
void SetHoverBoxAlignment (PSTHOVERBOX phb, int iAlign)
     {
     phb->iAlign = iAlign;

     switch (phb->iAlign)
            {
            case HOVER_LEFT:
                 phb->iAlignInt = DT_LEFT;
                 break;

            case HOVER_CENTER:
                 phb->iAlignInt = DT_CENTER;
                 break;

            case HOVER_RIGHT:
                 phb->iAlignInt = DT_RIGHT;
                 break;
            }

     WinInvalidateRect(phb->hwndHoverBox, NULL, FALSE);
     }

/*************************************************************************************
  SetHoverCallBack: Sets callback function for check if on window.
*************************************************************************************/
void SetHoverCallBack (PSTHOVERBOX phb, int (*fnUserTest)(void))
     {
     phb->fnUserTest = fnUserTest;
     }



/*-----------------------------------------------------------------------------
   MLEImport: Imports text into MLE (more than 64k supported).
-----------------------------------------------------------------------------*/
BOOL MLEImport(HWND hwndMLE, char *pszText, ULONG ulTextLength)
     {
     CHAR       *pszCurPos;
     void       *pBuffer;
     IPT        ipt = 0;
     ULONG      ulBytesCopied, ulToCopy;
     ULONG      ulBufSize = 65535;


     DosAllocMem ((PPVOID) &pBuffer, (ULONG) ulBufSize, PAG_COMMIT | PAG_READ | PAG_WRITE);

     WinSendMsg (hwndMLE, MLM_SETIMPORTEXPORT, MPFROMP (pBuffer), MPFROMLONG (ulBufSize));

     pszCurPos = pszText;

     do
        {
         ulToCopy = min(ulBufSize, ulTextLength);
         memcpy(pBuffer, pszCurPos, ulToCopy);
         ulBytesCopied = (ULONG) WinSendMsg (hwndMLE, MLM_IMPORT, MPFROMP(&ipt), MPFROMLONG(ulToCopy));
         pszCurPos += ulToCopy;
         ulTextLength -= ulToCopy;
        }
     while (ulTextLength > 0);

     DosFreeMem (pBuffer);
     return (TRUE);
     }


/*-----------------------------------------------------------------------------
   MLEExportLength: Length of (export) text in MLE.
-----------------------------------------------------------------------------*/
ULONG MLEExportLength(HWND hwndMLE)
     {
     IPT        ipt = 0;


     return (ULONG) WinSendMsg(hwndMLE, MLM_QUERYFORMATTEXTLENGTH,
                               MPFROMLONG(ipt), MPFROMLONG((-1)));

     }


/*-----------------------------------------------------------------------------
   MLEExport: Exports text from MLE (more than 64k supported).
-----------------------------------------------------------------------------*/
BOOL MLEExport(HWND hwndMLE, char *pszText, ULONG ulTextLength)
     {
     CHAR       *pszCurPos;
     void       *pBuffer;
     IPT        ipt = 0;
     ULONG      ulBufSize = 32768;
     ULONG      ulCopyLen, ulExported;


     DosAllocMem((PPVOID) &pBuffer, (ULONG) ulBufSize, PAG_COMMIT | PAG_READ | PAG_WRITE);

     pszCurPos = pszText;
     WinSendMsg(hwndMLE, MLM_SETIMPORTEXPORT, MPFROMP(pBuffer), MPFROMLONG(ulBufSize));
     do
        {
        ulCopyLen = min(ulBufSize, ulTextLength);
        ulExported = (ULONG) WinSendMsg(hwndMLE, MLM_EXPORT, MPFROMP(&ipt), MPFROMP(&ulCopyLen));
        memcpy(pszCurPos, pBuffer, ulExported);
        ulTextLength -= ulExported;
        pszCurPos += ulExported;
        }
     while (ulTextLength > 0);

     DosFreeMem(pBuffer);
     return (TRUE);
     }

/*************************************************************************************
  MessageBox: Simplified messagebox.
*************************************************************************************/
BOOL MessageBox (HWND hwnd, USHORT usBoxType, PSZ szTitle, PSZ szMessage)
     {
     ULONG      ulTmp;

     if (hwnd == 0)
        hwnd = HWND_DESKTOP;

     switch (usBoxType)
            {
            case 0:
                 WinMessageBox(HWND_DESKTOP, hwnd, szMessage, szTitle, 0, MB_CUANOTIFICATION | MB_APPLMODAL | MB_MOVEABLE);
                 return (TRUE);
                 break;
            case 1:
            case 2:
                 if (usBoxType == 1)
                    ulTmp = MB_DEFBUTTON1;
                 else
                    ulTmp = MB_DEFBUTTON2;
                 if (WinMessageBox(HWND_DESKTOP, hwnd, szMessage, szTitle, 0, MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL | MB_MOVEABLE | ulTmp) == MBID_YES)
                    return (TRUE);
                 else
                    return (FALSE);
                 break;
            case 3:
                 break;
            default:
                 break;
            }
     return (TRUE);
     }


/*************************************************************************************
  EXTDEBUG: Shows debug-message/WinLastError.
*************************************************************************************/
void EXTDEBUG (HAB hab, ULONG ulRetCode, PSZ szMessage)
     {
     ERRORID            erridLast;
     char               szErrNumber[256], szSevere[50];
     int                iSeverity = 0, iErrorNo = 0;


     erridLast = WinGetLastError(hab);

     iSeverity = ERRORIDSEV(erridLast);
     iErrorNo = ERRORIDERROR(erridLast);

      switch (iSeverity)
             {
             case SEVERITY_NOERROR:
                  sprintf(szSevere, "SEVERITY_NOERROR");
                  break;

             case SEVERITY_WARNING:
                  sprintf(szSevere, "SEVERITY_WARNING");
                  break;

             case SEVERITY_ERROR:
                  sprintf(szSevere, "SEVERITY_ERROR");
                  break;

             case SEVERITY_SEVERE:
                  sprintf(szSevere, "SEVERITY_SEVERE");
                  break;

             case SEVERITY_UNRECOVERABLE:
                  sprintf(szSevere, "SEVERITY_UNRECOVERABLE");
                  break;
             }

     sprintf(szErrNumber, "%s\nYour retcode => %d\nSeverity = %s \nErrorcode = %x", szMessage, ulRetCode, szSevere, iErrorNo);

     WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)szErrNumber, (PSZ)"*** EXT_DEBUG ***",
                   0, MB_CUANOTIFICATION | MB_APPLMODAL | MB_MOVEABLE);
     }


/*************************************************************************************
  DEBUG: Shows debug-message using HWND_DESKTOP only.
*************************************************************************************/
void DEBUG (PSZ szMessage)
     {
     WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)szMessage, (PSZ)"*** DEBUG ***",
                   0, MB_CUANOTIFICATION | MB_APPLMODAL | MB_MOVEABLE);
     }


/*************************************************************************************
  CreateSplitBar: Creates Splitbar.
*************************************************************************************/
HWND CreateSplitBar (HWND hwndParent, BOOL bDirection, PSPLITBAR psb)
     {
     static BOOL        bIsClassRegistered = 0;
     PSZ                pszClass = "STSplitBarClass";


     if (!bIsClassRegistered)
        {
        WinRegisterClass(WinQueryAnchorBlock(hwndParent), pszClass, (PFNWP)SplitBarWndProc,
                         CS_SIZEREDRAW | CS_SYNCPAINT | CS_SAVEBITS, 4);
        bIsClassRegistered = TRUE;
        }

     psb->bDirection = bDirection;
     psb->hwndParent = hwndParent;
     psb->bShow = TRUE;

     psb->hwndSplitBarChild = WinCreateWindow(hwndParent, pszClass, (PSZ)NULL, 0, 0, 0, 0, 0,
                                              hwndParent, HWND_TOP, 0, NULL, NULL);

     WinSetWindowPtr(psb->hwndSplitBarChild, 0, psb);

     return(psb->hwndSplitBarChild);
     }


/*************************************************************************************
  DestroySplitBar: Kills splitbar.
*************************************************************************************/
BOOL DestroySplitBar (PSPLITBAR psb)
     {
     return(WinDestroyWindow(psb->hwndSplitBarChild));
     }


/*************************************************************************************
  SplitBarWndProc: Handle messages to splitbar.
*************************************************************************************/
MRESULT EXPENTRY SplitBarWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static     BOOL            bDrag;
     static     HPOINTER        hPointerV, hPointerH;

     POINTL     ptl;
     RECTL      rcl;
     HPS        hps;
     PSPLITBAR  psb;

     switch (msg)
            {
            case WM_CREATE:
                 bDrag = FALSE;
                 hPointerV = WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZEWE, FALSE);
                 hPointerH = WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZENS, FALSE);
                 break;

            case WM_BEGINDRAG:
            case WM_BUTTON1MOTIONSTART:
                 bDrag = TRUE;
                 WinSetCapture(HWND_DESKTOP, hwnd);
                 break;

            case WM_MOUSEMOVE:
                 psb = (PSPLITBAR) WinQueryWindowPtr(hwnd, 0);
                 if (bDrag == TRUE)
                    {
                    WinQueryPointerPos(HWND_DESKTOP, &ptl);
                    WinMapWindowPoints(HWND_DESKTOP, psb->hwndParent, &ptl, 1);
                    switch (psb->bDirection)
                           {
                           case SBP_HORZ:
                                WinSetWindowPos(psb->hwndSplitBarChild, HWND_TOP, psb->iBorderBL, ptl.y, 0, 0,
                                                SWP_MOVE | SWP_SHOW | SWP_ZORDER);
                                break;

                           case SBP_VERT:
                                WinSetWindowPos(psb->hwndSplitBarChild, HWND_TOP, ptl.x, psb->iBorderBL, 0, 0,
                                                SWP_MOVE | SWP_SHOW | SWP_ZORDER);
                                break;

                           }
                    }
                 switch (psb->bDirection)
                        {
                        case SBP_HORZ:
                             WinSetPointer(HWND_DESKTOP, hPointerH);
                             break;

                        case SBP_VERT:
                             WinSetPointer(HWND_DESKTOP, hPointerV);
                             break;
                        }
                 break;

            case WM_ENDDRAG:
            case WM_BUTTON1MOTIONEND:
                 if (bDrag == TRUE)
                    {
                    psb = (PSPLITBAR) WinQueryWindowPtr(hwnd, 0);
                    WinSetCapture(HWND_DESKTOP, NULLHANDLE);
                    WinQueryPointerPos(HWND_DESKTOP, &ptl);
                    WinMapWindowPoints(HWND_DESKTOP, psb->hwndParent, &ptl, 1);
                    bDrag = FALSE;
                    switch (psb->bDirection)
                           {
                           case SBP_VERT:
                                WinPostMsg(psb->hwndParent, WM_SPLITBARMOVED, (PVOID) SHORT1FROMMP(ptl.x), NULL);
                                break;

                           case SBP_HORZ:
                                WinPostMsg(psb->hwndParent, WM_SPLITBARMOVED, (PVOID) SHORT1FROMMP(ptl.y), NULL);
                                break;
                           }
                    }
                 break;

            case WM_PAINT:
                 psb = (PSPLITBAR) WinQueryWindowPtr(hwnd, 0);
                 hps = WinBeginPaint(hwnd, 0L, 0L);
                 WinQueryWindowRect(hwnd, &rcl);
                 WinFillRect(hps, &rcl, CLR_PALEGRAY);

                 switch (psb->bDirection)
                        {
                        case SBP_VERT:
                             WinDrawBorder(hps, &rcl, 1, 0, CLR_DARKGRAY, CLR_PALEGRAY, DB_STANDARD);
                             break;

                        case SBP_HORZ:
                             WinDrawBorder(hps, &rcl, 0, 1, CLR_DARKGRAY, CLR_PALEGRAY, DB_STANDARD);
                             break;
                        }
                 WinEndPaint(hps);
                 break ;

            default:
                 return ((MRESULT) WinDefWindowProc(hwnd, msg, mp1, mp2));

            }
     return (FALSE);
     }


/*************************************************************************************
  PositionSplitBar: Places the splitbar at specified coordinates.
************************************************************************************/
ULONG PositionSplitBar (PSPLITBAR psb, int iBorderBL, int iBorderTR, ULONG ulSplitBarPos)
      {
      RECTL     rclParent;
      ULONG     lBorderWidth;
      ULONG     ypos, xpos, ysize, xsize;


      if (psb->hwndSplitBarChild == NULLHANDLE)
         return (0);

      if (psb->bShow == FALSE)
         return (0);

      WinQueryWindowRect(psb->hwndParent, &rclParent);

      xpos  = rclParent.xLeft;
      xsize = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
      ypos  = rclParent.yBottom;
      ysize = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);


      psb->iBorderBL = iBorderBL;
      psb->iBorderTR = iBorderTR;

      switch (psb->bDirection)
             {
             case SBP_HORZ:
                  ypos = ulSplitBarPos;
                  xpos = xpos + iBorderBL;
                  xsize = rclParent.xRight - (iBorderTR + iBorderBL);
                  break;

             case SBP_VERT:
                  xpos  = ulSplitBarPos;
                  ypos = ypos + iBorderBL;
                  ysize = rclParent.yTop - (iBorderTR + iBorderBL);
                  break;
             }

      WinSetWindowPos(psb->hwndSplitBarChild, HWND_TOP, xpos, ypos, xsize, ysize,
                      SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);

      if (psb->bDirection == SBP_VERT)
         return(xsize);
      else
         return(ysize);

      }


/*************************************************************************************
  ShowSplitBar: Shows (or hides) the split bar.
************************************************************************************/
BOOL ShowSplitBar(PSPLITBAR psb, BOOL bShow)
     {
     if (psb->hwndSplitBarChild == NULLHANDLE)
        return (FALSE);

     if (psb->bShow == bShow)
        return (psb->bShow);

     if (bShow == TRUE)
        WinSetWindowPos(psb->hwndSplitBarChild, 0, 0, 0, 0, 0, SWP_SHOW);
     else
        WinSetWindowPos(psb->hwndSplitBarChild, 0, 0, 0, 0, 0, SWP_HIDE);

     psb->bShow = bShow;

     return (!bShow);
     }



/*************************************************************************************
  CreateButtonBar: Creates buttonbar & loads icons.
*************************************************************************************/
HWND CreateStatusBar (HWND hwnd, PSTSTATUSBAR pstsb, int iType, int iItem1Type, int iItem2Type)
     {
     char       szClass[] = "STStatusBarClass";
     ULONG      flFlags;


     pstsb->hwndOwner = hwnd;
     pstsb->bShow = TRUE;

     if (!bIsReg)
        {
        WinRegisterClass(WinQueryAnchorBlock(pstsb->hwndOwner), szClass, (PFNWP)StatusBarWndProc, CS_SIZEREDRAW | CS_SYNCPAINT, 4);
        bIsReg = TRUE;
        }

     pstsb->iType = iType;

     pstsb->iType1 = iItem1Type;
     pstsb->iType2 = iItem2Type;

     pstsb->hwndStatus = WinCreateWindow(hwnd, szClass, (PSZ)NULL, 0, 0, 0, 0, 0, hwnd, HWND_TOP, 0, NULL, NULL);
     WinSetWindowPtr(pstsb->hwndStatus, 0, pstsb);

     flFlags = FCF_NOBYTEALIGN | WS_SYNCPAINT;

     if (pstsb->iType1 == STATUS_TEXT)
        pstsb->hwndChild1 = WinCreateWindow(pstsb->hwndStatus, WC_STATIC, (PSZ)NULL, SS_TEXT | DT_VCENTER,
                                             0, 0, 0, 0, pstsb->hwndStatus, HWND_BOTTOM, 0, NULL, NULL);
     else
        pstsb->hwndChild1 = WinCreateWindow(pstsb->hwndStatus, WC_BUTTON, (PSZ) NULL, BS_PUSHBUTTON | BS_USERBUTTON | BS_NOPOINTERFOCUS,
                                            0, 0, 0, 0, pstsb->hwndStatus, HWND_BOTTOM, 0, NULL, NULL);

     if (pstsb->iType2 == STATUS_TEXT)
        pstsb->hwndChild2 = WinCreateWindow(pstsb->hwndStatus, WC_STATIC, (PSZ)NULL, SS_TEXT | DT_VCENTER,
                                            0, 0, 0, 0, pstsb->hwndStatus, HWND_BOTTOM, 0, NULL, NULL);
     else
        pstsb->hwndChild2 = WinCreateWindow(pstsb->hwndStatus, WC_BUTTON, (PSZ) NULL, BS_PUSHBUTTON | BS_USERBUTTON | BS_NOPOINTERFOCUS,
                                            0, 0, 0, 0, pstsb->hwndStatus, HWND_BOTTOM, 0, NULL, NULL);

     SetStatusBarText(pstsb, "", "");
     SetStatusBarColor(pstsb, CLR_BLACK, CLR_BLACK);
     SetStatusBarBackColor(pstsb, CLR_PALEGRAY, CLR_PALEGRAY);
     SetStatusBarFont(pstsb, "8.Helv");
     SetStatusBarAlignment(pstsb, STATUS_LEFT, STATUS_LEFT);
     pstsb->iTwoSplit = 5;

     return(pstsb->hwndStatus);
     }


/*************************************************************************************
  DestroyStatusBar: Kills statusbar.
*************************************************************************************/
BOOL DestroyStatusBar (PSTSTATUSBAR pstsb)
     {
     WinDestroyWindow(pstsb->hwndChild1);
     WinDestroyWindow(pstsb->hwndChild2);

     return(WinDestroyWindow(pstsb->hwndStatus));
     }


/*************************************************************************************
  StatusBarWndProc: Handle Status bar paint.
*************************************************************************************/
MRESULT EXPENTRY StatusBarWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     RECTL              rcl, rcl1, rcl2, rcl11, rcl21;
     HPS                hps;
     POINTL             pp;
     int                iAlign, fc, bc, iStrLen;
     char               szText1[130], szText2[130];
     PSTSTATUSBAR       pstsb;
     PUSERBUTTON        ubInfo;


     switch (msg)
            {
            case WM_SIZE:
                 WinQueryWindowRect(hwnd, &rcl);
                 if (rcl.xRight == 0)
                    break;
                 else
                    ResizeStatusBarClients((PSTSTATUSBAR) WinQueryWindowPtr(hwnd, 0));
                 break;

            case WM_PAINT:
                 pstsb = (PSTSTATUSBAR) WinQueryWindowPtr(hwnd, 0);

                 hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
                 WinQueryWindowRect(hwnd, &rcl);

                 StatusBar3D(hps, rcl, 0, TRUE);

                 switch (pstsb->iType)
                        {
                        case STATUS_ONE:
                             CalcOneRect(pstsb, &rcl, &rcl1);
                             StatusBar3D(hps, rcl1, 1, FALSE);
                             break;

                        case STATUS_TWO:
                             CalcTwoRect(pstsb, &rcl, &rcl1, &rcl2);
                             StatusBar3D(hps, rcl1, 1, FALSE);
                             StatusBar3D(hps, rcl2, 1, FALSE);
                             break;
                        }
                 WinEndPaint(hps);
                 break;


            case WM_CONTROL:
                 pstsb = (PSTSTATUSBAR) WinQueryWindowPtr(hwnd, 0);
                 switch (SHORT2FROMMP(mp1))
                        {
                        case BN_PAINT:
                             ubInfo = (PUSERBUTTON) LONGFROMMP(mp2);

                             if (ubInfo->hwnd == pstsb->hwndChild1)
                                pstsb->iItemToRedraw = 1;
                             else
                                pstsb->iItemToRedraw = 2;

                             pstsb->hwndRedraw = ubInfo->hwnd;
                             pstsb->hpsRedraw = ubInfo->hps;
                             WinSendMsg(pstsb->hwndOwner, WM_STATUSREDRAW, MPFROMLONG(pstsb->iStatusId), MPFROMLONG(pstsb->iItemToRedraw));
                        }

            default:
                 return (WinDefWindowProc(hwnd, msg, mp1, mp2));
            }
     return (FALSE);
     }


/*************************************************************************************
  CalcOneRect: Calcs rect for one fields.
************************************************************************************/
void CalcOneRect(PSTSTATUSBAR pstsb, PRECTL prcl, PRECTL prcl1)
     {
     prcl1->yBottom = prcl->yBottom + STBORDERSPACE;
     prcl1->yTop = prcl->yTop - STBORDERSPACE;
     prcl1->xLeft = prcl->xLeft + STBORDERSPACE;
     prcl1->xRight = (prcl->xRight - 1) - STBORDERSPACE;
     }


/*************************************************************************************
  CalcTwoRect: Calcs rect for two fields.
************************************************************************************/
void CalcTwoRect(PSTSTATUSBAR pstsb, PRECTL prcl, PRECTL prcl1, PRECTL prcl2)
     {
     prcl1->yBottom = prcl->yBottom + STBORDERSPACE;
     prcl1->yTop = prcl->yTop - STBORDERSPACE;
     prcl1->xLeft = prcl->xLeft + STBORDERSPACE;
     prcl1->xRight = (((prcl->xRight - prcl->xLeft) / 10)  * pstsb->iTwoSplit) - STBORDERSPACE;

     prcl2->yBottom = prcl->yBottom + STBORDERSPACE;
     prcl2->yTop = prcl->yTop - STBORDERSPACE;
     prcl2->xLeft =  prcl1->xRight + STBORDERSPACE;
     prcl2->xRight = (prcl->xRight - 1) - STBORDERSPACE;
     }


/*************************************************************************************
  ResizeStatusBarClients: Sizes clients.
************************************************************************************/
void ResizeStatusBarClients (PSTSTATUSBAR pstsb)
     {
     RECTL              rcl, rcl1, rcl2;


     WinQueryWindowRect(pstsb->hwndStatus, &rcl);
     switch (pstsb->iType)
            {
            case STATUS_ONE:
                 CalcOneRect(pstsb, &rcl, &rcl1);
                 WinSetWindowPos(pstsb->hwndChild1, HWND_BOTTOM,
                                 rcl1.xLeft + 1, rcl1.yBottom + 1,
                                 (rcl1.xRight - 1) - (rcl1.xLeft + 1), (rcl1.yTop - 1) - (rcl1.yBottom + 1),
                                 SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
                 break;

            case STATUS_TWO:
                 CalcTwoRect(pstsb, &rcl, &rcl1, &rcl2);
                 WinSetWindowPos(pstsb->hwndChild1, HWND_BOTTOM,
                                 rcl1.xLeft + 1, rcl1.yBottom + 1,
                                 (rcl1.xRight - 1) - (rcl1.xLeft + 1), (rcl1.yTop - 1) - (rcl1.yBottom + 1),
                                 SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);

                 WinSetWindowPos(pstsb->hwndChild2, HWND_BOTTOM,
                                 rcl2.xLeft + 1, rcl2.yBottom + 1,
                                 (rcl2.xRight - 1) - (rcl2.xLeft + 1), (rcl2.yTop - 1) - (rcl2.yBottom + 1),
                                 SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
                 break;
            }
     }


/*************************************************************************************
  UpdateStatusBarItem: Redraws ownerdraw item.
************************************************************************************/
void UpdateStatusBarItem(PSTSTATUSBAR pstsb, int iItem)
     {
     if (iItem == 1)
        WinInvalidateRect(pstsb->hwndChild1, NULL, TRUE);
     else
        WinInvalidateRect(pstsb->hwndChild2, NULL, TRUE);
     }


/*************************************************************************************
  SizeStatusBar: Sizes the status bar.
************************************************************************************/
int SizeStatusBar (PSTSTATUSBAR pstsb, int iMaxLen)
     {
     RECTL              prclClient;
     FONTMETRICS        fm;
     ULONG              xpos, ypos, ysize, xsize;
     ULONG              yFont, xFont;
     HPS                hps;

     if (pstsb->hwndStatus == NULLHANDLE)
         return (0);

     if (pstsb->bShow == FALSE)
         return (0);

     WinQueryWindowRect(pstsb->hwndOwner, &prclClient);

     ypos = prclClient.yBottom;
     xpos = prclClient.xLeft;

     hps = WinGetPS(pstsb->hwndStatus);
     GpiQueryFontMetrics(hps, sizeof(fm), &fm);
     yFont = fm.lMaxBaselineExt;
     xFont = fm.lEmInc;
     WinReleasePS(hps);

     ysize = prclClient.yBottom + (STBORDERSPACE * 2) + yFont + 4;

     xsize = iMaxLen;
     if (xsize == 0)
        xsize = prclClient.xRight;

     WinSetWindowPos(pstsb->hwndStatus, HWND_TOP, xpos, ypos, xsize, ysize,
                     SWP_SHOW | SWP_SIZE | SWP_MOVE | SWP_ZORDER);
     return (ysize);
     }


/*************************************************************************************
  ShowStatusBar: Shows (or hides) the status bar.
************************************************************************************/
BOOL ShowStatusBar (PSTSTATUSBAR pstsb, BOOL bShow)
     {

     if (pstsb->hwndStatus == NULLHANDLE)
        return (FALSE);

     if (pstsb->bShow == bShow)
        return (pstsb->bShow);

     if (bShow == TRUE)
        WinSetWindowPos(pstsb->hwndStatus, 0, 0, 0, 0, 0, SWP_SHOW);
     else
        WinSetWindowPos(pstsb->hwndStatus, 0, 0, 0, 0, 0, SWP_HIDE);

     pstsb->bShow = bShow;

     return (!bShow);
     }


/*************************************************************************************
  SetStatusBarText: Sets text of statusbar.
*************************************************************************************/
void SetStatusBarText (PSTSTATUSBAR pstsb, char* szText1, char* szText2)
     {
     char       szTemp1[128], szTemp2[128];


     strncpy(szTemp1, szText1, 128);
     strncpy(szTemp2, szText2, 128);

     if (strcmp(szTemp1, pstsb->szText1) != 0)
        {
        sprintf(pstsb->szText1, " %s", szTemp1);
        WinSetWindowText(pstsb->hwndChild1, pstsb->szText1);
        }

     if (strcmp(szTemp2, pstsb->szText2) != 0)
        {
        sprintf(pstsb->szText2, " %s", szTemp2);
        WinSetWindowText(pstsb->hwndChild2, pstsb->szText2);
        }
     }


/*************************************************************************************
  SetStatusBarColor: Sets color of text in statusbar.
*************************************************************************************/
void SetStatusBarColor (PSTSTATUSBAR pstsb, int iColor1, int iColor2)
     {
     pstsb->iColor1 = iColor1;
     pstsb->iColor2 = iColor2;

     WinSetPresParam(pstsb->hwndChild1, PP_FOREGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColor1);
     WinSetPresParam(pstsb->hwndChild2, PP_FOREGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColor2);
     }


/*************************************************************************************
  SetStatusBarBackColor: Sets color of text in statusbar.
*************************************************************************************/
void SetStatusBarBackColor (PSTSTATUSBAR pstsb, int iColor1, int iColor2)
     {
     WinSetPresParam(pstsb->hwndChild1, PP_BACKGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColor1);
     WinSetPresParam(pstsb->hwndChild2, PP_BACKGROUNDCOLORINDEX, (ULONG)sizeof(long), (PVOID)&iColor2);
     }


/*************************************************************************************
  SetStatusBarFont: Sets font of statusbar.
*************************************************************************************/
void SetStatusBarFont (PSTSTATUSBAR pstsb, char* szFont)
     {
     strcpy(pstsb->szFont, szFont);
     WinSetPresParam(pstsb->hwndChild1, PP_FONTNAMESIZE, (ULONG)strlen(pstsb->szFont) + 1, (PVOID)&pstsb->szFont);
     WinSetPresParam(pstsb->hwndChild2, PP_FONTNAMESIZE, (ULONG)strlen(pstsb->szFont) + 1, (PVOID)&pstsb->szFont);
     }


/*************************************************************************************
  SetStatusBarAlignment: Sets aligmnent of statusbar text(s).
*************************************************************************************/
void SetStatusBarAlignment (PSTSTATUSBAR pstsb, int iAlign1, int iAlign2)
     {
     ULONG              ulValue;

     pstsb->iAlign1 = iAlign1;
     pstsb->iAlign2 = iAlign2;

     switch (pstsb->iAlign1)
            {
            case STATUS_LEFT:
                 pstsb->iAlignInt1 = DT_LEFT;
                 break;

            case STATUS_CENTER:
                 pstsb->iAlignInt1 = DT_CENTER;
                 break;

            case STATUS_RIGHT:
                 pstsb->iAlignInt1 = DT_RIGHT;
                 break;
            }

     switch (pstsb->iAlign2)
            {
            case STATUS_LEFT:
                 pstsb->iAlignInt2 = DT_LEFT;
                 break;

            case STATUS_CENTER:
                 pstsb->iAlignInt2 = DT_CENTER;
                 break;

            case STATUS_RIGHT:
                 pstsb->iAlignInt2 = DT_RIGHT;
                 break;
            }

     ulValue = WinQueryWindowULong(pstsb->hwndChild1, QWL_STYLE);
     ulValue |= pstsb->iAlignInt1;
     WinSetWindowULong(pstsb->hwndChild1, QWL_STYLE, ulValue);

     ulValue = WinQueryWindowULong(pstsb->hwndChild2, QWL_STYLE);
     ulValue |= pstsb->iAlignInt2;
     WinSetWindowULong(pstsb->hwndChild2, QWL_STYLE, ulValue);
     }


/*************************************************************************************
  SetStatusBarSplit: Where split the bar (if double).
*************************************************************************************/
void SetStatusBarSplit(PSTSTATUSBAR pstsb, int iSplit)
     {
     pstsb->iTwoSplit = iSplit;
     WinInvalidateRect(pstsb->hwndStatus, NULL, TRUE);
     }


/*************************************************************************************
  StatusBar3D: Draws 3D look in specified RECTL.
*************************************************************************************/
void StatusBar3D (HPS hps, RECTL rcl, int iType, BOOL bFill)
     {
     POINTL             pp;


     /* All around */
     if (iType == 1)
        GpiSetColor(hps, CLR_DARKGRAY);
     else
        GpiSetColor(hps, CLR_WHITE);

     pp.x = rcl.xLeft;
     pp.y = rcl.yTop - 1;
     GpiMove(hps, &pp);

     pp.x = rcl.xRight;
     pp.y = rcl.yBottom;
     GpiBox(hps, DRO_OUTLINE, &pp, 0, 0);


     /* Bottom/right */
     if (iType == 1)
        GpiSetColor(hps, CLR_WHITE);
     else
        GpiSetColor(hps, CLR_DARKGRAY);

     pp.x = rcl.xRight;
     pp.y = rcl.yBottom;
     GpiMove(hps, &pp);

     pp.x = rcl.xLeft + 1;
     pp.y = rcl.yTop - 2;
     GpiBox(hps, DRO_OUTLINE, &pp, 0, 0);


     /* Fill contents */
     if (bFill == TRUE)
        {
        GpiSetColor(hps, CLR_PALEGRAY);

        pp.x = rcl.xLeft + 1;
        pp.y = rcl.yBottom + 1;
        GpiMove(hps, &pp);

        pp.x = rcl.xRight - 1;
        pp.y = rcl.yTop - 2;
        GpiBox(hps, DRO_FILL, &pp, 0, 0);
        }
     }

/*************************************************************************************
  CreateHelp: Creates help instance.
*************************************************************************************/
HWND CreateHelp(PSTHELP psth, HWND hwndFrame, int iMainHelpPanelId, char *szWindowTitle, char *szLibraryName, char *szTutorial)
     {
     HWND         hwndHelp;
     HELPINIT     hini;


     psth->hwndOwner = hwndFrame;
     if (szLibraryName != NULL)
        strcpy(psth->szHelpLibrary, szLibraryName);
     else
        psth->szHelpLibrary[0] = '\0';

     if (szWindowTitle != NULL)
        strcpy(psth->szHelpWindowTitle, szWindowTitle);
     else
        psth->szHelpWindowTitle[0] = '\0';

     if (szTutorial != NULL)
        strcpy(psth->szTutorial, szTutorial);
     else
        psth->szTutorial[0] = '\0';

     hini.cb = sizeof(HELPINIT);
     hini.ulReturnCode = 0;
     hini.pszTutorialName = (PSZ) psth->szTutorial;
     hini.phtHelpTable = (PHELPTABLE) MAKELONG(iMainHelpPanelId, 0xFFFF);
     hini.hmodHelpTableModule = 0;
     hini.hmodAccelActionBarModule = 0;
     hini.idAccelTable = 0;
     hini.idActionBar = 0;
     hini.pszHelpWindowTitle = (PSZ)psth->szHelpWindowTitle;
     hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
     hini.pszHelpLibraryName = (PSZ)psth->szHelpLibrary;

     psth->hwndHelpInstance = WinCreateHelpInstance(WinQueryAnchorBlock(psth->hwndOwner), &hini);
     WinAssociateHelpInstance(psth->hwndHelpInstance, psth->hwndOwner);
     return(psth->hwndHelpInstance);
     }


/*************************************************************************************
  DestroyHelp: Destroys help instance.
*************************************************************************************/
BOOL DestroyHelp(PSTHELP psth)
     {
     if (psth->hwndHelpInstance != (HWND) NULL)
        {
        WinDestroyHelpInstance(psth->hwndHelpInstance);
        return (TRUE);
        }
     else
        return (FALSE);
     }


/*************************************************************************************
  DisplayHelp: Call this when you wish to display a help item.
*************************************************************************************/
BOOL DisplayHelp(PSTHELP psth, int iHelpId)
     {
     if (psth->hwndHelpInstance != NULLHANDLE)
        {
        WinSendMsg(psth->hwndHelpInstance, HM_DISPLAY_HELP, MPFROM2SHORT(iHelpId, NULL), MPFROMSHORT(HM_RESOURCEID));
        return (TRUE);
        }
     else
        return (FALSE);
     }


/*************************************************************************************
  DisplayHelpIndex: Call this when you wish to display the help index.
*************************************************************************************/
BOOL DisplayHelpIndex(PSTHELP psth)
     {
     if (psth->hwndHelpInstance != NULLHANDLE)
        {
        WinSendMsg(psth->hwndHelpInstance, HM_HELP_INDEX, NULL, NULL);
        return (TRUE);
        }
     else
        return (FALSE);
     }

/*************************************************************************************
  DisplayHelpContents: Call this when you wish to display the help index.
*************************************************************************************/
BOOL DisplayHelpContents(PSTHELP psth)
     {
     if (psth->hwndHelpInstance != NULLHANDLE)
        {
        WinSendMsg(psth->hwndHelpInstance, HM_HELP_CONTENTS, NULL, NULL);
        return (TRUE);
        }
     else
        return (FALSE);
     }


/*************************************************************************************
  SizeHelpWindow: Sets the size of the help window.
*************************************************************************************/
BOOL SizeHelpWindow(PSTHELP psth, LONG xLeft, LONG yBottom, LONG xRight, LONG yTop)
     {
     RECTL      rcl;


     rcl.xLeft = xLeft;
     rcl.yBottom = yBottom;
     rcl.xRight = xRight;
     rcl.yTop = yTop;
     if (psth->hwndHelpInstance != NULLHANDLE)
        {
        WinSendMsg(psth->hwndHelpInstance, HM_SET_COVERPAGE_SIZE, &rcl, NULL);
        return (TRUE);
        }
     else
        return (FALSE);
     }

/*-----------------------------------------------------------------------------
   ReadProfile: Reads settings from the application profile.
-----------------------------------------------------------------------------*/
BOOL ReadProfile (HAB hab, char* szFile, char* szApp, char* szKey, char* szItem, ULONG iItemSize)
     {
     ULONG          pulBufferMax;
     HINI           hIni;

     hIni = PrfOpenProfile(hab, szFile);

     if ( PrfQueryProfileData(hIni, (PSZ)szApp, (PSZ)szKey, (PVOID) szItem, (PULONG) &iItemSize) )
        {
        PrfCloseProfile(hIni);
        return (TRUE);
        }
     else
        return (FALSE);
     }


/*-----------------------------------------------------------------------------
   WriteProfile: Writes settings to the application profile.
-----------------------------------------------------------------------------*/
BOOL WriteProfile (HAB hab, char* szFile, char* szApp, char* szKey, char* szItem, ULONG iItemSize)
     {
     HINI               hIni;

     hIni = PrfOpenProfile(hab, szFile);
     if (hIni)
        {
        PrfWriteProfileData(hIni, (PSZ) szApp, (PSZ) szKey, (PVOID) szItem, iItemSize);
        PrfCloseProfile(hIni);
        return (TRUE);
        }
     else
        return (FALSE);
     }

/****************************************************************************
  PrtDevPrinterInfo: Returns info about default printer
****************************************************************************/
BOOL PrtDevPrinterInfo (PSZ szLogAddress, PSZ szDriverName, PSZ szDriver, PSZ szPort)
     {
     USHORT             usSize;
     CHAR               szWrk[256];
     CHAR               szTemp[256];
     PCH                pchScan;

     if ( (usSize = (SHORT)PrfQueryProfileString(HINI_PROFILE,
                                               (PSZ)"PM_SPOOLER",
                                               (PSZ)"PRINTER",
                                               NULL,
                                               (PSZ) szTemp,
                                               (LONG)sizeof(szTemp))) != 0)
        {
        szTemp[usSize-2] = 0;
        if (PrfQueryProfileString(HINI_PROFILE,
                                  (PSZ)"PM_SPOOLER_PRINTER",
                                  (PSZ)szTemp,
                                  NULL,
                                  (PSZ)szWrk,
                                  (LONG)sizeof(szWrk)) )
           {
           pchScan = strchr(szWrk, ';');
           pchScan++;
           pchScan = strchr(pchScan, ';');
           pchScan++;
           *(pchScan + strcspn(pchScan, ".,;")) = 0;
           strcpy(szLogAddress, pchScan);

           pchScan = strchr(szWrk, ';');
           pchScan++;
           *(pchScan + strcspn(pchScan, ";")) = 0;
           strcpy(szDriverName, pchScan);

           pchScan = strchr(szWrk, ';');
           pchScan++;
           *(pchScan + strcspn(pchScan, ".,;")) = 0;
           strcpy(szDriver, pchScan);

           pchScan = szWrk;
           *(pchScan + strcspn(pchScan, ".,;")) = 0;
           strcpy(szPort, pchScan);
           return (TRUE);
           }
        else
           return (FALSE);
        }
     else
        return (FALSE);
     }


/****************************************************************************
  PrtDevStartDoc: Starts a print job.
****************************************************************************/
HDC PrtDevStartDoc(HAB hab, PSZ szDriver, PSZ szLogAddress, PSZ szDocInfo)
     {
     HDC                hdcPrinter;
     DEVOPENSTRUC       DevData;

     memset(&DevData, 0, sizeof(DEVOPENSTRUC));
     DevData.pszLogAddress = szLogAddress;
     DevData.pszDriverName = szDriver;
     DevData.pszDataType = (PSZ)"PM_Q_STD";

     hdcPrinter = DevOpenDC(hab, OD_QUEUED,
                            (PSZ)"*", 4L, (PDEVOPENDATA)&DevData,
                            (HDC)NULL);

     DevEscape(hdcPrinter, DEVESC_STARTDOC,
               strlen(szDocInfo), szDocInfo, 0L, NULL);

     return (hdcPrinter);
     }


/****************************************************************************
  PrtDevClip: Returns the left/bottom margins (cliping) if any.
****************************************************************************/
void PrtDevClip(HDC hdcPrinter, PULONG pulBottomClip, PULONG pulLeftClip)
     {
     HCINFO             hc;
     ULONG              ulForms;
     ULONG              ulCounter;

     ulForms = DevQueryHardcopyCaps(hdcPrinter, 1L, 0L, &hc);

     if(ulForms == DQHC_ERROR)
       {
       *pulLeftClip   = 0;
       *pulBottomClip = 0;
       }
     else
       {
       for(ulCounter = 0; ulCounter <= (USHORT) ulForms; ulCounter++)
          {
          DevQueryHardcopyCaps(hdcPrinter, ulCounter, 1L, &hc);
          if (hc.flAttributes == HCAPS_CURRENT + HCAPS_SELECTABLE)
             break;
          }
       *pulLeftClip   = hc.xLeftClip;
       *pulBottomClip = hc.yBottomClip;
       }
     }


/****************************************************************************
  PrtDevCreatePS: Creates a presentation space.
****************************************************************************/
HPS PrtDevCreatePS(HAB hab, HDC hdcPrinter, ULONG ulPsUnit)
     {
     SIZEL              szl;

     szl.cx = 0;
     szl.cy = 0;

    return (GpiCreatePS(hab, hdcPrinter, &szl, ulPsUnit | GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC));
    }


/****************************************************************************
  PrtDevCharPos: Prints string.
****************************************************************************/
void PrtDevCharPos(HPS hps, ULONG ulyPos, ULONG ulxPos, PSZ szText)
     {
     POINTL             pointl;

     pointl.y = ulyPos;
     pointl.x = ulxPos;

     GpiMove(hps, &pointl);
     GpiCharString(hps, strlen(szText), szText);
     }


/****************************************************************************
  PrtDevCharAt: Prints string at logical char coordinates.
****************************************************************************/
void PrtDevCharAt(HPS hps, USHORT usRow, USHORT usCol, PSZ szText)
     {
     FONTMETRICS        fm;
     SIZEL              szl;
     POINTL             pointl;
     ULONG              yLen, xLen;


     GpiQueryFontMetrics(hps, sizeof(fm), &fm);
     yLen = fm.lMaxBaselineExt;
     xLen = fm.lAveCharWidth;

     GpiQueryPS(hps, &szl);

     pointl.y = szl.cy - ((usRow - 1) * yLen);
     pointl.x = (usCol - 1) * xLen;

     GpiMove(hps, &pointl);
     GpiCharString(hps, strlen(szText), szText);
     }

/****************************************************************************
  PrtDevNewPage: Tells GPI to do a formfeed.
****************************************************************************/
void PrtDevNewPage (HDC hdcPrinter)
     {
     DevEscape(hdcPrinter, DEVESC_NEWFRAME, 0L, (ULONG) NULL, 0L, (ULONG) NULL);
     }


/****************************************************************************
  PrtDevDestroyPS: Destroys a presentation space.
****************************************************************************/
void PrtDevDestroyPS (HPS hpsPrinter)
     {
     GpiAssociate(hpsPrinter, 0);
     GpiDestroyPS(hpsPrinter);
     }


/****************************************************************************
  PrtDevEndDoc: Ends a print job
****************************************************************************/
USHORT PrtDevEndDoc (HDC hdcPrinter)
     {
     CHAR               szJobId[1];
     LONG               lBufferLen = 1;

     DevEscape(hdcPrinter, DEVESC_ENDDOC, 0L, 0L, &lBufferLen, szJobId);
     DevCloseDC(hdcPrinter);
     return (0);
     }


/****************************************************************************
  PrtStart: Simplified start document.
****************************************************************************/
BOOL PrtStart(HAB hab, PSZ szDocInfo, ULONG ulPsUnit, PPMPRINTINFO pprt, PPRINTDEST ppDest)
     {
     HDC                hdcPrinter;
     DEVOPENSTRUC       DevData;
     PDEVOPENSTRUC      pDevData;

     if (ppDest == NULL)
        {
        PrtDevPrinterInfo(pprt->szLogAddress, pprt->szDriverName, pprt->szDriver, pprt->szPort);

        pprt->hdcPrinter = PrtDevStartDoc(hab, pprt->szDriver, pprt->szLogAddress, szDocInfo);
        }
     else
        {
        pDevData = (PDEVOPENSTRUC)ppDest->pdopData;
        sprintf(pprt->szLogAddress, pDevData->pszLogAddress);
        sprintf(pprt->szDriverName, pDevData->pszDriverName);

        pprt->hdcPrinter = DevOpenDC(hab, ppDest->lType, ppDest->pszToken, ppDest->lCount,
                                     ppDest->pdopData,(HDC)NULL);

        DevEscape(pprt->hdcPrinter, DEVESC_STARTDOC,
                  strlen(szDocInfo), szDocInfo, 0L, NULL);
        }

     if (ulPsUnit == 0)
        ulPsUnit = PU_LOMETRIC;

     pprt->hpsPrinter = PrtDevCreatePS(hab, pprt->hdcPrinter, ulPsUnit);

     /* Get forms information of default form */
     PrtForm(pprt);

     if (pprt->hdcPrinter == 0 || pprt->hpsPrinter == 0)
        return (FALSE);
     else
        return (TRUE);
     }


/****************************************************************************
  PrtForm: Get information of currently selected form.
****************************************************************************/
BOOL PrtForm(PPMPRINTINFO pprt)
     {
     HCINFO             hc;
     ULONG              ulForms;
     ULONG              ulCounter;

     ulForms = DevQueryHardcopyCaps(pprt->hdcPrinter, 1L, 0L, &hc);

     if (ulForms != DQHC_ERROR)
        {
        for(ulCounter = 0; ulCounter <= (USHORT) ulForms; ulCounter++)
           {
           DevQueryHardcopyCaps(pprt->hdcPrinter, ulCounter, 1L, &hc);
           if (hc.flAttributes == HCAPS_CURRENT + HCAPS_SELECTABLE)
              break;
           }
        sprintf(pprt->szFormName, hc.szFormname);
        pprt->lFormWidth = hc.cx;
        pprt->lFormHeight = hc.cy;
        pprt->xLeftClip = hc.xLeftClip;
        pprt->yBottomClip = hc.yBottomClip;
        pprt->xRightClip = hc.xRightClip;
        pprt->yTopClip = hc.yTopClip;
        pprt->lWidthPels = hc.xPels;
        pprt->lHeightPels = hc.yPels;
        return (TRUE);
        }
     else
        return (FALSE);
     }


/****************************************************************************
  PrtEnd: Ends a print job
****************************************************************************/
USHORT PrtEnd(PPMPRINTINFO pprt)
     {
     PrtDevDestroyPS (pprt->hpsPrinter);
     PrtDevEndDoc(pprt->hdcPrinter);
     return (0);
     }


/****************************************************************************
  PrtInfoOpen: Open printer DC and PS.
****************************************************************************/
BOOL PrtInfoOpen(HAB hab, ULONG ulPsUnit, PPMPRINTINFO pprt, PPRINTDEST ppDest)
     {
     DEVOPENSTRUC       DevData;
     PDEVOPENSTRUC      pDevData;



     if (ppDest == NULL)
        {
        PrtDevPrinterInfo(pprt->szLogAddress, pprt->szDriverName, pprt->szDriver, pprt->szPort);
        /* Prepare for DevOpenDC */
        memset(&DevData, 0, sizeof(DEVOPENSTRUC));
        DevData.pszLogAddress = pprt->szLogAddress;
        DevData.pszDriverName = pprt->szDriver;
        DevData.pszDataType = (PSZ)"PM_Q_STD";
        pprt->hdcPrinter = DevOpenDC(hab, OD_QUEUED,(PSZ)"*", 4L,
                                     (PDEVOPENDATA)&DevData,(HDC)NULL);
        }
     else
        {
        pDevData = (PDEVOPENSTRUC)ppDest->pdopData;
        sprintf(pprt->szLogAddress, pDevData->pszLogAddress);
        sprintf(pprt->szDriverName, pDevData->pszDriverName);

        pprt->hdcPrinter = DevOpenDC(hab, ppDest->lType, ppDest->pszToken, ppDest->lCount,
                                     ppDest->pdopData,(HDC)NULL);
        }

     if (ulPsUnit == 0)
        ulPsUnit = PU_LOMETRIC;

     pprt->hpsPrinter = PrtDevCreatePS(hab, pprt->hdcPrinter, ulPsUnit);

     /* Get forms information of default form */
     PrtForm(pprt);

     if (pprt->hdcPrinter == 0 || pprt->hpsPrinter == 0)
        return (FALSE);
     else
        return (TRUE);
     }


/****************************************************************************
  PrtInfoClose: Ends a DC and PS info.
****************************************************************************/
USHORT PrtInfoClose(PPMPRINTINFO pprt)
     {
     PrtDevDestroyPS (pprt->hpsPrinter);
     DevCloseDC(pprt->hdcPrinter);
     return (0);
     }


/****************************************************************************
   PrtGetQueueList: Gets list of queues.
****************************************************************************/
int PrtQueryQueues(void)
     {
     int                iReturned, iTotal, iNeeded;

     /* Get number of queues */
     SplEnumQueue(NULL, 3, NULL, 0, (unsigned long *)&iReturned, (unsigned long *)&iTotal, (unsigned long *)&iNeeded, NULL);
     return (iTotal);
     }


/****************************************************************************
   PrtGetQueues: Fill array with list of queues.
****************************************************************************/
BOOL PrtGetQueues(PPMQUEUEINFO pPmQueueArray, int iAntal)
     {
     int                iReturned, iTotal, iNeeded;
     int                iCounter;
     PPRQINFO3          pQueueInfo;


     /* Get number of queues */
     SplEnumQueue(NULL, 3, NULL, 0, (unsigned long *)&iReturned, (unsigned long *)&iTotal, (unsigned long *)&iNeeded, NULL);
     /* Return if changed */
     if (iTotal != iAntal)
        return (FALSE);

     /* Get queues */
     pQueueInfo = (PPRQINFO3) malloc(iNeeded);
     SplEnumQueue(NULL, 3, pQueueInfo, iNeeded, (unsigned long *)&iReturned, (unsigned long *)&iTotal, (unsigned long *)&iNeeded, NULL);

     /* Move data into PMQUEUEINFO array */
     for (iCounter = 0; iCounter < iReturned; iCounter++)
         {
         strcpy(pPmQueueArray[iCounter].szName, pQueueInfo[iCounter].pszName);
         strcpy(pPmQueueArray[iCounter].szComment, pQueueInfo[iCounter].pszComment);
         strcpy(pPmQueueArray[iCounter].szDriverName, pQueueInfo[iCounter].pszDriverName);
         strcpy(pPmQueueArray[iCounter].szPrinters, pQueueInfo[iCounter].pszPrinters);

         if (pQueueInfo[iCounter].pDriverData->cb < 1024)
            {
            memcpy(pPmQueueArray[iCounter].szDrivData, pQueueInfo[iCounter].pDriverData, pQueueInfo[iCounter].pDriverData->cb);
            pPmQueueArray[iCounter].iDrivDataLen = pQueueInfo[iCounter].pDriverData->cb;
            }
         else
            {
            memcpy(pPmQueueArray[iCounter].szDrivData, pQueueInfo[iCounter].pDriverData, 1024);
            pPmQueueArray[iCounter].iDrivDataLen = 1024;
            }

         pPmQueueArray[iCounter].bDefault = FALSE;
         if (pQueueInfo[iCounter].fsType & PRQ3_TYPE_APPDEFAULT)
            pPmQueueArray[iCounter].bDefault = TRUE;
         }
     free(pQueueInfo);
     return (TRUE);
     }


/****************************************************************************
   PrtQueryJobProperties: Allows user to set job properties.
****************************************************************************/
BOOL PrtQueryJobProperties(HAB hab, PPMQUEUEINFO pPmQueue)
     {
     char       szDriverName[128];
     char       szDeviceName[128];
     int        iPos;
     PSZ        pszTemp;


     /* Separate driver and device name
       ---------------------------------*/
     iPos = strcspn(pPmQueue->szDriverName, ".");
     if (iPos > 0)
        {
        strncpy(szDriverName, pPmQueue->szDriverName, iPos);
        szDriverName[iPos] = '\0';
        strcpy(szDeviceName, &pPmQueue->szDriverName[iPos + 1]);
        }
     else
        {
        strcpy(szDriverName, pPmQueue->szDriverName);
        *szDeviceName = '\0';
        }

     /* Get first printer for this queue
       ----------------------------------*/
     pszTemp = strchr(pPmQueue->szPrinters, ',');
     if (pszTemp != NULL)
        {
        *pszTemp = '\0' ;
        }

     /* Post the job properties dialog
       --------------------------------*/
     if (DevPostDeviceModes(hab, (PDRIVDATA) pPmQueue->szDrivData, szDriverName, szDeviceName, pPmQueue->szPrinters, DPDM_POSTJOBPROP) == DEV_OK)
        return (TRUE);
     else
        return(FALSE);
     }

//------------------------------------------------------------------------------
