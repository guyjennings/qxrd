#ifndef QXRDSCATTERPLOTGRAPHCONTROLLER_H
#define QXRDSCATTERPLOTGRAPHCONTROLLER_H

#include "qxrddataobjectgraphcontroller.h"

class QxrdScatterPlotGraphController : public QxrdDataObjectGraphController
{
  Q_OBJECT

public:
  QxrdScatterPlotGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
};

#endif // QXRDSCATTERPLOTGRAPHCONTROLLER_H
