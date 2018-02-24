#include "qxrdanalysiswindow.h"
#include <QThread>

QxrdAnalysisWindow::QxrdAnalysisWindow(QString name) :
  inherited(name)
{
}

void QxrdAnalysisWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdAnalysisWindow::~QxrdAnalysisWindow()
{
}

void QxrdAnalysisWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
