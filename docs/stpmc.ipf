:userdoc.
:title. STLIBPMC
:docprof  toc=123456 .

.*****************************************************************************
:h1 id=ID@1000.About this book
:p.
:p.
:font facename='Times' size=24x24.(C) Copyright JMA Software
Technologies&period. 1987-1998&period.  All Rights Reserved&period.
:font facename='System Proportional' size=0x0.
:p.
:p.
:font facename='Times' size=24x24.This book describes the functions and features
in the STLIBPMC library&period. Its designed to be used with the IBM VisualAge
C++ compiler for OS/2&period.
:font facename='System Proportional' size=0x0.
:p.
:p.
:font facename='Times' size=24x24.Btrieve is Copyright of Btrieve
Technologies&period.
:font facename='System Proportional' size=0x0.
:p.

.*****************************************************************************
:h1 id=ID@1001.General information
:p.To use the STLIBPMC library you must include the STLIBPMC&period.H file in
your source &period.C file (note that this library does not support C++)&period.
In addition you must include the STPMC&period.LIB in the linker stage&period.
:p.
:p.To use the Btreive functions you must include the BTISAM&period.H file and
link with the BTRCALLS&period.LIB file&period. At run time the
BTRCALLS&period.DLL file must be in your LIBPATH&period.
:p.
:p.Please note that this library is compiled with the MT (multitread)
support&period. Therefore you must also link all source using tis library with
the MT support turned on&period.
:p.
:p.The BTRCALLS&period.LIB and BTRCALLS&period.DLL is availible from Btrieve
Technologies&period.
:p.
:p.

.*****************************************************************************
:h1 id=ID@1002.Date

:h2 id=ID@3.DateSerial
:p.
:hp1.Prototype&colon.
:ehp1.
:p.
:hp1.:ehp1.
int DateSerial(char *szDate);
:p.
:p.
:hp1.Parameters&colon.
:ehp1.
:p.
:hp1.:ehp1.
szDate, date in YYMMDD format
.br
returns, serial number corresponding to date
:p.
:p.
:hp1.Notes&colon.
:ehp1.
:p.
:hp1.:ehp1.
Converts date (in YYMMDD format) to serial number used when calculate days
between dates etc&period.

:h2 id=ID@4.SerialDate
:p.
:hp1.Prototype&colon.
:ehp1.
:p.
:hp1.:ehp1.
char *SerialDate(int iDateSerial);
:p.
:p.
:hp1.Parameters&colon.
:ehp1.
:p.
:hp1.:ehp1.
iDateSerial, serial number to be converted into string
.br
returns, date string (YYMMDD) corresponding to serial number
:p.
:p.
:hp1.Notes&colon.
:ehp1.
:p.
:hp1.:ehp1.
Converts serial number to date (in YYMMDD format)&period.
:p.

:h2 id=ID@5.DayOfWeek
:p.
:hp1.Prototype&colon.
:ehp1.
:p.
:hp1.:ehp1.
int DayOfWeek(char *szDate);
:p.
:p.
:hp1.Parameters&colon.
:ehp1.
:p.
:hp1.:ehp1.
szDate, date string (YYMMDD) to find out date from&period.
.br
returns, number stating day of week (0 = sunday, 1 = monday etc&period.)
:p.
:p.
:hp1.Notes&colon.
:ehp1.
:p.
:hp1.:ehp1.
Returns a number specifying the day of week corresponding to given date&period.
:p.

:h2 id=ID@6.DateToday
:p.
:hp1.Prototype&colon.
:ehp1.
:p.
:hp1.:ehp1.
char *DateToday(int iFlag);
:p.
:p.
:hp1.Parameters&colon.
:ehp1.
:p.
:hp1.:ehp1.
iFlag, 0=
.br
returns,
:p.
:p.
:hp1.Notes&colon.
:ehp1.
:p.
:hp1.:ehp1.
&period.
:p.

:h2 id=ID@7.MonthLen
:p.
:hp1.Prototype&colon.
:ehp1.
:p.
:hp1.:ehp1.
int MonthLen(int iYear, int iMonth);
:p.
:p.
:hp1.Parameters&colon.
:ehp1.
:p.
:hp1.:ehp1.
&period.
:p.
:hp1.Notes&colon.
:ehp1.
:p.
:hp1.:ehp1.
&period.
:p.

.*****************************************************************************
:h1 id=ID@1003.Array

:h2 id=ID@8.ArrayNew
:p.void *ArrayNew(unsigned int iItemLen, unsigned int iAntal);

:h2 id=ID@9.ArrayFree
:p.void ArrayFree(char *array);

:h2 id=ID@10.ArrayResize
:p.void *ArrayResize(char *array, unsigned int iItemLen, unsigned int iNewSize);

:h2 id=ID@11.ArrayInsert
:p.void *ArrayInsert(char *array, unsigned int iItemLen, unsigned int iMaxValue,
unsigned int iPosition);

:h2 id=ID@12.ArrayAppend
:p.void *ArrayAppend(char *array, unsigned int iItemLen, unsigned int
iMaxValue);

:h2 id=ID@13.ArrayDelete
:p.void *ArrayDelete(char *array, unsigned int iItemLen, unsigned int iMaxValue,
unsigned int iPosition);

:h2 id=ID@14.ArrayCopy
:p.void ArrayCopy(char *array, unsigned int iItemLen, unsigned int iFrom,
unsigned int iTo);

:h2 id=ID@15.ArraySwap
:p.void ArraySwap(char *array, unsigned int iItemLen, unsigned int iItem1,
unsigned int iItem2);

.*****************************************************************************
:h1 id=ID@1005.Window management

:h2 id=ID@16.SetWindowTitle
:p.void SetWindowTitle (HWND hwnd, PSZ szNewTitle);

:h2 id=ID@17.EnableWindowUpdate
:p.void EnableWindowUpdate(HWND hwnd, int iFlag);

:h2.StartProgram
:p.BOOL StartProgram(HWND hwndNotify, char *szTitle, char *szProg, char *szParam, char *szStartDir);

