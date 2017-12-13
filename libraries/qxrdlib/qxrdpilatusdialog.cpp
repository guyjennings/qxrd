#include "qxrdpilatusdialog.h"
#include "qxrdpilatussettings.h"

QxrdPilatusDialog::QxrdPilatusDialog(
    QxrdPilatusSettingsPtr set, QWidget *parent) :
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
  }
}

void QxrdPilatusDialog::accept()
{
  if (m_Settings) {
    m_Settings->prop_DetectorName()     -> copyFrom(m_DetectorName);
  }

  QxrdDetectorDialog::accept();
}
