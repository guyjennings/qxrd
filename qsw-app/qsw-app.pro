include("qsw.version.pri")
include("../qxrd.platform.pri")

message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Destdir = $${DESTDIR})

TEMPLATE = app
TARGET   = qsw
DESTDIR  = ../

macx {
  ICON = $${TARGET}-icon.icns
}

win32 {
  RC_FILE = $${TARGET}.rc
}

OTHER_FILES += $${TARGET}-icon.icns $${TARGET}.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

CONFIG += qt

QT += network \
    qml \
    script \
    scripttools \
    testlib

unix {
    message(Version = $${VERSION})
    message(Plugin path = $${QXRD_PLUGIN_PATH})
}

DEFINES += QSW_VERSION=\"$$VERSION\"


*g++* {
    QMAKE_CXXFLAGS += -g
    QMAKE_CFLAGS += -g
    QMAKE_LFLAGS += -g
}

MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
RCC_DIR = rcc
message(Building: $${TARGET})

INCLUDEPATH += $${PWD}/../libraries/qxrdlib/
INCLUDEPATH += $${OUT_PWD}/../libraries/qxrdlib/

CONFIG += qceplib-qwt
CONFIG += qceplib-hdf5
CONFIG += qceplib-tiff

#CONFIG += qceplib-nexus

include(../libraries/qceplib/qceplib/qceplib-qwt-include.pri)
include(../libraries/qceplib/qceplib/qceplib-base-include.pri)
include(../libraries/qceplib/qceplib/qceplib-mar345-include.pri)
include(../libraries/qceplib/qceplib/qceplib-cbf-include.pri)
include(../libraries/qceplib/qceplib/qceplib-tiff-include.pri)
include(../libraries/qceplib/qceplib/qceplib-levmar-include.pri)
include(../libraries/qceplib/qceplib/qceplib-szip-include.pri)
include(../libraries/qceplib/qceplib/qceplib-zlib-include.pri)
include(../libraries/qceplib/qceplib/qceplib-hdf5-include.pri)
include(../libraries/qceplib/qceplib/qceplib-bzip2-include.pri)
include(../libraries/qceplib/qceplib/qceplib-specserver-include.pri)

SOURCES += \
    qsw-app.cpp
