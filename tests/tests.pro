#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T17:19:52
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = tests
#CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../include

SOURCES += main.cpp \
    ../src/string_utils.cpp \
    StringUtilsTests.cpp

HEADERS += \
    ../include/string_utils.h \
    StringUtilsTests.h
