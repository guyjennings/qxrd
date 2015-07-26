#ifndef QCEPSCATTERPLOTGRAPHCONTROLS_H
#define QCEPSCATTERPLOTGRAPHCONTROLS_H

#include "qcepgraphcontrolswidget.h"
#include "ui_qcepscatterplotgraphcontrols.h"

class QcepScatterPlotGraphControls : public QcepGraphControlsWidget, public Ui::QcepScatterPlotGraphControls
{
  Q_OBJECT

public:
  explicit QcepScatterPlotGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);
  ~QcepScatterPlotGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QcepScatterPlotGraphControls *ui;
};

#endif // QCEPSCATTERPLOTGRAPHCONTROLS_H
