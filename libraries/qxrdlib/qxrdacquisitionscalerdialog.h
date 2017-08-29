#ifndef QXRDACQUISITIONSCALERDIALOG_H
#define QXRDACQUISITIONSCALERDIALOG_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "ui_qxrdacquisitionscalerdialog.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include <QDockWidget>

class QXRD_EXPORT QxrdAcquisitionScalerDialog : public QDockWidget, public Ui::QxrdAcquisitionScalerDialog
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionScalerDialog(QxrdAcquisitionWPtr acq,
                                       QWidget *parent=0);
  ~QxrdAcquisitionScalerDialog();

private:
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdAcquisitionScalerModelPtr   m_Model;
};

#endif // QXRDACQUISITIONSCALERDIALOG_H
