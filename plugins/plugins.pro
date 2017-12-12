include("../qxrd.version.pri")
include("../qxrd.platform.pri")

TEMPLATE = subdirs

SUBDIRS =  \
  qxrdpilatusplugin \
  qxrdareadetectorplugin \
  qxrdcudaprocessorplugin \
  qxrdsimulatedplugin \
  qxrdfilewatcherplugin \
  qxrddexelaplugin

message(DEFINES == $$DEFINES)

contains(DEFINES,HAVE_PERKIN_ELMER) {
  message("HAVE_PERKIN_ELMER")
  SUBDIRS += qxrdperkinelmerplugin
}

contains(DEFINES,HAVE_NIDAQ) {
  message("HAVE_NIDAQ")
  SUBDIRS += qxrdnidaqplugin
}

CONFIG += ordered
