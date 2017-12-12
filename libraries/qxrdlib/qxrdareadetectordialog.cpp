#include "qxrdareadetectordialog.h"
#include "qxrdareadetectorsettings.h"

QxrdAreaDetectorDialog::QxrdAreaDetectorDialog(
    QxrdAreaDetectorSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);
}

void QxrdAreaDetectorDialog::accept()
{
  QxrdDetectorDialog::accept();
}
