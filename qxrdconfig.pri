win32 {
  QMAKE_CXXFLAGS += -g
  QMAKE_CFLAGS += -g
  QMAKE_LFLAGS += -g

  DEBUG_SUFFIX = d

#  CONFIG += console
}

unix {
  QMAKE_CXXFLAGS += -g
  QMAKE_CFLAGS += -g
  QMAKE_LFLAGS += -g

  DEBUG_SUFFIX = d
}

CONFIG += debug_and_release build_all

CONFIG(debug, debug|release) {
  SUFFIX_STR = $${DEBUG_SUFFIX}
} else {
  SUFFIX_STR = $${RELEASE_SUFFIX}
}
