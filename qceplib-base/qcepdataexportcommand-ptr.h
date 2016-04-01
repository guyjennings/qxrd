#ifndef QCEPDATAEXPORTCOMMANDPTR_H
#define QCEPDATAEXPORTCOMMANDPTR_H

#include <QSharedPointer>

class QcepDataExportCommand;

typedef QSharedPointer<QcepDataExportCommand> QcepDataExportCommandPtr;
typedef QWeakPointer<QcepDataExportCommand>   QcepDataExportCommandWPtr;

#endif // QCEPDATAEXPORTCOMMANDPTR_H
