#############################################################################
# Makefile for building: clemanglaise
# Generated by qmake (2.01a) (Qt 4.8.1) on: Sun Dec 23 13:14:00 2012
# Project:  clemanglaise.pro
# Template: app
# Command: /home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake -o Makefile clemanglaise.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/default -I. -I../../../../QtSDK/Desktop/Qt/4.8.1/gcc/include/QtCore -I../../../../QtSDK/Desktop/Qt/4.8.1/gcc/include/QtNetwork -I../../../../QtSDK/Desktop/Qt/4.8.1/gcc/include/QtGui -I../../../../QtSDK/Desktop/Qt/4.8.1/gcc/include -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -Wl,-rpath,/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib
LIBS          = $(SUBLIBS)  -L/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib -lQtGui -L/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib -L/usr/X11R6/lib -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = answerframe.cpp \
		main.cpp \
		questionframe.cpp \
		testframe.cpp \
		addframe.cpp \
		homewindow.cpp \
		languagebuttons.cpp \
		test.cpp \
		ampersand_escape.cpp \
		wordframe.cpp moc_questionframe.cpp \
		moc_testframe.cpp \
		moc_addframe.cpp \
		moc_homewindow.cpp \
		moc_languagebuttons.cpp \
		moc_wordframe.cpp
OBJECTS       = answerframe.o \
		main.o \
		questionframe.o \
		testframe.o \
		addframe.o \
		homewindow.o \
		languagebuttons.o \
		test.o \
		ampersand_escape.o \
		wordframe.o \
		moc_questionframe.o \
		moc_testframe.o \
		moc_addframe.o \
		moc_homewindow.o \
		moc_languagebuttons.o \
		moc_wordframe.o
DIST          = ../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/linux.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base-unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-base.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/qconfig.pri \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_functions.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_config.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/exclusive_builds.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_pre.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/release.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_post.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/gdb_dwarf_index.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/warn_on.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/thread.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/moc.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/resources.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/uic.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/yacc.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/lex.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/include_source_dir.prf \
		clemanglaise.pro
QMAKE_TARGET  = clemanglaise
DESTDIR       = 
TARGET        = clemanglaise

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: clemanglaise.pro  ../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/default/qmake.conf ../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/linux.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base-unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-base.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-unix.conf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/qconfig.pri \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_functions.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_config.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/exclusive_builds.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_pre.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/release.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_post.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/gdb_dwarf_index.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/warn_on.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/thread.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/moc.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/resources.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/uic.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/yacc.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/lex.prf \
		../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/include_source_dir.prf \
		/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtGui.prl \
		/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtCore.prl \
		/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtNetwork.prl
	$(QMAKE) -o Makefile clemanglaise.pro
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/unix.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/linux.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/gcc-base-unix.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-base.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/common/g++-unix.conf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/qconfig.pri:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/modules/qt_webkit_version.pri:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_functions.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt_config.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/exclusive_builds.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_pre.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/release.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/default_post.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/gdb_dwarf_index.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/warn_on.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/qt.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/unix/thread.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/moc.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/resources.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/uic.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/yacc.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/lex.prf:
../../../../QtSDK/Desktop/Qt/4.8.1/gcc/mkspecs/features/include_source_dir.prf:
/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtGui.prl:
/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtCore.prl:
/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/lib/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile clemanglaise.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/clemanglaise1.0.0 || $(MKDIR) .tmp/clemanglaise1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/clemanglaise1.0.0/ && $(COPY_FILE) --parents answerframe.h questionframe.h testframe.h addframe.h homewindow.h languagebuttons.h test.h ampersand_escape.h wordframe.h .tmp/clemanglaise1.0.0/ && $(COPY_FILE) --parents answerframe.cpp main.cpp questionframe.cpp testframe.cpp addframe.cpp homewindow.cpp languagebuttons.cpp test.cpp ampersand_escape.cpp wordframe.cpp .tmp/clemanglaise1.0.0/ && (cd `dirname .tmp/clemanglaise1.0.0` && $(TAR) clemanglaise1.0.0.tar clemanglaise1.0.0 && $(COMPRESS) clemanglaise1.0.0.tar) && $(MOVE) `dirname .tmp/clemanglaise1.0.0`/clemanglaise1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/clemanglaise1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_questionframe.cpp moc_testframe.cpp moc_addframe.cpp moc_homewindow.cpp moc_languagebuttons.cpp moc_wordframe.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_questionframe.cpp moc_testframe.cpp moc_addframe.cpp moc_homewindow.cpp moc_languagebuttons.cpp moc_wordframe.cpp
moc_questionframe.cpp: questionframe.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) questionframe.h -o moc_questionframe.cpp

moc_testframe.cpp: questionframe.h \
		answerframe.h \
		addframe.h \
		test.h \
		testframe.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) testframe.h -o moc_testframe.cpp

moc_addframe.cpp: test.h \
		addframe.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) addframe.h -o moc_addframe.cpp

moc_homewindow.cpp: testframe.h \
		questionframe.h \
		answerframe.h \
		addframe.h \
		test.h \
		languagebuttons.h \
		homewindow.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) homewindow.h -o moc_homewindow.cpp

moc_languagebuttons.cpp: test.h \
		languagebuttons.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) languagebuttons.h -o moc_languagebuttons.cpp

moc_wordframe.cpp: test.h \
		wordframe.h
	/home/vic/QtSDK/Desktop/Qt/4.8.1/gcc/bin/moc $(DEFINES) $(INCPATH) wordframe.h -o moc_wordframe.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

answerframe.o: answerframe.cpp answerframe.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o answerframe.o answerframe.cpp

main.o: main.cpp homewindow.h \
		testframe.h \
		questionframe.h \
		answerframe.h \
		addframe.h \
		test.h \
		languagebuttons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

questionframe.o: questionframe.cpp questionframe.h \
		ampersand_escape.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o questionframe.o questionframe.cpp

testframe.o: testframe.cpp testframe.h \
		questionframe.h \
		answerframe.h \
		addframe.h \
		test.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o testframe.o testframe.cpp

addframe.o: addframe.cpp addframe.h \
		test.h \
		ampersand_escape.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o addframe.o addframe.cpp

homewindow.o: homewindow.cpp homewindow.h \
		testframe.h \
		questionframe.h \
		answerframe.h \
		addframe.h \
		test.h \
		languagebuttons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o homewindow.o homewindow.cpp

languagebuttons.o: languagebuttons.cpp languagebuttons.h \
		test.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o languagebuttons.o languagebuttons.cpp

test.o: test.cpp test.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o test.o test.cpp

ampersand_escape.o: ampersand_escape.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ampersand_escape.o ampersand_escape.cpp

wordframe.o: wordframe.cpp wordframe.h \
		test.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wordframe.o wordframe.cpp

moc_questionframe.o: moc_questionframe.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_questionframe.o moc_questionframe.cpp

moc_testframe.o: moc_testframe.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_testframe.o moc_testframe.cpp

moc_addframe.o: moc_addframe.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_addframe.o moc_addframe.cpp

moc_homewindow.o: moc_homewindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_homewindow.o moc_homewindow.cpp

moc_languagebuttons.o: moc_languagebuttons.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_languagebuttons.o moc_languagebuttons.cpp

moc_wordframe.o: moc_wordframe.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_wordframe.o moc_wordframe.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