:h2 id=ID@23.WinPrint
:p.void WinPrint (HWND hwnd, HPS hps, USHORT usRow, USHORT usCol, PSZszMessage);

:h2.WinCenterWindow
:p.void WinCenterWindow(HWND hwnd);

:h2.WinSetDlgLen
:p.void WinSetDlgLen(HWND hwnd, int iItem, int iLen);

:h2.WinSelDlgEntry
:p.void WinSelDlgEntry(HWND hwnd, int iItem);

:h2.WinAddToTaskList
:p.LHANDLE WinAddToTaskList(HWND hwndFrame, char *szTitle);

:h2.WinRemoveFromTaskList
:p.void WinRemoveFromTaskList(HWND hwndFrame);

:h2.WinEmphaseDrag
:p.void WinEmphaseDrag (HWND hwnd, BOOL bFlag);

:h2.WinMaximizeWindow
:p.void WinMaximizeWindow (HWND hwnd);

:h2.WinMinimizeWindow
:p.void WinMinimizeWindow (HWND hwnd);

:h2.WinRestoreWindow
:p.void WinRestoreWindow (HWND hwnd);

.*****************************************************************************
:h1 id=ID@1004.Messageboxes

:h2.MSGBOX constants
:lines.
#define MSGBOX_OK       0
#define MSGBOX_YES      1
#define MSGBOX_NO       2
:elines.

:h2.MessageBox
:p.BOOL MessageBox (HWND hwnd, USHORT usBoxType, PSZ szTitle, PSZ szMessage);

:h2.DEBUG
:p.void DEBUG (PSZ szMessage);

:h2.EXTDEBUG
:p.void EXTDEBUG (HAB hab, ULONG ulRetCode, PSZ szMessage);

.*****************************************************************************
:h1 id=ID@21.Bitmaps

:h2.LoadBitmapFromFile
:p.HBITMAP LoadBitmapFromFile(HWND hwnd, char *szFileName);

.*****************************************************************************
:h1 id=ID@1006.Menues

:h2.MenuHwnd
:p.HWND MenuHwnd(HWND hwnd);

:h2.MenuEnableItem
:p.void MenuEnableItem (HWND hwndMenu, USHORT usMenuId, BOOL bEnabled);

:h2.MenuCheckItem
:p.void MenuCheckItem (HWND hwndMenu, USHORT usMenuId, BOOL bCheck);

:h2.MenuSetItemText
:p.void MenuSetItemText (HWND hwndMenu, USHORT usMenuId, PSZ szItemText);

:h2.MenuGetItemText
:p.PSZ MenuGetItemText (HWND hwndMenu, USHORT usMenuId);

:h2.MenuInsertItem
:p.void MenuInsertItem(HWND hwndMenu, USHORT usTopMenuId, USHORT usPosition, USHORT usNewMenuId, PSZ szItemText);

:h2.MenuRemoveItem
:p.void MenuRemoveItem(HWND hwndMenu, USHORT usMenuId);

:h2.ShowPopupMenu
:p.BOOL ShowPopupMenu (HWND hwndClient, ULONG ulMenuId);

.*****************************************************************************
:h1 id=ID@1007.Pointers

:h2 id=ID@40.IconSize
:p.void IconSize (PULONG pxIcon, PULONG pyIcon);

:h2 id=ID@41.PointerNormal
:p.void PointerNormal();

:h2 id=ID@42.PointerWait
:p.void PointerWait();

.*****************************************************************************
:h1 id=ID@1008.Clipboard

:h2 id=ID@44.ClipQueryText
:p.BOOL ClipQueryText(HAB hab);

:h2.ClipToText
:p.char* ClipToText(HWND hwnd);

:h2 id=ID@45.TextToClip
:p.void TextToClip(HAB hab, PSZ szText);

:h2 id=ID@46.ClipQueryBitmap
:p.BOOL ClipQueryBitmap(HAB hab);

:h2 id=ID@47.BitmapToClip
:p.BOOL BitmapToClip(HAB hab, HBITMAP hbm);

:h2 id=ID@48.ClipToBitmap
:p.HBITMAP ClipToBitmap(HAB hab);

.*****************************************************************************
:h1 id=ID@1009.Fonts

:h2 id=ID@50.QueryFontSize
:p.void QueryFontSize(HWND hwnd, PULONG pulFontWidth, PULONG pulFontHeight);

:h2 id=ID@51.SelectFont
:p.ULONG SelectFont(BOOL bFlag, HPS hps, char *szFontName, int iFontSize);

:h2 id=ID@52.ScaleFont
:p.BOOL ScaleFont (HPS hpsPrinter, USHORT usFontSize);

.*****************************************************************************
:h1 id=ID@1010.MLE

:h2 id=ID@54.MLEImport
:p.BOOL MLEImport(HWND hwndMLE, char *pszText, ULONG ulTextLength);

:h2 id=ID@55.MLEExportLength
:p.ULONG MLEExportLength(HWND hwndMLE);

:h2 id=ID@56.MLEExport
:p.BOOL MLEExport(HWND hwndMLE, char *pszText, ULONG ulTextLength);


.*****************************************************************************
:h1 id=ID@58.Memo

:h2._MEMOHEADER
:lines.
typedef struct _MEMOHEADER    /* memoheader */
        {
        char            szMemoFile[4];
        unsigned long   ulMemoId;
        unsigned long   ulMemoActive;
        char            szDate[6];
        } MEMOHEADER;

typedef MEMOHEADER *PMEMOHEADER;
:elines.

:h2._MEMOITEM
:lines.
typedef struct _MEMOITEM    /* memoitem */
        {
        unsigned long   ulMemoId;
        unsigned long   ulMemoLen;
        char            sMemoActive;
        char            szDate[6];
        unsigned long   ulNextMemoPos;
        } MEMOITEM;

typedef MEMOITEM *PMEMOITEM;
:elines.

