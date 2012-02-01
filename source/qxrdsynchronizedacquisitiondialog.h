#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "ui_qxrdsynchronizedacquisitiondialog.h"

class QxrdSynchronizedAcquisitionDialog : public QDockWidget, public Ui::QxrdSynchronizedAcquisitionDialog
{
  Q_OBJECT

public:
  explicit QxrdSynchronizedAcquisitionDialog(QWidget *parent, QxrdAcquisitionPtr acq);
  ~QxrdSynchronizedAcquisitionDialog();

protected:
  void changeEvent(QEvent *e);

private slots:
  void waveformChanged();

private:
  QxrdAcquisitionPtr           m_Acquisition;
  QxrdSynchronizedAcquisition *m_SynchronizedAcquisition;
};

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOG_H
