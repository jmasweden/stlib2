# Makefile for stlib2 (Open Watcom C on OS/2 / ArcaOS)
# wmake 2.0.1 on ArcaOS - explicit per-file rules (no pattern rules)

# ============================================================================
# Configuration
# ============================================================================
NAME    = stlib2
SRCDIR  = src
BINDIR  = bin

# ============================================================================
# Tools
# ============================================================================
CC      = wcc386

# ============================================================================
# Flags
# ============================================================================
CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0 -bm
CFLAGS  = $(CFLAGS) -i=$(OS2TK)\h -i=$(SRCDIR)

# ============================================================================
# Source files
# ============================================================================
OBJS = $(BINDIR)\bootset.obj $(BINDIR)\bootset.obj $(BINDIR)\bootset.obj $(BINDIR)\bootset.obj

# ============================================================================
# Targets
# ============================================================================

all : $(BINDIR)\st_gui.obj $(BINDIR)\st_isam.obj $(BINDIR)\st_serial.obj $(BINDIR)\st_std.obj $(BINDIR)\st_vio.obj .SYMBOLIC

$(BINDIR) :
	@if not exist $(BINDIR) mkdir $(BINDIR)

# ============================================================================
#  objects
# ============================================================================

$(BINDIR)\st_gui.obj : $(SRCDIR)\st_gui.cpp $(SRCDIR)\st_gui.h $(BINDIR)
	@echo Compiling src\st_gui.cpp
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\st_gui.cpp

$(BINDIR)\st_isam.obj : $(SRCDIR)\st_isam.cpp $(SRCDIR)\st_isam.h $(BINDIR)
	@echo Compiling src\st_isam.cpp
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\st_isam.cpp

$(BINDIR)\st_serial.obj : $(SRCDIR)\st_serial.cpp $(SRCDIR)\st_serial.h $(BINDIR)
	@echo Compiling src\st_serial.cpp
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\st_serial.cpp

$(BINDIR)\st_std.obj : $(SRCDIR)\st_std.cpp $(SRCDIR)\st_std.h $(BINDIR)
	@echo Compiling src\st_std.cpp
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\st_std.cpp

$(BINDIR)\st_vio.obj : $(SRCDIR)\st_vio.cpp $(SRCDIR)\st_vio.h $(BINDIR)
	@echo Compiling src\st_vio.cpp
	@$(CC) $(CFLAGS) -fo=$@ $(SRCDIR)\st_vio.cpp

# ============================================================================
# Clean
# ============================================================================

clean : .SYMBOLIC
	@if exist $(BINDIR)\*.obj del $(BINDIR)\*.obj >nul
	@echo Clean complete