:h2 id=ID@59.MemoInfo
:p.BOOL MemoInfo(char* szMemoFile, PMEMOHEADER pmh);

:h2 id=ID@60.MemoGet
:p.char* MemoGet(char* szMemoFile, unsigned long ulMemoPos);

:h2 id=ID@61.MemoGetId
:p.char* MemoGetId(char* szMemoFile, unsigned long ulMemoId);

:h2 id=ID@62.MemoQueryId
:p.int MemoQueryId(char* szMemoFile, unsigned long ulMemoPos);

:h2 id=ID@63.MemoQueryPos
:p.int MemoQueryPos(char* szMemoFile, unsigned long ulMemoId);

:h2 id=ID@64.MemoLen
:p.int MemoLen(char* szMemoFile, unsigned long ulMemoPos);

:h2 id=ID@65.MemoLenId
:p.int MemoLenId(char* szMemoFile, unsigned long ulMemoId);

:h2 id=ID@66.MemoPut
:p.int MemoPut(char* szMemoFile, unsigned long ulMemoSize, char* szMemo);

:h2 id=ID@67.MemoPutId
:p.int MemoPutId(char* szMemoFile, unsigned long ulMemoSize, char* szMemo);

:h2 id=ID@68.MemoDelete
:p.BOOL MemoDelete(char* szMemoFile, unsigned long ulMemoPos);

:h2 id=ID@69.MemoDeleteId
:p.BOOL MemoDeleteId(char* szMemoFile, unsigned long ulMemoId);

.*****************************************************************************
:h1 id=ID@1011.File

:h2.GetExeDir
:p.char* GetExeDir (void);

:h2.SetExeDirCurrent
:p.void SetExeDirCurrent (void);

:h2.FileGetPath
:p.char *FileGetPath (char* szFullFile);

:h2.FileGetFile
:p.char *FileGetFile (char* szFullFile);

:h2.FileGetExt
:p.char *FileGetExt (char* szFullFile);

:h2.FileGetFileExt
:p.char *FileGetFileExt (char* szFullFile);

:h2.FileSetDir
:p.void FileSetDir (char* szFullFile);

.*****************************************************************************
:h1 id=ID@74.STDLIB

:h2.strxtract
:p.char *strxtract(char *szString, int iPos1, int iPos2);

:h2.strpostrim
:p.char *strpostrim(char *szString, int iPosition);

:h2.fgetsz
:p.char* fgetsz(char *, int, FILE *);

:h2.strtrim
:p.void strtrim(char* szString);

:h2.strltrimn
:p.char *strltrimn(char *szString, int iPosition);

:h2.strrtrimn
:p.char *strrtrimn(char *szString, int iPosition);

.*****************************************************************************
:h1 id=ID@75.Profile

:h2.ReadProfile
:p.BOOL ReadProfile (HAB hab, char* szFile, char* szApp, char* szKey, char*szItem, ULONG iItemSize);

:h2.WriteProfile
:p.BOOL WriteProfile (HAB hab, char* szFile, char* szApp, char* szKey, char*szItem, ULONG iItemSize);

.*****************************************************************************
:h1 id=ID@76.Bars

:h2.STBORDER constants
:lines.
#define STBORDERSPACE     5
#define STBORDERWIDTH     1
:elines.

.*****************************************************************************
:h1 id=ID@77.Hoverbox

:h2.HOVER constants
:lines.
#define HOVER_LEFT      0
#define HOVER_CENTER    1
#define HOVER_RIGHT     2

#define IDT_HOVERTIMER  100
#define HOVER_TIMEOUT   200
:elines.

:h2._STHOVERBOX
:lines.
typedef struct _STHOVERBOX    /* hoverboxitem */
        {
        HWND            hwndParent;
        HWND            hwndHoverBox;
        HWND            hwndHoverBoxChild;

        char            szText[128];
        char            szFont[65];
        int             iColorText;
        int             iColorBack;
        int             iAlign;
        int             iAlignInt;
        int             ySize;
        int             xSize;
        BOOL            bMotion;

        ULONG           idTimer;
        HWND            hwndHit;
        int             iHitId;
        int             (*fnUserTest)(void);
        BOOL            bShow;
        } STHOVERBOX;

typedef STHOVERBOX *PSTHOVERBOX;
:elines.

:h2.CreateHoverBox
:p.HWND CreateHoverBox (HWND hwndParent, PSTHOVERBOX phb);

:h2.DestroyHoverBox
:p.BOOL DestroyHoverBox (PSTHOVERBOX phb);

:h2.SetHoverBoxFont
:p.void SetHoverBoxFont(PSTHOVERBOX phb, char* szFont);

:h2.SetHoverBoxColor
:p.void SetHoverBoxColor (PSTHOVERBOX phb, int iColorText, int iColorBack);

:h2.SetHoverBoxAlignment
:p.void SetHoverBoxAlignment (PSTHOVERBOX phb, int iAlign);

:h2.SetHoverBoxText
:p.void SetHoverBoxText (PSTHOVERBOX phb, char* szText);

:h2.SetHoverBoxMotion
:p.void SetHoverBoxMotion (PSTHOVERBOX phb, BOOL bMotion);

:h2.SetHoverCallBack
:p.void SetHoverCallBack (PSTHOVERBOX phb, int (*fnUserTest)(void));

:h2.ShowHoverBox
:p.void ShowHoverBox(PSTHOVERBOX phb, HWND hwndHit, int iHitId, BOOL bShow);


.*****************************************************************************
:h1 id=ID@78.Control window

:h2.HOVER Control window constants
:lines.
#define WM_CTRLMESSAGE           WM_USER + 1110L

#define IDSTCTRL_NONE             0
#define IDSTCTRL_TEXT            10
#define IDSTCTRL_BUTTON          11
#define IDSTCTRL_ENTRYFIELD      12
#define IDSTCTRL_CHECK           13
#define IDSTCTRL_RADIO           14
#define IDSTCTRL_COMBO           15
#define IDSTCTRL_BITMAP          30
#define IDSTCTRL_STATE           31
#define IDSTCTRL_MANYSTATE       32
#define IDSTCTRL_ANIMATE         50
#define IDSTCTRL_PROGRESS        51
#define IDSTCTRL_DATETIME        52
#define IDSTCTRL_PRINT           53
#define IDSTCTRL_OWNERDRAW       99

