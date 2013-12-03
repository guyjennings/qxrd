#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOG_H

#include <QDockWidget>
#include "ui_qxrdsynchronizedacquisitiondialog.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings.h"

class QxrdSynchronizedAcquisitionDialog : public QDockWidget, public Ui::QxrdSynchronizedAcquisitionDialog
{
  Q_OBJECT

public:
  explicit QxrdSynchronizedAcquisitionDialog(
      QxrdSynchronizedAcquisitionDialogSettingsWPtr settings,
      QWidget *parent,
      QxrdAcquisitionWPtr acq);

  ~QxrdSynchronizedAcquisitionDialog();

protected:
  void changeEvent(QEvent *e);

private slots:
  void deviceChanged();
  void waveformChanged();

private:
  QxrdSynchronizedAcquisitionDialogSettingsWPtr m_DialogSettings;
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
};

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
