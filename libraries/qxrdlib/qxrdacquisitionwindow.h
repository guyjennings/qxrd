#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdacquisitionwindow.h"

class QXRD_EXPORT QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAcquisitionWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionWindow(QString name,
                                 QxrdApplicationWPtr app,
                                 QxrdExperimentWPtr expt);
  ~QxrdAcquisitionWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDACQUISITIONWINDOW_H
