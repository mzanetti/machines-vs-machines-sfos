# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-machines-vs-machines-sfos
TARGET_DATA_DIR = /usr/share/$${TARGET}

CONFIG += sailfishapp C++11

SOURCES += src/machines-vs-machines-sfos.cpp \
    src/tower.cpp \
    src/engine.cpp \
    src/enemy.cpp \
    src/towerfactory.cpp \
    src/levelpacks.cpp \
    src/levelpack.cpp \
    src/wave.cpp \
    src/board.cpp \
    src/enemies.cpp \
    src/field.cpp \
    src/level.cpp \
    src/settings.cpp \
    src/towerproxymodel.cpp \
    src/boardproxymodel.cpp \


HEADERS += src/tower.h \
        src/engine.h \
        src/enemy.h \
        src/towerfactory.h \
        src/levelpacks.h \
        src/levelpack.h \
        src/wave.h \
        src/board.h \
        src/enemies.h \
        src/field.h \
        src/level.h \
        src/settings.h \
        src/towerproxymodel.h \
        src/boardproxymodel.h \


OTHER_FILES += rpm/machines-vs-machines-sfos.changes.in \
    translations/*.ts \
    harbour-machines-vs-machines-sfos.desktop \
    qml/graphics/* \
    data/lpbuild/*

# Data

TARGET_DATA = data
OTHER_FILES += $${TARGET_DATA}

data.files = $${TARGET_DATA}/lpbuild/*
data.path = $${TARGET_DATA_DIR}/data
INSTALLS += data

#copydata.commands = $(COPY_DIR) $$PWD/data/lpbuild /home/deploy/installroot/usr/share/harbour-machines-vs-machines-sfos/data/
#target.depends = $(target) copydata
#export(first.depends)
#export(copydata.commands)

QMAKE_EXTRA_TARGETS += target # copydata

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/machines-vs-machines-sfos-de.ts

DISTFILES += \
    rpm/machines-vs-machines-sfos.spec
