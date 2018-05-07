macx{
  dmg.depends          =  FORCE
  dmg.CONFIG          +=  recursive
  dmg.recurse_target   =  dmg
  QMAKE_EXTRA_TARGETS +=  dmg
} else:unix {
  rpm.depends          = FORCE
  rpm.CONFIG          += recursive
  rpm.recurse_target   = rpm
  QMAKE_EXTRA_TARGETS  = rpm
} else:win32 {
#  nsis.depends         = FORCE
#  nsis.CONFIG         += recursive
#  nsis.recurse_target  = nsis
#  QMAKE_EXTRA_TARGETS  = nsis
}
