TOPDIR = ../..
include($$TOPDIR/include.pri)

QT += network

TARGET = main

SOURCES   *= map_chooser.cpp

SOURCES   *= $$TOPDIR/SlippyMapWidget.cpp
HEADERS   *= $$TOPDIR/SlippyMapWidget.h
RESOURCES *= $$TOPDIR/SlippyMapWidget.qrc
SOURCES   *= $$TOPDIR/MapChooseWidget.cpp
HEADERS   *= $$TOPDIR/MapChooseWidget.h
SOURCES   *= $$TOPDIR/GeoUtils.cpp
SOURCES   *= $$TOPDIR/Downloader.cpp
HEADERS   *= $$TOPDIR/Downloader.h
SOURCES   *= $$TOPDIR/mydebug.cpp