#define IDSTCTRL_EFFECT_2D        0
#define IDSTCTRL_EFFECT_RAISED    1
#define IDSTCTRL_EFFECT_SUNKEN    2

#define IDSTCTRL_SUB_NONE         0
#define IDSTCTRL_SUB_TEXT         1
#define IDSTCTRL_SUB_BITMAP       2
#define IDSTCTRL_SUB_HBITMAP      3
#define IDSTCTRL_SUB_COLOR        4
#define IDSTCTRL_SUB_OWNERDRAW   99
:elines.

:h2._STCONTROLITEM
:lines.
typedef struct _STCONTROLITEM    /* stcontrolitem */
        {
        int             iItemId;                /* Identification */
        int             iItemType;              /* Subtype */

        ULONG           ulResourceId;           /* Generic variables */
        int             hResHandle;
        char            szText[128];
        int             iForeColor;
        int             iBackColor;

        char            szFlyOverText[64];      /* Fly over text */

        } STCONTROLITEM;

typedef STCONTROLITEM *PSTCONTROLITEM;
:elines.

:h2._STCONTROL
:lines.
typedef struct _STCONTROL    /* stcontrol */
        {
        HWND            hwndOwner;              /* Window to recieve messages */
        HWND            hwndControl;            /* Window around or control */
        HWND            hwndIntControl;         /* Control window inside */

        int             iItemId;                /* Identification */
        int             iControlId;             /* PM control id, MUST be unique */

        int             iType;                  /* Type of control */
        int             iCreateFlags;           /* Control create flags */

        int             iItemCount;             /* Items array size for multi controls */


        int             iResourceId1;            /* Generic variables */
        int             iResourceId2;
        ULONG           hResHandle1;
        ULONG           hResHandle2;
        char            szText1[128];
        char            szText2[128];
        int             iCurrentItem;
        int             iForeColor;
        int             iBackColor;
        char            szFont[33];
        int             iEffect;

        BOOL            bUseHover;              /* Fly over text */
        STHOVERBOX      sthb;
        char            szFlyOverText1[64];
        char            szFlyOverText2[64];

        int             iTimerId;               /* Animation */
        int             iTimerTicks;

        int             iCurrent;               /* Progress bar */
        int             iMaximum;
        BOOL            bShowText;

        MPARAM          mp1;                    /* mp from wndproc */
        MPARAM          mp2;

        HPS             hpsPaint;               /* hps from WM_PAINT */

        PSTCONTROLITEM  pStCi;                  /* Multiitem control settings */
        } STCONTROL;

typedef STCONTROL *PSTCONTROL;
:elines.

:h2.CreateControl
:p.HWND CreateControl(HWND hwndOwner, PSTCONTROL pStCtrl, int iType, int iItemId, int iCtrlId, int iCreateFlags, BOOL bUseHover, int iItemCount);

:h2.DestroyControl
:p.BOOL DestroyControl(PSTCONTROL pStCtrl);

:h2.ControlItemRealloc
:p.void ControlItemRealloc(PSTCONTROL pStCtrl, int iItemCount);

:h2.ControlSetItem
:p.BOOL ControlSetItem(PSTCONTROL pStCtrl, int iItem, int iItemId, int iItemType, ULONG ulResId, char *szItemText, int iForeColor, int iBackColor, char *szFlyOverText);

:h2.ControlSetColor
:p.void ControlSetColor(PSTCONTROL pStCtrl, int iForeColor, int iBackColor);

:h2.ControlSetFont
:p.void ControlSetFont(PSTCONTROL pStCtrl, char *szFont);

:h2.ControlSetFlyOverText
:p.void ControlSetFlyOverText(PSTCONTROL pStCtrl, char *szFlyOverText1, char *szFlyOverText2);

:h2.ControlSetEffect
:p.void ControlSetEffect(PSTCONTROL pStCtrl, int iEffect);

:h2.ControlResize
:p.int ControlResize(PSTCONTROL pStCtrl, int yPos, int xPos, int ySize, int xSize);

:h2.ControlSetText
:p.void ControlSetText(PSTCONTROL pStCtrl, char *szText1, char *szText2);

:h2.ControlGetText
:p.char *ControlGetText(PSTCONTROL pStCtrl);

:h2.ControlClear
:p.void ControlClear(PSTCONTROL pStCtrl);

:h2.AnimateSetTimerTicks
:p.void AnimateSetTimerTicks(PSTCONTROL pStCtrl, int iTimer);

:h2.AnimateSetTimerId
:p.void AnimateSetTimerId(PSTCONTROL pStCtrl, int iTimerId);

:h2.AnimateStart
:p.BOOL AnimateStart(PSTCONTROL pStCtrl);

:h2.AnimateStop
:p.void AnimateStop(PSTCONTROL pStCtrl);


.*****************************************************************************
:h1 id=ID@1013.Splitbar

:h2._SPLITBAR
:lines.
typedef struct _SPLITBAR    /* splitbaritem */
        {
        HWND            hwndParent;
        HWND            hwndSplitBar;
        HWND            hwndSplitBarChild;
        BOOL            bDirection;
        } SPLITBAR;

typedef SPLITBAR *PSPLITBAR;
:elines.

:h2.SBP constants
:lines.
#define SBP_HORZ                FALSE
#define SBP_VERT                TRUE
#define WM_SPLITBARMOVED        WM_USER + 1010L
:elines.

:h2.CreateSplitBar
:p.HWND CreateSplitBar (HWND hwndParent, BOOL bDirection, PSPLITBAR psb);

:h2.DestroySplitBar
:p.BOOL DestroySplitBar (PSPLITBAR psb);

