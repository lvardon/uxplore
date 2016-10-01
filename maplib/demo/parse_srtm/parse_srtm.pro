TOPDIR = ../..
include($$TOPDIR/include.pri)

CONFIG -= debug
CONFIG *= release

TARGET = main

SOURCES *= parse_srtm.cpp
SOURCES *= $$TOPDIR/SRTMv4.cpp
SOURCES *= $$TOPDIR/mydebug.cpp
