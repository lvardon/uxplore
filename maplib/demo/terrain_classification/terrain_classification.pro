TOPDIR = ../..
include($$TOPDIR/include.pri)

TARGET = main

INCLUDEPATH *= $$TOPDIR/demo/ele_profile

SOURCES *= terrain_classification.cpp
SOURCES *= $$TOPDIR/ProfileWidget.cpp
SOURCES *= $$TOPDIR/ITUTerrain.cpp
SOURCES *= $$TOPDIR/DrawITUTerrain.cpp
SOURCES *= $$TOPDIR/GeoUtils.cpp
SOURCES *= $$TOPDIR/SRTMv4.cpp
SOURCES *= $$TOPDIR/mydebug.cpp
