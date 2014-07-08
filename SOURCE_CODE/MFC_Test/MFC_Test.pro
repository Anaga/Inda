#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T23:23:39
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_mfc_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_mfc_test.cpp \
    arduino/MFC_Sim/massflowcontroller.cpp
    
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \    
    arduino/MFC_Sim/massflowcontroller.h \
    arduino/MFC_Sim/MFC_Sim.ino
    
