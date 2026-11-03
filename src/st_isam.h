/**************************************************************
                                                        980718
   Include File: st_isam.h
   Description:  Helper functions for prot mode Btrieve access.
**************************************************************/

/* Declare DLL entrypoint */
unsigned short _Far16 _Pascal btrcall(short operation, char * posblock, char * databuf,
                                      short * datalen, char * keybuf, short keylen, char ckeynum);

unsigned short _Far16 _Pascal BTRVINIT(char * szInitParam);

unsigned short _Far16 _Pascal BTRVSTOP(void);


/* BTRIEVE HELPERS STRUCTURES.
***********************************/
typedef struct _BTFILE    /* btfile */
{
   int          iCurKey;
   char         bPosBlock[128];
   int          iStatus;
} BTFILE;
typedef BTFILE FAR *PBTFILE;

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


typedef struct _BTCKEYSPEC    /* btcfilespec */
{
   short        sKeyPos;
   short        sKeyLen;
   short        sKeyFlags;
   long int     lKeyCount;
   char         bExtKey;
   char         bNullValue;
   char         bReserved[3];
} BTCKEYSPEC;
typedef BTCKEYSPEC FAR *PBTCKEYSPEC;


/* BTRIEVE HELPERS DECLARATIONS.
***********************************/
int BtInit(int iFlag, char * szInitString);
int BtReset(PBTFILE bt, long lConnectId);

int BtCreate(char * szFileName, PBTCFILESPEC pbtcf, PBTCKEYSPEC pbtck, char * szAltCol, PBTFILE bt);
int BtOpen(char * szFileName, char * szOwner, int iMode, PBTFILE bt);
int BtClose(PBTFILE bt);

int BtGetFirst(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);
int BtGetPrevious(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);
int BtGetNext(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);
int BtGetLast(PBTFILE bt, char * pbRecord, int iRecordLen, int iLock);

int BtGetEqual(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);
int BtGetGE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);
int BtGetGreater(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);
int BtGetLE(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);
int BtGetLess(PBTFILE bt, char * pbRecord, int iRecordLen, char * szKey, int iLock);

int BtInsert(PBTFILE bt, char * pbRecord, int iRecordLen);
int BtDelete(PBTFILE bt);
int BtUpdate(PBTFILE bt, char * pbRecord, int iRecordLen);
int BtUnlock(PBTFILE bt);

int BtLof(PBTFILE bt);
int BtBof(PBTFILE bt);
int BtEof(PBTFILE bt);

int BtSetOwner(PBTFILE bt, char * szOwner, int iRestrict);
int BtClearOwner(PBTFILE bt);

int BtTransBegin(PBTFILE bt);
int BtTransAbort(PBTFILE bt);
int BtTransEnd(PBTFILE bt);


/* Defines for BtCreate (file/key flags).
*******************************************/
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


/* Defines for BtOpen (iMode).
***********************************/
#define BT_OPEN_NORMAL       0
#define BT_OPEN_READONLY    -2
#define BT_OPEN_EXCLUSIVE   -4


/* Defines for BtXxxx functions.
***********************************/
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

/* Defines for Btrieve errors
***********************************/
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


/* BTRIEVE FUNCTIONS
***********************************/
unsigned short BTRV(int operation, char *posblock, char *databuf, int *datalen, char *keybuf, int keynum);
unsigned short BTRINIT(char *szInitParam);
unsigned short BTREXIT(void);


