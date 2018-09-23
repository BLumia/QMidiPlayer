#-------------------------------------------------
#
# Project created by QtCreator 2018-09-21T22:04:32
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

TARGET = bongocat-plugin
TEMPLATE = lib

DEFINES += BONGOCATPLUGIN_LIBRARY

SOURCES += bongocatplugin.cpp \
    bongocatwindow.cpp

HEADERS += bongocatplugin.hpp \
    bongocatwindow.hpp

unix {
	target.path = /usr/lib
	INSTALLS += target
}

RESOURCES     = bongocat.qrc
