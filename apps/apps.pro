TEMPLATE = subdirs

SUBDIRS += \
  qavrg-app \
  qpidctrl-app \
  qse-app \
  qsw-app \
  qxia-app \
  qxrd-app \
  qxrdviewer-app

CONFIG += ordered

include("../extras-recurse.pri")
