                               S T L I B 2
                       --------------------------

                           October 29, 2026

-----------------------------------------------------------------------------
THIS CODE IS FREEWARE AND AS SUCH YOU ARE PERMITTED TO DO WHAT YOU WISH WITH
THEM THESE PROGRAMS ARE PROVIDED AS IS WITHOUT ANY WARRANTY, EXPRESSED OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO FITNESS FOR A PARTICULAR PURPOSE.
-----------------------------------------------------------------------------

What is BootSet ?
------------------
BootSet allow batchfiles (batchcommands) to be run during the OS/2 boot-
sequence. BootSet is normally run before the graphic part of OS/2 (PM/WPS)
is loaded and therefore allows you to perform tasks that is not possible when
PM/WPS is loaded, ie. replacing systemfiles like DLL-files, drivers and
config.sys.

Optionally you may use BootSet to select different startup configurations
using the reboot feature of BootSet.

BootSet is built to support the smallest OS/2 configurations. You do not
need WPS nor PM to install/modify/run BootSet. BootSet might not be a
flashy PM program but does its job using very little system resources.


Requirements:
--------------
BootSet requires IBM OS/2 2.0 or later.
It has been sucessfully tested on OS/2 2.11, OS/2 Warp 3.0, 
OS/2 Warp Connect and  OS/2 Warp 4.0/4.5 (Merlin).
Bootset will also run on OS/2 Warp Server and OS/2 Lan Server.

BootSet will NOT work on IBM OS/2 Warp 3 with Fixpack 17 till fixpack 21 due
to a bug in these fixpacks. 

To be able to reboot BootSet its also required that the DOS.SYS device driver 
is loaded. This is normally done when installing the DOS support in OS/2.

Note: Bootset does not use DOS, it just happens to use a driver that is 
also used by the DOS support.


Installation:
--------------

Automatic installation:-------------------------------------------
Copy all the files into a temporary directory.

Start the installation program (INSTALL.EXE)
Change the path (if needed) and press F2 to start the installation.
BootSet will now be installed.
If you selected to allow BootSet to change CONFIG.SYS the old CONFIG.SYS
file will be saved as CONFIG.BAK. If you did not allow BootSet to change
CONFIG.SYS the changes will be recorded in the CONFIG.NEW file.
CONFIG.SYS must be changed in order to work.


Manual installation:---------------------------------------------
Copy the files to a directory of your choice. You may install BootSet in any
directory that you wish. The only files needed is the .EXE file (BOOTSET.EXE)
and its .SET file.
The file BOOTSET.SET is an example of how a .SET file can look. 
We recommend you to edit the file before you use it.

You must now also add the following line in your CONFIG.SYS file:

CALL=C:\BOOTSET\BOOTSET.EXE C:\BOOTSET\BOOTSET.SET

Where the first statement after CALL= is the full path and name of the
executable file and the second statement is the full path and name of the
INI-file.

The position of this line in your CONFIG.SYS file is arbitary since OS/2 does
not load the lines one after one but runs them in its own specific order. 
The lines starting with CALL are among the last to run.


History:
--------------
Bootset 1.0 - May 9, 1997 (First release)

Bootset 1.10 - June 13, 1997
- Small waitline added, displays a one line timeout
- Color support added/enabled
- Default timeout changed to 10 seconds
- Readme file fixed

Bootset 1.20 - June 24, 1997
- Length of waitline fixed, (display bug)
- After waitline row with keys was invisible
- Some redraw problems were fixed (although almost invisible)
- Unregistered timeout changed fron 8 seconds to 6 seconds

Bootset 2.0 - July 29, 1998
- Screen layout reworked
- BOOTSET.INI editor was added
- Installation program added

Bootset 2.01 - October 12, 1998
- Bugfix (trap in unregistered version)
- Renamed the .INI file to .SET

Bootset 2.02 - October 25, 1998
- Now FreeCode

Bootset 2.02 - September 16, 2026
- Ported to gcc


