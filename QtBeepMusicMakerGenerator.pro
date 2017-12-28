#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T18:19:55
#
#-------------------------------------------------

QT       += core gui

# CONFIG   += c++11

QMAKE_CXXFLAGS  +=   -std=c++11
QMAKE_LFLAGS    +=   -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBeepMusicMakerGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formnotenlaengeerfragen.cpp

HEADERS  += mainwindow.h \
    formnotenlaengeerfragen.h

FORMS    += mainwindow.ui \
    framenotenlaengeerfragen.ui
