#ifndef QXRDIMAGEHISTOGRAMGRAPHCONTROLS_H
#define QXRDIMAGEHISTOGRAMGRAPHCONTROLS_H

#include "qxrdgraphcontrolswidget.h"
#include "ui_qxrdimagehistogramgraphcontrols.h"

class QxrdImageHistogramGraphControls : public QxrdGraphControlsWidget, public Ui::QxrdImageHistogramGraphControls
{
  Q_OBJECT

public:
  explicit QxrdImageHistogramGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QxrdImageHistogramGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDIMAGEHISTOGRAMGRAPHCONTROLS_H
