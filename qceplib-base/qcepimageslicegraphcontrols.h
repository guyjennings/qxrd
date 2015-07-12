#ifndef QCEPIMAGESLICEGRAPHCONTROLS_H
#define QCEPIMAGESLICEGRAPHCONTROLS_H

#include "qcepgraphcontrolswidget.h"
#include "ui_qcepimageslicegraphcontrols.h"

class QcepImageSliceGraphControls : public QcepGraphControlsWidget, public Ui::QcepImageSliceGraphControls
{
  Q_OBJECT

public:
  explicit QcepImageSliceGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QcepImageSliceGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QCEPIMAGESLICEGRAPHCONTROLS_H
