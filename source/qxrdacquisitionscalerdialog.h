#ifndef QXRDACQUISITIONSCALERDIALOG_H
#define QXRDACQUISITIONSCALERDIALOG_H

#include "qcepmacros.h"
#include "ui_qxrdacquisitionscalerdialog.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include <QDockWidget>

class QxrdAcquisitionScalerDialog : public QDockWidget, public Ui::QxrdAcquisitionScalerDialog
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionScalerDialog(QxrdAcquisitionWPtr acq,
                                       QWidget *parent=0);
  ~QxrdAcquisitionScalerDialog();

private:
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdAcquisitionScalerModelWPtr  m_Model;
};

#endif // QXRDACQUISITIONSCALERDIALOG_H
