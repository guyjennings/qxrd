#ifndef QXRDACQUIREDIALOG_H
#define QXRDACQUIREDIALOG_H

#include "qcepmacros.h"
#include "qxrdacquiredialogbase.h"
#include "ui_qxrdacquiredialog.h"

class QxrdAcquireDialog : public QxrdAcquireDialogBase, public Ui::QxrdAcquireDialog {
  Q_OBJECT
public:
  QxrdAcquireDialog(QxrdExperimentWPtr doc,
                    QxrdWindowWPtr win,
                    QxrdAcquisitionWPtr acq,
                    QxrdDataProcessorWPtr proc,
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
