QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qceplib
TEMPLATE = app

SOURCES += \
  qcepdebug.cpp \
  qcepproperty.cpp \
  qcepmutexlocker.cpp \
  qcepdocumentationdictionary.cpp \
  qcepsettingssaver.cpp \
  qcepimagedata.cpp \
  qcepimagedataformatfactory.cpp \
  qcepimagedatadormat.cpp \
  qcepimagedataformattiff.cpp \
  qcepimagedataformatmar345.cpp \
  qspecserver.cpp \
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
  qcepimagedatadormat.h \
  qcepimagedataformattiff.h \
  qcepimagedataformatmar345.h \
  qcepimagedataformatmar345-ptr.h \
  qspecserver.h \
  spec_server.h \
