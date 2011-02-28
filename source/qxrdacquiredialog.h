#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include <QWidget>
#include "ui_qxrdacquiredialog.h"

class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;

class QxrdAcquireDialog : public QDockWidget, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent = 0);
  ~QxrdAcquireDialog();

  void onAcquisitionInit();

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

protected:
  void changeEvent(QEvent *e);

private slots:
  void acquireStarted(int isDark);
  void acquireComplete(int isDark);

private:
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
  QxrdDataProcessor *m_DataProcessor;
};

#endif // QXRDACQUIREDIALOG_H
