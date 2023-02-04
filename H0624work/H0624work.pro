#-------------------------------------------------
#
# Project created by QtCreator 2021-12-05T16:17:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = H0624work
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gray.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    gray.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    image.qrc \
    l.qrc
INCLUDEPATH += D:\Study\opencv\cmakebuild\install\include
LIBS += D:\Study\opencv\cmakebuild\lib\libopencv_*.a

TRANSLATIONS = my_zh_EN.ts
