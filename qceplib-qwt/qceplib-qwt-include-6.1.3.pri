message(qceplib-qwt-include PWD = $${PWD} QCEPLIB_QWT_VERSION = $${QCEPLIB_QWT_VERSION})

#contains (DEFINES,HAVE_BASE) {
#  error(must include qceplib-qwt prior to including qceplib-qwt)
#}

DEFINES += QWT_DLL

greaterThan(QT_MAJOR_VERSION,4): QT += printsupport concurrent

QT += opengl svg

QWTBASE = $${PWD}/qwt-6.1.3/src

INCLUDEPATH += $${QWTBASE}
