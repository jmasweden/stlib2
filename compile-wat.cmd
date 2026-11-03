@echo off
rem =========================================================================
rem Compile script for Open Watcom C on OS/2 / ArcaOS
rem Run from the project root:  compile-wat.cmd
rem =========================================================================

SET WATCOM=C:\WATCOM

rem Watcom OS/2 tools are installed under binp (Open Watcom on OS/2)
SET PATH=%WATCOM%\binp;%PATH%

rem Auto-detect OS/2 Toolkit
SET OS2TK=C:\usr\include\OS2TK45

rem Set up environment for Open Watcom on OS/2
set PATH=%WATCOM%\binp;%WATCOM%\binw;%PATH%
set BEGINLIBPATH=%WATCOM%\binp\dll
set INCLUDE=%WATCOM%\h;%WATCOM%\h\os2;%OS2TK%\h;src;%INCLUDE%
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\os2;C:\USR\LIB;%LIB%
SET WIPFC=%WATCOM%\wipfc

rem Log file
set LOGFILE=compile-wat.log

rem Start logging - write header
echo ========================================== > %LOGFILE%
echo Build Log >> %LOGFILE%
echo Date: %DATE% Time: %TIME% >> %LOGFILE%
echo WATCOM=%WATCOM% >> %LOGFILE%
echo OS2TK=%OS2TK% >> %LOGFILE%
echo ========================================== >> %LOGFILE%
echo. >> %LOGFILE%

rem Also show on screen
echo Building with Open Watcom...
echo WATCOM=%WATCOM%
echo OS2TK=%OS2TK%
echo Log: %LOGFILE%
echo.

rem Clean first
echo [CLEAN] Running wmake clean...
wmake -f makefile.wat clean >> %LOGFILE% 2>&1

set FAILED=0
echo [BUILD] Running wmake all...
wmake -f makefile.wat all >> %LOGFILE% 2>&1
if errorlevel 1 set FAILED=1

rem Check result
if "%FAILED%"=="1" goto failed
    echo.
    echo BUILD OK
    echo BUILD OK >> %LOGFILE%
    exit 0
:failed
echo.
echo BUILD FAILED
echo BUILD FAILED >> %LOGFILE%
exit 1