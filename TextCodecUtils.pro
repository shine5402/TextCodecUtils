#-------------------------------------------------
#
# Project created by QtCreator 2019-01-24T14:51:33
#
#-------------------------------------------------

QT       += widgets

TARGET = TextCodecUtils
TEMPLATE = lib

DEFINES += TEXTCODECUTILS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    availabletextcodecdialog.cpp \
    fileiowithcodechelper.cpp \
    qchardet.cpp \
    textcodecconvertdialog.cpp \
    textconverthelper.cpp

HEADERS += \
        fileiowithcodechelper.h \
        textcodecutils_global.h \ 
    availabletextcodecdialog.h \
    qchardet.h \
    textcodecconvertdialog.h \
    textconverthelper.h \
    public_defines.h

CONFIG(debug, debug|release) {
     mac: TARGET = $$join(TARGET,,,_debug)
     win32: TARGET = $$join(TARGET,,,d)
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    availabletextcodecdialog.ui \
    textcodecconvertdialog.ui

DISTFILES += \
    outsideLibrarys.pri
include(outsideLibrarys.pri)
