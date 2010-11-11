#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include <QWidget>
#include "qxrdforwardtypes.h"
#include "ui_qxrdacquiredialog.h"

class QxrdAcquireDialog : public QWidget, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdWindowPtr win, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc, QWidget *parent = 0);
  ~QxrdAcquireDialog();

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdWindowPtr        m_Window;
  QxrdAcquisitionPtr   m_Acquisition;
  QxrdDataProcessorPtr m_DataProcessor;
};

#endif // QXRDACQUIREDIALOG_H