:h2.PositionSplitBar
ULONG PositionSplitBar (PSPLITBAR psb, int iBorderBL, int iBorderTR, ULONG ulSplitBarPos);

.*****************************************************************************
:h1 id=ID@1014.Buttonbar

:h2.Buttonbar constants
:lines.
#define YICO            24
#define XICO            24

#define BBP_LEFT        0
#define BBP_TOP         1

#define BBT_BITMAP      0x002
#define BBT_SPACE       0x007

#define BBS_ENABLED     0
#define BBS_HIGHLITED   1
#define BBS_DISABLED    2

#define WM_BUTTONBARSELECT  WM_USER + 1100L
:elines.

:h2._STBTNBARITEM
:lines.
typedef struct _STBTNBARITEM    /* stbtnbaritem */
        {
        int     iItemId;
        int     iItemType;
        USHORT  usState;
        ULONG   ulResId1;
        int     hResHandle1;
        char    szFlyoverText1[32];
        ULONG   ulResId2;
        int     hResHandle2;
        char    szFlyoverText2[32];
        } STBTNBARITEM;
typedef STBTNBARITEM FAR *PSTBTNBARITEM;
:elines.

:h2._STBTNBAR
:lines.
typedef struct _STBTNBAR    /* stbtnbar */
        {
        HWND            hwndOwner;
        HWND            hwndButtonBar;
        USHORT          usItemCount;
        ULONG           ulBarType;
        PSTBTNBARITEM   pBtnItems;
        } STBTNBAR;
typedef STBTNBAR *PSTBTNBAR;
:elines.

:h2.CreateButtonBar
:p.HWND CreateButtonBar(HWND hwndOwner, PSTBTNBAR pBtnBar, int iBarType, USHORT usItems);

:h2.DestroyButtonBar
:p.BOOL DestroyButtonBar(PSTBTNBAR pBtnBar);

:h2.SetButtonBarItem
:p.BOOL SetButtonBarItem(PSTBTNBAR pBtnBar, int iItem, int iItemId, int iItemType, ULONG ulResId1, ULONG ulResId2, USHORT usState, char *szFlyoverText1, char *szFlyoverText2);

:h2.DrawButtonBar
:p.void DrawButtonBar(PSTBTNBAR pBtnBar, int ySize);

:h2.SizeButtonBar
:p.ULONG SizeButtonBar(PSTBTNBAR pBtnBar, ULONG ulBtnLen);

:h2.HiliteButton
:p.BOOL HiliteButton(PSTBTNBAR pBtnBar, int iItem, BOOL bHilite);

:h2.EnableButton
:p.BOOL EnableButton(PSTBTNBAR pBtnBar, int iItem, BOOL bEnable);

:h2.IsButtonHilited
:p.BOOL IsButtonHilited(PSTBTNBAR pBtnBar, int iItem);

:h2.IsButtonEnabled
:p.BOOL IsButtonEnabled(PSTBTNBAR pBtnBar, int iItem);


.*****************************************************************************
:h1 id=ID@88.Status bar

:h2.Statusbar constants
:lines.
#define WM_STATUSREDRAW        WM_USER + 1022

#define STATUS_ONE      0
#define STATUS_TWO      1

#define STATUS_LEFT     0
#define STATUS_CENTER   1
#define STATUS_RIGHT    2

#define STATUS_TEXT     1
#define STATUS_OWNER    2
:elines.

:h2._STSTATUSBAR
:lines.
typedef struct _STSTATUSBAR    /* ststatusbar */
        {
        int             iStatusId;
        int             iType;
        BOOL            bIsReg;
        HWND            hwndOwner;
        HWND            hwndStatus;
        HWND            hwndChild1;
        HWND            hwndChild2;
        char            szFont[40];
        int             iType1;
        int             iType2;
        char            szText1[128];
        char            szText2[128];
        int             iColor1;
        int             iColor2;
        int             iAlign1;
        int             iAlign2;
        int             iAlignInt1;
        int             iAlignInt2;
        int             iTwoSplit;
        int             iItemToRedraw;
        HWND            hwndRedraw;
        HPS             hpsRedraw;
        } STSTATUSBAR;

typedef STSTATUSBAR *PSTSTATUSBAR;
:elines.

:h2.CreateStatusBar
:p.HWND CreateStatusBar (HWND hwnd, PSTSTATUSBAR pstsb, int iType, int iItem1Type, int iItem2Type);

:h2.DestroyStatusBar
:p.BOOL DestroyStatusBar (PSTSTATUSBAR pstsb);

:h2. UpdateStatusBarItem
:p.void UpdateStatusBarItem(PSTSTATUSBAR pstsb, int iItem);

:h2.SizeStatusBar
:p.int SizeStatusBar (PSTSTATUSBAR pstsb, int iMaxLen);

:h2.SetStatusBarSplit
:p.void SetStatusBarSplit(PSTSTATUSBAR pstsb, int iSplit);

:h2.SetStatusBarText
:p.void SetStatusBarText (PSTSTATUSBAR pstsb, char* szText1, char* szText2);

:h2.SetStatusBarColor
:p.void SetStatusBarColor (PSTSTATUSBAR pstsb, int iColor1, int iColor2);

:h2.SetStatusBarBackColor
:p.void SetStatusBarBackColor (PSTSTATUSBAR pstsb, int iColor1, int iColor2);

:h2.SetStatusBarFont
:p.void SetStatusBarFont (PSTSTATUSBAR pstsb, char* szFont);

:h2.SetStatusBarAlignment
:p.void SetStatusBarAlignment (PSTSTATUSBAR pstsb, int iAlign1, int iAlign2);

.*****************************************************************************
:h1 id=ID@89.Control bar


:h2.Contolbar constants
:lines.
#define WM_CONTROLBARMSG        WM_USER + 1020
:elines.

:h2._STCONTROLBAR
:lines.
typedef struct _STCONTROLBAR    /* stcontrolbar */
        {
        HWND            hwndParent;
        HWND            hwndControlBar;
        HWND            hwndControl;
        int             iControlId;
        int             iControlHeight;
        int             iControlWidth;
        int             iControlDecend;
        } STCONTROLBAR;

