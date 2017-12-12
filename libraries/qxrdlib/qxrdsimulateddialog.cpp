#include "qxrdsimulateddialog.h"

QxrdSimulatedDialog::QxrdSimulatedDialog(
    QxrdSimulatedSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdSimulatedDialog::accept()
{
  QxrdDetectorDialog::accept();
}
