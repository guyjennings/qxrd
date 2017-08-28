#ifndef QCEPIMAGESLICEGRAPHCONTROLS_H
#define QCEPIMAGESLICEGRAPHCONTROLS_H

#include "qceplib_global.h"
#include "qcepgraphcontrolswidget.h"
#include "ui_qcepimageslicegraphcontrols.h"

class QCEP_EXPORT QcepImageSliceGraphControls : public QcepGraphControlsWidget, public Ui::QcepImageSliceGraphControls
{
  Q_OBJECT

public:
  explicit QcepImageSliceGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);
  ~QcepImageSliceGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
};

#endif // QCEPIMAGESLICEGRAPHCONTROLS_H
