// ---------------------------------------------------------------------
//  STVIO.H: Definition file for constants and functions.       980326
// ---------------------------------------------------------------------

// Add includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// vio and kbd handles OS/2
//--------------------------
#define VIO_HANDLE       0
#define KBD_HANDLE       0

// Define special types
#ifndef BOOL
#define BOOL int
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

//     ****** Hook's for VioEditString/VioListBox ******
#define IDH_NULL        0
#define IDH_UCASE       -32768
#define IDH_DATE        -32767
#define IDH_TIME        -32766
#define IDH_NUM         -32765
#define IDH_PSW         -32764
#define IDH_INT         -32763


//     ****** defines for VioFormatString ******
#define IDFS_STATIC     0
#define IDFS_LINE       1
#define IDFS_FRAME      2
#define IDFS_STRING     3
#define IDFS_INTEGER    4
#define IDFS_FLOAT      5
#define IDFS_DATE       6
#define IDFS_TIME       7
#define IDFS_DROP       8
#define IDFS_BUTTON     9
#define IDFS_BOOL       10
#define IDFS_CHECK      11
#define IDFS_OPTION     12
#define IDFS_LISTBOX    13


//     ****** Keypress constants ******
#define KEY_ENTER       13
#define KEY_ESC         27
#define KEY_TAB         9
#define KEY_BTAB        -15
#define KEY_BKSP        8
#define KEY_SPACE       32

#define KEY_INS         -82
#define KEY_DEL         -83

#define KEY_HOME        -71
#define KEY_END         -79
#define KEY_PAGEUP      -73
#define KEY_PAGEDN      -81
#define KEY_UARROW      -72
#define KEY_DARROW      -80
#define KEY_LARROW      -75
#define KEY_RARROW      -77
#define KEY_NUM5        -76


#define KEY_CTRLHOME    -119
#define KEY_CTRLEND     -117
#define KEY_CTRLPGUP    -132
#define KEY_CTRLPGDN    -118
#define KEY_CTRLLARROW  -115
#define KEY_CTRLRARROW  -116

#define KEY_ALTA        -30
#define KEY_ALTB        -48
#define KEY_ALTC        -46
#define KEY_ALTD        -32
#define KEY_ALTE        -18
#define KEY_ALTF        -33
#define KEY_ALTG        -34
#define KEY_ALTH        -35
#define KEY_ALTI        -23
#define KEY_ALTJ        -36
#define KEY_ALTK        -37
#define KEY_ALTL        -38
#define KEY_ALTM        -50
#define KEY_ALTN        -49
#define KEY_ALTO        -24
#define KEY_ALTP        -25
#define KEY_ALTQ        -16
#define KEY_ALTR        -19
#define KEY_ALTS        -31
#define KEY_ALTT        -20
#define KEY_ALTU        -22
#define KEY_ALTV        -47
#define KEY_ALTW        -17
#define KEY_ALTX        -45
#define KEY_ALTY        -21
#define KEY_ALTZ        -44

#define KEY_ALT0        -129
#define KEY_ALT1        -120
#define KEY_ALT2        -121
#define KEY_ALT3        -122
#define KEY_ALT4        -123
#define KEY_ALT5        -124
#define KEY_ALT6        -125
#define KEY_ALT7        -126
#define KEY_ALT8        -127
#define KEY_ALT9        -128

#define KEY_F1          -59
#define KEY_F2          -60
#define KEY_F3          -61
#define KEY_F4          -62
#define KEY_F5          -63
#define KEY_F6          -64
#define KEY_F7          -65
#define KEY_F8          -66
#define KEY_F9          -67
#define KEY_F10         -68
#define KEY_F11         -133
#define KEY_F12         -134

#define KEY_SHFTF1      -84
#define KEY_SHFTF2      -85
#define KEY_SHFTF3      -86
#define KEY_SHFTF4      -87
#define KEY_SHFTF5      -88
#define KEY_SHFTF6      -89
#define KEY_SHFTF7      -90
#define KEY_SHFTF8      -91
#define KEY_SHFTF9      -92
#define KEY_SHFTF10     -93
#define KEY_SHFTF11     -135
#define KEY_SHFTF12     -136

#define KEY_CTRLF1      -94
#define KEY_CTRLF2      -95
#define KEY_CTRLF3      -96
#define KEY_CTRLF4      -97
#define KEY_CTRLF5      -98
#define KEY_CTRLF6      -99
#define KEY_CTRLF7      -100
#define KEY_CTRLF8      -101
#define KEY_CTRLF9      -102
#define KEY_CTRLF10     -103
#define KEY_CTRLF11     -137
#define KEY_CTRLF12     -138

