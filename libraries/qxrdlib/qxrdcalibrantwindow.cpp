#include "qxrdcalibrantwindow.h"

QxrdCalibrantWindow::QxrdCalibrantWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt)
{
  setupUi(this);

  setupMenus();
}

QxrdCalibrantWindow::~QxrdCalibrantWindow()
{
}

void QxrdCalibrantWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
