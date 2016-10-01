TOPDIR = ../..
include($$TOPDIR/include.pri)

CONFIG -= debug
CONFIG *= release

TARGET = main

SOURCES *= ele_profile.cpp
SOURCES *= $$TOPDIR/ProfileWidget.cpp
SOURCES *= $$TOPDIR/SRTMv4.cpp
SOURCES *= $$TOPDIR/GeoUtils.cpp
SOURCES *= $$TOPDIR/mydebug.cpp
