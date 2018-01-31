#ifndef QXRDCORRECTIONDIALOG_H
#define QXRDCORRECTIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "qxrdacquisition-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "ui_qxrdcorrectiondialog.h"

class QXRD_EXPORT QxrdCorrectionDialog : public QDialog, public Ui::QxrdCorrectionDialog
{
  Q_OBJECT

public:
  explicit QxrdCorrectionDialog(QWidget *parent, QxrdAcquisitionWPtr acqp, QxrdProcessorWPtr procp);
  ~QxrdCorrectionDialog();

public slots:
  void doEditPreferences();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdProcessorWPtr     m_Processor;
};

#endif // QXRDCORRECTIONDIALOG_H