typedef STCONTROLBAR *PSTCONTROLBAR;
:elines.

:h2.CreateControlBar
:p.HWND CreateControlBar (HWND hwnd, PSTCONTROLBAR pcb, char* szType, int iStyles, int iId);

:h2.DestroyControlBar
:p.BOOL DestroyControlBar (PSTCONTROLBAR pcb);

:h2.SizeControlBar
:p.int SizeControlBar(PSTCONTROLBAR pcb, int iVertTop, int iMaxLen);

:h2.EnableControlBar
:p.void EnableControlBar(PSTCONTROLBAR pcb, BOOL bEnable);

:h2.SizeCtrlControlBar
:p.void SizeCtrlControlBar(PSTCONTROLBAR pcb, int iControlHeight, int iControlWidth, int iDecend);


.*****************************************************************************
:h1 id=ID@90.Container window


:h2.Contolbar constants
:lines.
#define WM_CONTAINERMSG         WM_USER + 1021

#define IDSTCNR_HBMFOLDER       0
#define IDSTCNR_HBMFILE         1
#define IDSTCNR_HBMOWNER1       2
#define IDSTCNR_HBMOWNER2       3
#define IDSTCNR_HBMOWNER3       4
#define IDSTCNR_HBMOWNER4       5
:elines.

:h2._STCONTAINER
:lines.
typedef struct _STCONTAINER
        {
        HWND                    hwndOwner;
        HWND                    hwndClient;
        HWND                    hwndCnr;
        int                     iIdCnr;
        HBITMAP                 hbmFolder;
        HBITMAP                 hbmFile;
        HBITMAP                 hbmXtra1;
        HBITMAP                 hbmXtra2;
        HBITMAP                 hbmXtra3;
        HBITMAP                 hbmXtra4;
        HBITMAP                 hbmPlus;
        HBITMAP                 hbmMinus;
        int                     iColorFore;
        int                     iColorBack;
        char                    szFont[64];
        int                     iItems;
        int                     iCount;
        int                     iExtra;
        PVOID                   prcNext;
        PVOID                   pList;
        } STCONTAINER;

typedef STCONTAINER *PSTCONTAINER;
:elines.

:h2.CreateContainer
:p.HWND CreateContainer(HWND hwndOwner, PSTCONTAINER pCnr, int iIdCnr, int iExtra, int iItems, int iCreateFlags);

:h2.DestroyContainer
:p.void DestroyContainer(PSTCONTAINER pCnr);

:h2.ContainerExpandTree
:p.BOOL ContainerExpandTree(PSTCONTAINER pCnr, PVOID pItem);

:h2.ContainerSelect
:p.BOOL ContainerSelect(PSTCONTAINER pCnr, PVOID pItem);

:h2.QueryContainerSelected
:p.PVOID QueryContainerSelected(PSTCONTAINER pCnr);

:h2.QueryContainerFolder
:p.BOOL QueryContainerFolder(PVOID prc);

:h2.ContainerQueryRecord
:p.PVOID ContainerQueryRecord(PSTCONTAINER pCnr, int iCommand, PVOID prcItem, PVOID prcParent);

:h2.InsertContainerFolder
:p.PVOID InsertContainerFolder(PSTCONTAINER pCnr, PVOID prcAfter, PVOID prcAskedParent);

:h2.InsertContainerFile
:p.PVOID InsertContainerFile(PSTCONTAINER pCnr, PVOID prcAfter, PVOID prcAskedParent);

:h2.DeleteContainerItem
:p.BOOL DeleteContainerItem(PSTCONTAINER pCnr, PVOID pItem);

:h2.EmptyContainer
:p.BOOL EmptyContainer(PSTCONTAINER pCnr);

:h2.SetContainerItem
:p.BOOL SetContainerItem(PSTCONTAINER pCnr, PVOID pItem, ULONG flRecordAttr, HBITMAP hbmShow, char *szItemText);

:h2.ContainerOpenEdit
:p.void ContainerOpenEdit(PSTCONTAINER pCnr, PVOID pItem);

:h2.QueryContainerColor
:p.void QueryContainerColor(PSTCONTAINER pCnr, int *iColorFore, int *iColorBack);

:h2.SetContainerColor
:p.void SetContainerColor(PSTCONTAINER pCnr, int iColorFore, int iColorBack);

:h2.QueryContainerFont
:p.void QueryContainerFont(PSTCONTAINER pCnr, char* szFont);

:h2.SetContainerFont
:p.void SetContainerFont(PSTCONTAINER pCnr, char* szFont);

:h2.SetContainerBitmap
:p.void SetContainerBitmap(PSTCONTAINER pCnr, ULONG ulFolderRes, ULONG ulFileRes, ULONG ulXtra1, ULONG ulXtra2, ULONG ulXtra3, ULONG ulXtra4);

:h2.SetContainerBitmapSize
:p.void SetContainerBitmapSize(PSTCONTAINER pCnr, int iSizeX, int iSizeY);

:h2.SetContainerTreeBitmap
:p.void SetContainerTreeBitmap(PSTCONTAINER pCnr, ULONG ulPlusRes, ULONG ulMinusRes);

:h2.SetContainerTreeBitmapSize
:p.void SetContainerTreeBitmapSize(PSTCONTAINER pCnr, int iSizeX, int iSizeY);

:h2.SetContainerWindowAttr
:p.void SetContainerWindowAttr(PSTCONTAINER pCnr, ULONG flWindowAttr);

:h2.QueryContainerTreeLineLen
:p.int QueryContainerTreeLineLen(PSTCONTAINER pCnr);

:h2. SetContainerTreeLineLen
:p.void SetContainerTreeLineLen(PSTCONTAINER pCnr, int iTreeLineLen);


.*****************************************************************************
:h1 id=ID@91.Help

