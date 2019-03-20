#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T22:00:45
#
#-------------------------------------------------

QT       += core gui network
CONFIG += console

unix:!mac{
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DRV
TEMPLATE = app

# Import OpenCV
#INCLUDEPATH += /usr/include/opencv
#LIBS += -L\usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_highgui


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
    src/analyser/clientconnection.cpp \
    src/analyser/servermaster.cpp \
    src/subWindows/bmparamdialog.cpp \
    src/subWindows/cameraparamdialog.cpp \
    src/subWindows/sgbmparamdialog.cpp \
    src/tools/cvqtinterface.cpp \
    src/tools/stereomap.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/custom_controller.cpp \
    src/analyser/iarobot.cpp \
    src/tools/calibration.cpp \
    src/tools/filters.cpp \
    src/tools/files.cpp \
    src/tools/utilities.cpp \
    include/tools/stereocamera.cpp \
    include/stereocamera.cpp

INCLUDEPATH += include/

HEADERS += \
    include/analyser/clientconnection.h \
    include/analyser/servermaster.h \
    include/subWindows/bmparamdialog.h \
    include/subWindows/cameraparamdialog.h \
    include/subWindows/sgbmparamdialog.h \
    include/tools/cvqtinterface.h \
    include/tools/stereomap.h \
    include/mainwindow.h \
    include/controller.h \
    include/custom_controller.h \
    include/analyser/iarobot.h \
    include/tools/calibration.h \
    include/tools/filters.h \
    include/tools/files.h \
    include/tools/utilities.h \
    include/tools/stereocamera.h \
    include/stereocamera.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    forms/subWindows/bmparamdialog.ui \
    forms/subWindows/cameraparamdialog.ui \
    forms/subWindows/sgbmparamdialog.ui \
    forms/mainwindow.ui



