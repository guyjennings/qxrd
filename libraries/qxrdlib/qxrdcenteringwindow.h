#ifndef QXRDCENTERINGWINDOW_H
#define QXRDCENTERINGWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcenteringwindow.h"

class QXRD_EXPORT QxrdCenteringWindow : public QxrdMainWindow, public Ui::QxrdCenteringWindow
{
  Q_OBJECT

public:
  explicit QxrdCenteringWindow(QString name,
                               QxrdApplicationWPtr app,
                               QxrdExperimentWPtr expt);
  ~QxrdCenteringWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDCENTERINGWINDOW_H
