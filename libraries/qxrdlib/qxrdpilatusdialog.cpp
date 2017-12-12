#include "qxrdpilatusdialog.h"

QxrdPilatusDialog::QxrdPilatusDialog(
    QxrdPilatusSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdPilatusDialog::accept()
{
  QxrdDetectorDialog::accept();
}
