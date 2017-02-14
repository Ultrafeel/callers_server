#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I/usr/share/boost_extr/boost_1_63_0 -I../settings_read -I/usr/share/boost_extr/boost_1_63_0/boost
CFLAGS = -Wall -std=c++11 -fexceptions
RESINC = 
LIBDIR = -L/usr/share/boost_extr/boost_1_63_0/stage/lib -L../settings_read/bin/\$\(currconf\) -L../settings_read/bin/\$\(TARGET_NAME\)
LIB = 
LDFLAGS = -lboost_regex -lboost_system -lsettings_read -lboost_serialization -lboost_thread -lboost_chrono

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -std=c++11 -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/caller_server

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/caller_server

OBJ_DEBUG = $(OBJDIR_DEBUG)/call_executor.o $(OBJDIR_DEBUG)/src/caller_server.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/call_executor.o $(OBJDIR_RELEASE)/src/caller_server.o

all: before_build build_debug build_release after_build

clean: clean_debug clean_release

before_build: 
	echo "TARGET_OUTPUT_DIR = $(TARGET_OUTPUT_DIR)"
	echo "TARGET_NAME  = $(TARGET_NAME)"

after_build: 

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

build_debug: before_debug out_debug after_debug

debug: before_build build_debug after_build

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/call_executor.o: call_executor.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c call_executor.cpp -o $(OBJDIR_DEBUG)/call_executor.o

$(OBJDIR_DEBUG)/src/caller_server.o: src/caller_server.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/caller_server.cpp -o $(OBJDIR_DEBUG)/src/caller_server.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

build_release: before_release out_release after_release

release: before_build build_release after_build

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/call_executor.o: call_executor.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c call_executor.cpp -o $(OBJDIR_RELEASE)/call_executor.o

$(OBJDIR_RELEASE)/src/caller_server.o: src/caller_server.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/caller_server.cpp -o $(OBJDIR_RELEASE)/src/caller_server.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_build after_build before_debug after_debug clean_debug before_release after_release clean_release