Settings file:
----------------
This is a description of the settings file. You might want to use the 
BOOTSET.SET editor (BSEDIT.EXE) instead.
BSEDIT.EXE has full online help.

The settings file is build in a specific order. There is a example included in
this package (BOOTSET.SET). Please refer to it when reading this text.

The settings file contains of a fixed header and after that the menu items are
listed. Following here is a description of the example file (BOOTSET.SET).
Please note that all lines in the header must be present even if you do not use
them or they are not implemented in this version.

Line	Text		Explanation
====    ====        =============================================
01       1          Tiny waitrow (0=off, 1=on) Shows single line with timeout
02       7          Foreground color (0-15)
03       1          Background color (0-15)
04      10          Timeout in seconds, time Bootset will wait for keypress
05       0          For future enhancement
06       0          For future enhancement
07       0          For future enhancement
08       0          For future enhancement
09       0          For future enhancement
10       0          For future enhancement
11       4          Number of menu items (a maximum of 255)

The rest of the file contains of the sections (one for each menu item).
The number of sections must be that same as the number of menu items
specified above. Each section contains the menu text (shown in boot-
set menu), the number of lines with commands to be run when this menu-
item is selected, a reboot flag and the lines of commands.

The menuitem contains a minimum of 4 lines.

1 - Text to be shown in menu
2 - Reboot/Do not reboot after execution of commands
3 - Number of lines with commands
4 - 1st line with command to be executed
5 - 2nd...
6 - 3rd...
etc...


Line    Text                   Explanation
====    ====                   =============================================
12      Start cmd-line         Menu text.
13        0                    Reboot after this menuitem is run.
                               0=no reboot, 1=reboot
14        1                    Number of lines with commands(a maximum of 100).
15      CMD.EXE                Command to execute, maximum 255 characters.
16      Start with network     Menu text.
17        1                    Here we reboot after menu item is run
18        5                    Five lines with commands
19      COPY C:\SAVE\CONFIG.NET C:\CONFIG.SYS
20      COPY C:\SAVE\AUTOEXEC.NET C:\AUTOEXEC.BAT
21      COPY C:\SAVE\STARTUP.NET C:\STARTUP.CMD
22      COPY C:\SAVE\PROTOCOL.SAV C:\IBMCOM\PROTOCOL.INI
23      COPY C:\SAVE\IBMLAN.SAV C:\IBMLAN\PROTOCOL.INI
24      Start standalone       Menu text
25        1                    Here we reboot after menu item is run
26        3                    Three lines with commands
27      COPY C:\SAVE\CONFIG.STD C:\CONFIG.SYS
28      COPY C:\SAVE\AUTOEXEC.STD C:\AUTOEXEC.BAT
29      DEL C:\STARTUP.CMD
30      Backup .INI & desktop  Menu text
31        0                    No reboot
32       11                    Eleven lines with commands
33      ATTRIB -r -s C:\OS2\OS2*.INI
34      XCOPY C:\CONFIG.SYS C:\CFG\DESKTOP /V
35      XCOPY C:\AUTOEXEC.BAT C:\CFG\DESKTOP /V
36      XCOPY C:\OS2\*.INI C:\CFG\DESKTOP\OS2 /V
37      XCOPY C:\OS2\MDOS\WINOS2\*.INI C:\CFG\DESKTOP\WINOS2 /V
38      XCOPY C:\IBMCOM\PROTOCOL.INI C:\CFG\DESKTOP\NET /V
39      XCOPY C:\IBMLAN\IBMLAN.INI C:\CFG\DESKTOP\NET /V
40      XCOPY C:\Desktop C:\CFG\DESKTOP\Desktop /V /S /E
41      XCOPY "C:\Maintenance Desktop" "C:\CFG\DESKTOP\Maintenance Desktop" /V /S /E
42      XCOPY C:\Nowhere C:\CFG\DESKTOP\Nowhere /V /S /E
43      XCOPY C:\Nowhere1 C:\CFG\DESKTOP\Nowhere1 /V /S /E

-------------------------------------------------------------------------------