:h2._STHELP
:lines.
typedef struct _STHELP    /* sthelp */
        {
        HWND            hwndOwner;
        HWND            hwndHelpInstance;

        char            szHelpLibrary[256];
        char            szHelpWindowTitle[128];
        char            szTutorial[256];

        } STHELP;

typedef STHELP *PSTHELP;
:elines.

:h2.CreateHelp
:p.HWND CreateHelp(PSTHELP psth, HWND hwndFrame, int iMainHelpPanelId, char *szWindowTitle, char *szLibraryName, char *szTutorial);

:h2.DestroyHelp
:p.BOOL DestroyHelp(PSTHELP psth);

:h2.DisplayHelp
:p.BOOL DisplayHelp(PSTHELP psth, int iHelpId);

:h2.DisplayHelpIndex
:p.BOOL DisplayHelpIndex(PSTHELP psth);

:h2.DisplayHelpContents
:p.BOOL DisplayHelpContents(PSTHELP psth);

:h2.SizeHelpWindow
:p.BOOL SizeHelpWindow(PSTHELP psth, LONG xLeft, LONG yBottom, LONG xRight, LONG yTop);


.*****************************************************************************
:h1 id=ID@1015.Print

:h2._PMPRINTINFO
:lines.
typedef struct _PMPRINTINFO    /* pmprintinfo */
{
   CHAR         szLogAddress[256];
   CHAR         szPort[256];
   CHAR         szDriver[256];
   CHAR         szDriverName[256];
   HDC          hdcPrinter;
   HPS          hpsPrinter;
   CHAR         szFormName[32];
   LONG         lFormWidth;
   LONG         lFormHeight;
   LONG         xLeftClip;
   LONG         yBottomClip;
   LONG         xRightClip;
   LONG         yTopClip;
   LONG         lWidthPels;
   LONG         lHeightPels;

} PMPRINTINFO;
typedef PMPRINTINFO FAR *PPMPRINTINFO;
:elines.

:h2._PMQUEUEINFO
:lines.
typedef struct _PMQUEUEINFO    /* pmqueueinfo */
{
   char         szName[128];
   char         szComment[128];
   char         szDriverName[128];
   char         szPrinters[128];
   char         szDrivData[1024];
   int          iDrivDataLen;
   BOOL         bDefault;
} PMQUEUEINFO;
typedef PMQUEUEINFO FAR *PPMQUEUEINFO;
:elines.

:h2.PrtStart
BOOL PrtStart(HAB hab, PSZ szDocInfo, ULONG ulPsUnit, PPMPRINTINFO pprt, PPRINTDEST ppDest);

:h2.PrtForm
:p.BOOL PrtForm(PPRINTERINFO pprt);

:h2.PrtEnd
:p.USHORT PrtEnd(PPRINTERINFO pprt);

:h2.PrtInfoOpen
:p.BOOL PrtInfoOpen(HAB hab, ULONG ulPsUnit, PPRINTERINFO pprt, PPRINTDEST ppDest);

:h2 id=ID@93.PrtInfoClose
:p.USHORT PrtInfoClose(PPRINTERINFO pprt);

:h2 id=ID@94.PrtDevPrinterInfo
:p.BOOL PrtDevPrinterInfo (PSZ szLogAddress, PSZ szDriverName, PSZ szDriver, PSZ szPort);

:h2 id=ID@96.PrtDevStartDoc
:p.HDC PrtDevStartDoc(HAB hab, PSZ szDriver, PSZ szLogAddress, PSZ szDocInfo);

:h2 id=ID@97.PrtDevClip
:p.void PrtDevClip(HDC hdcPrinter, PULONG pulBottomClip, PULONG pulLeftClip);

:h2 id=ID@98.PrtDevCreatePS
:p.HPS PrtDevCreatePS(HAB hab, HDC hdcPrinter, ULONG ulPsUnit);

:h2 id=ID@99.PrtDevCharPos
:p.void PrtDevCharPos(HPS hps, ULONG ulyPos, ULONG ulxPos, PSZ szText);

:h2 id=ID@100.PrtDevCharAt
:p.void PrtDevCharAt(HPS hps, USHORT usRow, USHORT usCol, PSZ szText);

:h2 id=ID@101.PrtDevNewPage
:p.void PrtDevNewPage (HDC hdcPrinter);

:h2 id=ID@102.PrtDevDestroyPS
:p.void PrtDevDestroyPS (HPS hpsPrinter);

:h2 id=ID@103.PrtDevEndDoc
:p.USHORT PrtDevEndDoc (HDC hdcPrinter);

:h2.PrtQueryQueues
:p.int PrtQueryQueues(void);

:h2.PrtGetQueues
:p.BOOL PrtGetQueues(PPMQUEUEINFO pPmQueueArray, int iAntal);

:h2.PrtQueryJobProperties
:p.BOOL PrtQueryJobProperties(HAB hab, PPMQUEUEINFO pPmQueue);

.*****************************************************************************
:h1 id=ID@1016.Btrieve (lowlevel)

:h2.BTRV
:p.unsigned short BTRV(int operation, char *posblock, char *databuf, int *datalen, char *keybuf, int keynum);

:h2.BTRINIT
:p.unsigned short BTRINIT(char *szInitParam);

:h2.BTREXIT
:p.unsigned short BTREXIT(void);

.*****************************************************************************
:h1 id=ID@1017.Btrieve (highlevel)

:h2._BTFILE
:lines.
typedef struct _BTFILE    /* btfile */
{
   int          iCurKey;
   char         bPosBlock[128];
   int          iStatus;
} BTFILE;
typedef BTFILE FAR *PBTFILE;
:elines.

:h2._BTCFILESPEC
:lines.
typedef struct _BTCFILESPEC    /* btcfilespec */
{
   short int    sRecordLen;
   short int    sPageSize;
   short int    sIndexCount;
   long  int    lRecCount;
   short int    sFileFlag;
   short int    sResWord;
   short int    sAllocation;
} BTCFILESPEC;
typedef BTCFILESPEC FAR *PBTCFILESPEC;
:elines.

