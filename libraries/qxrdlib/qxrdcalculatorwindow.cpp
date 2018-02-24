#include "qxrdcalculatorwindow.h"
#include <QThread>

QxrdCalculatorWindow::QxrdCalculatorWindow(QxrdCalculatorWindowSettingsWPtr set, QString name, QxrdAppCommonWPtr app, QxrdExperimentWPtr expt, QxrdAcqCommonWPtr acqw, QxrdProcessorWPtr procw) :
  inherited(name, app, expt, acqw, procw),
  m_CalculatorWindowSettings(set)
{
}

void QxrdCalculatorWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  setupUi(this);

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
