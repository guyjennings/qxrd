#ifndef QCEPLIB_GLOBAL_H
#define QCEPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QCEPLIB_LIBRARY)
#  define QCEPLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QCEPLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QCEPLIB_GLOBAL_H
