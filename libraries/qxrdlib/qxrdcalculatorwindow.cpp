#include "qxrdcalculatorwindow.h"
#include <QThread>

QxrdCalculatorWindow::QxrdCalculatorWindow(QString name) :
  inherited(name)
{
}

void QxrdCalculatorWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);
}

QxrdCalculatorWindow::~QxrdCalculatorWindow()
{
}

void QxrdCalculatorWindow::changeEvent(QEvent *e)
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
