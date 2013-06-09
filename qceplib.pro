QT       += core gui script network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qceplib
TEMPLATE = app

macx:include("tiffconfig/qt-libtiff-macx.pri")
win32:include("tiffconfig/qt-libtiff-win32.pri")

SOURCES += \
  qcepdebug.cpp \
  qcepproperty.cpp \
  qcepmutexlocker.cpp \
  qcepdocumentationdictionary.cpp \
  qcepsettingssaver.cpp \
  qcepimagedata.cpp \
  qcepimagedataformatfactory.cpp \
  qcepimagedataformat.cpp \
  qcepimagedataformattiff.cpp \
  qcepimagedataformatmar345.cpp \
#  qspecserver.cpp \
    main.cpp

HEADERS += \
  qcepdebug.h \
  qcepproperty.h \
  qcepproperty-ptr.h \
  qcepmutexlocker.h \
  qcepdocumentationdictionary.h \
  qcepsettingssaver.h \
  qcepsettingssaver-ptr.h \
  qcepimagedata.h \
  qcepimagedataformatfactory.h \
  qcepimagedataformatfactory-ptr.h \
  qcepimagedataformat.h \
  qcepimagedataformattiff.h \
  qcepimagedataformatmar345.h \
  qcepimagedataformatmar345-ptr.h \
#  qspecserver.h \
  spec_server.h \
