#ifndef QXRDHELPWINDOW_H
#define QXRDHELPWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdhelpwindow.h"

class QXRD_EXPORT QxrdHelpWindow : public QxrdMainWindow, public Ui::QxrdHelpWindow
{
  Q_OBJECT

public:
  explicit QxrdHelpWindow(QString name,
                          QxrdApplicationWPtr app,
                          QxrdExperimentWPtr expt);
  ~QxrdHelpWindow();

protected:
  void changeEvent(QEvent *e);
};

#endif // QXRDHELPWINDOW_H