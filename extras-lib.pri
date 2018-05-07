macx {
  dummyTarget.target   = dmg
  QMAKE_EXTRA_TARGETS += dummyTarget
} else:unix {
#  dummyTarget.target   = rpm
#  QMAKE_EXTRA_TARGETS += dummyTarget
} else:win32 {
#  dummyTarget.target   = nsis
#  QMAKE_EXTRA_TARGETS += dummyTarget
}
