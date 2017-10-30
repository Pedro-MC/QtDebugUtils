TEMPLATE = subdirs

SUBDIRS += \
    lib \
    test \
    xmoc

lib.depends = xmoc
test.depends = lib

OTHER_FILES = \
    .gitignore

DISTFILES += \
    LICENSE \
    AUTHORS.md \
    CHANGELOG.md \
    README.md \
    README-QAddressWiper.md \
    README-QValueStringifier.md \
    README-QObjectStringifier.md \
    README-QMethodStringifier.md \
    README-QSignalSlotMonitor.md \
    README-QUniversalSlot.md \
    README-QSignalDumper.md \
    README-QSignalLogger.md