:h2._BTCKEYSPEC
:lines.
typedef struct _BTCKEYSPEC    /* btcfilespec */
{
   short        sKeyPos;
   short        sKeyLen;
   short        sKeyFlags;
   long int     lKeyCount;
   char         bExtKey;
   char         bNullValue;
   char         bReserved[3];

 BTCKEYSPEC;
typedef BTCKEYSPEC FAR *PBTCKEYSPEC;
:elines.

:h2.Defines for BtCreate (file/key flags)
:lines.
#define BT_CREATE_VARLEN      1
#define BT_CREATE_BLANK       2
#define BT_CREATE_PREALLOC    4
#define BT_CREATE_COMPRESS    8
#define BT_CREATE_KEYONLY     16
#define BT_CREATE_FREE10      64
#define BT_CREATE_FREE20      128
#define BT_CREATE_FREE30      192
#define BT_CREATE_DUPLICAT    1
#define BT_CREATE_MODIFY      2
#define BT_CREATE_BINARY      4
#define BT_CREATE_NULL        8
#define BT_CREATE_SEGMENT     16
#define BT_CREATE_ALTCOL      32
#define BT_CREATE_DECENDING   64
#define BT_CREATE_EXTENDED    256
#define BT_CREATE_MANUAL      512
:elines.

:h2.Defines for BtOpen (iMode)
:lines.
#define BT_OPEN_NORMAL       0
#define BT_OPEN_READONLY    -2
#define BT_OPEN_EXCLUSIVE   -4
:elines.

:h2.Defines for BtXxxx functions
:lines.
#define BT_OP_OPEN                  0
#define BT_OP_CLOSE                 1
#define BT_OP_INSERT                2
#define BT_OP_UPDATE                3
#define BT_OP_DELETE                4
#define BT_OP_GETEQUAL              5
#define BT_OP_GETNEXT               6
#define BT_OP_GETPREVIOUS           7
#define BT_OP_GETGREATER            8
#define BT_OP_GETGREATEROREQUAL     9
#define BT_OP_GETLESSTHAN           10
#define BT_OP_GETLESSTHANOREQUAL    11
#define BT_OP_GETFIRST              12
#define BT_OP_GETLAST               13
#define BT_OP_CREATE                14
#define BT_OP_STAT                  15
#define BT_OP_UNLOCK                27
#define BT_OP_RESET                 28
#define BT_OP_TRANSBEGIN            19
#define BT_OP_TRANSABORT            21
#define BT_OP_TRANSEND              20
#define BT_OP_SETOWNER              29
#define BT_OP_CLEAROWNER            30
:elines.

:h2.Defines for Btrieve errors
:lines.
#define BT_ERR_NONE                 0
#define BT_ERR_INVALIDOP            1
#define BT_ERR_IO                   2
#define BT_ERR_FILENOTOPEN          3
#define BT_ERR_KEYVALNOTFOUND       4
#define BT_ERR_DUPLKEYVAL           5
#define BT_ERR_INVALIDKEYNR         6
#define BT_ERR_DIFFKEYNR            7
#define BT_ERR_INVALIDPOS           8
#define BT_ERR_EOF                  9
#define BT_ERR_MODIFYKEY            10
#define BT_ERR_INVALIDFILENAME      11
#define BT_ERR_FILENOTFOUND         12
#define BT_ERR_EXTENDFILE           13
#define BT_ERR_PREIMAGEOPEN         14
#define BT_ERR_PREIMAGEIO           15
#define BT_ERR_EXPANSION            16
#define BT_ERR_CLOSE                17
#define BT_ERR_DISKFULL             18
#define BT_ERR_UNRECOVERABLE        19
#define BT_ERR_INACTIVE             20
#define BT_ERR_KEYBUFTOSHORT        21
#define BT_ERR_DATABUFLEN           22
:elines.

:h2.BtInit
:p.int BtInit(int iFlag, char * szInitString);

:h2.BtReset
:p.int BtReset(PBTFILE bt, long lConnectId);

:h2.BtCreate
:p.int BtCreate(char * szFileName, PBTCFILESPEC pbtcf, PBTCKEYSPEC pbtck, char *szAltCol, PBTFILE bt);

:h2.BtOpen
:p.int BtOpen(char * szFileName, char * szOwner, int iMode, PBTFILE bt);

:h2.BtClose
:p.int BtClose(PBTFILE bt);

:h2.BtGetFirst
:p.int BtGetFirst(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);

:h2.BtGetPrevious
:p.int BtGetPrevious(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);

:h2.BtGetNext
:p.int BtGetNext(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);

:h2.BtGetLast
:p.int BtGetLast(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);

:h2.BtGetEqual
:p.int BtGetEqual(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);

:h2.BtGetGE
:p.int BtGetGE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);

:h2.BtGetGreater
:p.int BtGetGreater(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);

:h2.BtGetLE
:p.int BtGetLE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int
iLock);

:h2.BtGetLess
:p.int BtGetLess(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);

:h2.BtInsert
:p.int BtInsert(PBTFILE bt, char * pbRecord, int iRecordLen);

:h2.BtDelete
:p.int BtDelete(PBTFILE bt);

:h2.BtUpdate
:p.int BtUpdate(PBTFILE bt, char * pbRecord, int iRecordLen);

:h2.BtUnlock
:p.int BtUnlock(PBTFILE bt);

:h2.BtLof
:p.int BtLof(PBTFILE bt);

:h2.BtBof
:p.int BtBof(PBTFILE bt);

:h2.BtEof
:p.int BtEof(PBTFILE bt);

:h2.BtSetOwner
:p.int BtSetOwner(PBTFILE bt, char * szOwner, int iRestrict);

:h2.BtClearOwner
:p.int BtClearOwner(PBTFILE bt);

:h2.BtTransBegin
:p.int BtTransBegin(PBTFILE bt);

:h2.BtTransAbort
:p.int BtTransAbort(PBTFILE bt);

:h2.BtTransEnd
:p.int BtTransEnd(PBTFILE bt);

:euserdoc.