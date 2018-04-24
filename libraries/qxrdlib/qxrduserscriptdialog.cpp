#include "qxrduserscriptdialog.h"
#include "ui_qxrduserscriptdialog.h"
#include <stdio.h>
#include "qcepmacros.h"

QxrdUserScriptDialog::QxrdUserScriptDialog(QString title, QxrdExperimentWPtr experiment, QString editable, QString def, QWidget *parent) :
  QDialog(parent),
  m_Experiment(experiment)
{
  setupUi(this);

  m_Default = def;

  setWindowTitle(title);

  m_ScriptEditor->setText(editable);
  m_ScriptEditor->setExperiment(m_Experiment);

  CONNECT_CHECK(connect(m_ButtonBar, &QDialogButtonBox::clicked, this, &QxrdUserScriptDialog::onButtonClicked));
}

QxrdUserScriptDialog::~QxrdUserScriptDialog()
{
}

void QxrdUserScriptDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

QString QxrdUserScriptDialog::userScript()
{
  return m_ScriptEditor->toPlainText();
}

void QxrdUserScriptDialog::onButtonClicked(QAbstractButton* button)
{
  QDialogButtonBox::ButtonRole role = m_ButtonBar->buttonRole(button);

//  printf("Button %p, role %d\n", button, role);

  if (role == QDialogButtonBox::ResetRole) {
    m_ScriptEditor->setText(m_Default);
  }
}
