#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T08:17:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = productOfF
TEMPLATE = app
CONFIG += c++11
#CONFIG += console

SOURCES += main.cpp\
    Function/function.cpp \
        dialog.cpp \
    graphic.cpp

HEADERS  += dialog.h \
    Function/addition.h \
    Function/assingmentoperandtoregister.h \
    Function/assingmentregistertovariable.h \
    Function/binaryoperation.h \
    Function/callingfunction.h \
    Function/division.h \
    Function/function.h \
    Function/indexwithwhatsindexed.h \
    Function/multiplication.h \
    Function/operation.h \
    Function/powering.h \
    Function/remainder.h \
    Function/subtraction.h \
    graphic.h

FORMS    += dialog.ui
