TOPDIR = ../..
include($$TOPDIR/include.pri)

QT *= network

TARGET = main


MVG_YAML = $$TOPDIR/locations.yaml
include($$TOPDIR/mvg.pri)

SOURCES *= qth.cpp

HEADERS   *= $$TOPDIR/LocationMapDialog.h
SOURCES   *= $$TOPDIR/LocationMapDialog.cpp
SOURCES   *= $$TOPDIR/SlippyMapWidget.cpp
HEADERS   *= $$TOPDIR/SlippyMapWidget.h
RESOURCES *= $$TOPDIR/SlippyMapWidget.qrc
SOURCES   *= $$TOPDIR/SlippyLocations.cpp
HEADERS   *= $$TOPDIR/SlippyLocations.h
SOURCES   *= $$TOPDIR/Downloader.cpp
HEADERS   *= $$TOPDIR/Downloader.h
SOURCES   *= $$TOPDIR/GeoUtils.cpp
SOURCES   *= $$TOPDIR/ParseCSV.cpp
SOURCES   *= $$TOPDIR/mydebug.cpp
