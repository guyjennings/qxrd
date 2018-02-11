#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdsynchronizedacquisitiondialog.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisitionDialog : public QDockWidget, public Ui::QxrdSynchronizedAcquisitionDialog
{
  Q_OBJECT

public:
  explicit QxrdSynchronizedAcquisitionDialog(QxrdSynchronizedAcquisitionDialogSettingsWPtr settings,
                                             QWidget *parent,
                                             QxrdAcqCommonWPtr acq);

  ~QxrdSynchronizedAcquisitionDialog();

protected:
  void changeEvent(QEvent *e);

private slots:
  void deviceChanged();
  void waveformChanged();

private:
  QxrdSynchronizedAcquisitionDialogSettingsWPtr m_DialogSettings;
  QxrdAcqCommonWPtr                             m_Acquisition;
  QxrdSynchronizedAcquisitionWPtr               m_SynchronizedAcquisition;
};

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
