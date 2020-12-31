QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L/usr/local/lib -lGLU
LIBS += /usr/local/lib/libphidget22.so
#LIBS += /usr/lib/libphidget21.so
#LIBS += /usr/lib/libdxl_x64_cpp.so
#LIBS += /usr/lib/libEposCmd.so
#LIBS += /usr/lib/libHD.so.3.4
#LIBS += /usr/lib/libHDU.a

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Phidget22.cpp \
    main.cpp \
    mainwindow.cpp \
    # Phidget.cpp \
    phidgetRead.cpp \
    threadTest.cpp

HEADERS += \
    Phidget22.h \
    main.h \
    mainwindow.h \
    # Phidget.h \
    phidgetRead.h \
    threadTest.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

release:DESTDIR = $$PWD/build/release
release:OBJECTS_DIR = $$PWD/build/release/.obj
release:MOC_DIR = $$PWD/build/release/.moc
release:RCC_DIR = $$PWD/build/release/.rcc
release:UI_DIR = $$PWD/build/release/.ui

debug:DESTDIR = $$PWD/build/debug
debug:OBJECTS_DIR = $$PWD/build/debug/.obj
debug:MOC_DIR = $$PWD/build/debug/.moc
debug:RCC_DIR = $$PWD/build/debug/.rcc
debug:UI_DIR = $$PWD/build/debug/.ui

profile:DESTDIR = $$PWD/build/profile
profile:OBJECTS_DIR = $$PWD/build/profile/.obj
profile:MOC_DIR = $$PWD/build/profile/.moc
profile:RCC_DIR = $$PWD/build/profile/.rcc
profile:UI_DIR = $$PWD/build/profile/.ui
