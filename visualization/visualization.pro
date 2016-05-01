#-------------------------------------------------
#
# Project created by QtCreator 2016-04-24T22:04:32
#
#-------------------------------------------------

QT       -= core gui

CONFIG += c++11

TARGET = visualization
TEMPLATE = lib

DEFINES += VISUALIZATION_LIBRARY

SOURCES += qmpvisualization.cpp

HEADERS += qmpvisualization.hpp

unix {
	target.path = /usr/lib/qmidiplayer
	INSTALLS += target
	QMAKE_CXXFLAGS += -pthread -fPIC
}
#well...
INCLUDEPATH += /home/chrisoft/devel/BulletLabRemixIII/include/ /usr/include/freetype2
LIBS += -L/home/chrisoft/devel/BulletLabRemixIII/smelt/sdl/
LIBS += -L/home/chrisoft/devel/BulletLabRemixIII/extensions/
LIBS += -lstdc++ -lSDL2 -ljpeg -lpng -lfreetype -lz -lsmeltext -lsmelt-dumb -lCxImage