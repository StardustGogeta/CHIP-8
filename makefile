#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall `sdl-config --cflags` -std=c99
RESINC = 
LIBDIR = 
LIB = -lSDL2 -lSDL2_mixer
LDFLAGS = `sdl2-config --libs`

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -DDEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj_linux/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/StarC8

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj_linux/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/StarC8

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/display.o $(OBJDIR_DEBUG)/src/main.o $(OBJDIR_DEBUG)/src/opcode.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/display.o $(OBJDIR_RELEASE)/src/main.o $(OBJDIR_RELEASE)/src/opcode.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/display.o: src/display.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/display.c -o $(OBJDIR_DEBUG)/src/display.o

$(OBJDIR_DEBUG)/src/main.o: src/main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/main.c -o $(OBJDIR_DEBUG)/src/main.o

$(OBJDIR_DEBUG)/src/opcode.o: src/opcode.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/opcode.c -o $(OBJDIR_DEBUG)/src/opcode.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/display.o: src/display.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/display.c -o $(OBJDIR_RELEASE)/src/display.o

$(OBJDIR_RELEASE)/src/main.o: src/main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/main.c -o $(OBJDIR_RELEASE)/src/main.o

$(OBJDIR_RELEASE)/src/opcode.o: src/opcode.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/opcode.c -o $(OBJDIR_RELEASE)/src/opcode.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

