#include "qxrdareadetectordialog.h"
#include "qxrdareadetectorsettings.h"

QxrdAreaDetectorDialog::QxrdAreaDetectorDialog(QxrdAreaDetectorSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdAreaDetectorSettingsPtr aset(m_Settings);

  if (aset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(aset->get_DetectorTypeName())
                   .arg(aset->get_DetectorNumber())
                   .arg(aset->get_DetectorName()));

    aset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    aset->prop_DetectorName()     -> copyTo(m_DetectorName);
    aset->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    aset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
    aset->prop_HardwareSync()     -> copyTo(m_HardwareSync);
  }
}

void QxrdAreaDetectorDialog::accept()
{
  QxrdAreaDetectorSettingsPtr aset(m_Settings);

  if (aset) {
    aset->prop_DetectorName()     -> copyFrom(m_DetectorName);
    aset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
    aset->prop_HardwareSync()     -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
