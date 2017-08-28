#ifndef QCEPLIB_GLOBAL_H
#define QCEPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QCEPLIB_LIBRARY)
#  define QCEP_EXPORT Q_DECL_EXPORT
#else
#  define QCEP_EXPORT Q_DECL_IMPORT
#endif

#endif // QCEPLIB_GLOBAL_H
