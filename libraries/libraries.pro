TEMPLATE = subdirs

SUBDIRS += \
    qceplib \
    qxrdlib

CONFIG += ordered

include("../extras-recurse.pri")
