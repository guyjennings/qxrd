#ifndef QXRDIMAGEDATAGRAPHCONTROLS_H
#define QXRDIMAGEDATAGRAPHCONTROLS_H

#include "qxrdgraphcontrolswidget.h"
#include "ui_qxrdimagedatagraphcontrols.h"
#include "qcepimagedata.h"

class QxrdImageDataGraphControls : public QxrdGraphControlsWidget, public Ui::QxrdImageDataGraphControls
{
  Q_OBJECT

public:
  explicit QxrdImageDataGraphControls(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);
  ~QxrdImageDataGraphControls();

protected:
  void changeEvent(QEvent *e);

private:
  QcepImageDataBase          *m_Image;
};

#endif // QXRDIMAGEDATAGRAPHCONTROLS_H
