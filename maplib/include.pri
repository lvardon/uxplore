#DESTDIR     = $$TOPDIR/bin
UI_DIR      = $$TOPDIR/.obj
MOC_DIR     = $$TOPDIR/.obj
RCC_DIR     = $$TOPDIR/.obj
OBJECTS_DIR = $$TOPDIR/.obj

INCLUDEPATH *= $$TOPDIR
LIBS *= -lz

CONFIG -= release
CONFIG *= debug
CONFIG *= warn_on
CONFIG *= silent

QMAKE_CLEAN *= $$DESTDIR/$$TARGET
