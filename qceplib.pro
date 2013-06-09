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
    main.cpp

HEADERS += \
  qcepdebug.h \
  qcepproperty.h \
  qcepproperty-ptr.h \
  qcepmutexlocker.h \
  qcepdocumentationdictionary.h \
  qcepsettingssaver.h \
  qcepsettingssaver-ptr.h \
