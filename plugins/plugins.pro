include("../qxrd.version.pri")
include("../qxrd.platform.pri")

TEMPLATE = subdirs

SUBDIRS =  qxrdpilatusplugin qxrdareadetectorplugin qxrdcudaprocessorplugin

defined($$HAVE_PERKIN_ELMER) {
  SUBDIRS += qxrdperkinelmerplugin
}
