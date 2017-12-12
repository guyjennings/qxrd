#include "qxrddexeladialog.h"

QxrdDexelaDialog::QxrdDexelaDialog(
    QxrdDexelaSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdDexelaDialog::accept()
{
  QxrdDetectorDialog::accept();
}
