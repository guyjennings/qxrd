#include "qxrdsimulateddialog.h"
#include "qxrdsimulatedsettings.h"

QxrdSimulatedDialog::QxrdSimulatedDialog(QxrdSimulatedSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdSimulatedSettingsPtr simset(m_Settings);

  if (simset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(simset->get_DetectorTypeName())
                   .arg(simset->get_DetectorNumber())
                   .arg(simset->get_DetectorName()));

    simset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    simset->prop_DetectorName()     -> copyTo(m_DetectorName);
    simset->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    simset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
  }
}

void QxrdSimulatedDialog::accept()
{
  QxrdSimulatedSettingsPtr simset(m_Settings);

  if (simset) {
    simset->prop_DetectorName()     -> copyFrom(m_DetectorName);
    simset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
  }

  QxrdDetectorDialog::accept();
}
