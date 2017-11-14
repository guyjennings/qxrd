#ifndef QXRDPLOTCOMMANDPTR_H
#define QXRDPLOTCOMMANDPTR_H

#include <QSharedPointer>

class QxrdPlotCommand;

typedef QSharedPointer<QxrdPlotCommand> QxrdPlotCommandPtr;
typedef QWeakPointer<QxrdPlotCommand>   QxrdPlotCommandWPtr;

#endif // QXRDPLOTCOMMANDPTR_H
