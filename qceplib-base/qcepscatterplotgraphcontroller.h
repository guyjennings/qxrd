#ifndef QCEPSCATTERPLOTGRAPHCONTROLLER_H
#define QCEPSCATTERPLOTGRAPHCONTROLLER_H

#include "qcepdataobjectgraphcontroller.h"

class QcepScatterPlotGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepScatterPlotGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepScatterPlotGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QCEPSCATTERPLOTGRAPHCONTROLLER_H
