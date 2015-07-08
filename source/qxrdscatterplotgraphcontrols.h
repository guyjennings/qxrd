#ifndef QXRDSCATTERPLOTGRAPHCONTROLS_H
#define QXRDSCATTERPLOTGRAPHCONTROLS_H

#include "qxrdgraphcontrolswidget.h"
#include "ui_qxrdscatterplotgraphcontrols.h"

class QxrdScatterPlotGraphControls : public QxrdGraphControlsWidget, public Ui::QxrdScatterPlotGraphControls
{
  Q_OBJECT

public:
  explicit QxrdScatterPlotGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QxrdScatterPlotGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::QxrdScatterPlotGraphControls *ui;
};

#endif // QXRDSCATTERPLOTGRAPHCONTROLS_H
