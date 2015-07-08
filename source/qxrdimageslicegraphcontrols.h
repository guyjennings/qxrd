#ifndef QXRDIMAGESLICEGRAPHCONTROLS_H
#define QXRDIMAGESLICEGRAPHCONTROLS_H

#include "qxrdgraphcontrolswidget.h"
#include "ui_qxrdimageslicegraphcontrols.h"

class QxrdImageSliceGraphControls : public QxrdGraphControlsWidget, public Ui::QxrdImageSliceGraphControls
{
  Q_OBJECT

public:
  explicit QxrdImageSliceGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QxrdImageSliceGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QXRDIMAGESLICEGRAPHCONTROLS_H
