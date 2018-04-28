#include "qxrdpilatusdialog.h"
#include "qxrdpilatussettings.h"

QxrdPilatusDialog::QxrdPilatusDialog(QxrdPilatusSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdPilatusSettingsPtr pset(m_Settings);

  if (pset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(pset->get_DetectorTypeName())
                   .arg(pset->get_DetectorNumber())
                   .arg(pset->get_DetectorName()));

    pset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    pset->prop_DetectorName()     -> copyTo(m_DetectorName);
    pset->prop_DetectorTypeName() -> copyTo(m_DetectorType);

    pset->prop_PilatusHost()             -> copyTo(m_PilatusHost);
    pset->prop_PilatusUser()             -> copyTo(m_PilatusUser);
    pset->prop_PilatusSSH()              -> copyTo(m_PilatusSSH);
    pset->prop_PilatusSCP ()             -> copyTo(m_PilatusSCP);
    pset->prop_PilatusDataDirectory()    -> copyTo(m_PilatusDataDirectory);
    pset->prop_ReadFilesLocally ()       -> copyTo(m_ReadFilesLocally);
    pset->prop_DeleteFilesAfterReading() -> copyTo(m_DeleteFilesAfterReading);
    pset->prop_PilatusExtension()        -> copyTo(m_PilatusExtension);
    pset->prop_ExposureMode()            -> copyTo(m_ExposureMode);
    pset->prop_EnableFrequency()         -> copyTo(m_EnableFrequency);
    pset->prop_ExposureFactor()          -> copyTo(m_ExposureFactor);
    pset->prop_HardwareSync()            -> copyTo(m_HardwareSync);
  }
}

void QxrdPilatusDialog::accept()
{
  QxrdPilatusSettingsPtr pset(m_Settings);

  if (pset) {
    pset->prop_DetectorName()            -> copyFrom(m_DetectorName);

    pset->prop_PilatusHost()             -> copyFrom(m_PilatusHost);
    pset->prop_PilatusUser()             -> copyFrom(m_PilatusUser);
    pset->prop_PilatusSSH()              -> copyFrom(m_PilatusSSH);
    pset->prop_PilatusSCP ()             -> copyFrom(m_PilatusSCP);
    pset->prop_PilatusDataDirectory()    -> copyFrom(m_PilatusDataDirectory);
    pset->prop_ReadFilesLocally ()       -> copyFrom(m_ReadFilesLocally);
    pset->prop_DeleteFilesAfterReading() -> copyFrom(m_DeleteFilesAfterReading);
    pset->prop_PilatusExtension()        -> copyFrom(m_PilatusExtension);
    pset->prop_ExposureMode()            -> copyFrom(m_ExposureMode);
    pset->prop_EnableFrequency()         -> copyFrom(m_EnableFrequency);
    pset->prop_ExposureFactor()          -> copyFrom(m_ExposureFactor);
    pset->prop_HardwareSync()            -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
