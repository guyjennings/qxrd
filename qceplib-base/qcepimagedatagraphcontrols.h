#ifndef QCEPIMAGEDATAGRAPHCONTROLS_H
#define QCEPIMAGEDATAGRAPHCONTROLS_H

#include "qceplib_global.h"
#include "qcepgraphcontrolswidget.h"
#include "ui_qcepimagedatagraphcontrols.h"
#include "qcepimagedata.h"

class QCEP_EXPORT QcepImageDataGraphControls : public QcepGraphControlsWidget, public Ui::QcepImageDataGraphControls
{
  Q_OBJECT

public:
  explicit QcepImageDataGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);
  ~QcepImageDataGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
  QcepImageDataBase          *m_Image;
};

#endif // QCEPIMAGEDATAGRAPHCONTROLS_H
