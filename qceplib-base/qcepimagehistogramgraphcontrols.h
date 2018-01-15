#ifndef QCEPIMAGEHISTOGRAMGRAPHCONTROLS_H
#define QCEPIMAGEHISTOGRAMGRAPHCONTROLS_H

#include "qceplib_global.h"
#include "qcepgraphcontrolswidget.h"
#include "ui_qcepimagehistogramgraphcontrols.h"

class QCEP_EXPORT QcepImageHistogramGraphControls : public QcepGraphControlsWidget, public Ui::QcepImageHistogramGraphControls
{
  Q_OBJECT

public:
  explicit QcepImageHistogramGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);
  ~QcepImageHistogramGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QCEPIMAGEHISTOGRAMGRAPHCONTROLS_H