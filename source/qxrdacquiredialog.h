#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include "qxrdacquiredialogbase.h"
#include "ui_qxrdacquiredialog.h"

class QxrdAcquireDialog : public QxrdAcquireDialogBase, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent = 0);
  ~QxrdAcquireDialog();

  void onAcquisitionInit();

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

protected:
  void changeEvent(QEvent *e);
  void setupAcquireMenu(QMenu *menu);

private slots:
  void acquireStarted();
  void acquireComplete();
};

#endif // QXRDACQUIREDIALOG_H
