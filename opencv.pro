#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------

QT       += core gui sql network
LIBS += -lcurl
INCLUDEPATH += C:/Users/Admin/Desktop/opencv-qt/curl-8.11.0_2-win64-mingw/curl-8.11.0_2-win64-mingw/include
LIBS += -LC:/Users/Admin/Desktop/opencv-qt/curl-8.11.0_2-win64-mingw/curl-8.11.0_2-win64-mingw/lib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

# Enable C++11
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        connection.cpp \


HEADERS += \
        mainwindow.h \
        connection.h \


FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
