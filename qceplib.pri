
macx {
  include("$${QCEPLIB}/tiffconfig/qt-libtiff-macx.pri")
} else:win32 {
  include("$${QCEPLIB}/tiffconfig/qt-libtiff-win32.pri")
} else {
  LIBS += -ltiff
}

INCLUDEPATH += $${QCEPLIB}

SOURCES += \
  $${QCEPLIB}/qcepdebug.cpp \
  $${QCEPLIB}/qcepproperty.cpp \
  $${QCEPLIB}/qcepvector3dproperty.cpp \
  $${QCEPLIB}/qcepmatrix3x3property.cpp \
  $${QCEPLIB}/qcepmutexlocker.cpp \
  $${QCEPLIB}/qcepdocumentationdictionary.cpp \
  $${QCEPLIB}/qcepsettingssaver.cpp \
  $${QCEPLIB}/qcepimagedata.cpp \
  $${QCEPLIB}/qcepimagedataformatfactory.cpp \
  $${QCEPLIB}/qcepimagedataformat.cpp \
  $${QCEPLIB}/qcepimagedataformattiff.cpp \
  $${QCEPLIB}/qcepimagedataformatmar345.cpp \
#  $${QCEPLIB}/qspecserver.cpp \


HEADERS += \
  $${QCEPLIB}/qcepdebug.h \
  $${QCEPLIB}/qcepproperty.h \
  $${QCEPLIB}/qcepproperty-ptr.h \
  $${QCEPLIB}/qcepvector3dproperty.h \
  $${QCEPLIB}/qcepmatrix3x3property.h \
  $${QCEPLIB}/qcepmutexlocker.h \
  $${QCEPLIB}/qcepdocumentationdictionary.h \
  $${QCEPLIB}/qcepsettingssaver.h \
  $${QCEPLIB}/qcepsettingssaver-ptr.h \
  $${QCEPLIB}/qcepimagedata.h \
  $${QCEPLIB}/qcepimagedataformatfactory.h \
  $${QCEPLIB}/qcepimagedataformatfactory-ptr.h \
  $${QCEPLIB}/qcepimagedataformat.h \
  $${QCEPLIB}/qcepimagedataformattiff.h \
  $${QCEPLIB}/qcepimagedataformatmar345.h \
  $${QCEPLIB}/qcepimagedataformatmar345-ptr.h \
#  $${QCEPLIB}/qspecserver.h \
#  $${QCEPLIB}/spec_server.h \
