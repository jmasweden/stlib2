/**************************************************************
                                                        980718
   Include File: st_serial.h
   Description:  Helper functions for serial com.
**************************************************************/


/* SERIAL FUNCTIONS
***********************************/
typedef struct _SERIALSTRUCT
        {
        unsigned short   usPort;
        unsigned short   usBpsRate;
        unsigned short   usDataBit;
        unsigned short   usParity;
        unsigned short   usStopBit;
        unsigned short   usReadTimeOut;
        unsigned short   usWriteTimeOut;
        BOOL             bExitTask;

        char             acRecBuf[2000];
        int              iHead, iTail;

        HFILE            hSerial;
        TID              tidRecieveThread;
        unsigned long    hEvent; /* HEV */
        unsigned long    hMutex; /* HMTX */
        } SERIALSTRUCT;
typedef SERIALSTRUCT *PSERIALSTRUCT;


void SerialRecieveThread(PVOID pss);

int SerialInit(PSERIALSTRUCT pss);
int SerialExit(PSERIALSTRUCT pss);
int SerialSetSpeed(PSERIALSTRUCT pss, unsigned short usBpsRate);
int SerialSetTimeout(PSERIALSTRUCT pss, unsigned short usReadTimeOut, unsigned short usWriteTimeOut);
int SerialSetBits(PSERIALSTRUCT pss, USHORT usDataBit, USHORT usParity, USHORT usStopBit);
int SerialGetC(PSERIALSTRUCT pss, unsigned short usTimeout);
int SerialPutC(PSERIALSTRUCT pss, unsigned short usTimeout, char cSendChar);
int SerialRecieveString(PSERIALSTRUCT pss, char *szRcvString, int iMaxLen, unsigned short usTimeout);
int SerialSendString(PSERIALSTRUCT pss, unsigned short usTimeout, char *szSendString);
