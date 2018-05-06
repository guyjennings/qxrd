#ifndef QSWLIB_GLOBAL_H
#define QSWLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QSW_LIBRARY)
#  define QSW_EXPORT Q_DECL_EXPORT
#else
#  define QSW_EXPORT Q_DECL_IMPORT
#endif

#endif // QSWLIB_GLOBAL_H
