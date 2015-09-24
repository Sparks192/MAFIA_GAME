#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T21:16:48
#
#-------------------------------------------------

QT       += core gui network
CONFIG += static
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MafiaGame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mafiaclient.cpp \
    mafiaserver.cpp \
    playermodel.cpp \
    gamecontroller.cpp \
    gamemodel.cpp \
    message.cpp \
    npccontainer.cpp

HEADERS  += mainwindow.h \
    mafiaclient.h \
    mafiaserver.h \
    playermodel.h \
    gamecontroller.h \
    gamemodel.h \
    messagetypes.h \
    message.h \
    npccontainer.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
