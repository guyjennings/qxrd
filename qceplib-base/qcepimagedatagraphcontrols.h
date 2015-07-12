#ifndef QCEPIMAGEDATAGRAPHCONTROLS_H
#define QCEPIMAGEDATAGRAPHCONTROLS_H

#include "qcepgraphcontrolswidget.h"
#include "ui_qcepimagedatagraphcontrols.h"
#include "qcepimagedata.h"

class QcepImageDataGraphControls : public QcepGraphControlsWidget, public Ui::QcepImageDataGraphControls
{
  Q_OBJECT

public:
  explicit QcepImageDataGraphControls(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QcepImageDataGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
  QcepImageDataBase          *m_Image;
};

#endif // QCEPIMAGEDATAGRAPHCONTROLS_H
