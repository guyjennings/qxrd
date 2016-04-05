#ifndef QCEPFILEIMPORTERPTR_H
#define QCEPFILEIMPORTERPTR_H

#include <QSharedPointer>

class QcepFileImporter;

typedef QSharedPointer<QcepFileImporter> QcepFileImporterPtr;
typedef QWeakPointer<QcepFileImporter>   QcepFileImporterWPtr;

#endif // QCEPFILEIMPORTERPTR_H
