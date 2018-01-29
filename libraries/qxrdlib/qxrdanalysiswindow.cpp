#include "qxrdanalysiswindow.h"

QxrdAnalysisWindow::QxrdAnalysisWindow(QString name, QxrdAppCommonWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdAnalysisWindow::~QxrdAnalysisWindow()
{
}

void QxrdAnalysisWindow::changeEvent(QEvent *e)
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
