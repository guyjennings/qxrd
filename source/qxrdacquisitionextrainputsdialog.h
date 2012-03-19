#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOG_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"

class QxrdAcquisitionExtraInputsDialog : public QDockWidget, public Ui::QxrdAcquisitionExtraInputsDialog
{
  Q_OBJECT
  
public:
  explicit QxrdAcquisitionExtraInputsDialog(QWidget *parent, QxrdAcquisitionWPtr acq);
  ~QxrdAcquisitionExtraInputsDialog();

private:
  void setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch);

private:
  QxrdAcquisitionWPtr            m_Acquisition;
  QxrdAcquisitionExtraInputsWPtr m_AcquisitionExtraInputs;
};

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOG_H
