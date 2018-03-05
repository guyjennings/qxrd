#include "qxrdareadetectordialog.h"
#include "qxrdareadetectorsettings.h"

QxrdAreaDetectorDialog::QxrdAreaDetectorDialog(
    QxrdAreaDetectorSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  if (m_Settings) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(m_Settings->get_DetectorTypeName())
                   .arg(m_Settings->get_DetectorNumber())
                   .arg(m_Settings->get_DetectorName()));

    m_Settings->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    m_Settings->prop_DetectorName()     -> copyTo(m_DetectorName);
    m_Settings->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    m_Settings->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
    m_Settings->prop_HardwareSync()     -> copyTo(m_HardwareSync);
  }
}

void QxrdAreaDetectorDialog::accept()
{
  if (m_Settings) {
    m_Settings->prop_DetectorName()     -> copyFrom(m_DetectorName);
    m_Settings->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
    m_Settings->prop_HardwareSync()     -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