#define KEY_ALTF1       -104
#define KEY_ALTF2       -105
#define KEY_ALTF3       -106
#define KEY_ALTF4       -107
#define KEY_ALTF5       -108
#define KEY_ALTF6       -109
#define KEY_ALTF7       -110
#define KEY_ALTF8       -111
#define KEY_ALTF9       -112
#define KEY_ALTF10      -113
#define KEY_ALTF11      -140
#define KEY_ALTF12      -141

//     ****** Color declaration ******
#define COLOR_BLACK     0
#define COLOR_BLUE      1
#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_MANGENTA  5
#define COLOR_BROWN     6
#define COLOR_WHITE     7
#define COLOR_GRAY      8
#define COLOR_BRBLUE    9
#define COLOR_BRGREEN   10
#define COLOR_BRCYAN    11
#define COLOR_BRRED     12
#define COLOR_PINK      13
#define COLOR_YELLOW    14
#define COLOR_BRWHITE   15


// ---------------------------------------------------------------------
//  Declaration of struct's.
// ---------------------------------------------------------------------
typedef struct _STVIO
        {
        int      iOrgMaxRows;
        int      iOrgMaxCols;
        int      iOrgCursorRow;
        int      iOrgCursorCol;
        BOOL      bOrgCursorVisible;
        char     *pchOrgScreen;

        int      iMaxRows;
        int      iMaxCols;

        BOOL      bFirstTime;
        BOOL      bUseColor;
        int      ibfcolor;
        int      ibbcolor;
        int      iwfcolor;
        int      iwbcolor;
        int      iffcolor;
        int      ifbcolor;
        int      iefcolor;
        int      iebcolor;
        int      iFrame;
        BOOL      bShade;
        BOOL      bZoom;
        int      iZoomTime;

        char     cDecimalDelim;
        char     cDateDelim;
        char     cTimeDelim;
        int      iZeroSupress;
        } STVIO;
typedef STVIO *PSTVIO;


typedef struct _STVIOWINDOW
        {
        PSTVIO          pstv;
        int             iRow1;
        int             iCol1;
        int             iRow2;
        int             iCol2;
        char            *wndBuffer;

        int             iOldRow;
        int             iOldCol;
        int             iOldCursorShow;

        int             iFrame;
        BOOL            bZoom;
        BOOL            bShade;

        int             ibfcolor;
        int             ibbcolor;
        int             iwfcolor;
        int             iwbcolor;
        int             iffcolor;
        int             ifbcolor;
        int             iefcolor;
        int             iebcolor;

        int             iWndRows;
        int             iWndCols;
        int             iCursorRow;
        int             iCursorCol;
        BOOL            bCursorShow;

        int             iProgressStart;
        int             iProgressStep;
        int             iProgressStop;
        } STVIOWINDOW;
typedef STVIOWINDOW *PSTVIOWINDOW;


typedef struct _STVIOLISTBOX
        {
        int      row1;
        int      col1;
        int      rowTo;
        int      colTo;
        int      startat;
        int      llen;
        BOOL     mark;
        int      curpos;
        int      lvisible;
        int      offset;
        int      oldoffset;
        int      curRow;
        int      redraw;
        } STVIOLISTBOX;
typedef STVIOLISTBOX *PSTVIOLISTBOX;


// ---------------------------------------------------------------------
//  Declaration of functions in STVIO
// ---------------------------------------------------------------------

// ****  WWINDOW.CPP  ****
PSTVIO VioInit (int iSetMaxRow, int iSetMaxCol);
void VioExit(PSTVIO pstv);
void VioSetGlobal(PSTVIO pstv, BOOL bUseColor, int iFrame, BOOL bShade, BOOL bZoom, int iZoomTime, int bf, int bb, int wf, int wb, int ff, int fb, int ef, int eb);

PSTVIOWINDOW VioOpenStandardWindow(PSTVIO pstv, int iRow1, int iCol1, int iRowLen, int iColLen, char *szRubrik);
PSTVIOWINDOW VioOpenWindow(PSTVIO pstv, int iRow1, int iCol1, int iRowLen, int iColLen, char *szRubrik, int iFrame, int iShade, int iZoom, int iZoomTime, int bf, int bb, int wf, int wb, int ff, int fb, int ef, int eb);
void VioCloseWindow (PSTVIOWINDOW hwnd);

int VioGetKey (PSTVIOWINDOW hwnd, BOOL bWaitForKey, BOOL bEcho);
int VioGetKeyBoardShift (PSTVIOWINDOW hwnd);
void VioSetCursor (PSTVIOWINDOW hwnd, int iRow, int iCol, int iShow);
void VioGetCursor (PSTVIOWINDOW hwnd, int *iRow, int *iCol, int *iShow);

