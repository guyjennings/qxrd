#ifndef QXRDACQUISITIONTRIGGERDIALOG_H
#define QXRDACQUISITIONTRIGGERDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrdacquisitiontrigger.h"
#include "ui_qxrdacquisitiontriggerdialog.h"

class QxrdAcquisitionTriggerDialog : public QDockWidget, public Ui::QxrdAcquisitionTriggerDialog
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionTriggerDialog(QWidget *parent, QxrdAcquisitionWPtr acq);
  ~QxrdAcquisitionTriggerDialog();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdAcquisitionWPtr        m_Acquisition;
  QxrdAcquisitionTriggerWPtr m_AcquisitionTrigger;
};

#endif // QXRDACQUISITIONTRIGGERDIALOG_H

class QxrdAcquisitionTriggerDialog;
