TEMPLATE = subdirs

SUBDIRS += \
  qavrg-app \
  qmca-app \
  qpidctrl-app \
  qse-app \
  qsw-app \
  qxia-app \
  qxrayeye-app \
  qxrd-app \
  qxrdviewer-app

CONFIG += ordered

include("../extras-recurse.pri")
