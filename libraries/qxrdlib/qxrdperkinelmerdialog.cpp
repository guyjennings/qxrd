#include "qxrdperkinelmerdialog.h"

QxrdPerkinElmerDialog::QxrdPerkinElmerDialog(
    QxrdPerkinElmerSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdPerkinElmerDialog::accept()
{
  QxrdDetectorDialog::accept();
}
