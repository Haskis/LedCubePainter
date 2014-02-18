#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:51:11
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = LedCube_Podejscie1
TEMPLATE = app
include(ExtSerialPort/qextserialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    serialportoptionswidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    serialportoptionswidget.h

FORMS    += mainwindow.ui \
    serialportoptionswidget.ui
LIBS += -lglut -lGLU

CONFIG += extserialport

