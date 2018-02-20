#include "qxrddexeladialog.h"
#include "qxrddexelasettings.h"

QxrdDexelaDialog::QxrdDexelaDialog(
    QxrdDexelaSettingsPtr set, QWidget *parent) :
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
    m_Settings->prop_DetectorIndex()    -> copyTo(m_DetectorIndex);
    m_Settings->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
  }
}

void QxrdDexelaDialog::accept()
{
  if (m_Settings) {
    m_Settings->prop_DetectorName()     -> copyFrom(m_DetectorName);
    m_Settings->prop_DetectorIndex()    -> copyFrom(m_DetectorIndex);
    m_Settings->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
  }

  QxrdDetectorDialog::accept();
}
