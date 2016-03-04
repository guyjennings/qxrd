#ifndef QCEPSCATTERPLOTGRAPHCONTROLS_H
#define QCEPSCATTERPLOTGRAPHCONTROLS_H

#include "qcepgraphcontrolswidget.h"
#include "ui_qcepscatterplotgraphcontrols.h"
#include "qcepscatterplotgraphmodel-ptr.h"

class QcepScatterPlotGraphControls : public QcepGraphControlsWidget, public Ui::QcepScatterPlotGraphControls
{
  Q_OBJECT

public:
  explicit QcepScatterPlotGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);
  ~QcepScatterPlotGraphControls();

public slots:
//  void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void onClicked(const QModelIndex &index);

private:
  QcepScatterPlotGraphModelPtr m_Model;
};

#endif // QCEPSCATTERPLOTGRAPHCONTROLS_H
