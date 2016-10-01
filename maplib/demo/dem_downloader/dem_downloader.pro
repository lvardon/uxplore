TOPDIR = ../..
include($$TOPDIR/include.pri)

QT += network



TARGET = main

SOURCES *= dem_downloader.cpp

SOURCES *= $$TOPDIR/DemDownloader.cpp
HEADERS *= $$TOPDIR/DemDownloader.h
SOURCES *= $$TOPDIR/SRTMv4.cpp
SOURCES *= $$TOPDIR/mydebug.cpp
SOURCES *= $$TOPDIR/Downloader.cpp
HEADERS *= $$TOPDIR/Downloader.h

INCLUDEPATH *= $$TOPDIR/3rdparty/qzip
SOURCES *= $$TOPDIR/3rdparty/qzip/qzip.cpp


