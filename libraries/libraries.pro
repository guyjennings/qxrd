TEMPLATE = subdirs

SUBDIRS += \
    qceplib \
    qxrdlib

CONFIG += ordered

macx{
  dmg.depends          = FORCE
  dmg.CONFIG          += recursive
  dmg.recurse_target   = dmg
  QMAKE_EXTRA_TARGETS += dmg
}
