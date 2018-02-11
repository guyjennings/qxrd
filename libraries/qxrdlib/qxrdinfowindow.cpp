#include "qxrdinfowindow.h"

QxrdInfoWindow::QxrdInfoWindow(QString name,
                               QxrdAppCommonWPtr app,
                               QxrdExperimentWPtr expt,
                               QxrdAcqCommonWPtr acqw,
                               QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdInfoWindow::~QxrdInfoWindow()
{
}

void QxrdInfoWindow::changeEvent(QEvent *e)
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
