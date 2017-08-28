message(qceplib-levmar PWD = $${PWD} QCEPLIB_LEVMAR_VERSION = $${QCEPLIB_LEVMAR_VERSION})

include(qceplib-levmar-include-$${QCEPLIB_LEVMAR_VERSION}.pri)

SOURCES += \
  $${PWD}/levmar-2.6/Axb.c  \
  $${PWD}/levmar-2.6/lmlec.c       \
  $${PWD}/levmar-2.6/misc.c \
  $${PWD}/levmar-2.6/lmbc.c    \
  $${PWD}/levmar-2.6/lmblec.c     \
  $${PWD}/levmar-2.6/lmbleic.c      \
  $${PWD}/levmar-2.6/lm.c

#  levmar-2.6/expfit.c  \
#  levmar-2.6/lmdemo.c   \

HEADERS += \
  $${PWD}/levmar-2.6/compiler.h  \
  $${PWD}/levmar-2.6/levmar.h  \
  $${PWD}/levmar-2.6/lm.h  \
  $${PWD}/levmar-2.6/misc.h

