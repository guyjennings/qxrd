#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include "qxrdacquiredialogbase.h"
#include "qxrdacquisition.h"
#include "ui_qxrdacquiredialog.h"

class QxrdAcquireDialog : public QxrdAcquireDialogBase, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdExperimentPtr doc,
                    QxrdWindow *win,
                    QxrdAcquisition *acq,
                    QxrdDataProcessorPtr proc,
                    QWidget *parent);
  ~QxrdAcquireDialog();

  void onAcquisitionInit();

  void acquisitionReady();

protected:
  void changeEvent(QEvent *e);
  void setupAcquireMenu(QMenu *menu);

public slots:
  void doAcquire();
  void doAcquireDark();
  void doCancel();
  void acquireStarted();
  void acquireComplete();
};

#endif // QXRDACQUIREDIALOG_H
