#ifndef QXRDACQUIREFILEWATCHERDIALOG_H
#define QXRDACQUIREFILEWATCHERDIALOG_H

#include "qxrdacquiredialogbase.h"
#include "ui_qxrdacquirefilewatcherdialog.h"

class QxrdAcquireFileWatcherDialog : public QxrdAcquireDialogBase, public Ui::QxrdAcquireFileWatcherDialog
{
  Q_OBJECT

public:
  explicit QxrdAcquireFileWatcherDialog(QxrdExperiment *doc,
                                        QxrdWindow *win,
                                        QxrdAcquisition *acq,
                                        QxrdDataProcessor *proc,
                                        QWidget *parent = 0);
  ~QxrdAcquireFileWatcherDialog();

protected:
  void changeEvent(QEvent *e);

  void setupAcquireMenu(QMenu *menu);
  void onAcquisitionInit();
  void acquisitionReady();
};

#endif // QXRDACQUIREFILEWATCHERDIALOG_H