// Do not use these
void ValidPos (PSTVIOWINDOW hwnd, int *iRow, int *iCol, int *iLen);
void ZoomWindow (PSTVIO pstv, int iRow1, int iCol1, int iRow2, int iCol2, int iForeColor, int iBackColor, int iFrame);

// ****  WDESKTOP.cpp  ****
void VioDesktop(PSTVIO pstv, char *szRubrik, int iFillChar, BOOL bStatus);

// ****  WPRINT.cpp  ****
void VioColorPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, int fc, int bc, char *szText);
void VioEditPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText);
void VioFieldPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText);
void VioFrame (PSTVIOWINDOW hwnd, char *szTitle, int iRow1, int iCol1, int iRowTo, int iColTo, int iFrame);
void VioPrint (PSTVIOWINDOW hwnd, int iRow, int iCol, char *szText);
void VioStatusPrint (PSTVIO pstv, int iCol, int iField, BOOL bInverse, char *szText);
char *VioFormatString(int iVarType, char *szFormatString, void *variable);

// ****  WMSGBOX.cpp  ****
int VioMessageBox (PSTVIO pstv, int iBoxType, char *szRubrik, char *szRad1, char *szRad2, char *szRad3);

// ****  WPERBOX.cpp  ****
PSTVIOWINDOW VioProgressBox (PSTVIO pstv, PSTVIOWINDOW hwnd, int iFlag, char *szRubrik, int iRow, int iCol, int iStart, int iStop);

// ****  WEDITBTN.cpp  ****
int VioEditButton(PSTVIOWINDOW hwnd, char *szText, int iRow, int iCol, int iRet, int aiRet[], int iRetLen);

// ****  WEDITSTR.cpp  ****
int VioEditString (PSTVIOWINDOW hwnd, char *szString, int iRow, int iColHome, int iLength, int aiRet[], int iRetLen, int iHook);

// ****  WEDITINT.cpp  ****
int VioEditInt (PSTVIOWINDOW hwnd, int *iValue, int iRow, int iCol, int iFormLen, int aiRet[], int iRetLen);

// ****  WEDTBOOL.cpp  ****
int VioEditBool(PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szSelect, int aiRet[], int iRetLen);
int VioEditCheck (PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szText, int aiRet[], int iRetLen);
int VioEditOption (PSTVIOWINDOW hwnd, int *iCurVal, int iRow, int iCol, char *szText, int aiRet[], int iRetLen);

// ****  WLISTBOX.cpp  ****
int VioListBox (PSTVIOWINDOW hwnd, PSTVIOLISTBOX pstl, int iState, char *szStrings[], int aiRet[], int iHook);

// void ValidPos (PSTVIOWINDOW hwnd, int iRow, int iCol, int iLen);
// void ZoomWindow (PSTVIO pstv, int iRow1, int iCol1, int iRow2, int iCol2, int iForeColor, int iBackColor, int iFrame);
int ESHook(int iHook, int iKey, int iCol, char *szString);



// VIO Functions (OS dependant)
//------------------------------
void VioGetInfo(int *iMaxRows, int *iMaxCols, int *iColors);
BOOL VioSetScreenSize(int iMaxRows, int iMaxCols);

void VioCls(int iForeColor, int iBackColor);

void VioColorPrintString (int iRow, int iCol, int iForeColor, int iBackColor, char *szString);
void VioPrintString (int iRow, int iCol, char *szString);

int VioQueryCursorRow(void);
int VioQueryCursorColumn(void);
void VioSetCursorPosition(int iRow, int iCol);

void VioShowCursor(BOOL bFlag);
BOOL VioQueryCursorVisible(void);

char *VioGetWindow(int iRow1, int iCol1, int iRow2, int iCol2);
void VioPutWindow(char *pbWindow, int iRow1, int iCol1, int iRow2, int iCol2);
void VioFrameFill(int iRow1, int iCol1, int iRow2, int iCol2, int iForeColor, int iBackColor, int iFrameType);

void VioFill(int iRow, int iCol, int iLen, int iForeColor, int iBackColor);
void VioFillChar(int iRow, int iCol, int iLen, char cChar);
void VioShade(int iRow1, int iCol1, int iRow2, int iCol2);

int VioInkey(BOOL bWaitFlag, int *iShiftstate);

int VioTimer(void);

int strcenter(int x1, int x2, char *szString);

USHORT VioColor(USHORT usFore, USHORT usBack);


//-------------------------------------------------------------------------------
