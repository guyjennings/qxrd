#include "qxrduserscriptdialog.h"
#include "ui_qxrduserscriptdialog.h"
#include <stdio.h>

QxrdUserScriptDialog::QxrdUserScriptDialog(QString title, QxrdExperimentWPtr experiment, QString editable, QString def, QWidget *parent) :
  QDialog(parent),
  m_Experiment(experiment),
  ui(new Ui::QxrdUserScriptDialog)
{
  ui->setupUi(this);

  m_Highlighter = new QxrdHighlighter(ui->m_ScriptEditor->document());
  m_Default = def;

  setWindowTitle(title);

  ui->m_ScriptEditor->setText(editable);
  ui->m_ScriptEditor->setExperiment(m_Experiment);

  connect(ui->m_ButtonBar, SIGNAL(clicked(QAbstractButton *)), this, SLOT(onButtonClicked(QAbstractButton*)));
}

QxrdUserScriptDialog::~QxrdUserScriptDialog()
{
  delete ui;
}

void QxrdUserScriptDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

QString QxrdUserScriptDialog::userScript()
{
  return ui->m_ScriptEditor->toPlainText();
}

void QxrdUserScriptDialog::onButtonClicked(QAbstractButton* button)
{
  QDialogButtonBox::ButtonRole role = ui->m_ButtonBar->buttonRole(button);

//  printf("Button %p, role %d\n", button, role);

  if (role == QDialogButtonBox::ResetRole) {
    ui->m_ScriptEditor->setText(m_Default);
  }
}
