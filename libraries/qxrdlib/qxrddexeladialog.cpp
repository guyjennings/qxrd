#include "qxrddexeladialog.h"
#include "qxrddexelasettings.h"

QxrdDexelaDialog::QxrdDexelaDialog(QxrdDexelaSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdDexelaSettingsPtr dset(m_Settings);

  if (dset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(dset->get_DetectorTypeName())
                   .arg(dset->get_DetectorNumber())
                   .arg(dset->get_DetectorName()));

    dset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    dset->prop_DetectorName()     -> copyTo(m_DetectorName);
    dset->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    dset->prop_DetectorIndex()    -> copyTo(m_DetectorIndex);
    dset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
    dset->prop_HardwareSync()     -> copyTo(m_HardwareSync);
  }
}

void QxrdDexelaDialog::accept()
{
  QxrdDexelaSettingsPtr dset(m_Settings);

  if (dset) {
    dset->prop_DetectorName()     -> copyFrom(m_DetectorName);
    dset->prop_DetectorIndex()    -> copyFrom(m_DetectorIndex);
    dset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
    dset->prop_HardwareSync()     -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
