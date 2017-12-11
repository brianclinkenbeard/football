#-------------------------------------------------
#
# Project created by QtCreator 2017-10-25T17:00:41
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = football
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=\
    src/login.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/adminwindow.cpp \
    src/stadiummap.cpp

HEADERS  += \
    src/login.h \
    src/mainwindow.h \
    src/adminwindow.h \
    src/graph.h \
    src/cart.h  \
    src/stadium.h \
    src/stadiummap.h

FORMS    += \
    src/login.ui \
    src/mainwindow.ui \
    src/adminwindow.ui

RESOURCES += \
    res/resources.qrc
