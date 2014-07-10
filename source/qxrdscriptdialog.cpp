#include "qxrddebug.h"
#include "qxrdscriptdialog.h"
#include "ui_qxrdscriptdialog.h"
#include "qxrdexperiment.h"

QxrdScriptDialog::QxrdScriptDialog(QxrdScriptDialogSettingsWPtr settings, QxrdExperimentWPtr experiment, QWidget *parent) :
  QDockWidget(parent),
  m_ScriptDialogSettings(settings),
  m_Experiment(experiment),
  m_Highlighter(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptDialog::QxrdScriptDialog(%p)\n", this);
  }

  setupUi(this);

  m_Highlighter = new QxrdHighlighter(m_DefaultScript->document());

  QxrdExperimentPtr expt(experiment);

  if (expt) {
    expt->prop_DefaultScript()->linkTo(m_DefaultScript);
  }
}

QxrdScriptDialog::~QxrdScriptDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptDialog::~QxrdScriptDialog(%p)\n", this);
  }
}
