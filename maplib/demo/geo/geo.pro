TOPDIR = ../..
include($$TOPDIR/include.pri)

QT -= gui

TARGET = main

SOURCES *= geo.cpp
SOURCES *= $$TOPDIR/GeoUtils.cpp
SOURCES *= $$TOPDIR/mydebug.cpp
