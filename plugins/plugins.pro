include("../qxrd.version.pri")
include("plugin-tests.pri")
include("../compiler.pri")
include("../extras-recurse.pri")

TEMPLATE = subdirs

SUBDIRS =  \
  qxrdpilatusplugin \
  qxrdareadetectorplugin \
  qxrdsimulatedplugin \
  qxrdfilewatcherplugin

contains(DEFINES,HAVE_PERKIN_ELMER) {
  message("HAVE_PERKIN_ELMER")
  SUBDIRS += qxrdperkinelmerplugin
}

contains(DEFINES,HAVE_DEXELA) {
  message("HAVE_DEXELA")
  SUBDIRS += qxrddexelaplugin
}

contains(DEFINES,HAVE_NIDAQ) {
  message("HAVE_NIDAQ")
  SUBDIRS += qxrdnidaqplugin
}

contains(DEFINES,HAVE_ALLIEDVISION) {
  message("HAVE_ALLIEDVISION")
  SUBDIRS += qxrdalliedvisionplugin
}

CONFIG += ordered
