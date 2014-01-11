message(qceplib PWD = $${PWD})

macx {
  include("$${PWD}/tiffconfig/qt-libtiff-macx.pri")
} else:win32 {
  include("$${PWD}/tiffconfig/qt-libtiff-win32.pri")
} else {
  LIBS += -ltiff
}

include("qt-cbf.pri")

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
  $${PWD}/qcepimagedataformattiff.cpp \
  $${PWD}/qcepimagedataformatmar345.cpp \
  $${PWD}/qcepimagedataformatcbf.cpp \
  $${PWD}/qcepobjectnamer.cpp \
  $${PWD}/qcepexperiment.cpp \
  $${PWD}/qspecserver.cpp \
  $${PWD}/qceppropertyvalue.cpp \
  $${PWD}/qcepmatrix3x3.cpp \
  $${PWD}/qcepvector3d.cpp

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
  $${PWD}/qcepimagedataformattiff.h \
  $${PWD}/qcepimagedataformatmar345.h \
  $${PWD}/qcepimagedataformatmar345-ptr.h \
  $${PWD}/qcepimagedataformatcbf.h \
  $${PWD}/qcepobjectnamer.h \
  $${PWD}/qcepexperiment.h \
  $${PWD}/qcepexperiment-ptr.h \
  $${PWD}/qspecserver.h \
  $${PWD}/spec_server.h \
  $${PWD}/qceppropertyvalue.h \
  $${PWD}/qcepmatrix3x3.h \
  $${PWD}/qcepvector3d.h
