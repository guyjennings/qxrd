message(qceplib-cbf-include PWD = $${PWD} QCEPLIB_CBF_VERSION = $${QCEPLIB_CBF_VERSION})

INCLUDEPATH += $${PWD}

macx {
  CBFBASE = $${PWD}/CBFlib-$${QCEPLIB_CBF_VERSION}/src/
  CBFINCL = $${PWD}/CBFlib-$${QCEPLIB_CBF_VERSION}/include/

  DEFINES += CBF_NO_REGEX
} else:win32 {
  CBFBASE = $${PWD}/CBFlib-$${QCEPLIB_CBF_VERSION}/src/
  CBFINCL = $${PWD}/CBFlib-$${QCEPLIB_CBF_VERSION}/include/

  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
  } else {
    message(32 bit build)
  }

  DEFINES += CBF_NO_REGEX
#  DEFINES += CBF_USE_ULP
} else:unix {
  INCLUDEPATH += /usr/include/cbf/
  LIBS += -lcbf
}

macx|win32 {
  INCLUDEPATH += $${CBFINCL}
}

