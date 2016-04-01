#ifndef QCEPDATAIMPORTCOMMANDPTR_H
#define QCEPDATAIMPORTCOMMANDPTR_H

#include <QSharedPointer>

class QcepDataImportCommand;

typedef QSharedPointer<QcepDataImportCommand> QcepDataImportCommandPtr;
typedef QWeakPointer<QcepDataImportCommand>   QcepDataImportCommandWPtr;

#endif // QCEPDATAIMPORTCOMMANDPTR_H
