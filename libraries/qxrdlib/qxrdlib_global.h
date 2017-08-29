#ifndef QXRDLIB_GLOBAL_H
#define QXRDLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QXRDLIB_LIBRARY)
#  define QXRD_EXPORT Q_DECL_EXPORT
#else
#  define QXRD_EXPORT Q_DECL_IMPORT
#endif

#endif // QXRDLIB_GLOBAL_H
