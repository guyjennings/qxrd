#ifndef QXRDPLOTWIDGETPTR_H
#define QXRDPLOTWIDGETPTR_H

#include <QSharedPointer>

class QxrdPlotWidget;

typedef QSharedPointer<QxrdPlotWidget> QxrdPlotWidgetPtr;
typedef QWeakPointer<QxrdPlotWidget>   QxrdPlotWidgetWPtr;

#endif // QXRDPLOTWIDGETPTR_H
