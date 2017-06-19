#ifndef QCEPFILEFORMATTERPTR_H
#define QCEPFILEFORMATTERPTR_H

#include <QSharedPointer>

class QcepFileFormatter;

typedef QSharedPointer<QcepFileFormatter> QcepFileFormatterPtr;
typedef QWeakPointer<QcepFileFormatter>   QcepFileFormatterWPtr;

#endif // QCEPFILEFORMATTERPTR_H
