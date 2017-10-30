QT       -= gui

CONFIG   += warn_on c++11
TARGET    = QtDebugUtils
VERSION   = 1.0.0
TEMPLATE  = lib

DEFINES  += QT_DEPRECATED_WARNINGS
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES  += QT_NO_CAST_FROM_ASCII
DEFINES  += QT_NO_CAST_TO_ASCII

INCLUDEPATH += \
    QSignalSlotMonitor \
    QAddressWiper \
    QValueStringifier \
    QObjectStringifier \
    QMethodStringifier \
    QUniversalSlot \
    QSignalLogger \
    QSignalDumper

HEADERS += \
    QUniversalSlot/QUniversalSlot.h \
    QMethodStringifier/QMethodStringifier.h \
    QSignalSlotMonitor/QSignalSlotMonitor.h \
    QSignalLogger/QSignalLogger.h \
    QSignalDumper/QSignalDumper.h \
    QAddressWiper/QAddressWiper.h \
    QValueStringifier/QValueStringifier.h \
    QObjectStringifier/QObjectStringifier.h

SOURCES += \
    QSignalSlotMonitor/QSignalSlotMonitor.cpp \
    QUniversalSlot/QUniversalSlot.cpp \
    QMethodStringifier/QMethodStringifier.cpp \
    QSignalLogger/QSignalLogger.cpp \
    QSignalDumper/QSignalDumper.cpp \
    QValueStringifier/QValueStringifier.cpp \
    QObjectStringifier/QObjectStringifier.cpp

# Wrap the moc to post process the moc_*.cpp files and change the call to xslot()
# to pass it a pointer to all the signal's parameters pointers.
load(moc)
QMAKE_MOC="$${OUT_PWD}/../xmoc/xmoc" "$${QMAKE_MOC}"

# Enable coverage for debug binaries when using g++.
*-g++ {
    debug:QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    debug:QMAKE_LFLAGS += -lgcov --coverage
}

# Install
unix {
    target.path = /usr/lib
    INSTALLS += target
}
