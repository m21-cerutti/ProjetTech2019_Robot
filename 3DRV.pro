#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T22:00:45
#
#-------------------------------------------------

QT       += core gui

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DRV
TEMPLATE = app

# Import OpenCV
#INCLUDEPATH += /usr/include/opencv
#LIBS += -L//usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_highgui


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tools/imageanalyser.cpp \
    subWindows/sgbmparamdialog.cpp \
    subWindows/bmparamdialog.cpp \
    tools/cameracalibration.cpp \
    tools/cvqtinterface.cpp \
    tools/projectdebuger.cpp \
    subWindows/cameraparamdialog.cpp

HEADERS += \
        mainwindow.h \
    tools/imageanalyser.h \
    subWindows/sgbmparamdialog.h \
    subWindows/bmparamdialog.h \
    tools/cameracalibration.h \
    tools/cvqtinterface.h \
    tools/projectdebuger.h \
    subWindows/cameraparamdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui \
    subWindows/sgbmparamdialog.ui \
    subWindows/bmparamdialog.ui \
    subWindows/cameraparamdialog.ui

DISTFILES +=
