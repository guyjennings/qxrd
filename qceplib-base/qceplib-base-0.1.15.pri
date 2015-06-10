message(qceplib PWD = $${PWD} QCEPLIB_VERSION = $${QCEPLIB_VERSION})

greaterThan(QT_MAJOR_VERSION,4): QT += widgets concurrent

CONFIG += qt

QT += script

INCLUDEPATH += $${PWD}

SOURCES += \
  $${PWD}/qcepdebug.cpp \
  $${PWD}/qcepproperty.cpp \
  $${PWD}/qcepvector3dproperty.cpp \
  $${PWD}/qcepmatrix3x3property.cpp \
  $${PWD}/qcepplotmarker.cpp \
  $${PWD}/qcepplotmarkerlist.cpp \
  $${PWD}/qcepplotmarkerlistproperty.cpp \
  $${PWD}/qcepmutexlocker.cpp \
  $${PWD}/qcepdocumentationdictionary.cpp \
  $${PWD}/qcepsettingssaver.cpp \
  $${PWD}/qcepimagedata.cpp \
  $${PWD}/qcepimagedataformatfactory.cpp \
  $${PWD}/qcepimagedataformat.cpp \
  $${PWD}/qcepobjectnamer.cpp \
  $${PWD}/qcepexperiment.cpp \
  $${PWD}/qceppropertyvalue.cpp \
  $${PWD}/qcepmatrix3x3.cpp \
  $${PWD}/qcepvector3d.cpp \
  $${PWD}/qcepobject.cpp \
  $${PWD}/qcepdataobject.cpp \
  $${PWD}/qcepdatagroup.cpp \
  $${PWD}/qcepdataarray.cpp \
  $${PWD}/qcepdatacolumnscan.cpp \
  $${PWD}/qcepdatacolumn.cpp \
  $${PWD}/qcepdataset.cpp \
  $${PWD}/qcepdatasetmodel.cpp

HEADERS += \
  $${PWD}/qcepdebug.h \
  $${PWD}/qcepproperty.h \
  $${PWD}/qcepproperty-ptr.h \
  $${PWD}/qcepvector3dproperty.h \
  $${PWD}/qcepmatrix3x3property.h \
  $${PWD}/qcepplotmarker.h \
  $${PWD}/qcepplotmarkerlist.h \
  $${PWD}/qcepplotmarkerlistproperty.h \
  $${PWD}/qcepmutexlocker.h \
  $${PWD}/qcepdocumentationdictionary.h \
  $${PWD}/qcepsettingssaver.h \
  $${PWD}/qcepsettingssaver-ptr.h \
  $${PWD}/qcepimagedata.h \
  $${PWD}/qcepimagedataformatfactory.h \
  $${PWD}/qcepimagedataformatfactory-ptr.h \
  $${PWD}/qcepimagedataformat.h \
  $${PWD}/qcepobjectnamer.h \
  $${PWD}/qcepexperiment.h \
  $${PWD}/qcepexperiment-ptr.h \
  $${PWD}/qceppropertyvalue.h \
  $${PWD}/qcepmatrix3x3.h \
  $${PWD}/qcepvector3d.h \
  $${PWD}/qcepobject.h \
  $${PWD}/qcepdataobject.h \
  $${PWD}/qcepdatagroup.h \
  $${PWD}/qcepdataarray.h \
  $${PWD}/qcepdatacolumnscan.h \
  $${PWD}/qcepdatacolumn.h \
  $${PWD}/qcepdataarray-ptr.h \
  $${PWD}/qcepdatacolumn-ptr.h \
  $${PWD}/qcepdatacolumnscan-ptr.h \
  $${PWD}/qcepdatagroup-ptr.h \
  $${PWD}/qcepdataobject-ptr.h \
  $${PWD}/qcepdataset.h \
  $${PWD}/qcepdataset-ptr.h \
  $${PWD}/qcepdatasetmodel.h \
  $${PWD}/qcepdatasetmodel-ptr.h
