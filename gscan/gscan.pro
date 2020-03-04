#-------------------------------------------------
#
# Project created by QtCreator 2019-08-19T10:11:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HuaGoScan
TEMPLATE = app

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
    AdminDialog.cpp \
    SleepTimeDialog.cpp \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    scansettingsdialog.cpp \
    imageList.cpp \
    chosenpagedialog.cpp \
    scanlotsdialog.cpp \
    localbcdialog.cpp \
    fullscreenform.cpp \
    mygraphicsscene.cpp \
    getimgthread.cpp \
    consumestatusdialog.cpp \
    imginfodialog.cpp \
    FileExport.cpp \
    CallbackNotice.cpp \
    PreScanDialog.cpp

HEADERS += \
    AdminDialog.h \
    SleepTimeDialog.h \
    aboutdialog.h \
    mainwindow.h \
    scansettingsdialog.h \
    imageList.h \
    chosenpagedialog.h \
    scanlotsdialog.h \
    localbcdialog.h \
    fullscreenform.h \
    mygraphicsscene.h \
    getimgthread.h \
    settings.h \
    consumestatusdialog.h \
    imginfodialog.h \
    FileExport.h \
    CallbackNotice.h \
    PreScanDialog.h

FORMS += \
    AdminDialog.ui \
    SleepTimeDialog.ui \
    aboutdialog.ui \
    mainwindow.ui \
    scansettingsdialog.ui \
    chosenpagedialog.ui \
    localbcdialog.ui \
    fullscreenform.ui \
    consumestatusdialog.ui \
    imginfodialog.ui \
    PreScanDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    hg_sane_rsc.qrc

win32{
    INCLUDEPATH += $$PWD/../3rdparty/3rdparty/win/opencv/include \
                   $$PWD/../3rdparty/3rdparty/win/tiff/include \
                   $$PWD/../3rdparty/3rdparty/win/ofd/include
    DEPENDPATH += $$PWD/../3rdparty/3rdparty/win/opencv/include \
                  $$PWD/../3rdparty/3rdparty/win/tiff/include \
                  $$PWD/../3rdparty/3rdparty/win/ofd/include

#   LIBS += -L$$PWD/../3rdparty\3rdparty\win\ofd\x86_64 -llibSuwellConvert

    CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lColorLineSet -lcutDialog \
        -L$$PWD/../3rdparty/3rdparty/win/opencv/x86/lib -lopencv_world346 \
        -L$$PWD/../3rdparty/3rdparty/win/tiff/x86/Release -llibtiff \
        -L$$PWD/../3rdparty/3rdparty/win/turbojpeg/x86/lib/Release -llibjpeg-turbo \
        -L$$PWD/../3rdparty/3rdparty/win/zlib/Release -lzlib

    CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lColorLineSet -lcutDialog \
        -L$$PWD/../3rdparty/3rdparty/win/opencv/x86/lib -lopencv_world346d \
        -L$$PWD/../3rdparty/3rdparty/win/tiff/x86/Debug -llibtiff \
        -L$$PWD/../3rdparty/3rdparty/win/turbojpeg/x86/lib/Debug -llibjpeg-turbo \
        -L$$PWD/../3rdparty/3rdparty/win/zlib/Debug -lzlib
}

unix{
    LIBS += -L/opt/libjpeg-turbo/lib64 -lturbojpeg -lusb-1.0 -L$$PWD/../3rdparty/libtiff/x86/dynamic -ltiff
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lColorLineSet -lcutDialog -lgimgproc -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lgdev -lusb-1.0
    CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lColorLineSet -lcutDialog -lgimgproc -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lgdev -lusb-1.0
}

INCLUDEPATH += $$PWD/../ColorLineSet \
               $$PWD/../cutDialog
DEPENDPATH += $$PWD/../ColorLineSet \
               $$PWD/../cutDialog
CONFIG(release, debug|release): DESTDIR += ../bin/release
CONFIG(debug, debug|release): DESTDIR += ../bin/debug
