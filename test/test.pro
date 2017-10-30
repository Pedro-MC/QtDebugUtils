QT       += testlib
QT       -= gui

TARGET    = test
CONFIG   += warn_on console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES  += QT_NO_CAST_FROM_ASCII
DEFINES  += QT_NO_CAST_TO_ASCII

INCLUDEPATH += \
    ../lib/QSignalSlotMonitor \
    ../lib/QUniversalSlot \
    ../lib/QValueStringifier \
    ../lib/QObjectStringifier \
    ../lib/QMethodStringifier \
    ../lib/QSignalDumper \
    ../lib/QSignalLogger \
    ../lib/QAddressWiper

HEADERS += \
    QDebugUtilsTest.h \
    QTestSignaler.h \
    QTestSignalerD.h \
    QTestSignalSlotMonitor.h \
    QTestUniversalSlot.h

SOURCES += \
    main.cpp \
    QDebugUtilsTest.cpp \
    QTestSignalSlotMonitor.cpp \
    QTestSignaler.cpp \
    QTestSignalerD.cpp \
    testData.cpp \
    testQSignalDumper.cpp \
    testQUniversalSlot.cpp \
    testQSignalLogger.cpp \
    testQMethodStringifier.cpp \
    testQAddressWiper.cpp \
    testQSignalSlotMonitor.cpp \
    QTestUniversalSlot.cpp \
    testQValueStringifier.cpp \
    testQObjectStringifier.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32: SONAME=dll
else:  SONAME=so

LIBS += $${OUT_PWD}/../lib/libQtDebugUtils.$${SONAME}.1

debug:QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
debug:QMAKE_LFLAGS += -lgcov --coverage
