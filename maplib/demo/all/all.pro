TOPDIR = ../..
include($$TOPDIR/include.pri)

QT *= network
CONFIG -= release
CONFIG *= debug

TARGET = main


MVG_YAML = $$TOPDIR/locations.yaml
include($$TOPDIR/mvg.pri)

SOURCES *= profiler.cpp
SOURCES *= mainwindow.cpp
HEADERS *= mainwindow.h
FORMS   *= mainwindow.ui

HEADERS   *= $$TOPDIR/LocationMapDialog.h
SOURCES   *= $$TOPDIR/LocationMapDialog.cpp
SOURCES   *= $$TOPDIR/SlippyMapWidget.cpp
HEADERS   *= $$TOPDIR/SlippyMapWidget.h
RESOURCES *= $$TOPDIR/SlippyMapWidget.qrc
SOURCES   *= $$TOPDIR/SlippyLocations.cpp
HEADERS   *= $$TOPDIR/SlippyLocations.h
SOURCES   *= $$TOPDIR/MapChooseWidget.cpp
HEADERS   *= $$TOPDIR/MapChooseWidget.h
SOURCES   *= $$TOPDIR/ProfileWidget.cpp
HEADERS   *= $$TOPDIR/ProfileWidget.h
SOURCES   *= $$TOPDIR/Downloader.cpp
HEADERS   *= $$TOPDIR/Downloader.h
SOURCES   *= $$TOPDIR/SRTMv4.cpp
SOURCES   *= $$TOPDIR/GeoUtils.cpp
SOURCES   *= $$TOPDIR/ITUTerrain.cpp
SOURCES   *= $$TOPDIR/DrawITUTerrain.cpp
SOURCES   *= $$TOPDIR/ParseCSV.cpp
SOURCES   *= $$TOPDIR/mydebug.cpp
