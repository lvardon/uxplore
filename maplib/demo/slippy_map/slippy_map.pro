TOPDIR = ../..
include($$TOPDIR/include.pri)

QT += network

TARGET = main

SOURCES   *= slippy_map.cpp

SOURCES   *= $$TOPDIR/SlippyMapWidget.cpp
HEADERS   *= $$TOPDIR/SlippyMapWidget.h
RESOURCES *= $$TOPDIR/SlippyMapWidget.qrc
SOURCES   *= $$TOPDIR/GeoUtils.cpp
SOURCES   *= $$TOPDIR/Downloader.cpp
HEADERS   *= $$TOPDIR/Downloader.h
SOURCES   *= $$TOPDIR/mydebug.cpp

SOURCES   *= $$TOPDIR/SlippyLocations.cpp
HEADERS   *= $$TOPDIR/SlippyLocations.h
SOURCES   *= $$TOPDIR/ParseCSV.cpp
MVG_YAML  *= $$TOPDIR/locations.yaml

# The following switches disable certain aspects of the auto-generated
# model-view. But this makes our implementation of locations.{h,cpp}
# different from the one in demo/locations. Therefore we need to
# make sure that we have a local directory for moc and obj.
MVG_OPTIONS *= --no-model --no-view --no-dialog
OBJECTS_DIR = .obj
MOC_DIR     = .obj

include($$TOPDIR/mvg.pri)
