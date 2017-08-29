#include "qxrddebug.h"
#include "qxrdscriptdialog.h"
#include "ui_qxrdscriptdialog.h"
#include "qxrdexperiment.h"
#include <stdio.h>

QxrdScriptDialog::QxrdScriptDialog(QxrdScriptDialogSettingsWPtr settings, QxrdExperimentWPtr experiment, QWidget *parent) :
  QDockWidget(parent),
  m_ScriptDialogSettings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptDialog::QxrdScriptDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdExperimentPtr expt(experiment);

  if (expt) {
    m_DefaultScript->setExperiment(expt);

    expt->prop_DefaultScript()->linkTo(m_DefaultScript);
  }
}

QxrdScriptDialog::~QxrdScriptDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdScriptDialog::~QxrdScriptDialog(%p)\n", this);
  }
}
