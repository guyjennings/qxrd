#include "qxrdareadetectordialog.h"
#include "qxrdareadetectorsettings.h"

QxrdAreaDetectorDialog::QxrdAreaDetectorDialog(
    QxrdAreaDetectorSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  if (m_Settings) {
    setWindowTitle(tr("Configure Area Detector %1: %2")
                   .arg(m_Settings->get_DetectorNumber())
                   .arg(m_Settings->get_DetectorName()));
  }
}

void QxrdAreaDetectorDialog::accept()
{
  QxrdDetectorDialog::accept();
}
