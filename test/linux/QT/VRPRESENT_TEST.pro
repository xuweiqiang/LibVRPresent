#-------------------------------------------------
#
# Project created by QtCreator 2009-07-03T10:37:47
#
#-------------------------------------------------

TARGET = VRPRESENT_TEST
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../../inc ../../3rdparty/linux/libwin32/inc

LIBS += ../../lib/LibVrpresent.so
     +=../../3rdparty/linux/libwin32/inc/libWin32.a
