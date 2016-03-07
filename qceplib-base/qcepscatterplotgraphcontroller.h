#ifndef QCEPSCATTERPLOTGRAPHCONTROLLER_H
#define QCEPSCATTERPLOTGRAPHCONTROLLER_H

#include "qcepdataobjectgraphcontroller.h"
#include "qcepscatterplotgraphmodel-ptr.h"
#include "qcepdatacolumn-ptr.h"

class QcepScatterPlotGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepScatterPlotGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepScatterPlotGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
  void onPlotDataChanged(const QModelIndex &topLeft,
                         const QModelIndex &bottomRight);

private:
  void appendCurve(QString name, int axis, int curveNumber, const double *x, const double *y, int npts);
  void appendCurve(QString name, int axis, int curveNumber, QcepDataColumnPtr x, QcepDataColumnPtr y);

private:
  QcepScatterPlotGraphModelWPtr m_Model;
};

#endif // QCEPSCATTERPLOTGRAPHCONTROLLER_H
