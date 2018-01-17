message("qceplib-base PWD = $${PWD} QCEPLIB_VERSION = $${QCEPLIB_VERSION}")

greaterThan(QT_MAJOR_VERSION,4) {
  QT += widgets concurrent
}

CONFIG += qt

QT += script

contains (DEFINES,HAVE_QWT) {
 message(qceplib-base wants qwt)
} else {
 message(qceplib-base no qwt)
}

INCLUDEPATH += $${PWD}
DEPENDPATH  += $${PWD}
