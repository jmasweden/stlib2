# makefile
# 
# Tools used:
#  Compile::Resource Compiler
#  Compile::GNU C
#  Make: nmake

COMP += gcc
CFLAGS += --no-warnings -Zomf -c -O2 -Wwrite-strings -fpermissive

all: std isam serial vio gui

std: st_std.obj

isam: st_isam.obj

serial: st_serial.obj

vio: st_vio.obj

gui: st_gui.obj

st_std.obj: st_std.cpp st_std.h
	$(COMP) $(CFLAGS) st_std.cpp -o st_std.obj

st_isam.obj: st_isam.cpp st_isam.h
	$(COMP) $(CFLAGS) st_isam.cpp -o st_isam.obj

st_serial.obj: st_serial.cpp st_serial.h
	$(COMP) $(CFLAGS) st_serial.cpp -o st_serial.obj

st_vio.obj: st_vio.cpp st_vio.h
	$(COMP) $(CFLAGS) st_vio.cpp -o st_vio.obj

st_gui.obj: st_gui.cpp st_gui.h
	$(COMP) $(CFLAGS) st_gui.cpp -o st_gui.obj

clean :
	del *obj
